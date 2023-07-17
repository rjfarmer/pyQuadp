// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#include "qdef.h"


bool
PyObject_to_QuadIntObject(PyObject * in, QuadIntObject * out)
{
    char *buf;

    PyObject * obj_str = PyObject_Str(in);
    if (obj_str==NULL)
      return false;

    buf = PyBytes_AsString(obj_str);

    out->value = strtoflt128(buf, NULL);
    return true;
}

bool
PyObject_to_QuadCmplxObject(PyObject * in, QuadCmplxObject * out)
{
    char *buf;

    PyObject * obj_str = PyObject_Str(in);
    if (obj_str==NULL)
      return false;

    buf = PyBytes_AsString(obj_str);

    out->value = strtoflt128(buf, NULL);
    return true;
}

