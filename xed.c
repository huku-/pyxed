#include "includes.h"
#include "xed.h"
#include "instruction.h"


static int init(PyObject *self, PyObject *args, PyObject *kwds)
{
    XED *xed = (XED *)self;
    xed->runtime_address = PyLong_FromLong(0x10001000);
    xed->mmode = XED_MACHINE_MODE_LONG_COMPAT_32;
    xed->stack_addr_width = XED_ADDRESS_WIDTH_32b;
    xed->itext_offset = 0;
    return 0;
}


static PyObject *decode(PyObject *self, PyObject *args)
{
    XED *xed = (XED *)self;
    xed_decoded_inst_t *decoded_inst;
    xed_uint64_t runtime_address;
    const xed_uint8_t *buf;
    PyObject *r = NULL;

    /* Make sure we have a valid input buffer. */
    if(xed->itext == NULL || PyString_Check(xed->itext) == 0)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid instruction text");
        goto _err;
    }

    /* Make sure we have a valid runtime address. */
    if(PyInt_Check(xed->runtime_address) == 0 &&
            PyLong_Check(xed->runtime_address)  == 0)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid runtime address");
        goto _err;
    }

    /* Have we finished decoding? */
    if(xed->itext_offset >= PyString_Size(xed->itext))
    {
        Py_INCREF(Py_None);
        r = Py_None;
        goto _err;
    }

    /* Decode next instruction from the input buffer. */
    decoded_inst = PyMem_Malloc(sizeof(xed_decoded_inst_t));
    xed_decoded_inst_zero(decoded_inst);
    xed_decoded_inst_set_mode(decoded_inst, xed->mmode, xed->stack_addr_width);
    buf = (const xed_uint8_t *)PyString_AsString(xed->itext) +
        xed->itext_offset;
    xed_decode(decoded_inst, buf, PyString_Size(xed->itext) -
        xed->itext_offset);

    /* Compute the instruction's runtime address and update saved offset by the
     * number of bytes corresponding to the decoded instruction (the call to
     * `xed_decoded_inst_get_length()' will return a valid length even if the
     * decoded instruction is not valid).
     */
    runtime_address = PyLong_AsLong(xed->runtime_address) + xed->itext_offset;
    xed->itext_offset += xed_decoded_inst_get_length(decoded_inst);

    /* If the instruction is not valid, return `None'. */
    if(xed_decoded_inst_valid(decoded_inst) == 0)
    {
        PyMem_Free(decoded_inst);
        Py_INCREF(Py_None);
        r = Py_None;
        goto _err;
    }

    r = (PyObject *)new_instruction(decoded_inst, runtime_address);

_err:
    return r;
}


static PyObject *set_mode(PyObject *self, PyObject *args)
{
    XED *xed = (XED *)self;
    unsigned int mmode, stack_addr_width;
    if(PyArg_ParseTuple(args, "II", &mmode, &stack_addr_width) != 0)
    {
        xed->mmode = mmode;
        xed->stack_addr_width = stack_addr_width;
    }
    Py_RETURN_NONE;
}


static PyMemberDef members[] =
{
    {"itext", T_OBJECT, offsetof(XED, itext), 0,
        "String object holding instruction text bytes"},
    {"runtime_address", T_OBJECT, offsetof(XED, runtime_address), 0,
        "Runtime address of the instruction text being disassembled"},
    {NULL}
};

static PyMethodDef methods[] =
{
    {"set_mode", set_mode, METH_VARARGS,
        "Equivalent to `xed_decoded_inst_set_mode()'"},
    {"decode", decode, METH_NOARGS, "Equivalent to `xed_decode()'"},
    {NULL}
};

static PyTypeObject type =
{
    PyObject_HEAD_INIT(NULL)
    .tp_name = "pyxed.XED",
    .tp_basicsize = sizeof(XED),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Main decoder object",
    .tp_methods = methods,
    .tp_members = members,
    .tp_init = init,
    .tp_new = PyType_GenericNew
};


void register_xed_object(PyObject *module)
{
    if(PyType_Ready(&type) == 0)
    {
        Py_INCREF(&type);
        PyModule_AddObject(module, "XED", (PyObject *)&type);
    }
}

