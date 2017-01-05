"""Mat class tests."""
from .util import assert_float_equal
from math import pi
from matlib import Mat
from matlib import Vec
import pytest


def test_slice():
    m = Mat()
    x = [1, 0, 0, 0]
    y = [0, 1, 0, 0]
    z = [0, 0, 1, 0]
    w = [0, 0, 0, 1]

    assert m[0:] == x
    assert m[0:4] == x
    assert m[:0] == x

    assert m[1:] == y
    assert m[1:4] == y
    assert m[:1] == y

    assert m[2:] == z
    assert m[2:4] == z
    assert m[:2] == z

    assert m[3:] == w
    assert m[3:4] == w
    assert m[:3] == w

    with pytest.raises(IndexError):
        m[-1:]
    with pytest.raises(IndexError):
        m[5:]
    with pytest.raises(IndexError):
        m[0:5]
    with pytest.raises(IndexError):
        m[25:25]


def test_index():
    m = Mat()
    assert m[0, 0] == m[1, 1] == m[2, 2] == m[3, 3] == 1

    with pytest.raises(IndexError):
        m[-1, -1]
    with pytest.raises(IndexError):
        m[0, 5]
    with pytest.raises(IndexError):
        m[20, 20]


def test_translate():
    m = Mat()
    m.translate(-5, 10, 20)
    p = Vec()
    p = m * p
    assert p.x == -5
    assert p.y == 10
    assert p.z == 20

    m.ident()
    m.translatev(Vec(5, -10, -20))
    p2 = m * Vec()
    assert p2.x == 5
    assert p2.y == -10
    assert p2.z == -20

    m_i = m.inverse()
    p3 = m_i * Vec()
    assert p.x == p3.x
    assert p.y == p3.y
    assert p.z == p3.z


def test_scale():
    m = Mat()
    m.scale(1, 2, 3)
    assert m[0, 0] == 1
    assert m[1, 1] == 2
    assert m[2, 2] == 3

    m.ident()
    m.scalev(Vec(-1, -2, -3))
    assert m[0, 0] == -1
    assert m[1, 1] == -2
    assert m[2, 2] == -3

    m_i = m.inverse()
    assert_float_equal(m_i[0, 0], -1)
    assert_float_equal(m_i[1, 1], -0.5)
    assert_float_equal(m_i[2, 2], -0.3, 1)


def test_rotate():
    # TODO: check resulting rotation matrix
    m = Mat()
    m.rotate(1, 0, 0, pi / 4)

    m.ident()
    m.rotatev(Vec(1, 0, 0), pi / 4)


def test_lookat():
    # TODO: check resulting look at matrix
    m = Mat()
    m.lookat(5, 10, 20, 0, 0, 0)

    m.ident()
    m.lookatv(Vec(5, 10, 20), Vec(0, 0, 0))


def test_ortho():
    # TODO: check resulting projection matrix
    m = Mat()
    m.ortho(-200, 200, 500, -500, 100, 1000)


def test_persp():
    # TODO: check resulting projection matrix
    m = Mat()
    m.persp(30.0, 16 / 9., 100, 1000)
