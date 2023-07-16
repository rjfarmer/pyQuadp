// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>

#include "qdef.h"


static PyObject *
QuadObject_repr(QuadObject * obj)
{
    char buf[128];

    int n = quadmath_snprintf (buf, sizeof buf, "%.36Qe", obj->value);
    if ((size_t) n < sizeof buf)
        return PyUnicode_FromFormat("qfloat('%s')",
                                buf);
    else
        return PyUnicode_FromFormat("%s","Bad quad");

}


static PyObject *
QuadObject_str(QuadObject * obj)
{
    char buf[128];

    int n = quadmath_snprintf (buf, sizeof buf, "%.36Qe", obj->value);
    if ((size_t) n < sizeof buf)
        return PyUnicode_FromFormat("%s",buf);
    else
        return PyUnicode_FromFormat("%s","Bad quad");
}



static PyMemberDef Quad_members[] = {
    {NULL}  /* Sentinel */
};


static PyMethodDef Quad_methods[] = {
    {NULL}  /* Sentinel */
};


int
Quad_init(QuadObject *self, PyObject *args, PyObject *kwds)
{
    char *buf;

    if (!PyArg_ParseTuple(args, "s:", &buf))
        return -1;

    self->value = strtoflt128(buf, NULL);

    return 0;
}


static PyTypeObject QuadType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pyquad._qfloat",
    .tp_doc = PyDoc_STR("A single quad precision variable"),
    .tp_basicsize = sizeof(QuadObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_repr = (reprfunc) QuadObject_repr,
    .tp_str = (reprfunc) QuadObject_str,
    .tp_members = Quad_members,
    .tp_methods = Quad_methods,
    .tp_init = (initproc) Quad_init,
};

static PyModuleDef QuadModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qfloat",
    .m_doc = "Quad precision module for scalar quad's.",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_qfloat(void)
{
    PyObject *m;
    if (PyType_Ready(&QuadType) < 0)
        return NULL;

    m = PyModule_Create(&QuadModule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&QuadType);
    if (PyModule_AddObject(m, "_qfloat", (PyObject *) &QuadType) < 0) {
        Py_DECREF(&QuadType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
