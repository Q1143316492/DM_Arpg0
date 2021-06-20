#include "UEPyModule.h"

PyDoc_STRVAR(UEPyDoc, "Unreal Engine Python module.");

static PyModuleDef UEPyModuleDef = {
	PyModuleDef_HEAD_INIT,
	"unreal_engine",
	UEPyDoc,
	-1,
	NULL,
};

static PyMethodDef UEPyModuleMethods[] = {
	{ "UELog", UEPy_Log , METH_VARARGS, "" },
	{ "UELogError", UEPy_LogError , METH_VARARGS, "" },

	{ "CheckWithEditor", CheckWithEditor, METH_VARARGS, ""},
	{ "RegisterTick", UEPyRegisterTick, METH_VARARGS, ""},
	{ "RegisterOnceTick", UEPyRegisterOnceTick, METH_VARARGS, ""},
	{ "UnRegisterTick", UEPyUnRegisterTick, METH_VARARGS, ""},
	{ NULL }
};

static PyMethodDef UEPyUObjectMethods[] = {

	{ NULL }
};

static void UEPy_PyUObjectDealloc(UEPy_PyUObject* self)
{
	if (self->ue_UObject && self->ue_UObject->IsValidLowLevel() && self->ue_UObject->IsRooted())
	{
		self->ue_UObject->RemoveFromRoot();
	}
	Py_XDECREF(self->py_PyDict);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* UEPy_PyUObjectCall(UEPy_PyUObject* self, PyObject* args, PyObject* kw)
{
	return PyErr_Format(PyExc_Exception, "the specified uobject has no __call__ support");
}

static PyObject* UEPy_PyUObjectStr(UEPy_PyUObject* self)
{
	return PyUnicode_FromFormat("<unreal_engine.UObject '%s' (%p) UClass '%s' (refcnt: %d)>",
		TCHAR_TO_UTF8(*self->ue_UObject->GetName()), self->ue_UObject, TCHAR_TO_UTF8(*self->ue_UObject->GetClass()->GetName()), self->ob_base.ob_refcnt);
}

static PyObject* UEPy_PyUObjectGetattro(UEPy_PyUObject* self, PyObject* attr_name)
{
	PyObject* ret = PyObject_GenericGetAttr((PyObject*)self, attr_name);
	return ret;
}

static int UEPy_PyUObjectSetattro(UEPy_PyUObject* self, PyObject* attr_name, PyObject* value)
{
	return PyObject_GenericSetAttr((PyObject*)self, attr_name, value);
}

static PyTypeObject PyUObjectType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.UObject",				/* tp_name */
	sizeof(UEPy_PyUObject),					/* tp_basicsize */
	0,										/* tp_itemsize */
	(destructor)UEPy_PyUObjectDealloc,		/* tp_dealloc */
	0,										/* tp_print */
	0,										/* tp_getattr */
	0,										/* tp_setattr */
	0,										/* tp_reserved */
	0,										/* tp_repr */
	0,										/* tp_as_number */
	0,										/* tp_as_sequence */
	0,										/* tp_as_mapping */
	0,										/* tp_hash  */
	(ternaryfunc)UEPy_PyUObjectCall,		/* tp_call */
	(reprfunc)UEPy_PyUObjectStr,			/* tp_str */
	(getattrofunc)UEPy_PyUObjectGetattro,	/* tp_getattro */
	(setattrofunc)UEPy_PyUObjectSetattro,	/* tp_setattro */
	0,										/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,						/* tp_flags */
	"Unreal Engine UObject wrapper",		/* tp_doc */
	0,										/* tp_traverse */
	0,										/* tp_clear */
	0,										/* tp_richcompare */
	0,										/* tp_weaklistoffset */
	0,										/* tp_iter */
	0,										/* tp_iternext */
	UEPyUObjectMethods,						/* tp_methods */
};

UEPy_PyUObject* UEPy_GetPyUObject(UObject* UObj)
{
	if (UObj == nullptr) { return nullptr; }

	UEPy_PyUObject* PyUObject = FUEPyMgr::GetInstance()->GetPyUObject(UObj);

	if (!PyUObject)
	{
		if (!UObj->IsValidLowLevel() || UObj->IsPendingKillOrUnreachable())
		{
			return nullptr;
		}
		PyUObject = (UEPy_PyUObject*)PyObject_New(UEPy_PyUObject, &PyUObjectType);
		PyUObject->ue_UObject = UObj;
		PyUObject->py_PyObject = nullptr;
		PyUObject->py_PyDict = PyDict_New();
		FUEPyMgr::GetInstance()->RegisterPyUObject(PyUObject);
	}
	return PyUObject;
}

static PyObject * CreateUEPyModuleHandler()
{
	PyObject* UEPyModule = PyModule_Create(&UEPyModuleDef);
	if (!UEPyModule)
	{
		return nullptr;
	}
	return UEPyModule;
}

int UEPyModuleInit(UEPy_PyUObject *self, PyObject *args, PyObject *kwds)
{
	return 0;
}

void InitUnrealEnginePythonModule()
{
	PyImport_AppendInittab("unreal_engine", &CreateUEPyModuleHandler);

	PyObject* UEPyModule = PyImport_AddModule("unreal_engine");
	PyUObjectType.tp_new = PyType_GenericNew;
	PyUObjectType.tp_init = (initproc)UEPyModuleInit;
	PyUObjectType.tp_dictoffset = offsetof(UEPy_PyUObject, py_PyDict);

	if (PyType_Ready(&PyUObjectType) < 0)
		return;

	Py_INCREF(&PyUObjectType);

	PyModule_AddObject(UEPyModule, "UObject", (PyObject*)& PyUObjectType);
	PyObject* UEPyModuleDict = PyModule_GetDict(UEPyModule);

	PyMethodDef* UEPyModuleFunc;
	for (UEPyModuleFunc = UEPyModuleMethods; UEPyModuleFunc->ml_name != NULL; UEPyModuleFunc++)
	{
		PyObject* Func = PyCFunction_New(UEPyModuleFunc, NULL);
		PyDict_SetItemString(UEPyModuleDict, UEPyModuleFunc->ml_name, Func);
		Py_DECREF(Func);
	}
}

void UEPyLogPyError()
{
	PyObject* pType = NULL;
	PyObject* pValue = NULL;
	PyObject* pTraceback = NULL;

	PyErr_Fetch(&pType, &pValue, &pTraceback);
	PyErr_NormalizeException(&pType, &pValue, &pTraceback);

	if (!pValue)
	{
		PyErr_Clear();
		return;
	}

	char* pMsg = NULL;
	PyObject * PyMsg = PyUnicode_AsUTF8String(PyObject_Str(pValue));
	if (PyMsg)
	{
		pMsg = PyBytes_AsString(PyMsg);
	}
	if (!pMsg)
	{
		PyErr_Clear();
		return;
	}

	UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(pMsg));

	if (!pTraceback)
	{
		PyErr_Clear();
		return;
	}

	PyObject* TracebackModule = PyImport_ImportModule("traceback");
	if (!TracebackModule)
	{
		PyErr_Clear();
		return;
	}

	PyObject* TracebackDict = PyModule_GetDict(TracebackModule);
	PyObject* FormatException = PyDict_GetItemString(TracebackDict, "format_exception");

	if (FormatException)
	{
		PyObject* ExceptionList = PyObject_CallFunctionObjArgs(FormatException, pType, pValue, pTraceback, NULL);
		if (!ExceptionList)
		{
			PyErr_Clear();
			return;
		}
		if (PyList_Check(ExceptionList))
		{
			for (int nIndex = 0; nIndex < PyList_Size(ExceptionList); nIndex++)
			{
				PyObject* ExceptionItem = PyList_GetItem(ExceptionList, nIndex);
				if (ExceptionItem)
				{
					UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyObject_Str(ExceptionItem))));
				}
			}
		}
		else
		{
			UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyObject_Str(ExceptionList))));
		}
	}

	PyErr_Clear();
}