// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/Test/WndTestSub.h"

void UWndTestSub::TestFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("WndTestSub TestFunc ..."));

	UTextBlock* TxtTest = GetChild<UTextBlock>("txtNote");
	TxtTest->SetText(NSLOCTEXT("Namespace", "set text key", "测试set text"));
}
