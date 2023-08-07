// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "qfloat.h"

#pragma once

#ifndef Py_QCMPLX_H
#define Py_QCMPLX_H
#ifdef __cplusplus
extern "C" {
#endif

/* C API functions */
#define PyQcmplx_q2py_NUM 0
#define PyQcmplx_py2q_NUM 1
#define PyQcmplx_alloc_NUM 2
#define PyQcmplx_to_c128_NUM 3

/* Total number of C API pointers */
#define PyQcmplx_API_pointers 4

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

// exported
typedef struct {
    PyObject_HEAD
    union{
    __complex128 value;
    char bytes[sizeof(__complex128)];
    };
} QuadCObject;




#ifdef QCMPLX_MODULE

static PyObject* QuadCObject_to_PyObject(QuadCObject out);
static bool PyObject_to_QuadCObject(PyObject * in, QuadCObject * out, const bool alloc);
static bool PyObject_to_QuadCObject2(PyObject * in1,PyObject * in2, QuadCObject * out, const bool alloc);
static void alloc_QuadCType(QuadCObject * result);
static __complex128 QuadCObject_complex128(QuadCObject * out);
void QuadCObject_to_doubles(QuadCObject c, double *real, double *imag);

#else

static void **PyQcmplx_API;

#define QuadCObject_to_PyObject \
 (*(PyObject * (*)(QuadCObject)) PyQcmplx_API[PyQcmplx_q2py_NUM])

#define PyObject_to_QuadCObject \
 (*(bool (*)(PyObject *, QuadCObject *, const bool)) PyQcmplx_API[PyQcmplx_py2q_NUM])

#define alloc_QuadCType \
(*(void (*)(QuadCObject *)) PyQcmplx_API[PyQcmplx_alloc_NUM])

#define QuadCObject_complex128 \
(*( __complex128 (*)(QuadCObject *)) PyQcmplx_API[PyQcmplx_to_c128_NUM])

/* Return -1 on error, 0 on success.
 * PyCapsule_Import will set an exception if there's an error.
 */
static int
import_qcmplx(void)
{
    PyQcmplx_API = (void **)PyCapsule_Import("qcmplx._C_API", 0);
    return (PyQcmplx_API != NULL) ? 0 : -1;
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
void qcprintf(QuadCObject * out);
#pragma GCC diagnostic pop