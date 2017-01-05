"""Vector wrappers."""
from _matlib import ffi
from _matlib import lib


class Vec:
    """Vector."""

    def __init__(self, x=0, y=0, z=0, w=1):
        self._vec = ffi.new('Vec*')
        self._vec.data[0] = x
        self._vec.data[1] = y
        self._vec.data[2] = z
        self._vec.data[3] = w

    @property
    def x(self):
        return self._vec.data[0]

    @x.setter
    def x(self, x):
        self._vec.data[0] = x

    @property
    def y(self):
        return self._vec.data[1]

    @y.setter
    def y(self, y):
        self._vec.data[1] = y

    @property
    def z(self):
        return self._vec.data[2]

    @z.setter
    def z(self, z):
        self._vec.data[2] = z

    @property
    def w(self):
        return self._vec.data[3]

    @w.setter
    def w(self, w):
        self._vec.data[3] = w

    def __add__(self, other):
        result = Vec()
        if isinstance(other, float) or isinstance(other, int):
            lib.vec_addf(self._vec, float(other), result._vec)
        else:
            lib.vec_add(self._vec, other._vec, result._vec)
        return result

    def __iadd__(self, other):
        if isinstance(other, float) or isinstance(other, int):
            lib.vec_iaddf(self._vec, float(other))
        else:
            lib.vec_iadd(self._vec, other._vec)
        return self

    def __sub__(self, other):
        result = Vec()
        if isinstance(other, float) or isinstance(other, int):
            lib.vec_subf(self._vec, float(other), result._vec)
        else:
            lib.vec_sub(self._vec, other._vec, result._vec)
        return result

    def __isub__(self, other):
        if isinstance(other, float) or isinstance(other, int):
            lib.vec_isubf(self._vec, float(other))
        else:
            lib.vec_isub(self._vec, other._vec)
        return self

    def __mul__(self, scalar):
        result = Vec()
        lib.vec_mulf(self._vec, scalar, result._vec)
        return result

    def __imul__(self, scalar):
        lib.vec_imulf(self._vec, scalar)
        return self

    def mag(self):
        return lib.vec_mag(self._vec)

    def norm(self):
        lib.vec_norm(self._vec)

    def dot(self, other):
        return lib.vec_dot(self._vec, other._vec)

    def cross(self, other):
        result = Vec()
        lib.vec_cross(self._vec, other._vec, result._vec)
        return result

    def lerp(self, other, t):
        result = Vec()
        lib.vec_lerp(self._vec, other._vec, t, result._vec)
        return result