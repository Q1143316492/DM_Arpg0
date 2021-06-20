#include "UEPyEngine.h"


PyObject *UEPy_Log(PyObject * self, PyObject * args)
{
	PyObject *PyMsg;
	if (!PyArg_ParseTuple(args, "O:UEPy_Log", &PyMsg))
	{
		return NULL;
	}

	PyObject *PyStrMsg = PyObject_Str(PyMsg);
	if (!PyStrMsg)
	{
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	}

	const char *Msg = UEPyUnicode_AsUTF8(PyStrMsg);
	UE_LOG(LogPython, Log, TEXT("%s"), UTF8_TO_TCHAR(Msg));
	Py_DECREF(PyStrMsg);
	Py_RETURN_NONE;
}

PyObject * UEPy_LogError(PyObject *self, PyObject *args)
{
	PyObject *PyMsg;
	if (!PyArg_ParseTuple(args, "O:UEPyLogPyError", &PyMsg))
	{
		return NULL;
	}

	PyObject *PyStrMsg = PyObject_Str(PyMsg);
	if (!PyStrMsg)
	{
		return PyErr_Format(PyExc_Exception, "argument cannot be casted to string");
	}

	const char *Msg = UEPyUnicode_AsUTF8(PyStrMsg);
	UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(Msg));
	Py_DECREF(PyStrMsg);
	Py_RETURN_NONE;
}

PyObject *CheckWithEditor(PyObject *self, PyObject *args)
{
#if WITH_EDITOR
	Py_RETURN_TRUE;
#else
	Py_RETURN_FALSE;
#endif
	Py_RETURN_NONE;
}

PyObject * UEPyRegisterTick(PyObject *self, PyObject *args)
{
	float DelayTimeMS = 0.0f;
	if (!PyArg_ParseTuple(args, "f:RegisterTick", &DelayTimeMS))
	{
		return NULL;
	}
	int32 TickID = UEPyTicker::GetInstance()->AddTick(DelayTimeMS);
	return Py_BuildValue("i", TickID);
}

PyObject * UEPyRegisterOnceTick(PyObject *self, PyObject *args)
{
	float DelayTimeMS = 0.0f;
	if (!PyArg_ParseTuple(args, "f:RegisterOnceTick", &DelayTimeMS))
	{
		return NULL;
	}
	int32 TickID = UEPyTicker::GetInstance()->AddOnceTick(DelayTimeMS);
	return Py_BuildValue("i", TickID);
}

PyObject * UEPyUnRegisterTick(PyObject *self, PyObject *args)
{
	int32 TickID = 0;
	if (!PyArg_ParseTuple(args, "i:UnRegisterTick", &TickID))
	{
		return NULL;
	}
	bool bRet = UEPyTicker::GetInstance()->RemoveTick(TickID);
	if (!bRet)
	{
		Py_RETURN_FALSE;
	}
	Py_RETURN_TRUE;
}
