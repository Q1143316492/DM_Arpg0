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

}

void UMainGameInstance::Shutdown()
{
	Super::Shutdown();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Shutdown..."));
/*
	FScopePythonGIL gil;
	UMainBPLibTools::CppCallPythonFunction("ue_py.ue_life", "GameInstanceShutdown", nullptr);*/

	CheckPyObjectGC();
}
