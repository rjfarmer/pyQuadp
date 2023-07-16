// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <complex.h>

#include "qdef.h"


PyMemberDef QuadCmplx_members[] = {
    {NULL}  /* Sentinel */
};


PyMethodDef QuadCmplx_methods[] = {
    {NULL}  /* Sentinel */
};


int
QuadCmplx_init(QuadObject *self, PyObject *args, PyObject *kwds)
{
    char *buf1, *buf2;

    if (!PyArg_ParseTuple(args, "ss:", &buf1, &buf2))
        return -1;

    self->value = __builtin_complex(strtoflt128(buf1, NULL), strtoflt128(buf1, NULL));

    return 0;
}


PyTypeObject QuadCmplxType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pyquad._qcmplx",
    .tp_doc = PyDoc_STR("A single quad precision complex variable"),
    .tp_basicsize = sizeof(QuadObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    // .tp_repr = (reprfunc) QuadCmplxObject_repr,
    // .tp_str = (reprfunc) QuadCmplxObject_str,
    .tp_members = QuadCmplx_members,
    .tp_methods = QuadCmplx_methods,
    .tp_init = (initproc) QuadCmplx_init,
};

PyModuleDef QuadCmplxModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qcmplx",
    .m_doc = "Quad precision complex module for scalar quad's.",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_qcmplx(void)
{
    PyObject *m;
    if (PyType_Ready(&QuadCmplxType) < 0)
        return NULL;

    m = PyModule_Create(&QuadCmplxModule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&QuadCmplxType);
    if (PyModule_AddObject(m, "_qcmplx", (PyObject *) &QuadCmplxType) < 0) {
        Py_DECREF(&QuadCmplxType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
