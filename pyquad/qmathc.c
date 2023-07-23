// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <quadmath.h>
#include <stdbool.h>

#include "qfloat.h"
#include "qmathc.h"


static PyObject * _cos(PyObject *self, PyObject *args){
    QuadObject result;
    PyObject * obj = NULL;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse object");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return NULL;
    }

    result.value = cosq(result.value);

    return QuadObject_to_PyObject(result);

}


static PyMethodDef QMathMethods[] = {
    {"_cos", (PyCFunction) _cos, METH_VARARGS, "cos"},
    {NULL, NULL, 0, NULL}
};


PyModuleDef QMathModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qmathc",
    .m_doc = "Quad precision math library.",
    .m_size = -1,
    .m_methods = QMathMethods,
};

PyMODINIT_FUNC
PyInit_qmathc(void)
{

    PyObject *m;

    m = PyModule_Create(&QMathModule);
    if (m == NULL)
        return NULL;

    if (import_qfloat() < 0)
        return NULL;

    return m;
}