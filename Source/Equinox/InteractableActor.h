// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include <Blueprint/UserWidget.h>
#include "InteractableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractEvent);

UCLASS()
class EQUINOX_API AInteractableActor : public AActor
{
	GENERATED_BODY()

// Constructors
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

// Protected functions
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// Public functions
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

// Private functions
private:

// Public properties
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* TriggerBox;

	UPROPERTY(BlueprintReadOnly)
	bool IsOverlapped;

	UPROPERTY(BlueprintAssignable)
	FOnInteractEvent OnInteract;

	//Spawns the heads up display
	UPROPERTY(EditAnywhere, Category = "Interactable")
	TSubclassOf<class UInteractiveUserWidget> WBPClass;

	UPROPERTY()
	class UInteractiveUserWidget* InteractablePrompt;

	//What the text will say in the 
	UPROPERTY(EditAnywhere, Category = "Interactable")
	FString InteractionText = TEXT("Interactable");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool callEventOnOverlap;

// Private properties
private:

};
