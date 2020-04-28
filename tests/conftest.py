import pytest
import pyxed


@pytest.fixture()
def xed32():
    xed = pyxed.Decoder()
    xed.set_mode(pyxed.XED_MACHINE_MODE_LEGACY_32, pyxed.XED_ADDRESS_WIDTH_32b)
    return xed


@pytest.fixture()
def xed64():
    xed = pyxed.Decoder()
    xed.set_mode(pyxed.XED_MACHINE_MODE_LONG_64, pyxed.XED_ADDRESS_WIDTH_64b)
    return xed
