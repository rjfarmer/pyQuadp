// SPDX-License-Identifier: GPL-2.0+
#pragma once
#include "pyquadp.h"

#include <numpy/arrayobject.h>
#undef I

#pragma once

#ifndef Py_QFLOATArray_H
#define Py_QFLOATArray_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PyObject_HEAD
    __float128 *data;
    int nd;
    npy_intp *dimensions;
    npy_intp *strides;
    PyObject *base;
    PyArray_Descr *descr;
    int flags;
    PyObject *weakreflist;
    /* version dependent private members */
} PyQArrayObject;




#ifdef __cplusplus
}
#endif

#endif
