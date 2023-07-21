// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#pragma once

#define OP_add 1
#define OP_sub 2
#define OP_mult 3
#define OP_remainder 4
#define OP_divmod 5
#define OP_pow 6
#define OP_negative 7
#define OP_positive 8
#define OP_absolute 9
#define OP_bool 10
#define OP_invert 11
#define OP_lshift 12
#define OP_rshift 13
#define OP_and 14
#define OP_xor 15
#define OP_or 16
#define OP_int 17
//  void *nb_reserved; 18
#define OP_float 19

#define OP_inplace_add 20
#define OP_inplace_subtract 21
#define OP_inplace_multiply 22
#define OP_inplace_remainder 23
#define OP_inplace_power 24
#define OP_inplace_lshift 25
#define OP_inplace_rshift 26
#define OP_inplace_and 27
#define OP_inplace_xor 28
#define OP_inplace_or 29

#define OP_floor_divide 30
#define OP_true_divide 31
#define OP_inplace_floor_divide 32
#define OP_inplace_true_divide 33
#define OP_index 34
#define OP_matrix_multiply 35
#define OP_inplace_matrix_multiply 36

#define QUAD_BUF 128

typedef struct {
    PyObject_HEAD
    __float128 value;
} QuadObject;

PyObject* QuadObject_to_PyObject(QuadObject out);
bool PyObject_to_QuadObject(PyObject * in, QuadObject * out, const bool alloc);
void qprintf(QuadObject * out);
void alloc_QuadType(QuadObject * result);
