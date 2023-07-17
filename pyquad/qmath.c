// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <quadmath.h>

#include "qdef.h"

// PyObject * qfloatmod = PyImport_ImportModule("qfloat");

static PyObject *_cos(PyObject *self, PyObject *args){
    // QuadObject *q;
    // __float128 r;
    // char *buf;

    // if (!PyArg_ParseTuple(args, "s:", &buf)){
    //     Py_DECREF(&q);
    //     return NULL;
    // }

    // r = strtoflt128(buf, NULL);

    // q->value = cosq(r);

    // return (PyObject*)q;
    return NULL;
}


static PyMethodDef QMathMethods[] = {
    {"cos", (PyCFunction) _cos, METH_VARARGS, "cos"},
    {NULL, NULL, 0, NULL}
};


PyModuleDef QMathModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qmath",
    .m_doc = "Quad precision math library.",
    .m_size = -1,
    .m_methods = QMathMethods,
};

PyMODINIT_FUNC
PyInit_qmath(void)
{

    PyObject *m;

    m = PyModule_Create(&QMathModule);
    if (m == NULL)
        return NULL;

    return m;
}