// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IG_EnemyCharacter.generated.h"

class AIG_GameMode;
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

	// Pointer to the enemy health bar
	TObjectPtr<UUserWidget> HealthBarWidgetInstance{nullptr};

	// Cache often-used vars
	TObjectPtr<AIG_GameMode>		GameMode{nullptr};
	TObjectPtr<APlayerController>	PlayerController{nullptr};
	TObjectPtr<APawn>				PlayerPawn{nullptr};
	TObjectPtr<AIG_PlayerCharacter> PlayerCharacter{nullptr};
	FVector							PlayerLocation{};

	// Check if we are in attack range of the player
	bool InAttackRange() const;
	// Update the navigation target
	void UpdatePath();

	// Maximum enemy health
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxHealth{100};

	// Current enemy health
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentHealth{100};

	// The spawner responsible for this enemy
	TObjectPtr<AIG_EnemySpawner> ParentSpawner;

	// The health bar UI BP to use
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HealthBarWidget;

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
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Causes the enemy to take damage
	virtual float TakeDamage(const float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

	// Allow the spawner to let the enemy know who it is
	void SetParentSpawner(AIG_EnemySpawner* Spawner)
	{
		ParentSpawner = Spawner;
	}
};
