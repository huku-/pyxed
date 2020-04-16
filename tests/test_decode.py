import pytest
import pyxed


def test_decode_invalid_instruction(xed32):
    xed32.itext = b"\xff"

    with pytest.raises(pyxed.InvalidInstructionError):
        xed32.decode()


def test_decode_invalid_offset(xed32):
    xed32.itext = b"\xc3"
    xed32.itext_offset = 2

    with pytest.raises(pyxed.InvalidOffsetError):
        xed32.decode()


def test_runtime_address(xed32):
    xed32.itext = b"\x89\xd8\xc3"  # mov eax, ebx; ret
    xed32.runtime_address = 0x1000

    mov = xed32.decode()
    assert mov.runtime_address == 0x1000

    ret = xed32.decode()
    assert ret.runtime_address == 0x1000 + mov.get_length()


def test_decode_32(xed32):
    xed32.itext = b"\xc3"  # ret
    insn = xed32.decode()

    assert xed32.itext_offset == insn.get_length()

    assert insn.get_isa_set_str() == "I86"
    assert insn.get_category_str() == "RET"
    assert insn.get_extension_str() == "BASE"
    assert insn.get_iclass_str() == "RET_NEAR"
    assert insn.get_iform_str() == "RET_NEAR"
    assert insn.get_length() == 1
    assert insn.dump_intel_format() == "ret "

    assert xed32.decode() is None


def test_decode_64_basic(xed64):
    xed64.itext = b"\xc3"  # ret
    insn = xed64.decode()

    assert xed64.itext_offset == insn.get_length()

    assert insn.get_isa_set_str() == "I86"
    assert insn.get_category_str() == "RET"
    assert insn.get_extension_str() == "BASE"
    assert insn.get_iclass_str() == "RET_NEAR"
    assert insn.get_iform_str() == "RET_NEAR"
    assert insn.get_length() == 1
    assert insn.dump_intel_format() == "ret "

    assert xed64.decode() is None


def test_decode_64_avx512(xed64):
    xed64.itext = b"\x62\xf2\xfd\x48\x8a\xc8"  # vcompresspd zmm0, zmm1
    insn = xed64.decode()

    assert xed64.itext_offset == insn.get_length()

    assert insn.get_isa_set_str() == "AVX512F_512"
    assert insn.get_category_str() == "COMPRESS"
    assert insn.get_extension_str() == "AVX512EVEX"
    assert insn.get_iclass_str() == "VCOMPRESSPD"
    assert insn.get_iform_str() == "VCOMPRESSPD_ZMMf64_MASKmskw_ZMMf64_AVX512"
    assert insn.get_length() == 6
    assert insn.dump_intel_format() == "vcompresspd zmm0, zmm1"
