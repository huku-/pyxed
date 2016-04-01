#!/usr/bin/env python
'''dump_written_regs.py - Shows what registers are modified by each instruction.'''

__author__ = 'huku <huku@grhack.net>'


import sys
import binascii

import pyxed


def get_reg_name(reg):
    '''
    Return a register's name given its identifier as returned by `get_reg()' of
    class `Instruction'.
    '''

    reg_name = '?'
    for name in dir(pyxed):
        if name.startswith('XED_REG_') and getattr(pyxed, name) == reg:
            reg_name = name
            break
    return reg_name


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

        print '| %s' % inst.dump_intel_format()

        # Iterate through all the operands of the current instruction.
        for i in range(inst.get_noperands()):
            op = inst.get_operand(i)

            # If the operand is a register, check if it's modified by the
            # current instruction.
            if op.is_register():
                reg_name = get_reg_name(inst.get_reg(op.get_name()))
                if op.is_read_and_written():
                    print '\_ %-16s RW' % reg_name
                elif op.is_written_only():
                    print '\_ %-16s W' % reg_name

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))

