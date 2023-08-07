// SPDX-License-Identifier: GPL-2.0+
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define QINT_MODULE
#include "qint.h"



static PyObject *
QuadIObject_repr(QuadIObject * obj)
{
    char buf[QUAD_INT_STR_BUF];

    if(int128_to_str(obj->value, buf, QUAD_INT_STR_BUF, 10)){
        return PyUnicode_FromFormat("qint('%s')",buf);
    } else {
        return PyUnicode_FromFormat("Bad qint",buf);
    }
}


static PyObject *
QuadIObject_str(QuadIObject * obj)
{
    char buf[QUAD_INT_STR_BUF];

    if(int128_to_str(obj->value, buf, QUAD_INT_STR_BUF, 10)){
        return PyUnicode_FromFormat("%s",buf);
    } else {
        return PyUnicode_FromFormat("Bad qint",buf);
    }
}


static PyObject *
QuadIObject_binary_op1(const int op, PyObject * o1){

    QuadIObject q1, result;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadIType(&result);

    switch(op){
        case OP_negative:
            result.value= -1*(q1.value);
            break;
        case OP_positive:
            result.value= 1*q1.value;
            break;
        case OP_absolute:
            result.value= fabs(q1.value);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadIObject_to_PyObject(result);
}


static PyObject *
QuadIObject_binary_op2(const int op, PyObject * o1, PyObject * o2 ){

    QuadIObject q1, q2, result;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadIObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadIType(&result);

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
            result.value = remainder(q1.value, q2.value);
            break;
        case OP_floor_divide:
            result.value = floor(q1.value/q2.value);
            break;
        case OP_true_divide:
            result.value = q1.value/q2.value;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadIObject_to_PyObject(result);
}


static PyObject *
QuadIObject_binary_inplace_op2(const int op, PyObject * o1, PyObject * o2 ){

    QuadIObject q1, q2;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadIObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    switch(op){
        case OP_add:
            q1.value = q1.value + q2.value;
            break;
        case OP_sub:
            q1.value = q1.value - q2.value;
            break;
        case OP_mult:
            q1.value = q1.value * q2.value;
            break;
        case OP_remainder:
            q1.value = remainder(q1.value, q2.value);
            break;
        case OP_floor_divide:
            q1.value = floor(q1.value/q2.value);
            break;
        case OP_true_divide:
            q1.value = q1.value/q2.value;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadIObject_to_PyObject(q1);
}

static PyObject *
QuadIObject_binary_op2_int(const int op, PyObject * o1, PyObject * o2 ){

    QuadIObject q1, result;
    long int i;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyLong_Check(o2)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadIType(&result);

    i = PyLong_AsLong(o2);

    switch(op){
        case OP_lshift:
            result.value = q1.value << i;
            break;
        case OP_rshift:
            result.value = q1.value >> i;
            break;
        case OP_and:
            result.value = q1.value & i;
            break;
        case OP_xor:
            result.value = q1.value ^ i;
            break;
        case OP_or:
            result.value = q1.value | i;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadIObject_to_PyObject(result);
}

static PyObject *
QuadIObject_lshift(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2_int(OP_lshift, o1, o2);
}

static PyObject *
QuadIObject_rshift(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2_int(OP_rshift, o1, o2);
}

static PyObject *
QuadIObject_and(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2_int(OP_and, o1, o2);
}

static PyObject *
QuadIObject_xor(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2_int(OP_xor, o1, o2);
}

static PyObject *
QuadIObject_or(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2_int(OP_or, o1, o2);
}



static PyObject *
QuadIObject_binary_inplace_op2_int(const int op, PyObject * o1, PyObject * o2 ){

    QuadIObject q1;
    long int i;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyLong_Check(o2)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    i = PyLong_AsLong(o2);

    switch(op){
        case OP_inplace_lshift:
            q1.value <<= i;
            break;
        case OP_inplace_rshift:
            q1.value >>= i;
            break;
        case OP_inplace_and:
            q1.value &= i;
            break;
        case OP_inplace_xor:
            q1.value ^= i;
            break;
        case OP_inplace_or:
            q1.value |= i;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadIObject_to_PyObject(q1);
}


static PyObject *
QuadIObject_inplace_lshift(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_lshift, o1, o2);
}

static PyObject *
QuadIObject_inplace_rshift(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_rshift, o1, o2);
}

static PyObject *
QuadIObject_inplace_and(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_and, o1, o2);
}

static PyObject *
QuadIObject_inplace_xor(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_xor, o1, o2);
}

static PyObject *
QuadIObject_inplace_or(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_or, o1, o2);
}




static PyObject *
QuadIObject_add(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2(OP_add, o1, o2);
}


static PyObject *
QuadIObject_subtract(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2(OP_sub, o1, o2);
}


static PyObject *
QuadIObject_mult(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2(OP_mult, o1, o2);
}

static PyObject *
QuadIObject_remainder(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2(OP_remainder, o1, o2);
}

static PyObject *
QuadIObject_divmod(PyObject * o1, PyObject * o2 ){
    return Py_BuildValue("(OO)", \
           QuadIObject_binary_op2(OP_floor_divide, o1, o2), \
           QuadIObject_binary_op2(OP_remainder, o1, o2) \
    );
}

static PyObject *
QuadIObject_pow(PyObject * o1, PyObject * o2, PyObject * o3 ){
    QuadIObject q1, q2, q3, result;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadIObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadIType(&result);

    result.value = powq(q1.value, q2.value);

    if(o3 != Py_None) {
        if(!PyObject_to_QuadIObject(o2, &q3, true))
            Py_RETURN_NOTIMPLEMENTED;

        result.value = fmodq(result.value, q3.value);
    }

    return QuadIObject_to_PyObject(result);

}

static PyObject *
QuadIObject_neg(PyObject * o1){
    return QuadIObject_binary_op1(OP_negative, o1);
}

static PyObject *
QuadIObject_pos(PyObject * o1){
    return QuadIObject_binary_op1(OP_positive, o1);
}

static PyObject *
QuadIObject_abs(PyObject * o1){
    return QuadIObject_binary_op1(OP_absolute, o1);
}

static int QuadIObject_bool(PyObject * o1){

    QuadIObject q1;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        return 0;
    }

    if(q1.value==0)
        return 0;

    return 1;

}

static PyObject * QuadIObject_invert(PyObject * o1){

    QuadIObject q1;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert to quad int");
        return NULL;
    }

    q1.value = ~q1.value;

    return QuadIObject_to_PyObject(q1);
}

static PyObject *
QuadIObject_int(PyObject * o1){
    QuadIObject q1;
    PyObject * result;

    if(!PyObject_to_QuadIObject(o1, &q1,true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(q1.value <= LLONG_MAX && q1.value >= LLONG_MIN){
        result = PyLong_FromLongLong(q1.value);
        if(PyErr_Occurred()){
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_ValueError, "Value too large for a python int.");
        return NULL;
    }

   return result;
}

static PyObject *
QuadIObject_float(PyObject * o1){
    QuadIObject q1;
    PyObject * result;
    double d;

    if(!PyObject_to_QuadIObject(o1, &q1,true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

   d = (double) q1.value;

   result = PyFloat_FromDouble(d);

   return result;
}


static PyObject *
QuadIObject_inplace_add(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2(OP_add, o1, o2);
}


static PyObject *
QuadIObject_inplace_subtract(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2(OP_sub, o1, o2);
}


static PyObject *
QuadIObject_inplace_mult(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2(OP_mult, o1, o2);
}

static PyObject *
QuadIObject_inplace_remainder(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2(OP_remainder, o1, o2);
}

static PyObject *
QuadIObject_inplace_pow(PyObject * o1, PyObject * o2, PyObject * o3 ){
    QuadIObject q1, q2, q3;

    if(!PyObject_to_QuadIObject(o1, &q1,true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadIObject(o2, &q2,true)){
        Py_RETURN_NOTIMPLEMENTED;
    }


    q1.value = powq(q1.value, q2.value);

    if(o3 != Py_None) {
        if(!PyObject_to_QuadIObject(o2, &q3,true))
            Py_RETURN_NOTIMPLEMENTED;

        q1.value = fmodq(q1.value, q3.value);
    }

    return QuadIObject_to_PyObject(q1);

}

static PyObject *
QuadIObject_floor_divide(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2(OP_floor_divide, o1, o2);
}

static PyObject *
QuadIObject_true_divide(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2(OP_true_divide, o1, o2);
}

static PyObject *
QuadIObject_inplace_floor_divide(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2(OP_floor_divide, o1, o2);
}

static PyObject *
QuadIObject_inplace_true_divide(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2(OP_true_divide, o1, o2);
}



static PyObject *
QuadIType_RichCompare(PyObject * o1, PyObject * o2, int opid){
    QuadIObject q1, q2;
    PyObject * res=NULL;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadIObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    switch (opid){
        case Py_EQ:
            res = (q1.value == q2.value) ? Py_True : Py_False;
            break;
        case Py_NE:
            res = (q1.value != q2.value) ? Py_True : Py_False;
            break;
        case Py_LE:
            res = (q1.value <= q2.value) ? Py_True : Py_False;
            break;
        case Py_LT:
            res = (q1.value < q2.value) ? Py_True : Py_False;
            break;
        case Py_GT:
            res = (q1.value > q2.value) ? Py_True : Py_False;
            break;
        case Py_GE:
            res = (q1.value >= q2.value) ? Py_True : Py_False;
            break;
        default:
            PyErr_SetString(PyExc_AttributeError, "Unknown comparison function.");
            return NULL;
    }

    Py_INCREF(res);
    return res;
}


static PyObject * QuadIObject_to_bytes(QuadIObject * self, PyObject * args){
    return PyBytes_FromStringAndSize(self->bytes, sizeof(__int128));
}


static PyObject * QuadIObject_from_bytes(PyTypeObject *type, PyObject * arg){
    // Gets the type object not an instance in type
    // As its METH_O we dont need to unpack arg
    QuadIObject  res;

    alloc_QuadIType(&res);

    if(PyBytes_Size(arg) == sizeof(res.bytes)){
        memcpy(res.bytes, PyBytes_AsString(arg), PyBytes_Size(arg));
    } else{
        PyErr_SetString(PyExc_ValueError, "Byte array wrong size for a integer quad");
        return NULL;
    }

    return QuadIObject_to_PyObject(res);
}

static PyObject * QuadIObject_from_param(PyTypeObject *type, PyObject * arg){
    // Gets the type object not an instance in type
    // As its METH_O we dont need to unpack arg
    QuadIObject  res;

    if(!PyObject_to_QuadIObject(arg, &res, true)){
        return NULL;
    }

    return QuadIObject_to_bytes(&res, NULL);
}



static PyObject* _as_parameter_(PyObject * self, void * y){
    QuadIObject val;

    if(!PyObject_to_QuadIObject(self, &val, true)){
        return NULL;
    }
    return QuadIObject_to_bytes(&val, NULL);    
}

//Pickling
static PyObject *
QuadIObject___getstate__(QuadIObject *self, PyObject *Py_UNUSED(ignored)) {
    PyObject *ret = Py_BuildValue("{sisO}",
                                  PICKLE_VERSION_KEY, PICKLE_VERSION,
                                  "bytes", QuadIObject_to_bytes(self, NULL));
    return ret;
}

	

/* Un-pickle the object */
static PyObject *
QuadIObject___setstate__(QuadIObject *self, PyObject *state) {

    /* Error check. */
    if (!PyDict_CheckExact(state)) {
        PyErr_SetString(PyExc_ValueError, "Pickled object is not a dict.");
        return NULL;
    }
    /* Version check. */
    /* Borrowed reference but no need to increment as we create a C long
     * from it. */
    PyObject *temp = PyDict_GetItemString(state, PICKLE_VERSION_KEY);
    if (temp == NULL) {
        /* PyDict_GetItemString does not set any error state so we have to. */
        PyErr_Format(PyExc_KeyError, "No \"%s\" in pickled dict.",
                     PICKLE_VERSION_KEY);
        return NULL;
    }
    int pickle_version = (int) PyLong_AsLong(temp);
    if (pickle_version != PICKLE_VERSION) {
        PyErr_Format(PyExc_ValueError,
                     "Pickle version mismatch. Got version %d but expected version %d.",
                     pickle_version, PICKLE_VERSION);
        return NULL;
    }

    temp = PyDict_GetItemString(state, "bytes");

    if (temp == NULL) {
        PyErr_Format(PyExc_KeyError, "No bytes in pickled dict.");
        return NULL;
    }

    if(PyBytes_Size(temp) == sizeof(self->bytes)){
        memcpy(self->bytes, PyBytes_AsString(temp), PyBytes_Size(temp));
    } else{
        PyErr_SetString(PyExc_ValueError, "Byte array wrong size for a integer quad");
        return NULL;
    }

    Py_RETURN_NONE;
}


Py_hash_t QuadIObject_hash(QuadIObject *self){
    // Note this wont have the nice property that Python has with numeric values being equal 
    // hash to the same value i.e hash(1) == hash(1.0)
    return PyObject_Hash(QuadIObject_to_bytes(self, NULL));
}


static PyObject * QuadIObject_to_hex(QuadIObject * self, PyObject * args){
    char buf[QUAD_INT_STR_BUF];

    if(int128_to_str(self->value,buf,QUAD_INT_STR_BUF,16)){
        return PyUnicode_FromFormat("0x%s",buf);
    } else {
        PyErr_SetString(PyExc_ValueError, "Can not convert value to hex");
        return NULL;
    }
}


static PyObject * QuadIObject_from_hex(PyTypeObject *type, PyObject * arg){
    // Gets the type object not an instance in type
    // As its METH_O we dont need to unpack arg
    // QuadIObject res;

    // alloc_QuadIType(&res);

    // if(PyUnicode_Check(arg)){
    //     // Is a string
    //     const char *buf = PyUnicode_AsUTF8AndSize(arg, NULL);
    //     if (buf==NULL){
    //         PyErr_Print();
    //         return NULL;
    //     }

    //     char *sp=NULL;
    //     res.value = strtoflt128(buf, NULL);
    //     if(sp!=NULL){
    //         if(strcmp(sp,"")!=0){
    //             PyErr_SetString(PyExc_ValueError, "Can not convert value from hex");
    //             return NULL;
    //         }
    //     }
    //     return QuadIObject_to_PyObject(res);
    // } else {
    //     PyErr_SetString(PyExc_ValueError, "Can not convert value from hex");
    //     return NULL;
    // }
    Py_RETURN_NOTIMPLEMENTED;
}


// Header data

static PyNumberMethods QuadI_math_methods = {
    (binaryfunc) QuadIObject_add,
    (binaryfunc) QuadIObject_subtract,
    (binaryfunc) QuadIObject_mult,
    (binaryfunc) QuadIObject_remainder,
    (binaryfunc) QuadIObject_divmod,
    (ternaryfunc) QuadIObject_pow,
    (unaryfunc) QuadIObject_neg,
    (unaryfunc) QuadIObject_pos,
    (unaryfunc) QuadIObject_abs,
    (inquiry) QuadIObject_bool,
    (unaryfunc) QuadIObject_invert,//  unaryfunc nb_invert;
    (binaryfunc) QuadIObject_lshift,//  binaryfunc nb_lshift;
    (binaryfunc) QuadIObject_rshift,//  binaryfunc nb_rshift;
    (binaryfunc) QuadIObject_and,//  binaryfunc nb_and;
    (binaryfunc) QuadIObject_xor,//  binaryfunc nb_xor;
    (binaryfunc) QuadIObject_or,//  binaryfunc nb_or;
    (unaryfunc) QuadIObject_int,
    0,//  void *nb_reserved;
    (unaryfunc) QuadIObject_float,

    (binaryfunc) QuadIObject_inplace_add,
    (binaryfunc) QuadIObject_inplace_subtract,
    (binaryfunc) QuadIObject_inplace_mult,
    (binaryfunc) QuadIObject_inplace_remainder,
    (ternaryfunc) QuadIObject_inplace_pow,
    (binaryfunc) QuadIObject_inplace_lshift,//  binaryfunc nb_inplace_lshift;
    (binaryfunc) QuadIObject_inplace_rshift,//  binaryfunc nb_inplace_rshift;
    (binaryfunc) QuadIObject_inplace_and,//  binaryfunc nb_inplace_and;
    (binaryfunc) QuadIObject_inplace_xor,//  binaryfunc nb_inplace_xor;
    (binaryfunc) QuadIObject_inplace_or,//  binaryfunc nb_inplace_or;

    (binaryfunc) QuadIObject_floor_divide,//  binaryfunc nb_floor_divide;
    (binaryfunc) QuadIObject_true_divide,//  binaryfunc nb_true_divide;
    (binaryfunc) QuadIObject_inplace_floor_divide,//  binaryfunc nb_inplace_floor_divide;
    (binaryfunc) QuadIObject_inplace_true_divide,//  binaryfunc nb_inplace_true_divide;

    0,//(unaryfunc) QuadIObject_int,//  unaryfunc nb_index;

    0,//  binaryfunc nb_matrix_multiply;
    0,//  binaryfunc nb_inplace_matrix_multiply;
};

// attributes
static PyMemberDef QuadI_members[] = {
    {NULL}  /* Sentinel */
};


//Methods 
static PyMethodDef QuadI_methods[] = {
    {"to_bytes", (PyCFunction) QuadIObject_to_bytes, METH_NOARGS, "to_bytes"},
    {"from_bytes", (PyCFunction) QuadIObject_from_bytes, METH_CLASS|METH_O, "from_bytes"},
    {"from_param", (PyCFunction) QuadIObject_from_param, METH_CLASS|METH_O, "from_param"},
    {"__getstate__", (PyCFunction) QuadIObject___getstate__, METH_NOARGS, "Pickle a quad object object" },
    {"__setstate__", (PyCFunction) QuadIObject___setstate__, METH_O,"Un-pickle a quad object object"},
    {"hex", (PyCFunction) QuadIObject_to_hex, METH_NOARGS, "to_hex"},
    {"fromhex", (PyCFunction) QuadIObject_from_hex, METH_CLASS|METH_O, "from_hex"},    
    
    {NULL}  /* Sentinel */
};

// Properties 
static PyGetSetDef QuadI_cgetset[] = {
    {"_as_parameter_", _as_parameter_, NULL, "ctypes _as_parameter_" },
    {NULL}  /* Sentinel */
};

int
Quad_qinit(QuadIObject *self, PyObject *args, PyObject *kwds)
{
    PyObject * obj;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
        PyErr_Print();
        return -1;
    }
        
    if(!PyObject_to_QuadIObject(obj, self, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to integer quad precision.");
        return -1;
    }

    return 0;
}


static PyTypeObject QuadIType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pyquadp._qint",
    .tp_doc = PyDoc_STR("A single integer quad precision variable"),
    .tp_basicsize = sizeof(QuadIObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_repr = (reprfunc) QuadIObject_repr,
    .tp_str = (reprfunc) QuadIObject_str,
    .tp_members = QuadI_members,
    .tp_methods = QuadI_methods,
    .tp_init = (initproc) Quad_qinit,
    .tp_as_number = &QuadI_math_methods,
    .tp_getset = QuadI_cgetset,
    .tp_richcompare = (richcmpfunc) QuadIType_RichCompare,
    .tp_hash = (hashfunc) QuadIObject_hash,
};

static PyModuleDef QuadIModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquadp.qint",
    .m_doc = "Quad precision module for scalar integer quad's.",
    .m_size = -1,
};

PyObject* 
QuadIObject_to_PyObject(QuadIObject out) {
	QuadIObject* ret = (QuadIObject*) PyType_GenericAlloc(&QuadIType, 0);
    
	if (ret != NULL) {
		ret->value = out.value;
	}

	return (PyObject*) ret;
}


static __int128 QuadIObject_int128(QuadIObject * out) {
    return out->value;
}


bool
PyObject_to_QuadIObject(PyObject * in, QuadIObject * out, const bool alloc)
{
    if(alloc)
        alloc_QuadIType(out);

    if(QuadIObject_Check(in)){
        // Is a quad
        out->value = QuadIObject_int128((QuadIObject *) in);
        return true;
    }

    if(PyUnicode_Check(in)){
        // Is a string
        Py_ssize_t size;
        const char *buf = PyUnicode_AsUTF8AndSize(in,&size);
        if (buf==NULL){
            PyErr_Print();
            return false;
        }

        if(!str_to_int128(buf, size, &out->value)){
            PyErr_SetString(PyExc_ValueError, "Overflow error");
            return NULL;
        }

        return true;
    }

    if(PyNumber_Check(in)) {
        // Is a number
        if(PyLong_Check(in)){
            // int
            // Convert to string as we may be to big for PyLong
            PyObject * str = PyUnicode_FromFormat("%S",in);
            Py_ssize_t len;
            const char *buf = PyUnicode_AsUTF8AndSize(str, &len);
            if (buf==NULL){
                PyErr_Print();
                return false;
            }

            if(!str_to_int128(buf,len,&out->value)){
                PyErr_SetString(PyExc_TypeError, "Could not convert int to integer quad precision.");
                return false;
            }

            return true;
        }
    }

    return false;

}

static bool QuadIObject_Check(PyObject * obj){
    return PyObject_TypeCheck(obj, &QuadIType);
}


static void alloc_QuadIType(QuadIObject * result){
	result = (QuadIObject*) PyType_GenericAlloc(&QuadIType, 0);
}

PyMODINIT_FUNC
PyInit_qint(void)
{
    PyObject *m;
    static void *PyQInt_API[PyQInt_API_pointers];
    PyObject *c_api_object;


    if (PyType_Ready(&QuadIType) < 0)
        return NULL;

    m = PyModule_Create(&QuadIModule);
    if (m == NULL)
        return NULL;

    /* Initialize the C API pointer array */
    PyQInt_API[PyQInt_q2py_NUM] = (void *)QuadIObject_to_PyObject;
    PyQInt_API[PyQInt_py2q_NUM] = (void *)PyObject_to_QuadIObject;
    PyQInt_API[PyQInt_alloc_NUM] = (void *)alloc_QuadIType;
    PyQInt_API[PyQInt_int128_NUM] = (void *)QuadIObject_int128;
    PyQInt_API[PyQInt_check_NUM] = (void *)QuadIObject_Check;


    Py_INCREF(&QuadIType);
    if (PyModule_AddObject(m, "_qint", (PyObject *) &QuadIType) < 0) {
        Py_DECREF(&QuadIType);
        Py_DECREF(m);
        return NULL;
    }


    /* Create a Capsule containing the API pointer array's address */
    c_api_object = PyCapsule_New((void *)PyQInt_API, "qint._C_API", NULL);

    if (PyModule_AddObject(m, "_C_API", c_api_object) < 0) {
        Py_XDECREF(c_api_object);
        Py_DECREF(m);
        return NULL;
    }


    return m;
}

// TODO: Handle hex values?
bool str_to_int128(const char *str, Py_ssize_t length, __int128 *result){
    __int128 count=1;
    char sym;
    int sign=0;
    bool sign_set=false;

    __int128 c,sum,r;

    *result=0;

    if(length == 0)
        return false;

    sign = 1;
    for(int i=length;i>=0;i--){
        sym = str[i];

        if(isspace(sym))
            continue;

        if(!sign_set){
            if(sym=='-' || sym =='+'){
                sign_set = true;
                if(sym=='-')
                    sign=-1;
            }
        }

        if(isalpha(sym)){
            return false;
        }

        if(isdigit(sym)){
            // Compute result+= (sym - '0')*count;count*=10

            // we are working from back of the string forwards
            // so each time count increases by a factor 10 we are 
            // shifting to the next digit in base 10

            if(__builtin_mul_overflow(sym - '0', count, &sum))
                return false;

            if(__builtin_add_overflow(sum, *result, &r))
                return false;
            else
                *result = r;

            // Peak ahead to see if there is more still to process
            // otherwise we may overflow and error even though we 
            // dont need count again if we are close to max __int128
            if(i>0){
                if(isdigit(str[i-1])){
                    if(__builtin_mul_overflow(count, 10, &c))
                        return false;
                    else
                        count = c;
                }
            }
        }

    }
    *result *= sign;
    return true;
}

bool int128_to_str(__int128 num, char* str, int len, int base)
{
	__int128 sum = num;
	int i = 0,k=0;
	int digit;
    bool neg=false;
    char temp;
	if (len == 0)
		return false;

    if(sum<0){
        sum=-1*sum;
        neg=true;
    }
	do
	{
		digit = (sum % base);
		if (digit < 10)
			str[i++] = '0' + digit;
		else
			str[i++] = 'A' + digit - 10;

		sum /= base;
	}while (sum && (i < (len - 1)));
	if (i == (len - 1) && sum)
		return false;
    if(neg){
        str[i] = '-';
        i++;
    }
    str[i] = '\0';

    if (strlen(str) == 0)
        return false;

    // String needs to be reversed
    int slen = i;

    for(i = 0, k=slen-1 ; i < (slen/2); i++,k--)
    {
        temp = str[k];
        str[k] = str[i];
        str[i] = temp;

    }

	return true;
}

