// GPLv3

#pragma once

#include "IG_EnemyCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float PlayerMoveSpeed;

    UFUNCTION(BlueprintCallable)
	AIG_EnemyCharacter* DoHitDetection();

    UFUNCTION(BlueprintCallable)
    void ClearHitDetection();

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int MaxHealth;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int CurrentHealth;

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<AActor*> ActorsToIgnore;

};
