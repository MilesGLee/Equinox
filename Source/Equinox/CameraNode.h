// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MainCamera.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraNode.generated.h"

UCLASS()
class EQUINOX_API ACameraNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// Variables
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UBoxComponent* TriggerBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UArrowComponent* CameraPosition;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UArrowComponent* TargetPosition;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	bool _onlyWhileTouching;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FCameraData _data;
};
