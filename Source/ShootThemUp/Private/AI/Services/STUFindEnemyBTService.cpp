// Shoot Them Up Game. All Rights Reserved


#include "AI/Services/STUFindEnemyBTService.h"
#include "AIController.h"
#include "STUUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUAIPerceptionComponent.h"	

USTUFindEnemyBTService::USTUFindEnemyBTService()
{
	NodeName = "Find Enemy";
}
void USTUFindEnemyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogTemp, Display, TEXT("Find Enemy"));
	if (const auto Blackboard = OwnerComp.GetBlackboardComponent())
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PreceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
		if (PreceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PreceptionComponent->GetClosestEnemy());
			const auto Enemy = PreceptionComponent->GetClosestEnemy();
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, Enemy);
			if (Controller)
			{
				Controller->SetFocus(Enemy);
			}
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}