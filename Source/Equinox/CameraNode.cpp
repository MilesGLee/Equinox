// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraNode.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "PlayerCharacter.h"
#include "MainCamera.h"

// Sets default values
ACameraNode::ACameraNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACameraNode::OnBoxBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ACameraNode::OnBoxOverlapEnd);
	
	CameraPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Camera Position"));
	TargetPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Target Position"));

	_data.cameraMode = CameraMode::STATIC;
	_data.cameraPosition = FVector::ZeroVector;
	_data.lookPosition = FVector::ZeroVector;
	_data.attachOffset = FVector::ZeroVector;
	_data.orbitOffset = FVector2D::ZeroVector;
	_data.attachActor = nullptr;
	_data.cameraRotationSpeed = 0.05f;
 	_data.cameraMoveSpeed = 0.01f;
	_data.cameraMoveLerpModifier = 0.005f;
	_data.cameraRotationLerpModifier = 0.005f;
	_data.snapCameraPosition = false;
	_data.snapCameraRotation = false;
}

// Called when the game starts or when spawned
void ACameraNode::BeginPlay()
{
	Super::BeginPlay();
	
	_data.cameraPosition = CameraPosition->GetComponentLocation();
	_data.lookPosition = TargetPosition->GetComponentLocation();
}

// Called every frame
void ACameraNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACameraNode::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor))
	{
		if (AMainCamera* camera = Cast<AMainCamera>(player->GetCamera())) 
		{
			if (_data.cameraMode == CameraMode::ORBIT || _data.cameraMode == CameraMode::ATTACH || _data.cameraMode == CameraMode::ATTACHLOOK || _data.cameraMode == CameraMode::STATICLOOK)
			{
				_data.attachActor = camera->PlayerActor;
			}

			camera->SetCameraData(_data);
		}
	}
}

void ACameraNode::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor))
	{
		if (AMainCamera* camera = Cast<AMainCamera>(player->GetCamera()))
		{
			if (_onlyWhileTouching)
			{
				camera->SetCameraData(camera->OldCameraData);
			}
		}
	}
}


