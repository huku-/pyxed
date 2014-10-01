/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * operand.c - Exports a Python class, called `Operand', that represents a x86
 * instruction operand.
 */
#include "includes.h"
#include "operand.h"


static PyObject *get_imm(operand_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_operand_imm(self->operand));
}

static PyObject *get_name(operand_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_operand_name(self->operand));
}

static PyObject *get_visibility(operand_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_operand_operand_visibility(self->operand));
}

static PyObject *get_type(operand_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_operand_type(self->operand));
}

static PyObject *get_xtype(operand_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_operand_xtype(self->operand));
}

static PyObject *get_width(operand_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_operand_width(self->operand));
}

static PyObject *get_width_bits(operand_t *self, PyObject *args)
{
    xed_uint32_t eosz;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &eosz) != 0)
    {
        r = PyInt_FromSize_t(xed_operand_width_bits(self->operand, eosz));
    }
    return r;
}


/* There were two reasons that we decided _not_ to name the following function
 * `print()'. First, `print' is a reserved word in Python. Second, `dump()' is
 * more uniform with respect to some method names in class `Instruction' (e.g.
 * `dump_intel_format()').
 */
static PyObject *dump(operand_t *self, PyObject *args)
{
    char buf[64];
    xed_operand_print(self->operand, buf, sizeof(buf) - 1);
    return PyString_FromString(buf);
}


static PyObject *is_memory_addressing_register(operand_t *self, PyObject *args)
{
    return PyInt_FromSize_t(xed_operand_is_memory_addressing_register(
        xed_operand_name(self->operand)));
}

static PyObject *is_register(operand_t *self, PyObject *args)
{
    return PyInt_FromSize_t(xed_operand_is_register(xed_operand_name(
        self->operand)));
}


static PyMethodDef methods[] =
{
    M_NOARGS(get_imm),
    M_NOARGS(get_name),
    M_NOARGS(get_visibility),
    M_NOARGS(get_type),
    M_NOARGS(get_xtype),
    M_NOARGS(get_width),
    M_VARARGS(get_width_bits),
    M_NOARGS(dump),
    M_NOARGS(is_memory_addressing_register),
    M_NOARGS(is_register),
    {NULL}
};

static PyTypeObject type =
{
    PyObject_HEAD_INIT(NULL)
    .tp_name = "pyxed.Operand",
    .tp_basicsize = sizeof(operand_t),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Represents an instruction operand",
    .tp_methods = methods
};


/* Allocate and initialize a new `Operand' object given the associated
 * `xed_operand_t' structure.
 */
operand_t *new_operand(const xed_operand_t *operand)
{
    operand_t *op = (operand_t *)PyType_GenericNew(&type, NULL, NULL);
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

