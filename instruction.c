/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * instruction.c - Exports a Python class, called `Instruction', that represents
 * a decoded x86 instruction.
 */
#include "includes.h"
#include "check.h"
#include "instruction.h"
#include "operand.h"



static PyObject *get_noperands(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    return PyInt_FromSize_t(xed_decoded_inst_noperands(decoded_inst));
}

static PyObject *get_category(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    return PyInt_FromLong(xed_decoded_inst_get_category(decoded_inst));
}

static PyObject *get_extension(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    return PyInt_FromLong(xed_decoded_inst_get_extension(decoded_inst));
}

static PyObject *get_iclass(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    return PyInt_FromLong(xed_decoded_inst_get_iclass(decoded_inst));
}

static PyObject *get_iform(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    return PyInt_FromLong(xed_decoded_inst_get_iform_enum(decoded_inst));
}

static PyObject *get_length(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    return PyInt_FromLong(xed_decoded_inst_get_length(decoded_inst));
}

static PyObject *get_attribute(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    PyObject *r = NULL;
    xed_uint32_t is_set;
    int attr;

    if(PyArg_ParseTuple(args, "i", &attr) != 0)
    {
        is_set = xed_decoded_inst_get_attribute(decoded_inst, attr);
        r = (PyObject *)PyBool_FromLong(is_set);
    }
    return r;
}

static PyObject *get_branch_displacement(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    xed_uint_t disp = xed_decoded_inst_get_branch_displacement(decoded_inst);
    return PyInt_FromLong(disp);
}

static PyObject *dump_intel_format(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    PyObject *r = NULL;
    xed_uint64_t runtime_address;
    char tmp1[64], tmp2[96];

    if(is_int_or_long(instruction->runtime_address))
    {
        runtime_address = PyLong_AsLong(instruction->runtime_address);
        xed_decoded_inst_dump_intel_format(decoded_inst, tmp1, sizeof(tmp1) - 1,
            runtime_address);

        if(xed_decoded_inst_get_machine_mode_bits(decoded_inst) == 64)
            snprintf(tmp2, sizeof(tmp2), "%.16llx %s", runtime_address, tmp1);
        else
            snprintf(tmp2, sizeof(tmp2), "%.8lx %s",
                (unsigned long)runtime_address, tmp1);
        r = PyString_FromString(tmp2);
    }
    return r;
}

static PyObject *get_operand(PyObject *self, PyObject *args)
{
    instruction_t *instruction = (instruction_t *)self;
    xed_decoded_inst_t *decoded_inst = instruction->decoded_inst;
    const xed_inst_t *inst = instruction->inst;
    unsigned int i;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) != 0 &&
            i < xed_decoded_inst_noperands(decoded_inst))
        r = (PyObject *)new_operand(xed_inst_operand(inst, i));
    return r;
}


static PyMemberDef members[] =
{
    {"runtime_address", T_OBJECT, offsetof(instruction_t, runtime_address), 0,
        "Runtime address of this instruction"},
    {NULL}
};

static PyMethodDef methods[] =
{
    {"get_noperands", get_noperands, METH_NOARGS,
        "Equivalent to `xed_decoded_inst_noperands()'"},
    {"get_category", get_category, METH_NOARGS,
        "Equivalent to `xed_decoded_inst_get_category()'"},
    {"get_extension", get_extension, METH_NOARGS,
        "Equivalent to `xed_decoded_inst_get_extension()'"},
    {"get_iclass", get_iclass, METH_NOARGS,
        "Equivalent to `xed_decoded_inst_get_iclass()'"},
    {"get_iform", get_iform, METH_NOARGS,
        "Equivalent to `xed_decoded_inst_get_iform()'"},
    {"get_length", get_length, METH_NOARGS,
        "Equivalent to `xed_decoded_inst_get_length()'"},
    {"get_attribute", get_attribute, METH_VARARGS,
        "Equivalent to `xed_decoded_inst_get_attribute()'"},
    {"get_branch_displacement", get_branch_displacement, METH_NOARGS,
        "Equivalent to `xed_decoded_inst_get_branch_displacement'"},
    {"dump_intel_format", dump_intel_format, METH_NOARGS,
        "Equivalent to `xed_decoded_inst_dump_intel_format()'"},
    {"get_operand", get_operand, METH_VARARGS,
        "Equivalent to `xed_inst_operand()'"},
    {NULL}
};

static PyTypeObject type =
{
    PyObject_HEAD_INIT(NULL)
    .tp_name = "pyxed.Instruction",
    .tp_basicsize = sizeof(instruction_t),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Represents a decoded instruction",
    .tp_methods = methods,
    .tp_members = members
};


/* Allocate and initialize a new `Instruction' object given the associated
 * `xed_decoded_inst_t' structure.
 */
instruction_t *new_instruction(xed_decoded_inst_t *decoded_inst,
        xed_uint64_t runtime_address)
{
    instruction_t *instruction =
        (instruction_t *)PyType_GenericNew(&type, NULL, NULL);
    instruction->decoded_inst = decoded_inst;
    instruction->inst = xed_decoded_inst_inst(decoded_inst);
    instruction->runtime_address = PyLong_FromLong(runtime_address);
    return instruction;
}


void register_instruction_object(PyObject *module)
{
    if(PyType_Ready(&type) == 0)
    {
        Py_INCREF(&type);
        PyModule_AddObject(module, "Instruction", (PyObject *)&type);
    }
}

