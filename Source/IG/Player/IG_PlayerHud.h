// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IG_PlayerHud.generated.h"

class AIG_GameMode;
class AIG_PlayerCharacter;

/**
 * 
 */
UCLASS()
class IG_API AIG_PlayerHud : public AHUD
{
	GENERATED_BODY()
	
public:
	// Enables the restart button
	UFUNCTION(BlueprintCallable)
	void ShowRestartButton();

	friend class AIG_GameMode;
	friend class AIG_PlayerCharacter;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Template for the health bar
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HealthBarWidget;
	// Actual instance of the health bar
	UPROPERTY(EditAnywhere)
	TObjectPtr<UUserWidget> HealthBarWidgetInstance;

	// Template for the score text
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ScoreWidget;
	// Actual instance of the score text
	UPROPERTY(EditAnywhere)
	TObjectPtr<UUserWidget> ScoreWidgetInstance;

	// Template for the restart button
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> RestartButtonWidget;
	// Actual instance of the restart button
	UPROPERTY(EditAnywhere)
	TObjectPtr<UUserWidget> RestartButtonInstance;
	
};
