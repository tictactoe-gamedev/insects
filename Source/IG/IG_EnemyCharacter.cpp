// GPLv3

#include <algorithm>
#include "IG_EnemySpawner.h"
#include "IG_EnemyCharacter.h"
#include "IG_PlayerCharacter.h"
#include "IG_EnemyHealthBar.h"
#include "IG_GameMode.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AIG_EnemyCharacter::AIG_EnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIG_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set up the health bar widget
	HealthBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HealthBarWidget);
	HealthBarWidgetInstance->AddToViewport();
	HealthBarWidgetInstance->SetDesiredSizeInViewport(FVector2d(100, 16));
}

// Called every frame
void AIG_EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If dead, decrement removal countdown
	if (CurrentHealth <= 0)
	{
		if (DeathRemovalTime > 0.f)
		{
			DeathRemovalTime -= DeltaTime;			
		}
		else
		{
			K2_DestroyActor();
		}
	}
	else
	{
		// Grab references. TODO move these to call members for resuability
		auto player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		auto player = player_controller->GetPawn();
	
		FVector2d screen_pos;

		// Check if we can project to the screen (only works if enemy is within camera view)
		if (player_controller->ProjectWorldLocationToScreen(GetActorLocation(), screen_pos))
		{
			// Adjust offset to place health bar above enemies' head
			screen_pos.X -= 50;
			screen_pos.Y -= 100;
			HealthBarWidgetInstance->SetPositionInViewport(screen_pos);
		
			// Make sure the health bar is visible 
			HealthBarWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		} else
		{
			// Hide the health bar if enemy is off-screen
			HealthBarWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}

		// Early return if player is dead
		auto GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode->GetGameOver())
		{
			return;
		}

		// Calculate path to the player. TODO probably can cache this for better performance
		UNavigationPath* path = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), player->GetActorLocation(), this);

		// If the path is valid
		if (path && path->IsValid())
		{
			// Configure settings for movement requset
			FAIMoveRequest req;
			req.SetAcceptanceRadius(ChaseStopDistance); // Apply chase stop distance var
			req.SetUsePathfinding(true);

			AAIController* ai = Cast<AAIController>(GetController());
			if (ai)
			{
				// Apple the movement request
				ai->RequestMove(req, path->GetPath());
			}
		}

		// Check if the enemy is within attack range of the player
		if (FVector::Distance(GetActorLocation(), player->GetActorLocation()) <= AttackRange)
		{
			if (CurrentAttackTime >= AttackTime)
			{
				// Attack the player
				Attack(Cast<AIG_PlayerCharacter>(player));
				CurrentAttackTime = 0;
			}
			else
			{
				// Wait for the attack cooldown timer
				CurrentAttackTime += DeltaTime;
			}
		}
	}
}

// Called to bind functionality to input
void AIG_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AIG_EnemyCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) {

	UE_LOG(LogTemp, Warning, TEXT("Took damage: %.2f"), Damage);

	// Grab initial health
	int initial_health = CurrentHealth;

	// Calculate the result of the damage to the enemy health
	CurrentHealth = std::clamp(CurrentHealth - static_cast<int>(Damage), 0, MaxHealth);

	// Make sure we can access the health bar object
	if (HealthBarWidgetInstance)
	{
		// Convert health to a percentage
		float bar_percent = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
		// Send percentage to the health bar
		Cast<UIG_EnemyHealthBar>(HealthBarWidgetInstance)->HealthBar->SetPercent(bar_percent); 
	}

	// If health is low enough
	if (CurrentHealth == 0) {
		// Call death function for cleanup
		Died();
	}

	return (CurrentHealth - initial_health);
}

void AIG_EnemyCharacter::Died() {
	// Remove the health bar from the actor
	HealthBarWidgetInstance->RemoveFromParent();

	// Null out the pointer just in case it still gets ref-counted
	HealthBarWidgetInstance = nullptr;

	// Ask the spawner to clean up its lists
	spawner->CleanupEnemy(this);

	// Disable physics & collision
	auto capsule = GetComponentByClass<UCapsuleComponent>();
	capsule->SetSimulatePhysics(false);
	capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Increment player score in gamemode
	auto GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->IncrementScore();

	// enemy will be removed at the end of the death timer
}

void AIG_EnemyCharacter::Attack(AIG_PlayerCharacter* player)
{
	// Make sure the player is valid
	if (player)
	{
		// Apply very simple generic damage
		FDamageEvent dev;
		player->TakeDamage(AttackDamage, dev, GetController(), this);
	}
}
