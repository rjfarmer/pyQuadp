// SPDX-License-Identifier: GPL-2.0+

#define NPY_TARGET_VERSION NPY_2_0_API_VERSION
#define NPY_NO_DEPRECATED_API NPY_2_0_API_VERSION

#include "pyquadp.h"

#include <numpy/arrayobject.h>
#include <numpy/ufuncobject.h>
#include <limits.h>
#include <stdalign.h>
#include <string.h>

#define QIARRAY_MODULE
#include "qiarray.h"
#include "qint.h"

static int QuadIArrayTypeNum = -1;

static PyArray_ArrFuncs QuadIArrayFuncs;
static PyArray_Descr *QuadIArrayDescr;
static PyArray_DescrProto QuadIArrayDescrProto = {PyObject_HEAD_INIT(NULL)};

static int QuadIArray_setitem(PyObject *item, __int128 *data, void *array);

#define QIARRAY_SHIFT_BITS ((int)(sizeof(__int128) * CHAR_BIT))

static int
qiarray_validate_shift(__int128 shift)
{
  if (shift < 0 || shift >= (__int128)QIARRAY_SHIFT_BITS) {
    PyErr_SetString(PyExc_ValueError, "shift count out of range");
    return -1;
  }

  return 0;
}

static int
qiarray_floor_divmod(__int128 lhs, __int128 rhs, __int128 *quotient, __int128 *remainder)
{
  __int128 q;
  __int128 r;

  if (rhs == 0) {
    PyErr_SetString(PyExc_ZeroDivisionError, "division by zero");
    return -1;
  }

  if (rhs == -1 && (unsigned __int128)lhs == (((unsigned __int128)1) << (QIARRAY_SHIFT_BITS - 1))) {
    PyErr_SetString(PyExc_OverflowError, "int128 division overflow");
    return -1;
  }

  q = lhs / rhs;
  r = lhs % rhs;
  if (r != 0 && ((r > 0) != (rhs > 0))) {
    q -= 1;
    r += rhs;
  }

  *quotient = q;
  *remainder = r;
  return 0;
}

#define QIARRAY_FIXED_INTEGER_TYPES(X) \
  X(i8, npy_int8, NPY_INT8) \
  X(i16, npy_int16, NPY_INT16) \
  X(i32, npy_int32, NPY_INT32) \
  X(i64, npy_int64, NPY_INT64)

#define QIARRAY_DEFINE_MIXED_BINARY_OP(name, suffix, ctype, op) \
static void \
QuadIArray_ufunc_##name##_q##suffix(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *inq = args[0]; \
  char *ini = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    *(__int128 *)out = *(__int128 *)inq op (__int128)(*(ctype *)ini); \
    inq += steps[0]; \
    ini += steps[1]; \
    out += steps[2]; \
  } \
} \
\
static void \
QuadIArray_ufunc_##name##_##suffix##q(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *ini = args[0]; \
  char *inq = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    *(__int128 *)out = (__int128)(*(ctype *)ini) op *(__int128 *)inq; \
    ini += steps[0]; \
    inq += steps[1]; \
    out += steps[2]; \
  } \
}

#define QIARRAY_DEFINE_DIVMOD_OPS(suffix, ctype, npy_type) \
static void \
QuadIArray_ufunc_floor_divide_q##suffix(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *inq = args[0]; \
  char *ini = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    __int128 quotient; \
    __int128 remainder; \
    if (qiarray_floor_divmod(*(__int128 *)inq, (__int128)(*(ctype *)ini), &quotient, &remainder) < 0) { \
      return; \
    } \
    *(__int128 *)out = quotient; \
    inq += steps[0]; \
    ini += steps[1]; \
    out += steps[2]; \
  } \
} \
\
static void \
QuadIArray_ufunc_floor_divide_##suffix##q(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *ini = args[0]; \
  char *inq = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    __int128 quotient; \
    __int128 remainder; \
    if (qiarray_floor_divmod((__int128)(*(ctype *)ini), *(__int128 *)inq, &quotient, &remainder) < 0) { \
      return; \
    } \
    *(__int128 *)out = quotient; \
    ini += steps[0]; \
    inq += steps[1]; \
    out += steps[2]; \
  } \
} \
\
static void \
QuadIArray_ufunc_remainder_q##suffix(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *inq = args[0]; \
  char *ini = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    __int128 quotient; \
    __int128 remainder; \
    if (qiarray_floor_divmod(*(__int128 *)inq, (__int128)(*(ctype *)ini), &quotient, &remainder) < 0) { \
      return; \
    } \
    *(__int128 *)out = remainder; \
    inq += steps[0]; \
    ini += steps[1]; \
    out += steps[2]; \
  } \
} \
\
static void \
QuadIArray_ufunc_remainder_##suffix##q(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *ini = args[0]; \
  char *inq = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    __int128 quotient; \
    __int128 remainder; \
    if (qiarray_floor_divmod((__int128)(*(ctype *)ini), *(__int128 *)inq, &quotient, &remainder) < 0) { \
      return; \
    } \
    *(__int128 *)out = remainder; \
    ini += steps[0]; \
    inq += steps[1]; \
    out += steps[2]; \
  } \
}

#define QIARRAY_DEFINE_SHIFT_OPS(suffix, ctype, npy_type) \
static void \
QuadIArray_ufunc_left_shift_q##suffix(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *inq = args[0]; \
  char *ini = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    __int128 shift = (__int128)(*(ctype *)ini); \
    if (qiarray_validate_shift(shift) < 0) { \
      return; \
    } \
    *(__int128 *)out = *(__int128 *)inq << (unsigned int)shift; \
    inq += steps[0]; \
    ini += steps[1]; \
    out += steps[2]; \
  } \
} \
\
static void \
QuadIArray_ufunc_left_shift_##suffix##q(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *ini = args[0]; \
  char *inq = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    __int128 shift = *(__int128 *)inq; \
    if (qiarray_validate_shift(shift) < 0) { \
      return; \
    } \
    *(__int128 *)out = (__int128)(*(ctype *)ini) << (unsigned int)shift; \
    ini += steps[0]; \
    inq += steps[1]; \
    out += steps[2]; \
  } \
} \
\
static void \
QuadIArray_ufunc_right_shift_q##suffix(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *inq = args[0]; \
  char *ini = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    __int128 shift = (__int128)(*(ctype *)ini); \
    if (qiarray_validate_shift(shift) < 0) { \
      return; \
    } \
    *(__int128 *)out = *(__int128 *)inq >> (unsigned int)shift; \
    inq += steps[0]; \
    ini += steps[1]; \
    out += steps[2]; \
  } \
} \
\
static void \
QuadIArray_ufunc_right_shift_##suffix##q(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data)) \
{ \
  npy_intp i; \
  npy_intp n = dims[0]; \
  char *ini = args[0]; \
  char *inq = args[1]; \
  char *out = args[2]; \
\
  for (i = 0; i < n; ++i) { \
    __int128 shift = *(__int128 *)inq; \
    if (qiarray_validate_shift(shift) < 0) { \
      return; \
    } \
    *(__int128 *)out = (__int128)(*(ctype *)ini) >> (unsigned int)shift; \
    ini += steps[0]; \
    inq += steps[1]; \
    out += steps[2]; \
  } \
}

#define QIARRAY_DEFINE_CAST_FUNCS(suffix, ctype, npy_type) \
static void \
QuadIArray_cast_to_##suffix(void *from, void *to, npy_intp n, void *NPY_UNUSED(fromarr), void *NPY_UNUSED(toarr)) \
{ \
  npy_intp i; \
  __int128 *src = (__int128 *)from; \
  ctype *dst = (ctype *)to; \
\
  for (i = 0; i < n; ++i) { \
    dst[i] = (ctype)src[i]; \
  } \
} \
\
static void \
QuadIArray_cast_from_##suffix(void *from, void *to, npy_intp n, void *NPY_UNUSED(fromarr), void *NPY_UNUSED(toarr)) \
{ \
  npy_intp i; \
  ctype *src = (ctype *)from; \
  __int128 *dst = (__int128 *)to; \
\
  for (i = 0; i < n; ++i) { \
    dst[i] = (__int128)src[i]; \
  } \
}

#define QIARRAY_DEFINE_ADD_TYPE(suffix, ctype, npy_type) \
  QIARRAY_DEFINE_MIXED_BINARY_OP(add, suffix, ctype, +)

#define QIARRAY_DEFINE_SUBTRACT_TYPE(suffix, ctype, npy_type) \
  QIARRAY_DEFINE_MIXED_BINARY_OP(subtract, suffix, ctype, -)

#define QIARRAY_DEFINE_MULTIPLY_TYPE(suffix, ctype, npy_type) \
  QIARRAY_DEFINE_MIXED_BINARY_OP(multiply, suffix, ctype, *)

#define QIARRAY_DEFINE_AND_TYPE(suffix, ctype, npy_type) \
  QIARRAY_DEFINE_MIXED_BINARY_OP(bitwise_and, suffix, ctype, &)

#define QIARRAY_DEFINE_OR_TYPE(suffix, ctype, npy_type) \
  QIARRAY_DEFINE_MIXED_BINARY_OP(bitwise_or, suffix, ctype, |)

#define QIARRAY_DEFINE_XOR_TYPE(suffix, ctype, npy_type) \
  QIARRAY_DEFINE_MIXED_BINARY_OP(bitwise_xor, suffix, ctype, ^)

QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_DEFINE_ADD_TYPE)
QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_DEFINE_SUBTRACT_TYPE)
QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_DEFINE_MULTIPLY_TYPE)
QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_DEFINE_AND_TYPE)
QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_DEFINE_OR_TYPE)
QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_DEFINE_XOR_TYPE)
QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_DEFINE_DIVMOD_OPS)
QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_DEFINE_SHIFT_OPS)
QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_DEFINE_CAST_FUNCS)

static void
QuadIArray_ufunc_add(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__int128 *)out = *(__int128 *)in1 + *(__int128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_subtract(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__int128 *)out = *(__int128 *)in1 - *(__int128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_multiply(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__int128 *)out = *(__int128 *)in1 * *(__int128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_floor_divide(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    __int128 quotient;
    __int128 remainder;
    if (qiarray_floor_divmod(*(__int128 *)in1, *(__int128 *)in2, &quotient, &remainder) < 0) {
      return;
    }
    *(__int128 *)out = quotient;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_remainder(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    __int128 quotient;
    __int128 remainder;
    if (qiarray_floor_divmod(*(__int128 *)in1, *(__int128 *)in2, &quotient, &remainder) < 0) {
      return;
    }
    *(__int128 *)out = remainder;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_left_shift(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    __int128 shift = *(__int128 *)in2;
    if (qiarray_validate_shift(shift) < 0) {
      return;
    }
    *(__int128 *)out = *(__int128 *)in1 << (unsigned int)shift;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_right_shift(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    __int128 shift = *(__int128 *)in2;
    if (qiarray_validate_shift(shift) < 0) {
      return;
    }
    *(__int128 *)out = *(__int128 *)in1 >> (unsigned int)shift;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_bitwise_and(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__int128 *)out = *(__int128 *)in1 & *(__int128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_bitwise_or(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__int128 *)out = *(__int128 *)in1 | *(__int128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_bitwise_xor(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__int128 *)out = *(__int128 *)in1 ^ *(__int128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadIArray_ufunc_negative(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__int128 *)out = -*(__int128 *)in;
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadIArray_ufunc_positive(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__int128 *)out = +*(__int128 *)in;
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadIArray_ufunc_absolute(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    __int128 value = *(__int128 *)in;
    *(__int128 *)out = value < 0 ? -value : value;
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadIArray_ufunc_invert(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__int128 *)out = ~*(__int128 *)in;
    in += steps[0];
    out += steps[1];
  }
}

static int
QuadIArray_register_ufunc_binary(const char *name, PyUFuncGenericFunction loop)
{
  PyObject *numpy_mod;
  PyObject *ufunc;
  int types[3];

  numpy_mod = PyImport_ImportModule("numpy");
  if (numpy_mod == NULL) {
    return -1;
  }
  ufunc = PyObject_GetAttrString(numpy_mod, name);
  Py_DECREF(numpy_mod);
  if (ufunc == NULL) {
    return -1;
  }

  types[0] = QuadIArrayTypeNum;
  types[1] = QuadIArrayTypeNum;
  types[2] = QuadIArrayTypeNum;

  if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadIArrayTypeNum, loop, types, NULL) < 0) {
    Py_DECREF(ufunc);
    return -1;
  }

  Py_DECREF(ufunc);
  return 0;
}

static int
QuadIArray_register_ufunc_binary_types(const char *name, PyUFuncGenericFunction loop, int in0, int in1, int out)
{
  PyObject *numpy_mod;
  PyObject *ufunc;
  int types[3];

  numpy_mod = PyImport_ImportModule("numpy");
  if (numpy_mod == NULL) {
    return -1;
  }
  ufunc = PyObject_GetAttrString(numpy_mod, name);
  Py_DECREF(numpy_mod);
  if (ufunc == NULL) {
    return -1;
  }

  types[0] = in0;
  types[1] = in1;
  types[2] = out;

  if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadIArrayTypeNum, loop, types, NULL) < 0) {
    Py_DECREF(ufunc);
    return -1;
  }

  Py_DECREF(ufunc);
  return 0;
}

static int
QuadIArray_register_ufunc_unary(const char *name, PyUFuncGenericFunction loop)
{
  PyObject *numpy_mod;
  PyObject *ufunc;
  int types[2];

  numpy_mod = PyImport_ImportModule("numpy");
  if (numpy_mod == NULL) {
    return -1;
  }
  ufunc = PyObject_GetAttrString(numpy_mod, name);
  Py_DECREF(numpy_mod);
  if (ufunc == NULL) {
    return -1;
  }

  types[0] = QuadIArrayTypeNum;
  types[1] = QuadIArrayTypeNum;

  if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadIArrayTypeNum, loop, types, NULL) < 0) {
    Py_DECREF(ufunc);
    return -1;
  }

  Py_DECREF(ufunc);
  return 0;
}

static int
QuadIArray_register_ufuncs(void)
{
  if (QuadIArray_register_ufunc_binary("add", QuadIArray_ufunc_add) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_binary("subtract", QuadIArray_ufunc_subtract) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_binary("multiply", QuadIArray_ufunc_multiply) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_binary("floor_divide", QuadIArray_ufunc_floor_divide) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_binary("remainder", QuadIArray_ufunc_remainder) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_binary("left_shift", QuadIArray_ufunc_left_shift) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_binary("right_shift", QuadIArray_ufunc_right_shift) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_binary("bitwise_and", QuadIArray_ufunc_bitwise_and) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_binary("bitwise_or", QuadIArray_ufunc_bitwise_or) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_binary("bitwise_xor", QuadIArray_ufunc_bitwise_xor) < 0) {
    return -1;
  }

#define QIARRAY_REGISTER_MIXED_UFUNCS(suffix, ctype, npy_type) \
  if (QuadIArray_register_ufunc_binary_types("add", QuadIArray_ufunc_add_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("add", QuadIArray_ufunc_add_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("subtract", QuadIArray_ufunc_subtract_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("subtract", QuadIArray_ufunc_subtract_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("multiply", QuadIArray_ufunc_multiply_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("multiply", QuadIArray_ufunc_multiply_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("floor_divide", QuadIArray_ufunc_floor_divide_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("floor_divide", QuadIArray_ufunc_floor_divide_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("remainder", QuadIArray_ufunc_remainder_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("remainder", QuadIArray_ufunc_remainder_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("left_shift", QuadIArray_ufunc_left_shift_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("left_shift", QuadIArray_ufunc_left_shift_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("right_shift", QuadIArray_ufunc_right_shift_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("right_shift", QuadIArray_ufunc_right_shift_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("bitwise_and", QuadIArray_ufunc_bitwise_and_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("bitwise_and", QuadIArray_ufunc_bitwise_and_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("bitwise_or", QuadIArray_ufunc_bitwise_or_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("bitwise_or", QuadIArray_ufunc_bitwise_or_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("bitwise_xor", QuadIArray_ufunc_bitwise_xor_q##suffix, QuadIArrayTypeNum, npy_type, QuadIArrayTypeNum) < 0) { \
    return -1; \
  } \
  if (QuadIArray_register_ufunc_binary_types("bitwise_xor", QuadIArray_ufunc_bitwise_xor_##suffix##q, npy_type, QuadIArrayTypeNum, QuadIArrayTypeNum) < 0) { \
    return -1; \
  }

  QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_REGISTER_MIXED_UFUNCS)

#undef QIARRAY_REGISTER_MIXED_UFUNCS

  if (QuadIArray_register_ufunc_unary("negative", QuadIArray_ufunc_negative) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_unary("positive", QuadIArray_ufunc_positive) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_unary("absolute", QuadIArray_ufunc_absolute) < 0) {
    return -1;
  }
  if (QuadIArray_register_ufunc_unary("invert", QuadIArray_ufunc_invert) < 0) {
    return -1;
  }

  return 0;
}

static int
QuadIArray_register_cast_pair(
  PyArray_Descr *quad_descr,
  int quad_type_num,
  int type_num,
  PyArray_VectorUnaryFunc *to_func,
  PyArray_VectorUnaryFunc *from_func)
{
  PyArray_Descr *type_descr;

  if (PyArray_RegisterCastFunc(quad_descr, type_num, to_func) < 0) {
    return -1;
  }

  type_descr = PyArray_DescrFromType(type_num);
  if (type_descr == NULL) {
    return -1;
  }
  if (PyArray_RegisterCastFunc(type_descr, quad_type_num, from_func) < 0) {
    Py_DECREF(type_descr);
    return -1;
  }
  Py_DECREF(type_descr);

  return 0;
}

static int
QuadIArray_register_casts(PyArray_Descr *quad_descr, int quad_type_num)
{
#define QIARRAY_REGISTER_CASTS(suffix, ctype, npy_type) \
  if (QuadIArray_register_cast_pair(quad_descr, quad_type_num, npy_type, QuadIArray_cast_to_##suffix, QuadIArray_cast_from_##suffix) < 0) { \
    return -1; \
  }

  QIARRAY_FIXED_INTEGER_TYPES(QIARRAY_REGISTER_CASTS)

#undef QIARRAY_REGISTER_CASTS

  return 0;
}

static PyArrayObject *
QuadIArray_new_empty(int nd, npy_intp *dims)
{
  PyArray_Descr *descr;

  if (QuadIArrayDescr == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "qiarray dtype not initialized");
    return NULL;
  }

  descr = QuadIArrayDescr;
  Py_INCREF(descr);
  return (PyArrayObject *)PyArray_SimpleNewFromDescr(nd, dims, descr);
}

static PyObject *
qiarray_zeros(PyObject *NPY_UNUSED(self), PyObject *args)
{
  Py_ssize_t n;
  npy_intp dims[1];
  PyArrayObject *arr;

  if (!PyArg_ParseTuple(args, "n", &n)) {
    return NULL;
  }
  if (n < 0) {
    PyErr_SetString(PyExc_ValueError, "size must be non-negative");
    return NULL;
  }

  dims[0] = (npy_intp)n;
  arr = QuadIArray_new_empty(1, dims);
  if (arr == NULL) {
    return NULL;
  }

  memset(PyArray_DATA(arr), 0, (size_t)dims[0] * sizeof(__int128));
  return (PyObject *)arr;
}

static PyObject *
qiarray_empty(PyObject *NPY_UNUSED(self), PyObject *args)
{
  Py_ssize_t n;
  npy_intp dims[1];

  if (!PyArg_ParseTuple(args, "n", &n)) {
    return NULL;
  }
  if (n < 0) {
    PyErr_SetString(PyExc_ValueError, "size must be non-negative");
    return NULL;
  }

  dims[0] = (npy_intp)n;
  return (PyObject *)QuadIArray_new_empty(1, dims);
}

static PyObject *
qiarray_ones(PyObject *NPY_UNUSED(self), PyObject *args)
{
  Py_ssize_t n;
  npy_intp i;
  npy_intp dims[1];
  PyArrayObject *arr;
  __int128 *data;

  if (!PyArg_ParseTuple(args, "n", &n)) {
    return NULL;
  }
  if (n < 0) {
    PyErr_SetString(PyExc_ValueError, "size must be non-negative");
    return NULL;
  }

  dims[0] = (npy_intp)n;
  arr = QuadIArray_new_empty(1, dims);
  if (arr == NULL) {
    return NULL;
  }

  data = (__int128 *)PyArray_DATA(arr);
  for (i = 0; i < dims[0]; ++i) {
    data[i] = 1;
  }
  return (PyObject *)arr;
}

static PyObject *
qiarray_full(PyObject *NPY_UNUSED(self), PyObject *args)
{
  PyObject *fill_obj;
  Py_ssize_t n;
  npy_intp i;
  npy_intp dims[1];
  PyArrayObject *arr;
  __int128 fill;
  __int128 *data;

  if (!PyArg_ParseTuple(args, "nO", &n, &fill_obj)) {
    return NULL;
  }
  if (n < 0) {
    PyErr_SetString(PyExc_ValueError, "size must be non-negative");
    return NULL;
  }

  dims[0] = (npy_intp)n;
  arr = QuadIArray_new_empty(1, dims);
  if (arr == NULL) {
    return NULL;
  }

  if (QuadIArray_setitem(fill_obj, &fill, arr) < 0) {
    Py_DECREF(arr);
    return NULL;
  }

  data = (__int128 *)PyArray_DATA(arr);
  for (i = 0; i < dims[0]; ++i) {
    data[i] = fill;
  }

  return (PyObject *)arr;
}

static int
QuadIArray_arange_length(__int128 start, __int128 stop, __int128 step, npy_intp *length)
{
  unsigned __int128 diff;
  unsigned __int128 step_mag;
  unsigned __int128 count;

  if (step == 0) {
    PyErr_SetString(PyExc_ValueError, "step must not be zero");
    return -1;
  }

  if ((step > 0 && start >= stop) || (step < 0 && start <= stop)) {
    *length = 0;
    return 0;
  }

  if (step > 0) {
    diff = (unsigned __int128)stop - (unsigned __int128)start;
    step_mag = (unsigned __int128)step;
  } else {
    diff = (unsigned __int128)start - (unsigned __int128)stop;
    step_mag = 0 - (unsigned __int128)step;
  }

  count = ((diff - (unsigned __int128)1) / step_mag) + (unsigned __int128)1;
  if (count > (unsigned __int128)NPY_MAX_INTP) {
    PyErr_SetString(PyExc_OverflowError, "arange result is too large");
    return -1;
  }

  *length = (npy_intp)count;
  return 0;
}

static PyObject *
qiarray_arange(PyObject *NPY_UNUSED(self), PyObject *args)
{
  Py_ssize_t nargs;
  PyObject *start_obj = NULL;
  PyObject *stop_obj = NULL;
  PyObject *step_obj = NULL;
  __int128 start = 0;
  __int128 stop = 0;
  __int128 step = 1;
  npy_intp n;
  npy_intp i;
  npy_intp dims[1];
  PyArrayObject *arr;
  __int128 *data;
  __int128 current;

  nargs = PyTuple_GET_SIZE(args);
  if (nargs < 1 || nargs > 3) {
    PyErr_SetString(PyExc_TypeError, "arange expects 1 to 3 positional arguments");
    return NULL;
  }

  if (nargs == 1) {
    stop_obj = PyTuple_GET_ITEM(args, 0);
  } else if (nargs == 2) {
    start_obj = PyTuple_GET_ITEM(args, 0);
    stop_obj = PyTuple_GET_ITEM(args, 1);
  } else {
    start_obj = PyTuple_GET_ITEM(args, 0);
    stop_obj = PyTuple_GET_ITEM(args, 1);
    step_obj = PyTuple_GET_ITEM(args, 2);
  }

  if (start_obj != NULL && QuadIArray_setitem(start_obj, &start, NULL) < 0) {
    return NULL;
  }
  if (QuadIArray_setitem(stop_obj, &stop, NULL) < 0) {
    return NULL;
  }
  if (step_obj != NULL && QuadIArray_setitem(step_obj, &step, NULL) < 0) {
    return NULL;
  }

  if (QuadIArray_arange_length(start, stop, step, &n) < 0) {
    return NULL;
  }

  dims[0] = n;
  arr = QuadIArray_new_empty(1, dims);
  if (arr == NULL) {
    return NULL;
  }

  data = (__int128 *)PyArray_DATA(arr);
  current = start;
  for (i = 0; i < n; ++i) {
    data[i] = current;
    if (i + 1 < n) {
      current += step;
    }
  }

  return (PyObject *)arr;
}

static PyObject *
qiarray_from_list(PyObject *NPY_UNUSED(self), PyObject *args)
{
  PyObject *obj;
  PyObject *seq;
  Py_ssize_t i;
  Py_ssize_t n;
  npy_intp dims[1];
  PyArrayObject *arr;
  __int128 *data;

  if (!PyArg_ParseTuple(args, "O", &obj)) {
    return NULL;
  }

  seq = PySequence_Fast(obj, "from_list requires a sequence");
  if (seq == NULL) {
    return NULL;
  }

  n = PySequence_Fast_GET_SIZE(seq);
  dims[0] = (npy_intp)n;
  arr = QuadIArray_new_empty(1, dims);
  if (arr == NULL) {
    Py_DECREF(seq);
    return NULL;
  }

  data = (__int128 *)PyArray_DATA(arr);
  for (i = 0; i < n; ++i) {
    PyObject *item = PySequence_Fast_GET_ITEM(seq, i);
    if (QuadIArray_setitem(item, &data[i], arr) < 0) {
      Py_DECREF(arr);
      Py_DECREF(seq);
      return NULL;
    }
  }

  Py_DECREF(seq);
  return (PyObject *)arr;
}

static PyObject *
qiarray_from_array(PyObject *NPY_UNUSED(self), PyObject *args)
{
  PyObject *obj;
  PyArrayObject *input;
  PyArrayObject *arr;
  PyArrayIterObject *it;
  npy_intp i;
  __int128 *data;

  if (!PyArg_ParseTuple(args, "O", &obj)) {
    return NULL;
  }

  input = (PyArrayObject *)PyArray_FROM_O(obj);
  if (input == NULL) {
    return NULL;
  }

  arr = QuadIArray_new_empty(PyArray_NDIM(input), PyArray_DIMS(input));
  if (arr == NULL) {
    Py_DECREF(input);
    return NULL;
  }

  it = (PyArrayIterObject *)PyArray_IterNew((PyObject *)input);
  if (it == NULL) {
    Py_DECREF(arr);
    Py_DECREF(input);
    return NULL;
  }

  data = (__int128 *)PyArray_DATA(arr);
  for (i = 0; i < it->size; ++i) {
    PyObject *item = PyArray_GETITEM(input, it->dataptr);
    if (item == NULL) {
      Py_DECREF(it);
      Py_DECREF(arr);
      Py_DECREF(input);
      return NULL;
    }
    if (QuadIArray_setitem(item, &data[i], arr) < 0) {
      Py_DECREF(item);
      Py_DECREF(it);
      Py_DECREF(arr);
      Py_DECREF(input);
      return NULL;
    }
    Py_DECREF(item);
    PyArray_ITER_NEXT(it);
  }

  Py_DECREF(it);
  Py_DECREF(input);
  return (PyObject *)arr;
}

static PyObject *
qiarray_asarray(PyObject *self, PyObject *args)
{
  return qiarray_from_array(self, args);
}

static PyObject *
qiarray_array(PyObject *self, PyObject *args)
{
  return qiarray_from_array(self, args);
}

static PyObject *
qiarray_empty_like(PyObject *NPY_UNUSED(self), PyObject *args)
{
  PyObject *obj;
  PyArrayObject *input;
  PyArrayObject *arr;

  if (!PyArg_ParseTuple(args, "O", &obj)) {
    return NULL;
  }

  input = (PyArrayObject *)PyArray_FROM_O(obj);
  if (input == NULL) {
    return NULL;
  }

  arr = QuadIArray_new_empty(PyArray_NDIM(input), PyArray_DIMS(input));
  Py_DECREF(input);
  if (arr == NULL) {
    return NULL;
  }

  return (PyObject *)arr;
}

static PyObject *
qiarray_zeros_like(PyObject *NPY_UNUSED(self), PyObject *args)
{
  PyArrayObject *arr = (PyArrayObject *)qiarray_empty_like(NULL, args);
  if (arr == NULL) {
    return NULL;
  }

  memset(PyArray_DATA(arr), 0, (size_t)PyArray_NBYTES(arr));
  return (PyObject *)arr;
}

static PyObject *
qiarray_full_like(PyObject *NPY_UNUSED(self), PyObject *args)
{
  PyObject *obj;
  PyObject *value;
  PyArrayObject *input;
  PyArrayObject *arr;
  __int128 *data;
  npy_intp i;
  npy_intp n;

  if (!PyArg_ParseTuple(args, "OO", &obj, &value)) {
    return NULL;
  }

  input = (PyArrayObject *)PyArray_FROM_O(obj);
  if (input == NULL) {
    return NULL;
  }

  arr = QuadIArray_new_empty(PyArray_NDIM(input), PyArray_DIMS(input));
  Py_DECREF(input);
  if (arr == NULL) {
    return NULL;
  }

  data = (__int128 *)PyArray_DATA(arr);
  n = PyArray_SIZE(arr);
  for (i = 0; i < n; ++i) {
    if (QuadIArray_setitem(value, &data[i], arr) < 0) {
      Py_DECREF(arr);
      return NULL;
    }
  }

  return (PyObject *)arr;
}

static PyObject *
qiarray_ones_like(PyObject *NPY_UNUSED(self), PyObject *args)
{
  PyObject *obj;
  PyObject *arg_tuple;
  PyObject *ret;

  if (!PyArg_ParseTuple(args, "O", &obj)) {
    return NULL;
  }

  arg_tuple = Py_BuildValue("(Oi)", obj, 1);
  if (arg_tuple == NULL) {
    return NULL;
  }

  ret = qiarray_full_like(NULL, arg_tuple);
  Py_DECREF(arg_tuple);
  return ret;
}

static PyMethodDef QuadIArrayMethods[] = {
  {"arange", qiarray_arange, METH_VARARGS, "Create a 1-D qiarray with evenly spaced integer values in an interval."},
  {"empty", qiarray_empty, METH_VARARGS, "Create a 1-D uninitialized qiarray."},
  {"full", qiarray_full, METH_VARARGS, "Create a 1-D qiarray filled with a value."},
  {"zeros", qiarray_zeros, METH_VARARGS, "Create a 1-D qiarray of zeros."},
  {"ones", qiarray_ones, METH_VARARGS, "Create a 1-D qiarray of ones."},
  {"from_list", qiarray_from_list, METH_VARARGS, "Create a qiarray from a Python sequence."},
  {"from_array", qiarray_from_array, METH_VARARGS, "Create a qiarray from an array-like object."},
  {"asarray", qiarray_asarray, METH_VARARGS, "Create a qiarray from an array-like object."},
  {"array", qiarray_array, METH_VARARGS, "Create a qiarray from an array-like object."},
  {"empty_like", qiarray_empty_like, METH_VARARGS, "Create an empty qiarray with the same shape as input."},
  {"zeros_like", qiarray_zeros_like, METH_VARARGS, "Create a zero-filled qiarray with the same shape as input."},
  {"ones_like", qiarray_ones_like, METH_VARARGS, "Create a one-filled qiarray with the same shape as input."},
  {"full_like", qiarray_full_like, METH_VARARGS, "Create a qiarray filled with a value and the same shape as input."},
  {NULL, NULL, 0, NULL},
};

static PyModuleDef QuadIArrayModule = {
  PyModuleDef_HEAD_INIT,
  .m_name = "qiarray",
  .m_doc = "Signed __int128 NumPy array support.",
  .m_methods = QuadIArrayMethods,
  .m_size = -1,
};

static npy_bool
QuadIArray_nonzero(void *ip, void *arr)
{
  int elsize = PyArray_ITEMSIZE(arr);
  char *ptr = ip;

  while (elsize--) {
    if (*ptr++ != 0) {
      return NPY_TRUE;
    }
  }
  return NPY_FALSE;
}

static void
QuadIArray_copyswap(__int128 *dst, __int128 *src, int swap, void *NPY_UNUSED(arr))
{
  size_t i;
  char *bytes;

  if (src == NULL) {
    src = dst;
  }
  if (src != dst) {
    memcpy(dst, src, sizeof(__int128));
  }

  if (swap != 0) {
    char tmp;
    bytes = (char *)dst;
    for (i = 0; i < (sizeof(__int128) / 2); ++i) {
      tmp = bytes[i];
      bytes[i] = bytes[sizeof(__int128) - 1 - i];
      bytes[sizeof(__int128) - 1 - i] = tmp;
    }
  }
}

static void
QuadIArray_copyswapn(void *dst, npy_intp dstride, void *src, npy_intp sstride, npy_intp n, int swap, void *arr)
{
  npy_intp i;
  PyArray_CopySwapFunc *copyswap;
  char *dstptr = dst;
  char *srcptr = src;

  copyswap = PyDataType_GetArrFuncs(PyArray_DESCR(arr))->copyswap;
  for (i = 0; i < n; ++i) {
    copyswap(dstptr, srcptr, swap, arr);
    dstptr += dstride;
    srcptr += sstride;
  }
}

static int
QuadIArray_setitem(PyObject *item, __int128 *data, void *array)
{
  QuadIObject tmp;

  (void)array;

  if (PyObject_to_QuadIObject(item, &tmp, false)) {
    *data = tmp.value;
  } else {
    if (!PyErr_Occurred()) {
      PyErr_SetString(PyExc_TypeError, "Failed to setitem in QuadIArray");
    }
    return -1;
  }
  return 0;
}

static PyObject *
QuadIArray_getitem(__int128 *data, void *NPY_UNUSED(arr))
{
  QuadIObject tmp;

  tmp.value = *data;
  return QuadIObject_to_PyObject(tmp);
}

static int
QuadIArray_compare(__int128 *pa, __int128 *pb, PyArrayObject *NPY_UNUSED(ap))
{
  if (*pa < *pb) {
    return -1;
  }
  if (*pa > *pb) {
    return 1;
  }
  return 0;
}

static int
QuadIArray_argmax(__int128 *ip, npy_intp n, npy_intp *max_ind, PyArrayObject *NPY_UNUSED(aip))
{
  npy_intp i;
  __int128 mp = *ip;

  *max_ind = 0;
  for (i = 1; i < n; ++i) {
    ip++;
    if (*ip >= mp) {
      mp = *ip;
      *max_ind = i;
    }
  }
  return 0;
}

static int
QuadIArray_argmin(__int128 *ip, npy_intp n, npy_intp *min_ind, PyArrayObject *NPY_UNUSED(aip))
{
  npy_intp i;
  __int128 mp = *ip;

  *min_ind = 0;
  for (i = 1; i < n; ++i) {
    ip++;
    if (*ip <= mp) {
      mp = *ip;
      *min_ind = i;
    }
  }
  return 0;
}

static void
QuadIArray_fillwithscalar(__int128 *buffer, npy_intp length, __int128 *value, void *NPY_UNUSED(ignored))
{
  npy_intp i;
  __int128 val = *value;

  for (i = 0; i < length; ++i) {
    buffer[i] = val;
  }
}

PyMODINIT_FUNC
PyInit_qiarray(void)
{
  PyObject *m;
  int qiarrayNum;

  m = PyModule_Create(&QuadIArrayModule);
  if (m == NULL) {
    return NULL;
  }

  if (import_qmint() < 0) {
    Py_DECREF(m);
    return NULL;
  }

  import_array();
  if (PyErr_Occurred()) {
    Py_DECREF(m);
    return NULL;
  }
  import_umath();
  if (PyErr_Occurred()) {
    Py_DECREF(m);
    return NULL;
  }

  PyArray_InitArrFuncs(&QuadIArrayFuncs);
  QuadIArrayFuncs.nonzero = (PyArray_NonzeroFunc *)QuadIArray_nonzero;
  QuadIArrayFuncs.copyswap = (PyArray_CopySwapFunc *)QuadIArray_copyswap;
  QuadIArrayFuncs.copyswapn = (PyArray_CopySwapNFunc *)QuadIArray_copyswapn;
  QuadIArrayFuncs.setitem = (PyArray_SetItemFunc *)QuadIArray_setitem;
  QuadIArrayFuncs.getitem = (PyArray_GetItemFunc *)QuadIArray_getitem;
  QuadIArrayFuncs.compare = (PyArray_CompareFunc *)QuadIArray_compare;
  QuadIArrayFuncs.argmax = (PyArray_ArgFunc *)QuadIArray_argmax;
  QuadIArrayFuncs.argmin = (PyArray_ArgFunc *)QuadIArray_argmin;
  QuadIArrayFuncs.fillwithscalar = (PyArray_FillWithScalarFunc *)QuadIArray_fillwithscalar;

  QuadIArrayDescrProto = (PyArray_DescrProto){
    .typeobj = &QuadIType,
    .kind = 'V',
    .type = 'i',
    .byteorder = '=',
    .flags = NPY_NEEDS_PYAPI | NPY_USE_GETITEM | NPY_USE_SETITEM,
    .type_num = 0,
    .elsize = sizeof(__int128),
    .alignment = alignof(__int128),
    .f = &QuadIArrayFuncs,
    .subarray = NULL,
    .fields = NULL,
    .names = NULL,
    .metadata = NULL,
    .c_metadata = NULL,
  };

  Py_SET_TYPE(&QuadIArrayDescrProto, &PyArrayDescr_Type);


  Py_INCREF(&QuadIType);
  qiarrayNum = PyArray_RegisterDataType(&QuadIArrayDescrProto);
  if (qiarrayNum < 0) {
    Py_DECREF(m);
    return NULL;
  }

  QuadIArrayTypeNum = qiarrayNum;
  QuadIArrayDescr = PyArray_DescrFromType(qiarrayNum);
  if (QuadIArrayDescr == NULL) {
    Py_DECREF(m);
    return NULL;
  }

  if (QuadIArray_register_casts(QuadIArrayDescr, qiarrayNum) < 0) {
    Py_DECREF(m);
    return NULL;
  }

  if (QuadIArray_register_ufuncs() < 0) {
    Py_DECREF(m);
    return NULL;
  }

  if (PyModule_AddObjectRef(m, "qiarray", (PyObject *)&QuadIType) < 0) {
    Py_DECREF(m);
    return NULL;
  }
  if (PyModule_AddIntConstant(m, "dtype_num", qiarrayNum) < 0) {
    Py_DECREF(m);
    return NULL;
  }
  if (PyModule_AddObjectRef(m, "dtype", (PyObject *)QuadIArrayDescr) < 0) {
    Py_DECREF(m);
    return NULL;
  }

  return m;
}