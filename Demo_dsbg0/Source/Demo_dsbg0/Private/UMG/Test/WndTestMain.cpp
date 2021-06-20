// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/Test/WndTestMain.h"

void UWndTestMain::InitLogic()
{
	UButton* BtnTest = GetChild<UButton>("btnTest");
	BtnTest->OnClicked.AddDynamic(this, &UWndTestMain::OnClickTest);

	UWndTestSub *TestSubWnd = GetChildWnd<UWndTestSub>("TestSubWnd");
	TestSubWnd->TestFunc();
}

void UWndTestMain::HandleFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleFunc ..."));
}

void UWndTestMain::OnClickTest()
{
	UE_LOG(LogTemp, Warning, TEXT("OnClickTest ..."));
}
