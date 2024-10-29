// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "GameFramework/PlayerState.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		if (GEngine)
		{
			FString PlayerName = PlayerState->GetPlayerName();
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green, *PlayerName);
		}
	}
}
