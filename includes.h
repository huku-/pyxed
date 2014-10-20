#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include "Python.h"
#include "structmember.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xed-interface.h"

/* Define two macros used for laying out readable `PyMethodDef[]' definitions.
 * Document strings are set to `NULL'; they are more or less useless since all
 * functions visible to Python have names similar to the original XED2 API. For
 * more information, consult the online manual.
 */
#define M_NOARGS(x) {#x, ((PyCFunction)(x)), METH_NOARGS, NULL}
#define M_VARARGS(x) {#x, ((PyCFunction)(x)), METH_VARARGS, NULL}

/* Define `M_NULL' to avoid using `{NULL}' in `PyMethodDef[]' definitions. Fixes
 * several compiler warnings about missing initializers thrown on my Mac OS X 
 * system by LLVM. According to K&R, however, using `{NULL}' is correct.
 */
#define M_NULL {NULL, NULL, 0, NULL}

#endif /* _INCLUDES_H_ */
