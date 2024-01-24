// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MainCamera.generated.h"

UENUM(BlueprintType)
enum CameraMode 
{
	STATIC UMETA(DisplayName = "Static Camera"),
	STATICLOOK UMETA(DisplayName = "Static Looking Camera"),
	ATTACH UMETA(DisplayName = "Attached Camera"),
	ATTACHLOOK UMETA(DisplayName = "Attached Looking Camera"),
	ORBIT UMETA(DisplayName = "Orbit Camera"),
};

USTRUCT(BlueprintType)
struct FCameraData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<CameraMode> cameraMode;

	UPROPERTY(BlueprintReadOnly)
	FVector cameraPosition;

	UPROPERTY(BlueprintReadOnly)
	FVector lookPosition;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector attachOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector2D orbitOffset;

	UPROPERTY(BlueprintReadOnly)
	AActor* attachActor;

	UPROPERTY(BlueprintReadOnly)
	float cameraMoveSpeed;

	UPROPERTY(BlueprintReadOnly)
	float cameraRotationSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float cameraMoveLerpModifier = 0.05f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float cameraRotationLerpModifier = 0.05f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool snapCameraPosition;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool snapCameraRotation;
};

UCLASS()
class EQUINOX_API AMainCamera : public ACameraActor
{
	GENERATED_BODY()
	
// Contructors
public:
	AMainCamera();

// Functions
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets the camera's data to change its style in scene
	UFUNCTION(BlueprintCallable)
	void SetCameraData(FCameraData data);

	UFUNCTION(BlueprintCallable)
	void DefaultCameraData();

private:
	// Rotates the camera to look at its target
	void RotateCamera();

	// Moves the camera
	void MoveCamera();

// Variables
public:
	UPROPERTY(BlueprintReadOnly)
	FCameraData CurrentCameraData;

	FCameraData OldCameraData;

	UPROPERTY(BlueprintReadWrite)
	AActor* PlayerActor;

	UPROPERTY(EditAnywhere)
	FVector DefaultAttachVector;

private:
	TEnumAsByte<CameraMode> _cameraMode;
};
