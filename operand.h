#ifndef _OPERAND_H_
#define _OPERAND_H_

#include "includes.h"

#define m_operand(x) ((Operand *)(x))->operand


typedef struct
{
    PyObject_HEAD
    /* Members private to the implementation. */
    const xed_operand_t *operand;
} Operand;


Operand *new_operand(const xed_operand_t *);
void register_operand_object(PyObject *);


#endif /* _OPERAND_H_ */
