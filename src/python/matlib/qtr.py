"""Quaternion wrappers."""
from _matlib import ffi
from _matlib import lib


class Qtr:
    """Quaternion."""

    def __init__(self, w=1, x=0, y=0, z=0, ptr=None):
        self._ptr = ptr or ffi.new('Qtr*')
        if not ptr:
            self.w = w
            self.x = x
            self.y = y
            self.z = z

    @property
    def w(self):
        return self._ptr.data[0]

    @w.setter
    def w(self, v):
        self._ptr.data[0] = v

    @property
    def x(self):
        return self._ptr.data[1]

    @x.setter
    def x(self, v):
        self._ptr.data[1] = v

    @property
    def y(self):
        return self._ptr.data[2]

    @y.setter
    def y(self, v):
        self._ptr.data[2] = v

    @property
    def z(self):
        return self._ptr.data[3]

    @z.setter
    def z(self, v):
        self._ptr.data[3] = v

    def rotate(self, x, y, z, angle):
        lib.qtr_rotate(self._ptr, float(x), float(y), float(z), float(angle))

    def rotatev(self, axis, angle):
        lib.qtr_rotatev(self._ptr, axis._ptr, float(angle))

    def norm(self):
        lib.qtr_norm(self._ptr)

    def lerp(self, other, t):
        result = Qtr()
        lib.qtr_lerp(self._ptr, other._ptr, t, result._ptr)
        return result

    def __add__(self, other):
        result = Qtr()
        lib.qtr_add(self._ptr, other._ptr, result._ptr)
        return result

    def __iadd__(self, other):
        lib.qtr_iadd(self._ptr, other._ptr)
        return self

    def __mul__(self, other):
        result = Qtr()
        if isinstance(other, float) or isinstance(other, int):
            lib.qtr_mulf(self._ptr, float(other), result._ptr)
        else:
            lib.qtr_mul(self._ptr, other._ptr, result._ptr)
        return result

    def __imul__(self, other):
        if isinstance(other, float) or isinstance(other, int):
            lib.qtr_imulf(self._ptr, float(other))
        else:
            lib.qtr_imul(self._ptr, other._ptr)
        return self
