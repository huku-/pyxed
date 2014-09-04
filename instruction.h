#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "includes.h"

typedef struct
{
    PyObject_HEAD
    /* Members exported via `members[]'. */
    PyObject *runtime_address;
    /* Members private to the implementation. */
    xed_decoded_inst_t *decoded_inst;
    const xed_inst_t *inst;
} instruction_t;


instruction_t *new_instruction(xed_decoded_inst_t *, xed_uint64_t);
void register_instruction_object(PyObject *);


#endif /* _INSTRUCTION_H_ */
