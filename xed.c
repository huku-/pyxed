/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * xed.c - Exports class `XED', the main decoder object.
 */
#include "includes.h"
#include "pyxed.h"
#include "check.h"
#include "xed.h"
#include "instruction.h"



static int init(PyObject *self, PyObject *args, PyObject *kwds)
{
    xed_t *xed = (xed_t *)self;
    xed->itext_offset = PyLong_FromLong(0);
    xed->runtime_address = PyLong_FromLong(0x10001000);
    xed->mmode = XED_MACHINE_MODE_LONG_COMPAT_32;
    xed->stack_addr_width = XED_ADDRESS_WIDTH_32b;
    return 0;
}


static PyObject *decode(xed_t *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst;
    xed_uint64_t runtime_address;
    Py_ssize_t itext_offset;
    const xed_uint8_t *buf;
    unsigned int bytes;
    PyObject *r = NULL;

    /* Make sure we have a valid input buffer. */
    if(is_string(self->itext) == 0)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid instruction text");
        goto _err;
    }

    /* Make sure we have a valid runtime address. */
    if(is_int_or_long(self->runtime_address) == 0)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid runtime address");
        goto _err;
    }

    /* Now verify we have a valid offset. */
    if(is_int_or_long(self->itext_offset) == 0)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid instruction text offset");
        goto _err;
    }

    /* Have we finished decoding? */
    itext_offset = PyLong_AsLong(self->itext_offset);
    if(itext_offset == PyString_Size(self->itext))
    {
        Py_INCREF(Py_None);
        r = Py_None;
        goto _err;
    }

    /* If the offset given is invalid, raise `InvalidOffsetException'. */
    if(itext_offset < 0 || itext_offset > PyString_Size(self->itext))
    {
        PyErr_SetString(invalid_offset, "Invalid instruction text offset");
        goto _err;
    }

    /* Decode next instruction from the input buffer. */
    decoded_inst = PyMem_Malloc(sizeof(xed_decoded_inst_t));
    xed_decoded_inst_zero(decoded_inst);
    xed_decoded_inst_set_mode(decoded_inst, self->mmode,
        self->stack_addr_width);
    buf = (const xed_uint8_t *)PyString_AsString(self->itext) + itext_offset;

    /* If we set `bytes' to something more than `XED_MAX_INSTRUCTION_BYTES', the
     * call to `xed_decode()' below may fail when decoding certain instructions.
     */
    bytes = PyString_Size(self->itext) - itext_offset;
    if(bytes > XED_MAX_INSTRUCTION_BYTES)
        bytes = XED_MAX_INSTRUCTION_BYTES;
    xed_decode(decoded_inst, buf, bytes);

    /* Compute the instruction's runtime address and update saved offset by the
     * number of bytes corresponding to the decoded instruction (the call to
     * `xed_decoded_inst_get_length()' will return a valid length even if the
     * decoded instruction is not valid).
     */
    runtime_address = PyLong_AsLong(self->runtime_address) + itext_offset;
    itext_offset += xed_decoded_inst_get_length(decoded_inst);
    self->itext_offset = PyLong_FromLong(itext_offset);

    /* If the instruction is not valid, raise `InvalidInstructionError'. */
    if(xed_decoded_inst_valid(decoded_inst) == 0)
    {
        PyMem_Free(decoded_inst);
        PyErr_SetString(invalid_instruction, "Decoded instruction is invalid");
        goto _err;
    }

    r = (PyObject *)new_instruction(decoded_inst, runtime_address);

_err:
    return r;
}


static PyObject *set_mode(xed_t *self, PyObject *args)
{
    unsigned int mmode, stack_addr_width;
    if(PyArg_ParseTuple(args, "II", &mmode, &stack_addr_width) != 0)
    {
        self->mmode = mmode;
        self->stack_addr_width = stack_addr_width;
    }
    Py_RETURN_NONE;
}


static PyMemberDef members[] =
{
    {"itext", T_OBJECT, offsetof(xed_t, itext), 0,
        "String object holding instruction text bytes"},
    {"itext_offset", T_OBJECT, offsetof(xed_t, itext_offset), 0,
        "Offset in instruction text to start decoding from"},
    {"runtime_address", T_OBJECT, offsetof(xed_t, runtime_address), 0,
        "Runtime address of the instruction text being disassembled"},
    {NULL, 0, 0, 0, NULL}
};

static PyMethodDef methods[] =
{
    M_VARARGS(set_mode),
    M_NOARGS(decode),
    M_NULL
};

static PyTypeObject type =
{
    PyObject_HEAD_INIT(NULL)
    .tp_name = "pyxed.XED",
    .tp_basicsize = sizeof(xed_t),
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

