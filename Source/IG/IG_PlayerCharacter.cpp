// GPLv3



#include "IG_PlayerCharacter.h"
#include "IG_GameMode.h"
#include "IG_PlayerHealthBar.h"
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

    FVector start_location = GetMesh()->GetSocketLocation("Start");
    FVector end_location = GetMesh()->GetSocketLocation("End");

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Reserve(1);
    ObjectTypes.Emplace(ECollisionChannel::ECC_Pawn);

    FHitResult HitResult = FHitResult(ForceInit);
    bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
            GetWorld(),
            start_location,
            end_location,
            20.f,
            ObjectTypes,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::Type::ForDuration,
            HitResult,
            true,
            FLinearColor(255,0,0,255),
            FLinearColor(0,255,0,255),
            1.f
            );

    if (hit)
    {
    	//auto hit_guid = HitResult.GetActor()->GetActorGuid();
    	auto hit_actor = HitResult.GetActor();
    	
    	if (ActorsToIgnore.Find(hit_actor) == INDEX_NONE)
    	{
    		ActorsToIgnore.AddUnique(HitResult.GetActor());
    		AIG_EnemyCharacter* enemy = Cast<AIG_EnemyCharacter>(HitResult.GetActor());
    		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(HitResult.GetActor()->GetName()));

    		return enemy;
    	}
    }

    return nullptr;
}

void AIG_PlayerCharacter::ClearHitDetection() {
    ActorsToIgnore.Empty();
    ActorsToIgnore.Push(this);
}

float AIG_PlayerCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) {

	UE_LOG(LogTemp, Warning, TEXT("Player took %.2f damage"), Damage);
	int initial_health = CurrentHealth;
	CurrentHealth = std::clamp(CurrentHealth - static_cast<int>(Damage), 0, MaxHealth);
	
	if (CurrentHealth == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Player died"), Damage);
	}

	AIG_GameMode* gamemode = Cast<AIG_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gamemode)
	{
		auto healthbar = Cast<UIG_PlayerHealthBar>(gamemode->HealthBarWidgetInstance);
		if (healthbar)
		{
			auto hba = healthbar->HealthBar;
			if (hba)
			{
				float bar_percent = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
				hba->SetPercent(bar_percent);
			}
		}
	}

	return (CurrentHealth - initial_health);
}