// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class EQUINOX_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Sets the players horizontal and vertical input variables
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SetInput(float horizontal, float vertical);

	UFUNCTION()
	void SetInteraction(AInteractableActor* interact)	{_currentInteraction = interact;}

	UFUNCTION()
	AInteractableActor* GetCurrentInteraction() { return _currentInteraction; }

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	FVector GetPlayerFeet() {return _playerFeetLocation; }

	UFUNCTION(BlueprintCallable)
	ACameraActor* GetCamera() { return PlayerCamera; }

	// The players camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACameraActor* PlayerCamera;

// Private functions
private:
	/// <summary>
	/// Move the player
	/// </summary>
	void ApplyMovement();

	/// <summary>
	/// Rotate the player
	/// </summary>
	void ApplyRotation();

	/// <summary>
	/// Uses line traces to check if the available space to move is there while shadow moving
	/// </summary>
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	void TraceMovementLocation();

	/// <summary>
	/// Returns the inputted movement direction
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable)
	FVector GetMovementDirection();

	/// <summary>
	/// Gets the angle in degrees between two points
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	float AngleBetweenTwoVectors(FVector a, FVector b);

	/// <summary>
	/// Moves and rotates the player during shadow movement
	/// </summary>
	void ApplyShadowMovementAndRotation();

	/// <summary>
	/// Switches the current movement from shadow movement to normal movement
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SwitchMovement();

	/// <summary>
	/// Sets the movement type of the player
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SetMovement(bool set);

// Private variables
private:
	// The input floats for the players movement
	float _horizontalInput, _verticalInput;

	// Vectors used for players movement direction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FVector _playerForwardVector; 

	// Vectors used for players movement direction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FVector _playerRightVector;

	// The player's feet position
	UPROPERTY(EditAnywhere)
	class UArrowComponent* _playerFeet;

	// The location of the players feet
	FVector _playerFeetLocation;

	// The current interactable actor the player is interacting with
	class AInteractableActor* _currentInteraction;

	// During shadow movement the target move location
	FVector _targetLocation;

	// The rotation the player needs to be in during shadow movement
	FRotator _targetRotation;

	// The current plane the shadow walking player is on
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FHitResult _currentHitResult;

	// If the player is in shadow movment or not
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool _inShadowMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FVector _shadowFV;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	FVector _shadowRV;
};
