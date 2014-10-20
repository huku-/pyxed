PYTHON27_PREFIX=/usr
PYTHON27_HEADERS=$(PYTHON27_PREFIX)/include/python2.7
PYTHON27_LIBS=$(PYTHON27_PREFIX)/lib

XED_PREFIX=pin-2.14-67254-gcc.4.4.7-linux/extras/xed2-ia32
# XED_PREFIX=pin-2.14-67254-gcc.4.4.7-linux/extras/xed2-intel64
XED_HEADERS=$(XED_PREFIX)/include
XED_LIBS=$(XED_PREFIX)/lib

W=-Wall -Wextra \
    -Wno-unused-parameter \
    -Wno-old-style-declaration \
    -Wno-strict-aliasing

CFLAGS=-fPIC -O2 -I$(PYTHON27_HEADERS) -I$(XED_HEADERS) $(W) -ggdb
LDFLAGS=-L$(PYTHON27_LIBS) -lpython2.7 -L$(XED_LIBS) -lxed -shared

OBJS=check.o operand.o instruction.o xed.o pyxed.o
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