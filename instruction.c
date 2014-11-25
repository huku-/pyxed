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


/* Instruction information related methods. */

static PyObject *get_category(instruction_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_decoded_inst_get_category(self->decoded_inst));
}

static PyObject *get_extension(instruction_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_decoded_inst_get_extension(self->decoded_inst));
}

static PyObject *get_iclass(instruction_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_decoded_inst_get_iclass(self->decoded_inst));
}

static PyObject *get_iform(instruction_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_decoded_inst_get_iform_enum(self->decoded_inst));
}

static PyObject *get_attribute(instruction_t *self, PyObject *args)
{
    PyObject *r = NULL;
    xed_uint32_t is_set;
    int attr;

    /* Treat enumerations as `int' values (see comment in "pyxed.c"). */
    if(PyArg_ParseTuple(args, "i", &attr) != 0)
    {
        is_set = xed_decoded_inst_get_attribute(self->decoded_inst, attr);
        r = (PyObject *)PyBool_FromLong(is_set);
    }
    return r;
}

static PyObject *get_length(instruction_t *self, PyObject *args)
{
    return PyInt_FromLong(xed_decoded_inst_get_length(self->decoded_inst));
}

static PyObject *conditionally_writes_registers(instruction_t *self,
        PyObject *args)
{
    xed_bool_t writes_registers =
        xed_decoded_inst_conditionally_writes_registers(self->decoded_inst);
    return PyBool_FromLong((long)writes_registers);
}

static PyObject *dump_intel_format(instruction_t *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst = self->decoded_inst;
    PyObject *r = NULL;
    xed_uint64_t runtime_address;
    char tmp1[64], tmp2[96];

    if(is_int_or_long(self->runtime_address))
    {
        runtime_address = PyLong_AsLong(self->runtime_address);

        /* In Pin versions before "pin-2.14-*", `xed_format_context()' takes 6
         * parameters instead of 7. Unfortunately, there's no consistent way of
         * dumping instructions between various XED versions and there's no easy
         * way of detecting the actual XED version at compile time.
         */
        xed_format_context(XED_SYNTAX_INTEL, decoded_inst, tmp1,
            sizeof(tmp1) - 1, runtime_address, NULL, NULL);

        if(xed_decoded_inst_get_machine_mode_bits(decoded_inst) == 64)
            snprintf(tmp2, sizeof(tmp2), "%.16llx %s",
                (unsigned long long)runtime_address, tmp1);
        else
            snprintf(tmp2, sizeof(tmp2), "%.8lx %s",
                (unsigned long)runtime_address, tmp1);
        r = PyString_FromString(tmp2);
    }
    return r;
}


/* Branch displacement related methods (getters & setters). */

static PyObject *get_branch_displacement(instruction_t *self, PyObject *args)
{
    xed_uint_t disp =
        xed_decoded_inst_get_branch_displacement(self->decoded_inst);
    return PyLong_FromUnsignedLong(disp);
}

static PyObject *get_branch_displacement_width(instruction_t *self,
        PyObject *args)
{
    xed_uint_t width =
        xed_decoded_inst_get_branch_displacement_width(self->decoded_inst);
    return PyLong_FromUnsignedLong(width);
}

static PyObject *get_branch_displacement_width_bits(instruction_t *self,
        PyObject *args)
{
    xed_uint_t width =
        xed_decoded_inst_get_branch_displacement_width_bits(self->decoded_inst);
    return PyLong_FromUnsignedLong(width);
}

static PyObject *set_branch_displacement(instruction_t *self, PyObject *args)
{
    xed_int32_t disp;
    xed_uint_t length_bytes;

    if(PyArg_ParseTuple(args, "iI", &disp, &length_bytes) != 0)
        xed_decoded_inst_set_branch_displacement(self->decoded_inst, disp,
            length_bytes);
    return NULL;
}

static PyObject *set_branch_displacement_bits(instruction_t *self,
        PyObject *args)
{
    xed_int32_t disp;
    xed_uint_t length_bits;

    if(PyArg_ParseTuple(args, "iI", &disp, &length_bits) != 0)
        xed_decoded_inst_set_branch_displacement_bits(self->decoded_inst, disp,
            length_bits);
    return NULL;
}


/* Memory displacement related methods (getters & setters). */

static PyObject *get_memory_displacement(instruction_t *self, PyObject *args)
{
    xed_int64_t disp;
    unsigned int mem_idx;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &mem_idx) != 0)
    {
        disp = xed_decoded_inst_get_memory_displacement(self->decoded_inst,
            mem_idx);
        r = (PyObject *)PyLong_FromLongLong(disp);
    }
    return r;
}

static PyObject *get_memory_displacement_width(instruction_t *self,
        PyObject *args)
{
    xed_uint_t width;
    unsigned int mem_idx;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &mem_idx) != 0)
    {
        width = xed_decoded_inst_get_memory_displacement_width(
            self->decoded_inst, mem_idx);
        r = (PyObject *)PyLong_FromUnsignedLong(width);
    }
    return r;
}

static PyObject *get_memory_displacement_width_bits(instruction_t *self,
        PyObject *args)
{
    xed_uint_t width;
    unsigned int mem_idx;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &mem_idx) != 0)
    {
        width = xed_decoded_inst_get_memory_displacement_width_bits(
            self->decoded_inst, mem_idx);
        r = (PyObject *)PyLong_FromUnsignedLong(width);
    }
    return r;
}

static PyObject *set_memory_displacement(instruction_t *self, PyObject *args)
{
    xed_int64_t disp;
    xed_uint_t length_bytes;

    if(PyArg_ParseTuple(args, "LI", &disp, &length_bytes) != 0)
        xed_decoded_inst_set_memory_displacement(self->decoded_inst, disp,
            length_bytes);
    return NULL;
}

static PyObject *set_memory_displacement_bits(instruction_t *self,
        PyObject *args)
{
    xed_int64_t disp;
    xed_uint_t length_bits;

    if(PyArg_ParseTuple(args, "LI", &disp, &length_bits) != 0)
        xed_decoded_inst_set_memory_displacement_bits(self->decoded_inst, disp,
            length_bits);
    return NULL;
}


/* Operand information related methods. */

static PyObject *get_noperands(instruction_t *self, PyObject *args)
{
    return PyInt_FromSize_t(xed_decoded_inst_noperands(self->decoded_inst));
}

static PyObject *get_operand(instruction_t *self, PyObject *args)
{
    xed_decoded_inst_t *decoded_inst = self->decoded_inst;
    const xed_inst_t *inst = self->inst;
    unsigned int i;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &i) != 0 &&
            i < xed_decoded_inst_noperands(decoded_inst))
        r = (PyObject *)new_operand(xed_inst_operand(inst, i));
    return r;
}

static PyObject *get_base_reg(instruction_t *self, PyObject *args)
{
    unsigned int mem_idx;
    xed_reg_enum_t reg;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &mem_idx) != 0)
    {
        reg = xed_decoded_inst_get_base_reg(self->decoded_inst, mem_idx);
        r = (PyObject *)PyInt_FromLong(reg);
    }
    return r;
}

static PyObject *get_index_reg(instruction_t *self, PyObject *args)
{
    unsigned int mem_idx;
    xed_reg_enum_t reg;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &mem_idx) != 0)
    {
        reg = xed_decoded_inst_get_index_reg(self->decoded_inst, mem_idx);
        r = (PyObject *)PyInt_FromLong(reg);
    }
    return r;
}

static PyObject *get_reg(instruction_t *self, PyObject *args)
{
    int reg_operand;
    xed_reg_enum_t reg;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "i", &reg_operand) != 0)
    {
        reg = xed_decoded_inst_get_reg(self->decoded_inst, reg_operand);
        r = (PyObject *)PyInt_FromLong(reg);
    }
    return r;
}

static PyObject *get_seg_reg(instruction_t *self, PyObject *args)
{
    unsigned int mem_idx;
    xed_reg_enum_t reg;
    PyObject *r = NULL;

    if(PyArg_ParseTuple(args, "I", &mem_idx) != 0)
    {
        reg = xed_decoded_inst_get_seg_reg(self->decoded_inst, mem_idx);
        r = (PyObject *)PyInt_FromLong(reg);
    }
    return r;
}


/* Immediate information related methods. */

static PyObject *get_immediate_is_signed(instruction_t *self, PyObject *args)
{
    xed_uint_t is_signed;
    is_signed = xed_decoded_inst_get_immediate_is_signed(self->decoded_inst);
    return (PyObject *)PyLong_FromUnsignedLong(is_signed);
}

static PyObject *get_immediate_width(instruction_t *self, PyObject *args)
{
    xed_uint_t width;
    width = xed_decoded_inst_get_immediate_width(self->decoded_inst);
    return (PyObject *)PyLong_FromUnsignedLong(width);
}

static PyObject *get_immediate_width_bits(instruction_t *self, PyObject *args)
{
    xed_uint_t width;
    width = xed_decoded_inst_get_immediate_width_bits(self->decoded_inst);
    return (PyObject *)PyLong_FromUnsignedLong(width);
}

static PyObject *get_second_immediate(instruction_t *self, PyObject *args)
{
    xed_uint8_t imm = xed_decoded_inst_get_second_immediate(self->decoded_inst);
    return (PyObject *)PyLong_FromUnsignedLong(imm);
}

static PyObject *get_signed_immediate(instruction_t *self, PyObject *args)
{
    xed_int32_t imm = xed_decoded_inst_get_signed_immediate(self->decoded_inst);
    return (PyObject *)PyLong_FromLong(imm);
}

static PyObject *get_unsigned_immediate(instruction_t *self, PyObject *args)
{
    xed_uint64_t imm;
    imm = xed_decoded_inst_get_unsigned_immediate(self->decoded_inst);
    return (PyObject *)PyLong_FromUnsignedLongLong(imm);
}


static PyMemberDef members[] =
{
    {"runtime_address", T_OBJECT, offsetof(instruction_t, runtime_address), 0,
        "Runtime address of this instruction"},
    {NULL, 0, 0, 0, NULL}
};

static PyMethodDef methods[] =
{
    /* Instruction information related methods. */
    M_NOARGS(get_category),
    M_NOARGS(get_extension),
    M_NOARGS(get_iclass),
    M_NOARGS(get_iform),
    M_VARARGS(get_attribute),
    M_NOARGS(get_length),
    M_NOARGS(conditionally_writes_registers),
    M_NOARGS(dump_intel_format),

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
    M_VARARGS(get_base_reg),
    M_VARARGS(get_index_reg),
    M_VARARGS(get_reg),
    M_VARARGS(get_seg_reg),

    /* Immediate information related methods. */
    M_NOARGS(get_immediate_is_signed),
    M_NOARGS(get_immediate_width),
    M_NOARGS(get_immediate_width_bits),
    M_NOARGS(get_second_immediate),
    M_NOARGS(get_signed_immediate),
    M_NOARGS(get_unsigned_immediate),

    M_NULL
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

