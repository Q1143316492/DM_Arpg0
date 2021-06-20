// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include <include/Python.h>
#include <include/pystate.h>

#include <fcntl.h>

class FUEPyMgr;

typedef struct
{
	PyObject_HEAD
	UObject *ue_UObject;
	PyObject *py_PyObject;
	PyObject *py_PyDict;
} UEPy_PyUObject;

const char* UEPyUnicode_AsUTF8(PyObject *PyStr);

DECLARE_LOG_CATEGORY_EXTERN(LogPython, Log, All);

//#define UE_PY_CHECK(PyUObj) if (!UEPyMgr::Get()->IsValidPyUObject(PyUObj))\
//	return PyErr_Format(PyExc_Exception, "PyUObject is in invalid state")
//
//#define UE_PY_CHECK_RET(PyUObj) if (!UEPyMgr::Get()->IsValidPyUObject(PyUObj))\
//	{\
//		PyErr_SetString(PyExc_Exception, "PyUObject is in invalid state");\
//		return -1;\
//	}

void InitUnrealEnginePythonModule();

UEPy_PyUObject* UEPy_GetPyUObject(UObject* Uobj);

UE2PY_API void UEPyLogPyError();

UE2PY_API void CheckPyObjectGC();

class FUE2PyModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	void SetupScriptPath();
};

struct FScopePythonGIL
{
	PyGILState_STATE state;

	FScopePythonGIL()
	{
		state = PyGILState_Ensure();
	}

	~FScopePythonGIL()
	{
		PyGILState_Release(state);
	}
};