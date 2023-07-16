// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>

#include "qdef.h"


PyObject *
QuadIntObject_repr(QuadObject * obj)
{
    char buf[128];

    int n = quadmath_snprintf (buf, sizeof buf, "%.36Qe", obj->value);
    if ((size_t) n < sizeof buf)
        return PyUnicode_FromFormat("qfloat('%s')",
                                buf);
    else
        return PyUnicode_FromFormat("%s","Bad quad");

}


PyObject *
QuadIntObject_str(QuadObject * obj)
{
    char buf[128];

    int n = quadmath_snprintf (buf, sizeof buf, "%.36Qe", obj->value);
    if ((size_t) n < sizeof buf)
        return PyUnicode_FromFormat("%s",buf);
    else
        return PyUnicode_FromFormat("%s","Bad quad");
}



PyMemberDef QuadInt_members[] = {
    {NULL}  /* Sentinel */
};


PyMethodDef QuadInt_methods[] = {
    {NULL}  /* Sentinel */
};


int
QuadInt_init(QuadObject *self, PyObject *args, PyObject *kwds)
{
    char *buf;

    if (!PyArg_ParseTuple(args, "s:", &buf))
        return -1;

    self->value = strtoflt128(buf, NULL);

    return 0;
}


PyTypeObject QuadIntType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pyquad._qint",
    .tp_doc = PyDoc_STR("A single quad precision variable"),
    .tp_basicsize = sizeof(QuadObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_repr = (reprfunc) QuadIntObject_repr,
    .tp_str = (reprfunc) QuadIntObject_str,
    .tp_members = QuadInt_members,
    .tp_methods = QuadInt_methods,
    .tp_init = (initproc) QuadInt_init,
};

PyModuleDef QuadIntModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qint",
    .m_doc = "Quad precision integer module for scalar quad's.",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_qint(void)
{
    PyObject *m;
    if (PyType_Ready(&QuadIntType) < 0)
        return NULL;

    m = PyModule_Create(&QuadIntModule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&QuadIntType);
    if (PyModule_AddObject(m, "_qint", (PyObject *) &QuadIntType) < 0) {
        Py_DECREF(&QuadIntType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
