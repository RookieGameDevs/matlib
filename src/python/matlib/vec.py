"""Vector wrappers."""
from _matlib import ffi
from _matlib import lib


class Vec:
    """Vector."""

    def __init__(self, x=0, y=0, z=0, w=1, ptr=None):
        self._ptr = ptr or ffi.new('Vec*')
        self._ptr.data[0] = x
        self._ptr.data[1] = y
        self._ptr.data[2] = z
        self._ptr.data[3] = w

    @property
    def x(self):
        return self._ptr.data[0]

    @x.setter
    def x(self, x):
        self._ptr.data[0] = x

    @property
    def y(self):
        return self._ptr.data[1]

    @y.setter
    def y(self, y):
        self._ptr.data[1] = y

    @property
    def z(self):
        return self._ptr.data[2]

    @z.setter
    def z(self, z):
        self._ptr.data[2] = z

    @property
    def w(self):
        return self._ptr.data[3]

    @w.setter
    def w(self, w):
        self._ptr.data[3] = w

    def __add__(self, other):
        result = Vec()
        if isinstance(other, float) or isinstance(other, int):
            lib.vec_addf(self._ptr, float(other), result._ptr)
        else:
            lib.vec_add(self._ptr, other._ptr, result._ptr)
        return result

    def __iadd__(self, other):
        if isinstance(other, float) or isinstance(other, int):
            lib.vec_iaddf(self._ptr, float(other))
        else:
            lib.vec_iadd(self._ptr, other._ptr)
        return self

    def __sub__(self, other):
        result = Vec()
        if isinstance(other, float) or isinstance(other, int):
            lib.vec_subf(self._ptr, float(other), result._ptr)
        else:
            lib.vec_sub(self._ptr, other._ptr, result._ptr)
        return result

    def __isub__(self, other):
        if isinstance(other, float) or isinstance(other, int):
            lib.vec_isubf(self._ptr, float(other))
        else:
            lib.vec_isub(self._ptr, other._ptr)
        return self

    def __mul__(self, scalar):
        result = Vec()
        lib.vec_mulf(self._ptr, scalar, result._ptr)
        return result

    def __imul__(self, scalar):
        lib.vec_imulf(self._ptr, scalar)
        return self

    def mag(self):
        return lib.vec_mag(self._ptr)

    def norm(self):
        lib.vec_norm(self._ptr)

    def dot(self, other):
        return lib.vec_dot(self._ptr, other._ptr)

    def cross(self, other):
        result = Vec()
        lib.vec_cross(self._ptr, other._ptr, result._ptr)
        return result

    def lerp(self, other, t):
        result = Vec()
        lib.vec_lerp(self._ptr, other._ptr, t, result._ptr)
        return result
