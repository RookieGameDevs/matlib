def assert_float_equal(a, b, prec=4):
    assert abs(round(a, prec) - round(b, prec)) < 1e-6


def assert_vec_equal(a, b):
    assert_float_equal(a.x, b.x)
    assert_float_equal(a.y, b.y)
    assert_float_equal(a.z, b.z)
    assert_float_equal(a.w, b.w)
