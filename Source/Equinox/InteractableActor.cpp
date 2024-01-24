// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include <Blueprint/UserWidget.h>
#include "InteractiveUserWidget.h"
#include <Components/TextBlock.h>

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnBoxBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnBoxEndOverlap);

	WBPClass = nullptr;
	InteractablePrompt = nullptr;
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	//Check to see if there is a . . .
	if (WBPClass)
	{
		//. . . It'll creata a refrence based on that widget 
		InteractablePrompt = CreateWidget<UInteractiveUserWidget>(GetWorld(), WBPClass);
		check(InteractablePrompt);

		//creates a instance of the text box based on it's name on the widget
		UTextBlock* TextBox = Cast<UTextBlock>(InteractablePrompt->GetWidgetFromName(TEXT("TextBlock_41")));

		//Considering if that text box doses exists 
		if (TextBox)
			// Sests that text box to be that of the interactable text
			TextBox->SetText(FText::FromString(InteractionText));

		//Prompts that to scene 
		InteractablePrompt->AddToPlayerScreen();

		//Starts off hidden untill see
		InteractablePrompt->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Began")));
	IsOverlapped = true;
	
	if (!callEventOnOverlap) 
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor))
		{
			if (player->GetCurrentInteraction() != this)
			{
				player->SetInteraction(this);
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Set player interact")));
			}
		}
	}
	else 
	{
		OnInteract.Broadcast();
	}

	// If no Widget attached to this compoenent 
	if (!InteractablePrompt)
		// Just prompt a debug message
		return;

	// If the it's a character as well if there is a wedgit attached to the compoenent
	if (Cast<APlayerCharacter>(OtherActor) && InteractablePrompt)
		// Shoe the UI/Widget to the screen
		InteractablePrompt->SetVisibility(ESlateVisibility::Visible);
}

void AInteractableActor::OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Ended")));
	IsOverlapped = false;
	
	if (APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor))
	{
		if (player->GetCurrentInteraction() == this) 
		{
			player->SetInteraction(nullptr);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Removed player interact")));
		}
	}

	// If there is a widget attached to this component once the player leaves the overlap . . . 
	if (InteractablePrompt)
		// Hidde the widget from scene 
		InteractablePrompt->SetVisibility(ESlateVisibility::Hidden);
}

