// GPLv3

#pragma once

#include "IG_EnemyCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "IG_PlayerCharacter.generated.h"

UCLASS()
class IG_API AIG_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIG_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * The amount of damage the player will deal
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PlayerDamage = 20.f;
	
	/**
     * Movement speed of the player
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float PlayerMoveSpeedModifier = 1.f;

	/**
	 * Run hit detection on the mace head
	 * @return The enemy that was hit (or nullptr for no hit)
	 */
	UFUNCTION(BlueprintCallable)
	AIG_EnemyCharacter* DoHitDetection();

	/**
	 * Clear contact history
	 */
	UFUNCTION(BlueprintCallable)
    void ClearHitDetection();

	/**
	 * Maximum health of the player
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int MaxHealth = 100;

	/**
	 * Current health of the player
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int CurrentHealth = 100;

	// Player is dead
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Dead = false;

	float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInputMappingContext* InputContext{nullptr};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInputAction* InputActionMove{nullptr};
protected:
	/**
	 * Array of actors to ignore from hit detections
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<AActor*> ActorsToIgnore;

	void OnMove(const FInputActionValue& Value);

	APlayerController* PlayerController{nullptr};
};
