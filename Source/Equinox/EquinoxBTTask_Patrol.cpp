// Fill out your copyright notice in the Description page of Project Settings.


#include "EquinoxBTTask_Patrol.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyCharacter.h"
#include "PathPoint.h"
#include <NavigationSystem.h>

UEquinoxBTTask_Patrol::UEquinoxBTTask_Patrol() {
	NodeName = TEXT("Patrol Path");

	//Sets a vector to only accept Vectors 
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UEquinoxBTTask_Patrol, BlackboardKey));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UEquinoxBTTask_Patrol, BlackboardKey), APawn::StaticClass());
	bNotifyTick = true;
}

EBTNodeResult::Type UEquinoxBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{

		return PlannedPatrolPoints(Owner, NodeMemory);
}



FString UEquinoxBTTask_Patrol::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}

void UEquinoxBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector CurrentVelocity = ControlledPawn->GetVelocity();
	if (!CurrentVelocity.IsNearlyZero())
	{
		FVector LookAtTarget = ControlledPawn->GetActorLocation() + CurrentVelocity;
		AIController->SetFocalPoint(LookAtTarget);
	}
}

EBTNodeResult::Type UEquinoxBTTask_Patrol::RandomPatrolPoints(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{

	FNavLocation Location{};

	//Gets AI Pawn
	AAIController* AIController{ Owner.GetAIOwner() };
	APawn* AIPawn{ AIController->GetPawn() };

	//Gets Pawns Origin
	const FVector Origin{ AIPawn->GetActorLocation() };

	//Obtain Navigation System and find a random location 
	const UNavigationSystemV1* NavSystem{ UNavigationSystemV1::GetCurrent(GetWorld()) };

	//if navigation system is valid and was able to get a random point from the navigation system
	if (IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(Origin, m_serachRadius, Location)) {
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);

		FVector Destination = AIController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		AIController->MoveToLocation(Destination);

		//Signals the behavoior Tree Component that the task finished with Success
		FinishLatentTask(Owner, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	//Signals the behavoior Tree Component that the task finished with Failed
	FinishLatentTask(Owner, EBTNodeResult::Failed);

	return EBTNodeResult::Failed;

}

EBTNodeResult::Type UEquinoxBTTask_Patrol::PlannedPatrolPoints(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	//If there is no AIcontrorller 
	AAIController* AIController = Owner.GetAIOwner();
	if (!AIController)
		//assign it a random point to follow 
		return RandomPatrolPoints(Owner, NodeMemory);

	// If there is no enemy pawn just assing 
	AEnemyCharacter* EnemyPawn = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!EnemyPawn)
		return RandomPatrolPoints(Owner, NodeMemory);

	//Checks if th eenemy is set to standing still
	if (EnemyPawn->StandsStill)
	{
		//Just retunrs as Succeeded
		//Signals the behavoior Tree Component that the task finished with Success
		FinishLatentTask(Owner, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	//If no blackboard component
	UBlackboardComponent* BlackboardComp = Owner.GetBlackboardComponent();
	if (!BlackboardComp)
		//Assaigns random path point instead 
		return RandomPatrolPoints(Owner, NodeMemory);

	//Checks the number of path points 
	if (EnemyPawn->PathPoints.Num() == 0)
	{
		//If not alerts dev that there is none 
		UE_LOG(LogTemp, Warning, TEXT("No patrol points set"));
		return RandomPatrolPoints(Owner, NodeMemory);
	}

	//iteration runs it's end of the index 
	if (m_pathIndex >= EnemyPawn->PathPoints.Num())
		//sets the index back to the start
		m_pathIndex = 0;

	//Fallows the path point assaigned to enemy by the path point array
	AActor* NextPatrolPoint = EnemyPawn->PathPoints[m_pathIndex];

	AIController->MoveToActor(NextPatrolPoint);
	FVector LookAtTarget = EnemyPawn->GetActorForwardVector() + EnemyPawn->GetVelocity();
	AIController->SetFocalPoint(LookAtTarget);

	m_pathIndex++;

	return EBTNodeResult::Succeeded;
}