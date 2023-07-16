// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>

#include "qdef.h"

int
PyObject_to_QuadIntObject(PyObject * in, QuadIntObject * out)
{
   // PyExc_NotImplementedError(NULL, NULL);
    return -1;
}

int
PyObject_to_QuadObject(PyObject * in, QuadObject * out)
{
   // PyExc_NotImplementedError(NULL, NULL);
    return -1;
}

int
PyObject_to_QuadCmplxObject(PyObject * in, QuadCmplxObject * out)
{
  //  PyExc_NotImplementedError(NULL, NULL);
    return -1;
}