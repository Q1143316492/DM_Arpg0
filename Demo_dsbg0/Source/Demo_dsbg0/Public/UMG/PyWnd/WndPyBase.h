// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/WndBase.h"
#include "UMG/PyWnd/PyWidgetBase.h"
#include "UE2Py.h"
#include "WndPyBase.generated.h"

class UWndPyBase;
class UMainGameInstance;

typedef struct
{
	PyObject_HEAD
	PyObject* Owner;
	UWndPyBase* PyWnd;
} UEPy_PyWndObj;

void DEMO_DSBG0_API UEPy_InitPyWndClass();
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

	UEPy_PyUWidgetObj* GetChildPyUWidget(FString WidgetName);

	void RegisterOnClickEvent(PyObject* PyWndObj, FString UWidgetName);
private:
	TMap<FString, UEPy_PyUWidgetObj*> m_mapUWidget;
};
