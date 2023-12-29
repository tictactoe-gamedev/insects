// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IG_EnemyCharacter.generated.h"

class AIG_GameMode;
class AIG_EnemySpawner;
class AIG_PlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, AIG_EnemyCharacter*, Enemy);

UCLASS()
class IG_API AIG_EnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIG_EnemyCharacter();

	// Enemy death delegate
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDeath OnEnemyDeathDelegate;

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
	bool InAttackRange() const
	{
		return FVector::Distance(GetActorLocation(), PlayerLocation) <= AttackRange;
	}
	
	// Update the navigation target
	void UpdatePath();

	// Maximum enemy health
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxHealth{100};

	// Current enemy health
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentHealth{100};

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

	// Callback for player death event
	UFUNCTION()
	void OnPlayerDeath()
	{
		// Deactivate ourselves
		SetActorTickEnabled(false);
	}

	// Callback to remove the corpse
	UFUNCTION()
	void OnCorpseRemovalTimer()
	{
		K2_DestroyActor();
	}
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Causes the enemy to take damage
	virtual float TakeDamage(const float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;
};
