/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * operand.c - Exports a Python class, called `Operand', that represents a x86
 * instruction operand.
 */
#include "includes.h"
#include "operand.h"


static PyObject *get_imm(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    return PyInt_FromLong(xed_operand_imm(operand->operand));
}

static PyObject *get_name(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    return PyInt_FromLong(xed_operand_name(operand->operand));
}

static PyObject *get_visibility(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    return PyInt_FromLong(xed_operand_operand_visibility(operand->operand));
}

static PyObject *get_type(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    return PyInt_FromLong(xed_operand_type(operand->operand));
}

static PyObject *get_xtype(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    return PyInt_FromLong(xed_operand_xtype(operand->operand));
}

static PyObject *get_width(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    return PyInt_FromLong(xed_operand_width(operand->operand));
}

static PyObject *get_width_bits(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    xed_uint32_t eosz;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &eosz) != 0)
    {
        r = PyInt_FromSize_t(xed_operand_width_bits(operand->operand, eosz));
    }
    return r;
}

static PyObject *dump(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    char buf[64];
    xed_operand_print(operand->operand, buf, sizeof(buf) - 1);
    return PyString_FromString(buf);
}


static PyObject *is_memory_addressing_register(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    return PyInt_FromSize_t(xed_operand_is_memory_addressing_register(
        xed_operand_name(operand->operand)));
}

static PyObject *is_register(PyObject *self, PyObject *args)
{
    operand_t *operand = (operand_t *)self;
    return PyInt_FromSize_t(xed_operand_is_register(xed_operand_name(
        operand->operand)));
}


static PyMethodDef methods[] =
{
    {"get_imm", get_imm, METH_NOARGS, "Equivalent to `xed_operand_imm()'"},
    {"get_name", get_name, METH_NOARGS, "Equivalent to `xed_operand_name()'"},
    {"get_visibility", get_visibility, METH_NOARGS,
        "Equivalent to `xed_operand_operand_visibility()'"},
    {"get_type", get_type, METH_NOARGS, "Equivalent to `xed_operand_type()'"},
    {"get_xtype", get_xtype, METH_NOARGS,
        "Equivalent to `xed_operand_xtype()'"},
    {"get_width", get_width, METH_NOARGS,
        "Equivalent to `xed_operand_get_width()'"},
    {"get_width_bits", get_width_bits, METH_VARARGS,
        "Equivalent to `xed_operand_get_width_bits()'"},
    /* There were two reasons we decided _not_ to name this function `print()'.
     * First, `print' is a reserved word in Python. Second, `dump()' is more
     * uniform with respect to some method names in class `Instruction' (e.g.
     * `dump_intel_format()').
     */
    {"dump", dump, METH_NOARGS, "Equivalent to `xed_operand_print()'"},
    {"is_memory_addressing_register", is_memory_addressing_register,
        METH_NOARGS,
        "Equivalent to `xed_operand_is_memory_addressing_register()'"},
    {"is_register", is_register, METH_NOARGS,
        "Equivalent to `xed_operand_is_register()'"},
    {NULL}
};

static PyTypeObject type =
{
    PyObject_HEAD_INIT(NULL)
    .tp_name = "pyxed.Operand",
    .tp_basicsize = sizeof(operand_t),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Represents an instruction operand",
    .tp_methods = methods,
    .tp_new = PyType_GenericNew
};


operand_t *new_operand(const xed_operand_t *operand)
{
    PyObject *type_object = (PyObject *)&type;
    operand_t *op = (operand_t *)PyObject_CallObject(type_object, NULL);
    op->operand = operand;
    return op;
}


void register_operand_object(PyObject *module)
{
    if(PyType_Ready(&type) == 0)
    {
        Py_INCREF(&type);
        PyModule_AddObject(module, "Operand", (PyObject *)&type);
    }
}

