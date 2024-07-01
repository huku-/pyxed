#!/usr/bin/env python
#
# When modifying this file, always keep in mind that it has to work on both
# Python 2.x and 3.x.
#
from __future__ import print_function

import contextlib
import os
import setuptools
import subprocess
import sys


DIR = os.path.dirname(os.path.abspath(__file__))

XED_BUILD_DIR = os.path.join(DIR, "build")
XED_KITS_DIR = os.path.join(XED_BUILD_DIR, "kits")
XED_CURRENT_KIT_DIR = os.getenv("XED_CURRENT_KIT_DIR")


def get_sources():
    return [
        name
        for name in os.listdir(DIR)
        if os.path.isfile(os.path.join(DIR, name)) and name.endswith(".c")
    ]


def get_extra_link_args():
    if sys.platform.startswith("linux"):
        return ["-Wl,--whole-archive", "-lxed", "-Wl,--no-whole-archive"]
    elif sys.platform.startswith("darwin"):
        return ["-Wl,-force_load", os.path.join(XED_CURRENT_KIT_DIR, "lib", "libxed.a")]
    else:
        raise EnvironmentError(
            "Don't know how to link static XED to pyxed on platform %s" % sys.platform
        )


def build_xed():

    mfile = os.path.join(DIR, "external", "xed", "mfile.py")

    if not os.path.exists(mfile):
        raise EnvironmentError(
            "XED directory not initialized, run: git submodule update --init --recursive"
        )

    if not os.path.exists(XED_KITS_DIR):
        subprocess.check_call(
            "%s --static --extra-flags=-fPIC install" % mfile, shell=True
        )


@contextlib.contextmanager
def mkchdir(wd):

    cwd = os.getcwd()

    if not os.path.exists(wd):
        os.makedirs(wd)

    try:
        os.chdir(wd)
        yield wd
    finally:
        os.chdir(cwd)


def main(argv):

    global XED_CURRENT_KIT_DIR

    if XED_CURRENT_KIT_DIR is None:

        with mkchdir(XED_BUILD_DIR):
            build_xed()

        for name in os.listdir(XED_KITS_DIR):
            if "xed-install-base" in name:
                XED_CURRENT_KIT_DIR = os.path.join(XED_KITS_DIR, name)

        if XED_CURRENT_KIT_DIR is None:
            raise EnvironmentError("Could not find compiled Intel XED kit")

    pyxed_mod = setuptools.Extension(
        "pyxed",
        include_dirs=[os.path.join(XED_CURRENT_KIT_DIR, "include", "xed")],
        library_dirs=[os.path.join(XED_CURRENT_KIT_DIR, "lib")],
        extra_compile_args=["-fPIC"],
        extra_link_args=get_extra_link_args(),
        sources=[os.path.join(DIR, name) for name in get_sources()],
    )

    setuptools.setup(
        name="pyxed",
        version="1.1",
        description="pyxed",
        author="huku",
        author_email="huku@grhack.net",
        url="https://github.com/huku-/pyxed",
        ext_modules=[pyxed_mod],
        extras_require={"dev": ["pytest"]},
    )

    return os.EX_OK


if __name__ == "__main__":
    sys.exit(main(sys.argv))
