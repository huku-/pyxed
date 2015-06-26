# Python bindings for Intel's XED2

huku &lt;[huku@grhack.net](mailto:huku@grhack.net)&gt;

<a href="https://pledgie.com/campaigns/27150"><img alt="" src="https://pledgie.com/campaigns/27150.png?skin_name=chrome" border="0" ></a>


## About

WIP Python extension that uses Intel's XED2 library to decode x86 instructions.

**pyxed** is designed to be a wrapper around XED's C API and, as such, it
doesn't provide any high level abstractions.

This project, which is actually my first attempt to develop a useful Python
extension, is in its very early stages of development, so, expect things to
change in the near future. If your compiler throws a warning, if you happen
to hit a bug, or if you have any comments/suggestions please let me know.


## Compiling pyxed

### Unixoids

To compile **pyxed**, set appropriate paths for **PYTHON27_PREFIX** and
**XED_PREFIX** in the accompanying makefile and then run **make**.


### Microsoft Windows

Compiling on Microsoft Windows requires Visual Studio to be installed. Just open
a console, enter **pyxed**'s toplevel directory and run the following commands:

```
C:\pyxed> "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
C:\pyxed> nmake /F Makefile.nmake
```

### Compiling for use with IDAPython

To use **pyxed** with **IDAPython**, make sure you link the first with the
appropriate Python version.

For example, on my MacOS X system, **IDAPython** runs on Python 2.6.x:

```
Python>sys.version
2.6.7 (r267:88850, Oct 11 2012, 20:15:00) 
[GCC 4.2.1 Compatible Apple Clang 4.0 (tags/Apple/clang-418.0.60)]
```

To make **pyxed** work correctly I had to replace **-lpython2.7** with
**-lpython2.6** in **Makefile**.


### Precompiled binaries

Windows users can use an experimental precompiled 32-bit release of pyxed which
can be downloaded from [here](https://www.grhack.net/pyxed-x86.zip).

Debian packages are also planned for the near future.


## Using pyxed

For information on how to use **pyxed**, have a look at **examples/**.

