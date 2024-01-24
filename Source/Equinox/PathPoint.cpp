// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPoint.h"

// Sets default values
APathPoint::APathPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Hide the actor during runtime, but show it in the editor
	SetActorHiddenInGame(true);
}
