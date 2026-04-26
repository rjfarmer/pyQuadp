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
    PyObject *bytes_obj = QuadObject_to_bytes(self, NULL);
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
QuadObject___setstate__(QuadObject *self, PyObject *state) {

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
        PyErr_SetString(PyExc_ValueError, "Byte array wrong size for a quad");
        return NULL;
    }

    Py_RETURN_NONE;
}


Py_hash_t QuadObject_hash(QuadObject *self){
    // Note this wont have the nice property that Python has with numeric values being equal 
    // hash to the same value i.e hash(1) == hash(1.0)
    PyObject *bytes_obj = QuadObject_to_bytes(self, NULL);
    Py_hash_t h;

    if (bytes_obj == NULL) {
        return -1;
    }

    h = PyObject_Hash(bytes_obj);
    Py_DECREF(bytes_obj);
    return h;
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


static int
hex_digit_value(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    }
    if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    }
    return -1;
}


static PyObject *
u128_to_pylong(__uint128_t v)
{
    unsigned long long lo = (unsigned long long)v;
    unsigned long long hi = (unsigned long long)(v >> 64);
    PyObject *lo_obj;

    lo_obj = PyLong_FromUnsignedLongLong(lo);
    if (lo_obj == NULL) {
        return NULL;
    }

    if (hi == 0ULL) {
        return lo_obj;
    }

    {
        PyObject *hi_obj = PyLong_FromUnsignedLongLong(hi);
        PyObject *shift_obj = PyLong_FromLong(64);
        PyObject *hi_shifted;
        PyObject *res;

        if (hi_obj == NULL || shift_obj == NULL) {
            Py_XDECREF(hi_obj);
            Py_XDECREF(shift_obj);
            Py_DECREF(lo_obj);
            return NULL;
        }

        hi_shifted = PyNumber_Lshift(hi_obj, shift_obj);
        Py_DECREF(hi_obj);
        Py_DECREF(shift_obj);
        if (hi_shifted == NULL) {
            Py_DECREF(lo_obj);
            return NULL;
        }

        res = PyNumber_Add(hi_shifted, lo_obj);
        Py_DECREF(hi_shifted);
        Py_DECREF(lo_obj);
        return res;
    }
}


static PyObject *
QuadObject_as_integer_ratio(QuadObject *self, PyObject *Py_UNUSED(ignored))
{
    char buf[QUAD_BUF];
    char *p_exp;
    char *p;
    long exp2;
    long frac_nibbles = 0;
    long shift_exp;
    long den_shift;
    int seen_dot = 0;
    int neg = signbitq(self->value);
    __uint128_t mant = 0;
    PyObject *num_obj;
    PyObject *den_obj;
    PyObject *tuple_obj;

    if (isnanq(self->value)) {
        PyErr_SetString(PyExc_ValueError, "cannot convert NaN to integer ratio");
        return NULL;
    }
    if (isinfq(self->value)) {
        PyErr_SetString(PyExc_OverflowError, "cannot convert Infinity to integer ratio");
        return NULL;
    }
    if (self->value == 0.0Q) {
        return Py_BuildValue("(ii)", 0, 1);
    }

    if (quadmath_snprintf(buf, sizeof buf, "%Qa", fabsq(self->value)) <= 0) {
        PyErr_SetString(PyExc_ValueError, "failed to convert qfloat to hexadecimal form");
        return NULL;
    }

    p_exp = strchr(buf, 'p');
    if (p_exp == NULL) {
        PyErr_SetString(PyExc_ValueError, "unexpected hexadecimal form for qfloat");
        return NULL;
    }

    exp2 = strtol(p_exp + 1, NULL, 10);

    p = buf;
    if (p[0] == '-') {
        p++;
    }
    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
        p += 2;
    }

    while (p < p_exp) {
        int d;
        if (*p == '.') {
            seen_dot = 1;
            p++;
            continue;
        }
        d = hex_digit_value(*p);
        if (d < 0) {
            PyErr_SetString(PyExc_ValueError, "unexpected hexadecimal digit in qfloat");
            return NULL;
        }
        mant = (mant << 4) | (__uint128_t)d;
        if (seen_dot) {
            frac_nibbles++;
        }
        p++;
    }

    shift_exp = exp2 - (4 * frac_nibbles);
    if (shift_exp < 0) {
        den_shift = -shift_exp;
        while (den_shift > 0 && (mant & 1U) == 0) {
            mant >>= 1;
            den_shift--;
        }
    } else {
        den_shift = 0;
    }

    num_obj = u128_to_pylong(mant);
    if (num_obj == NULL) {
        return NULL;
    }

    if (shift_exp > 0) {
        PyObject *shift_obj = PyLong_FromLong(shift_exp);
        PyObject *shifted;
        if (shift_obj == NULL) {
            Py_DECREF(num_obj);
            return NULL;
        }
        shifted = PyNumber_Lshift(num_obj, shift_obj);
        Py_DECREF(shift_obj);
        Py_DECREF(num_obj);
        if (shifted == NULL) {
            return NULL;
        }
        num_obj = shifted;
    }

    if (neg) {
        PyObject *tmp = PyNumber_Negative(num_obj);
        Py_DECREF(num_obj);
        if (tmp == NULL) {
            return NULL;
        }
        num_obj = tmp;
    }

    if (den_shift == 0) {
        den_obj = PyLong_FromLong(1);
    } else {
        PyObject *one = PyLong_FromLong(1);
        PyObject *shift_obj = PyLong_FromLong(den_shift);
        if (one == NULL || shift_obj == NULL) {
            Py_XDECREF(one);
            Py_XDECREF(shift_obj);
            Py_DECREF(num_obj);
            return NULL;
        }
        den_obj = PyNumber_Lshift(one, shift_obj);
        Py_DECREF(one);
        Py_DECREF(shift_obj);
    }

    if (den_obj == NULL) {
        Py_DECREF(num_obj);
        return NULL;
    }

    tuple_obj = PyTuple_Pack(2, num_obj, den_obj);
    Py_DECREF(num_obj);
    Py_DECREF(den_obj);
    if (tuple_obj == NULL) {
        return NULL;
    }
    return tuple_obj;
}


static PyObject *
QuadObject_round(QuadObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"ndigits", NULL};
    PyObject *ndigits_obj = NULL;
    long long ndigits = 0;
    QuadObject result;
    __float128 scale;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|O:__round__", kwlist, &ndigits_obj)) {
        return NULL;
    }

    if (ndigits_obj != NULL && ndigits_obj != Py_None) {
        ndigits = PyLong_AsLongLong(ndigits_obj);
        if (PyErr_Occurred()) {
            return NULL;
        }
    }

    alloc_QuadType(&result);
    if (ndigits_obj == NULL || ndigits_obj == Py_None) {
        result.value = roundq(self->value);
        return QuadObject_to_PyObject(result);
    }

    scale = powq(10.0Q, (__float128)ndigits);
    if (!finiteq(scale) || scale == 0.0Q) {
        PyErr_SetString(PyExc_OverflowError, "ndigits is out of range for qfloat round");
        return NULL;
    }

    result.value = roundq(self->value * scale) / scale;
    return QuadObject_to_PyObject(result);
}


static PyObject *
QuadObject_trunc(QuadObject *self, PyObject *Py_UNUSED(ignored))
{
    QuadObject result;
    alloc_QuadType(&result);
    result.value = truncq(self->value);
    return QuadObject_to_PyObject(result);
}


static PyObject *
QuadObject_floor_method(QuadObject *self, PyObject *Py_UNUSED(ignored))
{
    QuadObject result;
    alloc_QuadType(&result);
    result.value = floorq(self->value);
    return QuadObject_to_PyObject(result);
}


static PyObject *
QuadObject_ceil_method(QuadObject *self, PyObject *Py_UNUSED(ignored))
{
    QuadObject result;
    alloc_QuadType(&result);
    result.value = ceilq(self->value);
    return QuadObject_to_PyObject(result);
}


static PyObject *
QuadObject_is_integer(QuadObject *self, PyObject *Py_UNUSED(ignored))
{
    if (!finiteq(self->value)) {
        Py_RETURN_FALSE;
    }

    if (truncq(self->value) == self->value) {
        Py_RETURN_TRUE;
    }

    Py_RETURN_FALSE;
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
    {"as_integer_ratio", (PyCFunction) QuadObject_as_integer_ratio, METH_NOARGS, "Return (numerator, denominator) exact ratio for finite qfloat."},
    {"__round__", (PyCFunction) QuadObject_round, METH_VARARGS|METH_KEYWORDS, "Round qfloat and return qfloat."},
    {"__trunc__", (PyCFunction) QuadObject_trunc, METH_NOARGS, "Truncate qfloat and return qfloat."},
    {"__floor__", (PyCFunction) QuadObject_floor_method, METH_NOARGS, "Floor qfloat and return qfloat."},
    {"__ceil__", (PyCFunction) QuadObject_ceil_method, METH_NOARGS, "Ceil qfloat and return qfloat."},
    {"is_integer", (PyCFunction) QuadObject_is_integer, METH_NOARGS, "Return True if qfloat has no fractional component."},
    
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
    (void)kwds;

    if (!PyArg_ParseTuple(args, "O:", &obj)){
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
            long v = PyLong_AsLong(in);
            if (v == -1 && PyErr_Occurred()) {
                return false;
            }
            out->value = (__float128) v;
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
    PyQfloat_API[PyQfloat_type_NUM] = (void *)&QuadType;


    if (PyModule_AddType(m, &QuadType) < 0) {
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
