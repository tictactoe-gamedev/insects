// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"

#include "../Enemy/IG_EnemyCharacter.h"
#include "IG_PlayerCharacter.generated.h"

class UIG_PlayerHealthBar;
class AIG_PlayerHud;

UCLASS()
class IG_API AIG_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIG_PlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Callback for receiving damage
	virtual float TakeDamage(const float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// The amount of damage the player will deal
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PlayerDamage = 20.f;
	
	// Movement speed of the player
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DefaultPlayerMoveSpeed = 0.0f;

	/**
	 * Run hit detection on the mace head
	 * @return The enemy that was hit (or nullptr for no hit)
	 */
	UFUNCTION(BlueprintCallable)
	AIG_EnemyCharacter* DoHitDetection();
	
	// Clear contact history for hits
	UFUNCTION(BlueprintCallable)
	void ClearHitDetection();
	
	// Maximum health of the player 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxHealth = 100;
	
	//Current health of the player 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentHealth = 100;

	// Player is dead
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Dead = false;

	// Multiplier for sprint speed
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SprintModifier = 2.0f;

	// Input context and action assignments
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInputMappingContext* InputContext{nullptr};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInputAction* InputActionMove{nullptr};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInputAction* InputActionSprint{nullptr};
	
	// Array of actors to ignore from hit detections
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> ActorsToIgnore{};

	// Callbacks for inputs
	void OnMove(const FInputActionValue& Value);
	void OnSprintStart(const FInputActionValue& Value);
	void OnSprintEnd(const FInputActionValue& Value);

	// Cache vars
	TObjectPtr<APlayerController>		PlayerController{nullptr};
	TObjectPtr<USkeletalMeshComponent>	Weapon{nullptr};
	TObjectPtr<AIG_GameMode>			GameMode{nullptr};
	TObjectPtr<AIG_PlayerHud>			PlayerHud{nullptr};
	TObjectPtr<UIG_PlayerHealthBar>		PlayerHealthBar{nullptr};

};
