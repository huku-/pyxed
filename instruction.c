#include "includes.h"
#include "instruction.h"


static PyObject *dump_intel_format(PyObject *self, PyObject *args)
{
    char tmp1[64], tmp2[96];
    Instruction *inst = (Instruction *)self;
    xed_decoded_inst_dump_intel_format(inst->decoded_inst, tmp1,
        sizeof(tmp1) - 1, inst->runtime_address);

    if(xed_decoded_inst_get_machine_mode_bits(inst->decoded_inst) == 64)
        snprintf(tmp2, sizeof(tmp2), "%.8lx`%.8lx %s",
            LUINT((inst->runtime_address & 0xffffffff00000000) >> 32),
            LUINT((inst->runtime_address & 0x00000000ffffffff)), tmp1);
    else
        snprintf(tmp2, sizeof(tmp2), "%.8lx %s", LUINT(inst->runtime_address),
            tmp1);
    return PyString_FromString(tmp2);
}


static PyMethodDef methods[] =
{
    {"dump_intel_format", dump_intel_format, METH_NOARGS,
        "Equivalent to `xed_decoded_inst_dump_intel_format()'"},
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

