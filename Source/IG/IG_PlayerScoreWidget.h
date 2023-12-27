// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "IG_PlayerScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class IG_API UIG_PlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(BindWidget))
	UTextBlock* TextBlock;
	
};
