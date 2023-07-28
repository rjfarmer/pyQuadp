// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define QCMPLX_MODULE
#include "qcmplx.h"
#include "qfloat.h"


static PyObject *
QuadCObject_repr(QuadCObject * obj)
{
    char buf1[QUAD_BUF],buf2[QUAD_BUF];

    int n1 = quadmath_snprintf (buf1, sizeof buf1, "%.36Qe", obj->real);
    int n2 = quadmath_snprintf (buf2, sizeof buf2, "%.36Qe", obj->imag);
    if ((size_t) n1 < sizeof buf1 && (size_t) n2 < sizeof buf2 )
        return PyUnicode_FromFormat("qcmplx('%s+%sj')",
                                buf1,buf2);
    else
        return PyUnicode_FromFormat("%s","Bad complex quad");

}


static PyObject *
QuadCObject_str(QuadCObject * obj)
{
    char buf1[QUAD_BUF],buf2[QUAD_BUF];

    int n1 = quadmath_snprintf (buf1, sizeof buf1, "%.36Qe", obj->real);
    int n2 = quadmath_snprintf (buf2, sizeof buf2, "%.36Qe", obj->imag);
    if ((size_t) n1 < sizeof buf1 && (size_t) n2 < sizeof buf2 )
        return PyUnicode_FromFormat("(%s+%sj)",buf1,buf2);
    else
        return PyUnicode_FromFormat("%s","Bad complex quad");

}

static PyObject *
QuadCObject_binary_op1(const int op, PyObject * o1){

    QuadCObject q1, result;

    if(!PyObject_to_QuadCObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadCType(&result);

    switch(op){
        case OP_negative:
            result.real = -1*(q1.real);
            result.imag = -1*(q1.imag);
            break;
        case OP_positive:
            result.real = 1*(q1.real);
            result.imag = 1*(q1.imag);
            break;
        case OP_absolute:
            result.real = fabsq(q1.real);
            result.imag = fabsq(q1.imag);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadCObject_to_PyObject(result);
}


static PyObject *
QuadCObject_binary_op2(const int op, PyObject * o1, PyObject * o2 ){

    QuadCObject q1, q2, result;
    __float128 num;

    if(!PyObject_to_QuadCObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadCObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadCType(&result);

    switch(op){
        case OP_add:
            result.real = q1.real + q2.real;
            result.imag = q1.imag + q2.imag;
            break;
        case OP_sub:
            result.real = q1.real - q2.real;
            result.imag = q1.imag - q2.imag;
            break;
        case OP_mult:
            result.real = q1.real * q2.real - q1.imag * q2.imag;
            result.imag = q1.imag * q2.imag - q1.real * q2.real;
            break;
        case OP_true_divide:
            num = powq(q1.real,2) + powq(q1.imag,2);
            result.real = (q1.real*q2.real + q1.imag*q2.imag)/num;
            result.imag = (q1.imag*q2.real - q1.real*q2.imag)/num;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadCObject_to_PyObject(result);
}


static PyObject *
QuadCObject_binary_self_op2(const int op, PyObject * o1, PyObject * o2 ){

    QuadCObject q1, q2;
    __float128 num, real, imag;

    if(!PyObject_to_QuadCObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadCObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    switch(op){
        case OP_add:
            q1.real = q1.real + q2.real;
            q1.imag = q1.imag + q2.imag;
            break;
        case OP_sub:
            q1.real = q1.real - q2.real;
            q1.imag = q1.imag - q2.imag;
            break;
        case OP_mult:
            real = q1.real;
            imag = q1.imag;

            q1.real = real * q2.real - imag * q2.imag;
            q1.imag = imag * q2.imag - real * q2.real;
            break;
        case OP_true_divide:
            real = q1.real;
            imag = q1.imag;

            num = powq(real,2) + powq(imag,2);
            q1.real = (real*q2.real + imag*q2.imag)/num;
            q1.imag = (imag*q2.real - real*q2.imag)/num;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadCObject_to_PyObject(q1);
}



static PyObject *
QuadCObject_add(PyObject * o1, PyObject * o2 ){
    return QuadCObject_binary_op2(OP_add, o1, o2);
}


static PyObject *
QuadCObject_subtract(PyObject * o1, PyObject * o2 ){
    return QuadCObject_binary_op2(OP_sub, o1, o2);
}


static PyObject *
QuadCObject_mult(PyObject * o1, PyObject * o2 ){
    return QuadCObject_binary_op2(OP_mult, o1, o2);
}

static PyObject *
QuadCObject_pow(PyObject * o1, PyObject * o2, PyObject * o3 ){
    // QuadCObject q1, q2, q3, result;

    // if(!PyObject_to_QuadObject(o1, &q1, true)){
    //     Py_RETURN_NOTIMPLEMENTED;
    // }

    // if(!PyObject_to_QuadObject(o2, &q2, true)){
    //     Py_RETURN_NOTIMPLEMENTED;
    // }

    // alloc_QuadType(&result);

    // result.value = powq(q1.value, q2.value);

    // if(o3 != Py_None) {
    //     if(!PyObject_to_QuadObject(o2, &q3, true))
    //         Py_RETURN_NOTIMPLEMENTED;

    //     result.value = fmodq(result.value, q3.value);
    // }

    // return QuadCObject_to_PyObject(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *
QuadCObject_neg(PyObject * o1){
    return QuadCObject_binary_op1(OP_negative, o1);
}

static PyObject *
QuadCObject_pos(PyObject * o1){
    return QuadCObject_binary_op1(OP_positive, o1);
}

static PyObject *
QuadCObject_abs(PyObject * o1){
    return QuadCObject_binary_op1(OP_absolute, o1);
}

static int QuadCObject_bool(PyObject * o1){

    QuadCObject q1;

    if(!PyObject_to_QuadCObject(o1, &q1, true)){
        return 0;
    }

    if(q1.real==0 && q1.imag==0)
        return 0;

    return 1;

}

static PyObject *
QuadCObject_inplace_add(PyObject * o1, PyObject * o2 ){
    return QuadCObject_binary_self_op2(OP_add, o1, o2);
}


static PyObject *
QuadCObject_inplace_subtract(PyObject * o1, PyObject * o2 ){
    return QuadCObject_binary_self_op2(OP_sub, o1, o2);
}


static PyObject *
QuadCObject_inplace_mult(PyObject * o1, PyObject * o2 ){
    return QuadCObject_binary_self_op2(OP_mult, o1, o2);
}

static PyObject *
QuadCObject_inplace_pow(PyObject * o1, PyObject * o2, PyObject * o3 ){
    // QuadCObject q1, q2, q3;

    // if(!PyObject_to_QuadObject(o1, &q1,true)){
    //     Py_RETURN_NOTIMPLEMENTED;
    // }

    // if(!PyObject_to_QuadObject(o2, &q2,true)){
    //     Py_RETURN_NOTIMPLEMENTED;
    // }


    // q1.value = powq(q1.value, q2.value);

    // if(o3 != Py_None) {
    //     if(!PyObject_to_QuadObject(o2, &q3,true))
    //         Py_RETURN_NOTIMPLEMENTED;

    //     q1.value = fmodq(q1.value, q3.value);
    // }

    // return QuadCObject_to_PyObject(q1);

    Py_RETURN_NOTIMPLEMENTED;

}


static PyObject *
QuadCObject_true_divide(PyObject * o1, PyObject * o2 ){
    return QuadCObject_binary_op2(OP_true_divide, o1, o2);
}

static PyObject *
QuadCObject_inplace_true_divide(PyObject * o1, PyObject * o2 ){
    return QuadCObject_binary_self_op2(OP_true_divide, o1, o2);
}



// Header data

static PyNumberMethods Quad_cmath_methods = {
    (binaryfunc) QuadCObject_add,
    (binaryfunc) QuadCObject_subtract,
    (binaryfunc) QuadCObject_mult,
    0, //(binaryfunc) QuadCObject_remainder,
    0, // QuadCObject_divmod,
    (ternaryfunc) QuadCObject_pow,
    (unaryfunc) QuadCObject_neg,
    (unaryfunc) QuadCObject_pos,
    (unaryfunc) QuadCObject_abs,
    (inquiry) QuadCObject_bool,
    0,//  unaryfunc nb_invert;
    0,//  binaryfunc nb_lshift;
    0,//  binaryfunc nb_rshift;
    0,//  binaryfunc nb_and;
    0,//  binaryfunc nb_xor;
    0,//  binaryfunc nb_or;
    0,//  binaryfunc nb_int;
    0,//  void *nb_reserved;
    0,//  binaryfunc nb_float;

    (binaryfunc) QuadCObject_inplace_add,
    (binaryfunc) QuadCObject_inplace_subtract,
    (binaryfunc) QuadCObject_inplace_mult,
    0,//(binaryfunc) QuadCObject_inplace_remainder,
    (ternaryfunc) QuadCObject_inplace_pow,
    0,//  binaryfunc nb_inplace_lshift;
    0,//  binaryfunc nb_inplace_rshift;
    0,//  binaryfunc nb_inplace_and;
    0,//  binaryfunc nb_inplace_xor;
    0,//  binaryfunc nb_inplace_or;

    0, //(binaryfunc) QuadCObject_floor_divide,//  binaryfunc nb_floor_divide;
    (binaryfunc) QuadCObject_true_divide,//  binaryfunc nb_true_divide;
    0, //(binaryfunc) QuadCObject_inplace_floor_divide,//  binaryfunc nb_inplace_floor_divide;
    (binaryfunc) QuadCObject_inplace_true_divide,//  binaryfunc nb_inplace_true_divide;

    0,//(unaryfunc) QuadCObject_int,//  unaryfunc nb_index;

    0,//  binaryfunc nb_matrix_multiply;
    0,//  binaryfunc nb_inplace_matrix_multiply;
};


PyObject* get_real(PyObject * x, void * y){
    QuadCObject val;
    QuadObject q1;

    if(!PyObject_to_QuadCObject(x, &val, true)){
        return NULL;
    }

    alloc_QuadType(&q1);

    q1.value = val.real;

    return QuadObject_to_PyObject(q1);

}

PyObject* get_imag(PyObject * x, void * y){
    QuadCObject val;
    QuadObject q1;

    if(!PyObject_to_QuadCObject(x, &val, true)){
        return NULL;
    }

    alloc_QuadType(&q1);

    q1.value = val.imag;

    return QuadObject_to_PyObject(q1);
    
}

static PyMemberDef Quad_cmembers[] = {
    {NULL}  /* Sentinel */
};


static PyMethodDef Quad_cmethods[] = {
    {NULL}  /* Sentinel */
};

static PyGetSetDef Quad_cgetset[] = {
    {"real", get_real, NULL, "Real component" },
    {"imag", get_imag, NULL, "Imaginary component" },
    {NULL}  /* Sentinel */
};


int
Quad_cinit(QuadCObject *self, PyObject *args, PyObject *kwds)
{
    PyObject * obj1=NULL,*obj2=NULL;

    if(PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        if(!PyObject_to_QuadCObject2(obj1, obj2, self, true)){
            PyErr_SetString(PyExc_TypeError, "Can not convert both values to quad complex precision.");
            return -1;
        } else {
            return 0;
        }
    } else {
        // Ignore error, might be 1 argument form
        PyErr_Clear();
    }


    if (PyArg_ParseTuple(args, "O:", &obj1)){
        if(!PyObject_to_QuadCObject(obj1, self, true)){
            PyErr_SetString(PyExc_TypeError, "Can not convert single value to quad complex precision.");
            return -1;
        }
    } else {
        PyErr_SetString(PyExc_TypeError,"qcmplx takes between 1 and 2 arguments");
        return -1;
    }
        
    return 0;
}


static PyTypeObject QuadCType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pyquad._qcmplx",
    .tp_doc = PyDoc_STR("A single quad precision complex variable"),
    .tp_basicsize = sizeof(QuadCObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_repr = (reprfunc) QuadCObject_repr,
    .tp_str = (reprfunc) QuadCObject_str,
    .tp_members = Quad_cmembers,
    .tp_methods = Quad_cmethods,
    .tp_init = (initproc) Quad_cinit,
    .tp_as_number = &Quad_cmath_methods,
    .tp_getset = Quad_cgetset,
    //.tp_richcompare = (richcmpfunc) QuadCType_RichCompare,
};

static PyModuleDef QuadCModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qcmplx",
    .m_doc = "Quad precision module for complex quad's.",
    .m_size = -1,
};

PyObject* 
QuadCObject_to_PyObject(QuadCObject out) {
	QuadCObject* ret = (QuadCObject*)QuadCType.tp_alloc(&QuadCType, 0);

	if (ret != NULL) {
		ret->real = out.real;
        ret->imag = out.imag;
	}

	return (PyObject*) ret;
}


static __float128 QuadCObject_float128_real(QuadCObject * out) {
    return out->real;
}

static __float128 QuadCObject_float128_imag(QuadCObject * out) {
    return out->imag;
}


bool
PyObject_to_QuadCObject(PyObject * in, QuadCObject * out, const bool alloc)
{
    if(alloc)
        alloc_QuadCType(out);

    //printf("%d\n",PyObject_TypeCheck(in, &QuadCType));

    if(PyObject_TypeCheck(in, &QuadCType)){
        // Is a complex quad
        out->real = QuadCObject_float128_real((QuadCObject *) in);
        out->imag = QuadCObject_float128_imag((QuadCObject *) in);
        return true;
    }

    if(QuadObject_Check(in)){
        // Is a quad
        out->real = QuadObject_float128((QuadObject *) in);
        out->imag = 0;
        return true;
    }

    if(PyComplex_Check(in)){
        out->real = (__float128) PyComplex_RealAsDouble(in);
        out->imag = (__float128) PyComplex_ImagAsDouble(in);
        return true;
    }

    return false;

}

bool
PyObject_to_QuadCObject2(PyObject * in1,PyObject * in2, QuadCObject * out, const bool alloc)
{
    bool set1=false,set2=false;
    QuadObject q1,q2;


    if(alloc)
        alloc_QuadCType(out);

    out->real = 0;
    out->imag = 0;

    if(QuadObject_Check(in1)){
        // Is a quad
        out->real = QuadObject_float128((QuadObject *) in1);
        set1 = true;
    }

    if(!set1){
        if(PyObject_to_QuadObject(in1, &q1,true)){
            out->real = QuadObject_float128(&q1);
            set1=true;
        }
    }
    
    if(QuadObject_Check(in2)){
        // Is a quad
        out->imag = QuadObject_float128((QuadObject *) in2);
        set2 = true;
    }

    if(!set2){
        if(PyObject_to_QuadObject(in2, &q2,true)){
            out->imag = QuadObject_float128(&q2);
            set2=true;
        }
    }

    if(set1 && set2)
        return true;

    if(set1)
        return true;

    return false;

}




#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void qcprintf(QuadCObject * out){
    char output1[QUAD_BUF], output2[QUAD_BUF];
    quadmath_snprintf(output1, sizeof output1, "%.36Qg", 35, out->real);
    quadmath_snprintf(output2, sizeof output2, "%.36Qg", 35, out->imag);
    printf("%s %s\n", output1,output2);
}
#pragma GCC diagnostic pop

static void alloc_QuadCType(QuadCObject * result){
	result = (QuadCObject*)QuadCType.tp_alloc(&QuadCType, 0);
    Py_INCREF(result);
}

PyMODINIT_FUNC
PyInit_qcmplx(void)
{
    PyObject *m;
    static void *PyQcmplx_API[PyQcmplx_API_pointers];
    PyObject *c_api_object;


    if (PyType_Ready(&QuadCType) < 0)
        return NULL;

    m = PyModule_Create(&QuadCModule);
    if (m == NULL)
        return NULL;

    /* Initialize the C API pointer array */
    PyQcmplx_API[PyQcmplx_q2py_NUM] = (void *)QuadCObject_to_PyObject;
    PyQcmplx_API[PyQcmplx_py2q_NUM] = (void *)PyObject_to_QuadCObject;
    PyQcmplx_API[PyQcmplx_alloc_NUM] = (void *)alloc_QuadCType;

    Py_INCREF(&QuadCType);
    if (PyModule_AddObject(m, "_qcmplx", (PyObject *) &QuadCType) < 0) {
        Py_DECREF(&QuadCType);
        Py_DECREF(m);
        return NULL;
    }


    /* Create a Capsule containing the API pointer array's address */
    c_api_object = PyCapsule_New((void *)PyQcmplx_API, "qcmplx._C_API", NULL);

    if (PyModule_AddObject(m, "_C_API", c_api_object) < 0) {
        Py_XDECREF(c_api_object);
        Py_DECREF(m);
        return NULL;
    }

    if (import_qfloat() < 0)
        return NULL;


    return m;
}
