// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IG_PlayerHud.generated.h"

/**
 * 
 */
UCLASS()
class IG_API AIG_PlayerHud : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> HealthBarWidget;
	UPROPERTY(EditAnywhere) UUserWidget* HealthBarWidgetInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
