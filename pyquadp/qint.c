// SPDX-License-Identifier: GPL-2.0+
#include "pyquadp.h"

#define QINT_MODULE
#include "qint.h"

static __uint128_t u128_from_i128(__int128 v)
{
    return (__uint128_t)v;
}

static __int128 wrap_add_i128(__int128 a, __int128 b)
{
    return (__int128)(u128_from_i128(a) + u128_from_i128(b));
}

static __int128 wrap_sub_i128(__int128 a, __int128 b)
{
    return (__int128)(u128_from_i128(a) - u128_from_i128(b));
}

static __int128 wrap_mul_i128(__int128 a, __int128 b)
{
    return (__int128)(u128_from_i128(a) * u128_from_i128(b));
}

static bool floor_divmod_i128(__int128 a, __int128 b, __int128 *q, __int128 *r)
{
    __int128 q0;
    __int128 r0;

    if (b == 0) {
        PyErr_SetString(PyExc_ZeroDivisionError, "division or modulo by zero");
        return false;
    }

    q0 = a / b;
    r0 = a % b;

    if (r0 != 0 && ((r0 > 0) != (b > 0))) {
        r0 = wrap_add_i128(r0, b);
        q0 = wrap_sub_i128(q0, 1);
    }

    *q = q0;
    *r = r0;
    return true;
}

static bool normalize_shift_count(PyObject *obj, long *count)
{
    long i;

    if (!PyLong_Check(obj)) {
        PyErr_SetString(PyExc_TypeError, "shift count must be an integer");
        return false;
    }

    i = PyLong_AsLong(obj);
    if (i == -1 && PyErr_Occurred()) {
        return false;
    }

    if (i < 0) {
        PyErr_SetString(PyExc_ValueError, "negative shift count");
        return false;
    }

    *count = i;
    return true;
}

static PyObject *
QuadIObject_true_divide(PyObject * o1, PyObject * o2 ){
    QuadIObject q1, q2;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadIObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if (q2.value == 0) {
        PyErr_SetString(PyExc_ZeroDivisionError, "division by zero");
        return NULL;
    }

    return PyFloat_FromDouble((double)q1.value / (double)q2.value);
}

static PyObject *
QuadIObject_inplace_true_divide(PyObject * o1, PyObject * o2 ){
    return QuadIObject_true_divide(o1, o2);
}



static PyObject *
QuadIObject_repr(QuadIObject * obj)
{
    char buf[QUAD_INT_STR_BUF];

    if(int128_to_str(obj->value, buf, QUAD_INT_STR_BUF, 10)){
        return PyUnicode_FromFormat("qint(%s)",buf);
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

    switch(op){
        case OP_negative:
            result.value = wrap_sub_i128(0, q1.value);
            break;
        case OP_positive:
            result.value = q1.value;
            break;
        case OP_absolute:
            if (q1.value < 0) {
                result.value = wrap_sub_i128(0, q1.value);
            } else {
                result.value = q1.value;
            }
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

    switch(op){
        case OP_add:
            result.value = wrap_add_i128(q1.value, q2.value);
            break;
        case OP_sub:
            result.value = wrap_sub_i128(q1.value, q2.value);
            break;
        case OP_mult:
            result.value = wrap_mul_i128(q1.value, q2.value);
            break;
        case OP_remainder: {
            __int128 q;
            __int128 r;
            if (!floor_divmod_i128(q1.value, q2.value, &q, &r)) {
                return NULL;
            }
            result.value = r;
            break;
        }
        case OP_floor_divide: {
            __int128 q;
            __int128 r;
            if (!floor_divmod_i128(q1.value, q2.value, &q, &r)) {
                return NULL;
            }
            result.value = q;
            break;
        }
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
            q1.value = wrap_add_i128(q1.value, q2.value);
            break;
        case OP_sub:
            q1.value = wrap_sub_i128(q1.value, q2.value);
            break;
        case OP_mult:
            q1.value = wrap_mul_i128(q1.value, q2.value);
            break;
        case OP_remainder: {
            __int128 q;
            __int128 r;
            if (!floor_divmod_i128(q1.value, q2.value, &q, &r)) {
                return NULL;
            }
            q1.value = r;
            break;
        }
        case OP_floor_divide: {
            __int128 q;
            __int128 r;
            if (!floor_divmod_i128(q1.value, q2.value, &q, &r)) {
                return NULL;
            }
            q1.value = q;
            break;
        }
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadIObject_to_PyObject(q1);
}

static PyObject *
QuadIObject_binary_op2_int(const int op, PyObject * o1, PyObject * o2 ){

    QuadIObject q1, q2, result;
    long int i;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadIObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    switch(op){
        case OP_lshift:
            if (!normalize_shift_count(o2, &i)) {
                return NULL;
            }
            if (i >= 128) {
                result.value = 0;
            } else {
                result.value = (__int128)(u128_from_i128(q1.value) << i);
            }
            break;
        case OP_rshift:
            if (!normalize_shift_count(o2, &i)) {
                return NULL;
            }
            if (i >= 128) {
                result.value = (q1.value < 0) ? -1 : 0;
            } else {
                result.value = q1.value >> i;
            }
            break;
        case OP_and:
            result.value = q1.value & q2.value;
            break;
        case OP_xor:
            result.value = q1.value ^ q2.value;
            break;
        case OP_or:
            result.value = q1.value | q2.value;
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

    QuadIObject q1, q2;
    long int i;

    if(!PyObject_to_QuadIObject(o1, &q1, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    if(!PyObject_to_QuadIObject(o2, &q2, true)){
        Py_RETURN_NOTIMPLEMENTED;
    }

    switch(op){
        case OP_inplace_lshift:
            if (!normalize_shift_count(o2, &i)) {
                return NULL;
            }
            if (i >= 128) {
                q1.value = 0;
            } else {
                q1.value = (__int128)(u128_from_i128(q1.value) << i);
            }
            break;
        case OP_inplace_rshift:
            if (!normalize_shift_count(o2, &i)) {
                return NULL;
            }
            if (i >= 128) {
                q1.value = (q1.value < 0) ? -1 : 0;
            } else {
                q1.value >>= i;
            }
            break;
        case OP_inplace_and:
            q1.value &= q2.value;
            break;
        case OP_inplace_xor:
            q1.value ^= q2.value;
            break;
        case OP_inplace_or:
            q1.value |= q2.value;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    return QuadIObject_to_PyObject(q1);
}


static PyObject *
QuadIObject_inplace_lshift(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_inplace_lshift, o1, o2);
}

static PyObject *
QuadIObject_inplace_rshift(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_inplace_rshift, o1, o2);
}

static PyObject *
QuadIObject_inplace_and(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_inplace_and, o1, o2);
}

static PyObject *
QuadIObject_inplace_xor(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_inplace_xor, o1, o2);
}

static PyObject *
QuadIObject_inplace_or(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2_int(OP_inplace_or, o1, o2);
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
    PyObject *q = QuadIObject_binary_op2(OP_floor_divide, o1, o2);
    PyObject *r;

    if (q == NULL) {
        return NULL;
    }

    r = QuadIObject_binary_op2(OP_remainder, o1, o2);
    if (r == NULL) {
        Py_DECREF(q);
        return NULL;
    }

    return Py_BuildValue("(NN)", q, r);
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

    if (q2.value < 0 && o3 != Py_None) {
        PyErr_SetString(PyExc_ValueError, "pow() 2nd argument cannot be negative when 3rd argument specified");
        return NULL;
    }

    result.value = (__int128)powq((__float128)q1.value, (__float128)q2.value);

    if(o3 != Py_None) {
        __int128 q;
        __int128 r;
        if(!PyObject_to_QuadIObject(o3, &q3, true))
            Py_RETURN_NOTIMPLEMENTED;

        if (!floor_divmod_i128(result.value, q3.value, &q, &r)) {
            return NULL;
        }
        result.value = r;
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

    {
        char buf[QUAD_INT_STR_BUF];
        if (!int128_to_str(q1.value, buf, QUAD_INT_STR_BUF, 10)) {
            PyErr_SetString(PyExc_ValueError, "Could not convert qint to python int.");
            return NULL;
        }
        result = PyLong_FromString(buf, NULL, 10);
        if (result == NULL) {
            return NULL;
        }
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


    if (q2.value < 0 && o3 != Py_None) {
        PyErr_SetString(PyExc_ValueError, "pow() 2nd argument cannot be negative when 3rd argument specified");
        return NULL;
    }

    q1.value = (__int128)powq((__float128)q1.value, (__float128)q2.value);

    if(o3 != Py_None) {
        __int128 q;
        __int128 r;
        if(!PyObject_to_QuadIObject(o3, &q3,true))
            Py_RETURN_NOTIMPLEMENTED;

        if (!floor_divmod_i128(q1.value, q3.value, &q, &r)) {
            return NULL;
        }
        q1.value = r;
    }

    return QuadIObject_to_PyObject(q1);

}

static PyObject *
QuadIObject_floor_divide(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_op2(OP_floor_divide, o1, o2);
}

static PyObject *
QuadIObject_inplace_floor_divide(PyObject * o1, PyObject * o2 ){
    return QuadIObject_binary_inplace_op2(OP_floor_divide, o1, o2);
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
        PyErr_SetString(PyExc_KeyError, "No pickle version in pickled dict.");
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
    PyObject *as_int = QuadIObject_int((PyObject *)self);
    Py_hash_t h;

    if (as_int == NULL) {
        return -1;
    }

    h = PyObject_Hash(as_int);
    Py_DECREF(as_int);
    return h;
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

    (unaryfunc) QuadIObject_int,//  unaryfunc nb_index;

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
    .tp_name = "qint",
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
    .m_name = "qmint",
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
    (void)alloc;

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
            return false;
        }

        if(!str_to_int128(buf, size, &out->value)){
            PyErr_SetString(PyExc_ValueError, "Overflow error");
            return false;
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
                Py_XDECREF(str);
                return false;
            }

            if(!str_to_int128(buf,len,&out->value)){
                Py_DECREF(str);
                PyErr_SetString(PyExc_TypeError, "Could not convert int to integer quad precision.");
                return false;
            }

            Py_DECREF(str);

            return true;
        }
    }

    return false;

}

static bool QuadIObject_Check(PyObject * obj){
    return PyObject_TypeCheck(obj, &QuadIType);
}


static void alloc_QuadIType(QuadIObject * result){
    result->value = 0;
}

PyMODINIT_FUNC
PyInit_qmint(void)
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
    PyQInt_API[PyQInt_type_NUM] = (void *)&QuadIType;


    Py_INCREF(&QuadIType);
    if (PyModule_AddObject(m, "qint", (PyObject *) &QuadIType) < 0) {
        Py_DECREF(&QuadIType);
        Py_DECREF(m);
        return NULL;
    }


    /* Create a Capsule containing the API pointer array's address */
    c_api_object = PyCapsule_New((void *)PyQInt_API, "pyquadp.qmint._C_API", NULL);

    if (PyModule_AddObject(m, "_C_API", c_api_object) < 0) {
        Py_XDECREF(c_api_object);
        Py_DECREF(m);
        return NULL;
    }


    return m;
}

// TODO: Handle hex values?
bool str_to_int128(const char *str, Py_ssize_t length, __int128 *result){
    Py_ssize_t i = 0;
    int sign = 1;
    __uint128_t acc = 0;
    bool has_digit = false;
    __uint128_t max_abs = (((__uint128_t)1) << 127);
    __uint128_t limit = max_abs - 1u;

    if (length == 0)
        return false;

    while (i < length && isspace((unsigned char)str[i]))
        i++;

    if (i < length && (str[i] == '+' || str[i] == '-')) {
        sign = (str[i] == '-') ? -1 : 1;
        i++;
    }

    if (sign < 0) {
        limit = max_abs;
    }

    while (i < length && isspace((unsigned char)str[i]))
        i++;

    for (; i < length; i++) {
        unsigned char c = (unsigned char)str[i];
        if (isspace(c)) {
            while (i < length && isspace((unsigned char)str[i]))
                i++;
            if (i != length)
                return false;
            break;
        }
        if (!isdigit(c))
            return false;

        has_digit = true;

        if (acc > (limit / 10u))
            return false;
        if (acc == (limit / 10u) && (unsigned)(c - '0') > (unsigned)(limit % 10u))
            return false;

        acc = acc * 10u + (unsigned)(c - '0');
        if (acc > limit)
            return false;
    }

    if (!has_digit)
        return false;

    if (sign > 0) {
        *result = (__int128)acc;
    } else if (acc == max_abs) {
        *result = (__int128)max_abs;
    } else {
        *result = -(__int128)acc;
    }

    return true;
}

bool int128_to_str(__int128 num, char* str, int len, int base)
{
	int i = 0,k=0;
	int digit;
    bool neg=false;
    char temp;
    __uint128_t usum;
	if (len == 0)
		return false;

    if (base < 2 || base > 36)
        return false;

    if(num<0){
        usum = (__uint128_t)(-(num + 1)) + 1u;
        neg=true;
    } else {
        usum = (__uint128_t)num;
    }
	do
	{
        digit = (int)(usum % (unsigned)base);
		if (digit < 10)
			str[i++] = '0' + digit;
		else
			str[i++] = 'A' + digit - 10;

        usum /= (unsigned)base;
    }while (usum && (i < (len - 1)));
    if (i == (len - 1) && usum)
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

