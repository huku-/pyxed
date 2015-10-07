#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "includes.h"

typedef struct
{
    PyObject_HEAD
    unsigned long long runtime_address;
    xed_decoded_inst_t *decoded_inst;
    const xed_inst_t *inst;
} instruction_t;


instruction_t *new_instruction(xed_decoded_inst_t *, xed_uint64_t);
void register_instruction_object(PyObject *);


#endif /* _INSTRUCTION_H_ */
