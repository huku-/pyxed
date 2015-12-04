#ifndef _DECODER_H_
#define _DECODER_H_

#include "includes.h"

typedef struct
{
    PyObject_HEAD
    PyObject *itext;
    unsigned long long itext_offset;
    unsigned long long runtime_address;
    xed_machine_mode_enum_t mmode;
    xed_address_width_enum_t stack_addr_width;
} xed_t;


void register_decoder_object(PyObject *);

#endif /* _DECODER_H_ */
