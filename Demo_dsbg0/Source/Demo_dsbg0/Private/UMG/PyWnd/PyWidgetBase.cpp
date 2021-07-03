#include "UMG/PyWnd/PyWidgetBase.h"
#include "Main/MainGameInstance.h"


static PyObject *GetWidgetBaseName(UEPy_PyUWidgetObj *self, PyObject *args)
{
	return Py_BuildValue("s", TCHAR_TO_UTF8(*self->WidgetName));
}

static PyMethodDef UEPy_PyUWidgetMethods[] = {
	{"GetName", (PyCFunction)GetWidgetBaseName, METH_VARARGS, "" },
	{ NULL }
};

static PyTypeObject UEPy_PyUWidgetType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.PyUWidget",				/* tp_name */
	sizeof(UEPy_PyUWidgetObj),				/* tp_basicsize */
	0,										/* tp_itemsize */
	0,										/* tp_dealloc */
	0,										/* tp_print */
	0,										/* tp_getattr */
	0,										/* tp_setattr */
	0,										/* tp_reserved */
	0,										/* tp_repr */
	0,										/* tp_as_number */
	0,										/* tp_as_sequence */
	0,										/* tp_as_mapping */
	0,										/* tp_hash  */
	(ternaryfunc)0,							/* tp_call */
	(reprfunc)0,							/* tp_str */
	(getattrofunc)0,						/* tp_getattro */
	(setattrofunc)0,						/* tp_setattro */
	0,										/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,						/* tp_flags */
	"Unreal Engine PyUWidget",				/* tp_doc */
	0,										/* tp_traverse */
	0,										/* tp_clear */
	0,										/* tp_richcompare */
	0,										/* tp_weaklistoffset */
	0,										/* tp_iter */
	0,										/* tp_iternext */
	UEPy_PyUWidgetMethods,					/* tp_methods */
};

UEPy_PyUWidgetObj* UEPy_NewPyUWidgetObj(FString WidgetName, UWidget* UWidgetObj)
{
	UEPy_PyUWidgetObj* obj = (UEPy_PyUWidgetObj*)PyObject_New(UEPy_PyUWidgetObj, &UEPy_PyUWidgetType);
	obj->UWidgetObj = UWidgetObj;
	obj->WidgetName = WidgetName;
	Py_INCREF(obj);
	return obj;
}

bool UEPy_CheckIsPyUWidgetType(PyObject *Obj)
{
	if (PyObject_IsInstance(Obj, (PyObject *)&UEPy_PyUWidgetType))
	{
		return true;
	}
	return false;
}

void UEPy_InitPyUWidgetClass()
{
	FScopePythonGIL gil;
	PyObject *UEPyModule = PyImport_ImportModule("unreal_engine");

	if (UEPyModule == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UEPy_InitWndClass fail"));
		return;
	}

	UEPy_PyUWidgetType.tp_new = PyType_GenericNew;

	if (PyType_Ready(&UEPy_PyUWidgetType) < 0)
	{
		return;
	}

	Py_INCREF(&UEPy_PyUWidgetType);
	PyModule_AddObject(UEPyModule, "PyUWidget", (PyObject *)&UEPy_PyUWidgetType);
}
