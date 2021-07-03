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

UWndPyBase* AUEPyHUD::GetPythonWnd(PyObject* PyObjWndName)
{
	return this->GetPythonWnd(UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyObjWndName)));
}

bool AUEPyHUD::HasCreatePyWnd(FString WndName)
{
	if (this->m_mapPyWnd.Find(WndName) == nullptr)
	{
		return false;
	}
	return true;
}

bool AUEPyHUD::HasCreatePyWnd(PyObject* PyObjWndName)
{
	return this->HasCreatePyWnd(UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyObjWndName)));
}

//UWidget* AUEPyHUD::GetChildWidget(PyObject* PyStrWndName, PyObject* PyStrChildWidgetName)
//{
//	FString WndName(UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyStrWndName)));
//	FString ChildWidgetName(UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyStrChildWidgetName)));
//
//	UWndPyBase* CurWnd = GetPythonWnd(PyStrWndName);
//	if (CurWnd == nullptr)
//	{
//		return nullptr;
//	}
//	return CurWnd->GetChildWidget(ChildWidgetName);
//}
