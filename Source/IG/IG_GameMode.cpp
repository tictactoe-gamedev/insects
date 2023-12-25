// GPLv3


#include "IG_GameMode.h"

TObjectPtr<const UInputAction> AIG_GameMode::GetMoveAction() const
{
	if(MoveAction)
		return MoveAction;
	
	return FindInputAction("PlayerMove");
}

TObjectPtr<const UInputAction> AIG_GameMode::GetAttackAction() const
{
	if(AttackAction)
		return AttackAction;
	
	return FindInputAction("PlayerAttack");
}

TObjectPtr<const UInputAction> AIG_GameMode::FindInputAction(const FString& InputMappingName) const
{
	TArray<FEnhancedActionKeyMapping> Mappings = this->CurrentInputContext->GetMappings();

	UE_LOG(LogTemp,Warning,TEXT("%d keymappings found"),Mappings.Num());

	for(const auto& Mapping : Mappings)
	{
		TObjectPtr<const UInputAction> Action = Mapping.Action;

		if(Action->GetName() == InputMappingName)
		{
			return Action;
		}
	}

	UE_LOG(LogTemp,Error,TEXT("No mapping found with name %s"), *InputMappingName);
	return nullptr;
}