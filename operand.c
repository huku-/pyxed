/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * operand.c - Exports a Python class, called `Operand', that represents a x86
 * instruction operand.
 */
#include "includes.h"
#include "operand.h"


/* Operand information related methods. */

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

static PyObject *is_memory_addressing_register(operand_t *self, PyObject *args)
{
    return PyBool_FromLong(xed_operand_is_memory_addressing_register(
        xed_operand_name(self->operand)));
}

static PyObject *is_register(operand_t *self, PyObject *args)
{
    return PyBool_FromLong(xed_operand_is_register(xed_operand_name(
        self->operand)));
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


/* Read/write information related methods. */

static PyObject *is_conditional_read(operand_t *self, PyObject *args)
{
    return PyBool_FromLong(xed_operand_conditional_read(self->operand));
}

static PyObject *is_conditional_write(operand_t *self, PyObject *args)
{
    return PyBool_FromLong(xed_operand_conditional_write(self->operand));
}

static PyObject *is_read(operand_t *self, PyObject *args)
{
    return PyBool_FromLong(xed_operand_read(self->operand));
}

static PyObject *is_read_and_written(operand_t *self, PyObject *args)
{
    return PyBool_FromLong(xed_operand_read_and_written(self->operand));
}

static PyObject *is_read_only(operand_t *self, PyObject *args)
{
    return PyBool_FromLong(xed_operand_read_only(self->operand));
}

static PyObject *is_written(operand_t *self, PyObject *args)
{
    return PyBool_FromLong(xed_operand_written(self->operand));
}

static PyObject *is_written_only(operand_t *self, PyObject *args)
{
    return PyBool_FromLong(xed_operand_written_only(self->operand));
}

/* The original name is `xed_operand_rw()' but `get_rw_action()' is far more
 * descriptive and intuitive.
 */
static PyObject *get_rw_action(operand_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_operand_rw(self->operand));
}



static PyMethodDef methods[] =
{
    /* Operand information related methods. */
    M_NOARGS(get_imm),
    M_NOARGS(get_name),
    M_NOARGS(get_visibility),
    M_NOARGS(get_type),
    M_NOARGS(get_xtype),
    M_NOARGS(get_width),
    M_VARARGS(get_width_bits),
    M_NOARGS(is_memory_addressing_register),
    M_NOARGS(is_register),
    M_NOARGS(dump),

    /* Read/write information related methods. */
    M_NOARGS(is_conditional_read),
    M_NOARGS(is_conditional_write),
    M_NOARGS(is_read),
    M_NOARGS(is_read_and_written),
    M_NOARGS(is_read_only),
    M_NOARGS(is_written),
    M_NOARGS(is_written_only),
    M_NOARGS(get_rw_action),

    M_NULL
};


/* See comment in "decoder.c" for more information. */
static PyObject type_base =
{
    PyObject_HEAD_INIT(NULL)
};

static PyTypeObject type;


/* Allocate and initialize a new `Operand' object given the associated
 * `xed_operand_t' structure.
 */
operand_t *new_operand(const xed_operand_t *operand)
{
    operand_t *op = (operand_t *)PyType_GenericNew(&type, NULL, NULL);
    op->operand = operand;
    return op;
}


/* Initialization of `pyxed.Operand' type should go here. */
static void initialize_operand_type(PyTypeObject *type)
{
    *(PyObject *)type = type_base;
    type->tp_name = "pyxed.Operand";
    type->tp_basicsize = sizeof(operand_t);
    type->tp_flags = Py_TPFLAGS_DEFAULT;
    type->tp_doc = "Represents an instruction operand";
    type->tp_methods = methods;
}

void register_operand_object(PyObject *module)
{
    initialize_operand_type(&type);
    if(PyType_Ready(&type) == 0)
    {
        Py_INCREF(&type);
        PyModule_AddObject(module, "Operand", (PyObject *)&type);
    }
}

