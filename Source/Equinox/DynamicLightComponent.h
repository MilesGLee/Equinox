// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynamicLightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EQUINOX_API UDynamicLightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDynamicLightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Randomize the variables controlling the light
	UFUNCTION(BlueprintCallable)
	void FluctuateLight();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPointLightComponent* Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxIntensity = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinIntensity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RangeModifier = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanChange = true;

private:
	float _desiredIntensity;

	float _desiredRange;

};
