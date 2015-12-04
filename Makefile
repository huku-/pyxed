PYTHON27_HEADERS=$(shell python2.7-config --includes)
PYTHON27_LIBS=$(shell python2.7-config --libs)

XED_PREFIX=pin-2.14/extras/xed-ia32
# XED_PREFIX=pin-2.14-71313-clang.5.1-mac/extras/xed-ia32
# XED_PREFIX=pin-2.14-71313-clang.5.1-mac/extras/xed-intel64
# XED_PREFIX=pin-2.14-71313-gcc.4.4.7-linux/extras/xed-ia32
# XED_PREFIX=pin-2.14-71313-gcc.4.4.7-linux/extras/xed-intel64
XED_HEADERS=$(XED_PREFIX)/include
XED_LIBS=$(XED_PREFIX)/lib

W=-Wall -Wextra \
    -Wno-unknown-warning \
    -Wno-unknown-warning-option \
    -Wno-unused-parameter \
    -Wno-old-style-declaration \
    -Wno-strict-aliasing

CFLAGS=-fPIC -O2 $(PYTHON27_HEADERS) -I$(XED_HEADERS) $(W) -ggdb
LDFLAGS=$(PYTHON27_LIBS) -lpython2.7 -L$(XED_LIBS) -lxed -shared

OBJS=check.o rflags.o operand.o instruction.o decoder.o encoder.o pyxed.o
BIN=pyxed.so

CC=cc

all: pyc $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LDFLAGS)

.PHONY: clean
clean: pyc
	rm -fr $(OBJS) $(BIN) core *.core a.out *~

.PHONY: pyc
pyc:
	rm -fr *.pyc

