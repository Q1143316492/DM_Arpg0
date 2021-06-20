// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/WndBase.h"
#include "UMG/Test//WndTestSub.h"
#include "WndTestMain.generated.h"

/**
 *
 */
UCLASS()
class DEMO_DSBG0_API UWndTestMain : public UWndBase
{
	GENERATED_BODY()
public:
	virtual void InitLogic() override;

	void HandleFunc();

	UFUNCTION()
		void OnClickTest();
private:

};
