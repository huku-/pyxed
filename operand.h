#ifndef _OPERAND_H_
#define _OPERAND_H_

#include "includes.h"

typedef struct
{
    PyObject_HEAD
    /* Members private to the implementation. */
    const xed_operand_t *operand;
} operand_t;


operand_t *new_operand(const xed_operand_t *);
void register_operand_object(PyObject *);


#endif /* _OPERAND_H_ */
