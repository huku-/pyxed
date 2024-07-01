# Python bindings for Intel's XED

huku &lt;[huku@grhack.net](mailto:huku@grhack.net)&gt;


## About

**pyxed** is a Python extension that uses Intel's XED library to decode x86
instructions. It is designed to be a wrapper around XED's C API and, as such,
it doesn't provide any high level abstractions.


## Compiling pyxed

### Linux &amp; MacOS X

The **pyxed** build process is based on `setuptools` and should work on both
Python 3.x and Python 2.x, even though the latter is obsolete.

Begin by cloning **pyxed** and its dependencies:

    $ git submodule update --init --recursive

Create a new virtual environment for installing and testing:

    $ python -m venv /tmp/pyxed-venv               # Python 3.x
    $ virtualenv --python=python2 /tmp/pyxed-venv  # Python 2.x

Build and install **pyxed**:

    $ . /tmp/pyxed-venv/bin/activate
    $ python setup.py build
    $ python setup.py install

And, last but not least, make sure everything works as expected:

    $ pip install pytest
    $ pytest tests/


### Notes on MacOS X

Compiling on MacOS X requires the XCode command line utilities to be installed.

Depending on the XED version, you might get the following error when trying to
import **pyxed** in your project.

    Traceback (most recent call last):
      File "<string>", line 1, in <module>
    ImportError: dlopen(pyxed.so, 2): Library not loaded: obj/libxed.dylib
      Referenced from: pyxed.so
      Reason: unsafe use of relative rpath obj/libxed.dylib in pyxed.so with restricted binary

This is a problem related to hardcoded **rpath** values in Intel's **libxed.dylib**.
To fix it, run the following command after substituting **/path/to/libxed.dylib**
with the actual path of **libxed.dylib** in your filesystem.

    $ otool -L pyxed.so | grep libxed.dylib
        obj/libxed.dylib
    $ install_name_tool -change obj/libxed.dylib /path/to/libxed.dylib pyxed.so


### Microsoft Windows

Compiling on Microsoft Windows requires Visual Studio to be installed.

First set `PYTHON27_PREFIX` and `XED_PREFIX` in **Makefile.nmake** to the location
of Python and XED on your system accordingly. Then, run the following commands
to build a 32-bit binary:

    C:\pyxed> "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
    C:\pyxed> nmake /F Makefile.nmake

Or the following to build a 64-bit binary:

    C:\pyxed> "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" amd64
    C:\pyxed> nmake /F Makefile.nmake

In newer versions of Visual Studio you might need to replace the `amd64` argument
with `x86_amd64`.


### Compiling for use with IDAPython

To use **pyxed** with **IDAPython**, make sure you link the first with the
appropriate Python version.

For example, on my MacOS X system, **IDAPython** runs on Python 2.6.x:

    Python>sys.version
    2.6.7 (r267:88850, Oct 11 2012, 20:15:00)
    [GCC 4.2.1 Compatible Apple Clang 4.0 (tags/Apple/clang-418.0.60)]

To make **pyxed** work correctly I had to replace **python2.7-config** with
**python2.6-config** in **Makefile**.


## Using pyxed

For information on how to use **pyxed**, have a look at **examples/**.

If your compiler throws a warning, if you happen to hit a bug, or if you have
any comments or suggestions please let me know.


# Links

  * XED [source code](https://github.com/intelxed)
  * XED [documentation](https://intelxed.github.io/)
