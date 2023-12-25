// GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InputMappingContext.h"
#include "IG_GameMode.generated.h"

UCLASS()
class IG_API AIG_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	
public:
	TObjectPtr<const UInputAction> GetMoveAction() const;
	TObjectPtr<const UInputAction> GetAttackAction() const;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> CurrentInputContext;

	UPROPERTY()
	TObjectPtr<const UInputAction> MoveAction=nullptr;
	
	UPROPERTY()
	TObjectPtr<const UInputAction> AttackAction=nullptr;

	
	TObjectPtr<const UInputAction> FindInputAction(const FString& InputMappingName) const;
};
