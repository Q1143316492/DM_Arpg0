#pragma once


#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/WidgetBase.h"
#include "UE2Py.h"
#include "PyWidgetBase.generated.h"

class UWidgetBase;
class UMainGameInstance;

typedef struct
{
	PyObject_HEAD
	UWidget* UWidgetObj;
	FString WidgetName;
	
	void OnClick()
	{
		//this->WidgetName;
	}

} UEPy_PyUWidgetObj;

UEPy_PyUWidgetObj* UEPy_NewPyUWidgetObj(FString WidgetName, UWidget* UWidgetObj);
bool UEPy_CheckIsPyUWidgetType(PyObject *Obj);
void DEMO_DSBG0_API UEPy_InitPyUWidgetClass();
/**
 *
 */
UCLASS()
class DEMO_DSBG0_API UPyWidgetBase : public UWidgetBase
{
	GENERATED_BODY()
public:

private:
};
