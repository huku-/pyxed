#ifndef _RFLAGS_H_
#define _RFLAGS_H_

#include "includes.h"


typedef struct
{
    PyObject_HEAD
    unsigned int _if;
    unsigned int ac;
    unsigned int af;
    unsigned int cf;
    unsigned int df;
    unsigned int id;
    unsigned int nt;
    unsigned int of;
    unsigned int pf;
    unsigned int rf;
    unsigned int sf;
    unsigned int tf;
    unsigned int vif;
    unsigned int vip;
    unsigned int vm;
    unsigned int zf;
} rflags_t;


rflags_t *new_rflags(const xed_flag_set_t *);
void register_rflags_object(PyObject *);


#endif /* _RFLAGS_H_ */
