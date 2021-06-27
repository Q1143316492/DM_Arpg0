// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/UEPyMethodExtendMgr.h"
#include "Main/MainGameInstance.h"

PyObject * UETestExtendMethod(PyObject * self, PyObject * args)
{
	Py_RETURN_TRUE;
}

PyObject * AddWndClassInfo(PyObject * self, PyObject * args)
{
	PyObject * WndName;
	int32 WndLayer;
	int32 ZOrder;
	PyObject * BlueprintPath;
	if (!PyArg_ParseTuple(args, "OiiO:AddWndClassInfo", &WndName, &WndLayer, &ZOrder, &BlueprintPath))
	{
		return NULL;
	}
	PyObject *PyStrWndName = PyObject_Str(WndName);
	PyObject *PyStrBlueprintPath = PyObject_Str(BlueprintPath);
	
	if (!PyStrWndName || !BlueprintPath)
	{
		Py_XDECREF(PyStrWndName);
		Py_XDECREF(PyStrBlueprintPath);
		return PyErr_Format(PyExc_Exception, "wnd name cannot be casted to string");
	}

	if (!MyGlobalGameInstance)
	{
		return PyErr_Format(PyExc_Exception, "GameInstance None");
	}

	MyGlobalGameInstance->GetUIMgr()->LoadUMGCfg(UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyStrWndName)),
		UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyStrBlueprintPath)), ZOrder);

	Py_RETURN_NONE;
}

PyObject * SetWndVisibility(PyObject * self, PyObject * args)
{
	PyObject* WndName;
	int32 VisibilityEnum;
	if (!PyArg_ParseTuple(args, "Oi:SetWndVisibility", &WndName, &VisibilityEnum))
	{
		return NULL;
	}

	PyObject *PyStrWndName = PyObject_Str(WndName);
	if (!PyStrWndName)
	{
		return PyErr_Format(PyExc_Exception, "wnd name cannot be casted to string");
	}

	if (!MyGlobalGameInstance)
	{
		return PyErr_Format(PyExc_Exception, "GameInstance None");
	}
	UWndPyBase* PyWnd = nullptr;
	FString FStrWndName(UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyStrWndName)));
	if (!MyGlobalGameInstance->GetUIMgr()->HasCreatePyWnd(FStrWndName))
	{
		PyWnd = MyGlobalGameInstance->GetUIMgr()->CreatePythonWnd(FStrWndName);
	}
	else
	{
		PyWnd = MyGlobalGameInstance->GetUIMgr()->GetPythonWnd(FStrWndName);
	}
	PyWnd->SetWndVisibility(ESlateVisibility::Visible);

	Py_RETURN_NONE;
}


static PyMethodDef UEPyModuleExtendMethods[] = {
	{"UETestExtendMethod", UETestExtendMethod, METH_VARARGS, "" },

	// UI
	{"AddWndClassInfo", AddWndClassInfo, METH_VARARGS, "" },
	{"SetWndVisibility", SetWndVisibility, METH_VARARGS, "" },
	{ NULL }
};

UUEPyMethodExtendMgr* UUEPyMethodExtendMgr::GetInstance()
{
	static UUEPyMethodExtendMgr *Singleton;
	if (!Singleton)
	{
		Singleton = NewObject<UUEPyMethodExtendMgr>();
	}
	return Singleton;
}

void UUEPyMethodExtendMgr::UEPyExtendEngineMethod()
{
	FScopePythonGIL gil;

	PyObject *UEPyModule = PyImport_ImportModule("unreal_engine");

	if (UEPyModule == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UEPyExtendMethod fail"));
		return;
	}

	PyObject* UEPyModuleDict = PyModule_GetDict(UEPyModule);
	PyMethodDef* UEPyModuleFunc;
	for (UEPyModuleFunc = UEPyModuleExtendMethods; UEPyModuleFunc->ml_name != NULL; UEPyModuleFunc++)
	{
		PyObject* Func = PyCFunction_New(UEPyModuleFunc, NULL);
		PyDict_SetItemString(UEPyModuleDict, UEPyModuleFunc->ml_name, Func);
		Py_DECREF(Func);
	}
}
