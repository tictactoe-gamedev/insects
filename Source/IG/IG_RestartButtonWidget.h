// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "IG_RestartButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class IG_API UIG_RestartButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ButtonPressed();
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(BindWidget))
	UButton* Button;
	
};
