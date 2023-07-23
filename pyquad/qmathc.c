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
        // case OP_atan2q:
        // Two arguments
        //     break;
        case OP_cbrtq:
            result.value = cbrtq(result.value);
            break;
        case OP_ceilq:
            result.value = ceilq(result.value);
            break;
        // case OP_copysignq:
        // two arguments
        //     break;
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
            result.value = exp2q(result.value);
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
        // case OP_fdimq:
        // two arguments
        //     break;
        // case OP_finiteq:
        // returns int
        //     break;
        case OP_floorq:
            result.value = floorq(result.value);
            break;
        // case OP_fmaq:
        //     break;
        // case OP_fmaxq:
        //     break;
        // case OP_fminq:
        //     break;
        // case OP_fmodq:
        //     break;
        // case OP_frexpq:
        //     break;
        // case OP_hypotq:
        //     break;
        // case OP_ilogbq:
        //     break;
        // case OP_isinfq:
        //     break;
        // case OP_isnanq:
        //     break;
        // case OP_issignalingq:
        //     break;
        case OP_j0q:
            result.value = j0q(result.value);
            break;
        case OP_j1q:
            result.value = j1q(result.value);
            break;
        // case OP_jnq:
        //     break;
        // case OP_ldexpq:
        //     break;
        case OP_lgammaq:
            result.value = lgammaq(result.value);
            break;
        // case OP_llrintq:
        //     break;
        // case OP_llroundq:
        //     break;
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
        // case OP_lrintq:
        //     break;
        // case OP_lroundq:
        //     break;
        // case OP_modfq:
        //     break;
        // case OP_nanq:
        //     break;
        case OP_nearbyintq:
            result.value = nearbyintq(result.value);
            break;
        // case OP_nextafterq:
        //     break;
        // case OP_powq:
        //     break;
        // case OP_remainderq:
        //     break;
        // case OP_remquoq:
        //     break;
        case OP_rintq:
            result.value = rintq(result.value);
            break;
        case OP_roundq:
            result.value = roundq(result.value);
            break;
        // case OP_scalblnq:
        //     break;
        // case OP_scalbnq:
        //     break;
        // case OP_signbitq:
        //     break;
        // case OP_sincosq:
        //     break;
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
        // case OP_ynq:
        //     break;

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
        // case OP_finiteq:
        //     break;
        // case OP_fmaq:
        //     break;
        case OP_fmaxq:
            result.value = fmaxq(result.value, q1.value);
            break;
        case OP_fminq:
            result.value = fminq(result.value, q1.value);
            break;
        case OP_fmodq:
            result.value = fmodq(result.value, q1.value);
            break;
        // case OP_frexpq:
        //     break;
        case OP_hypotq:
            result.value = hypotq(result.value, q1.value);
            break;
        // case OP_ilogbq:
        //     break;
        // case OP_isinfq:
        //     break;
        // case OP_isnanq:
        //     break;
        // case OP_issignalingq:
        //     break;
        // case OP_jnq:
        //     break;
        // case OP_ldexpq:
        //     break;
        // case OP_llrintq:
        //     break;
        // case OP_llroundq:
        //     break;
        // case OP_lrintq:
        //     break;
        // case OP_lroundq:
        //     break;
        // case OP_modfq:
        //     break;
        // case OP_nanq:
        //     break;
        case OP_nextafterq:
            result.value = nextafterq(result.value, q1.value);
            break;
        case OP_powq:
            result.value = powq(result.value, q1.value);
            break;
        case OP_remainderq:
            result.value = remainderq(result.value, q1.value);
            break;
        // case OP_remquoq:
        //     break;
        // case OP_scalblnq:
        //     break;
        // case OP_scalbnq:
        //     break;
        // case OP_signbitq:
        //     break;
        // case OP_sincosq:
        //     break;
        // case OP_ynq:
        //     break;

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



static PyObject * _cos(PyObject *self, PyObject *args){
    return QuadObject_qmath_op1(OP_cosq, self, args);
}


static PyMethodDef QMathMethods[] = {
    {"_cos", (PyCFunction) _cos, METH_VARARGS, "cos"},
    {NULL, NULL, 0, NULL}
};


PyModuleDef QMathModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qmathc",
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