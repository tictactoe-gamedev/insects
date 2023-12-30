// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IG_GameMode.generated.h"

class UIG_PlayerScoreWidget;
class AIG_PlayerHud;

/**
 * 
 */
UCLASS()
class IG_API AIG_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	friend class AIG_PlayerHud;

public:

	virtual void BeginPlay() override;
	
	// Called each time an enemy is killed
	UFUNCTION(BlueprintCallable)
	void IncrementScore();

	// Reload the level
	UFUNCTION(BlueprintCallable)
	void RestartGame();

	// Check if gameover is set
	UFUNCTION(BlueprintCallable)
	bool IsGameOver()
	{
		return bIsGameOver;
	}

	// Callback for player death delegate
	UFUNCTION()
	void PlayerDied();

protected:
	// Incremented once for every enemy killed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlayerScore{0};

	// Track if gameover has occurred
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGameOver{false};

	// Cache vars
	TObjectPtr<AIG_PlayerHud> PlayerHud{nullptr};
	TObjectPtr<UIG_PlayerScoreWidget> ScoreWidget{nullptr};

	// Callback to let the HUD tell us when it is ready
	void HudReady();
};
