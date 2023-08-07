// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "qfloat.h"
#include "qmathc.h"

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)


static PyObject * QuadObject_qmath_op1(const int op, PyObject * self, PyObject * args){
    QuadObject result;
    PyObject * obj = NULL;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse object");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return NULL;
    }

    switch(op){
        case OP_acosq:
            result.value = acosq(result.value);
            break;
        case OP_acoshq:
            result.value = acoshq(result.value);
            break;
        case OP_asinq:
            result.value = asinq(result.value);
            break;
        case OP_asinhq:
            result.value = asinhq(result.value);
            break;
        case OP_atanq:
            result.value = atanq(result.value);
            break;
        case OP_atanhq:
            result.value = atanhq(result.value);
            break;
        case OP_cbrtq:
            result.value = cbrtq(result.value);
            break;
        case OP_ceilq:
            result.value = ceilq(result.value);
            break;
        case OP_coshq:
            result.value = coshq(result.value);
            break;
        case OP_cosq:
            result.value = cosq(result.value);
            break;
        case OP_erfq:
            result.value = erfq(result.value);
            break;
        case OP_erfcq:
            result.value = erfcq(result.value);
            break;
        case OP_exp2q:
            #if __GNUC__ > 90000
            result.value = exp2q(result.value);
            #else
            result.value = powq(2.0,result.value);
            #endif
            break;
        case OP_expq:
            result.value = expq(result.value);
            break;
        case OP_expm1q:
            result.value = expm1q(result.value);
            break;
        case OP_fabsq:
            result.value = fabsq(result.value);
            break;
        case OP_floorq:
            result.value = floorq(result.value);
            break;
        case OP_j0q:
            result.value = j0q(result.value);
            break;
        case OP_j1q:
            result.value = j1q(result.value);
            break;
        case OP_lgammaq:
            result.value = lgammaq(result.value);
            break;
        case OP_logbq:
            result.value = logbq(result.value);
            break;
        case OP_logq:
            result.value = logq(result.value);
            break;
        case OP_log10q:
            result.value = log10q(result.value);
            break;
        case OP_log1pq:
            result.value = log1pq(result.value);
            break;
        case OP_log2q:
            result.value = log2q(result.value);
            break;
        case OP_rintq:
            result.value = rintq(result.value);
            break;
        case OP_roundq:
            result.value = roundq(result.value);
            break;
        case OP_sinhq:
            result.value = sinhq(result.value);
            break;
        case OP_sinq:
            result.value = sinq(result.value);
            break;
        case OP_sqrtq:
            result.value = sqrtq(result.value);
            break;
        case OP_tanq:
            result.value = tanq(result.value);
            break;
        case OP_tanhq:
            result.value = tanhq(result.value);
            break;
        case OP_tgammaq:
            result.value = tgammaq(result.value);
            break;
        case OP_truncq:
            result.value = truncq(result.value);
            break;
        case OP_y0q:
            result.value = y0q(result.value);
            break;
        case OP_y1q:
            result.value = y1q(result.value);
            break;

        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadObject_to_PyObject(result);

}


static PyObject * QuadObject_qmath_op2(const int op, PyObject * self, PyObject * args){
    QuadObject result, q1;
    PyObject * obj1 = NULL, * obj2 = NULL;

    if (!PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj2, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert second value to quad precision.");
        return NULL;
    }

    switch(op){
        case OP_atan2q:
            result.value = atan2q(result.value, q1.value);
            break;
        case OP_copysignq:
            result.value = copysignq(result.value, q1.value);
            break;
        case OP_fdimq:
            result.value = fdimq(result.value, q1.value);
            break;
        case OP_fmaxq:
            result.value = fmaxq(result.value, q1.value);
            break;
        case OP_fminq:
            result.value = fminq(result.value, q1.value);
            break;
        case OP_fmodq:
            result.value = fmodq(result.value, q1.value);
            break;
        case OP_hypotq:
            result.value = hypotq(result.value, q1.value);
            break;
        case OP_nextafterq:
            result.value = nextafterq(result.value, q1.value);
            break;
        case OP_powq:
            result.value = powq(result.value, q1.value);
            break;
        case OP_remainderq:
            result.value = remainderq(result.value, q1.value);
            break;

        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadObject_to_PyObject(result);

}


static PyObject * QuadObject_qmath_op3(const int op, PyObject * self, PyObject * args){
    QuadObject result, q1, q2;
    PyObject * obj1 = NULL, * obj2 = NULL, * obj3 = NULL;

    if (!PyArg_ParseTuple(args, "OOO:", &obj1, &obj2, &obj3)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj2, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert second value to quad precision.");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj3, &q2, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert third value to quad precision.");
        return NULL;
    }

    switch(op){
        case OP_fmaq:
            result.value = fmaq(result.value, q1.value, q2.value);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadObject_to_PyObject(result);

}


static PyObject * QuadObject_qmath_op1_int(const int op, PyObject * self, PyObject * args){
    PyObject *result = NULL;
    PyObject * obj = NULL;
    QuadObject q1;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse object");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return NULL;
    }

    switch(op){
        case OP_finiteq:
            result =  PyLong_FromLong(finiteq(q1.value));
            break;
        case OP_isinfq:
            result =  PyLong_FromLong(isinfq(q1.value));
            break;
        case OP_ilogbq:
            result =  PyLong_FromLong(ilogbq(q1.value));
            break;
        case OP_isnanq:
            result =  PyLong_FromLong(isnanq(q1.value));
            break;
        case OP_issignalingq:
            #if __GNUC__ > 90000
            result =  PyLong_FromLong(issignalingq(q1.value));
            #else
            PyErr_SetString(PyExc_NotImplementedError,"issignalingq not supported on gcc <=8");
            return NULL;
            #endif
            break;
        case OP_llrintq:
            result =  PyLong_FromLong(llrintq(q1.value));
            break;
        case OP_llroundq:
            result =  PyLong_FromLongLong(llroundq(q1.value));
            break;
        case OP_lrintq:
            result =  PyLong_FromLongLong(lrintq(q1.value));
            break;
        case OP_lroundq:
            result =  PyLong_FromLong(lroundq(q1.value));
            break;
        case OP_signbitq:
            result =  PyLong_FromLong(signbitq(q1.value));
            break;
        case OP_nearbyintq:
            result = PyLong_FromLong(nearbyintq(q1.value));
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }


    return result;

}


static PyObject * _acos(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_acosq, self, args);
};

static PyObject * _acosh(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_acoshq, self, args);
};

static PyObject * _asin(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_asinq, self, args);
};

static PyObject * _asinh(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_asinhq, self, args);
};

static PyObject * _atan(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_atanq, self, args);
};

static PyObject * _atanh(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_atanhq, self, args);
};

static PyObject * _atan2(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_atan2q, self, args);
};

static PyObject * _cbrt(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_cbrtq, self, args);
};

static PyObject * _ceil(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_ceilq, self, args);
};

static PyObject * _copysign(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_copysignq, self, args);
};

static PyObject * _cosh(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_coshq, self, args);
};

static PyObject * _cos(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_cosq, self, args);
};

static PyObject * _erf(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_erfq, self, args);
};

static PyObject * _erfc(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_erfcq, self, args);
};

static PyObject * _exp2(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_exp2q, self, args);
};

static PyObject * _exp(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_expq, self, args);
};

static PyObject * _expm1(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_expm1q, self, args);
};

static PyObject * _fabs(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_fabsq, self, args);
};

static PyObject * _fdim(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_fdimq, self, args);
};

static PyObject * _finite(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_finiteq, self, args);
};

static PyObject * _floor(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_floorq, self, args);
};

static PyObject * _fma(PyObject *self, PyObject *args){
    return QuadObject_qmath_op3(OP_fmaq, self, args);
};

static PyObject * _fmax(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_fmaxq, self, args);
};

static PyObject * _fmin(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_fminq, self, args);
};

static PyObject * _fmod(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_fmodq, self, args);
};

static PyObject * _frexp(PyObject *self, PyObject *args){
    QuadObject result;
    PyObject * obj1 = NULL;
    int eptr;

    if (!PyArg_ParseTuple(args, "O:", &obj1)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    result.value = frexpq(result.value, &eptr);

    return Py_BuildValue("(OO)", \
           QuadObject_to_PyObject(result),
           PyLong_FromLong(eptr)
    ); 

};

static PyObject * _hypot(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_hypotq, self, args);
};

static PyObject * _ilogb(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_ilogbq, self, args);
};

static PyObject * _isinf(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_isinfq, self, args);
};

static PyObject * _isnan(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_isnanq, self, args);
};

static PyObject * _issignaling(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_issignalingq, self, args);
};

static PyObject * _j0(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_j0q, self, args);
};

static PyObject * _j1(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_j1q, self, args);
};

static PyObject * _jn(PyObject *self, PyObject *args){
    QuadObject result;
    PyObject * obj1 = NULL,* obj2 = NULL;
    long n;

    if (!PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyLong_Check(obj1)){
        PyErr_SetString(PyExc_TypeError, "First argument must be a int");
        return NULL;
    }

    n = PyLong_AsLong(obj1);
    if(PyErr_Occurred()){
        PyErr_SetString(PyExc_TypeError, "First argument must be a int");
        return NULL; 
    }

    if(!PyObject_to_QuadObject(obj2, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    result.value = jnq(n, result.value);

    return QuadObject_to_PyObject(result);
};

static PyObject * _ldexp(PyObject *self, PyObject *args){
    QuadObject result;
    PyObject * obj1 = NULL,* obj2 = NULL;
    long n;

    if (!PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyLong_Check(obj2)){
        PyErr_SetString(PyExc_TypeError, "Second argument must be a int");
        return NULL;
    }

    n = PyLong_AsLong(obj2);
    if(PyErr_Occurred()){
        PyErr_SetString(PyExc_TypeError, "Second argument must be a int");
        return NULL; 
    }

    if(!PyObject_to_QuadObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    result.value = ldexpq(result.value, n);

    return QuadObject_to_PyObject(result);
};

static PyObject * _lgamma(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_lgammaq, self, args);
};

static PyObject * _llrint(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_llrintq, self, args);
};

static PyObject * _llround(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_llroundq, self, args);
};

static PyObject * _logb(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_logbq, self, args);
};

static PyObject * _log(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_logq, self, args);
};

static PyObject * _log10(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_log10q, self, args);
};

static PyObject * _log1p(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_log1pq, self, args);
};

static PyObject * _log2(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_log2q, self, args);
};

static PyObject * _lrint(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_lrintq, self, args);
};

static PyObject * _lround(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_lroundq, self, args);
};

static PyObject * _modf(PyObject *self, PyObject *args){
    QuadObject result, q1;
    PyObject * obj1 = NULL;

    if (!PyArg_ParseTuple(args, "O:", &obj1)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    alloc_QuadType(&q1);

    result.value = modfq(result.value, &q1.value);

    return Py_BuildValue("(OO)", \
           QuadObject_to_PyObject(result),
           QuadObject_to_PyObject(q1)
    ); 
};

static PyObject * _nearbyint(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_nearbyintq, self, args);
};

static PyObject * _nextafter(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_nextafterq, self, args);
};

static PyObject * _pow(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_powq, self, args);
};

static PyObject * _remainder(PyObject *self, PyObject *args){
    return QuadObject_qmath_op2(OP_remainderq, self, args);
};

static PyObject * _remquo(PyObject *self, PyObject *args){
    QuadObject result, q1;
    PyObject * obj1 = NULL, * obj2 = NULL;
    int quo;

    if (!PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj2, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert second value to quad precision.");
        return NULL;
    }

    result.value = remquoq(result.value, q1.value, &quo);

    return Py_BuildValue("(OO)", \
           QuadObject_to_PyObject(result),
           PyLong_FromLong(quo)
    ); 
};


static PyObject * _rint(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_rintq, self, args);
};

static PyObject * _round(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_roundq, self, args);
};

static PyObject * _scalbln(PyObject *self, PyObject *args){
    QuadObject result;
    PyObject * obj1 = NULL,* obj2 = NULL;
    long n;

    if (!PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyLong_Check(obj2)){
        PyErr_SetString(PyExc_TypeError, "Second argument must be a int");
        return NULL;
    }

    n = PyLong_AsLong(obj2);
    if(PyErr_Occurred()){
        PyErr_SetString(PyExc_TypeError, "Second argument must be a int");
        return NULL; 
    }

    if(!PyObject_to_QuadObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    result.value = scalbln(result.value, n);

    return QuadObject_to_PyObject(result);};

static PyObject * _scalbn(PyObject *self, PyObject *args){
    QuadObject result;
    PyObject * obj1 = NULL,* obj2 = NULL;
    long n;

    if (!PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyLong_Check(obj2)){
        PyErr_SetString(PyExc_TypeError, "Second argument must be a int");
        return NULL;
    }

    n = PyLong_AsLong(obj2);
    if(PyErr_Occurred()){
        PyErr_SetString(PyExc_TypeError, "Second argument must be a int");
        return NULL; 
    }

    if(!PyObject_to_QuadObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    result.value = scalbn(result.value, n);

    return QuadObject_to_PyObject(result);};

static PyObject * _signbit(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1_int(OP_signbitq, self, args);
};

static PyObject * _sincos(PyObject *self, PyObject *args){
    QuadObject result, q1, q2;
    PyObject * obj1 = NULL;

    if (!PyArg_ParseTuple(args, "O:", &obj1)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert argument to quad precision.");
        return NULL;
    }

    alloc_QuadType(&q1);
    alloc_QuadType(&q2);

    sincosq(result.value, &q1.value, &q2.value);

    return Py_BuildValue("(OO)", \
           QuadObject_to_PyObject(q1),
           QuadObject_to_PyObject(q2)
    ); 
};

static PyObject * _sinh(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_sinhq, self, args);
};

static PyObject * _sin(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_sinq, self, args);
};

static PyObject * _sqrt(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_sqrtq, self, args);
};

static PyObject * _tan(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_tanq, self, args);
};

static PyObject * _tanh(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_tanhq, self, args);
};

static PyObject * _tgamma(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_tgammaq, self, args);
};

static PyObject * _trunc(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_truncq, self, args);
};

static PyObject * _y0(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_y0q, self, args);
};

static PyObject * _y1(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_y1q, self, args);
};

static PyObject * _yn(PyObject *self, PyObject *args){
    QuadObject result;
    PyObject * obj1 = NULL,* obj2 = NULL;
    long n;

    if (!PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyLong_Check(obj1)){
        PyErr_SetString(PyExc_TypeError, "First argument must be a int");
        return NULL;
    }

    n = PyLong_AsLong(obj1);
    if(PyErr_Occurred()){
        PyErr_SetString(PyExc_TypeError, "First argument must be a int");
        return NULL; 
    }

    if(!PyObject_to_QuadObject(obj2, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    result.value = ynq(n, result.value);

    return QuadObject_to_PyObject(result);
};


static PyMethodDef QMathMethods[] = {
    {"_acos", (PyCFunction) _acos, METH_VARARGS, "acos"},
    {"_acosh", (PyCFunction) _acosh, METH_VARARGS, "acosh"},
    {"_asin", (PyCFunction) _asin, METH_VARARGS, "asin"},
    {"_asinh", (PyCFunction) _asinh, METH_VARARGS, "asinh"},
    {"_atan", (PyCFunction) _atan, METH_VARARGS, "atan"},
    {"_atanh", (PyCFunction) _atanh, METH_VARARGS, "atanh"},
    {"_atan2", (PyCFunction) _atan2, METH_VARARGS, "atan2"},
    {"_cbrt", (PyCFunction) _cbrt, METH_VARARGS, "cbrt"},
    {"_ceil", (PyCFunction) _ceil, METH_VARARGS, "ceil"},
    {"_copysign", (PyCFunction) _copysign, METH_VARARGS, "copysign"},
    {"_cosh", (PyCFunction) _cosh, METH_VARARGS, "cosh"},
    {"_cos", (PyCFunction) _cos, METH_VARARGS, "cos"},
    {"_erf", (PyCFunction) _erf, METH_VARARGS, "erf"},
    {"_erfc", (PyCFunction) _erfc, METH_VARARGS, "erfc"},
    {"_exp2", (PyCFunction) _exp2, METH_VARARGS, "exp2"},
    {"_exp", (PyCFunction) _exp, METH_VARARGS, "exp"},
    {"_expm1", (PyCFunction) _expm1, METH_VARARGS, "expm1"},
    {"_fabs", (PyCFunction) _fabs, METH_VARARGS, "fabs"},
    {"_fdim", (PyCFunction) _fdim, METH_VARARGS, "fdim"},
    {"_finite", (PyCFunction) _finite, METH_VARARGS, "finite"},
    {"_floor", (PyCFunction) _floor, METH_VARARGS, "floor"},
    {"_fma", (PyCFunction) _fma, METH_VARARGS, "fma"},
    {"_fmax", (PyCFunction) _fmax, METH_VARARGS, "fmax"},
    {"_fmin", (PyCFunction) _fmin, METH_VARARGS, "fmin"},
    {"_fmod", (PyCFunction) _fmod, METH_VARARGS, "fmod"},
    {"_frexp", (PyCFunction) _frexp, METH_VARARGS, "frexp"},
    {"_hypot", (PyCFunction) _hypot, METH_VARARGS, "hypot"},
    {"_ilogb", (PyCFunction) _ilogb, METH_VARARGS, "ilogb"},
    {"_isinf", (PyCFunction) _isinf, METH_VARARGS, "isinf"},
    {"_isnan", (PyCFunction) _isnan, METH_VARARGS, "isnan"},
    {"_issignaling", (PyCFunction) _issignaling, METH_VARARGS, "issignaling"},
    {"_j0", (PyCFunction) _j0, METH_VARARGS, "j0"},
    {"_j1", (PyCFunction) _j1, METH_VARARGS, "j1"},
    {"_jn", (PyCFunction) _jn, METH_VARARGS, "jn"},
    {"_ldexp", (PyCFunction) _ldexp, METH_VARARGS, "ldexp"},
    {"_lgamma", (PyCFunction) _lgamma, METH_VARARGS, "lgamma"},
    {"_llrint", (PyCFunction) _llrint, METH_VARARGS, "llrint"},
    {"_llround", (PyCFunction) _llround, METH_VARARGS, "llround"},
    {"_logb", (PyCFunction) _logb, METH_VARARGS, "logb"},
    {"_log", (PyCFunction) _log, METH_VARARGS, "log"},
    {"_log10", (PyCFunction) _log10, METH_VARARGS, "log10"},
    {"_log1p", (PyCFunction) _log1p, METH_VARARGS, "log1p"},
    {"_log2", (PyCFunction) _log2, METH_VARARGS, "log2"},
    {"_lrint", (PyCFunction) _lrint, METH_VARARGS, "lrint"},
    {"_lround", (PyCFunction) _lround, METH_VARARGS, "lround"},
    {"_modf", (PyCFunction) _modf, METH_VARARGS, "modf"},
    //{"_nan", (PyCFunction) _nan, METH_VARARGS, "nan"},
    {"_nearbyint", (PyCFunction) _nearbyint, METH_VARARGS, "nearbyint"},
    {"_nextafter", (PyCFunction) _nextafter, METH_VARARGS, "nextafter"},
    {"_pow", (PyCFunction) _pow, METH_VARARGS, "pow"},
    {"_remainder", (PyCFunction) _remainder, METH_VARARGS, "remainder"},
    {"_remquo", (PyCFunction) _remquo, METH_VARARGS, "remquo"},
    {"_rint", (PyCFunction) _rint, METH_VARARGS, "rint"},
    {"_round", (PyCFunction) _round, METH_VARARGS, "round"},
    {"_scalbln", (PyCFunction) _scalbln, METH_VARARGS, "scalbln"},
    {"_scalbn", (PyCFunction) _scalbn, METH_VARARGS, "scalbn"},
    {"_signbit", (PyCFunction) _signbit, METH_VARARGS, "signbit"},
    {"_sincos", (PyCFunction) _sincos, METH_VARARGS, "sincos"},
    {"_sinh", (PyCFunction) _sinh, METH_VARARGS, "sinh"},
    {"_sin", (PyCFunction) _sin, METH_VARARGS, "sin"},
    {"_sqrt", (PyCFunction) _sqrt, METH_VARARGS, "sqrt"},
    {"_tan", (PyCFunction) _tan, METH_VARARGS, "tan"},
    {"_tanh", (PyCFunction) _tanh, METH_VARARGS, "tanh"},
    {"_tgamma", (PyCFunction) _tgamma, METH_VARARGS, "tgamma"},
    {"_trunc", (PyCFunction) _trunc, METH_VARARGS, "trunc"},
    {"_y0", (PyCFunction) _y0, METH_VARARGS, "y0"},
    {"_y1", (PyCFunction) _y1, METH_VARARGS, "y1"},
    {"_yn", (PyCFunction) _yn, METH_VARARGS, "yn"},
    {NULL, NULL, 0, NULL}
};


PyModuleDef QMathModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquadp.qmathc",
    .m_doc = "Quad precision math library.",
    .m_size = -1,
    .m_methods = QMathMethods,
};

PyMODINIT_FUNC
PyInit_qmathc(void)
{

    PyObject *m;

    m = PyModule_Create(&QMathModule);
    if (m == NULL)
        return NULL;

    if (import_qfloat() < 0)
        return NULL;

    return m;
}