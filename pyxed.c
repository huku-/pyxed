/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * pyxed.c - Holds the extension's entry point.
 */
#include "includes.h"
#include "decoder.h"
#include "instruction.h"
#include "operand.h"
#include "rflags.h"


/* Exceptions thrown by this module should go here. Don't forget to also declare
 * them as `extern' in "pyxed.h".
 */
PyObject *invalid_instruction;
PyObject *invalid_offset;


static PyMethodDef methods[] =
{
    M_NULL
};


/* Register several XED related constants in the module's global scope. */
static void register_constants(PyObject *module)
{
    int i;
    const char *name;
    char full_name[BUFSIZ];

    /* Constants from "xed-address-width-enum.h". */
    PyModule_AddObject(module, "XED_ADDRESS_WIDTH_INVALID",
        PyInt_FromLong(XED_ADDRESS_WIDTH_INVALID));
    PyModule_AddObject(module, "XED_ADDRESS_WIDTH_16b",
        PyInt_FromLong(XED_ADDRESS_WIDTH_16b));
    PyModule_AddObject(module, "XED_ADDRESS_WIDTH_32b",
        PyInt_FromLong(XED_ADDRESS_WIDTH_32b));
    PyModule_AddObject(module, "XED_ADDRESS_WIDTH_64b",
        PyInt_FromLong(XED_ADDRESS_WIDTH_64b));
    PyModule_AddObject(module, "XED_ADDRESS_WIDTH_LAST",
        PyInt_FromLong(XED_ADDRESS_WIDTH_LAST));


    /* "The identifiers in an enumerator list are declared as constants that
     *  have type `int' and may appear wherever such are permitted."
     *  -- N1570 6.7.2.2
     */

    /* Constants from "xed-machine-mode-enum.h". */
    for(i = XED_MACHINE_MODE_INVALID; i <= XED_MACHINE_MODE_LAST; i++)
    {
        name = xed_machine_mode_enum_t2str((xed_machine_mode_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_MACHINE_MODE_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-iform-enum.h". */
    for(i = XED_IFORM_INVALID; i <= XED_IFORM_LAST; i++)
    {
        name = xed_iform_enum_t2str((xed_iform_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_IFORM_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-iclass-enum.h". */
    for(i = XED_ICLASS_INVALID; i <= XED_ICLASS_LAST; i++)
    {
        name = xed_iclass_enum_t2str((xed_iclass_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_ICLASS_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-category-enum.h". */
    for(i = XED_CATEGORY_INVALID; i <= XED_CATEGORY_LAST; i++)
    {
        name = xed_category_enum_t2str((xed_category_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_CATEGORY_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-extension-enum.h". */
    for(i = XED_EXTENSION_INVALID; i <= XED_EXTENSION_LAST; i++)
    {
        name = xed_extension_enum_t2str((xed_extension_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_EXTENSION_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-isa-set-enum.h". */
    for(i = XED_ISA_SET_INVALID; i <= XED_ISA_SET_LAST; i++)
    {
        name = xed_isa_set_enum_t2str((xed_isa_set_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_ISA_SET_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-operand-action-enum.h". */
    for(i = XED_OPERAND_ACTION_INVALID; i <= XED_OPERAND_ACTION_LAST; i++)
    {
        name = xed_operand_action_enum_t2str((xed_operand_action_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_OPERAND_ACTION_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-operand-enum.h". */
    for(i = XED_OPERAND_INVALID; i <= XED_OPERAND_LAST; i++)
    {
        name = xed_operand_enum_t2str((xed_operand_type_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_OPERAND_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-operand-type-enum.h". */
    for(i = XED_OPERAND_TYPE_INVALID; i <= XED_OPERAND_TYPE_LAST; i++)
    {
        name = xed_operand_type_enum_t2str((xed_operand_type_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_OPERAND_TYPE_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-operand-element-xtype-enum.h". */
    for(i = XED_OPERAND_XTYPE_INVALID; i <= XED_OPERAND_XTYPE_LAST; i++)
    {
        name = xed_operand_element_xtype_enum_t2str(
            (xed_operand_element_xtype_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_OPERAND_XTYPE_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-operand-width-enum.h". */
    for(i = XED_OPERAND_WIDTH_INVALID; i <= XED_OPERAND_WIDTH_LAST; i++)
    {
        name = xed_operand_width_enum_t2str((xed_operand_width_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_OPERAND_WIDTH_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-attribute-enum.h". */
    for(i = XED_ATTRIBUTE_INVALID; i <= XED_ATTRIBUTE_LAST; i++)
    {
        name = xed_attribute_enum_t2str((xed_attribute_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_ATTRIBUTE_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }

    /* Constants from "xed-reg-enum.h". */
    for(i = XED_REG_INVALID; i <= XED_REG_LAST; i++)
    {
        name = xed_reg_enum_t2str((xed_reg_enum_t)i);
        snprintf(full_name, sizeof(full_name), "XED_REG_%s", name);
        PyModule_AddObject(module, full_name, PyInt_FromLong(i));
    }
}


static void register_exceptions(PyObject *module)
{
    /* Initialize `InvalidInstructionError' exception object. */
    invalid_instruction = PyErr_NewException("pyxed.InvalidInstructionError",
        PyExc_StandardError, NULL);
    PyModule_AddObject(module, "InvalidInstructionError", invalid_instruction);

    /* Initialize `InvalidOffsetException' exception object. */
    invalid_offset = PyErr_NewException("pyxed.InvalidOffsetException",
        PyExc_StandardError, NULL);
    PyModule_AddObject(module, "InvalidOffsetException", invalid_offset);
}


PyMODINIT_FUNC initpyxed(void)
{
    PyObject *module;

    /* XED specific initialization. */
    xed_tables_init();

    /* Make XED objects visible to Python. */
    module = Py_InitModule("pyxed", methods);
    register_constants(module);
    register_exceptions(module);
    register_rflags_object(module);
    register_operand_object(module);
    register_instruction_object(module);
    register_decoder_object(module);
}

