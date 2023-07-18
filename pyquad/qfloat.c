// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>

#include "qdef.h"
#include "qfloat.h"


// Examples https://github.com/Zuzu-Typ/Python-C-API-extension-template/blob/master/template.cpp

static PyObject *
QuadObject_repr(QuadObject * obj)
{
    char buf[QUAD_BUF];

    int n = quadmath_snprintf (buf, sizeof buf, "%.36Qe", obj->value);
    if ((size_t) n < sizeof buf)
        return PyUnicode_FromFormat("qfloat('%s')",
                                buf);
    else
        return PyUnicode_FromFormat("%s","Bad quad");

}


static PyObject *
QuadObject_str(QuadObject * obj)
{
    char buf[QUAD_BUF];

    int n = quadmath_snprintf (buf, sizeof buf, "%.36Qe", obj->value);
    if ((size_t) n < sizeof buf)
        return PyUnicode_FromFormat("%s",buf);
    else
        return PyUnicode_FromFormat("%s","Bad quad");
}


static PyObject *
QuadObject_binary_op1(const int op, PyObject * o1){

    QuadObject q1, result;

    if(!PyObject_to_QuadObject(o1, &q1)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadType(&result);

    switch(op){
        case OP_negative:
            result.value= -1*(q1.value);
            break;
        case OP_positive:
            result.value= 1*q1.value;
            break;
        case OP_absolute:
            result.value= fabsq(q1.value);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadObject_to_PyObject(result);
}


static PyObject *
QuadObject_binary_op2(const int op, PyObject * o1, PyObject * o2 ){

    QuadObject q1, q2, result;

    if(!PyObject_to_QuadObject(o1, &q1)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadObject(o2, &q2)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadType(&result);

    switch(op){
        case OP_add:
            result.value = q1.value + q2.value;
            break;
        case OP_sub:
            result.value = q1.value - q2.value;
            break;
        case OP_mult:
            result.value = q1.value * q2.value;
            break;
        case OP_remainder:
            result.value = remainderq(q1.value, q2.value);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadObject_to_PyObject(result);
}



static PyObject *
QuadObject_add(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_op2(OP_add, o1, o2);
}


static PyObject *
QuadObject_subtract(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_op2(OP_sub, o1, o2);
}


static PyObject *
QuadObject_mult(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_op2(OP_mult, o1, o2);
}

static PyObject *
QuadObject_remainder(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_op2(OP_remainder, o1, o2);
}

static PyObject *
QuadObject_divmod(PyObject * o1, PyObject * o2 ){
    return Py_BuildValue("(OO)", \
           QuadObject_binary_op2(OP_floor_divide, o1, o2), \
           QuadObject_binary_op2(OP_remainder, o1, o2) \
    );
}

static PyObject *
QuadObject_pow(PyObject * o1, PyObject * o2, PyObject * o3 ){
    QuadObject q1, q2, q3, result;

    if(!PyObject_to_QuadObject(o1, &q1)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadObject(o2, &q2)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadType(&result);

    result.value = powq(q1.value, q2.value);

    if(o3 != Py_None) {
        if(!PyObject_to_QuadObject(o2, &q3))
            Py_RETURN_NOTIMPLEMENTED;

        result.value = fmodq(result.value, q3.value);
    }

    return QuadObject_to_PyObject(result);

}

static PyObject *
QuadObject_neg(PyObject * o1){
    return QuadObject_binary_op1(OP_negative, o1);
}

static PyObject *
QuadObject_pos(PyObject * o1){
    return QuadObject_binary_op1(OP_positive, o1);
}

static PyObject *
QuadObject_abs(PyObject * o1){
    return QuadObject_binary_op1(OP_absolute, o1);
}

static int QuadObject_bool(PyObject * o1){

    QuadObject q1;

    if(!PyObject_to_QuadObject(o1, &q1)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(q1.value==0)
        return 0;

    return 1;

}

static PyObject *
QuadObject_int(PyObject * o1){
    QuadObject q1;
    PyObject * result;

    if(!PyObject_to_QuadObject(o1, &q1)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    // TODO: Add error checking and switch to fromstring
   result = PyLong_FromDouble((double) q1.value);

   return result;
}

static PyObject *
QuadObject_float(PyObject * o1){
    QuadObject q1;
    PyObject * result;

    if(!PyObject_to_QuadObject(o1, &q1)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    // TODO: Add error checking and switch to fromstring
   result = PyFloat_FromDouble((double) q1.value);

   return result;
}



// Header data

static PyNumberMethods Quad_math_methods = {
    (binaryfunc) QuadObject_add,
    (binaryfunc) QuadObject_subtract,
    (binaryfunc) QuadObject_mult,
    (binaryfunc) QuadObject_remainder,
    (binaryfunc) QuadObject_divmod,
    (ternaryfunc) QuadObject_pow,
    (unaryfunc) QuadObject_neg,
    (unaryfunc) QuadObject_pos,
    (unaryfunc) QuadObject_abs,
    (inquiry) QuadObject_bool,
    0,//  unaryfunc nb_invert;
    0,//  binaryfunc nb_lshift;
    0,//  binaryfunc nb_rshift;
    0,//  binaryfunc nb_and;
    0,//  binaryfunc nb_xor;
    0,//  binaryfunc nb_or;
    (unaryfunc) QuadObject_int,//  unaryfunc nb_int;
    0,//  void *nb_reserved;
    (unaryfunc) QuadObject_float,//  unaryfunc nb_float;

    0,//  binaryfunc nb_inplace_add;
    0,//  binaryfunc nb_inplace_subtract;
    0,//  binaryfunc nb_inplace_multiply;
    0,//  binaryfunc nb_inplace_remainder;
    0,//  ternaryfunc nb_inplace_power;
    0,//  binaryfunc nb_inplace_lshift;
    0,//  binaryfunc nb_inplace_rshift;
    0,//  binaryfunc nb_inplace_and;
    0,//  binaryfunc nb_inplace_xor;
    0,//  binaryfunc nb_inplace_or;

    0,//  binaryfunc nb_floor_divide;
    0,//  binaryfunc nb_true_divide;
    0,//  binaryfunc nb_inplace_floor_divide;
    0,//  binaryfunc nb_inplace_true_divide;

    0,//  unaryfunc nb_index;

    0,//  binaryfunc nb_matrix_multiply;
    0,//  binaryfunc nb_inplace_matrix_multiply;
};




static PyMemberDef Quad_members[] = {
    {NULL}  /* Sentinel */
};


static PyMethodDef Quad_methods[] = {
    {NULL}  /* Sentinel */
};

int
Quad_init(QuadObject *self, PyObject *args, PyObject *kwds)
{
    PyObject * obj;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_Print();
        return -1;
    }
        
    if(!PyObject_to_QuadObject(obj, self)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return -1;
    }

    return 0;
}


static PyTypeObject QuadType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pyquad._qfloat",
    .tp_doc = PyDoc_STR("A single quad precision variable"),
    .tp_basicsize = sizeof(QuadObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_repr = (reprfunc) QuadObject_repr,
    .tp_str = (reprfunc) QuadObject_str,
    .tp_members = Quad_members,
    .tp_methods = Quad_methods,
    .tp_init = (initproc) Quad_init,
    .tp_as_number = &Quad_math_methods,
};

static PyModuleDef QuadModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquad.qfloat",
    .m_doc = "Quad precision module for scalar quad's.",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_qfloat(void)
{
    PyObject *m;
    if (PyType_Ready(&QuadType) < 0)
        return NULL;

    m = PyModule_Create(&QuadModule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&QuadType);
    if (PyModule_AddObject(m, "_qfloat", (PyObject *) &QuadType) < 0) {
        Py_DECREF(&QuadType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}


PyObject* 
QuadObject_to_PyObject(QuadObject out) {
	QuadObject* ret = (QuadObject*)QuadType.tp_alloc(&QuadType, 0);

	if (ret != NULL) {
		ret->value = out.value;
	}

	return (PyObject*) ret;
}


bool
PyObject_to_QuadObject(PyObject * in, QuadObject * out)
{
    alloc_QuadType(out);

    if(PyUnicode_Check(in) || PyObject_TypeCheck(in, &QuadType)){
        // Is a string

        PyObject * obj_str = PyObject_Str(in);
        if (obj_str==NULL){
            PyErr_Print();
            return false;
        }

        const char *buf = PyUnicode_AsUTF8(obj_str);
        if (buf==NULL){
            PyErr_Print();
            Py_DECREF(obj_str);
            return false;
        }

        if(strcmp(buf,"nan")==0) {
            out->value = nanq("");
        } else {
            char **sp;

            out->value = strtoflt128(buf, &sp);
            if(sp!=NULL){
                if(strcmp(sp,"")){
                    return false;
                }
            }
        } 

        return true;
    }

    if(PyNumber_Check(in)) {
        // Is a number
        
        if(PyLong_Check(in)){
            // int
            out->value = (__float128) PyLong_AsLong(in);
            return true;
        }

        if(PyFloat_Check(in)){
            // float
            out->value = (__float128) PyFloat_AsDouble(in);
            return true;
        }


        return false;
    }


    return false;

}

void qprintf(QuadObject * out){
    char output[QUAD_BUF];
    quadmath_snprintf(output, sizeof output, "%.36Qg", 35, out->value);
    printf("%s\n", output);
}

void alloc_QuadType(QuadObject * result){
	result = (QuadObject*)QuadType.tp_alloc(&QuadType, 0);
}