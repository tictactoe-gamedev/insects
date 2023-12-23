// GPLv3



#include "IG_PlayerCharacter.h"
#include "IG_GameMode.h"
#include "IG_PlayerHealthBar.h"
#include "IG_PlayerHud.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AIG_PlayerCharacter::AIG_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIG_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIG_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AIG_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

AIG_EnemyCharacter* AIG_PlayerCharacter::DoHitDetection() {

	// Grab start and end locations fom the mesh sockets
    FVector start_location = GetMesh()->GetSocketLocation("Start");
    FVector end_location = GetMesh()->GetSocketLocation("End");

	// Generate array of object types to hit
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Reserve(1);
    ObjectTypes.Emplace(ECollisionChannel::ECC_Pawn);

	// Fire the collision trace
    FHitResult HitResult = FHitResult(ForceInit);
    bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
            GetWorld(),
            start_location,
            end_location,
            20.f,
            ObjectTypes,
            false,
            ActorsToIgnore,		// Our dynamic ignore list
            EDrawDebugTrace::Type::ForDuration,
            HitResult,
            true,
            FLinearColor(255,0,0,255),
            FLinearColor(0,255,0,255),
            1.f
            );

    if (hit)
    {
    	// Get the actor that was hit
    	auto hit_actor = HitResult.GetActor();

    	// Check if the actor is already in the list
    	if (ActorsToIgnore.Find(hit_actor) == INDEX_NONE)
    	{
    		// If not, add it to the list for next time
    		ActorsToIgnore.AddUnique(HitResult.GetActor());

    		// Get the enemy character and log name
    		AIG_EnemyCharacter* enemy = Cast<AIG_EnemyCharacter>(HitResult.GetActor());
    		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(HitResult.GetActor()->GetName()));

			// Damage is applied in the BP currently
    		
    		return enemy;
    	}
    }

	// Return nullptr it we hit nothing
    return nullptr;
}

void AIG_PlayerCharacter::ClearHitDetection() {
	// Reset the ignored list
    ActorsToIgnore.Empty();
	// Add self always
    ActorsToIgnore.Push(this);
}

float AIG_PlayerCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) {

	UE_LOG(LogTemp, Warning, TEXT("Player took %.2f damage"), Damage);
	
	// Grab initial health
	int initial_health = CurrentHealth;

	// Calculate the result of the damage to the enemy health
	CurrentHealth = std::clamp(CurrentHealth - static_cast<int>(Damage), 0, MaxHealth);

	// Don't kill the player, just print a message
	if (CurrentHealth == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Player died"), Damage);
	}

	// Grab the healbar from the gamemode
	AIG_PlayerHud* hud = Cast<AIG_PlayerHud>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (hud)
	{
		auto healthbar = Cast<UIG_PlayerHealthBar>(hud->HealthBarWidgetInstance);
		if (healthbar)
		{
			// Calc and send the new percentage to the health bar
			float bar_percent = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
			healthbar->HealthBar->SetPercent(bar_percent);
		}
	}

	return (CurrentHealth - initial_health);
}