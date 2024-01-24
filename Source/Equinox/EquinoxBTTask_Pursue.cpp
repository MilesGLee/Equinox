// Fill out your copyright notice in the Description page of Project Settings.


#include "EquinoxBTTask_Pursue.h"
#include <AIController.h>
#include <GameFramework/Character.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <NavigationSystem.h>
#include "EnemyAIController.h"
#include "EnemyCharacter.h"

UEquinoxBTTask_Pursue::UEquinoxBTTask_Pursue() {

	NodeName = TEXT("Pursue Path");
	bNotifyTick = true;

	//Sets a vector to only accept Vectors 
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UEquinoxBTTask_Pursue, BlackboardKey));
}

EBTNodeResult::Type UEquinoxBTTask_Pursue::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	FNavLocation Location{};

	//Gets AI Pawn
	AEnemyAIController* EnemyControlleroller{ Cast<AEnemyAIController>(Owner.GetAIOwner()) };
	AEnemyCharacter* AIPawn{ Cast<AEnemyCharacter>(EnemyControlleroller->GetPawn()) };

	//Gets Pawns Origin
	const FVector Origin{ AIPawn->GetActorLocation() };

	//Obtain Navigation System and find a random location 
	const UNavigationSystemV1* NavSystem{ UNavigationSystemV1::GetCurrent(GetWorld())};

	FVector Target = { EnemyControlleroller->PlayerPosistion };

	if (EnemyControlleroller)
	{
		Location.Location = Target;

		//if navigation system is valid and was able to get a random point from the navigation system
		if (IsValid(NavSystem) && NavSystem->FindPathToLocationSynchronously(GetWorld(), Origin,Location.Location)) {

		
				FVector TargetLocation = EnemyControlleroller->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

				EnemyControlleroller->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
				EnemyControlleroller->MoveToLocation(TargetLocation);

				//Signals the behavoior Tree Component that the task finished with Success
				FinishLatentTask(Owner, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
		}
		
		//Signals the behavoior Tree Component that the task finished with Failed
		FinishLatentTask(Owner, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	//Signals the behavoior Tree Component that the task finished with Failed
	FinishLatentTask(Owner, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}

FString UEquinoxBTTask_Pursue::GetStaticDescription() const
{
    return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
