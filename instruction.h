#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "includes.h"

#define m_decoded_inst(x) ((Instruction *)(x))->decoded_inst
#define m_inst(x) ((Instruction *)(x))->inst
#define m_runtime_address(x) ((Instruction *)(x))->runtime_address


typedef struct
{
    PyObject_HEAD
    /* Members private to the implementation. */
    xed_decoded_inst_t *decoded_inst;
    const xed_inst_t *inst;
    xed_uint64_t runtime_address;
} Instruction;


Instruction *new_instruction(xed_decoded_inst_t *, xed_uint64_t);
void register_instruction_object(PyObject *);


#endif /* _INSTRUCTION_H_ */
