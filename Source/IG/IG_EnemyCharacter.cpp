// GPLv3

#include <algorithm>
#include "IG_EnemySpawner.h"
#include "IG_EnemyCharacter.h"


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
	
}

// Called every frame
void AIG_EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AIG_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AIG_EnemyCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) {

	UE_LOG(LogTemp, Warning, TEXT("Took damage: %.2f"), Damage);
	int initial_health = CurrentHealth;
	CurrentHealth = std::clamp(CurrentHealth - static_cast<int>(Damage), 0, MaxHealth);
	if (CurrentHealth == 0) {
		Died();
	}

	return (CurrentHealth - initial_health);
}

void AIG_EnemyCharacter::Died() {
	spawner->CleanupEnemy(this);
	K2_DestroyActor();
}
