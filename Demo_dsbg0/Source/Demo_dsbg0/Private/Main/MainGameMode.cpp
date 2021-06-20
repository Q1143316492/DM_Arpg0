// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/MainGameMode.h"

AMainGameMode::AMainGameMode()
{
	HUDClass = AUEPyHUD::StaticClass();
}

void AMainGameMode::StartPlay()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode StartPlay GetWorld None"));
		return;
	}
	
	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode StartPlay Cast GameInstance fail"));
		return;
	}
	GameInstance->LogicInit();

	// 测试 UMG C++
	//UWndTestMain* WndTest = GameInstance->GetUIMgr()->ShowPanel<UWndTestMain>(TEXT("wnd_test_in_cpp"));
	//WndTest->HandleFunc();
}
