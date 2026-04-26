// SPDX-License-Identifier: GPL-2.0+
#include "pyquadp.h"

#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <complex.h> 
#include <float.h>

#define QCMPLX_MODULE
#include "qcmplx.h"
#include "qfloat.h"

static PyTypeObject *QuadCType = NULL;


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
    PyObject *bytes_obj = QuadCObject_to_bytes(self, NULL);
    PyObject *ret;

    if (bytes_obj == NULL) {
        return NULL;
    }

    ret = Py_BuildValue("{sisN}",
                        PICKLE_VERSION_KEY, PICKLE_VERSION,
                        "bytes", bytes_obj);
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
    PyObject *key = PyUnicode_FromString(PICKLE_VERSION_KEY);
    PyObject *temp;

    if (key == NULL) {
        return NULL;
    }

    temp = PyDict_GetItemWithError(state, key);
    Py_DECREF(key);
    if (temp == NULL) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_KeyError, "No pickle version in pickled dict.");
        }
        return NULL;
    }
    int pickle_version = (int) PyLong_AsLong(temp);
    if (pickle_version != PICKLE_VERSION) {
        PyErr_Format(PyExc_ValueError,
                     "Pickle version mismatch. Got version %d but expected version %d.",
                     pickle_version, PICKLE_VERSION);
        return NULL;
    }

    key = PyUnicode_FromString("bytes");
    if (key == NULL) {
        return NULL;
    }

    temp = PyDict_GetItemWithError(state, key);
    Py_DECREF(key);

    if (temp == NULL) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_KeyError, "No bytes in pickled dict.");
        }
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
    PyObject *bytes_obj = QuadCObject_to_bytes(self, NULL);
    Py_hash_t h;

    if (bytes_obj == NULL) {
        return -1;
    }

    h = PyObject_Hash(bytes_obj);
    Py_DECREF(bytes_obj);
    return h;
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
    (void)kwds;

    if(PyArg_ParseTuple(args, "OO:", &obj1, &obj2)){
        if(!PyObject_to_QuadCObject2(obj1, obj2, self, true)){
            PyErr_SetString(PyExc_TypeError, "Can not convert both values to quad complex precision.");
            return -1;
        } else {
            return 0;
        }
    } else {
        // Ignore only parse mismatch for the 2-arg form; propagate other errors.
        if (PyErr_ExceptionMatches(PyExc_TypeError)) {
            PyErr_Clear();
        } else {
            return -1;
        }
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



static PyType_Slot QuadCType_slots[] = {
    {Py_tp_doc, (void *)PyDoc_STR("A single quad precision complex variable")},
    {Py_tp_new, (void *)PyType_GenericNew},
    {Py_tp_repr, (void *)QuadCObject_repr},
    {Py_tp_str, (void *)QuadCObject_str},
    {Py_tp_members, (void *)Quad_cmembers},
    {Py_tp_methods, (void *)Quad_cmethods},
    {Py_tp_init, (void *)Quad_cinit},
    {Py_tp_getset, (void *)Quad_cgetset},
    {Py_tp_richcompare, (void *)QuadCType_RichCompare},
    {Py_tp_hash, (void *)QuadCObject_hash},

    {Py_nb_add, (void *)QuadCObject_add},
    {Py_nb_subtract, (void *)QuadCObject_subtract},
    {Py_nb_multiply, (void *)QuadCObject_mult},
    {Py_nb_power, (void *)QuadCObject_pow},
    {Py_nb_negative, (void *)QuadCObject_neg},
    {Py_nb_positive, (void *)QuadCObject_pos},
    {Py_nb_absolute, (void *)QuadCObject_abs},
    {Py_nb_bool, (void *)QuadCObject_bool},
    {Py_nb_inplace_add, (void *)QuadCObject_inplace_add},
    {Py_nb_inplace_subtract, (void *)QuadCObject_inplace_subtract},
    {Py_nb_inplace_multiply, (void *)QuadCObject_inplace_mult},
    {Py_nb_inplace_power, (void *)QuadCObject_inplace_pow},
    {Py_nb_true_divide, (void *)QuadCObject_true_divide},
    {Py_nb_inplace_true_divide, (void *)QuadCObject_inplace_true_divide},
    {0, NULL}
};

static PyType_Spec QuadCType_spec = {
    .name = "pyquadp.qmcmplx.qcmplx",
    .basicsize = sizeof(QuadCObject),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = QuadCType_slots,
};

static PyModuleDef QuadCModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "qmcmplx",
    .m_doc = PyDoc_STR("Quad precision module for complex quad's."),
    .m_size = -1,
};

PyObject* 
QuadCObject_to_PyObject(QuadCObject out) {
	QuadCObject* ret;

    if (QuadCType == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "qcmplx type not initialized");
        return NULL;
    }

	ret = (QuadCObject*) PyType_GenericAlloc(QuadCType, 0);

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

    if(QuadCType != NULL && PyObject_TypeCheck(in, QuadCType)){
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
        __real__ out->value = (__float128) PyComplex_RealAsDouble(in);

        if(PyErr_Occurred())
            return NULL;

        __imag__ out->value = (__float128) PyComplex_ImagAsDouble(in);
        
        if(PyErr_Occurred())
            return NULL;

        return true;
    }

    if(PyUnicode_Check(in)){
        // Is a string
        const char *buf = PyUnicode_AsUTF8AndSize(in, NULL);
        if (buf==NULL){
            return false;
        }
        char *sp = NULL;
        char *sp2 = NULL;
        char *buf2 = strdup(buf);
        char *p = NULL;
        char *split_sign = NULL;
        char *real_str = NULL;
        char *imag_str = NULL;
        size_t len = 0;

        if (buf2 == NULL) {
            PyErr_NoMemory();
            return false;
        }

        out->value = 0 + 0*I;

        // Trim trailing spaces to reliably detect a terminal j/J.
        len = strlen(buf2);
        while (len > 0 && (buf2[len - 1] == ' ' || buf2[len - 1] == '\t')) {
            buf2[len - 1] = '\0';
            len--;
        }

        if (len > 0 && (buf2[len - 1] == 'j' || buf2[len - 1] == 'J')) {
            // Complex string with explicit imaginary component.
            buf2[len - 1] = '\0';

            // Find split sign between real and imag, ignoring exponent signs.
            p = buf2 + 1;
            while (*p) {
                if ((*p == '+' || *p == '-') && *(p - 1) != 'e' && *(p - 1) != 'E') {
                    split_sign = p;
                }
                p++;
            }

            if (split_sign) {
                size_t real_len = (size_t)(split_sign - buf2);
                real_str = (char *)malloc(real_len + 1);
                if (real_str == NULL) {
                    free(buf2);
                    PyErr_NoMemory();
                    return false;
                }
                memcpy(real_str, buf2, real_len);
                real_str[real_len] = '\0';

                imag_str = strdup(split_sign);
                if (imag_str == NULL) {
                    free(real_str);
                    free(buf2);
                    PyErr_NoMemory();
                    return false;
                }

                __real__ out->value = strtoflt128(real_str, &sp);
                __imag__ out->value = strtoflt128(imag_str, &sp2);

                free(real_str);
                free(imag_str);
            } else {
                // Imaginary-only string, e.g. "-9.87j".
                __real__ out->value = 0;
                __imag__ out->value = strtoflt128(buf2, &sp2);
            }
        } else {
            // Real-only string.
            __real__ out->value = strtoflt128(buf2, &sp);
            __imag__ out->value = 0;
        }

        free(buf2);
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
        __real__ out->value = QuadObject_float128((QuadObject *) in1);
        set1 = true;
    }

    if(!set1){
        if(PyObject_to_QuadObject(in1, &q1,true)){
            __real__ out->value = QuadObject_float128(&q1);
            set1=true;
        }
    }
    
    if(QuadObject_Check(in2)){
        // Is a quad
        __imag__ out->value = QuadObject_float128((QuadObject *) in2);
        set2 = true;
    }

    
    if(!set2){
        if(PyObject_to_QuadObject(in2, &q2,true)){
            __imag__ out->value =  QuadObject_float128(&q2);
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
    if (QuadCType == NULL) {
        return;
    }
    result = (QuadCObject*) PyType_GenericAlloc(QuadCType, 0);
}

PyMODINIT_FUNC
PyInit_qmcmplx(void)
{
    PyObject *m;
    static void *PyQcmplx_API[PyQcmplx_API_pointers];
    PyObject *c_api_object;
    PyObject *quadc_type_obj;
    PyObject *module_name_obj;


    m = PyModule_Create(&QuadCModule);
    if (m == NULL)
        return NULL;

    quadc_type_obj = PyType_FromSpec(&QuadCType_spec);
    if (quadc_type_obj == NULL) {
        Py_DECREF(m);
        return NULL;
    }
    module_name_obj = PyUnicode_FromString("pyquadp.qmcmplx");
    if (module_name_obj == NULL) {
        Py_DECREF(quadc_type_obj);
        Py_DECREF(m);
        return NULL;
    }
    if (PyObject_SetAttrString(quadc_type_obj, "__module__", module_name_obj) < 0) {
        Py_DECREF(module_name_obj);
        Py_DECREF(quadc_type_obj);
        Py_DECREF(m);
        return NULL;
    }
    Py_DECREF(module_name_obj);

    QuadCType = (PyTypeObject *)quadc_type_obj;

    /* Initialize the C API pointer array */
    PyQcmplx_API[PyQcmplx_q2py_NUM] = (void *)QuadCObject_to_PyObject;
    PyQcmplx_API[PyQcmplx_py2q_NUM] = (void *)PyObject_to_QuadCObject;
    PyQcmplx_API[PyQcmplx_alloc_NUM] = (void *)alloc_QuadCType;
    PyQcmplx_API[PyQcmplx_type_NUM] = (void *)QuadCType;

    if (PyModule_AddObject(m, "qcmplx", quadc_type_obj) < 0) {
        Py_DECREF(quadc_type_obj);
        Py_DECREF(m);
        return NULL;
    }


    /* Create a Capsule containing the API pointer array's address */
    c_api_object = PyCapsule_New((void *)PyQcmplx_API, "pyquadp.qmcmplx._C_API", NULL);

    if (PyModule_AddObject(m, "_C_API", c_api_object) < 0) {
        Py_XDECREF(c_api_object);
        Py_DECREF(m);
        return NULL;
    }

    if (PyDict_SetItemString(PyImport_GetModuleDict(), "qmcmplx", m) < 0) {
        Py_DECREF(m);
        return NULL;
    }

    if (import_qmfloat() < 0)
        return NULL;


    return m;
}
