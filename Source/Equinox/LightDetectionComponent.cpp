// Fill out your copyright notice in the Description page of Project Settings.


#include "LightDetectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Light.h"
#include <Components/PointLightComponent.h>
#include <Components/LightComponent.h>
#include <DrawDebugHelpers.h>

// Sets default values for this component's properties
ULightDetectionComponent::ULightDetectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void ULightDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), UPointLightComponent::StaticClass(), m_lightActors);
}


// Called every frame
void ULightDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = Cast<AActor>(GetOwner());

	

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALight::StaticClass(), m_lightActors);

	m_lightsHitting = false;

	for (AActor* lightActor : m_lightActors)
	{
		float distance = FVector::Dist(lightActor->GetActorLocation(), Owner->GetActorLocation());
		
		if (UPointLightComponent* light = lightActor->FindComponentByClass<UPointLightComponent>() ) {


			// check to see if the distance is close to the raduia of the lights attenuation 
			if (distance <= light->AttenuationRadius) {

				FVector StartLocation = Owner->GetActorLocation() + m_lightstart; // Set the starting location of the raycast
				FVector EndLocation = lightActor->GetActorLocation(); // Set the ending location of the raycast

				FHitResult bHit;
				FCollisionQueryParams params;

				//Sets the parameters that the raytrace can't hit 
				params.AddIgnoredActor(Owner);
				params.AddIgnoredActor(lightActor);


				if (GetWorld()->LineTraceSingleByChannel(bHit, StartLocation, EndLocation, ECollisionChannel::ECC_Destructible, params, FCollisionResponseParams()))
						m_canDive = true;
				
				else 
					m_lightsHitting = true;

				
			}
		}

		else continue;

		m_canDive = true;
	}
	if (m_lightsHitting)
		m_canDive = false;

}