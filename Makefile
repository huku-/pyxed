PYTHON27_HEADERS=$(shell python2.7-config --includes)
PYTHON27_LIBS=$(shell python2.7-config --libs)

#
# Path to XED kit produced by "./mfile.py install". See XED's documentation for
# more information on how to compile it from source.
#
XED_PREFIX=

XED_HEADERS=$(XED_PREFIX)/include/xed
XED_LIBS=$(XED_PREFIX)/lib

W=-Wall -Wextra \
    -Wno-cast-function-type \
    -Wno-old-style-declaration \
    -Wno-strict-aliasing \
    -Wno-unknown-warning \
    -Wno-unknown-warning-option \
    -Wno-unused-parameter

CFLAGS=-fPIC -O2 $(PYTHON27_HEADERS) -I$(XED_HEADERS) $(W) -ggdb
LDFLAGS=$(PYTHON27_LIBS) -lpython2.7 -L$(XED_LIBS) -lxed -shared

OBJS=check.o rflags.o operand.o instruction.o decoder.o encoder.o pyxed.o
BIN=pyxed.so

CC=cc

all: pyc $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LDFLAGS)

.PHONY: install
install:
	python setup.py install

.PHONY: clean
clean: pyc
	rm -fr $(OBJS) $(BIN) core *.core a.out *~ build

.PHONY: pyc
pyc:
	rm -fr *.pyc
