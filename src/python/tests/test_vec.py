from .util import assert_float_equal
from .util import assert_vec_equal
from matlib import Vec
import pytest


def test_init():
    v = Vec(1, 2, 3, 4)
    assert v.x == 1
    assert v.y == 2
    assert v.z == 3
    assert v.w == 4

    v.x = 123.456
    v.y = v.x
    assert_float_equal(v.x, 123.456)
    assert_float_equal(v.y, 123.456)
    assert_float_equal(v.y, v.x)

    v.x = v.y = v.z = v.w = 1
    assert sum(v._vec.data) == 4


@pytest.mark.parametrize('a,b,result', [
    ((1, 2, 3, 4), (4, 3, 2, 1), (-3, -1, 1, 3)),
    ((1.1, 2.2, 3.3, 4.4), (4, 3, 2, 1), (-2.9, -0.8, 1.3, 3.4)),
])
def test_sub(a, b, result):
    va = Vec(*a)
    vb = Vec(*b)
    assert_vec_equal(va - vb, Vec(*result))

    # in-place variant
    va -= vb
    assert_vec_equal(va, Vec(*result))


@pytest.mark.parametrize('v,scalar,result', [
    ((1, 2, 3, 4), 100, (-99, -98, -97, -96)),
    ((-0.1, -0.2, -0.3, -0.4), -1, (0.9, 0.8, 0.7, 0.6)),
])
def test_subf(v, scalar, result):
    x, y, z, w = v
    v = Vec(x, y, z, w)
    r = v - scalar
    assert_vec_equal(r, Vec(*result))

    v -= scalar
    assert_vec_equal(v, Vec(*result))


@pytest.mark.parametrize('a,b,result', [
    ((1, 2, 3, 4), (4, 3, 2, 1), (5, 5, 5, 5)),
    ((1.1, 2.2, 3.3, 4.4), (4, 3, 2, 1), (5.1, 5.2, 5.3, 5.4)),
])
def test_add(a, b, result):
    va = Vec(*a)
    vb = Vec(*b)
    assert_vec_equal(va + vb, Vec(*result))

    # in-place variant
    va += vb
    assert_vec_equal(va, Vec(*result))


@pytest.mark.parametrize('v,scalar,result', [
    ((1, 2, 3, 4), 100, (101, 102, 103, 104)),
    ((-0.1, -0.2, -0.3, -0.4), -1, (-1.1, -1.2, -1.3, -1.4)),
])
def test_addf(v, scalar, result):
    x, y, z, w = v
    v = Vec(x, y, z, w)
    r = v + scalar
    assert_vec_equal(r, Vec(*result))

    v += scalar
    assert_vec_equal(v, Vec(*result))


@pytest.mark.parametrize('v,scalar,result', [
    ((1, 2, 3, 4), 100, (100, 200, 300, 400)),
    ((-0.1, -0.2, -0.3, -0.4), -1, (0.1, 0.2, 0.3, 0.4)),
])
def test_mulf(v, scalar, result):
    x, y, z, w = v
    v = Vec(x, y, z, w)
    r = v * scalar
    assert_vec_equal(r, Vec(*result))

    v *= scalar
    assert_vec_equal(v, Vec(*result))


def test_mag():
    x = Vec(1, 0, 0)
    assert x.mag() == 1

    y = Vec(0, 2, 0)
    assert y.mag() == 2

    z = Vec(0, 0, 3)
    assert z.mag() == 3


def test_cross():
    x = Vec(1, 0, 0, 0)
    y = Vec(0, 1, 0, 0)
    z = x.cross(y)
    assert z.mag() == 1
    assert_vec_equal(z, Vec(0, 0, 1, 0))


def test_dot():
    x = Vec(1, 0, 0, 0)
    x1 = Vec(0.5, 0, 0, 0)
    y = Vec(0, 1, 0, 0)

    assert_float_equal(x.dot(x1), 0.5)
    assert_float_equal(x.dot(y), 0)


def test_norm():
    x = Vec(20, 0, 0, 0)
    x.norm()
    assert x.mag() == 1

    v = Vec(0.7071, 0.7071, 0)
    v.norm()
    assert_float_equal(v.mag(), 1)


def test_lerp():
    v1 = Vec(20, 10, 5)
    v2 = Vec()
    vt = v1.lerp(v2, 0.5)
    assert_vec_equal(vt, Vec(10, 5, 2.5))
