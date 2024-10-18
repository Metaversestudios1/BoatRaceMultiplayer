// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;

UCLASS()
class BOATRACE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual bool Initialize() override;


private:
	UFUNCTION()
	void NewGameButtonClicked();

	UFUNCTION()
	void MulitplayerButtonClicked();

	UFUNCTION()
	void QuitGameButtonClicked();


	UPROPERTY(meta = (BindWidget))
	UButton* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MultiplayerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;
};
