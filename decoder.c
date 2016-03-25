/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * decoder.c - Exports class `Decoder', the main decoder object.
 */
#include "includes.h"
#include "pyxed.h"
#include "check.h"
#include "decoder.h"
#include "instruction.h"



static int init(PyObject *self, PyObject *args, PyObject *kwds)
{
    /* The call to `tp_alloc()' will initialize memory with zeros. */
    xed_t *xed = (xed_t *)self;
    xed->mmode = XED_MACHINE_MODE_LONG_COMPAT_32;
    xed->stack_addr_width = XED_ADDRESS_WIDTH_32b;
    return 0;
}


static PyObject *decode(xed_t *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst;
    unsigned long long runtime_address, itext_offset, itext_size;
    const xed_uint8_t *buf;
    unsigned int bytes;
    PyObject *r = NULL;

    /* Make sure we have a valid input buffer. */
    if(is_string(self->itext) == 0)
    {
        PyErr_SetString(PyExc_TypeError, "Invalid instruction text");
        goto _err;
    }

    itext_offset = self->itext_offset;
    itext_size = (unsigned long long)PyString_Size(self->itext);

    /* Have we finished decoding? */
    if(itext_offset == itext_size)
    {
        Py_INCREF(Py_None);
        r = Py_None;
        goto _err;
    }

    /* If the offset given is invalid, raise `InvalidOffsetException'. */
    if(itext_offset > itext_size)
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
    if(itext_size - itext_offset <= XED_MAX_INSTRUCTION_BYTES)
        bytes = (unsigned int)(itext_size - itext_offset);
    else
        bytes = XED_MAX_INSTRUCTION_BYTES;

    /* XXX: Check return value here? */
    xed_decode(decoded_inst, buf, bytes);

    /* Compute the instruction's runtime address and update saved offset by the
     * number of bytes corresponding to the decoded instruction (the call to
     * `xed_decoded_inst_get_length()' will return a valid length even if the
     * decoded instruction is not valid).
     */
    runtime_address = self->runtime_address + itext_offset;
    self->itext_offset += xed_decoded_inst_get_length(decoded_inst);

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
    {"itext_offset", T_ULONGLONG, offsetof(xed_t, itext_offset), 0,
        "Offset in instruction text to start decoding from"},
    {"runtime_address", T_ULONGLONG, offsetof(xed_t, runtime_address), 0,
        "Runtime address of the instruction text being disassembled"},
    {NULL, 0, 0, 0, NULL}
};

static PyMethodDef methods[] =
{
    M_VARARGS(set_mode),
    M_NOARGS(decode),
    M_NULL
};


/* When compiling on Microsoft Windows with older versions of Visual Studio, we
 * can't use designated initializers for structure types; this C99 feature is
 * not supported. To properly initialize a `PyTypeObject' and avoid compiler
 * warnings, we first need to initialize a `PyObject' (fully initialized by
 * `PyObject_HEAD_INIT()') and then assign the latter to the first. Have a look
 * at `initialize_decoder_type()' below for more information.
 */
static PyObject type_base =
{
    PyObject_HEAD_INIT(NULL)
};

static PyTypeObject type;


/* Initialization of `pyxed.Decoder' type should go here. */
static void initialize_decoder_type(PyTypeObject *type)
{
    /* All Python structures are castable to `PyObject' and structure assignment
     * is a well defined construct.
     */
    *(PyObject *)type = type_base;
    type->tp_name = "pyxed.Decoder";
    type->tp_basicsize = sizeof(xed_t);
    type->tp_flags = Py_TPFLAGS_DEFAULT;
    type->tp_doc = "Main decoder object";
    type->tp_methods = methods;
    type->tp_members = members;
    type->tp_init = init;
    type->tp_new = PyType_GenericNew;
}

void register_decoder_object(PyObject *module)
{
    initialize_decoder_type(&type);
    if(PyType_Ready(&type) == 0)
    {
        Py_INCREF(&type);
        PyModule_AddObject(module, "Decoder", (PyObject *)&type);
    }
}

