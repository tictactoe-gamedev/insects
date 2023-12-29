// GPLv3

#include "IG_EnemyCharacter.h"
#include "IG_EnemySpawner.h"
#include "IG_EnemyHealthBar.h"
#include "../IG_GameMode.h"
#include "../Player/IG_PlayerCharacter.h"

#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

#include <algorithm>

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

	// Cache often-used vars
	GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerPawn = PlayerController->GetPawn();
	PlayerCharacter = Cast<AIG_PlayerCharacter>(PlayerPawn);

	// Subscribe to player death event
	PlayerCharacter->OnPlayerDeathDelegate.AddDynamic(this, &AIG_EnemyCharacter::OnPlayerDeath);
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

		return;
	}

	// TODO: get the enemy health bar to work as a child of the enemy
	// Check if we can project to the screen (only works if enemy is within camera view)
	if (FVector2d ScreenPos; PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenPos))
	{
		// Adjust offset to place health bar above enemies' head
		ScreenPos.X -= 50;
		ScreenPos.Y -= 100;
		HealthBarWidgetInstance->SetPositionInViewport(ScreenPos);
	
		// Make sure the health bar is visible 
		HealthBarWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		// Hide the health bar if enemy is off-screen
		HealthBarWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
	
	// Refresh player location
	PlayerLocation = PlayerPawn->GetActorLocation();
	
	// Run nav updates
	UpdatePath();

	// Check if we are in range to attack
	if (InAttackRange())
	{
		// Check the attack cooldown timer
		if (CurrentAttackTime >= AttackTime)
		{
			// Apply very simple generic damage
			const FDamageEvent Dev;
			PlayerCharacter->TakeDamage(AttackDamage, Dev, GetController(), this);
			CurrentAttackTime = 0;
		}
		else
		{
			// Wait for the attack cooldown timer
			CurrentAttackTime += DeltaTime;
		}
	}
}

// Called to bind functionality to input
void AIG_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AIG_EnemyCharacter::TakeDamage(const float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) {

	UE_LOG(LogTemp, Warning, TEXT("Took damage: %.2f"), Damage);

	// Grab initial health
	const int InitialHealth = CurrentHealth;

	// Calculate the result of the damage to the enemy health
	CurrentHealth = std::clamp(CurrentHealth - static_cast<int>(Damage), 0, MaxHealth);

	// Make sure we can access the health bar object
	if (HealthBarWidgetInstance)
	{
		// Convert health to a percentage
		const float BarPercent = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
		// Send percentage to the health bar
		Cast<UIG_EnemyHealthBar>(HealthBarWidgetInstance)->HealthBar->SetPercent(BarPercent); 
	}

	// If health is low enough
	if (CurrentHealth == 0) {
		// Call death function for cleanup
		Died();
	}

	// Return the difference (amount of damage)
	return (CurrentHealth - InitialHealth);
}

void AIG_EnemyCharacter::Died() {
	// Remove the health bar from the actor
	HealthBarWidgetInstance->RemoveFromParent();

	// Null out the pointer just in case it still gets ref-counted
	HealthBarWidgetInstance = nullptr;

	// Ask the spawner to clean up its lists
	ParentSpawner->CleanupEnemy(this);

	// Disable physics & collision
	const auto Capsule = GetComponentByClass<UCapsuleComponent>();
	Capsule->SetSimulatePhysics(false);
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Increment player score in gamemode
	GameMode->IncrementScore();

	// enemy will be removed at the end of the death timer
}

void AIG_EnemyCharacter::UpdatePath()
{
	// Calculate path to the player. TODO probably can cache this for better performance
	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
		GetWorld(),
		GetActorLocation(),
		PlayerLocation,
		this
	);

	// If the path is valid
	if (Path && Path->IsValid())
	{
		// Configure settings for movement request
		FAIMoveRequest Req;
		Req.SetAcceptanceRadius(ChaseStopDistance); // Apply chase stop distance var
		Req.SetUsePathfinding(true);

		AAIController* AiController = Cast<AAIController>(GetController());
		if (AiController)
		{
			// Apple the movement request
			AiController->RequestMove(Req, Path->GetPath());
		}
	}
}
