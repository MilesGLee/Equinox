// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/Character.h>
#include "Camera/CameraActor.h"
#include "InteractableActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_playerFeet = CreateDefaultSubobject<UArrowComponent>(TEXT("Player Feet"));
	_playerFeet->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	_playerFeetLocation = _playerFeet->GetComponentLocation();

	_inShadowMovement = false;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!_inShadowMovement) 
	{
		ApplyRotation();
		ApplyMovement();
	}
	else if (_inShadowMovement) 
	{
		//ApplyShadowMovementAndRotation();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::SetInput(float horizontal, float vertical)
{
	// Set the horizontal and vertical input
	_horizontalInput = horizontal;
	_verticalInput = vertical;
}

void APlayerCharacter::Interact()
{
	if (_currentInteraction != nullptr) 
	{
		_currentInteraction->OnInteract.Broadcast();
	}
}

void APlayerCharacter::ApplyMovement()
{
	// If the player is inputting a direction, update the forward and right vectors
	if (_verticalInput == 0)
		_playerForwardVector = PlayerCamera->GetActorForwardVector();
	if (_horizontalInput == 0)
		_playerRightVector = PlayerCamera->GetActorRightVector();

	// Add the movement input to the forward and right vectors
	FVector forwardDirection = _playerForwardVector * _verticalInput;
	FVector rightDirection = _playerRightVector * _horizontalInput;

	forwardDirection.Normalize();
	rightDirection.Normalize();

	// Apply movement to the target character
	AddMovementInput(forwardDirection, 10.0f);
	AddMovementInput(rightDirection, 10.0f);
}

void APlayerCharacter::ApplyRotation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("H:%f, V:%f"), _horizontalInput, _verticalInput));

	// If the player is inputting a direction
	if (_horizontalInput == 0 && _verticalInput == 0) return;

	// Get the direction of the player
	FVector dir = (_verticalInput * _playerForwardVector) + (_horizontalInput * _playerRightVector);
	dir.Z = 0;
	dir.Normalize();

	// Apply rotation to target character
	APlayerController* controller = UGameplayStatics::GetPlayerController(this->GetWorld(), 0);
	controller->SetControlRotation(FMath::Lerp(GetActorRotation(), UKismetMathLibrary::MakeRotFromXZ(dir, FVector::UpVector), 0.05f));
	//SetActorRotation(FMath::Lerp(GetActorRotation(), UKismetMathLibrary::MakeRotFromXZ(dir, FVector::UpVector), 1.05f));
}

void APlayerCharacter::TraceMovementLocation()
{
	// the first location that the first set of traces will use
	FVector firstLocation = (GetMovementDirection() * 500.0f) + (GetActorUpVector() * -500.0f);
	firstLocation.Normalize();
	// the second location that the second set of traces will use
	FVector secondLocation = (GetActorLocation()) + (firstLocation * 100.0f);

	FCollisionQueryParams params;
	FHitResult hitF1;
	FHitResult hitF2;
	FHitResult hitR;
	
	GetWorld()->LineTraceSingleByChannel(hitF1, GetActorLocation(), (GetActorLocation() + (firstLocation * 200.0f)), ECC_Visibility, params);

	bool angleCheck = false;

	if (hitF1.bBlockingHit) 
	{
		/*if (hitF1.Actor != nullptr)

			if (hitF1.Actor->ActorHasTag("NoWalk")) return;*/

		_currentHitResult = hitF1;

		FVector endPoint = (GetMovementDirection() * 600.0f) + (GetActorUpVector() * -500.0f);
		endPoint.Normalize();
		endPoint *= 250.0f;
		endPoint += GetActorLocation();

		GetWorld()->LineTraceSingleByChannel(hitF2, GetActorLocation(), endPoint, ECC_Visibility, params);

		if(hitF2.bBlockingHit)
		{
			FVector endPointR = GetActorLocation() + (GetMovementDirection() * 500.0f) + (GetActorUpVector() * -500.0f) + (UKismetMathLibrary::RotateAngleAxis(GetMovementDirection(), 90.0f, GetActorUpVector()) * 100.0f);

			GetWorld()->LineTraceSingleByChannel(hitR, GetActorLocation(), endPointR, ECC_Visibility, params);

			if (hitR.bBlockingHit)
			{
				FVector targetLoc = (hitF1.Location + (hitF1.Normal * 60.0f));
				FRotator targetRot = UKismetMathLibrary::MakeRotationFromAxes(UKismetMathLibrary::GetDirectionUnitVector(hitF1.Location, hitF2.Location), UKismetMathLibrary::GetDirectionUnitVector(hitF1.Location, hitR.Location), hitF1.Normal);

				_targetLocation = targetLoc;
				_targetRotation = targetRot;
			}
		}
		else 
		{
			angleCheck = true;
		}
	}
	else 
	{
		angleCheck = true;
	}

	if (angleCheck) 
	{
		GetWorld()->LineTraceSingleByChannel(hitF1, secondLocation, secondLocation + (UKismetMathLibrary::RotateAngleAxis(firstLocation, 90.0f, GetActorRightVector()) * 200.0f), ECC_Visibility, params);

		if (hitF1.bBlockingHit)
		{
			_currentHitResult = hitF1;

			GetWorld()->LineTraceSingleByChannel(hitF2, secondLocation, secondLocation + (UKismetMathLibrary::RotateAngleAxis(firstLocation, 85.0f, GetActorRightVector()) * 200.0f), ECC_Visibility, params);
			
			if (hitF2.bBlockingHit)
			{
				FVector endPoint = (UKismetMathLibrary::RotateAngleAxis((UKismetMathLibrary::RotateAngleAxis(firstLocation, 90.0f, GetActorRightVector())), 5.0f, firstLocation)) * 200.0f;

				GetWorld()->LineTraceSingleByChannel(hitR, secondLocation, secondLocation + endPoint, ECC_Visibility, params);

				if (hitR.bBlockingHit)
				{
					FVector targetLoc = (hitF1.Location + (hitF1.Normal * 60.0f));
					FRotator targetRot = UKismetMathLibrary::MakeRotationFromAxes(UKismetMathLibrary::GetDirectionUnitVector(hitF1.Location, hitF2.Location), UKismetMathLibrary::GetDirectionUnitVector(hitF1.Location, hitR.Location), hitF1.Normal);

					_targetLocation = targetLoc;
					_targetRotation = targetRot;
				}
			}
		}
	}
}

FVector APlayerCharacter::GetMovementDirection()
{
	_shadowFV = FVector(PlayerCamera->GetActorForwardVector().X, PlayerCamera->GetActorForwardVector().Y, GetActorForwardVector().Z);
	_shadowRV = FVector(PlayerCamera->GetActorRightVector().X, PlayerCamera->GetActorRightVector().Y, GetActorRightVector().Z);
	FVector direction = (_shadowFV * _verticalInput) + (_shadowRV * _horizontalInput);
	
	direction.Normalize();

	return direction;
}

float APlayerCharacter::AngleBetweenTwoVectors(FVector a, FVector b)
{
	a.Normalize();
	b.Normalize();

	float x = FVector::DotProduct(a, b);

	x = FMath::Acos(x);

	return x;
}

void APlayerCharacter::ApplyShadowMovementAndRotation()
{
	float inputValue = FMath::Abs(_verticalInput) + FMath::Abs(_horizontalInput);
	if (inputValue > 0.0f) 
	{
		if (FVector::Distance(_targetLocation, GetActorLocation()) > 2.0f) 
		{
			FVector direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), _targetLocation);
			AddMovementInput(direction, 1.0f, false);

			FRotator rotation = FMath::Lerp(GetActorRotation(), _targetRotation, 0.05f);
			SetActorRotation(rotation);
		}
	}
}

void APlayerCharacter::SwitchMovement()
{
	_inShadowMovement = !_inShadowMovement;

	if (_inShadowMovement) 
	{
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		_targetLocation = GetActorLocation();
	}
	else if (!_inShadowMovement) 
	{
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		SetActorRotation(FRotator(0, 0, 0));
	}
}

void APlayerCharacter::SetMovement(bool set)
{
	if (set) 
	{
		_inShadowMovement = set;
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
		bUseControllerRotationYaw = false;
		_targetLocation = GetActorLocation();
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else 
	{
		_inShadowMovement = set;
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		SetActorRotation(FRotator(0, 0, 0));
	}
}

