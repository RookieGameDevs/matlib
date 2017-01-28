"""Matrix wrappers."""
from _matlib import ffi
from _matlib import lib
from matlib.vec import Vec


class Mat:
    """Matrix 4x4."""

    def __init__(self, ptr=None):
        self._ptr = ptr or ffi.new('Mat*')
        if not ptr:
            self.ident()

    def ident(self):
        lib.mat_ident(self._ptr)

    def inverse(self):
        result = Mat()
        lib.mat_inverse(self._ptr, result._ptr)
        return result

    def translate(self, tx, ty, tz):
        lib.mat_translate(self._ptr, float(tx), float(ty), float(tz))

    def translatev(self, tv):
        lib.mat_translatev(self._ptr, tv._ptr)

    def rotate(self, x, y, z, angle):
        lib.mat_rotate(self._ptr, float(x), float(y), float(z), float(angle))

    def rotatev(self, axis, angle):
        lib.mat_rotatev(self._ptr, axis._ptr, float(angle))

    def rotateq(self, rq):
        lib.mat_rotateq(self._ptr, rq._ptr)

    def scale(self, sx, sy, sz):
        lib.mat_scale(self._ptr, float(sx), float(sy), float(sz))

    def scalev(self, sv):
        lib.mat_scalev(self._ptr, sv._ptr)

    def lookat(self, eye_x, eye_y, eye_z, center_x, center_y, center_z,
            up_x=0, up_y=1, up_z=0):
        lib.mat_lookat(
            self._ptr,
            float(eye_x),
            float(eye_y),
            float(eye_z),
            float(center_x),
            float(center_y),
            float(center_z),
            float(up_x),
            float(up_y),
            float(up_z))

    def lookatv(self, eye, center, up=None):
        up = up or Vec(1, 0, 0)
        lib.mat_lookatv(self._ptr, eye._ptr, center._ptr, up._ptr)

    def ortho(self, left, right, top, bottom, near, far):
        lib.mat_ortho(
            self._ptr,
            float(left),
            float(right),
            float(top),
            float(bottom),
            float(near),
            float(far))

    def persp(self, fovy, aspect, near, far):
        lib.mat_persp(
            self._ptr,
            float(fovy),
            float(aspect),
            float(near),
            float(far))

    def __mul__(self, other):
        if isinstance(other, Vec):
            result = Vec()
            lib.mat_mulv(self._ptr, other._ptr, result._ptr)
            return result

        result = Mat()
        lib.mat_mul(self._ptr, other._ptr, result._ptr)
        return result

    def __imul__(self, other):
        lib.mat_imul(self._ptr, other._ptr)
        return self

    def __getitem__(self, index):
        d = self._ptr.data

        try:
            if isinstance(index, slice):
                if index.start is not None:
                    start = index.start
                    stop = 4 if index.stop is None else index.stop
                    if not 0 <= start <= 4 or not 0 <= stop <= 4:
                        raise IndexError
                    return [d[start * 4 + c] for c in range(stop)]

                elif index.stop is not None:
                    if not 0 <= index.stop <= 4:
                        raise IndexError
                    stop = max(index.stop, 3)
                    return [d[r * 4 + index.stop] for r in range(4)]

                return [d[i] for i in range(16)]

            elif isinstance(index, tuple):
                row, col = index
                if not 0 <= row < 4 or not 0 <= col < 4:
                    raise IndexError
                return d[row * 4 + col]

            else:
                raise ValueError

        except IndexError:
            raise IndexError('index {} out of bounds'.format(index))
        except (TypeError, ValueError, AttributeError):
            raise IndexError(
                'invalid index {}\n'
                'matrix indices must be either slices or (m,n) tuples'.format(index))
