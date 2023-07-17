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
    char buf[128];

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
    char buf[128];

    int n = quadmath_snprintf (buf, sizeof buf, "%.36Qe", obj->value);
    if ((size_t) n < sizeof buf)
        return PyUnicode_FromFormat("%s",buf);
    else
        return PyUnicode_FromFormat("%s","Bad quad");
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
        case OP_ADD:
            qprintf(&q1);
            qprintf(&q2);

            result.value = q1.value + q2.value;
            break;
        case OP_SUB:
            result.value = q1.value - q2.value;
            break;
        case OP_MULT:
            result.value = q1.value * q2.value;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadObject_to_PyObject(result);
}




static PyObject *
QuadObject_add(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_op2(OP_ADD, o1, o2);
}


static PyObject *
QuadObject_subtract(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_op2(OP_SUB, o1, o2);
}


static PyObject *
QuadObject_mult(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_op2(OP_MULT, o1, o2);
}


// Header data

static PyNumberMethods Quad_math_methods = {
    (binaryfunc) QuadObject_add,
    (binaryfunc) QuadObject_subtract,
    (binaryfunc) QuadObject_mult,
    //  binaryfunc nb_remainder;
    //  binaryfunc nb_divmod;
    //  ternaryfunc nb_power;
    //  unaryfunc nb_negative;
    //  unaryfunc nb_positive;
    //  unaryfunc nb_absolute;
    //  inquiry nb_bool;
    //  unaryfunc nb_invert;
    //  binaryfunc nb_lshift;
    //  binaryfunc nb_rshift;
    //  binaryfunc nb_and;
    //  binaryfunc nb_xor;
    //  binaryfunc nb_or;
    //  unaryfunc nb_int;
    //  void *nb_reserved;
    //  unaryfunc nb_float;

    //  binaryfunc nb_inplace_add;
    //  binaryfunc nb_inplace_subtract;
    //  binaryfunc nb_inplace_multiply;
    //  binaryfunc nb_inplace_remainder;
    //  ternaryfunc nb_inplace_power;
    //  binaryfunc nb_inplace_lshift;
    //  binaryfunc nb_inplace_rshift;
    //  binaryfunc nb_inplace_and;
    //  binaryfunc nb_inplace_xor;
    //  binaryfunc nb_inplace_or;

    //  binaryfunc nb_floor_divide;
    //  binaryfunc nb_true_divide;
    //  binaryfunc nb_inplace_floor_divide;
    //  binaryfunc nb_inplace_true_divide;

    //  unaryfunc nb_index;

    //  binaryfunc nb_matrix_multiply;
    //  binaryfunc nb_inplace_matrix_multiply;
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
    char *buf;

    if (!PyArg_ParseTuple(args, "s:", &buf))
        return -1;

    self->value = strtoflt128(buf, NULL);

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

	alloc_QuadType(out);
    Py_DECREF(obj_str);

    out->value = strtoflt128(buf, NULL);  
    qprintf(out);

    return true;
}

void qprintf(QuadObject * out){
    char output[128];
    quadmath_snprintf(output, sizeof output, "%.36Qg", 35, out->value);
    printf("%s\n", output);
}

void alloc_QuadType(QuadObject * result){
	result = (QuadObject*)QuadType.tp_alloc(&QuadType, 0);
}