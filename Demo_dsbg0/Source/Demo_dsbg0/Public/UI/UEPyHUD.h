// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/WndBase.h"
#include "UI/WidgetBase.h"
#include "UMG/PyWnd/WndPyBase.h"
#include "UEPyHUD.generated.h"


struct UMGData
{
	FString WndName;
	FString UMGPath;
	int32 ZOrder;

	UMGData(FString WndName, FString UMGPath, int32 ZOrder) : WndName(WndName), UMGPath(UMGPath), ZOrder(ZOrder) {}
};

/**
 * 
 */
UCLASS()
class DEMO_DSBG0_API AUEPyHUD : public AHUD
{
	GENERATED_BODY()
public:
	void InitUIMgr();
	void InitUMGCfgData();
	void LoadUMGCfg(const TCHAR* szWndName, const TCHAR* szWndPath, int32 ZOrder);

	template<typename WndType>
	WndType* ShowPanel(FString WndName)
	{
		if (!this->m_bInit)
		{
			UE_LOG(LogTemp, Warning, TEXT("UIMgr not init"));
			return nullptr;
		}

		if (!this->m_mapUMGCfgData.Contains(WndName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Wnd (%s) not found"), *WndName);
			return nullptr;
		}

		UMGData Data = this->m_mapUMGCfgData[WndName];
		TSubclassOf<UWidgetBase> WidgeClass = LoadClass<UWidgetBase>(NULL, *Data.UMGPath);
		if (WidgeClass == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UIMgr LoadClass (%s) Fail ..."), *WndName);
			return nullptr;
		}

		UWidgetBase* Widget = CreateWidget<UWidgetBase>(GetWorld(), WidgeClass);
		if (Widget == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UIMgr CreateWidget (%s) Fail ..."), *WndName);
			return nullptr;
		}

		WndType* CreateWnd = NewObject<WndType>();
		if (CreateWnd == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UIMgr Cast Wnd Class Fail (%s) ..."), *WndName);
			return nullptr;
		}
		CreateWnd->InitWidge(Widget);
		CreateWnd->InitLogic();

		Widget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("UIMgr CreateWidget (%s) Success ..."), *WndName);
		return CreateWnd;
	}

	bool IsInit();
	UWndPyBase* CreatePythonWnd(FString WndName);
	UWndPyBase* GetPythonWnd(FString WndName);
	bool HasCreatePyWnd(FString WndName);
private:
	bool m_bInit = false;
	TMap<FString, UMGData> m_mapUMGCfgData;
	TMap<FString, UWndBase*> m_mapPyWnd;
};
