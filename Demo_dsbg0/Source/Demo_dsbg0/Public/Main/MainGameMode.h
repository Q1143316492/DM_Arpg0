// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameInstance.h"
#include "UI/UEPyHUD.h"
#include "UMG/Test/WndTestMain.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_DSBG0_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMainGameMode();
	virtual void StartPlay() override;
};
