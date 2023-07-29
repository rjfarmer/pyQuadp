// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <complex.h> 
#include <float.h>

#include "qcmplx.h"
#include "qfloat.h"
#include "qcmathc.h"

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

static PyObject * QuadCObject_qmath_op1_qfloat(const int op, PyObject * self, PyObject * args){
    QuadObject result;
    QuadCObject q1;
    __complex128 c1;
    PyObject * obj = NULL;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse object");
        return NULL;
    }

    if(!PyObject_to_QuadCObject(obj, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return NULL;
    }

    c1 = QuadCObject_complex128(&q1);

    alloc_QuadType(&result);

    switch(op){
        case OP_cabsq:
            result.value = cabsq(c1);
            break;
        case OP_cargq:
            result.value = cargq(c1);
            break;
        case OP_cimagq:
            result.value = crealq(c1);
            break;
        case OP_crealq:
            result.value = crealq(c1);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadObject_to_PyObject(result);

}


static PyObject *
QuadCObject_qmath_op1(const int op, PyObject * self, PyObject * args){
    QuadCObject result;
    PyObject * obj = NULL;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse object");
        return NULL;
    }

    if(!PyObject_to_QuadCObject(obj, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return NULL;
    }

    switch(op){
        case OP_cacoshq:
            result.value = cacoshq(result.value);
            break;
        case OP_cacosq:
            result.value = cacosq(result.value);
            break;
        case OP_casinhq:
            result.value = casinhq(result.value);
            break;
        case OP_casinq:
            result.value = casinq(result.value);
            break;
        case OP_catanhq:
            result.value = catanhq(result.value);
            break;
        case OP_catanq:
            result.value = catanq(result.value);
            break;
        case OP_ccosq:
            result.value = ccosq(result.value);
            break;
        case OP_ccoshq:
            result.value = ccoshq(result.value);
            break;
        case OP_cexpq:
            result.value = cexpq(result.value);
            break;
        case OP_cexpiq:
            result.value = cexpiq(result.value);
            break;
        case OP_clogq:
            result.value = clogq(result.value);
            break;
        case OP_clog10q:
            result.value = clog10q(result.value);
            break;
        case OP_conjq:
            result.value = conjq(result.value);
            break;
        case OP_cprojq:
            result.value = cprojq(result.value);
            break;
        case OP_csinq:
            result.value = csinq(result.value);
            break;
        case OP_csinhq:
            result.value = csinhq(result.value);
            break;
        case OP_csqrtq:
            result.value = csqrtq(result.value);
            break;
        case OP_ctanq:
            result.value = ctanq(result.value);
            break;
        case OP_ctanhq:
            result.value = ctanhq(result.value);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadCObject_to_PyObject(result);
}


static PyObject *
QuadCObject_qmath_op2(const int op, PyObject * self, PyObject * args){
    QuadCObject result, q1;
    PyObject * obj1 = NULL, * obj2 = NULL;

    if (!PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyObject_to_QuadCObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    if(!PyObject_to_QuadCObject(obj2, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert second value to quad precision.");
        return NULL;
    }

    switch(op){
        case OP_cpowq:
            result.value = cpowq(result.value, q1.value);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadCObject_to_PyObject(result);
}


static PyObject * _cabs(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1_qfloat(OP_cabsq, self, args);
};

static PyObject * _carg(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1_qfloat(OP_cargq, self, args);
};

static PyObject * _cimag(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1_qfloat(OP_cimagq, self, args);
};

static PyObject * _creal(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1_qfloat(OP_crealq, self, args);
};

static PyObject * _cacosh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_cacoshq, self, args);
};

static PyObject * _cacos(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_cacosq, self, args);
};

static PyObject * _casinh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_casinhq, self, args);
};

static PyObject * _casin(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_casinq, self, args);
};

static PyObject * _catanh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_catanhq, self, args);
};

static PyObject * _catan(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_catanq, self, args);
};

static PyObject * _ccos(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_ccosq, self, args);
};

static PyObject * _ccosh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_ccoshq, self, args);
};

static PyObject * _cexp(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_cexpq, self, args);
};

static PyObject * _cexpi(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_cexpiq, self, args);
};

static PyObject * _clog(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_clogq, self, args);
};

static PyObject * _clog10(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_clog10q, self, args);
};

static PyObject * _conj(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_conjq, self, args);
};

static PyObject * _cpow(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op2(OP_cpowq, self, args);
};

static PyObject * _cproj(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_cprojq, self, args);
};

static PyObject * _csin(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_csinq, self, args);
};

static PyObject * _csinh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_csinhq, self, args);
};

static PyObject * _csqrt(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_csqrtq, self, args);
};

static PyObject * _ctan(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_ctanq, self, args);
};

static PyObject * _ctanh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_ctanhq, self, args);
};



static PyMethodDef QMathCMethods[] = {
    {"_cabs", (PyCFunction) _cabs, METH_VARARGS, "cabs"},
    {"_carg", (PyCFunction) _carg, METH_VARARGS, "carg"},
    {"_cimag", (PyCFunction) _cimag, METH_VARARGS, "cimag"},
    {"_creal", (PyCFunction) _creal, METH_VARARGS, "creal"},
    {"_cacosh", (PyCFunction) _cacosh, METH_VARARGS, "cacosh"},
    {"_cacos", (PyCFunction) _cacos, METH_VARARGS, "cacos"},
    {"_casinh", (PyCFunction) _casinh, METH_VARARGS, "casinh"},
    {"_casin", (PyCFunction) _casin, METH_VARARGS, "casin"},
    {"_catanh", (PyCFunction) _catanh, METH_VARARGS, "catanh"},
    {"_catan", (PyCFunction) _catan, METH_VARARGS, "catan"},
    {"_ccos", (PyCFunction) _ccos, METH_VARARGS, "ccos"},
    {"_ccosh", (PyCFunction) _ccosh, METH_VARARGS, "ccosh"},
    {"_cexp", (PyCFunction) _cexp, METH_VARARGS, "cexp"},
    {"_cexpi", (PyCFunction) _cexpi, METH_VARARGS, "cexpi"},
    {"_clog", (PyCFunction) _clog, METH_VARARGS, "clog"},
    {"_clog10", (PyCFunction) _clog10, METH_VARARGS, "clog10"},
    {"_conj", (PyCFunction) _conj, METH_VARARGS, "conj"},
    {"_cpow", (PyCFunction) _cpow, METH_VARARGS, "cpow"},
    {"_cproj", (PyCFunction) _cproj, METH_VARARGS, "cproj"},
    {"_csin", (PyCFunction) _csin, METH_VARARGS, "csin"},
    {"_csinh", (PyCFunction) _csinh, METH_VARARGS, "csinh"},
    {"_csqrt", (PyCFunction) _csqrt, METH_VARARGS, "csqrt"},
    {"_ctan", (PyCFunction) _ctan, METH_VARARGS, "ctan"},
    {"_ctanh", (PyCFunction) _ctanh, METH_VARARGS, "ctanh"},
    {NULL, NULL, 0, NULL}
};


PyModuleDef QMathCModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qcmathc",
    .m_doc = "Quad precision complex math library.",
    .m_size = -1,
    .m_methods = QMathCMethods,
};

PyMODINIT_FUNC
PyInit_qcmathc(void)
{

    PyObject *m;

    m = PyModule_Create(&QMathCModule);
    if (m == NULL)
        return NULL;

    if (import_qfloat() < 0)
        return NULL;

    if (import_qcmplx() < 0)
        return NULL;

    return m;
}