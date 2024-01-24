// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamera.h"

// Constructor
AMainCamera::AMainCamera()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMainCamera::BeginPlay()
{
	Super::BeginPlay();

	//DefaultCameraData();
}

void AMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateCamera();

	MoveCamera();
}

void AMainCamera::SetCameraData(FCameraData data)
{
	OldCameraData = CurrentCameraData;
	CurrentCameraData = data;
	CurrentCameraData.cameraMoveSpeed = 0.001f;
	CurrentCameraData.cameraRotationSpeed = 0.001f;
}

void AMainCamera::DefaultCameraData()
{
	FCameraData data;
	data.cameraMode = CameraMode::ATTACH;
	data.attachActor = PlayerActor;
	data.attachOffset = DefaultAttachVector;
	data.cameraRotationSpeed = 0.05f;
	data.cameraRotationLerpModifier = 0.05f;
	data.cameraMoveSpeed = 0.01f;
	data.cameraMoveLerpModifier = 0.005f;

	SetCameraData(data);
}

void AMainCamera::RotateCamera()
{
	// The location of what the camera should look at
	FVector direction;

	// Set the direction vector based on the camera mode
	if (CurrentCameraData.cameraMode == CameraMode::ATTACH) 
	{
		if (CurrentCameraData.attachActor == nullptr) return;

		CurrentCameraData.lookPosition = CurrentCameraData.attachActor->GetActorLocation();
		direction = CurrentCameraData.lookPosition;
	}
	else if (CurrentCameraData.cameraMode == CameraMode::ATTACHLOOK)
	{
		direction = CurrentCameraData.lookPosition;
	}
	else if (CurrentCameraData.cameraMode == CameraMode::STATIC)
	{
		direction = CurrentCameraData.lookPosition;
	}
	else if (CurrentCameraData.cameraMode == CameraMode::STATICLOOK)
	{
		if (CurrentCameraData.attachActor == nullptr) return;

		CurrentCameraData.lookPosition = CurrentCameraData.attachActor->GetActorLocation();
		direction = CurrentCameraData.lookPosition;
	}
	else if (CurrentCameraData.cameraMode == CameraMode::ORBIT)
	{
		direction = CurrentCameraData.lookPosition;
	}

	// The direction towards where the camera needs to look
	FVector desiredDirection = direction - GetActorLocation();
	desiredDirection.Normalize();

	// Turn that direction into a rotation and lerp the cameras rotation into it
	FRotator desiredRotation = FRotationMatrix::MakeFromX(desiredDirection).Rotator();
	SetActorRotation(FMath::Lerp(GetActorRotation(), desiredRotation, CurrentCameraData.cameraRotationSpeed));

	// Slowly make the camera rotate faster over time
	if (CurrentCameraData.cameraRotationSpeed < 0.5f) 
	{
		CurrentCameraData.cameraRotationSpeed += CurrentCameraData.cameraRotationLerpModifier;
	}
}

void AMainCamera::MoveCamera()
{
	// The location the camera should move to
	FVector destination;

	// Set the destination based on the camera mode
	if (CurrentCameraData.cameraMode == CameraMode::ATTACH) 
	{
		if (CurrentCameraData.attachActor == nullptr) return;

		CurrentCameraData.cameraPosition = CurrentCameraData.attachActor->GetActorLocation() - CurrentCameraData.attachOffset;
		destination = CurrentCameraData.cameraPosition;
	}
	else if (CurrentCameraData.cameraMode == CameraMode::ATTACHLOOK) 
	{
		if (CurrentCameraData.attachActor == nullptr) return;

		CurrentCameraData.cameraPosition = CurrentCameraData.attachActor->GetActorLocation() - CurrentCameraData.attachOffset;
		destination = CurrentCameraData.cameraPosition;
	}
	else if (CurrentCameraData.cameraMode == CameraMode::ORBIT)
	{
		if (CurrentCameraData.attachActor == nullptr) return;

		FVector direction = CurrentCameraData.attachActor->GetActorLocation() - CurrentCameraData.lookPosition;
		FVector origin = CurrentCameraData.attachActor->GetActorLocation();
		direction *= CurrentCameraData.orbitOffset.X;
		origin += direction;
		origin.Z = CurrentCameraData.attachActor->GetActorLocation().Z;
		origin.Z += CurrentCameraData.orbitOffset.Y;

		CurrentCameraData.cameraPosition = origin;
		destination = CurrentCameraData.cameraPosition;
	}
	else if (CurrentCameraData.cameraMode == CameraMode::STATIC)
	{
		destination = CurrentCameraData.cameraPosition;
	}
	else if (CurrentCameraData.cameraMode == CameraMode::STATICLOOK)
	{
		destination = CurrentCameraData.cameraPosition;
	}

	// Lerp the cameras position over time to the destination
	SetActorLocation(FMath::Lerp(GetActorLocation(), destination, CurrentCameraData.cameraMoveSpeed));

	// Slowly move the camera faster over time to its destination
	if (CurrentCameraData.cameraMoveSpeed < 0.5f) 
	{
		CurrentCameraData.cameraMoveSpeed += CurrentCameraData.cameraMoveLerpModifier;
	}
}