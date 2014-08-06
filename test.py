import sys
import binascii

import pyxed


def main(argv):

    if len(argv) != 2:
        print '%s <hex>' % argv[0]
        return 1

    xed = pyxed.XED()
    xed.set_mode(pyxed.XED_MACHINE_MODE_LEGACY_32, pyxed.XED_ADDRESS_WIDTH_32b)
    # xed.set_mode(pyxed.XED_MACHINE_MODE_LONG_64, pyxed.XED_ADDRESS_WIDTH_64b)
    xed.itext = binascii.unhexlify(argv[1])
    xed.runtime_address = 0x10001000

    while True:
        inst = xed.decode()
        if inst is None:
            break
        print inst.dump_intel_format()

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))

