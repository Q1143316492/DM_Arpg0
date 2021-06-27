// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/WndBase.h"
#include "WndPyBase.generated.h"

/**
 *
 */
UCLASS()
class DEMO_DSBG0_API UWndPyBase : public UWndBase
{
	GENERATED_BODY()
public:
	virtual void InitLogic() override;
	
	void SetWndVisibility(int32 eSlateVisibility);
	void SetWndVisibility(ESlateVisibility InVisibility);
private:

};
