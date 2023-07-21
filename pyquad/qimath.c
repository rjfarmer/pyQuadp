// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <quadmath.h>


static PyMethodDef QMathMethods[] = {
    {NULL, NULL, 0, NULL}
};


static PyModuleDef QMathModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qimath",
    .m_doc = "Quad precision integer math library.",
    .m_size = -1,
    .m_methods = QMathMethods,
};

PyMODINIT_FUNC
PyInit_qimath(void)
{
    return PyModule_Create(&QMathModule);
}