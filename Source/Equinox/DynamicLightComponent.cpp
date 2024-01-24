// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicLightComponent.h"
#include "Components/PointLightComponent.h"

// Sets default values for this component's properties
UDynamicLightComponent::UDynamicLightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	Light->bUseInverseSquaredFalloff = false;
	Light->SetLightFalloffExponent(0.001f);
	Light->ShadowSharpen = 1.0f;
	Light->SetLightColor(LightColor);
	Light->SetAttenuationRadius(Range);
	Light->SetIntensity(MaxIntensity);
}


// Called when the game starts
void UDynamicLightComponent::BeginPlay()
{
	Super::BeginPlay();

	FluctuateLight();
}


// Called every frame
void UDynamicLightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Light->SetIntensity(FMath::Lerp(Light->Intensity, _desiredIntensity, 0.05f));
	Light->SetAttenuationRadius(FMath::Lerp(Light->AttenuationRadius, _desiredRange, 0.05f));
}

void UDynamicLightComponent::FluctuateLight()
{
	float x = FMath::RandRange(MinIntensity, MaxIntensity);
	float y;
	if (x >= (MaxIntensity + MinIntensity) / 2) 
	{
		y = Range + (x * RangeModifier);
	}
	else 
	{
		y = Range - (x * RangeModifier);
	}
	
	_desiredIntensity = x;
	_desiredRange = y;
}

