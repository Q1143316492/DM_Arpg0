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
	//LoadUMGCfg(TEXT("wnd_test_in_cpp"), TEXT("/Game/Res/UMG/Test/TestMainWnd.TestMainWnd_C"), 0);
}

void AUEPyHUD::LoadUMGCfg(const TCHAR* szWndName, const TCHAR* szWndPath, int32 ZOrder)
{
	this->m_mapUMGCfgData.Add(FString(szWndName), UMGData(FString(szWndName), FString(szWndPath), ZOrder));
}

bool AUEPyHUD::IsInit()
{
	return this->m_bInit;
}

UWndPyBase* AUEPyHUD::CreatePythonWnd(FString WndName)
{
	UWndPyBase* PyWnd = this->ShowPanel<UWndPyBase>(WndName);
	return PyWnd;
}

UWndPyBase* AUEPyHUD::GetPythonWnd(FString WndName)
{
	return Cast<UWndPyBase>(*this->m_mapPyWnd.Find(WndName));
}

bool AUEPyHUD::HasCreatePyWnd(FString WndName)
{
	if (this->m_mapPyWnd.Find(WndName) == nullptr)
	{
		return false;
	}
	return true;
}
