#!/usr/bin/env python
from __future__ import print_function
from setuptools import setup, Extension
import subprocess
import os
import sys

HERE = os.path.dirname(__file__)

XED_BUILD_DIR = os.path.abspath(os.getenv("XED_BUILD_DIR", "./build/"))
XED_SRC_DIR = os.path.join(HERE, "external/xed")
XED_MFILE = os.path.join(XED_SRC_DIR, "mfile.py")
XED_KITS_DIR = os.path.join(XED_BUILD_DIR, "kits")
XED_CURRENT_KIT_DIR = os.getenv("XED_CURRENT_KIT_DIR")

SOURCES = [
    "check.c",
    "decoder.c",
    "encoder.c",
    "instruction.c",
    "operand.c",
    "pyxed.c",
    "rflags.c",
]


def build_xed():

    if not os.path.exists(XED_MFILE):
        print(XED_MFILE)
        print(
            "Xed directory not initialized, run:\ngit submodule update --init --recursive",
            file=sys.stderr,
        )
        return False

    if not os.path.exists(XED_KITS_DIR):
        err = subprocess.call(XED_MFILE + ' --static --extra-flags="-fPIC"', shell=True)

        if err != 0:
            print("Problem with xed build", file=sys.stderr)
            return False

        ret = subprocess.check_output(
            XED_MFILE + ' --static --extra-flags="-fPIC" install', shell=True
        )

        if not ret:
            print("Xed installation step failed", file=sys.stderr)
            return False

    return True


def extra_link_args():
    if sys.platform.startswith("linux"):
        return ["-Wl,--whole-archive", "-lxed", "-Wl,--no-whole-archive"]
    elif sys.platform.startswith("darwin"):
        return ["-Wl,-force_load", os.path.join(XED_CURRENT_KIT_DIR, "lib/libxed.a")]
    else:
        print(
            f"Don't know how to link static XED to pyxed on your platform: {sys.platform}"
        )
        sys.exit(1)


# Make a directory, don't care if it already exists
try:
    os.mkdir(XED_BUILD_DIR)
except OSError:
    pass

# Change build directory for objects

old_dir = os.getcwd()
try:
    os.chdir(XED_BUILD_DIR)
except OSError as e:
    print("Error opening build directory", e, file=sys.stderr)
    sys.exit(3)

if XED_CURRENT_KIT_DIR is None and not build_xed():
    sys.exit(1)

# Restore the old directory
os.chdir(old_dir)

# Find the full kit directory
if XED_CURRENT_KIT_DIR is None:
    try:
        for d in os.listdir(XED_KITS_DIR):
            if "xed-install-base" in d:
                XED_CURRENT_KIT_DIR = os.path.join(XED_KITS_DIR, d)
    except OSError as e:
        print("Could not find compiled Intel XED kit", e, file=sys.stderr)
        sys.exit(2)

# Set up our sources, libraries, and includes for compilation
pyxed_mod = Extension(
    "pyxed",
    include_dirs=[os.path.join(XED_CURRENT_KIT_DIR, "include/xed")],
    library_dirs=[os.path.join(XED_CURRENT_KIT_DIR, "lib")],
    extra_compile_args=["-fPIC"],
    extra_link_args=extra_link_args(),
    sources=[os.path.join(HERE, source) for source in SOURCES],
)

setup(
    name="pyxed",
    version="1.1",
    description="pyxed",
    author="huku",
    author_email="huku@grhack.net",
    url="https://github.com/huku-/pyxed",
    packages=[""],
    ext_modules=[pyxed_mod],
    extras_require={"dev": ["pytest"]},
)
