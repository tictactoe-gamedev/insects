// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "IG_EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class IG_API UIG_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(BindWidget))
	UProgressBar* HealthBar;
};
