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
	void InitWidge(UWidgetBase *Widget, FString WndName);
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
		SubWnd->InitWidge(Widget, Name.ToString());
		SubWnd->InitLogic();
		return SubWnd;
	}

	FString GetName() { return this->m_WndName;  }
protected:
	UWidgetBase* m_UserWidget = nullptr;
	FString m_WndName;
};
