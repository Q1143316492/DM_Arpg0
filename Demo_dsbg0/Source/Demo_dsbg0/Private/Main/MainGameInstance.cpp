// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/MainGameInstance.h"

extern DEMO_DSBG0_API UMainGameInstance* MyGlobalGameInstance = nullptr;

void UMainGameInstance::LogicInit()
{
	if (this->m_bInit)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance Has not Been Init ..."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("GameInstance LogicInit..."));
	MyGlobalGameInstance = this;
	this->m_bInit = true;
	this->ExtendUEPyMethod();
}

void UMainGameInstance::ExtendUEPyMethod()
{
	UUEPyMethodExtendMgr::GetInstance()->UEPyExtendEngineMethod();

	UEPy_InitPyWndClass();
	UEPy_InitPyUWidgetClass();
}

void UMainGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init..."));
	this->LogicInit();
}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Shutdown..."));

	FScopePythonGIL gil;
	UMainBPLibTools::CppCallPythonFunction("ue_py.ue_life", "GameInstanceShutdown", nullptr);
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
			AUEPyHUD *UIMgr = Cast<AUEPyHUD>(PlayerController->GetHUD());
			if (UIMgr == nullptr)
			{
				return nullptr;
			}
			if (!UIMgr->IsInit())
			{
				UIMgr->InitUIMgr();
			}

			return UIMgr;
		}
		return nullptr;
	}
	return nullptr;
}