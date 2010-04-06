#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include <CoreServices/CoreServices.h>

static PyObject* send2trash_osx_send(PyObject *self, PyObject *args)
{
    UInt8 *utf8_chars;
    FSRef fp;
    OSStatus op_result;
    
    if (!PyArg_ParseTuple(args, "es", "utf-8", &utf8_chars)) {
        return NULL;
    }
    
	FSPathMakeRefWithOptions(utf8_chars, kFSPathMakeRefDoNotFollowLeafSymlink, &fp, NULL);
	op_result = FSMoveObjectToTrashSync(&fp, NULL, kFSFileOperationDefaultOptions);
	if (op_result != noErr) {
        PyErr_SetString(PyExc_OSError, GetMacOSStatusCommentString(op_result));
        return NULL;
    }
    return Py_None;
}

static PyMethodDef TrashMethods[] = {
    {"send",  send2trash_osx_send, METH_VARARGS, ""},
};

PyMODINIT_FUNC
init_send2trash_osx(void)
{
    PyObject *m = Py_InitModule("_send2trash_osx", TrashMethods);
    if (m == NULL) {
        return;
    }
}
