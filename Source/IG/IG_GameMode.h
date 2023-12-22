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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> HealthBarWidget;
	UPROPERTY(EditAnywhere) UUserWidget* HealthBarWidgetInstance;
	
};
