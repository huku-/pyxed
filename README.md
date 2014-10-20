# Python bindings for Intel's XED2

huku &lt;[huku@grhack.net](@grhack.net)&gt;

## About

WIP Python extension that uses Intel's XED2 library to decode x86 instructions.

**pyxed** is designed to be a wrapper around XED's C API and, as such, it
doesn't provide any high level abstractions.

This project, which is actually my first attempt to develop a useful Python
extension, is in its very early stages of development, so, expect things to
change in the near future. If your compiler throws a warning, if you happen
to hit a bug, or if you have any comments/suggestions please let me know.

## Compiling pyxed

To compile **pyxed**, set appropriate paths for **PYTHON27_PREFIX** and
**XED_PREFIX** in the accompanying makefile and then run **make**.

## Using pyxed

For information on how to use **pyxed**, have a look at **examples/**.

