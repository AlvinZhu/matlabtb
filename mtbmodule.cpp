#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "Python.h"
#include "numpy/arrayobject.h"
#include "imResampleMex.hpp"

static PyObject *
imResample(PyObject *dummy, PyObject *args) {
    PyObject * arg1 = NULL, *out = NULL;
    PyArrayObject *arr1 = NULL, *oarr = NULL;

    if (!PyArg_ParseTuple(args, "O!O!", &PyArray_Type, &arg1,
                          &PyArray_Type, &out))
        return NULL;

    arr1 = (PyArrayObject *) PyArray_FROM_OF(arg1, NPY_ARRAY_F_CONTIGUOUS | NPY_ARRAY_ALIGNED);
    if (arr1 == NULL) return NULL;
    oarr = (PyArrayObject *) PyArray_FROM_OF(out, NPY_ARRAY_F_CONTIGUOUS | NPY_ARRAY_ALIGNED | NPY_ARRAY_WRITEABLE | NPY_ARRAY_UPDATEIFCOPY);
    if (oarr == NULL) {
        Py_XDECREF(arr1);
        PyArray_XDECREF_ERR(oarr);
        return NULL;
    }
    
    void *a = PyArray_DATA(arr1);
    void *b = PyArray_DATA(oarr);
    int nd_a = PyArray_NDIM(arr1);
    int nd_b = PyArray_NDIM(oarr);
    npy_intp *shape_a = PyArray_DIMS(arr1);
    npy_intp *shape_b = PyArray_DIMS(oarr);
    int type_a = PyArray_TYPE(arr1);
    int type_b = PyArray_TYPE(oarr);

    if (nd_a != nd_b || nd_a < 2 || nd_a > 3 || type_a != type_b){
        Py_DECREF(arr1);
        Py_DECREF(oarr);
        Py_INCREF(Py_None);
        return Py_None;
    }
    

    int ha = (int) shape_a[0];
    int hb = (int) shape_b[0];
    int wa = (int) shape_a[1];
    int wb = (int) shape_b[1];
    int d = 1;
    if (nd_a == 3){
        d = (int) shape_a[2];
    }

    if (type_a == NPY_FLOAT){
        resample<float>((float *) a, (float *) b, ha, hb, wa, wb, d, 1.0);
    } else if (type_a == NPY_DOUBLE) {
        resample<double>((double *) a, (double *) b, ha, hb, wa, wb, d, 1.0);
    }

    Py_DECREF(arr1);
    Py_DECREF(oarr);
    Py_INCREF(Py_None);
    return Py_None;
}

/* ==== Set up the methods table ====================== */
static struct PyMethodDef methods[] = {
        {"imResample", imResample, METH_VARARGS, "resample A using bilinear interpolation and and store result in B"},
        {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef moduledef = {
       PyModuleDef_HEAD_INIT,
       "matlabtb",
       "Implementation of PioTr Toolbox imResample in Python",
       NULL,
       methods,
       NULL,
       NULL,
       NULL,
       NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_matlabtb(void)

#else
#define INITERROR return

PyMODINIT_FUNC
initmatlabtb(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    	PyObject *module = PyModule_Create(&moduledef);
#else	
	 PyObject *module = Py_InitModule("matlabtb", methods);
#endif
    if (module == NULL)
        INITERROR;

    PyModule_AddStringConstant(module, "__author__", "Shashikant Ghangare <shashikant.ghangare@outlook.in>");
    PyModule_AddStringConstant(module, "__version__", "2");
    import_array();  // Must be present for NumPy.  Called first after above line.
#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}

