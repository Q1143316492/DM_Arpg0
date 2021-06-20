// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UEPyHUD.h"

void AUEPyHUD::InitUIMgr()
{
	if (this->m_bInit)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIMgr Has Been Init ..."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("UIMgr Init ..."));
	this->m_bInit = true;

	this->InitUMGCfgData();
}

void AUEPyHUD::InitUMGCfgData()
{
	LoadUMGCfg(TEXT("wnd_test_in_cpp"), TEXT("/Game/Res/UMG/Test/TestMainWnd.TestMainWnd_C"), 0);
}

void AUEPyHUD::LoadUMGCfg(const TCHAR* szWndName, const TCHAR* szWndPath, int32 ZOrder)
{
	this->m_mapUMGCfgData.Add(FString(szWndName), UMGData(FString(szWndName), FString(szWndPath), ZOrder));
}
