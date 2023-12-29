// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "IG_PlayerHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class IG_API UIG_PlayerHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
};
