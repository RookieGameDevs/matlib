"""Quaternion class tests."""
from math import pi
from matlib.qtr import Qtr
from matlib.vec import Vec


def test_rotation():
    # TODO: extensive testing
    q = Qtr()
    q.rotate(1, 0, 0, pi / 4)

    q = Qtr()
    q.rotatev(Vec(1, 0, 0), pi / 4)


def test_norm():
    # TODO: extensive testing
    q = Qtr()
    q.norm()


def test_lerp():
    # TODO: extensive testing
    q1 = Qtr()
    q2 = Qtr()
    q3 = q1.lerp(q2, 0.5)
    assert q3


def test_add():
    # TODO: extensive testing
    q1 = Qtr()
    q2 = Qtr()
    qr = q1 + q2
    assert qr

    q1 += q2


def test_mul():
    # TODO: extensive testing
    q1 = Qtr()
    q2 = Qtr()
    qr = q1 * q2
    assert qr

    q1 *= q2


def test_mulf():
    # TODO: extensive testing
    q = Qtr()
    qr = q * 123.456
    assert qr

    q *= 123.456
