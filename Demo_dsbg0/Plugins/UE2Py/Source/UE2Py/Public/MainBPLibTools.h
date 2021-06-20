// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UE2Py.h"
#include "MainBPLibTools.generated.h"

/**
 * 
 */
UCLASS()
class UE2PY_API UMainBPLibTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void TestFunc();

	static void CppCallPythonParmParse() { }

	template<typename FirstType, typename... Args>
	static void CppCallPythonParmParse(FirstType FirstPyParam, Args... OtherPyParams)
	{
		
	}

	template<typename ReturnType, typename... Args>
	static void CppCallPythonFunction(ReturnType *Ret, const char* ModuleName, const char* FunctionName, const char* ParseFmt, Args... PyParams)
	{
		FScopePythonGIL gil;
		PyObject* PyModule = PyImport_ImportModule(ModuleName);
		if (!PyModule)
		{
			UEPyLogPyError();
			return; 
		}
		PyObject* PyModuleDict = PyModule_GetDict(PyModule);
		if (!PyModuleDict)
		{
			UEPyLogPyError();
			return;
		}

		PyObject* Func = PyDict_GetItemString(PyModuleDict, FunctionName);
		if (Func && PyCallable_Check(Func))
		{
			PyObject* FuncRet = PyObject_CallFunction(Func, ParseFmt, PyParams...);
			Py_XDECREF(FuncRet);
			Py_XDECREF(Func);
		}
		Py_DECREF(PyModule);
	}

	template<typename... Args>
	static void CppCallPythonFunction(const char* ModuleName, const char* FunctionName, const char* ParseFmt, Args... PyParams)
	{
		FScopePythonGIL gil;
		PyObject* PyModule = PyImport_ImportModule(ModuleName);
		if (!PyModule)
		{
			UEPyLogPyError();
			return;
		}
		PyObject* PyModuleDict = PyModule_GetDict(PyModule);
		if (!PyModuleDict)
		{
			UEPyLogPyError();
			return;
		}

		PyObject* Func = PyDict_GetItemString(PyModuleDict, FunctionName);
		if (Func && PyCallable_Check(Func))
		{
			PyObject* FuncRet = PyObject_CallFunction(Func, ParseFmt, PyParams...);
			Py_XDECREF(FuncRet);
		}
		Py_DECREF(PyModule);
	}
private:
};
