// SPDX-License-Identifier: GPL-2.0+

// Helpful guide https://github.com/moble/quaternion/blob/main/src/numpy_quaternion.c

#define NPY_TARGET_VERSION NPY_2_0_API_VERSION
#define NPY_NO_DEPRECATED_API NPY_2_0_API_VERSION

#include "pyquadp.h"

#include <numpy/arrayobject.h>
#include <numpy/npy_math.h>
#include <numpy/ufuncobject.h>
#include <stdalign.h>

#define QFLOATARRAY_MODULE
#include "qfloatarray.h"
#include "qfloat.h"

static PyTypeObject QuadArrayType;
PyArray_ArrFuncs QuadArrayFuncs;
PyArray_Descr* QuadArrayDescr;
PyArray_DescrProto QuadArrayDescrProto;

static PyModuleDef QuadArrayModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "qarray",
    .m_doc = "Quad precision module for array quad's.",
    .m_size = -1,
};


static npy_bool 
QuadArray_nonzero(void *ip, void *arr){

    int elsize = PyArray_ITEMSIZE(arr);
    char *ptr = ip;
    while (elsize--) {
        if (*ptr++ != 0) {
            return NPY_TRUE;
        }
    }
    return NPY_FALSE;
}

static void
QuadArray_copyswap(__float128 *dst, __float128 *src,
                    int swap, void *NPY_UNUSED(arr))
{
    if(src== NULL) return;

    if(swap!=0){
        __float128 *tmp = dst;
        dst = src;
        src = tmp;
    } else {
        dst = src;
    }
}

static void
QuadArray_copyswapn(void *dst, npy_intp dstride, void *src,
                   npy_intp sstride, npy_intp n, int swap, void *arr)
{
    npy_intp i;
    PyArray_CopySwapFunc *copyswap;
    char *dstptr = dst;
    char *srcptr = src;

    copyswap = PyDataType_GetArrFuncs(PyArray_DESCR(arr))->copyswap;

    for (i = 0; i < n; i++) {
        copyswap(dstptr, srcptr, swap, arr);
        dstptr += dstride;
        srcptr += sstride;
    }
}


static int QuadArray_setitem(PyObject* item, __float128* data, void* array){

    QuadObject *tmp;

    if(PyObject_to_QuadObject(item, tmp, true)){
        data = &tmp->value;
    } else {
        PyErr_SetString(PyExc_TypeError,
                    "Failed to setitem in QuadArray");
        return -1;
    }
    return 0;
}

static PyObject *
QuadArray_getitem(__float128* data, void* NPY_UNUSED(arr))
{
  QuadObject tmp;

  tmp.value = *data;

  return QuadObject_to_PyObject(tmp);
}

static int
QuadArray_compare(__float128 *pa, __float128 *pb, PyArrayObject *NPY_UNUSED(ap))
{
  npy_bool anan, bnan;
  int ret;

  anan = isnanq(*pa);
  bnan = isnanq(*pb);

  if (anan) {
    ret = bnan ? 0 : -1;
  } else if (bnan) {
    ret = 1;
  } else if(*pa<*pb) {
    ret = -1;
  } else if(*pa>*pb) {
    ret = 1;
  } else {
    ret = 0;
  }

  return ret;
}


static int
QuadArray_argmax(__float128 *ip, npy_intp n, npy_intp *max_ind, PyArrayObject *NPY_UNUSED(aip))
{
  npy_intp i;
  __float128 mp = *ip;

  *max_ind = 0;

  if (isnanq(mp)) {
    // nan encountered; it's maximal
    return 0;
  }

  for (i = 1; i < n; i++) {
    ip++;
    //Propagate nans, similarly as max() and min()
    if (!(*ip<mp)) {  // negated, for correct nan handling
      mp = *ip;
      *max_ind = i;
      if (isnanq(mp)) {
        // nan encountered, it's maximal
        break;
      }
    }
  }
  return 0;
}

static void
QuadArray_fillwithscalar(__float128 *buffer, npy_intp length, __float128 *value, void *NPY_UNUSED(ignored))
{
  npy_intp i;
  __float128 val = *value;

  for (i = 0; i < length; ++i) {
    buffer[i] = val;
  }
}

PyMODINIT_FUNC
PyInit_qfloatArray(void)
{

    PyObject *m;
    PyObject* numpy;
    PyObject* numpy_dict;
    int qarrayNum;

    m = PyModule_Create(&QuadArrayModule);
    if (m == NULL)
        return NULL;

    // Initialize numpy
    import_array();
    if (PyErr_Occurred()) {
        return NULL;
    }
    import_umath();
    if (PyErr_Occurred()) {
        return NULL;
    }
    numpy = PyImport_ImportModule("numpy");
    if (!numpy) {
        return NULL;
    }
    numpy_dict = PyModule_GetDict(numpy);
    if (!numpy_dict) {
        return NULL;
    }

    QuadArrayType.tp_base = &PyGenericArrType_Type;
    if (PyType_Ready(&QuadArrayType) < 0) {
        PyErr_Print();
        PyErr_SetString(PyExc_SystemError, "Could not initialize QuadArrayType.");
        return NULL;
    }

    PyArray_InitArrFuncs(&QuadArrayFuncs);
    QuadArrayFuncs.nonzero = (PyArray_NonzeroFunc*) QuadArray_nonzero;
    QuadArrayFuncs.copyswap = (PyArray_CopySwapFunc*) QuadArray_copyswap;
    QuadArrayFuncs.copyswapn = (PyArray_CopySwapNFunc*) QuadArray_copyswapn;
    QuadArrayFuncs.setitem = (PyArray_SetItemFunc*) QuadArray_setitem;
    QuadArrayFuncs.getitem = (PyArray_GetItemFunc*) QuadArray_getitem;
    QuadArrayFuncs.compare = (PyArray_CompareFunc*) QuadArray_compare;
    QuadArrayFuncs.argmax = (PyArray_ArgFunc*) QuadArray_argmax;
    QuadArrayFuncs.fillwithscalar = (PyArray_FillWithScalarFunc*) QuadArray_fillwithscalar;


    Py_SET_TYPE(&QuadArrayDescrProto, &PyArrayDescr_Type);
    QuadArrayDescrProto.typeobj = &QuadArrayType;
    QuadArrayDescrProto.kind = 'V';
    QuadArrayDescrProto.type = 'f';
    QuadArrayDescrProto.byteorder = '=';
    QuadArrayDescrProto.flags = NPY_NEEDS_PYAPI | NPY_USE_GETITEM | NPY_USE_SETITEM;
    QuadArrayDescrProto.type_num = 0; // assigned at registration
    QuadArrayDescrProto.elsize = sizeof(__float128);
    QuadArrayDescrProto.alignment = alignof(__float128);
    QuadArrayDescrProto.f = &QuadArrayFuncs;
    QuadArrayDescrProto.subarray = NULL;
    QuadArrayDescrProto.fields = NULL;
    QuadArrayDescrProto.names = NULL;
    QuadArrayDescrProto.metadata = NULL;
    QuadArrayDescrProto.c_metadata = NULL;

    Py_INCREF(&QuadArrayType);
    qarrayNum = PyArray_RegisterDataType(&QuadArrayDescrProto);

    if (qarrayNum < 0) {
        return NULL;
    }
    QuadArrayDescr = PyArray_DescrFromType(qarrayNum);


    PyModule_AddObject(m, "qarray", (PyObject *)&QuadArrayType);

    return m;
}