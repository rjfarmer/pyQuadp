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

#define QCMPLX_MODULE
#include "qcmplx.h"
#include "qfloat.h"


static PyObject *
QuadCObject_repr(QuadCObject * obj)
{
    char buf1[QUAD_BUF],buf2[QUAD_BUF];

    int n1 = quadmath_snprintf (buf1, sizeof buf1, "%.36Qe", crealq(obj->value));
    int n2 = quadmath_snprintf (buf2, sizeof buf2, "%.36Qe", cimagq(obj->value));
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

    int n1 = quadmath_snprintf (buf1, sizeof buf1, "%.36Qe", crealq(obj->value));
    int n2 = quadmath_snprintf (buf2, sizeof buf2, "%.36Qe", cimagq(obj->value));
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
            result.value = -1 * q1.value;
            break;
        case OP_positive:
            result.value = 1*q1.value;
            break;
        case OP_absolute:
            result.value = fabsq(q1.value);
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadCObject_to_PyObject(result);
}


static PyObject *
QuadCObject_binary_op2(const int op, PyObject * o1, PyObject * o2 ){

    QuadCObject q1, q2, result;

    if(!PyObject_to_QuadCObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadCObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadCType(&result);

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
        case OP_true_divide:
            result.value = q1.value/q2.value;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadCObject_to_PyObject(result);
}


static PyObject *
QuadCObject_binary_self_op2(const int op, PyObject * o1, PyObject * o2 ){

    QuadCObject q1, q2;

    if(!PyObject_to_QuadCObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadCObject(o2, &q2, true)){
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
        case OP_true_divide:
            q1.value = q1.value/q2.value;
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
    QuadCObject q1, q2, result;

    if(!PyObject_to_QuadCObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadCObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadCType(&result);

    result.value = cpowq(QuadCObject_complex128(&q1),QuadCObject_complex128(&q2));

    if(o3 != Py_None) {
        PyErr_SetString(PyExc_ValueError, "complex modulo");
        return NULL;
    }

    return QuadCObject_to_PyObject(result);
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

    if(crealq(q1.value)==0 && cimagq(q1.value))
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
    QuadCObject q1, q2, result;

    if(!PyObject_to_QuadCObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadCObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadCType(&result);

    q1.value =  cpowq(QuadCObject_complex128(&q1),QuadCObject_complex128(&q2));

    if(o3 != Py_None) {
        PyErr_SetString(PyExc_ValueError, "complex modulo");
        return NULL;
    }

    return QuadCObject_to_PyObject(q1);

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

    q1.value = crealq(val.value);

    return QuadObject_to_PyObject(q1);

}

PyObject* get_imag(PyObject * x, void * y){
    QuadCObject val;
    QuadObject q1;

    if(!PyObject_to_QuadCObject(x, &val, true)){
        return NULL;
    }

    alloc_QuadType(&q1);

    q1.value = cimagq(val.value);

    return QuadObject_to_PyObject(q1);
    
}


PyObject* QuadCObject_to_pycmplx(PyObject * self, PyObject * args){
    QuadCObject val;
    double r,i;

    if(!PyObject_to_QuadCObject(self, &val, true)){
        return NULL;
    }

    QuadCObject_to_doubles(val, &r, &i);

    return  PyComplex_FromDoubles(r,i);    
}


PyObject* QuadCObject_to_abs(PyObject * self, PyObject * args){
    QuadCObject val;
    QuadObject res;

    if(!PyObject_to_QuadCObject(self, &val, true)){
        return NULL;
    }

    alloc_QuadType(&res);
    res.value = cabsq(val.value);

    return  QuadObject_to_PyObject(res);
}

PyObject* QuadCObject_conjugate(PyObject * self, PyObject * args){
    QuadCObject val;

    if(!PyObject_to_QuadCObject(self, &val, true)){
        return NULL;
    }

    val.value = crealq(val.value) - cimagq(val.value)*I;

    return  QuadCObject_to_PyObject(val);
}

static PyObject * QuadCObject_to_bytes(QuadCObject * self, PyObject * args){
    return PyBytes_FromStringAndSize(self->bytes, sizeof(__complex128));
}


static PyObject * QuadCObject_from_bytes(PyTypeObject *type, PyObject * arg){
    // Gets the type object not an instance in type
    // As its METH_O we dont need to unpack arg
    QuadCObject  res;

    alloc_QuadCType(&res);

    if(PyBytes_Size(arg) == sizeof(res.bytes)){
        memcpy(res.bytes, PyBytes_AsString(arg), PyBytes_Size(arg));
    } else{
        PyErr_SetString(PyExc_ValueError, "Byte array wrong size for a complex quad");
        return NULL;
    }

    return QuadCObject_to_PyObject(res);
}


PyObject* _as_parameter_(PyObject * self, void * y){
    QuadCObject val;

    if(!PyObject_to_QuadCObject(self, &val, true)){
        return NULL;
    }
    return QuadCObject_to_bytes(&val, NULL);    
}


static PyObject * QuadCObject_from_param(PyTypeObject *type, PyObject * arg){
    // Gets the type object not an instance in type
    // As its METH_O we dont need to unpack arg
    QuadCObject  res;

    if(!PyObject_to_QuadCObject(arg, &res, true)){
        return NULL;
    }

    return QuadCObject_to_bytes(&res, NULL);
}


//Pickling
static PyObject *
QuadCObject___getstate__(QuadCObject *self, PyObject *Py_UNUSED(ignored)) {
    PyObject *ret = Py_BuildValue("{sisO}",
                                  PICKLE_VERSION_KEY, PICKLE_VERSION,
                                  "bytes", QuadCObject_to_bytes(self, NULL));
    return ret;
}

	

/* Un-pickle the object */
static PyObject *
QuadCObject___setstate__(QuadCObject *self, PyObject *state) {

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
        PyErr_SetString(PyExc_ValueError, "Byte array wrong size for a complex quad");
        return NULL;
    }

    Py_RETURN_NONE;
}

Py_hash_t QuadCObject_hash(QuadCObject *self){
    // Note this wont have the nice property that Python has with numeric values being equal 
    // hash to the same value i.e hash(1) == hash(1.0)
    return PyObject_Hash(QuadCObject_to_bytes(self, NULL));
}



static PyMemberDef Quad_cmembers[] = {
    {NULL}  /* Sentinel */
};


static PyMethodDef Quad_cmethods[] = {
    {"__abs__", (PyCFunction) QuadCObject_to_abs, METH_VARARGS, "Absolute value"},
    {"__complex__",(PyCFunction) QuadCObject_to_pycmplx, METH_VARARGS, "Complex number"},
    {"conjugate",(PyCFunction) QuadCObject_conjugate, METH_VARARGS, "conjugate"},
    {"to_bytes", (PyCFunction) QuadCObject_to_bytes, METH_NOARGS, "to_bytes"},
    {"from_bytes", (PyCFunction) QuadCObject_from_bytes, METH_CLASS|METH_O, "from_bytes"},
    {"from_param", (PyCFunction) QuadCObject_from_param, METH_CLASS|METH_O, "from_param"},
    {"__getstate__", (PyCFunction) QuadCObject___getstate__, METH_NOARGS, "Pickle a complex quad object object" },
    {"__setstate__", (PyCFunction) QuadCObject___setstate__, METH_O,"Un-pickle a complex quad object object"},
    {NULL}  /* Sentinel */
};

static PyGetSetDef Quad_cgetset[] = {
    {"real", get_real, NULL, "Real component" },
    {"imag", get_imag, NULL, "Imaginary component" },
    {"_as_parameter_", _as_parameter_, NULL, "ctypes _as_parameter_" },
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


static PyObject *
QuadCType_RichCompare(PyObject * o1, PyObject * o2, int opid){
    QuadCObject q1, q2;
    PyObject * res=NULL;
    int real,imag;

    if(!PyObject_to_QuadCObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadCObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    real = crealq(q1.value) == crealq(q2.value);
    imag = cimagq(q1.value) == cimagq(q2.value);


    switch (opid){
        case Py_EQ:
            res = real && imag ? Py_True : Py_False;
            break;
        case Py_NE:
            res = !real || !imag ? Py_True : Py_False;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    Py_INCREF(res);
    return res;
}



static PyTypeObject QuadCType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pyquadp._qcmplx",
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
    .tp_richcompare = (richcmpfunc) QuadCType_RichCompare,
    .tp_hash = (hashfunc) QuadCObject_hash,
};

static PyModuleDef QuadCModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pyquadp.qcmplx",
    .m_doc = "Quad precision module for complex quad's.",
    .m_size = -1,
};

PyObject* 
QuadCObject_to_PyObject(QuadCObject out) {
	QuadCObject* ret = (QuadCObject*) PyType_GenericAlloc(&QuadCType, 0);

	if (ret != NULL) {
		ret->value = crealq(out.value) + cimagq(out.value)*I;
	}

	return (PyObject*) ret;
}

static __complex128 QuadCObject_complex128(QuadCObject * out) {
    return crealq(out->value) + cimagq(out->value) * I;
}

void QuadCObject_to_doubles(QuadCObject c, double *real, double *imag){

    __complex128 q = QuadCObject_complex128(&c);

    *real = __float128_to_double(crealq(q));
    *imag = __float128_to_double(cimagq(q));

}


bool
PyObject_to_QuadCObject(PyObject * in, QuadCObject * out, const bool alloc)
{
    if(alloc)
        alloc_QuadCType(out);

    //printf("%d\n",PyObject_TypeCheck(in, &QuadCType));

    if(PyObject_TypeCheck(in, &QuadCType)){
        // Is a complex quad
        out->value = QuadCObject_complex128((QuadCObject *)in);
        return true;
    }

    if(QuadObject_Check(in)){
        // Is a quad
        out->value = QuadObject_float128((QuadObject *) in) + 0*I;
        return true;
    }

    if(PyComplex_Check(in)){
        out->value = (__float128) PyComplex_RealAsDouble(in) + (__float128) PyComplex_ImagAsDouble(in) * I;
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

    if(QuadObject_Check(in1)){
        // Is a quad
        out->value = QuadObject_float128((QuadObject *) in1) + 0*I;
        set1 = true;
    }

    if(!set1){
        if(PyObject_to_QuadObject(in1, &q1,true)){
            out->value = QuadObject_float128(&q1)+ 0*I;
            set1=true;
        }
    }
    
    if(QuadObject_Check(in2)){
        // Is a quad
        out->value += QuadObject_float128((QuadObject *) in2)*I;
        set2 = true;
    }

    
    if(!set2){
        if(PyObject_to_QuadObject(in2, &q2,true)){
            out->value +=  QuadObject_float128(&q2)*I;
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
    quadmath_snprintf(output1, sizeof output1, "%.36Qg", 35, crealq(out->value));
    quadmath_snprintf(output2, sizeof output2, "%.36Qg", 35, cimagq(out->value));
    printf("%s %s\n", output1,output2);
}
#pragma GCC diagnostic pop

static void alloc_QuadCType(QuadCObject * result){
	result = (QuadCObject*) PyType_GenericAlloc(&QuadCType, 0);
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
