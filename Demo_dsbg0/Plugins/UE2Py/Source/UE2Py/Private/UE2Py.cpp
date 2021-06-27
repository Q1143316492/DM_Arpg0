// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE2Py.h"

DEFINE_LOG_CATEGORY(LogPython);

#define LOCTEXT_NAMESPACE "FUE2PyModule"

static void SetupStdoutAndStderr()
{
	char const* code = "import sys\n"
		"import unreal_engine\n"
		"class UnrealEngineOutput:\n"
		"    def __init__(self, logger):\n"
		"        self.logger = logger\n"
		"    def write(self, buf):\n"
		"        self.logger(buf)\n"
		"    def flush(self):\n"
		"        return\n"
		"    def isatty(self):\n"
		"        return False\n"
		"sys.stdout = UnrealEngineOutput(unreal_engine.UELog)\n"
		"sys.stderr = UnrealEngineOutput(unreal_engine.UELogError)\n"
		"\n"
		"class event:\n"
		"    def __init__(self, event_signature):\n"
		"        self.event_signature = event_signature\n"
		"    def __call__(self, f):\n"
		"        f.ue_event = self.event_signature\n"
		"        return f\n"
		"\n"
		"unreal_engine.event = event";
	PyRun_SimpleString(code);
}

void FUE2PyModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("UE2Py StartupModule..."));

	Py_Initialize();

#if PLATFORM_WINDOWS
	// Restore stdio state after Py_Initialize set it to O_BINARY, otherwise
	// everything that the engine will output is going to be encoded in UTF-16.
	// The behaviour is described here: https://bugs.python.org/issue16587
	_setmode(_fileno(stdin), O_TEXT);
	_setmode(_fileno(stdout), O_TEXT);
	_setmode(_fileno(stderr), O_TEXT);

	// Also restore the user-requested UTF-8 flag if relevant (behaviour copied
	// from LaunchEngineLoop.cpp).
	if (FParse::Param(FCommandLine::Get(), TEXT("UTF8Output")))
	{
		FPlatformMisc::SetUTF8Output();
	}
#endif

	PyEval_InitThreads();

	SetupScriptPath();
	InitUnrealEnginePythonModule();
	SetupStdoutAndStderr();

	PyEval_SaveThread();
}

void FUE2PyModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("UE2Py ShutdownModule..."));

	PyGILState_Ensure();
	Py_Finalize();
}

void FUE2PyModule::SetupScriptPath()
{
	PyObject *PySys = PyImport_ImportModule("sys");
	PyObject *PySysDict = PyModule_GetDict(PySys);
	PyObject *PyPath = PyDict_GetItemString(PySysDict, "path");

	FString ProjectScriptsPath = FPaths::Combine(*FPaths::ProjectContentDir(), UTF8_TO_TCHAR("Scripts"));
	if (!FPaths::DirectoryExists(ProjectScriptsPath))
	{
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*ProjectScriptsPath);
	}
	char *ScriptsPath = TCHAR_TO_UTF8(*ProjectScriptsPath);
	PyObject *PyScriptsPath = PyUnicode_FromString(ScriptsPath);
	PyList_Insert(PyPath, 0, PyScriptsPath);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE2PyModule, UE2Py)

const char* UEPyUnicode_AsUTF8(PyObject *PyStr)
{
	return PyUnicode_AsUTF8(PyStr);
}
