// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IG_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class IG_API AIG_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Called each time an enemy is killed
	UFUNCTION(BlueprintCallable)
	void IncrementScore();

	// Get the player score
	UFUNCTION(BlueprintCallable)
	int GetPlayerScore();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	void SetGameOver();

	UFUNCTION(BlueprintCallable)
	bool GetGameOver();

protected:
	// Incremented once for every enemy killed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlayerScore = 0;

	// Track if gameover has occurred
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool GameOver = false;
};
