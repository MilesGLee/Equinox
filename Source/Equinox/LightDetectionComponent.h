// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightDetectionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EQUINOX_API ULightDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULightDetectionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	bool CanDive() { return m_canDive; };


private:
	UPROPERTY(EditAnywhere, Category = "LightDetection")
	TArray<AActor*> m_lightActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightDetection", meta = (AllowPrivateAccess = true))
	bool m_canDive = true;

	bool m_lightsHitting = false;

	UPROPERTY(EditAnywhere, Category = "LightDetection", meta = (AllowPrivateAccess = true))
	FVector m_lightstart;
};
