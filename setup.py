#!/usr/bin/env python
from __future__ import print_function
from distutils.core import setup, Extension
import subprocess
import os
import sys

XED_DIR = os.path.abspath("./build/")
XED_SRC_DIR = os.path.abspath("./external/xed")
XED_MFILE = os.path.join(XED_SRC_DIR, "mfile.py")
XED_KITS_DIR = os.path.join(XED_DIR, "kits")
XED_CURRENT_KIT_DIR = None


def build_xed():

    if not os.path.exists(XED_MFILE):
        print(XED_MFILE)
        print("Xed directory not initialized, run:\ngit submodule update --init --recursive", file=sys.stderr)
        return False

    if not os.path.exists(XED_KITS_DIR):
        err = subprocess.call(XED_MFILE + " --shared", shell=True)

        if err != 0:
            print("Problem with xed build", file=sys.stderr)
            return False

        ret = subprocess.check_output(XED_MFILE + " --shared install", shell=True)

        if not ret:
            print("Xed installation step failed", file=sys.stderr)
            return False

    return True


# Make a directory, don't care if it already exists
try:
    os.mkdir("./build")
except OSError:
    pass

# Change build directory for objects

old_dir = os.getcwd()
try:
    os.chdir("./build")
except OSError as e:
    print("Error opening build directory", e, file=sys.stderr)
    sys.exit(3)

if not build_xed():
    sys.exit(1)

# Restore the old directory
os.chdir(old_dir)

# Find the full kit directory
try:
    for d in os.listdir(XED_KITS_DIR):
        if "xed-install-base" in d:
            XED_CURRENT_KIT_DIR = os.path.join(XED_KITS_DIR, d)
except OSError as e:
    print("Could not find compiled Intel XED kit", e, file=sys.stderr)
    sys.exit(2)

# Set up our sources, libraries, and includes for compilation
pyxed_mod = Extension('pyxed',
                      include_dirs=[os.path.join(XED_CURRENT_KIT_DIR, "include/xed")],
                      library_dirs=[os.path.join(XED_CURRENT_KIT_DIR, "lib")],
                      libraries=['xed'],
                      sources=['check.c',
                               'decoder.c',
                               'encoder.c',
                               'instruction.c',
                               'operand.c',
                               'pyxed.c',
                               'rflags.c'])

XED_PACKAGE_DATA = ['pyxed.so',
                    XED_CURRENT_KIT_DIR + '/lib/libxed-ild.so',
                    XED_CURRENT_KIT_DIR + '/lib/libxed.so']

setup(
    name='pyxed',
    version='1.1',
    description='pyxed',
    author='huku',
    author_email='huku@grhack.net',
    url='https://github.com/huku-/pyxed',
    packages=[''],
    package_data={'': XED_PACKAGE_DATA},
    ext_modules=[pyxed_mod])

