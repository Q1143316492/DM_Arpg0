// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/MainGameInstance.h"

void UMainGameInstance::LogicInit()
{
	if (this->m_bInit)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance Has not Been Init ..."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("GameInstance LogicInit..."));
	
	this->m_bInit = true;
	//this->GetUIMgr()->InitUIMgr();
}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Shutdown..."));

	//FScopePythonGIL gil;
	//UMainBPLibTools::CppCallPythonFunction("ue_py.ue_life", "GameInstanceShutdown", nullptr);

	//CheckPyObjectGC();
}


AUEPyHUD* UMainGameInstance::GetUIMgr()
{
	if (!this->m_bInit)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance Has not Been Init ..."));
		return nullptr;
	}

	if (GetWorld())
	{
		APlayerController *PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			return Cast<AUEPyHUD>(PlayerController->GetHUD());
		}
		return nullptr;
	}
	return nullptr;
}