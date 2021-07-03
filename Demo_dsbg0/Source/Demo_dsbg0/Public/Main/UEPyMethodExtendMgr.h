// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UE2Py.h"
#include "UMG/PyWnd/WndPyBase.h"
#include "UI/UEPyHUD.h"
#include "UEPyMethodExtendMgr.generated.h"


class UMainGameInstance;


/**
 * 
 */
UCLASS()
class DEMO_DSBG0_API UUEPyMethodExtendMgr : public UObject
{
	GENERATED_BODY()
public:
	static UUEPyMethodExtendMgr* GetInstance();

	void UEPyExtendEngineMethod();
};

