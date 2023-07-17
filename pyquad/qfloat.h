// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>

#pragma once

#include "qdef.h"

PyObject* QuadObject_to_PyObject(QuadObject out);
bool PyObject_to_QuadObject(PyObject * in, QuadObject * out);
void qprintf(QuadObject * out);
void alloc_QuadType(QuadObject * result);