// Fill out your copyright notice in the Description page of Project Settings.


#include "PyActor.h"


APyActor::APyActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

APyActor::~APyActor()
{
	if (PyActorInstance)
	{
		FScopePythonGIL gil;
		FUEPyMgr::GetInstance()->UnRegisterPyUObject(this->PyUObject);
	}
}

void APyActor::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	this->bEnable = false;

	if (PythonModule.IsEmpty() || PythonClass.IsEmpty())
	{
		return;
	}

	FScopePythonGIL gil;
	PyUObject = UEPy_GetPyUObject(this);
	if (!PyUObject)
	{
		UEPyLogPyError();
		return;
	}

	PyObject *PyActorModule = PyImport_ImportModule(TCHAR_TO_UTF8(*PythonModule));
	if (!PyActorModule)
	{
		UEPyLogPyError();
		return;
	}

#if WITH_EDITOR
	PyActorModule = PyImport_ReloadModule(PyActorModule);
	if (!PyActorModule)
	{
		UEPyLogPyError();
		return;
	}
#endif

	PyObject* PyActorModuleDict = PyModule_GetDict(PyActorModule);
	PyObject* PyActorClass = PyDict_GetItemString(PyActorModuleDict, TCHAR_TO_UTF8(*PythonClass));
	if (!PyActorClass)
	{
		UEPyLogPyError();
		return;
	}

	PyActorInstance = PyObject_CallObject(PyActorClass, NULL);
	if (!PyActorInstance)
	{
		UEPyLogPyError();
		return;
	}
	PyUObject->py_PyObject = PyActorInstance;
	PyObject_SetAttrString(PyActorInstance, (char*)"uobject", (PyObject *)PyUObject);

	if (!PyObject_HasAttrString(PyActorInstance, (char *)"Tick"))
	{
		SetActorTickEnabled(false);
	}
	this->bEnable = true;

	if (!PyObject_HasAttrString(PyActorInstance, (char *)"PreInitComponents"))
	{
		return;
	}
	PyObject *bRet = PyObject_CallMethod(PyActorInstance, (char *)"PreInitComponents", NULL);
	if (!bRet)
	{
		UEPyLogPyError();
		return;
	}
	Py_DECREF(bRet);
}

void APyActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!bEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("PostInitializeComponents PyActor not init"));
		return;
	}

	FScopePythonGIL gil;
	if (!PyObject_HasAttrString(PyActorInstance, (char *)"PostInitComponents"))
		return;

	PyObject *bRet = PyObject_CallMethod(PyActorInstance, (char *)"PostInitComponents", NULL);
	if (!bRet)
	{
		UEPyLogPyError();
		return;
	}
	Py_DECREF(bRet);
}

void APyActor::BeginPlay()
{
	Super::BeginPlay();

	if (!bEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay PyActor not init"));
		return;
	}

	FScopePythonGIL gil;
	if (!PyObject_HasAttrString(PyActorInstance, (char *)"BeginPlay"))
	{
		return;
	}

	PyObject *bRet = PyObject_CallMethod(PyActorInstance, (char *)"BeginPlay", NULL);
	if (!bRet)
	{
		UEPyLogPyError();
		return;
	}
	Py_DECREF(bRet);
	
}

void APyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!bEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndPlay PyActor not init"));
		return;
	}

	FScopePythonGIL gil;
	if (bEnable && PyObject_HasAttrString(PyActorInstance, (char *)"EndPlay"))
	{
		PyObject *bRet = PyObject_CallMethod(PyActorInstance, (char *)"EndPlay", (char *)"i", EndPlayReason);
		if (!bRet)
		{
			UEPyLogPyError();
		}
		Py_DECREF(bRet);
	}
	Super::EndPlay(EndPlayReason);
}

void APyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bEnable)
	{
		return;
	}

	FScopePythonGIL gil;
	if (!PyObject_HasAttrString(PyActorInstance, (char *)"Tick"))
	{
		return;
	}

	PyObject *bRet = PyObject_CallMethod(PyActorInstance, (char *)"Tick", (char *)"f", DeltaTime);
	if (!bRet)
	{
		UEPyLogPyError();
		return;
	}
	Py_DECREF(bRet);

}

