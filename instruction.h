#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "includes.h"


typedef struct
{
    PyObject_HEAD
    xed_decoded_inst_t *decoded_inst;
    xed_uint64_t runtime_address;
} Instruction;


Instruction *new_instruction(xed_decoded_inst_t *, xed_uint64_t);
void register_instruction_object(PyObject *);


#endif /* _INSTRUCTION_H_ */
