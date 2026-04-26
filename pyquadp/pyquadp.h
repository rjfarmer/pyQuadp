// SPDX-License-Identifier: GPL-2.0+

#pragma once
#define PY_SSIZE_T_CLEAN
#define Py_LIMITED_API 0x030A0000
#include <Python.h>
#include <structmember.h> // Removed in 3.12

#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <complex.h> 
#include <float.h>
#include <strings.h>

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

