/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * instruction.c - Exports a Python class, called `Instruction', that represents
 * a decoded x86 instruction.
 */
#include "includes.h"
#include "instruction.h"
#include "operand.h"


static PyObject *get_noperands(PyObject *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst = m_decoded_inst(self);
    return PyInt_FromSize_t(xed_decoded_inst_noperands(decoded_inst));
}

static PyObject *get_category(PyObject *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst = m_decoded_inst(self);
    return PyInt_FromLong(xed_decoded_inst_get_category(decoded_inst));
}

static PyObject *get_extension(PyObject *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst = m_decoded_inst(self);
    return PyInt_FromLong(xed_decoded_inst_get_extension(decoded_inst));
}

static PyObject *get_iclass(PyObject *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst = m_decoded_inst(self);
    return PyInt_FromLong(xed_decoded_inst_get_iclass(decoded_inst));
}

static PyObject *get_iform(PyObject *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst = m_decoded_inst(self);
    return PyInt_FromLong(xed_decoded_inst_get_iform_enum(decoded_inst));
}

static PyObject *dump_intel_format(PyObject *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst = m_decoded_inst(self);
    xed_uint64_t runtime_address = m_runtime_address(self);
    char tmp1[64], tmp2[96];

    xed_decoded_inst_dump_intel_format(decoded_inst, tmp1, sizeof(tmp1) - 1,
        runtime_address);

    if(xed_decoded_inst_get_machine_mode_bits(decoded_inst) == 64)
        snprintf(tmp2, sizeof(tmp2), "%.16llx %s", runtime_address, tmp1);
    else
        snprintf(tmp2, sizeof(tmp2), "%.8lx %s", (unsigned long)runtime_address,
            tmp1);
    return PyString_FromString(tmp2);
}

static PyObject *get_operand(PyObject *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst = m_decoded_inst(self);
    const xed_inst_t *inst = m_inst(self);
    unsigned int i;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) != 0 &&
            i < xed_decoded_inst_noperands(decoded_inst))
        r = (PyObject *)new_operand(xed_inst_operand(inst, i));
    return r;
}


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
    .tp_basicsize = sizeof(Instruction),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Represents a decoded instruction",
    .tp_methods = methods,
    .tp_new = PyType_GenericNew
};


Instruction *new_instruction(xed_decoded_inst_t *decoded_inst,
        xed_uint64_t runtime_address)
{
    PyObject *type_object = (PyObject *)&type;
    Instruction *inst = (Instruction *)PyObject_CallObject(type_object, NULL);
    inst->decoded_inst = decoded_inst;
    inst->inst = xed_decoded_inst_inst(decoded_inst);
    inst->runtime_address = runtime_address;
    return inst;
}


void register_instruction_object(PyObject *module)
{
    if(PyType_Ready(&type) == 0)
    {
        Py_INCREF(&type);
        PyModule_AddObject(module, "Instruction", (PyObject *)&type);
    }
}

