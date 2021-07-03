#include "UMG/PyWnd/WndPyBase.h"
#include "Main/MainGameInstance.h"

static PyObject *ShowUpWnd(UEPy_PyWndObj *self, PyObject *args)
{
	bool bShow;
	if (!PyArg_ParseTuple(args, "b:ShowUpWnd", &bShow))
	{
		return NULL;
	}
	if (bShow)
	{
		self->PyWnd->SetWndVisibility(ESlateVisibility::Visible);
	}
	else
	{
		self->PyWnd->SetWndVisibility(ESlateVisibility::Collapsed);
	}
	Py_RETURN_NONE;
}

static PyObject *GetChildPyUWidget(UEPy_PyWndObj *self, PyObject *args)
{
	PyObject *WidgetName;
	if (!PyArg_ParseTuple(args, "O:GetChildPyUWidget", &WidgetName))
	{
		return NULL;
	}

	PyObject *PyStrWidgetName = PyObject_Str(WidgetName);
	if (!PyStrWidgetName)
	{
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	}

	UEPy_PyUWidgetObj* UWidgetObj = self->PyWnd->GetChildPyUWidget(UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyStrWidgetName)));
	if (!UWidgetObj)
	{
		Py_RETURN_NONE;
	}
	return Py_BuildValue("O", UWidgetObj);
}

static PyObject *RegisterOnClickEvent(UEPy_PyWndObj *self, PyObject *args)
{
	PyObject *UWidgetObj;
	if (!PyArg_ParseTuple(args, "O:RegisterOnClickEvent", &UWidgetObj))
	{
		return NULL;
	}

	UEPy_PyUWidgetObj* UEPyUWidgetObj = nullptr;
	if (PyUnicode_Check(UWidgetObj))
	{
		PyObject *PyStrUWidgetName = PyObject_Str(UWidgetObj);
		if (!PyStrUWidgetName)
		{
			return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
		}

		FString UWidgetName(UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyStrUWidgetName)));
		UEPyUWidgetObj = self->PyWnd->GetChildPyUWidget(UWidgetName);
		if (!UEPyUWidgetObj)
		{
			return PyErr_Format(PyExc_Exception, "uwidget not exist");
		}
	}
	else if (UEPy_CheckIsPyUWidgetType(UWidgetObj))
	{
		UEPyUWidgetObj = (UEPy_PyUWidgetObj*)UWidgetObj;
	}
	else
	{
		return PyErr_Format(PyExc_Exception, "argument must be string or PyUWidget");
	}

	self->PyWnd->RegisterOnClickEvent(self->Owner, UEPyUWidgetObj->WidgetName);

	Py_RETURN_NONE;
}

static PyMethodDef UEPy_PyWndMethods[] = {
	{"ShowUpWnd", (PyCFunction)ShowUpWnd, METH_VARARGS, "" },
	{"GetChild", (PyCFunction)GetChildPyUWidget, METH_VARARGS, "" },

	{"RegisterOnClickEvent", (PyCFunction)RegisterOnClickEvent, METH_VARARGS, "" },
	{ NULL }
};

static PyTypeObject UEPy_PyWndType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.PyWndWrapper",			/* tp_name */
	sizeof(UEPy_PyWndObj),					/* tp_basicsize */
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
	"Unreal Engine PyWndWrapper",		/* tp_doc */
	0,										/* tp_traverse */
	0,										/* tp_clear */
	0,										/* tp_richcompare */
	0,										/* tp_weaklistoffset */
	0,										/* tp_iter */
	0,										/* tp_iternext */
	UEPy_PyWndMethods,						/* tp_methods */
};

static int UEPy_UEPyWndInit(UEPy_PyWndObj *self, PyObject *args, PyObject *kwargs)
{
	PyObject* Owner;
	PyObject* WndName;
	if (!PyArg_ParseTuple(args, "OO:UEPy_UEPyWndInit", &Owner, &WndName))
	{
		return -1;
	}
	PyObject *PyStrWndName = PyObject_Str(WndName);
	if (!PyStrWndName)
	{
		return -1;
	}

	self->Owner = Owner;
	Py_INCREF(Owner);  // TODO 在 del 里面释放

	FString FStrWndName(UTF8_TO_TCHAR(UEPyUnicode_AsUTF8(PyStrWndName)));
	Py_XDECREF(PyStrWndName);

	if (!MyGlobalGameInstance->GetUIMgr()->HasCreatePyWnd(FStrWndName))
	{
		self->PyWnd = MyGlobalGameInstance->GetUIMgr()->CreatePythonWnd(FStrWndName);
	}
	else
	{
		self->PyWnd = MyGlobalGameInstance->GetUIMgr()->GetPythonWnd(FStrWndName);
	}
	self->PyWnd->SetWndVisibility(ESlateVisibility::Collapsed);
	return 0;
}

void UEPy_InitPyWndClass()
{
	FScopePythonGIL gil;
	PyObject *UEPyModule = PyImport_ImportModule("unreal_engine");

	if (UEPyModule == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UEPy_InitWndClass fail"));
		return;
	}
	UEPy_PyWndType.tp_new = PyType_GenericNew;
	UEPy_PyWndType.tp_init = (initproc)UEPy_UEPyWndInit;

	if (PyType_Ready(&UEPy_PyWndType) < 0)
	{
		return;
	}

	Py_INCREF(&UEPy_PyWndType);
	PyModule_AddObject(UEPyModule, "PyWndWrapper", (PyObject *)&UEPy_PyWndType);

}

void UWndPyBase::InitLogic()
{
	UE_LOG(LogTemp, Warning, TEXT("[WND] PyWndCreate"));
}

void UWndPyBase::SetWndVisibility(int32 eSlateVisibility)
{
	switch (eSlateVisibility)
	{
		case 1:
			this->m_UserWidget->SetVisibility(ESlateVisibility::Visible);
		case 2:
			this->m_UserWidget->SetVisibility(ESlateVisibility::Collapsed);
		case 3:
			this->m_UserWidget->SetVisibility(ESlateVisibility::Hidden);
		case 4:
			this->m_UserWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		case 5:
			this->m_UserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWndPyBase::SetWndVisibility(ESlateVisibility InVisibility)
{
	this->m_UserWidget->SetVisibility(InVisibility);
}

UEPy_PyUWidgetObj* UWndPyBase::GetChildPyUWidget(FString WidgetName)
{
	UEPy_PyUWidgetObj* Obj = *this->m_mapUWidget.Find(UWidgetName);
	if (Obj)
	{
		return Obj;
	}

	UWidget* WidgetObj = this->GetChild<UWidget>(FName(*WidgetName));
	if (!WidgetObj)
	{
		return nullptr;
	}
	UEPy_PyUWidgetObj* UWidgetObj = UEPy_NewPyUWidgetObj(WidgetName, WidgetObj);
	this->m_mapUWidget.Add(WidgetName, UWidgetObj);
	return UWidgetObj;
}

void UWndPyBase::RegisterOnClickEvent(PyObject* PyWndObj, FString UWidgetName)
{
	UEPy_PyUWidgetObj* PyUWidgetObj = *this->m_mapUWidget.find(UWidgetName);
	if (PyUWidgetObj == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("bind on click event %s error"), *UWidgetName);
		return;
	}

	UButton* CurButton = Cast<UButton>(PyUWidgetObj->UWidgetObj);
	if (CurButton)
	{
		CurButton->OnClicked.AddDynamic(PyUWidgetObj, &UEPy_PyUWidgetObj::OnClick);
		return;
	}
}

