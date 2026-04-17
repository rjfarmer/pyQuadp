// SPDX-License-Identifier: GPL-2.0+

// Helpful guide https://github.com/moble/quaternion/blob/main/src/numpy_quaternion.c

#define NPY_TARGET_VERSION NPY_2_0_API_VERSION
#define NPY_NO_DEPRECATED_API NPY_2_0_API_VERSION

#include "pyquadp.h"

#include <numpy/arrayobject.h>
#include <numpy/npy_math.h>
#include <numpy/ufuncobject.h>
#include <stdalign.h>
#include <string.h>

#define QFLOATARRAY_MODULE
#include "qfloatarray.h"
#include "qfloat.h"

static int QuadArrayTypeNum = -1;

static PyTypeObject QuadArrayType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "pyquadp.qarray",
  .tp_basicsize = sizeof(PyObject),
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_doc = "NumPy custom dtype support for quad precision values",
};
PyArray_ArrFuncs QuadArrayFuncs;
PyArray_Descr* QuadArrayDescr;
PyArray_DescrProto QuadArrayDescrProto;

static int QuadArray_setitem(PyObject* item, __float128* data, void* array);

static void
QuadArray_ufunc_add(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = *(__float128 *)in1 + *(__float128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_subtract(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = *(__float128 *)in1 - *(__float128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_multiply(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = *(__float128 *)in1 * *(__float128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_divide(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in1 = args[0];
  char *in2 = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = *(__float128 *)in1 / *(__float128 *)in2;
    in1 += steps[0];
    in2 += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_add_qd(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *inq = args[0];
  char *ind = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = *(__float128 *)inq + (__float128)(*(npy_float64 *)ind);
    inq += steps[0];
    ind += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_add_dq(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *ind = args[0];
  char *inq = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = (__float128)(*(npy_float64 *)ind) + *(__float128 *)inq;
    ind += steps[0];
    inq += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_subtract_qd(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *inq = args[0];
  char *ind = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = *(__float128 *)inq - (__float128)(*(npy_float64 *)ind);
    inq += steps[0];
    ind += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_subtract_dq(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *ind = args[0];
  char *inq = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = (__float128)(*(npy_float64 *)ind) - *(__float128 *)inq;
    ind += steps[0];
    inq += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_multiply_qd(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *inq = args[0];
  char *ind = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = *(__float128 *)inq * (__float128)(*(npy_float64 *)ind);
    inq += steps[0];
    ind += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_multiply_dq(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *ind = args[0];
  char *inq = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = (__float128)(*(npy_float64 *)ind) * *(__float128 *)inq;
    ind += steps[0];
    inq += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_divide_qd(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *inq = args[0];
  char *ind = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = *(__float128 *)inq / (__float128)(*(npy_float64 *)ind);
    inq += steps[0];
    ind += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_divide_dq(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *ind = args[0];
  char *inq = args[1];
  char *out = args[2];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = (__float128)(*(npy_float64 *)ind) / *(__float128 *)inq;
    ind += steps[0];
    inq += steps[1];
    out += steps[2];
  }
}

static void
QuadArray_ufunc_negative(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = -*(__float128 *)in;
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_positive(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = +*(__float128 *)in;
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_absolute(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = fabsq(*(__float128 *)in);
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_square(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    __float128 v = *(__float128 *)in;
    *(__float128 *)out = v * v;
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_sqrt(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = sqrtq(*(__float128 *)in);
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_exp(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = expq(*(__float128 *)in);
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_log(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = logq(*(__float128 *)in);
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_sin(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = sinq(*(__float128 *)in);
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_cos(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = cosq(*(__float128 *)in);
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_tan(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = tanq(*(__float128 *)in);
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_sinh(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = sinhq(*(__float128 *)in);
    in += steps[0];
    out += steps[1];
  }
}

static void
QuadArray_ufunc_cosh(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
  npy_intp i;
  npy_intp n = dims[0];
  char *in = args[0];
  char *out = args[1];

  for (i = 0; i < n; ++i) {
    *(__float128 *)out = coshq(*(__float128 *)in);
    in += steps[0];
    out += steps[1];
  }
}

static int
QuadArray_register_ufunc_binary(const char *name, PyUFuncGenericFunction loop)
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

  types[0] = QuadArrayTypeNum;
  types[1] = QuadArrayTypeNum;
  types[2] = QuadArrayTypeNum;

  if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadArrayTypeNum, loop, types, NULL) < 0) {
    Py_DECREF(ufunc);
    return -1;
  }

  Py_DECREF(ufunc);
  return 0;
}

static int
QuadArray_register_ufunc_binary_types(const char *name, PyUFuncGenericFunction loop, int in0, int in1, int out)
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

  if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadArrayTypeNum, loop, types, NULL) < 0) {
    Py_DECREF(ufunc);
    return -1;
  }

  Py_DECREF(ufunc);
  return 0;
}

static int
QuadArray_register_ufunc_unary(const char *name, PyUFuncGenericFunction loop)
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

  types[0] = QuadArrayTypeNum;
  types[1] = QuadArrayTypeNum;

  if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadArrayTypeNum, loop, types, NULL) < 0) {
    Py_DECREF(ufunc);
    return -1;
  }

  Py_DECREF(ufunc);
  return 0;
}

static int
QuadArray_register_ufuncs(void)
{
  if (QuadArray_register_ufunc_binary("add", QuadArray_ufunc_add) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary("subtract", QuadArray_ufunc_subtract) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary("multiply", QuadArray_ufunc_multiply) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary("divide", QuadArray_ufunc_divide) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary_types("add", QuadArray_ufunc_add_qd, QuadArrayTypeNum, NPY_DOUBLE, QuadArrayTypeNum) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary_types("add", QuadArray_ufunc_add_dq, NPY_DOUBLE, QuadArrayTypeNum, QuadArrayTypeNum) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary_types("subtract", QuadArray_ufunc_subtract_qd, QuadArrayTypeNum, NPY_DOUBLE, QuadArrayTypeNum) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary_types("subtract", QuadArray_ufunc_subtract_dq, NPY_DOUBLE, QuadArrayTypeNum, QuadArrayTypeNum) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary_types("multiply", QuadArray_ufunc_multiply_qd, QuadArrayTypeNum, NPY_DOUBLE, QuadArrayTypeNum) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary_types("multiply", QuadArray_ufunc_multiply_dq, NPY_DOUBLE, QuadArrayTypeNum, QuadArrayTypeNum) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary_types("divide", QuadArray_ufunc_divide_qd, QuadArrayTypeNum, NPY_DOUBLE, QuadArrayTypeNum) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_binary_types("divide", QuadArray_ufunc_divide_dq, NPY_DOUBLE, QuadArrayTypeNum, QuadArrayTypeNum) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("negative", QuadArray_ufunc_negative) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("positive", QuadArray_ufunc_positive) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("absolute", QuadArray_ufunc_absolute) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("square", QuadArray_ufunc_square) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("sqrt", QuadArray_ufunc_sqrt) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("exp", QuadArray_ufunc_exp) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("log", QuadArray_ufunc_log) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("sin", QuadArray_ufunc_sin) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("cos", QuadArray_ufunc_cos) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("tan", QuadArray_ufunc_tan) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("sinh", QuadArray_ufunc_sinh) < 0) {
    return -1;
  }
  if (QuadArray_register_ufunc_unary("cosh", QuadArray_ufunc_cosh) < 0) {
    return -1;
  }
  return 0;
}

static void
QuadArray_cast_to_float64(void *from, void *to, npy_intp n, void *NPY_UNUSED(fromarr), void *NPY_UNUSED(toarr))
{
  npy_intp i;
  __float128 *src = (__float128 *)from;
  npy_float64 *dst = (npy_float64 *)to;

  for (i = 0; i < n; ++i) {
    dst[i] = (npy_float64)__float128_to_double(src[i]);
  }
}

static void
QuadArray_cast_to_float32(void *from, void *to, npy_intp n, void *NPY_UNUSED(fromarr), void *NPY_UNUSED(toarr))
{
  npy_intp i;
  __float128 *src = (__float128 *)from;
  npy_float32 *dst = (npy_float32 *)to;

  for (i = 0; i < n; ++i) {
    dst[i] = (npy_float32)__float128_to_double(src[i]);
  }
}

static void
QuadArray_cast_from_float64(void *from, void *to, npy_intp n, void *NPY_UNUSED(fromarr), void *NPY_UNUSED(toarr))
{
  npy_intp i;
  npy_float64 *src = (npy_float64 *)from;
  __float128 *dst = (__float128 *)to;

  for (i = 0; i < n; ++i) {
    dst[i] = (__float128)src[i];
  }
}

static void
QuadArray_cast_from_float32(void *from, void *to, npy_intp n, void *NPY_UNUSED(fromarr), void *NPY_UNUSED(toarr))
{
  npy_intp i;
  npy_float32 *src = (npy_float32 *)from;
  __float128 *dst = (__float128 *)to;

  for (i = 0; i < n; ++i) {
    dst[i] = (__float128)src[i];
  }
}

static int
QuadArray_register_casts(PyArray_Descr *quad_descr, int quad_type_num)
{
  PyArray_Descr *float64_descr;
  PyArray_Descr *float32_descr;

  if (PyArray_RegisterCastFunc(quad_descr, NPY_DOUBLE, QuadArray_cast_to_float64) < 0) {
    return -1;
  }
  if (PyArray_RegisterCastFunc(quad_descr, NPY_FLOAT, QuadArray_cast_to_float32) < 0) {
    return -1;
  }
  if (PyArray_RegisterCanCast(quad_descr, NPY_DOUBLE, NPY_NOSCALAR) < 0) {
    return -1;
  }
  if (PyArray_RegisterCanCast(quad_descr, NPY_FLOAT, NPY_NOSCALAR) < 0) {
    return -1;
  }

  float64_descr = PyArray_DescrFromType(NPY_DOUBLE);
  if (float64_descr == NULL) {
    return -1;
  }
  if (PyArray_RegisterCastFunc(float64_descr, quad_type_num, QuadArray_cast_from_float64) < 0) {
    Py_DECREF(float64_descr);
    return -1;
  }
  if (PyArray_RegisterCanCast(float64_descr, quad_type_num, NPY_NOSCALAR) < 0) {
    Py_DECREF(float64_descr);
    return -1;
  }
  Py_DECREF(float64_descr);

  float32_descr = PyArray_DescrFromType(NPY_FLOAT);
  if (float32_descr == NULL) {
    return -1;
  }
  if (PyArray_RegisterCastFunc(float32_descr, quad_type_num, QuadArray_cast_from_float32) < 0) {
    Py_DECREF(float32_descr);
    return -1;
  }
  if (PyArray_RegisterCanCast(float32_descr, quad_type_num, NPY_NOSCALAR) < 0) {
    Py_DECREF(float32_descr);
    return -1;
  }
  Py_DECREF(float32_descr);

  return 0;
}

static PyArrayObject *
QuadArray_new_empty(int nd, npy_intp *dims)
{
  PyArray_Descr *descr;

  if (QuadArrayDescr == NULL) {
    PyErr_SetString(PyExc_RuntimeError, "qarray dtype not initialized");
    return NULL;
  }

  descr = QuadArrayDescr;
  Py_INCREF(descr);
  return (PyArrayObject *)PyArray_SimpleNewFromDescr(nd, dims, descr);
}

static PyObject *
qarray_zeros(PyObject *NPY_UNUSED(self), PyObject *args)
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
  arr = QuadArray_new_empty(1, dims);
  if (arr == NULL) {
    return NULL;
  }

  memset(PyArray_DATA(arr), 0, (size_t)dims[0] * sizeof(__float128));
  return (PyObject *)arr;
}

static PyObject *
qarray_ones(PyObject *NPY_UNUSED(self), PyObject *args)
{
  Py_ssize_t n;
  npy_intp i;
  npy_intp dims[1];
  PyArrayObject *arr;
  __float128 *data;

  if (!PyArg_ParseTuple(args, "n", &n)) {
    return NULL;
  }
  if (n < 0) {
    PyErr_SetString(PyExc_ValueError, "size must be non-negative");
    return NULL;
  }

  dims[0] = (npy_intp)n;
  arr = QuadArray_new_empty(1, dims);
  if (arr == NULL) {
    return NULL;
  }

  data = (__float128 *)PyArray_DATA(arr);
  for (i = 0; i < dims[0]; ++i) {
    data[i] = 1.0Q;
  }
  return (PyObject *)arr;
}

static PyObject *
qarray_from_list(PyObject *NPY_UNUSED(self), PyObject *args)
{
  PyObject *obj;
  PyObject *seq;
  Py_ssize_t i;
  Py_ssize_t n;
  npy_intp dims[1];
  PyArrayObject *arr;
  __float128 *data;

  if (!PyArg_ParseTuple(args, "O", &obj)) {
    return NULL;
  }

  seq = PySequence_Fast(obj, "from_list requires a sequence");
  if (seq == NULL) {
    return NULL;
  }

  n = PySequence_Fast_GET_SIZE(seq);
  dims[0] = (npy_intp)n;
  arr = QuadArray_new_empty(1, dims);
  if (arr == NULL) {
    Py_DECREF(seq);
    return NULL;
  }

  data = (__float128 *)PyArray_DATA(arr);
  for (i = 0; i < n; ++i) {
    PyObject *item = PySequence_Fast_GET_ITEM(seq, i);
    if (QuadArray_setitem(item, &data[i], arr) < 0) {
      Py_DECREF(arr);
      Py_DECREF(seq);
      return NULL;
    }
  }

  Py_DECREF(seq);
  return (PyObject *)arr;
}

static PyObject *
qarray_from_array(PyObject *NPY_UNUSED(self), PyObject *args)
{
  PyObject *obj;
  PyArrayObject *input;
  PyArrayObject *arr;
  PyArrayIterObject *it;
  npy_intp i;
  __float128 *data;

  if (!PyArg_ParseTuple(args, "O", &obj)) {
    return NULL;
  }

  input = (PyArrayObject *)PyArray_FROM_O(obj);
  if (input == NULL) {
    return NULL;
  }

  arr = QuadArray_new_empty(PyArray_NDIM(input), PyArray_DIMS(input));
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

  data = (__float128 *)PyArray_DATA(arr);
  for (i = 0; i < it->size; ++i) {
    PyObject *item = PyArray_GETITEM(input, it->dataptr);
    if (item == NULL) {
      Py_DECREF(it);
      Py_DECREF(arr);
      Py_DECREF(input);
      return NULL;
    }
    if (QuadArray_setitem(item, &data[i], arr) < 0) {
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

static PyMethodDef QuadArrayMethods[] = {
  {"zeros", qarray_zeros, METH_VARARGS, "Create a 1-D qarray of zeros."},
  {"ones", qarray_ones, METH_VARARGS, "Create a 1-D qarray of ones."},
  {"from_list", qarray_from_list, METH_VARARGS, "Create a qarray from a Python sequence."},
  {"from_array", qarray_from_array, METH_VARARGS, "Create a qarray from an array-like object."},
  {NULL, NULL, 0, NULL},
};

static PyModuleDef QuadArrayModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "qarray",
    .m_doc = "Quad precision module for array quad's.",
  .m_methods = QuadArrayMethods,
    .m_size = -1,
};


static npy_bool 
QuadArray_nonzero(void *ip, void *arr){

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
QuadArray_copyswap(__float128 *dst, __float128 *src,
                    int swap, void *NPY_UNUSED(arr))
{
  size_t i;
  char *bytes;

  if (src == NULL) {
    src = dst;
  }
  if (src != dst) {
    memcpy(dst, src, sizeof(__float128));
  }

  if (swap != 0) {
    char tmp;
    bytes = (char *)dst;
    for (i = 0; i < (sizeof(__float128) / 2); ++i) {
      tmp = bytes[i];
      bytes[i] = bytes[sizeof(__float128) - 1 - i];
      bytes[sizeof(__float128) - 1 - i] = tmp;
    }
    }
}

static void
QuadArray_copyswapn(void *dst, npy_intp dstride, void *src,
                   npy_intp sstride, npy_intp n, int swap, void *arr)
{
    npy_intp i;
    PyArray_CopySwapFunc *copyswap;
    char *dstptr = dst;
    char *srcptr = src;

    copyswap = PyDataType_GetArrFuncs(PyArray_DESCR(arr))->copyswap;

    for (i = 0; i < n; i++) {
        copyswap(dstptr, srcptr, swap, arr);
        dstptr += dstride;
        srcptr += sstride;
    }
}


static int QuadArray_setitem(PyObject* item, __float128* data, void* array){
  QuadObject tmp;

  (void)array;

  if (PyObject_to_QuadObject(item, &tmp, false)) {
    *data = tmp.value;
  } else {
        PyErr_SetString(PyExc_TypeError,
                    "Failed to setitem in QuadArray");
        return -1;
    }
    return 0;
}

static PyObject *
QuadArray_getitem(__float128* data, void* NPY_UNUSED(arr))
{
  QuadObject tmp;

  tmp.value = *data;

  return QuadObject_to_PyObject(tmp);
}

static int
QuadArray_compare(__float128 *pa, __float128 *pb, PyArrayObject *NPY_UNUSED(ap))
{
  npy_bool anan, bnan;
  int ret;

  anan = isnanq(*pa);
  bnan = isnanq(*pb);

  if (anan) {
    ret = bnan ? 0 : -1;
  } else if (bnan) {
    ret = 1;
  } else if(*pa<*pb) {
    ret = -1;
  } else if(*pa>*pb) {
    ret = 1;
  } else {
    ret = 0;
  }

  return ret;
}


static int
QuadArray_argmax(__float128 *ip, npy_intp n, npy_intp *max_ind, PyArrayObject *NPY_UNUSED(aip))
{
  npy_intp i;
  __float128 mp = *ip;

  *max_ind = 0;

  if (isnanq(mp)) {
    // nan encountered; it's maximal
    return 0;
  }

  for (i = 1; i < n; i++) {
    ip++;
    //Propagate nans, similarly as max() and min()
    if (!(*ip<mp)) {  // negated, for correct nan handling
      mp = *ip;
      *max_ind = i;
      if (isnanq(mp)) {
        // nan encountered, it's maximal
        break;
      }
    }
  }
  return 0;
}

static int
QuadArray_argmin(__float128 *ip, npy_intp n, npy_intp *min_ind, PyArrayObject *NPY_UNUSED(aip))
{
  npy_intp i;
  __float128 mp = *ip;

  *min_ind = 0;

  if (isnanq(mp)) {
    // nan encountered; it's minimal
    return 0;
  }

  for (i = 1; i < n; i++) {
    ip++;
    // Propagate nans, similarly as max() and min()
    if (!(*ip > mp)) {  // negated, for correct nan handling
      mp = *ip;
      *min_ind = i;
      if (isnanq(mp)) {
        // nan encountered, it's minimal
        break;
      }
    }
  }
  return 0;
}

static void
QuadArray_fillwithscalar(__float128 *buffer, npy_intp length, __float128 *value, void *NPY_UNUSED(ignored))
{
  npy_intp i;
  __float128 val = *value;

  for (i = 0; i < length; ++i) {
    buffer[i] = val;
  }
}

PyMODINIT_FUNC
PyInit_qarray(void)
{

    PyObject *m;
    int qarrayNum;

    m = PyModule_Create(&QuadArrayModule);
    if (m == NULL)
        return NULL;

    if (import_qmfloat() < 0) {
      Py_DECREF(m);
      return NULL;
    }

    // Initialize numpy
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

    QuadArrayType.tp_base = &PyGenericArrType_Type;
    if (PyType_Ready(&QuadArrayType) < 0) {
        PyErr_Print();
        PyErr_SetString(PyExc_SystemError, "Could not initialize QuadArrayType.");
      Py_DECREF(m);
        return NULL;
    }

    PyArray_InitArrFuncs(&QuadArrayFuncs);
    QuadArrayFuncs.nonzero = (PyArray_NonzeroFunc*) QuadArray_nonzero;
    QuadArrayFuncs.copyswap = (PyArray_CopySwapFunc*) QuadArray_copyswap;
    QuadArrayFuncs.copyswapn = (PyArray_CopySwapNFunc*) QuadArray_copyswapn;
    QuadArrayFuncs.setitem = (PyArray_SetItemFunc*) QuadArray_setitem;
    QuadArrayFuncs.getitem = (PyArray_GetItemFunc*) QuadArray_getitem;
    QuadArrayFuncs.compare = (PyArray_CompareFunc*) QuadArray_compare;
    QuadArrayFuncs.argmax = (PyArray_ArgFunc*) QuadArray_argmax;
    QuadArrayFuncs.argmin = (PyArray_ArgFunc*) QuadArray_argmin;
    QuadArrayFuncs.fillwithscalar = (PyArray_FillWithScalarFunc*) QuadArray_fillwithscalar;


    Py_SET_TYPE(&QuadArrayDescrProto, &PyArrayDescr_Type);
    QuadArrayDescrProto.typeobj = &QuadArrayType;
    QuadArrayDescrProto.kind = 'V';
    QuadArrayDescrProto.type = 'f';
    QuadArrayDescrProto.byteorder = '=';
    QuadArrayDescrProto.flags = NPY_NEEDS_PYAPI | NPY_USE_GETITEM | NPY_USE_SETITEM;
    QuadArrayDescrProto.type_num = 0; // assigned at registration
    QuadArrayDescrProto.elsize = sizeof(__float128);
    QuadArrayDescrProto.alignment = alignof(__float128);
    QuadArrayDescrProto.f = &QuadArrayFuncs;
#if NPY_ABI_VERSION < 0x02000000
    QuadArrayDescrProto.name = "qfloat";
#endif
    QuadArrayDescrProto.subarray = NULL;
    QuadArrayDescrProto.fields = NULL;
    QuadArrayDescrProto.names = NULL;
    QuadArrayDescrProto.metadata = NULL;
    QuadArrayDescrProto.c_metadata = NULL;

    Py_INCREF(&QuadArrayType);
    qarrayNum = PyArray_RegisterDataType(&QuadArrayDescrProto);

    if (qarrayNum < 0) {
      Py_DECREF(m);
        return NULL;
    }
    QuadArrayTypeNum = qarrayNum;
    QuadArrayDescr = PyArray_DescrFromType(qarrayNum);
    if (QuadArrayDescr == NULL) {
      Py_DECREF(m);
      return NULL;
    }

    if (QuadArray_register_casts(QuadArrayDescr, qarrayNum) < 0) {
      Py_DECREF(m);
      return NULL;
    }

    if (QuadArray_register_ufuncs() < 0) {
      Py_DECREF(m);
      return NULL;
    }

    if (PyModule_AddObject(m, "qarray", (PyObject *)&QuadArrayType) < 0) {
      Py_DECREF(m);
      return NULL;
    }
    if (PyModule_AddIntConstant(m, "dtype_num", qarrayNum) < 0) {
      Py_DECREF(m);
      return NULL;
    }
    Py_INCREF(QuadArrayDescr);
    if (PyModule_AddObject(m, "dtype", (PyObject *)QuadArrayDescr) < 0) {
      Py_DECREF(QuadArrayDescr);
      Py_DECREF(m);
      return NULL;
    }

    return m;
}