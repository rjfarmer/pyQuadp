# SPDX-License-Identifier: GPL-2.0+

import pytest
import numpy as np


@pytest.mark.qarray
class TestQArrayImport:
    def test_qarray_module_imports(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        assert qarray is not None

    def test_qarray_type_exported(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        assert hasattr(qarray, "qarray")


@pytest.mark.qarray
class TestQArrayConstructors:
    def test_zeros_constructor(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        arr = qarray.zeros(4)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (4,)
        assert arr.dtype == qarray.dtype
        assert arr.dtype.name == "qfloat"

    def test_ones_constructor(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        arr = qarray.ones(3)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (3,)
        assert arr.dtype == qarray.dtype

    def test_from_list_constructor(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        arr = qarray.from_list([1, 2.5, "3.5"])

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (3,)
        assert arr.dtype == qarray.dtype

    def test_from_array_constructor(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        src = np.array([[1.0, 2.0], [3.0, 4.0]], dtype=np.float64)
        arr = qarray.from_array(src)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (2, 2)
        assert arr.dtype == qarray.dtype


@pytest.mark.qarray
class TestQArrayInterop:
    def test_cast_qarray_to_float64(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        arr = qarray.from_list([1.25, 2.5, 3.75])

        out = np.asarray(arr, dtype=np.float64)
        assert out.dtype == np.float64
        assert np.allclose(out, np.array([1.25, 2.5, 3.75], dtype=np.float64))

    def test_cast_float64_to_qarray_dtype(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        src = np.array([1.25, 2.5, 3.75], dtype=np.float64)

        out = np.asarray(src, dtype=qarray.dtype)
        assert out.dtype == qarray.dtype
        assert float(out[0]) == pytest.approx(1.25)
        assert float(out[2]) == pytest.approx(3.75)

    def test_cast_float32_to_qarray_dtype(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        src = np.array([0.5, 1.5], dtype=np.float32)

        out = np.asarray(src, dtype=qarray.dtype)
        assert out.dtype == qarray.dtype
        assert float(out[0]) == pytest.approx(0.5)
        assert float(out[1]) == pytest.approx(1.5)


@pytest.mark.qarray
class TestQArrayUfuncs:
    def test_add_ufunc(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        a = qarray.from_list([1.0, 2.0, 3.0])
        b = qarray.from_list([0.5, 1.5, 2.5])

        out = np.add(a, b)
        assert out.dtype == qarray.dtype
        assert np.allclose(np.asarray(out, dtype=np.float64), [1.5, 3.5, 5.5])

    def test_subtract_multiply_divide_ufuncs(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        a = qarray.from_list([4.0, 6.0, 8.0])
        b = qarray.from_list([2.0, 3.0, 4.0])

        sub = np.subtract(a, b)
        mul = np.multiply(a, b)
        div = np.divide(a, b)

        assert sub.dtype == qarray.dtype
        assert mul.dtype == qarray.dtype
        assert div.dtype == qarray.dtype
        assert np.allclose(np.asarray(sub, dtype=np.float64), [2.0, 3.0, 4.0])
        assert np.allclose(np.asarray(mul, dtype=np.float64), [8.0, 18.0, 32.0])
        assert np.allclose(np.asarray(div, dtype=np.float64), [2.0, 2.0, 2.0])

    def test_add_ufunc_broadcasting(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        a = qarray.from_list([1.0, 2.0, 3.0])
        b = qarray.from_list([10.0])

        out = np.add(a, b)

        assert out.dtype == qarray.dtype
        assert np.allclose(np.asarray(out, dtype=np.float64), [11.0, 12.0, 13.0])

    def test_unary_positive_negative_ufuncs(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        a = qarray.from_list([1.0, -2.0, 3.0])

        pos = np.positive(a)
        neg = np.negative(a)

        assert pos.dtype == qarray.dtype
        assert neg.dtype == qarray.dtype
        assert np.allclose(np.asarray(pos, dtype=np.float64), [1.0, -2.0, 3.0])
        assert np.allclose(np.asarray(neg, dtype=np.float64), [-1.0, 2.0, -3.0])

    def test_absolute_and_square_ufuncs(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        a = qarray.from_list([-1.5, 2.0, -3.0])

        abs_out = np.absolute(a)
        sq_out = np.square(a)

        assert abs_out.dtype == qarray.dtype
        assert sq_out.dtype == qarray.dtype
        assert np.allclose(np.asarray(abs_out, dtype=np.float64), [1.5, 2.0, 3.0])
        assert np.allclose(np.asarray(sq_out, dtype=np.float64), [2.25, 4.0, 9.0])

    def test_sqrt_exp_log_sin_cos_ufuncs(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        a = qarray.from_list([0.25, 1.0, 4.0])

        sqrt_out = np.sqrt(a)
        exp_out = np.exp(a)
        log_out = np.log(a)
        sin_out = np.sin(a)
        cos_out = np.cos(a)

        assert sqrt_out.dtype == qarray.dtype
        assert exp_out.dtype == qarray.dtype
        assert log_out.dtype == qarray.dtype
        assert sin_out.dtype == qarray.dtype
        assert cos_out.dtype == qarray.dtype

        expected = np.array([0.25, 1.0, 4.0], dtype=np.float64)
        assert np.allclose(np.asarray(sqrt_out, dtype=np.float64), np.sqrt(expected))
        assert np.allclose(np.asarray(exp_out, dtype=np.float64), np.exp(expected))
        assert np.allclose(np.asarray(log_out, dtype=np.float64), np.log(expected))
        assert np.allclose(np.asarray(sin_out, dtype=np.float64), np.sin(expected))
        assert np.allclose(np.asarray(cos_out, dtype=np.float64), np.cos(expected))

    def test_mixed_qarray_float64_binary_ufuncs_promote_to_qarray(self):
        qarray = pytest.importorskip("pyquadp.qarray")
        q = qarray.from_list([2.0, 3.0, 4.0])
        d = np.array([0.5, 1.5, 2.0], dtype=np.float64)

        add_qd = np.add(q, d)
        add_dq = np.add(d, q)
        sub_qd = np.subtract(q, d)
        sub_dq = np.subtract(d, q)
        mul_qd = np.multiply(q, d)
        mul_dq = np.multiply(d, q)
        div_qd = np.divide(q, d)
        div_dq = np.divide(d, q)

        for out in [add_qd, add_dq, sub_qd, sub_dq, mul_qd, mul_dq, div_qd, div_dq]:
            assert out.dtype == qarray.dtype

        qf = np.array([2.0, 3.0, 4.0], dtype=np.float64)
        assert np.allclose(np.asarray(add_qd, dtype=np.float64), qf + d)
        assert np.allclose(np.asarray(add_dq, dtype=np.float64), d + qf)
        assert np.allclose(np.asarray(sub_qd, dtype=np.float64), qf - d)
        assert np.allclose(np.asarray(sub_dq, dtype=np.float64), d - qf)
        assert np.allclose(np.asarray(mul_qd, dtype=np.float64), qf * d)
        assert np.allclose(np.asarray(mul_dq, dtype=np.float64), d * qf)
        assert np.allclose(np.asarray(div_qd, dtype=np.float64), qf / d)
        assert np.allclose(np.asarray(div_dq, dtype=np.float64), d / qf)
