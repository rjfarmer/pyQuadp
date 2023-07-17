// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#pragma once


#define OP_ADD 1
#define OP_SUB 2
#define OP_MULT 3

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


bool PyObject_to_QuadIntObject(PyObject * in, QuadIntObject * out);

bool PyObject_to_QuadObject(PyObject * in, QuadObject * out);

bool PyObject_to_QuadCmplxObject(PyObject * in, QuadCmplxObject * out);
