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
#include "rflags.h"


/* Instruction information related methods. */

static PyObject *get_isa_set(instruction_t *self)
{
    return PyInt_FromLong(xed_decoded_inst_get_isa_set(self->decoded_inst));
}

static PyObject *get_isa_set_str(instruction_t *self)
{
    return PyUnicode_FromString(xed_isa_set_enum_t2str(xed_decoded_inst_get_isa_set(self->decoded_inst)));
}

static PyObject *get_category(instruction_t *self)
{
    return PyInt_FromLong(xed_decoded_inst_get_category(self->decoded_inst));
}

static PyObject *get_category_str(instruction_t *self)
{
    return PyUnicode_FromString(xed_category_enum_t2str(xed_decoded_inst_get_category(self->decoded_inst)));
}

static PyObject *get_extension(instruction_t *self)
{
    return PyInt_FromLong(xed_decoded_inst_get_extension(self->decoded_inst));
}

static PyObject *get_extension_str(instruction_t *self)
{
    return PyUnicode_FromString(xed_extension_enum_t2str(xed_decoded_inst_get_extension(self->decoded_inst)));
}

static PyObject *get_iclass(instruction_t *self)
{
    return PyInt_FromLong(xed_decoded_inst_get_iclass(self->decoded_inst));
}

static PyObject *get_iclass_str(instruction_t *self)
{
    return PyUnicode_FromString(xed_iclass_enum_t2str(xed_decoded_inst_get_iclass(self->decoded_inst)));
}

static PyObject *get_iform(instruction_t *self)
{
    return PyInt_FromLong(xed_decoded_inst_get_iform_enum(self->decoded_inst));
}

static PyObject *get_iform_str(instruction_t *self)
{
    return PyUnicode_FromString(xed_iform_enum_t2str(xed_decoded_inst_get_iform_enum(self->decoded_inst)));
}

static PyObject *get_attribute(instruction_t *self, PyObject *args)
{
    int i;
    xed_uint32_t flag;

    PyObject *r = NULL;

    /* Treat enumerations as `int' values (see comment in "pyxed.c"). */
    if(PyArg_ParseTuple(args, "i", &i) == 0)
        goto _err;

    if(i <= XED_ATTRIBUTE_INVALID || i >= XED_ATTRIBUTE_LAST)
    {
        PyErr_SetString(PyExc_ValueError, "Invalid attribute");
        goto _err;
    }

    flag = xed_decoded_inst_get_attribute(self->decoded_inst, i);
    r = PyBool_FromLong(flag);

_err:
    return r;
}

static PyObject *get_length(instruction_t *self)
{
    return PyInt_FromLong(xed_decoded_inst_get_length(self->decoded_inst));
}

static PyObject *get_modrm(instruction_t *self)
{
    xed_uint8_t modrm;
    modrm = xed_decoded_inst_get_modrm(self->decoded_inst);
    return PyLong_FromUnsignedLong(modrm);
}

static PyObject *get_nprefixes(instruction_t *self)
{
    xed_uint_t nprefixes;
    nprefixes = xed_decoded_inst_get_nprefixes(self->decoded_inst);
    return PyLong_FromUnsignedLong(nprefixes);
}

static PyObject *conditionally_writes_registers(instruction_t *self)
{
    xed_bool_t flag;
    flag = xed_decoded_inst_conditionally_writes_registers(self->decoded_inst);
    return PyBool_FromLong((long)flag);
}

static PyObject *dump_intel_format(instruction_t *self, PyObject *args, PyObject *kwargs)
{
    char *kwlist[] = {"address", NULL};
    int print_address = 0;
    char buf[64];

    PyObject *r = NULL;

    if(PyArg_ParseTupleAndKeywords(args, kwargs, "|$p", kwlist, &print_address) == 0)
        goto _err;

    /* In Pin versions before "pin-2.14-*", `xed_format_context()' takes 6
     * parameters instead of 7. Unfortunately, there's no consistent way of
     * dumping instructions between various XED versions and there's no easy
     * way of detecting the actual XED version at compile time.
     */
    xed_format_context(XED_SYNTAX_INTEL, self->decoded_inst, buf, sizeof(buf) - 1,
        self->runtime_address, NULL, NULL);

    if(print_address)
        r = PyUnicode_FromFormat("%p %s", (void *)self->runtime_address, buf);
    else
        r = PyUnicode_FromString(buf);

_err:
    return r;
}


/* EFLAGS/RFLAGS information related methods. */

static PyObject *uses_rflags(instruction_t *self)
{
    return PyBool_FromLong(xed_decoded_inst_uses_rflags(self->decoded_inst));
}

static PyObject *get_rflags_read(instruction_t *self)
{
    const xed_simple_flag_t *flags;
    PyObject *r = Py_None;

    if((flags = xed_decoded_inst_get_rflags_info(self->decoded_inst)) != NULL)
        r = (PyObject *)new_rflags(&flags->read);
    else
        Py_INCREF(r);
    return r;
}

static PyObject *get_rflags_undefined(instruction_t *self)
{
    const xed_simple_flag_t *flags;
    PyObject *r = Py_None;

    if((flags = xed_decoded_inst_get_rflags_info(self->decoded_inst)) != NULL)
        r = (PyObject *)new_rflags(&flags->undefined);
    else
        Py_INCREF(r);
    return r;
}

static PyObject *get_rflags_written(instruction_t *self)
{
    const xed_simple_flag_t *flags;
    PyObject *r = Py_None;

    if((flags = xed_decoded_inst_get_rflags_info(self->decoded_inst)) != NULL)
        r = (PyObject *)new_rflags(&flags->written);
    else
        Py_INCREF(r);
    return r;
}


/* Branch displacement related methods (getters & setters). */

static PyObject *get_branch_displacement(instruction_t *self)
{
    xed_uint_t disp;
    disp = xed_decoded_inst_get_branch_displacement(self->decoded_inst);
    return PyLong_FromUnsignedLong(disp);
}

static PyObject *get_branch_displacement_width(instruction_t *self)
{
    xed_uint_t width;
    width = xed_decoded_inst_get_branch_displacement_width(self->decoded_inst);
    return PyLong_FromUnsignedLong(width);
}

static PyObject *get_branch_displacement_width_bits(instruction_t *self,
        PyObject *args)
{
    xed_uint_t width;
    width = xed_decoded_inst_get_branch_displacement_width_bits(self->decoded_inst);
    return PyLong_FromUnsignedLong(width);
}

static PyObject *set_branch_displacement(instruction_t *self, PyObject *args)
{
    xed_int32_t disp;
    xed_uint_t length;

    if(PyArg_ParseTuple(args, "iI", &disp, &length) == 0)
        goto _err;

    xed_decoded_inst_set_branch_displacement(self->decoded_inst, disp, length);

_err:
    Py_RETURN_NONE;
}

static PyObject *set_branch_displacement_bits(instruction_t *self, PyObject *args)
{
    xed_int32_t disp;
    xed_uint_t length;

    if(PyArg_ParseTuple(args, "iI", &disp, &length) == 0)
        goto _err;

    xed_decoded_inst_set_branch_displacement_bits(self->decoded_inst, disp, length);

_err:
    Py_RETURN_NONE;
}


/* Memory displacement related methods (getters & setters). */

static PyObject *get_memory_displacement(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_int64_t disp;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    disp = xed_decoded_inst_get_memory_displacement(decoded_inst, i);
    r = PyLong_FromLongLong(disp);

_err:
    return r;
}

static PyObject *get_memory_displacement_width(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_uint_t width;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    width = xed_decoded_inst_get_memory_displacement_width(decoded_inst, i);
    r = PyLong_FromUnsignedLong(width);

_err:
    return r;
}

static PyObject *get_memory_displacement_width_bits(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_uint_t width;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    width = xed_decoded_inst_get_memory_displacement_width_bits(decoded_inst, i);
    r = PyLong_FromUnsignedLong(width);

_err:
    return r;
}

static PyObject *set_memory_displacement(instruction_t *self, PyObject *args)
{
    xed_int64_t disp;
    xed_uint_t length;

    if(PyArg_ParseTuple(args, "LI", &disp, &length) == 0)
        goto _err;

    xed_decoded_inst_set_memory_displacement(self->decoded_inst, disp, length);

_err:
    Py_RETURN_NONE;
}

static PyObject *set_memory_displacement_bits(instruction_t *self, PyObject *args)
{
    xed_int64_t disp;
    xed_uint_t length;

    if(PyArg_ParseTuple(args, "LI", &disp, &length) == 0)
        goto _err;

    xed_decoded_inst_set_memory_displacement_bits(self->decoded_inst, disp, length);

_err:
    Py_RETURN_NONE;
}


/* Operand information related methods. */

static PyObject *get_noperands(instruction_t *self, PyObject *args)
{
    return PyInt_FromSize_t(xed_decoded_inst_noperands(self->decoded_inst));
}

static PyObject *get_operand(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_noperands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    r = (PyObject *)new_operand(xed_inst_operand(self->inst, i));

_err:
    return r;
}

static PyObject *get_operand_length(instruction_t *self, PyObject *args)
{
    unsigned int i, length;
    xed_decoded_inst_t *decoded_inst;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_noperands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    length = xed_decoded_inst_operand_length(decoded_inst, i);
    r = PyLong_FromUnsignedLong(length);

_err:
    return r;
}

static PyObject *get_operand_length_bits(instruction_t *self, PyObject *args)
{
    unsigned int i, length;
    xed_decoded_inst_t *decoded_inst;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_noperands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    length = xed_decoded_inst_operand_length_bits(decoded_inst, i);
    r = PyLong_FromUnsignedLong(length);

_err:
    return r;
}

static PyObject *get_reg(instruction_t *self, PyObject *args)
{
    int i;
    xed_decoded_inst_t *decoded_inst;
    xed_reg_enum_t reg;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "i", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i <= XED_OPERAND_INVALID || i >= XED_OPERAND_LAST)
    {
        PyErr_SetString(PyExc_ValueError, "Invalid register");
        goto _err;
    }

    reg = xed_decoded_inst_get_reg(decoded_inst, i);
    r = PyInt_FromLong(reg);

_err:
    return r;
}


/* Immediate information related methods. */

static PyObject *is_immediate_signed(instruction_t *self)
{
    xed_uint_t flag;
    flag = xed_decoded_inst_get_immediate_is_signed(self->decoded_inst);
    return PyBool_FromLong(flag);
}

static PyObject *get_immediate_width(instruction_t *self)
{
    xed_uint_t width;
    width = xed_decoded_inst_get_immediate_width(self->decoded_inst);
    return PyLong_FromUnsignedLong(width);
}

static PyObject *get_immediate_width_bits(instruction_t *self)
{
    xed_uint_t width;
    width = xed_decoded_inst_get_immediate_width_bits(self->decoded_inst);
    return PyLong_FromUnsignedLong(width);
}

static PyObject *get_second_immediate(instruction_t *self)
{
    xed_uint8_t imm;
    imm = xed_decoded_inst_get_second_immediate(self->decoded_inst);
    return PyLong_FromUnsignedLong(imm);
}

static PyObject *get_signed_immediate(instruction_t *self)
{
    xed_int32_t imm;
    imm = xed_decoded_inst_get_signed_immediate(self->decoded_inst);
    return PyLong_FromLong(imm);
}

static PyObject *get_unsigned_immediate(instruction_t *self)
{
    xed_uint64_t imm;
    imm = xed_decoded_inst_get_unsigned_immediate(self->decoded_inst);
    return PyLong_FromUnsignedLongLong(imm);
}


/* Memory operand related methods. */

static PyObject *get_number_of_memory_operands(instruction_t *self)
{
    xed_uint_t num;
    num = xed_decoded_inst_number_of_memory_operands(self->decoded_inst);
    return PyLong_FromUnsignedLong(num);
}

static PyObject *get_seg_reg(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_reg_enum_t reg;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    reg = xed_decoded_inst_get_seg_reg(decoded_inst, i);
    r = PyInt_FromLong(reg);

_err:
    return r;
}

static PyObject *get_base_reg(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_reg_enum_t reg;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    reg = xed_decoded_inst_get_base_reg(decoded_inst, i);
    r = PyInt_FromLong(reg);

_err:
    return r;
}

static PyObject *get_index_reg(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_reg_enum_t reg;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    reg = xed_decoded_inst_get_index_reg(decoded_inst, i);
    r = PyInt_FromLong(reg);

_err:
    return r;
}

static PyObject *get_memory_operand_length(instruction_t *self, PyObject *args)
{
    unsigned int i, length;
    xed_decoded_inst_t *decoded_inst;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    length = xed_decoded_inst_get_memory_operand_length(decoded_inst, i);
    r = PyLong_FromUnsignedLong(length);

_err:
    return r;
}

static PyObject *is_mem_read(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_bool_t flag;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    flag = xed_decoded_inst_mem_read(decoded_inst, i);
    r = PyBool_FromLong(flag);

_err:
    return r;
}

static PyObject *is_mem_written(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_bool_t flag;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    flag = xed_decoded_inst_mem_written(decoded_inst, i);
    r = PyBool_FromLong(flag);

_err:
    return r;
}

static PyObject *is_mem_written_only(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_bool_t flag;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    flag = xed_decoded_inst_mem_written_only(decoded_inst, i);
    r = PyBool_FromLong(flag);

_err:
    return r;
}

static PyObject *get_scale(instruction_t *self, PyObject *args)
{
    unsigned int i;
    xed_decoded_inst_t *decoded_inst;
    xed_uint_t scale;

    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) == 0)
        goto _err;

    decoded_inst = self->decoded_inst;
    if(i >= xed_decoded_inst_number_of_memory_operands(decoded_inst))
    {
        PyErr_SetString(PyExc_IndexError, "Invalid operand index");
        goto _err;
    }

    scale = xed_decoded_inst_get_scale(decoded_inst, i);
    r = PyLong_FromUnsignedLong(scale);

_err:
    return r;
}


static PyMemberDef members[] =
{
    {"runtime_address", T_ULONGLONG, offsetof(instruction_t, runtime_address),
        0, "Runtime address of this instruction"},
    {NULL, 0, 0, 0, NULL}
};

static PyMethodDef methods[] =
{
    /* Instruction information related methods. */
    M_NOARGS(get_isa_set),
    M_NOARGS(get_isa_set_str),
    M_NOARGS(get_category),
    M_NOARGS(get_category_str),
    M_NOARGS(get_extension),
    M_NOARGS(get_extension_str),
    M_NOARGS(get_iclass),
    M_NOARGS(get_iclass_str),
    M_NOARGS(get_iform),
    M_NOARGS(get_iform_str),
    M_VARARGS(get_attribute),
    M_NOARGS(get_length),
    M_NOARGS(get_modrm),
    M_NOARGS(get_nprefixes),
    M_NOARGS(conditionally_writes_registers),
    M_KWARGS(dump_intel_format),

    /* EFLAGS/RFLAGS information related methods. */
    M_NOARGS(uses_rflags),
    M_NOARGS(get_rflags_read),
    M_NOARGS(get_rflags_undefined),
    M_NOARGS(get_rflags_written),

    /* Branch displacement related methods (getters & setters). */
    M_NOARGS(get_branch_displacement),
    M_NOARGS(get_branch_displacement_width),
    M_NOARGS(get_branch_displacement_width_bits),
    M_VARARGS(set_branch_displacement),
    M_VARARGS(set_branch_displacement_bits),

    /* Memory displacement related methods (getters & setters). */
    M_VARARGS(get_memory_displacement),
    M_VARARGS(get_memory_displacement_width),
    M_VARARGS(get_memory_displacement_width_bits),
    M_VARARGS(set_memory_displacement),
    M_VARARGS(set_memory_displacement_bits),

    /* Operand information related methods. */
    M_NOARGS(get_noperands),
    M_VARARGS(get_operand),
    M_VARARGS(get_operand_length),
    M_VARARGS(get_operand_length_bits),
    M_VARARGS(get_reg),

    /* Immediate information related methods. */
    M_NOARGS(is_immediate_signed),
    M_NOARGS(get_immediate_width),
    M_NOARGS(get_immediate_width_bits),
    M_NOARGS(get_second_immediate),
    M_NOARGS(get_signed_immediate),
    M_NOARGS(get_unsigned_immediate),

    /* Memory operand related methods. */
    M_NOARGS(get_number_of_memory_operands),
    M_VARARGS(get_seg_reg),
    M_VARARGS(get_base_reg),
    M_VARARGS(get_index_reg),
    M_VARARGS(get_memory_operand_length),
    M_VARARGS(is_mem_read),
    M_VARARGS(is_mem_written),
    M_VARARGS(is_mem_written_only),
    M_VARARGS(get_scale),

    M_NULL
};


static PyTypeObject type;


/* Allocate and initialize a new `Instruction' object given the associated
 * `xed_decoded_inst_t' structure.
 */
instruction_t *new_instruction(xed_decoded_inst_t *decoded_inst,
        xed_uint64_t runtime_address)
{
    instruction_t *instruction;
    instruction = (instruction_t *)PyType_GenericNew(&type, NULL, NULL);
    instruction->decoded_inst = decoded_inst;
    instruction->inst = xed_decoded_inst_inst(decoded_inst);
    instruction->runtime_address = runtime_address;
    return instruction;
}


/* Destructor for `pyxed.Instruction' objects. */
static void finalize_instruction_type(instruction_t *self)
{
    PyMem_Free(self->decoded_inst);
    PyObject_Del((PyObject *)self);
}

/* Initialization of `pyxed.Instruction' type should go here. */
static void initialize_instruction_type(PyTypeObject *type)
{
    /* See comment in "decoder.c" for more information. */
    static PyObject type_base[] =
    {
        PYOBJECT_INITIALIZER
    };

    *(PyObject *)type = type_base[0];
    type->tp_name = "pyxed.Instruction";
    type->tp_basicsize = sizeof(instruction_t);
    type->tp_dealloc = (destructor)finalize_instruction_type;
    type->tp_flags = Py_TPFLAGS_DEFAULT;
    type->tp_doc = "Represents a decoded instruction.";
    type->tp_methods = methods;
    type->tp_members = members;
}

void register_instruction_object(PyObject *module)
{
    initialize_instruction_type(&type);
    if(PyType_Ready(&type) == 0)
    {
        Py_INCREF(&type);
        PyModule_AddObject(module, "Instruction", (PyObject *)&type);
    }
}
