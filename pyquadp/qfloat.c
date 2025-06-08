// SPDX-License-Identifier: GPL-2.0+
#include "pyquadp.h"

#define QFLOAT_MODULE
#include "qfloat.h"



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

    if(!PyObject_to_QuadObject(o1, &q1, true)){
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

    if(!PyObject_to_QuadObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadObject(o2, &q2, true)){
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
        case OP_floor_divide:
            result.value = floorq(q1.value/q2.value);
            break;
        case OP_true_divide:
            result.value = q1.value/q2.value;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadObject_to_PyObject(result);
}


static PyObject *
QuadObject_binary_self_op2(const int op, PyObject * o1, PyObject * o2 ){

    QuadObject q1, q2;

    if(!PyObject_to_QuadObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadObject(o2, &q2, true)){
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
            q1.value = remainderq(q1.value, q2.value);
            break;
        case OP_floor_divide:
            q1.value = floorq(q1.value/q2.value);
            break;
        case OP_true_divide:
            q1.value = q1.value/q2.value;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadObject_to_PyObject(q1);
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

    if(!PyObject_to_QuadObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    alloc_QuadType(&result);

    result.value = powq(q1.value, q2.value);

    if(o3 != Py_None) {
        if(!PyObject_to_QuadObject(o2, &q3, true))
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

    if(!PyObject_to_QuadObject(o1, &q1, true)){
        return 0;
    }

    if(q1.value==0)
        return 0;

    return 1;

}

static PyObject *
QuadObject_int(PyObject * o1){
    QuadObject q1;
    PyObject * result;

    if(!PyObject_to_QuadObject(o1, &q1,true)){
        Py_RETURN_NOTIMPLEMENTED;
    }


   result = PyLong_FromLongLong(q1.value);
   if(PyErr_Occurred())
       return NULL;

   return result;
}

static PyObject *
QuadObject_float(PyObject * o1){
    QuadObject q1;
    PyObject * result;
    double d;

    if(!PyObject_to_QuadObject(o1, &q1,true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

   d = QuadObject_to_double(&q1);

   result = PyFloat_FromDouble(d);

   return result;
}


static PyObject *
QuadObject_inplace_add(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_self_op2(OP_add, o1, o2);
}


static PyObject *
QuadObject_inplace_subtract(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_self_op2(OP_sub, o1, o2);
}


static PyObject *
QuadObject_inplace_mult(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_self_op2(OP_mult, o1, o2);
}

static PyObject *
QuadObject_inplace_remainder(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_self_op2(OP_remainder, o1, o2);
}

static PyObject *
QuadObject_inplace_pow(PyObject * o1, PyObject * o2, PyObject * o3 ){
    QuadObject q1, q2, q3;

    if(!PyObject_to_QuadObject(o1, &q1,true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadObject(o2, &q2,true)){
        Py_RETURN_NOTIMPLEMENTED;
    }


    q1.value = powq(q1.value, q2.value);

    if(o3 != Py_None) {
        if(!PyObject_to_QuadObject(o2, &q3,true))
            Py_RETURN_NOTIMPLEMENTED;

        q1.value = fmodq(q1.value, q3.value);
    }

    return QuadObject_to_PyObject(q1);

}

static PyObject *
QuadObject_floor_divide(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_op2(OP_floor_divide, o1, o2);
}

static PyObject *
QuadObject_true_divide(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_op2(OP_true_divide, o1, o2);
}

static PyObject *
QuadObject_inplace_floor_divide(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_self_op2(OP_floor_divide, o1, o2);
}

static PyObject *
QuadObject_inplace_true_divide(PyObject * o1, PyObject * o2 ){
    return QuadObject_binary_self_op2(OP_true_divide, o1, o2);
}



static PyObject *
QuadType_RichCompare(PyObject * o1, PyObject * o2, int opid){
    QuadObject q1, q2;
    PyObject * res=NULL;

    if(!PyObject_to_QuadObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadObject(o2, &q2, true)){
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


static PyObject * QuadObject_to_bytes(QuadObject * self, PyObject * args){
    return PyBytes_FromStringAndSize(self->bytes, sizeof(__float128));
}


static PyObject * QuadObject_from_bytes(PyTypeObject *type, PyObject * arg){
    // Gets the type object not an instance in type
    // As its METH_O we dont need to unpack arg
    QuadObject  res;

    alloc_QuadType(&res);

    if(PyBytes_Size(arg) == sizeof(res.bytes)){
        memcpy(res.bytes, PyBytes_AsString(arg), PyBytes_Size(arg));
    } else{
        PyErr_SetString(PyExc_ValueError, "Byte array wrong size for a quad");
        return NULL;
    }

    return QuadObject_to_PyObject(res);
}

static PyObject * QuadObject_from_param(PyTypeObject *type, PyObject * arg){
    // Gets the type object not an instance in type
    // As its METH_O we dont need to unpack arg
    QuadObject  res;

    if(!PyObject_to_QuadObject(arg, &res, true)){
        return NULL;
    }

    return QuadObject_to_bytes(&res, NULL);
}



static PyObject* _as_parameter_(PyObject * self, void * y){
    QuadObject val;

    if(!PyObject_to_QuadObject(self, &val, true)){
        return NULL;
    }
    return QuadObject_to_bytes(&val, NULL);    
}

//Pickling
static PyObject *
QuadObject___getstate__(QuadObject *self, PyObject *Py_UNUSED(ignored)) {
    PyObject *ret = Py_BuildValue("{sisO}",
                                  PICKLE_VERSION_KEY, PICKLE_VERSION,
                                  "bytes", QuadObject_to_bytes(self, NULL));
    return ret;
}

	

/* Un-pickle the object */
static PyObject *
QuadObject___setstate__(QuadObject *self, PyObject *state) {

    /* Error check. */
    if (!PyDict_CheckExact(state)) {
        PyErr_SetString(PyExc_ValueError, "Pickled object is not a dict.");
        return NULL;
    }
    /* Version check. */
    /* Borrowed reference but no need to increment as we create a C long
     * from it. */
    PyObject *temp = PyDict_GetItemWithError(state, PyUnicode_FromFormat(PICKLE_VERSION_KEY));
    if (temp == NULL) {
        return NULL;
    }
    int pickle_version = (int) PyLong_AsLong(temp);
    if (pickle_version != PICKLE_VERSION) {
        PyErr_Format(PyExc_ValueError,
                     "Pickle version mismatch. Got version %d but expected version %d.",
                     pickle_version, PICKLE_VERSION);
        return NULL;
    }

    temp = PyDict_GetItemWithError(state, PyUnicode_FromFormat("bytes"));

    if (temp == NULL) {
        return NULL;
    }

    if(PyBytes_Size(temp) == sizeof(self->bytes)){
        memcpy(self->bytes, PyBytes_AsString(temp), PyBytes_Size(temp));
    } else{
        PyErr_SetString(PyExc_ValueError, "Byte array wrong size for a quad");
        return NULL;
    }

    Py_RETURN_NONE;
}


Py_hash_t QuadObject_hash(QuadObject *self){
    // Note this wont have the nice property that Python has with numeric values being equal 
    // hash to the same value i.e hash(1) == hash(1.0)
    return PyObject_Hash(QuadObject_to_bytes(self, NULL));
}


static PyObject * QuadObject_to_hex(QuadObject * self, PyObject * args){
    char buf[QUAD_BUF];

    int n = quadmath_snprintf (buf, sizeof buf, "%Qa", self->value);
    if ((size_t) n < sizeof buf){
        return PyUnicode_FromFormat("%s",buf);
    } else {
        PyErr_SetString(PyExc_ValueError, "Can not convert value to hex");
        return NULL;
    }
}


static PyObject * QuadObject_from_hex(PyTypeObject *type, PyObject * arg){
    // Gets the type object not an instance in type
    // As its METH_O we dont need to unpack arg
    QuadObject res;

    alloc_QuadType(&res);

    if(PyUnicode_Check(arg)){
        // Is a string
        const char *buf = PyUnicode_AsUTF8AndSize(arg, NULL);
        if (buf==NULL){
            PyErr_Print();
            return NULL;
        }

        char *sp=NULL;
        res.value = strtoflt128(buf, NULL);
        if(sp!=NULL){
            if(strcmp(sp,"")!=0){
                PyErr_SetString(PyExc_ValueError, "Can not convert value from hex");
                return NULL;
            }
        }
        return QuadObject_to_PyObject(res);
    } else {
        PyErr_SetString(PyExc_ValueError, "Can not convert value from hex");
        return NULL;
    }

}



// Header data

static PyNumberMethods Quad_math_methods = {
    .nb_add = (binaryfunc) QuadObject_add,
    .nb_subtract = (binaryfunc) QuadObject_subtract,
    .nb_multiply = (binaryfunc) QuadObject_mult,
    .nb_remainder = (binaryfunc) QuadObject_remainder,
    .nb_divmod = (binaryfunc) QuadObject_divmod,
    .nb_power = (ternaryfunc) QuadObject_pow,
    .nb_negative = (unaryfunc) QuadObject_neg,
    .nb_positive = (unaryfunc) QuadObject_pos,
    .nb_absolute = (unaryfunc) QuadObject_abs,
    .nb_bool = (inquiry) QuadObject_bool,
    .nb_int = (unaryfunc) QuadObject_int,
    .nb_float = (unaryfunc) QuadObject_float,
    .nb_inplace_add = (binaryfunc) QuadObject_inplace_add,
    .nb_inplace_subtract = (binaryfunc) QuadObject_inplace_subtract,
    .nb_inplace_multiply = (binaryfunc) QuadObject_inplace_mult,
    .nb_inplace_remainder = (binaryfunc) QuadObject_inplace_remainder,
    .nb_inplace_power = (ternaryfunc) QuadObject_inplace_pow,
    .nb_floor_divide = (binaryfunc) QuadObject_floor_divide,//  binaryfunc nb_floor_divide;
    .nb_true_divide = (binaryfunc) QuadObject_true_divide,//  binaryfunc nb_true_divide;
    .nb_inplace_floor_divide = (binaryfunc) QuadObject_inplace_floor_divide,//  binaryfunc nb_inplace_floor_divide;
    .nb_inplace_true_divide = (binaryfunc) QuadObject_inplace_true_divide,//  binaryfunc nb_inplace_true_divide;
};

// attributes
static PyMemberDef Quad_members[] = {
    {NULL}  /* Sentinel */
};


//Methods 
static PyMethodDef Quad_methods[] = {
    {"to_bytes", (PyCFunction) QuadObject_to_bytes, METH_NOARGS, "to_bytes"},
    {"from_bytes", (PyCFunction) QuadObject_from_bytes, METH_CLASS|METH_O, "from_bytes"},
    {"from_param", (PyCFunction) QuadObject_from_param, METH_CLASS|METH_O, "from_param"},
    {"__getstate__", (PyCFunction) QuadObject___getstate__, METH_NOARGS, "Pickle a quad object object" },
    {"__setstate__", (PyCFunction) QuadObject___setstate__, METH_O,"Un-pickle a quad object object"},
    {"hex", (PyCFunction) QuadObject_to_hex, METH_NOARGS, "to_hex"},
    {"fromhex", (PyCFunction) QuadObject_from_hex, METH_CLASS|METH_O, "from_hex"},    
    
    {NULL}  /* Sentinel */
};

// Properties 
static PyGetSetDef Quad_cgetset[] = {
    {"_as_parameter_", _as_parameter_, NULL, "ctypes _as_parameter_" },
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
        
    if(!PyObject_to_QuadObject(obj, self, true)){
        PyErr_SetString(PyExc_TypeError, "Can not convert value to quad precision.");
        return -1;
    }

    return 0;
}


static PyTypeObject QuadType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "qfloat",
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
    .tp_getset = Quad_cgetset,
    .tp_richcompare = (richcmpfunc) QuadType_RichCompare,
    .tp_hash = (hashfunc) QuadObject_hash,
};

static PyModuleDef QuadModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "qmfloat",
    .m_doc = PyDoc_STR("Quad precision module for scalar quad's."),
    .m_size = -1,
};

PyObject* 
QuadObject_to_PyObject(QuadObject out) {
	QuadObject* ret = (QuadObject*) PyType_GenericAlloc(&QuadType, 0);
    
	if (ret != NULL) {
		ret->value = out.value;
	}

	return (PyObject*) ret;
}


static __float128 QuadObject_float128(QuadObject * out) {
    return out->value;
}


bool
PyObject_to_QuadObject(PyObject * in, QuadObject * out, const bool alloc)
{
    if(alloc)
        alloc_QuadType(out);

    if(QuadObject_Check(in)){
        // Is a quad
        out->value = QuadObject_float128((QuadObject *) in);
        return true;
    }

    if(PyUnicode_Check(in)){
        // Is a string
        const char *buf = PyUnicode_AsUTF8AndSize(in, NULL);
        if (buf==NULL){
            PyErr_Print();
            return false;
        }

        if(strcmp(buf,"nan")==0) {
            out->value = nanq("");
        } else if (strcmp(buf,"inf")==0){
            out->value = INFINITY;
        } else if (strcmp(buf,"-inf")==0){
            out->value = -INFINITY;
        } else {
            char *sp=NULL;

            out->value = strtoflt128(buf, &sp);
            if(sp!=NULL){
                if(strcmp(sp,"")!=0){
                   // printf("%s %d\n",sp,strcmp(sp,""));
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

static bool QuadObject_Check(PyObject * obj){
    if(PyObject_TypeCheck(obj, &QuadType))
        return true;
    return false;
}


static double QuadObject_to_double(QuadObject * x){
    double ret;
    
    if(x->value > DBL_MAX){
        ret = INFINITY;
    } else if(x->value < -DBL_MAX){
        ret = -INFINITY;
    } else if (isinfq(x->value)) {
        ret = INFINITY;
    } else if (isnanq(x->value)) {
        ret = NAN;
    } else {
        ret = x->value;
    }

    return ret;

}

static double __float128_to_double(__float128 x){
    double ret;
    
    if(x > DBL_MAX){
        ret = INFINITY;
    } else if(x < -DBL_MAX){
        ret = -INFINITY;
    } else if (isinfq(x)) {
        ret = INFINITY;
    } else if (isnanq(x)) {
        ret = NAN;
    } else {
        ret = x;
    }

    return ret;

}





#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void qprintf(QuadObject * out){
    char output[QUAD_BUF];
    quadmath_snprintf(output, sizeof output, "%.36Qg", 35, out->value);
    printf("%s\n", output);
}
#pragma GCC diagnostic pop

static void alloc_QuadType(QuadObject * result){
	result = (QuadObject*) PyType_GenericAlloc(&QuadType, 0);
}

PyMODINIT_FUNC
PyInit_qmfloat(void)
{
    PyObject *m;
    static void *PyQfloat_API[PyQfloat_API_pointers];
    PyObject *c_api_object;


    if (PyType_Ready(&QuadType) < 0)
        return NULL;

    m = PyModule_Create(&QuadModule);
    if (m == NULL)
        return NULL;

    /* Initialize the C API pointer array */
    PyQfloat_API[PyQfloat_q2py_NUM] = (void *)QuadObject_to_PyObject;
    PyQfloat_API[PyQfloat_py2q_NUM] = (void *)PyObject_to_QuadObject;
    PyQfloat_API[PyQfloat_alloc_NUM] = (void *)alloc_QuadType;
    PyQfloat_API[PyQfloat_float128_NUM] = (void *)QuadObject_float128;
    PyQfloat_API[PyQfloat_check_NUM] = (void *)QuadObject_Check;
    PyQfloat_API[PyQfloat_dble_NUM] = (void *)QuadObject_to_double;
    PyQfloat_API[PyQfloat_f_to_dble_NUM] = (void *) __float128_to_double;


    Py_INCREF(&QuadType);
    if (PyModule_AddObject(m, "qfloat", (PyObject *) &QuadType) < 0) {
        Py_DECREF(&QuadType);
        Py_DECREF(m);
        return NULL;
    }


    /* Create a Capsule containing the API pointer array's address */
    c_api_object = PyCapsule_New((void *)PyQfloat_API, "pyquadp.qmfloat._C_API", NULL);

    if (PyModule_AddObject(m, "_C_API", c_api_object) < 0) {
        Py_XDECREF(c_api_object);
        Py_DECREF(m);
        return NULL;
    }


    return m;
}
