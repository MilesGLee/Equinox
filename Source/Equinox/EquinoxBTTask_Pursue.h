// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EquinoxBTTask_Pursue.generated.h"

/**
 * 
 */
UCLASS()
class EQUINOX_API UEquinoxBTTask_Pursue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UEquinoxBTTask_Pursue();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

};
