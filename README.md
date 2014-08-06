# Python bindings for Intel's XED2

huku <huku@grhack.net>

## About

WIP Python extension that uses Intel's XED2 library to decode x86 instructions.
Currently, it can only print decoded instructions in Intel format.

This project, which is actually my first attempt to develop a useful Python
extension, is in its very early stages of development, so, expect things to
change in the near future. If your compiler throws a warning, if you happen
to hit a bug, or if you have any comments/suggestions please let me know.

## Compiling pyxed

To compile **pyxed**, set appropriate paths for **PYTHON27_PREFIX** and
**XED_PREFIX** in the accompanying makefile and then run **make**.

## Using pyxed

For information on how to use **pyxed**, have a look at **test.py**.

```
$ python test.py e90000000090
10001000 jmp 0x10001005
10001005 nop 
```
