// GPLv3

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IG_EnemyCharacter.h"
#include "IG_EnemySpawner.generated.h"

UCLASS()
class IG_API AIG_EnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIG_EnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AIG_EnemyCharacter> EnemyBase;

    UFUNCTION(BlueprintCallable)
    void Spawn();

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UBoxComponent* CollisionMesh = nullptr;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float SpawnTimer;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float SpawnTimeCurrent = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AIG_EnemyCharacter*> spawned_enemies;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxEnemies = 6;

	UFUNCTION(BlueprintCallable)
	void CleanupEnemy(AIG_EnemyCharacter* enemy);
};
