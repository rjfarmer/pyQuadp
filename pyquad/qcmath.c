// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <quadmath.h>


static PyMethodDef QMathMethods[] = {
    {NULL, NULL, 0, NULL}
};


PyModuleDef QMathModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qcmath",
    .m_doc = "Quad precision complex math library.",
    .m_size = -1,
    .m_methods = QMathMethods,
};

PyMODINIT_FUNC
PyInit_qcmath(void)
{
    return PyModule_Create(&QMathModule);
}