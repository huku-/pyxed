# Python bindings for Intel's XED

huku &lt;[huku@grhack.net](mailto:huku@grhack.net)&gt;

<a href="https://pledgie.com/campaigns/27150"><img alt="" src="https://pledgie.com/campaigns/27150.png?skin_name=chrome" border="0" ></a>


## About

**pyxed** is a Python extension that uses Intel's XED library to decode x86
instructions. It is designed to be a wrapper around XED's C API and, as such,
it doesn't provide any high level abstractions.


## Compiling pyxed

**pyxed** comes with Makefiles for the most widely used platforms, namely
Microsoft Windows, MacOS X and Linux. All Makefiles require that you edit them
and set **XED_PREFIX** appropriately.


### Linux

Compiling on Linux requires GCC to be installed (chances are it is). Just open a
terminal, enter **pyxed**'s top-level directory and run **make**.


### MacOS X

Compiling on MacOS X requires the XCode command line utilities to be installed.
Just open a terminal, enter **pyxed**'s top-level directory and run **make**.

To test that everything works as expected, use the following command:

```sh
$ PYTHONPATH=. python examples/dump_asm.py 41414141
```

Depending on the XED version, you might get the following error when trying to
import **pyxed** in your project.

```
Traceback (most recent call last):
  File "examples/dump_asm.py", line 10, in <module>
    import pyxed
ImportError: dlopen(pyxed.so, 2): Library not loaded: obj-mac-clang-5.1.0-shared/libxed.dylib
  Referenced from: pyxed.so
  Reason: unsafe use of relative rpath obj-mac-clang-5.1.0-shared/libxed.dylib in pyxed.so with restricted binary
```

This is a problem related to hardcoded **rpath** values in Intel's **libxed.dylib**.
To fix it, run the following command after substituting **/path/to/libxed.dylib**
with the actual path of **libxed.dylib** in your filesystem.

```sh
$ install_name_tool -change obj-mac-clang-5.1.0-shared/libxed.dylib /path/to/libxed.dylib pyxed.so
```


### Microsoft Windows

Compiling on Microsoft Windows requires Visual Studio to be installed. Just open
a console, enter **pyxed**'s top-level directory and set **PYTHON27_PREFIX** in
**Makefile** to the location where Python is installed on your system. Then, run
the following commands to build a 32-bit binary:

```
C:\pyxed> "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
C:\pyxed> nmake /F Makefile.nmake
```

Or the following to build a 64-bit binary:

```
C:\pyxed> "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" amd64
C:\pyxed> nmake /F Makefile.nmake
```

In newer versions of Visual Studio you might need to replace the **amd64**
argument with **x86_amd64**.


### Compiling for use with IDAPython

To use **pyxed** with **IDAPython**, make sure you link the first with the
appropriate Python version.

For example, on my MacOS X system, **IDAPython** runs on Python 2.6.x:

```
Python>sys.version
2.6.7 (r267:88850, Oct 11 2012, 20:15:00)
[GCC 4.2.1 Compatible Apple Clang 4.0 (tags/Apple/clang-418.0.60)]
```

To make **pyxed** work correctly I had to replace **python2.7-config** with
**python2.6-config** in **Makefile**.


### Precompiled binaries

Experimental precompiled binaries for Microsoft Windows can be downloaded from
[here](https://www.grhack.net/pyxed-vs2012-xed_2016_02_02.tgz). The tarball
includes both a 32-bit and a 64-bit release of **pyxed**.

Debian packages are also planned for the near future.


## Using pyxed

For information on how to use **pyxed**, have a look at **examples/**.

If your compiler throws a warning, if you happen to hit a bug, or if you have
any comments or suggestions please let me know.


## References

  * XED [downloads](https://software.intel.com/en-us/articles/xed-x86-encoder-decoder-software-library)
  * XED [documentation](https://software.intel.com/sites/landingpage/xed/ref-manual/html/index.html)

