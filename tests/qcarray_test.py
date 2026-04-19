# SPDX-License-Identifier: GPL-2.0+

import numpy as np
import pytest


@pytest.mark.qcarray
class TestQCArrayImport:
    def test_qcarray_module_imports(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        assert qcarray is not None

    def test_qcarray_type_exported(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        assert hasattr(qcarray, "qcarray")


@pytest.mark.qcarray
class TestQCArrayConstructors:
    def test_linspace_constructor_endpoint_true(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.linspace(1.0 + 2.0j, -1.0 + 0.5j, 5)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (5,)
        assert arr.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(arr, dtype=np.complex128),
            np.linspace(1.0 + 2.0j, -1.0 + 0.5j, 5),
        )

    def test_linspace_constructor_endpoint_false(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.linspace(1.0 + 2.0j, -1.0 + 0.5j, 4, False)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (4,)
        assert arr.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(arr, dtype=np.complex128),
            np.linspace(1.0 + 2.0j, -1.0 + 0.5j, 4, endpoint=False),
        )

    def test_empty_constructor(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.empty(4)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (4,)
        assert arr.dtype == qcarray.dtype

    def test_full_constructor(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.full(3, 1.5 - 0.25j)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (3,)
        assert arr.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(arr, dtype=np.complex128),
            np.array([1.5 - 0.25j, 1.5 - 0.25j, 1.5 - 0.25j], dtype=np.complex128),
        )

    def test_zeros_constructor(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.zeros(3)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (3,)
        assert arr.dtype == qcarray.dtype

    def test_ones_constructor(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.ones(2)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (2,)
        assert arr.dtype == qcarray.dtype

        out = np.asarray(arr, dtype=np.complex128)
        assert np.allclose(out, np.array([1 + 0j, 1 + 0j], dtype=np.complex128))

    def test_from_list_constructor(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.from_list([1 + 2j, complex(0.5, -1.25), 3 + 0j])

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (3,)
        assert arr.dtype == qcarray.dtype

        out = np.asarray(arr, dtype=np.complex128)
        assert np.allclose(
            out, np.array([1 + 2j, 0.5 - 1.25j, 3 + 0j], dtype=np.complex128)
        )

    def test_from_array_constructor(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        src = np.array(
            [[1.0 + 2.0j, -3.0 + 0.5j], [0.25 - 4.0j, 5.0 + 0.0j]], dtype=np.complex128
        )

        arr = qcarray.from_array(src)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (2, 2)
        assert arr.dtype == qcarray.dtype
        np.testing.assert_allclose(np.asarray(arr, dtype=np.complex128), src)

    def test_array_and_asarray_aliases(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        src = np.array([[1.0 + 2.0j, -3.0 + 0.5j]], dtype=np.complex128)

        arr1 = qcarray.array(src)
        arr2 = qcarray.asarray(src)

        assert arr1.dtype == qcarray.dtype
        assert arr2.dtype == qcarray.dtype
        assert arr1.shape == src.shape
        assert arr2.shape == src.shape

    def test_like_constructors(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        src = np.array([[1.0 + 2.0j, -3.0 + 0.5j]], dtype=np.complex128)

        empty = qcarray.empty_like(src)
        zeros = qcarray.zeros_like(src)
        ones = qcarray.ones_like(src)
        full = qcarray.full_like(src, "1.5-0.25j")

        assert empty.dtype == qcarray.dtype
        assert zeros.dtype == qcarray.dtype
        assert ones.dtype == qcarray.dtype
        assert full.dtype == qcarray.dtype
        assert empty.shape == src.shape
        assert zeros.shape == src.shape
        assert ones.shape == src.shape
        assert full.shape == src.shape

        np.testing.assert_allclose(
            np.asarray(zeros, dtype=np.complex128), np.zeros_like(src)
        )
        np.testing.assert_allclose(
            np.asarray(ones, dtype=np.complex128), np.ones_like(src)
        )
        np.testing.assert_allclose(
            np.asarray(full, dtype=np.complex128),
            np.full_like(src, 1.5 - 0.25j),
        )


@pytest.mark.qcarray
class TestQCArrayInterop:
    def test_cast_qcarray_to_complex128(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.from_list([1 + 2j, -3 + 0.25j])

        out = np.asarray(arr, dtype=np.complex128)
        assert out.dtype == np.complex128
        assert np.allclose(out, np.array([1 + 2j, -3 + 0.25j], dtype=np.complex128))

    def test_cast_complex128_to_qcarray_dtype(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        src = np.array([1.25 - 2j, 3 + 4j], dtype=np.complex128)

        out = np.asarray(src, dtype=qcarray.dtype)
        assert out.dtype == qcarray.dtype
        roundtrip = np.asarray(out, dtype=np.complex128)
        assert np.allclose(roundtrip, src)


@pytest.mark.qcarray
class TestQCArrayUfuncs:
    def test_binary_ufuncs(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        a = qcarray.from_list([1 + 1j, 2 - 3j])
        b = qcarray.from_list([0.5 - 0.5j, -1 + 2j])

        add_out = np.add(a, b)
        sub_out = np.subtract(a, b)
        mul_out = np.multiply(a, b)
        div_out = np.divide(a, b)
        pow_out = np.power(a, b)

        assert add_out.dtype == qcarray.dtype
        assert sub_out.dtype == qcarray.dtype
        assert mul_out.dtype == qcarray.dtype
        assert div_out.dtype == qcarray.dtype
        assert pow_out.dtype == qcarray.dtype

        a128 = np.asarray(a, dtype=np.complex128)
        b128 = np.asarray(b, dtype=np.complex128)
        assert np.allclose(np.asarray(add_out, dtype=np.complex128), a128 + b128)
        assert np.allclose(np.asarray(sub_out, dtype=np.complex128), a128 - b128)
        assert np.allclose(np.asarray(mul_out, dtype=np.complex128), a128 * b128)
        assert np.allclose(np.asarray(div_out, dtype=np.complex128), a128 / b128)
        assert np.allclose(
            np.asarray(pow_out, dtype=np.complex128), np.power(a128, b128)
        )

    def test_mixed_with_complex128(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        q = qcarray.from_list([2 + 1j, -3 + 0.5j])
        c = np.array([1 - 4j, 0.25 + 0.75j], dtype=np.complex128)

        out_qc = np.add(q, c)
        out_cq = np.add(c, q)

        assert out_qc.dtype == qcarray.dtype
        assert out_cq.dtype == qcarray.dtype
        assert np.allclose(
            np.asarray(out_qc, dtype=np.complex128),
            np.asarray(q, dtype=np.complex128) + c,
        )
        assert np.allclose(
            np.asarray(out_cq, dtype=np.complex128),
            c + np.asarray(q, dtype=np.complex128),
        )

    @pytest.mark.parametrize(
        ("ufunc", "expected_qc", "expected_cq"),
        [
            (np.add, lambda q, c: q + c, lambda q, c: c + q),
            (np.subtract, lambda q, c: q - c, lambda q, c: c - q),
            (np.multiply, lambda q, c: q * c, lambda q, c: c * q),
            (np.divide, lambda q, c: q / c, lambda q, c: c / q),
            (np.power, lambda q, c: np.power(q, c), lambda q, c: np.power(c, q)),
        ],
    )
    def test_mixed_complex128_binary_ufuncs(self, ufunc, expected_qc, expected_cq):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        q = qcarray.from_list([2 + 1j, -3 + 0.5j, 0.25 - 2j])
        c = np.array([1 - 4j, 0.25 + 0.75j, -2 + 0.5j], dtype=np.complex128)

        out_qc = ufunc(q, c)
        out_cq = ufunc(c, q)

        assert out_qc.dtype == qcarray.dtype
        assert out_cq.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(out_qc, dtype=np.complex128),
            expected_qc(np.asarray(q, dtype=np.complex128), c),
        )
        np.testing.assert_allclose(
            np.asarray(out_cq, dtype=np.complex128),
            expected_cq(np.asarray(q, dtype=np.complex128), c),
        )

    @pytest.mark.parametrize(
        "ufunc", [np.add, np.subtract, np.multiply, np.divide, np.power]
    )
    def test_mixed_complex128_scalar_binary_ufuncs(self, ufunc):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        q = qcarray.from_list([2 + 1j, -3 + 0.5j, 0.25 - 2j])
        scalar = np.complex128(1.5 - 0.25j)
        q128 = np.asarray(q, dtype=np.complex128)

        out_qs = ufunc(q, scalar)
        out_sq = ufunc(scalar, q)

        assert out_qs.dtype == qcarray.dtype
        assert out_sq.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(out_qs, dtype=np.complex128),
            ufunc(q128, scalar),
        )
        np.testing.assert_allclose(
            np.asarray(out_sq, dtype=np.complex128),
            ufunc(scalar, q128),
        )

    @pytest.mark.parametrize(
        "ufunc", [np.add, np.subtract, np.multiply, np.divide, np.power]
    )
    def test_mixed_complex128_broadcasting_binary_ufuncs(self, ufunc):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        q = qcarray.from_list([2 + 1j, -3 + 0.5j, 0.25 - 2j])
        c = np.array([1.5 - 0.25j], dtype=np.complex128)
        q128 = np.asarray(q, dtype=np.complex128)

        out_qc = ufunc(q, c)
        out_cq = ufunc(c, q)

        assert out_qc.dtype == qcarray.dtype
        assert out_cq.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(out_qc, dtype=np.complex128), ufunc(q128, c)
        )
        np.testing.assert_allclose(
            np.asarray(out_cq, dtype=np.complex128), ufunc(c, q128)
        )

    def test_binary_ufuncs_broadcasting(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        left = qcarray.from_list([1 + 1j, 2 - 3j, -4 + 0.5j])
        right = qcarray.from_list([0.5 - 0.25j])

        out = np.add(left, right)

        assert out.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(out, dtype=np.complex128),
            np.asarray(left, dtype=np.complex128)
            + np.asarray(right, dtype=np.complex128),
        )

    def test_unary_sign_ufuncs(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        q = qcarray.from_list([1 - 2j, -3 + 4j])

        pos = np.positive(q)
        neg = np.negative(q)

        assert pos.dtype == qcarray.dtype
        assert neg.dtype == qcarray.dtype
        q128 = np.asarray(q, dtype=np.complex128)
        assert np.allclose(np.asarray(pos, dtype=np.complex128), +q128)
        assert np.allclose(np.asarray(neg, dtype=np.complex128), -q128)

    def test_conjugate_ufunc(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        q = qcarray.from_list([1 - 2j, -3 + 4j, 0.5 + 0j])

        out = np.conjugate(q)

        assert out.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(out, dtype=np.complex128),
            np.conjugate(np.asarray(q, dtype=np.complex128)),
        )

    def test_unary_math_ufuncs(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        qarray = pytest.importorskip("pyquadp.qarray")
        q = qcarray.from_list([0.25 + 0.5j, 1.0 + 0j, -0.75 + 0.25j])
        q128 = np.asarray(q, dtype=np.complex128)

        abs_out = np.absolute(q)
        sq_out = np.square(q)
        sqrt_out = np.sqrt(q)
        exp_out = np.exp(q)
        log_out = np.log(q)
        sin_out = np.sin(q)
        cos_out = np.cos(q)
        tan_out = np.tan(q)
        sinh_out = np.sinh(q)
        cosh_out = np.cosh(q)

        assert abs_out.dtype == qarray.dtype
        assert sq_out.dtype == qcarray.dtype
        assert sqrt_out.dtype == qcarray.dtype
        assert exp_out.dtype == qcarray.dtype
        assert log_out.dtype == qcarray.dtype
        assert sin_out.dtype == qcarray.dtype
        assert cos_out.dtype == qcarray.dtype
        assert tan_out.dtype == qcarray.dtype
        assert sinh_out.dtype == qcarray.dtype
        assert cosh_out.dtype == qcarray.dtype

        abs_expected = np.abs(q128)
        assert np.allclose(np.asarray(abs_out, dtype=np.float64), abs_expected)
        assert np.allclose(np.asarray(sq_out, dtype=np.complex128), np.square(q128))
        assert np.allclose(np.asarray(sqrt_out, dtype=np.complex128), np.sqrt(q128))
        assert np.allclose(np.asarray(exp_out, dtype=np.complex128), np.exp(q128))
        assert np.allclose(np.asarray(log_out, dtype=np.complex128), np.log(q128))
        assert np.allclose(np.asarray(sin_out, dtype=np.complex128), np.sin(q128))
        assert np.allclose(np.asarray(cos_out, dtype=np.complex128), np.cos(q128))
        assert np.allclose(np.asarray(tan_out, dtype=np.complex128), np.tan(q128))
        assert np.allclose(np.asarray(sinh_out, dtype=np.complex128), np.sinh(q128))
        assert np.allclose(np.asarray(cosh_out, dtype=np.complex128), np.cosh(q128))

    def test_ufunc_out_parameter_for_qcarray_outputs(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        q = qcarray.from_list([0.25 + 0.5j, 1.0 + 0j, -0.75 + 0.25j])
        q128 = np.asarray(q, dtype=np.complex128)

        out_sin = qcarray.zeros(3)
        returned = np.sin(q, out=out_sin)

        assert returned is out_sin
        assert out_sin.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(out_sin, dtype=np.complex128), np.sin(q128)
        )

    def test_ufunc_out_parameter_for_absolute_qarray_output(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        qarray = pytest.importorskip("pyquadp.qarray")
        q = qcarray.from_list([3 + 4j, 5 + 12j, -8 + 15j])

        out_abs = qarray.zeros(3)
        returned = np.absolute(q, out=out_abs)

        assert returned is out_abs
        assert out_abs.dtype == qarray.dtype
        np.testing.assert_allclose(
            np.asarray(out_abs, dtype=np.float64),
            np.abs(np.asarray(q, dtype=np.complex128)),
        )

    def test_absolute_returns_qarray_after_mixed_arithmetic(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        qarray = pytest.importorskip("pyquadp.qarray")

        left = qcarray.from_list([1 + 2j, -3 + 4j, 0.5 - 0.25j])
        right = np.array([0.5 - 1j, 1.5 + 0.5j, -2 + 0j], dtype=np.complex128)

        mixed = np.add(left, right)
        abs_out = np.absolute(mixed)

        expected = np.abs(np.asarray(left, dtype=np.complex128) + right)

        assert mixed.dtype == qcarray.dtype
        assert abs_out.dtype == qarray.dtype
        assert np.allclose(np.asarray(abs_out, dtype=np.float64), expected)


@pytest.mark.qcarray
class TestQCArrayHardening:
    def test_nan_propagates_through_add(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        left = qcarray.from_list([1 + 0j, complex(np.nan, 2.0), -3 + 4j])
        right = qcarray.from_list([2 - 1j, 1 + 1j, 0.5 - 0.5j])

        out = np.add(left, right)

        np.testing.assert_allclose(
            np.asarray(out, dtype=np.complex128),
            np.asarray(left, dtype=np.complex128)
            + np.asarray(right, dtype=np.complex128),
            equal_nan=True,
        )

    def test_inf_propagates_through_multiply(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        left = qcarray.from_list([1 + 0j, complex(np.inf, 0.0), complex(-1.0, np.inf)])
        right = qcarray.from_list([2 - 1j, 0.5 + 0j, 3 + 2j])

        with pytest.warns(RuntimeWarning, match="invalid value"):
            out = np.multiply(left, right)
        with pytest.warns(RuntimeWarning, match="invalid value"):
            expected = np.asarray(left, dtype=np.complex128) * np.asarray(
                right, dtype=np.complex128
            )

        np.testing.assert_allclose(
            np.asarray(out, dtype=np.complex128),
            expected,
            equal_nan=True,
        )

    def test_absolute_of_nan_returns_nan_qarray(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        qarray = pytest.importorskip("pyquadp.qarray")
        arr = qcarray.from_list([complex(np.nan, 1.0), 3 - 4j])

        out = np.absolute(arr)
        with pytest.warns(RuntimeWarning, match="invalid value"):
            vals = np.asarray(out, dtype=np.float64)

        assert out.dtype == qarray.dtype
        assert np.isnan(vals[0])
        assert vals[1] == pytest.approx(5.0)

    def test_noncontiguous_add(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        base = qcarray.from_list([0 + 0j, 1 - 1j, 2 + 2j, 3 - 3j, 4 + 4j, 5 - 5j])
        left = base[::2]
        right = base[1::2]

        out = np.add(left, right)

        assert out.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(out, dtype=np.complex128),
            np.asarray(left, dtype=np.complex128)
            + np.asarray(right, dtype=np.complex128),
        )

    def test_noncontiguous_unary(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        base = qcarray.from_list([1 + 2j, -2 - 3j, 4 + 5j, -6 + 7j])
        sliced = base[1::2]

        out = np.negative(sliced)

        assert out.dtype == qcarray.dtype
        np.testing.assert_allclose(
            np.asarray(out, dtype=np.complex128),
            -np.asarray(sliced, dtype=np.complex128),
        )

    def test_2d_array_ufunc(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        src = np.array([[1 + 1j, 2 - 2j], [-3 + 0.5j, 4 + 4j]], dtype=np.complex128)
        left = qcarray.from_array(src)
        right = qcarray.from_array(src * (1 - 0.5j))

        out = np.add(left, right)

        assert out.dtype == qcarray.dtype
        assert out.shape == (2, 2)
        np.testing.assert_allclose(
            np.asarray(out, dtype=np.complex128),
            np.asarray(left, dtype=np.complex128)
            + np.asarray(right, dtype=np.complex128),
        )

    def test_setitem_and_getitem_nan_inf(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.zeros(3)
        arr[0] = complex(np.nan, 1.0)
        arr[1] = complex(np.inf, -np.inf)
        arr[2] = complex(-np.inf, 0.0)

        values = np.asarray(arr, dtype=np.complex128)
        assert np.isnan(values[0].real)
        assert values[0].imag == pytest.approx(1.0)
        assert np.isposinf(values[1].real)
        assert np.isneginf(values[1].imag)
        assert np.isneginf(values[2].real)
        assert values[2].imag == pytest.approx(0.0)

    def test_argmax_uses_complex_magnitude(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.from_list([1 + 1j, 2 + 0j, 1 + 3j, -2 - 2j])

        idx = np.argmax(arr)

        assert idx == 2

    def test_argmax_returns_first_nan(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.from_list(
            [1 + 1j, complex(np.nan, 0.0), complex(0.0, np.nan), 10 + 0j]
        )

        idx = np.argmax(arr)

        assert idx == 1

    def test_sort_and_argsort_match_complex128(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        values = np.array(
            [1 + 2j, 1 + 1j, 0 + 5j, -1 + 10j, -1 + 2j, complex(np.nan, 0.0)],
            dtype=np.complex128,
        )
        arr = qcarray.from_array(values)

        np.testing.assert_array_equal(np.argsort(arr), np.argsort(values))
        np.testing.assert_allclose(
            np.asarray(np.sort(arr), dtype=np.complex128),
            np.sort(values),
            equal_nan=True,
        )

    def test_argmin_uses_lexicographic_order(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.from_list([1 + 2j, 1 + 1j, 0 + 5j, -1 + 10j, -1 + 2j])

        idx = np.argmin(arr)

        assert idx == 4

    def test_argmin_returns_first_nan(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        arr = qcarray.from_list(
            [1 + 1j, complex(np.nan, 0.0), complex(0.0, np.nan), -10 + 0j]
        )

        idx = np.argmin(arr)

        assert idx == 1

    def test_min_and_max_match_complex128(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        values = np.array(
            [1 + 2j, 1 + 1j, 0 + 5j, -1 + 10j, -1 + 2j, complex(np.nan, 0.0)],
            dtype=np.complex128,
        )
        arr = qcarray.from_array(values)

        min_out = np.min(arr)
        max_out = np.max(arr)

        np.testing.assert_allclose(
            np.array([complex(min_out)], dtype=np.complex128),
            np.array([np.min(values)], dtype=np.complex128),
            equal_nan=True,
        )
        np.testing.assert_allclose(
            np.array([complex(max_out)], dtype=np.complex128),
            np.array([np.max(values)], dtype=np.complex128),
            equal_nan=True,
        )

    def test_axis_ordering_reductions_match_complex128(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        values = np.array(
            [[1 + 2j, -1 + 10j, complex(np.nan, 0.0)], [1 + 1j, -1 + 2j, 3 + 0j]],
            dtype=np.complex128,
        )
        arr = qcarray.from_array(values)

        np.testing.assert_allclose(
            np.asarray(np.min(arr, axis=0), dtype=np.complex128),
            np.min(values, axis=0),
            equal_nan=True,
        )
        np.testing.assert_allclose(
            np.asarray(np.min(arr, axis=1), dtype=np.complex128),
            np.min(values, axis=1),
            equal_nan=True,
        )
        np.testing.assert_allclose(
            np.asarray(np.max(arr, axis=0), dtype=np.complex128),
            np.max(values, axis=0),
            equal_nan=True,
        )
        np.testing.assert_allclose(
            np.asarray(np.max(arr, axis=1), dtype=np.complex128),
            np.max(values, axis=1),
            equal_nan=True,
        )
        np.testing.assert_array_equal(np.argmin(arr, axis=0), np.argmin(values, axis=0))
        np.testing.assert_array_equal(np.argmin(arr, axis=1), np.argmin(values, axis=1))
        np.testing.assert_array_equal(np.argmax(arr, axis=0), np.argmax(values, axis=0))
        np.testing.assert_array_equal(np.argmax(arr, axis=1), np.argmax(values, axis=1))
        np.testing.assert_array_equal(
            np.argsort(arr, axis=1), np.argsort(values, axis=1)
        )

    def test_sort_along_axes_matches_complex128(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        values = np.array(
            [
                [1 + 2j, -1 + 10j, complex(np.nan, 0.0)],
                [1 + 1j, -1 + 2j, 3 + 0j],
                [0 + 5j, -2 + 3j, -4 + 9j],
            ],
            dtype=np.complex128,
        )
        arr = qcarray.from_array(values)

        np.testing.assert_allclose(
            np.asarray(np.sort(arr, axis=0), dtype=np.complex128),
            np.sort(values, axis=0),
            equal_nan=True,
        )
        np.testing.assert_allclose(
            np.asarray(np.sort(arr, axis=1), dtype=np.complex128),
            np.sort(values, axis=1),
            equal_nan=True,
        )
        np.testing.assert_array_equal(
            np.argsort(arr, axis=0),
            np.argsort(values, axis=0),
        )

    def test_ordering_reductions_on_mixed_view_match_complex128(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        left = qcarray.from_list([1 + 2j, -3 + 4j, 0.5 - 0.25j, -2 + 3j])
        right = np.array(
            [0.5 - 1j, 1.5 + 0.5j, -2 + 0j, 0.25 - 0.75j],
            dtype=np.complex128,
        )

        mixed = np.add(left, right)
        view = mixed[::-1]
        expected = np.asarray(view, dtype=np.complex128)

        assert complex(np.min(view)) == complex(np.min(expected))
        assert complex(np.max(view)) == complex(np.max(expected))
        np.testing.assert_array_equal(np.argmin(view), np.argmin(expected))
        np.testing.assert_array_equal(np.argmax(view), np.argmax(np.abs(expected)))
        np.testing.assert_array_equal(np.argsort(view), np.argsort(expected))

    def test_reduction_out_parameter_matches_complex128(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        values = np.array(
            [
                [1 + 2j, -1 + 10j, 2 + 0j],
                [1 + 1j, -1 + 2j, 3 + 0j],
            ],
            dtype=np.complex128,
        )
        arr = qcarray.from_array(values)

        out_min = qcarray.zeros(3)
        out_max = qcarray.zeros(3)

        ret_min = np.min(arr, axis=0, out=out_min)
        ret_max = np.max(arr, axis=0, out=out_max)

        assert ret_min is out_min
        assert ret_max is out_max
        np.testing.assert_allclose(
            np.asarray(out_min, dtype=np.complex128),
            np.min(values, axis=0),
        )
        np.testing.assert_allclose(
            np.asarray(out_max, dtype=np.complex128),
            np.max(values, axis=0),
        )

    def test_mixed_broadcast_reduce_parity(self):
        qcarray = pytest.importorskip("pyquadp.qcarray")
        left = qcarray.from_list([1 + 2j, -3 + 4j, 0.5 - 0.25j])
        right = np.array([0.25 - 1j, -2 + 0.5j], dtype=np.complex128)

        mixed = np.multiply(left[:, None], right[None, :]) + (1 - 0.5j)
        expected = np.asarray(left, dtype=np.complex128)[:, None] * right[None, :] + (
            1 - 0.5j
        )

        np.testing.assert_allclose(
            np.asarray(np.min(mixed, axis=0), dtype=np.complex128),
            np.min(expected, axis=0),
        )
        np.testing.assert_allclose(
            np.asarray(np.max(mixed, axis=1), dtype=np.complex128),
            np.max(expected, axis=1),
        )
        np.testing.assert_array_equal(
            np.argmin(mixed, axis=0), np.argmin(expected, axis=0)
        )
        np.testing.assert_array_equal(
            np.argmax(mixed, axis=1), np.argmax(np.abs(expected), axis=1)
        )
        np.testing.assert_array_equal(
            np.argsort(mixed, axis=1), np.argsort(expected, axis=1)
        )
