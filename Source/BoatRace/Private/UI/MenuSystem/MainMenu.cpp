// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuSystem/MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	 
	if (NewGameButton)
	{
		NewGameButton->OnClicked.AddDynamic(this, &ThisClass::NewGameButtonClicked);
	}
	if (MultiplayerButton)
	{
		MultiplayerButton->OnClicked.AddDynamic(this, &ThisClass::MulitplayerButtonClicked);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::QuitGameButtonClicked);
	}

	return true;
}

void UMainMenu::NewGameButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString(TEXT("New Game C++ logic")));
	}
	UGameplayStatics::OpenLevel(this, FName("MapSelection"));
}

void UMainMenu::MulitplayerButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(TEXT("Multiplayer BP Logic")));
	}
}

void UMainMenu::QuitGameButtonClicked()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, true);
	}
}
