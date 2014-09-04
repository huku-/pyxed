#ifndef _CHECK_H_
#define _CHECK_H_

#include "includes.h"

int is_int(PyObject *);
int is_long(PyObject *);
int is_int_or_long(PyObject *);
int is_string(PyObject *);

#endif /* _CHECK_H_ */
