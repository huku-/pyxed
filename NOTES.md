# Notes

Scraps of notes on **pyxed** development.

huku &lt;[huku@grhack.net](mailto:huku@grhack.net)&gt;


## Converting C datatypes to Python PyObjects

Most functions exported to Python are simple wrappers around the low level C API 
offered by XED2. To be properly returned to the user, the return values of those
C functions must be converted to one of the datatypes recognized by the Python 
runtime (i.e. **PyObject** pointers). For the sake of developing **pyxed**, I
settled down to the following set of simple guidelines:

  * When a XED2 function returns **xed_uint_t** to indicate a true/false result,
    or **xed_bool_t** (also **typedef**'ed to **unsigned int**), use 
    **PyBool_FromLong()** to retrieve the corresponding Python object.

  * When functions return arbitrary values of type **xed_uint_t**, use
    **PyLong_FromUnsignedLong()**.

  * For return values of type **xed_*_enum_t**, use **PyInt_FromLong()**
    (according to the C standard, enumerations are treated as **int** values).
