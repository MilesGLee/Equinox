// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include <Kismet/GameplayStatics.h>

AEnemyAIController::AEnemyAIController() {

	PrimaryActorTick.bCanEverTick = true;

	m_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Compoent"));
	m_behaviorTreeComponenet = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Compoenent"));
}

void AEnemyAIController::BeginPlay() {
	Super::BeginPlay();

	
	//If there is a refrencce to a bejavior tree
	if (m_behaviorTree)
	{
		//run the behavior tree 
		RunBehaviorTree(m_behaviorTree);
		//Thne start once on begin 
		m_behaviorTreeComponenet->StartTree(*m_behaviorTree);
	}
}

void AEnemyAIController::OnPossess(APawn* pawn){
    Super::OnPossess(pawn);
	//If blackboard is valid as well as the behavior tree
	if (IsValid(Blackboard->GetArchetype()) && m_behaviorTree)
		//Init the blackboard asset
		Blackboard->InitializeBlackboard(*m_behaviorTree->BlackboardAsset);
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	// If there is a ref to a player in the character array 
	if (ACharacter* player = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		//Update PLayers Positison 
		PlayerPosistion = player->GetActorLocation();
}

FRotator AEnemyAIController::GetControlRotation() const
{
	//If there is no pawn owner 
	if (GetPawn() == nullptr)
	{
		//return a empty FRoatator
		return FRotator();
	}

	//other wise return a FRoatator with pawns Yaw updated 
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}
