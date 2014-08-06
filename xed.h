#ifndef _XED_H_
#define _XED_H_

#include "includes.h"


typedef struct
{
    PyObject_HEAD
    /* Members exported through the `members[]' array below. */
    PyObject *itext;
    PyObject *runtime_address;
    /* Members private to the implementation. */
    xed_machine_mode_enum_t mmode;
    xed_address_width_enum_t stack_addr_width;
    ptrdiff_t itext_offset;
} XED;


void register_xed_object(PyObject *);

#endif /* _XED_H_ */
