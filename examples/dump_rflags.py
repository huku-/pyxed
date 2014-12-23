#!/usr/bin/env python
'''dump_rflags.py - Dump the flags read/written by each instruction.'''

__author__ = 'huku <huku@grhack.net>'


import sys
import binascii

import pyxed


def rflags_to_str(flags):
    '''Return a string representation of the given `Rflags' object.'''

    names = [
        '_if', 'ac', 'af', 'cf', 'df', 'id', 'nt', 'of', 'pf', 'rf', 'sf', 'tf',
        'vif', 'vip', 'vm', 'zf'
    ]

    s = ''
    for name in names:
        s += '%s=%d ' % (name, getattr(flags, name))
    return s


def main(argv):

    if len(argv) != 2:
        print '%s <hex>' % argv[0]
        return 1

    xed = pyxed.Decoder()
    xed.set_mode(pyxed.XED_MACHINE_MODE_LEGACY_32, pyxed.XED_ADDRESS_WIDTH_32b)
    xed.itext = binascii.unhexlify(argv[1])
    xed.runtime_address = 0x10001000

    while True:
        inst = xed.decode()
        if inst is None:
            break
        print '%s' % inst.dump_intel_format()
        print 'Read: %s' % rflags_to_str(inst.get_rflags_read())
        print 'Undefined: %s' % rflags_to_str(inst.get_rflags_undefined()) 
        print 'Written: %s' % rflags_to_str(inst.get_rflags_written()) 
        print '-'

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))


