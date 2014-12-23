#ifndef _DECODER_H_
#define _DECODER_H_

#include "includes.h"

typedef struct
{
    PyObject_HEAD
    /* Members exported via `members[]'. */
    PyObject *itext;
    PyObject *itext_offset;
    PyObject *runtime_address;
    /* Members private to the implementation. */
    xed_machine_mode_enum_t mmode;
    xed_address_width_enum_t stack_addr_width;
} xed_t;


void register_decoder_object(PyObject *);

#endif /* _XED_H_ */
