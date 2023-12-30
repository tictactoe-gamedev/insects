// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "IG_EnemyHealthBar.generated.h"

class AIG_EnemyCharacter;

/**
 * 
 */
UCLASS()
class IG_API UIG_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

	// TODO: Convert health bar updates to use delegate subscription
	friend class AIG_EnemyCharacter;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
};
