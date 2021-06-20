// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UE2Py.h"
#include "MainBPLibTools.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_DSBG0_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void LogicInit();
	virtual void Shutdown() override;
private:
	bool m_bInit = false;
};
