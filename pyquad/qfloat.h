// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#pragma once

#ifndef Py_QFLOAT_H
#define Py_QFLOAT_H
#ifdef __cplusplus
extern "C" {
#endif

/* C API functions */
#define PyQfloat_q2py_NUM 0
#define PyQfloat_py2q_NUM 1
#define PyQfloat_alloc_NUM 2
#define PyQfloat_float128_NUM 3
#define PyQfloat_check_NUM 4
#define PyQfloat_dble_NUM 5
#define PyQfloat_f_to_dble_NUM 6

/* Total number of C API pointers */
#define PyQfloat_API_pointers 7



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

#define PICKLE_VERSION_KEY  "_pickle_version"
#define PICKLE_VERSION 1

typedef struct {
    PyObject_HEAD
    union{
    __float128 value;
    char bytes[sizeof(__float128)];
    };
} QuadObject;

#ifdef QFLOAT_MODULE

// exported

static PyObject* QuadObject_to_PyObject(QuadObject out);
static bool PyObject_to_QuadObject(PyObject * in, QuadObject * out, const bool alloc);
static void alloc_QuadType(QuadObject * result);
static __float128 QuadObject_float128(QuadObject * out);
static bool QuadObject_Check(PyObject * obj);
static double QuadObject_to_double(QuadObject * x);
static double __float128_to_double(__float128 x);

#else

static void **PyQfloat_API;

#define QuadObject_to_PyObject \
 (*(PyObject * (*)(QuadObject)) PyQfloat_API[PyQfloat_q2py_NUM])

#define PyObject_to_QuadObject \
 (*(bool (*)(PyObject *, QuadObject *, const bool)) PyQfloat_API[PyQfloat_py2q_NUM])

#define alloc_QuadType \
(*(void (*)(QuadObject *)) PyQfloat_API[PyQfloat_alloc_NUM])

#define QuadObject_float128 \
(*(__float128 (*)(QuadObject *)) PyQfloat_API[PyQfloat_float128_NUM])

#define QuadObject_Check \
(*(bool (*)(PyObject *)) PyQfloat_API[PyQfloat_check_NUM])

#define QuadObject_to_double \
(*(double (*)(QuadObject *)) PyQfloat_API[PyQfloat_dble_NUM])

#define __float128_to_double \
(*(double (*)(__float128)) PyQfloat_API[PyQfloat_f_to_dble_NUM])

/* Return -1 on error, 0 on success.
 * PyCapsule_Import will set an exception if there's an error.
 */
static int
import_qfloat(void)
{
    PyQfloat_API = (void **)PyCapsule_Import("qfloat._C_API", 0);
    return (PyQfloat_API != NULL) ? 0 : -1;
}



#endif



// end exported


#ifdef __cplusplus
}
#endif

#endif


// Debugging
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void qprintf(QuadObject * out);
#pragma GCC diagnostic pop