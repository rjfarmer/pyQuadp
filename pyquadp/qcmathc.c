// SPDX-License-Identifier: GPL-2.0+
#include "pyquadp.h"

#include "qcmathc.h"
#include "qfloat.h"
#include "qcmplx.h"


static PyObject * QuadCObject_qmath_op1_qfloat(const int op, PyObject * self, PyObject * args){
    QuadObject result;
    QuadCObject q1;
    PyObject * obj = NULL;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse object");
        return NULL;
    }

    if(!PyObject_to_QuadCObject(obj, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return NULL;
    }

    alloc_QuadType(&result);

    switch(op){
        case OP_cabsq:
            result.value = cabsq(q1.value); // hypot underthehood
            break;
        case OP_cargq:
            result.value = cargq(q1.value); // This is phase underthehood (or atan2)
            break;
        case OP_cimagq:
            result.value = cimagq(q1.value);
            break;
        case OP_crealq:
            result.value = crealq(q1.value);
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
        case OP_phase:
            result.value = cargq(result.value); // This is atan2 underthehood
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

static PyObject * QuadCObject_qcmath_op1_int(const int op, PyObject * self, PyObject * args){
    PyObject *result = NULL;
    PyObject * obj = NULL;
    QuadCObject q1;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse object");
        return NULL;
    }

    if(!PyObject_to_QuadCObject(obj, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return NULL;
    }

    switch(op){
        case OP_finitecq:
            result =  PyBool_FromLong(finiteq(crealq(q1.value))&& (finiteq(cimagq(q1.value))));
            break;
        case OP_isinfcq:
            result =  PyBool_FromLong(isinfq(crealq(q1.value))|| (isinfq(cimagq(q1.value))));
            break;
        case OP_isnancq:
            result =  PyBool_FromLong(isnanq(crealq(q1.value)) || (isnanq(cimagq(q1.value))));
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }


    return result;

}

static PyObject *
QuadCObject_qmath_op1_q_to_c(const int op, PyObject * self, PyObject * args){
    QuadObject q1;
    PyObject * obj = NULL;
    QuadCObject result;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse object");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return NULL;
    }

    alloc_QuadCType(&result);

    switch(op){
        case OP_cexpiq:
            result.value =  cexpiq(q1.value);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }


    return QuadCObject_to_PyObject(result);

}




static PyObject * _qcabs(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1_qfloat(OP_cabsq, self, args);
};

static PyObject * _qcarg(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1_qfloat(OP_cargq, self, args);
};

static PyObject * _qcimag(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1_qfloat(OP_cimagq, self, args);
};

static PyObject * _qcreal(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1_qfloat(OP_crealq, self, args);
};

static PyObject * _qcacosh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_cacoshq, self, args);
};

static PyObject * _qcacos(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_cacosq, self, args);
};

static PyObject * _qcasinh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_casinhq, self, args);
};

static PyObject * _qcasin(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_casinq, self, args);
};

static PyObject * _qcatanh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_catanhq, self, args);
};

static PyObject * _qcatan(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_catanq, self, args);
};

static PyObject * _qccos(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_ccosq, self, args);
};

static PyObject * _qccosh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_ccoshq, self, args);
};

static PyObject * _qcexp(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_cexpq, self, args);
};

static PyObject * _qcexpi(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1_q_to_c(OP_cexpiq, self, args);
};

static PyObject * _qclog(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_clogq, self, args);
};

static PyObject * _qclog10(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_clog10q, self, args);
};

static PyObject * _qconj(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_conjq, self, args);
};

static PyObject * _qcpow(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op2(OP_cpowq, self, args);
};

static PyObject * _qcproj(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_cprojq, self, args);
};

static PyObject * _qcsin(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_csinq, self, args);
};

static PyObject * _qcsinh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_csinhq, self, args);
};

static PyObject * _qcsqrt(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_csqrtq, self, args);
};

static PyObject * _qctan(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_ctanq, self, args);
};

static PyObject * _qctanh(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_ctanhq, self, args);
};

static PyObject * _qfinitecq(PyObject *self, PyObject *args){
    return QuadCObject_qcmath_op1_int(OP_finitecq, self, args);
};

static PyObject * _qisnancq(PyObject *self, PyObject *args){
    return QuadCObject_qcmath_op1_int(OP_isnancq, self, args);
};

static PyObject * _qisinfcq(PyObject *self, PyObject *args){
    return QuadCObject_qcmath_op1_int(OP_isinfcq, self, args);
};


static PyObject * _qphase(PyObject *self, PyObject *args){
    return QuadCObject_qmath_op1(OP_phase, self, args);
};

static PyObject * _qpolar(PyObject *self, PyObject *args){
    QuadCObject result;
    PyObject * obj1 = NULL;
    QuadObject absx, phasex;

    if (!PyArg_ParseTuple(args, "O:", &obj1)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyObject_to_QuadCObject(obj1, &result, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    absx.value = cabsq(result.value);

    phasex.value = cargq(result.value);

    return Py_BuildValue("(OO)", \
           QuadObject_to_PyObject(absx),
           QuadObject_to_PyObject(phasex)
    ); 
};

static PyObject * _qrect(PyObject *self, PyObject *args){
    QuadCObject result;
    PyObject * obj1 = NULL, * obj2 = NULL;
    QuadObject r, phi;

    if (!PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        PyErr_SetString(PyExc_ValueError, "Failed to parse arguments");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj1, &r, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert first value to quad precision.");
        return NULL;
    }

    if(!PyObject_to_QuadObject(obj2, &phi, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert second value to quad precision.");
        return NULL;
    }

    alloc_QuadCType(&result);

    __real__ result.value = r.value * cosq(phi.value); 
    __imag__ result.value = r.value * sinq(phi.value);

    return QuadCObject_to_PyObject(result);

};



static PyMethodDef QMathCMethods[] = {
    {"_cabs", (PyCFunction) _qcabs, METH_VARARGS, "cabs"},
    {"_carg", (PyCFunction) _qcarg, METH_VARARGS, "carg"},
    {"_cimag", (PyCFunction) _qcimag, METH_VARARGS, "cimag"},
    {"_creal", (PyCFunction) _qcreal, METH_VARARGS, "creal"},
    {"_cacosh", (PyCFunction) _qcacosh, METH_VARARGS, "cacosh"},
    {"_cacos", (PyCFunction) _qcacos, METH_VARARGS, "cacos"},
    {"_casinh", (PyCFunction) _qcasinh, METH_VARARGS, "casinh"},
    {"_casin", (PyCFunction) _qcasin, METH_VARARGS, "casin"},
    {"_catanh", (PyCFunction) _qcatanh, METH_VARARGS, "catanh"},
    {"_catan", (PyCFunction) _qcatan, METH_VARARGS, "catan"},
    {"_ccos", (PyCFunction) _qccos, METH_VARARGS, "ccos"},
    {"_ccosh", (PyCFunction) _qccosh, METH_VARARGS, "ccosh"},
    {"_cexp", (PyCFunction) _qcexp, METH_VARARGS, "cexp"},
    {"_cexpi", (PyCFunction) _qcexpi, METH_VARARGS, "cexpi"},
    {"_clog", (PyCFunction) _qclog, METH_VARARGS, "clog"},
    {"_clog10", (PyCFunction) _qclog10, METH_VARARGS, "clog10"},
    {"_conj", (PyCFunction) _qconj, METH_VARARGS, "conj"},
    {"_cpow", (PyCFunction) _qcpow, METH_VARARGS, "cpow"},
    {"_cproj", (PyCFunction) _qcproj, METH_VARARGS, "cproj"},
    {"_csin", (PyCFunction) _qcsin, METH_VARARGS, "csin"},
    {"_csinh", (PyCFunction) _qcsinh, METH_VARARGS, "csinh"},
    {"_csqrt", (PyCFunction) _qcsqrt, METH_VARARGS, "csqrt"},
    {"_ctan", (PyCFunction) _qctan, METH_VARARGS, "ctan"},
    {"_ctanh", (PyCFunction) _qctanh, METH_VARARGS, "ctanh"},
    {"_finitecq", (PyCFunction) _qfinitecq, METH_VARARGS, "isfinite"},
    {"_isnancq", (PyCFunction) _qisnancq, METH_VARARGS, "isnan"},
    {"_isinfcq", (PyCFunction) _qisinfcq, METH_VARARGS, "isinf"},
    {"_phase", (PyCFunction) _qphase, METH_VARARGS, "phase"},
    {"_polar", (PyCFunction) _qpolar, METH_VARARGS, "polar"},
    {"_rect", (PyCFunction) _qrect, METH_VARARGS, "rect"},
    {NULL, NULL, 0, NULL}
};


PyModuleDef QMathCModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "qcmathc",
    .m_doc = PyDoc_STR("Quad precision complex math library."),
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

    if (import_qmfloat() < 0)
        return NULL;

    if (import_qmcmplx() < 0)
        return NULL;

    return m;
}