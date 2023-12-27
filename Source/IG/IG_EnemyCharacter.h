// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IG_EnemyCharacter.generated.h"

class AIG_EnemySpawner;
class AIG_PlayerCharacter;

UCLASS()
class IG_API AIG_EnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIG_EnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when health reaches zero. Handles cleanup.
	void Died();

	// Pointer to the enemy healthbar
	UUserWidget* HealthBarWidgetInstance{nullptr};

	// Performs an attack against a playerg
	UFUNCTION(BlueprintCallable)
	void Attack(AIG_PlayerCharacter* player);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Maximum enemy health
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int MaxHealth;

	// Current enemy health
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int CurrentHealth;

	// The spawner responsible for this enemy
	AIG_EnemySpawner* spawner;
	
	// Causes the enemy to take damage
	float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser);

	// The health bar UI BP to use
	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> HealthBarWidget;

	// How close to attempt to get to the target
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseStopDistance = 100.f;

	// How close to the target before enemy can attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = ChaseStopDistance + 50.f;

	// How frequently the enemy can attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackTime = 2.f;

	// How long before the next attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentAttackTime = AttackTime;

	// How much damage the enemy does per attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage = 5.f;

	// Timer for removal on death
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeathRemovalTime = 3.f;
};
