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

	/*
	 * Remove an enemy from the collection
	 *  @param Enemy the enemy actor to clean up
	 */
	UFUNCTION(BlueprintCallable)
	void CleanupEnemy(AIG_EnemyCharacter* Enemy);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AIG_EnemyCharacter> EnemyBase;

	// Spawns an enemy
	UFUNCTION(BlueprintCallable)
	void Spawn();

	// Box area that we are allowed to spawn in
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionMesh{nullptr};

	// How long (in seconds) between each spawn
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpawnTimer{5};

	// The current timer for spawn
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpawnTimeCurrent{0};

	// A collection of enemies we have spawned, used to track how many are alive
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AIG_EnemyCharacter*> SpawnedEnemies;

	// The total number of alive enemies we will spawn at once
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxEnemies = 6;

	// Cached vars
	TObjectPtr<AIG_GameMode> GameMode{nullptr};

	// Callback for player death event
	UFUNCTION()
	void OnPlayerDeath()
	{
		// Deactivate ourselves
		SetActorTickEnabled(false);
	}
};
