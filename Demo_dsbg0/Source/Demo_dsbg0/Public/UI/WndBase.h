// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UI/WidgetBase.h"
#include "WndBase.generated.h"

/**
 *
 */
UCLASS()
class DEMO_DSBG0_API UWndBase : public UObject
{
	GENERATED_BODY()
public:
	void InitWidge(UWidgetBase *Widget);
	UWidgetBase* GetWidget();

	virtual void InitLogic();
protected:
	template <typename UWidgetType>
	UWidgetType* GetChild(const FName &Name)
	{
		return Cast<UWidgetType>(this->m_UserWidget->GetWidgetFromName(Name));
	}

	template <typename SubWndType>
	SubWndType* GetChildWnd(const FName &Name)
	{
		UWidgetBase *Widget = Cast<UWidgetBase>(this->m_UserWidget->GetWidgetFromName(Name));
		if (Widget == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Get SubWnd (%s) Cast Fail"), *Name.ToString());
			return nullptr;
		}
		SubWndType *SubWnd = NewObject<SubWndType>();
		SubWnd->InitWidge(Widget);
		SubWnd->InitLogic();
		return SubWnd;
	}

protected:
	UWidgetBase* m_UserWidget = nullptr;
};
