// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "include/Python.h"
#include "UE2Py.h"
#include "UMG/PyWnd/WndPyBase.h"
#include "UI/UEPyHUD.h"

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#else
#include "Engine/GameEngine.h"
#endif

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

