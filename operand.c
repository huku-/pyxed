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
    const xed_operand_t *operand = m_operand(self);
    return PyInt_FromLong(xed_operand_imm(operand));
}

static PyObject *get_name(PyObject *self, PyObject *args)
{
    const xed_operand_t *operand = m_operand(self);
    return PyInt_FromLong(xed_operand_name(operand));
}

static PyObject *get_visibility(PyObject *self, PyObject *args)
{
    const xed_operand_t *operand = m_operand(self);
    return PyInt_FromLong(xed_operand_operand_visibility(operand));
}

static PyObject *get_type(PyObject *self, PyObject *args)
{
    const xed_operand_t *operand = m_operand(self);
    return PyInt_FromLong(xed_operand_type(operand));
}

static PyObject *get_xtype(PyObject *self, PyObject *args)
{
    const xed_operand_t *operand = m_operand(self);
    return PyInt_FromLong(xed_operand_xtype(operand));
}

static PyObject *get_width(PyObject *self, PyObject *args)
{
    const xed_operand_t *operand = m_operand(self);
    return PyInt_FromLong(xed_operand_width(operand));
}

static PyObject *get_width_bits(PyObject *self, PyObject *args)
{
    const xed_operand_t *operand;
    xed_uint32_t eosz;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &eosz) != 0)
    {
        operand = m_operand(self);
        r = PyInt_FromSize_t(xed_operand_width_bits(operand, eosz));
    }
    return r;
}

static PyObject *dump(PyObject *self, PyObject *args)
{
    char buf[64];
    xed_operand_print(m_operand(self), buf, sizeof(buf) - 1);
    return PyString_FromString(buf);
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
    {NULL}
};

static PyTypeObject type =
{
    PyObject_HEAD_INIT(NULL)
    .tp_name = "pyxed.Operand",
    .tp_basicsize = sizeof(Operand),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Represents an instruction operand",
    .tp_methods = methods,
    .tp_new = PyType_GenericNew
};


Operand *new_operand(const xed_operand_t *operand)
{
    PyObject *type_object = (PyObject *)&type;
    Operand *op = (Operand *)PyObject_CallObject(type_object, NULL);
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

