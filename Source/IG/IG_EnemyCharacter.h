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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int MaxHealth;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int CurrentHealth;

	AIG_EnemySpawner* spawner;

	UFUNCTION(BlueprintCallable)
	void Died();

	float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser);

	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> HealthBarWidget;
	UPROPERTY(EditAnywhere) UUserWidget* HealthBarWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseStopDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentAttackTime = AttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage = 5.f;

	UFUNCTION(BlueprintCallable)
	void Attack(AIG_PlayerCharacter* player);
};
