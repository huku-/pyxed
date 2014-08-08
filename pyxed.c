/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * pyxed.c - Holds the extension's entry point.
 */
#include "includes.h"
#include "xed.h"
#include "instruction.h"
#include "operand.h"


static PyMethodDef methods[] =
{
    {NULL}
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
}


PyMODINIT_FUNC initpyxed(void)
{
    PyObject *module;

    /* XED specific initialization. */
    xed_tables_init();

    /* Make XED objects visible to Python. */
    module = Py_InitModule("pyxed", methods);
    register_constants(module);
    register_operand_object(module);
    register_instruction_object(module);
    register_xed_object(module);
}

