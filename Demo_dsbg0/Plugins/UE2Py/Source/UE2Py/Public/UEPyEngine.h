#pragma once

PyObject *UEPy_Log(PyObject *self, PyObject *args);
PyObject *UEPy_LogError(PyObject *self, PyObject *args);

PyObject *CheckWithEditor(PyObject *self, PyObject *args);

PyObject *UEPyRegisterTick(PyObject *self, PyObject *args);
PyObject *UEPyRegisterOnceTick(PyObject *self, PyObject *args);
PyObject *UEPyUnRegisterTick(PyObject *self, PyObject *args);