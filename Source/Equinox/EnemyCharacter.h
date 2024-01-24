// Fill out your copyright notice in the Description page of Project Settings.

#pragma hdrstop

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class EQUINOX_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

public:
	//Collection of path points that will decide the path this enemy will go
	UPROPERTY(EditAnywhere, Category = "EnemyStates")
		TArray<class APathPoint*> PathPoints;

	//Bool deciding if this enemy should stand still 
	UPROPERTY(EditAnywhere, Category = "EnemyStates")
	bool StandsStill = false;
};
