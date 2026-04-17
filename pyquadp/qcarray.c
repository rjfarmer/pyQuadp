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

#define QCARRAY_MODULE
#include "qcarray.h"
#include "qcmplx.h"

static int QuadCArrayTypeNum = -1;
static int QuadArrayTypeNum = -1;

static PyTypeObject QuadCArrayType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pyquadp.qcarray",
    .tp_basicsize = sizeof(PyObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "NumPy custom dtype support for quad precision complex values",
};

static PyArray_ArrFuncs QuadCArrayFuncs;
static PyArray_Descr *QuadCArrayDescr;
static PyArray_DescrProto QuadCArrayDescrProto;

static inline __complex128
qcomplex_from_cdouble(npy_cdouble z)
{
    __complex128 out = 0.0Q;
    __real__ out = (__float128)npy_creal(z);
    __imag__ out = (__float128)npy_cimag(z);
    return out;
}

static inline npy_cdouble
cdouble_from_qcomplex(__complex128 z)
{
    return npy_cpack((double)(__real__ z), (double)(__imag__ z));
}

static inline npy_bool
qcomplex_isnan(__complex128 z)
{
    return isnanq(__real__ z) || isnanq(__imag__ z);
}

static inline __float128
qcomplex_abs2(__complex128 z)
{
    __float128 r = __real__ z;
    __float128 i = __imag__ z;
    return r * r + i * i;
}

static int QuadCArray_setitem(PyObject *item, __complex128 *data, void *array);

static void
QuadCArray_ufunc_add(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in1 = args[0];
    char *in2 = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = *(__complex128 *)in1 + *(__complex128 *)in2;
        in1 += steps[0];
        in2 += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_subtract(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in1 = args[0];
    char *in2 = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = *(__complex128 *)in1 - *(__complex128 *)in2;
        in1 += steps[0];
        in2 += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_multiply(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in1 = args[0];
    char *in2 = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = *(__complex128 *)in1 * *(__complex128 *)in2;
        in1 += steps[0];
        in2 += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_divide(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in1 = args[0];
    char *in2 = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = *(__complex128 *)in1 / *(__complex128 *)in2;
        in1 += steps[0];
        in2 += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_add_qc(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *inq = args[0];
    char *inc = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = *(__complex128 *)inq + qcomplex_from_cdouble(*(npy_cdouble *)inc);
        inq += steps[0];
        inc += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_add_cq(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *inc = args[0];
    char *inq = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = qcomplex_from_cdouble(*(npy_cdouble *)inc) + *(__complex128 *)inq;
        inc += steps[0];
        inq += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_subtract_qc(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *inq = args[0];
    char *inc = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = *(__complex128 *)inq - qcomplex_from_cdouble(*(npy_cdouble *)inc);
        inq += steps[0];
        inc += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_subtract_cq(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *inc = args[0];
    char *inq = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = qcomplex_from_cdouble(*(npy_cdouble *)inc) - *(__complex128 *)inq;
        inc += steps[0];
        inq += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_multiply_qc(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *inq = args[0];
    char *inc = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = *(__complex128 *)inq * qcomplex_from_cdouble(*(npy_cdouble *)inc);
        inq += steps[0];
        inc += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_multiply_cq(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *inc = args[0];
    char *inq = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = qcomplex_from_cdouble(*(npy_cdouble *)inc) * *(__complex128 *)inq;
        inc += steps[0];
        inq += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_divide_qc(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *inq = args[0];
    char *inc = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = *(__complex128 *)inq / qcomplex_from_cdouble(*(npy_cdouble *)inc);
        inq += steps[0];
        inc += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_divide_cq(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *inc = args[0];
    char *inq = args[1];
    char *out = args[2];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = qcomplex_from_cdouble(*(npy_cdouble *)inc) / *(__complex128 *)inq;
        inc += steps[0];
        inq += steps[1];
        out += steps[2];
    }
}

static void
QuadCArray_ufunc_negative(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = -*(__complex128 *)in;
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_positive(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = *(__complex128 *)in;
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_conjugate(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = conjq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_absolute(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__float128 *)out = cabsq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_square(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        __complex128 v = *(__complex128 *)in;
        *(__complex128 *)out = v * v;
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_sqrt(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = csqrtq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_exp(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = cexpq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_log(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = clogq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_sin(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = csinq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_cos(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = ccosq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_tan(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = ctanq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_sinh(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = csinhq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static void
QuadCArray_ufunc_cosh(char **args, const npy_intp *dims, const npy_intp *steps, void *NPY_UNUSED(data))
{
    npy_intp i;
    npy_intp n = dims[0];
    char *in = args[0];
    char *out = args[1];

    for (i = 0; i < n; ++i) {
        *(__complex128 *)out = ccoshq(*(__complex128 *)in);
        in += steps[0];
        out += steps[1];
    }
}

static int
QuadCArray_register_ufunc_binary(const char *name, PyUFuncGenericFunction loop)
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

    types[0] = QuadCArrayTypeNum;
    types[1] = QuadCArrayTypeNum;
    types[2] = QuadCArrayTypeNum;

    if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadCArrayTypeNum, loop, types, NULL) < 0) {
        Py_DECREF(ufunc);
        return -1;
    }

    Py_DECREF(ufunc);
    return 0;
}

static int
QuadCArray_register_ufunc_binary_types(
    const char *name,
    PyUFuncGenericFunction loop,
    int in0,
    int in1,
    int out)
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

    if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadCArrayTypeNum, loop, types, NULL) < 0) {
        Py_DECREF(ufunc);
        return -1;
    }

    Py_DECREF(ufunc);
    return 0;
}

static int
QuadCArray_register_ufunc_unary(const char *name, PyUFuncGenericFunction loop)
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

    types[0] = QuadCArrayTypeNum;
    types[1] = QuadCArrayTypeNum;

    if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadCArrayTypeNum, loop, types, NULL) < 0) {
        Py_DECREF(ufunc);
        return -1;
    }

    Py_DECREF(ufunc);
    return 0;
}

static int
QuadCArray_register_ufunc_unary_types(
    const char *name,
    PyUFuncGenericFunction loop,
    int in0,
    int out)
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

    types[0] = in0;
    types[1] = out;

    if (PyUFunc_RegisterLoopForType((PyUFuncObject *)ufunc, QuadCArrayTypeNum, loop, types, NULL) < 0) {
        Py_DECREF(ufunc);
        return -1;
    }

    Py_DECREF(ufunc);
    return 0;
}

static int
QuadCArray_register_ufuncs(void)
{
    if (QuadCArray_register_ufunc_binary("add", QuadCArray_ufunc_add) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary("subtract", QuadCArray_ufunc_subtract) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary("multiply", QuadCArray_ufunc_multiply) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary("divide", QuadCArray_ufunc_divide) < 0) {
        return -1;
    }

    if (QuadCArray_register_ufunc_binary_types("add", QuadCArray_ufunc_add_qc, QuadCArrayTypeNum, NPY_CDOUBLE, QuadCArrayTypeNum) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary_types("add", QuadCArray_ufunc_add_cq, NPY_CDOUBLE, QuadCArrayTypeNum, QuadCArrayTypeNum) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary_types("subtract", QuadCArray_ufunc_subtract_qc, QuadCArrayTypeNum, NPY_CDOUBLE, QuadCArrayTypeNum) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary_types("subtract", QuadCArray_ufunc_subtract_cq, NPY_CDOUBLE, QuadCArrayTypeNum, QuadCArrayTypeNum) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary_types("multiply", QuadCArray_ufunc_multiply_qc, QuadCArrayTypeNum, NPY_CDOUBLE, QuadCArrayTypeNum) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary_types("multiply", QuadCArray_ufunc_multiply_cq, NPY_CDOUBLE, QuadCArrayTypeNum, QuadCArrayTypeNum) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary_types("divide", QuadCArray_ufunc_divide_qc, QuadCArrayTypeNum, NPY_CDOUBLE, QuadCArrayTypeNum) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_binary_types("divide", QuadCArray_ufunc_divide_cq, NPY_CDOUBLE, QuadCArrayTypeNum, QuadCArrayTypeNum) < 0) {
        return -1;
    }

    if (QuadCArray_register_ufunc_unary("negative", QuadCArray_ufunc_negative) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("positive", QuadCArray_ufunc_positive) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("conjugate", QuadCArray_ufunc_conjugate) < 0) {
        return -1;
    }
    if (QuadArrayTypeNum < 0) {
        PyErr_SetString(PyExc_RuntimeError, "qarray dtype not initialized");
        return -1;
    }
    if (QuadCArray_register_ufunc_unary_types("absolute", QuadCArray_ufunc_absolute, QuadCArrayTypeNum, QuadArrayTypeNum) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("square", QuadCArray_ufunc_square) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("sqrt", QuadCArray_ufunc_sqrt) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("exp", QuadCArray_ufunc_exp) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("log", QuadCArray_ufunc_log) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("sin", QuadCArray_ufunc_sin) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("cos", QuadCArray_ufunc_cos) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("tan", QuadCArray_ufunc_tan) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("sinh", QuadCArray_ufunc_sinh) < 0) {
        return -1;
    }
    if (QuadCArray_register_ufunc_unary("cosh", QuadCArray_ufunc_cosh) < 0) {
        return -1;
    }

    return 0;
}

static void
QuadCArray_cast_to_complex128(void *from, void *to, npy_intp n, void *NPY_UNUSED(fromarr), void *NPY_UNUSED(toarr))
{
    npy_intp i;
    __complex128 *src = (__complex128 *)from;
    npy_cdouble *dst = (npy_cdouble *)to;

    for (i = 0; i < n; ++i) {
        dst[i] = cdouble_from_qcomplex(src[i]);
    }
}

static void
QuadCArray_cast_from_complex128(
    void *from,
    void *to,
    npy_intp n,
    void *NPY_UNUSED(fromarr),
    void *NPY_UNUSED(toarr))
{
    npy_intp i;
    npy_cdouble *src = (npy_cdouble *)from;
    __complex128 *dst = (__complex128 *)to;

    for (i = 0; i < n; ++i) {
        dst[i] = qcomplex_from_cdouble(src[i]);
    }
}

static void
QuadCArray_cast_from_float64(
    void *from,
    void *to,
    npy_intp n,
    void *NPY_UNUSED(fromarr),
    void *NPY_UNUSED(toarr))
{
    npy_intp i;
    npy_float64 *src = (npy_float64 *)from;
    __complex128 *dst = (__complex128 *)to;

    for (i = 0; i < n; ++i) {
        __complex128 out = 0.0Q;
        __real__ out = (__float128)src[i];
        __imag__ out = 0.0Q;
        dst[i] = out;
    }
}

static int
QuadCArray_register_casts(PyArray_Descr *quad_descr, int quad_type_num)
{
    PyArray_Descr *complex_descr;
    PyArray_Descr *float_descr;

    if (PyArray_RegisterCastFunc(quad_descr, NPY_CDOUBLE, QuadCArray_cast_to_complex128) < 0) {
        return -1;
    }
    if (PyArray_RegisterCanCast(quad_descr, NPY_CDOUBLE, NPY_NOSCALAR) < 0) {
        return -1;
    }

    complex_descr = PyArray_DescrFromType(NPY_CDOUBLE);
    if (complex_descr == NULL) {
        return -1;
    }
    if (PyArray_RegisterCastFunc(complex_descr, quad_type_num, QuadCArray_cast_from_complex128) < 0) {
        Py_DECREF(complex_descr);
        return -1;
    }
    if (PyArray_RegisterCanCast(complex_descr, quad_type_num, NPY_NOSCALAR) < 0) {
        Py_DECREF(complex_descr);
        return -1;
    }
    Py_DECREF(complex_descr);

    float_descr = PyArray_DescrFromType(NPY_DOUBLE);
    if (float_descr == NULL) {
        return -1;
    }
    if (PyArray_RegisterCastFunc(float_descr, quad_type_num, QuadCArray_cast_from_float64) < 0) {
        Py_DECREF(float_descr);
        return -1;
    }
    if (PyArray_RegisterCanCast(float_descr, quad_type_num, NPY_NOSCALAR) < 0) {
        Py_DECREF(float_descr);
        return -1;
    }
    Py_DECREF(float_descr);

    return 0;
}

static PyArrayObject *
QuadCArray_new_empty(int nd, npy_intp *dims)
{
    PyArray_Descr *descr;

    if (QuadCArrayDescr == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "qcarray dtype not initialized");
        return NULL;
    }

    descr = QuadCArrayDescr;
    Py_INCREF(descr);
    return (PyArrayObject *)PyArray_SimpleNewFromDescr(nd, dims, descr);
}

static PyObject *
qcarray_zeros(PyObject *NPY_UNUSED(self), PyObject *args)
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
    arr = QuadCArray_new_empty(1, dims);
    if (arr == NULL) {
        return NULL;
    }

    memset(PyArray_DATA(arr), 0, (size_t)dims[0] * sizeof(__complex128));
    return (PyObject *)arr;
}

static PyObject *
qcarray_ones(PyObject *NPY_UNUSED(self), PyObject *args)
{
    Py_ssize_t n;
    npy_intp i;
    npy_intp dims[1];
    PyArrayObject *arr;
    __complex128 *data;

    if (!PyArg_ParseTuple(args, "n", &n)) {
        return NULL;
    }
    if (n < 0) {
        PyErr_SetString(PyExc_ValueError, "size must be non-negative");
        return NULL;
    }

    dims[0] = (npy_intp)n;
    arr = QuadCArray_new_empty(1, dims);
    if (arr == NULL) {
        return NULL;
    }

    data = (__complex128 *)PyArray_DATA(arr);
    for (i = 0; i < dims[0]; ++i) {
        __complex128 v = 0.0Q;
        __real__ v = 1.0Q;
        __imag__ v = 0.0Q;
        data[i] = v;
    }
    return (PyObject *)arr;
}

static PyObject *
qcarray_from_list(PyObject *NPY_UNUSED(self), PyObject *args)
{
    PyObject *obj;
    PyObject *seq;
    Py_ssize_t i;
    Py_ssize_t n;
    npy_intp dims[1];
    PyArrayObject *arr;
    __complex128 *data;

    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }

    seq = PySequence_Fast(obj, "from_list requires a sequence");
    if (seq == NULL) {
        return NULL;
    }

    n = PySequence_Fast_GET_SIZE(seq);
    dims[0] = (npy_intp)n;
    arr = QuadCArray_new_empty(1, dims);
    if (arr == NULL) {
        Py_DECREF(seq);
        return NULL;
    }

    data = (__complex128 *)PyArray_DATA(arr);
    for (i = 0; i < n; ++i) {
        PyObject *item = PySequence_Fast_GET_ITEM(seq, i);
        if (QuadCArray_setitem(item, &data[i], arr) < 0) {
            Py_DECREF(arr);
            Py_DECREF(seq);
            return NULL;
        }
    }

    Py_DECREF(seq);
    return (PyObject *)arr;
}

static PyObject *
qcarray_from_array(PyObject *NPY_UNUSED(self), PyObject *args)
{
    PyObject *obj;
    PyArrayObject *input;
    PyArrayObject *arr;
    PyArrayIterObject *it;
    npy_intp i;
    __complex128 *data;

    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }

    input = (PyArrayObject *)PyArray_FROM_O(obj);
    if (input == NULL) {
        return NULL;
    }

    arr = QuadCArray_new_empty(PyArray_NDIM(input), PyArray_DIMS(input));
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

    data = (__complex128 *)PyArray_DATA(arr);
    for (i = 0; i < it->size; ++i) {
        PyObject *item = PyArray_GETITEM(input, it->dataptr);
        if (item == NULL) {
            Py_DECREF(it);
            Py_DECREF(arr);
            Py_DECREF(input);
            return NULL;
        }
        if (QuadCArray_setitem(item, &data[i], arr) < 0) {
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

static PyMethodDef QuadCArrayMethods[] = {
    {"zeros", qcarray_zeros, METH_VARARGS, "Create a 1-D qcarray of zeros."},
    {"ones", qcarray_ones, METH_VARARGS, "Create a 1-D qcarray of ones."},
    {"from_list", qcarray_from_list, METH_VARARGS, "Create a qcarray from a Python sequence."},
    {"from_array", qcarray_from_array, METH_VARARGS, "Create a qcarray from an array-like object."},
    {NULL, NULL, 0, NULL},
};

static PyModuleDef QuadCArrayModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "qcarray",
    .m_doc = "Quad precision complex NumPy dtype module.",
    .m_methods = QuadCArrayMethods,
    .m_size = -1,
};

static npy_bool
QuadCArray_nonzero(void *ip, void *arr)
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
QuadCArray_copyswap(__complex128 *dst, __complex128 *src, int swap, void *NPY_UNUSED(arr))
{
    size_t i;
    char *bytes;

    if (src == NULL) {
        src = dst;
    }
    if (src != dst) {
        memcpy(dst, src, sizeof(__complex128));
    }

    if (swap != 0) {
        char tmp;
        bytes = (char *)dst;
        for (i = 0; i < (sizeof(__complex128) / 2); ++i) {
            tmp = bytes[i];
            bytes[i] = bytes[sizeof(__complex128) - 1 - i];
            bytes[sizeof(__complex128) - 1 - i] = tmp;
        }
    }
}

static void
QuadCArray_copyswapn(void *dst, npy_intp dstride, void *src, npy_intp sstride, npy_intp n, int swap, void *arr)
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

static int
QuadCArray_setitem(PyObject *item, __complex128 *data, void *array)
{
    QuadCObject tmp;

    (void)array;

    if (PyObject_to_QuadCObject(item, &tmp, false)) {
        *data = tmp.value;
    } else {
        PyErr_SetString(PyExc_TypeError, "Failed to setitem in QuadCArray");
        return -1;
    }
    return 0;
}

static PyObject *
QuadCArray_getitem(__complex128 *data, void *NPY_UNUSED(arr))
{
    QuadCObject tmp;

    tmp.value = *data;
    return QuadCObject_to_PyObject(tmp);
}

static int
QuadCArray_compare(__complex128 *pa, __complex128 *pb, PyArrayObject *NPY_UNUSED(ap))
{
    int ret;
    npy_bool anan = qcomplex_isnan(*pa);
    npy_bool bnan = qcomplex_isnan(*pb);

    if (anan) {
        return bnan ? 0 : 1;
    }
    if (bnan) {
        return -1;
    }

    if (__real__ *pa < __real__ *pb) {
        ret = -1;
    } else if (__real__ *pa > __real__ *pb) {
        ret = 1;
    } else if (__imag__ *pa < __imag__ *pb) {
        ret = -1;
    } else if (__imag__ *pa > __imag__ *pb) {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

static int
QuadCArray_argmax(__complex128 *ip, npy_intp n, npy_intp *max_ind, PyArrayObject *NPY_UNUSED(aip))
{
    npy_intp i;
    __complex128 mp = *ip;
    __float128 mp_abs2 = qcomplex_abs2(mp);

    *max_ind = 0;

    if (qcomplex_isnan(mp)) {
        return 0;
    }

    for (i = 1; i < n; i++) {
        __float128 ip_abs2;
        ip++;
        if (qcomplex_isnan(*ip)) {
            *max_ind = i;
            break;
        }
        ip_abs2 = qcomplex_abs2(*ip);
        if (ip_abs2 > mp_abs2) {
            mp = *ip;
            mp_abs2 = ip_abs2;
            *max_ind = i;
        }
    }
    (void)mp;
    return 0;
}

static int
QuadCArray_argmin(__complex128 *ip, npy_intp n, npy_intp *min_ind, PyArrayObject *NPY_UNUSED(aip))
{
    npy_intp i;
    __complex128 mp = *ip;

    *min_ind = 0;

    if (qcomplex_isnan(mp)) {
        return 0;
    }

    for (i = 1; i < n; i++) {
        ip++;
        if (qcomplex_isnan(*ip)) {
            *min_ind = i;
            break;
        }
        if (QuadCArray_compare(ip, &mp, NULL) < 0) {
            mp = *ip;
            *min_ind = i;
        }
    }
    return 0;
}

static void
QuadCArray_fillwithscalar(__complex128 *buffer, npy_intp length, __complex128 *value, void *NPY_UNUSED(ignored))
{
    npy_intp i;
    __complex128 val = *value;

    for (i = 0; i < length; ++i) {
        buffer[i] = val;
    }
}

PyMODINIT_FUNC
PyInit_qcarray(void)
{
    PyObject *m;
    PyObject *qarray_mod;
    PyObject *qarray_type_num_obj;
    int qcarrayNum;

    m = PyModule_Create(&QuadCArrayModule);
    if (m == NULL) {
        return NULL;
    }

    if (import_qmcmplx() < 0) {
        Py_DECREF(m);
        return NULL;
    }

    qarray_mod = PyImport_ImportModule("pyquadp.qarray");
    if (qarray_mod == NULL) {
        Py_DECREF(m);
        return NULL;
    }
    qarray_type_num_obj = PyObject_GetAttrString(qarray_mod, "dtype_num");
    Py_DECREF(qarray_mod);
    if (qarray_type_num_obj == NULL) {
        Py_DECREF(m);
        return NULL;
    }
    QuadArrayTypeNum = (int)PyLong_AsLong(qarray_type_num_obj);
    Py_DECREF(qarray_type_num_obj);
    if (QuadArrayTypeNum < 0 && PyErr_Occurred()) {
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

    QuadCArrayType.tp_base = &PyGenericArrType_Type;
    if (PyType_Ready(&QuadCArrayType) < 0) {
        PyErr_Print();
        PyErr_SetString(PyExc_SystemError, "Could not initialize QuadCArrayType.");
        Py_DECREF(m);
        return NULL;
    }

    PyArray_InitArrFuncs(&QuadCArrayFuncs);
    QuadCArrayFuncs.nonzero = (PyArray_NonzeroFunc *)QuadCArray_nonzero;
    QuadCArrayFuncs.copyswap = (PyArray_CopySwapFunc *)QuadCArray_copyswap;
    QuadCArrayFuncs.copyswapn = (PyArray_CopySwapNFunc *)QuadCArray_copyswapn;
    QuadCArrayFuncs.setitem = (PyArray_SetItemFunc *)QuadCArray_setitem;
    QuadCArrayFuncs.getitem = (PyArray_GetItemFunc *)QuadCArray_getitem;
    QuadCArrayFuncs.compare = (PyArray_CompareFunc *)QuadCArray_compare;
    QuadCArrayFuncs.argmax = (PyArray_ArgFunc *)QuadCArray_argmax;
    QuadCArrayFuncs.argmin = (PyArray_ArgFunc *)QuadCArray_argmin;
    QuadCArrayFuncs.fillwithscalar = (PyArray_FillWithScalarFunc *)QuadCArray_fillwithscalar;

    Py_SET_TYPE(&QuadCArrayDescrProto, &PyArrayDescr_Type);
    QuadCArrayDescrProto.typeobj = &QuadCArrayType;
    QuadCArrayDescrProto.kind = 'V';
    QuadCArrayDescrProto.type = 'c';
    QuadCArrayDescrProto.byteorder = '=';
    QuadCArrayDescrProto.flags = NPY_NEEDS_PYAPI | NPY_USE_GETITEM | NPY_USE_SETITEM;
    QuadCArrayDescrProto.type_num = 0;
    QuadCArrayDescrProto.elsize = sizeof(__complex128);
    QuadCArrayDescrProto.alignment = alignof(__complex128);
    QuadCArrayDescrProto.f = &QuadCArrayFuncs;
#if NPY_ABI_VERSION < 0x02000000
    QuadCArrayDescrProto.name = "qcarray";
#endif
    QuadCArrayDescrProto.subarray = NULL;
    QuadCArrayDescrProto.fields = NULL;
    QuadCArrayDescrProto.names = NULL;
    QuadCArrayDescrProto.metadata = NULL;
    QuadCArrayDescrProto.c_metadata = NULL;

    Py_INCREF(&QuadCArrayType);
    qcarrayNum = PyArray_RegisterDataType(&QuadCArrayDescrProto);

    if (qcarrayNum < 0) {
        Py_DECREF(m);
        return NULL;
    }
    QuadCArrayTypeNum = qcarrayNum;
    QuadCArrayDescr = PyArray_DescrFromType(qcarrayNum);
    if (QuadCArrayDescr == NULL) {
        Py_DECREF(m);
        return NULL;
    }

    if (QuadCArray_register_casts(QuadCArrayDescr, qcarrayNum) < 0) {
        Py_DECREF(m);
        return NULL;
    }

    if (QuadCArray_register_ufuncs() < 0) {
        Py_DECREF(m);
        return NULL;
    }

    if (PyModule_AddObject(m, "qcarray", (PyObject *)&QuadCArrayType) < 0) {
        Py_DECREF(m);
        return NULL;
    }
    if (PyModule_AddIntConstant(m, "dtype_num", qcarrayNum) < 0) {
        Py_DECREF(m);
        return NULL;
    }
    Py_INCREF(QuadCArrayDescr);
    if (PyModule_AddObject(m, "dtype", (PyObject *)QuadCArrayDescr) < 0) {
        Py_DECREF(QuadCArrayDescr);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}