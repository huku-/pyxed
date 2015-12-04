/* pyxed - Python bindings for Intel's XED2
 * huku <huku@grhack.net>
 *
 * check.c - Functions for checking the underlying type of `PyObject' variables.
 */
#include "includes.h"
#include "check.h"


/* We always check if the passed object pointer is `NULL' (i.e. the object has
 * been deleted).
 */

int is_int(PyObject *object)
{
    return (object && PyInt_CheckExact(object));
}

int is_long(PyObject *object)
{
    return (object && PyLong_CheckExact(object));
}

int is_int_or_long(PyObject *object)
{
    return (is_int(object) || is_long(object));
}

int is_string(PyObject *object)
{
    return (object && PyString_CheckExact(object));
}

