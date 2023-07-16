// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>

#pragma once

typedef struct {
    PyObject_HEAD
    __int128 value;
} QuadIntObject;

typedef struct {
    PyObject_HEAD
    __float128 value;
} QuadObject;

typedef struct {
    PyObject_HEAD
    __complex128 value;
} QuadCmplxObject;


int PyObject_to_QuadIntObject(PyObject * in, QuadIntObject * out);

int PyObject_to_QuadObject(PyObject * in, QuadObject * out);

int PyObject_to_QuadCmplxObject(PyObject * in, QuadCmplxObject * out);
