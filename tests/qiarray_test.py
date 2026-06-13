# SPDX-License-Identifier: GPL-2.0+

import numpy as np
import pytest

import pyquadp.qiarray as qiarray

FIXED_WIDTH_DTYPES = [
    np.int8,
    np.int16,
    np.int32,
    np.int64,
]


@pytest.mark.qiarray
class TestQIArrayImport:
    def test_qiarray_module_imports(self):

        assert qiarray is not None

    def test_qiarray_type_exported(self):

        assert hasattr(qiarray, "qiarray")


@pytest.mark.qiarray
class TestQIArrayConstructors:
    def test_arange_stop_constructor(self):

        arr = qiarray.arange(4)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (4,)
        assert arr.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(arr, dtype=np.int64), np.arange(4, dtype=np.int64)
        )

    def test_arange_start_stop_step_constructor(self):

        arr = qiarray.arange(5, -1, -2)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (3,)
        assert arr.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(arr, dtype=np.int64), np.array([5, 3, 1], dtype=np.int64)
        )

    def test_empty_full_zeros_ones_constructors(self):

        empty = qiarray.empty(5)
        full = qiarray.full(4, "-2")
        zeros = qiarray.zeros(4)
        ones = qiarray.ones(3)

        assert empty.shape == (5,)
        assert full.dtype == qiarray.dtype
        assert zeros.dtype == qiarray.dtype
        assert ones.dtype == qiarray.dtype
        assert zeros.dtype.name == "qint"
        np.testing.assert_array_equal(
            np.asarray(full, dtype=np.int64), np.array([-2, -2, -2, -2], dtype=np.int64)
        )
        np.testing.assert_array_equal(
            np.asarray(zeros, dtype=np.int64), np.zeros(4, dtype=np.int64)
        )
        np.testing.assert_array_equal(
            np.asarray(ones, dtype=np.int64), np.ones(3, dtype=np.int64)
        )

    def test_shape_tuple_constructors(self):

        zeros = qiarray.zeros((2, 3))
        ones = qiarray.ones((2, 1, 2))
        full = qiarray.full((2, 2), "-7")

        assert zeros.shape == (2, 3)
        assert ones.shape == (2, 1, 2)
        assert full.shape == (2, 2)
        np.testing.assert_array_equal(
            np.asarray(zeros, dtype=np.int64), np.zeros((2, 3), dtype=np.int64)
        )
        np.testing.assert_array_equal(
            np.asarray(ones, dtype=np.int64), np.ones((2, 1, 2), dtype=np.int64)
        )
        np.testing.assert_array_equal(
            np.asarray(full, dtype=np.int64), np.full((2, 2), -7, dtype=np.int64)
        )

    def test_from_list_constructor(self):

        arr = qiarray.from_list([1, "2", -3])

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (3,)
        assert arr.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(arr, dtype=np.int64), np.array([1, 2, -3], dtype=np.int64)
        )

    def test_from_list_accepts_numpy_integer_scalars(self):

        arr = qiarray.from_list([np.int64(1), np.int32(-2), np.int64(3)])

        assert isinstance(arr, np.ndarray)
        assert arr.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(arr, dtype=np.int64), np.array([1, -2, 3], dtype=np.int64)
        )

    def test_setitem_accepts_numpy_integer_scalars(self):

        arr = qiarray.zeros(3)

        arr[0] = np.int64(9)
        arr[1] = np.int32(-4)
        arr[2] = np.int64(7)

        np.testing.assert_array_equal(
            np.asarray(arr, dtype=np.int64), np.array([9, -4, 7], dtype=np.int64)
        )

    def test_from_array_constructor(self):

        src = np.array([[1, 2], [3, 4]], dtype=np.int64)
        arr = qiarray.from_array(src)

        assert isinstance(arr, np.ndarray)
        assert arr.shape == (2, 2)
        assert arr.dtype == qiarray.dtype
        np.testing.assert_array_equal(np.asarray(arr, dtype=np.int64), src)

    def test_array_and_asarray_aliases(self):

        src = np.array([[1, 2], [3, 4]], dtype=np.int64)

        arr1 = qiarray.array(src)
        arr2 = qiarray.asarray(src)

        assert arr1.dtype == qiarray.dtype
        assert arr2.dtype == qiarray.dtype
        assert arr1.shape == src.shape
        assert arr2.shape == src.shape

    def test_asfortranarray_returns_fortran_contiguous(self):

        src = np.array([[1, 2], [3, 4]], dtype=np.int64)

        arr = qiarray.asfortranarray(src)

        assert arr.dtype == qiarray.dtype
        assert arr.shape == src.shape
        assert arr.flags["F_CONTIGUOUS"]
        assert not arr.flags["C_CONTIGUOUS"]
        np.testing.assert_array_equal(np.asarray(arr, dtype=np.int64), src)

    def test_ravel_returns_flat_qiarray(self):

        src = np.array([[1, 2], [3, 4]], dtype=np.int64)

        arr = qiarray.ravel(src)

        assert arr.dtype == qiarray.dtype
        assert arr.shape == (4,)
        assert arr.flags["C_CONTIGUOUS"]
        np.testing.assert_array_equal(
            np.asarray(arr, dtype=np.int64), np.ravel(src, order="C")
        )

    def test_ravel_order_keyword(self):

        src = np.array([[1, 2], [3, 4]], dtype=np.int64)

        arr = qiarray.ravel(src, order="F")

        assert arr.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(arr, dtype=np.int64), np.ravel(src, order="F")
        )

    def test_array_asarray_keyword_support(self):

        src = np.array([1, 2, 3], dtype=np.int64)

        arr = qiarray.array(src, ndmin=2, order="C", copy=True)
        asarr = qiarray.asarray(src, ndmin=2, order="C", copy=False)

        assert arr.dtype == qiarray.dtype
        assert asarr.dtype == qiarray.dtype
        assert arr.shape == (1, 3)
        assert asarr.shape == (1, 3)

    def test_like_constructors(self):

        src = np.array([[1, 2], [3, 4]], dtype=np.int64)

        empty = qiarray.empty_like(src)
        zeros = qiarray.zeros_like(src)
        ones = qiarray.ones_like(src)
        full = qiarray.full_like(src, "-2")

        assert empty.dtype == qiarray.dtype
        assert zeros.dtype == qiarray.dtype
        assert ones.dtype == qiarray.dtype
        assert full.dtype == qiarray.dtype
        assert empty.shape == src.shape
        assert zeros.shape == src.shape
        assert ones.shape == src.shape
        assert full.shape == src.shape

        np.testing.assert_array_equal(
            np.asarray(zeros, dtype=np.int64), np.zeros_like(src)
        )
        np.testing.assert_array_equal(
            np.asarray(ones, dtype=np.int64), np.ones_like(src)
        )
        np.testing.assert_array_equal(
            np.asarray(full, dtype=np.int64), np.full_like(src, -2)
        )


@pytest.mark.qiarray
class TestQIArrayInterop:
    def test_cast_qiarray_to_int64(self):

        arr = qiarray.from_list([1, 2, 3])

        out = np.asarray(arr, dtype=np.int64)
        assert out.dtype == np.int64
        np.testing.assert_array_equal(out, np.array([1, 2, 3], dtype=np.int64))

    def test_cast_int64_to_qiarray_dtype(self):

        src = np.array([1, -2, 3], dtype=np.int64)

        out = np.asarray(src, dtype=qiarray.dtype)
        assert out.dtype == qiarray.dtype
        np.testing.assert_array_equal(np.asarray(out, dtype=np.int64), src)

    def test_cast_int32_to_qiarray_dtype(self):

        src = np.array([5, -7], dtype=np.int32)

        out = np.asarray(src, dtype=qiarray.dtype)
        assert out.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(out, dtype=np.int64), np.array([5, -7], dtype=np.int64)
        )

    @pytest.mark.parametrize("dtype", FIXED_WIDTH_DTYPES)
    def test_cast_fixed_width_integer_dtype_to_qiarray_dtype(self, dtype):

        src = np.array([0, 1, 7], dtype=dtype)

        out = np.asarray(src, dtype=qiarray.dtype)
        assert out.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(out, dtype=np.int64),
            src.astype(np.int64),
        )

    @pytest.mark.parametrize("dtype", FIXED_WIDTH_DTYPES)
    def test_cast_qiarray_to_fixed_width_integer_dtype(self, dtype):

        arr = qiarray.from_list([0, 1, 7])

        out = np.asarray(arr, dtype=dtype)
        assert out.dtype == np.dtype(dtype)
        np.testing.assert_array_equal(
            out.astype(np.int64), np.array([0, 1, 7], dtype=np.int64)
        )

    def test_numpy_sum_and_prod_reduce_qiarray(self):

        values = np.array([[2, -3], [4, 5]], dtype=np.int64)
        arr = qiarray.from_array(values)

        sum_all = np.sum(arr)
        sum_axis0 = np.sum(arr, axis=0)
        prod_axis1 = np.prod(arr, axis=1)

        assert int(sum_all) == int(np.sum(values))
        np.testing.assert_array_equal(
            np.asarray(sum_axis0, dtype=np.int64),
            np.sum(values, axis=0),
        )
        np.testing.assert_array_equal(
            np.asarray(prod_axis1, dtype=np.int64),
            np.prod(values, axis=1),
        )

    def test_numpy_concatenate_qiarray_inputs(self):

        left = qiarray.from_list([1, 2])
        right = qiarray.from_list([-4, 7])

        out = np.concatenate([left, right])

        assert out.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(out, dtype=np.int64),
            np.concatenate(
                [
                    np.asarray(left, dtype=np.int64),
                    np.asarray(right, dtype=np.int64),
                ]
            ),
        )

    def test_numpy_stack_qiarray_inputs(self):

        a = qiarray.from_list([1, -3, 2])
        b = qiarray.from_list([5, 4, -2])

        out = np.stack([a, b], axis=0)

        assert out.dtype == qiarray.dtype
        assert out.shape == (2, 3)
        np.testing.assert_array_equal(
            np.asarray(out, dtype=np.int64),
            np.stack(
                [np.asarray(a, dtype=np.int64), np.asarray(b, dtype=np.int64)],
                axis=0,
            ),
        )


@pytest.mark.qiarray
class TestQIArrayUfuncs:
    def test_add_subtract_multiply_ufuncs(self):

        a = qiarray.from_list([4, 6, 8])
        b = qiarray.from_list([1, 2, 3])

        add = np.add(a, b)
        sub = np.subtract(a, b)
        mul = np.multiply(a, b)

        assert add.dtype == qiarray.dtype
        assert sub.dtype == qiarray.dtype
        assert mul.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(add, dtype=np.int64), np.array([5, 8, 11], dtype=np.int64)
        )
        np.testing.assert_array_equal(
            np.asarray(sub, dtype=np.int64), np.array([3, 4, 5], dtype=np.int64)
        )
        np.testing.assert_array_equal(
            np.asarray(mul, dtype=np.int64), np.array([4, 12, 24], dtype=np.int64)
        )

    def test_mixed_qiarray_int64_binary_ufuncs(self):

        q = qiarray.from_list([2, 3, 4])
        i = np.array([10, 20, 30], dtype=np.int64)

        add_qi = np.add(q, i)
        add_iq = np.add(i, q)
        xor_qi = np.bitwise_xor(q, i)
        xor_iq = np.bitwise_xor(i, q)

        for out in [add_qi, add_iq, xor_qi, xor_iq]:
            assert out.dtype == qiarray.dtype

        np.testing.assert_array_equal(
            np.asarray(add_qi, dtype=np.int64), np.array([12, 23, 34], dtype=np.int64)
        )
        np.testing.assert_array_equal(
            np.asarray(add_iq, dtype=np.int64), np.array([12, 23, 34], dtype=np.int64)
        )
        np.testing.assert_array_equal(
            np.asarray(xor_qi, dtype=np.int64),
            np.bitwise_xor(np.array([2, 3, 4], dtype=np.int64), i),
        )
        np.testing.assert_array_equal(
            np.asarray(xor_iq, dtype=np.int64),
            np.bitwise_xor(i, np.array([2, 3, 4], dtype=np.int64)),
        )

    @pytest.mark.parametrize("dtype", FIXED_WIDTH_DTYPES)
    def test_mixed_fixed_width_integer_binary_ufuncs(self, dtype):

        q = qiarray.from_list([2, 3, 4])
        values = np.array([5, 6, 7], dtype=dtype)

        add_qi = np.add(q, values)
        add_iq = np.add(values, q)
        mul_qi = np.multiply(q, values)
        or_iq = np.bitwise_or(values, q)

        for out in [add_qi, add_iq, mul_qi, or_iq]:
            assert out.dtype == qiarray.dtype

        q64 = np.array([2, 3, 4], dtype=np.int64)
        v64 = values.astype(np.int64)
        np.testing.assert_array_equal(np.asarray(add_qi, dtype=np.int64), q64 + v64)
        np.testing.assert_array_equal(np.asarray(add_iq, dtype=np.int64), v64 + q64)
        np.testing.assert_array_equal(np.asarray(mul_qi, dtype=np.int64), q64 * v64)
        np.testing.assert_array_equal(
            np.asarray(or_iq, dtype=np.int64), np.bitwise_or(v64, q64)
        )

    def test_floor_divide_and_remainder_follow_python_integer_semantics(self):

        left = qiarray.from_list([-7, 7, -7, 7])
        right = qiarray.from_list([3, 3, -3, -3])

        floordiv = np.floor_divide(left, right)
        remainder = np.remainder(left, right)

        assert floordiv.dtype == qiarray.dtype
        assert remainder.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(floordiv, dtype=np.int64),
            np.array([-3, 2, 2, -3], dtype=np.int64),
        )
        np.testing.assert_array_equal(
            np.asarray(remainder, dtype=np.int64),
            np.array([2, 1, -1, -2], dtype=np.int64),
        )

    def test_mixed_floor_divide_and_remainder_with_fixed_width_dtype(self):

        q = qiarray.from_list([-7, 7, -7, 7])
        i = np.array([3, 3, -3, -3], dtype=np.int16)

        floordiv_qi = np.floor_divide(q, i)
        floordiv_iq = np.floor_divide(i, q)
        remainder_qi = np.remainder(q, i)
        remainder_iq = np.remainder(i, q)

        q64 = np.array([-7, 7, -7, 7], dtype=np.int64)
        i64 = i.astype(np.int64)
        np.testing.assert_array_equal(
            np.asarray(floordiv_qi, dtype=np.int64), np.floor_divide(q64, i64)
        )
        np.testing.assert_array_equal(
            np.asarray(floordiv_iq, dtype=np.int64), np.floor_divide(i64, q64)
        )
        np.testing.assert_array_equal(
            np.asarray(remainder_qi, dtype=np.int64), np.remainder(q64, i64)
        )
        np.testing.assert_array_equal(
            np.asarray(remainder_iq, dtype=np.int64), np.remainder(i64, q64)
        )

    def test_shift_ufuncs(self):

        values = qiarray.from_list([1, 8, 16])
        shifts = qiarray.from_list([1, 2, 3])

        left = np.left_shift(values, shifts)
        right = np.right_shift(values, shifts)

        assert left.dtype == qiarray.dtype
        assert right.dtype == qiarray.dtype
        np.testing.assert_array_equal(
            np.asarray(left, dtype=np.int64), np.array([2, 32, 128], dtype=np.int64)
        )
        np.testing.assert_array_equal(
            np.asarray(right, dtype=np.int64), np.array([0, 2, 2], dtype=np.int64)
        )

    def test_mixed_shift_ufuncs_with_fixed_width_dtype(self):

        q = qiarray.from_list([1, 8, 16])
        shift_counts = np.array([1, 2, 3], dtype=np.int16)
        base = np.array([4, 8, 16], dtype=np.int16)

        left_qi = np.left_shift(q, shift_counts)
        left_iq = np.left_shift(base, q)
        right_qi = np.right_shift(q, shift_counts)
        right_iq = np.right_shift(base, q)

        np.testing.assert_array_equal(
            np.asarray(left_qi, dtype=np.int64),
            np.left_shift(
                np.array([1, 8, 16], dtype=np.int64), shift_counts.astype(np.int64)
            ),
        )
        np.testing.assert_array_equal(
            np.asarray(left_iq, dtype=np.int64),
            np.left_shift(base.astype(np.int64), np.array([1, 8, 16], dtype=np.int64)),
        )
        np.testing.assert_array_equal(
            np.asarray(right_qi, dtype=np.int64),
            np.right_shift(
                np.array([1, 8, 16], dtype=np.int64), shift_counts.astype(np.int64)
            ),
        )
        np.testing.assert_array_equal(
            np.asarray(right_iq, dtype=np.int64),
            np.right_shift(base.astype(np.int64), np.array([1, 8, 16], dtype=np.int64)),
        )

    def test_unary_and_bitwise_ufuncs(self):

        a = qiarray.from_list([1, -2, 3])
        b = qiarray.from_list([7, 4, 1])

        pos = np.positive(a)
        neg = np.negative(a)
        abs_out = np.absolute(a)
        inv = np.invert(a)
        bit_and = np.bitwise_and(a, b)
        bit_or = np.bitwise_or(a, b)

        assert pos.dtype == qiarray.dtype
        assert neg.dtype == qiarray.dtype
        assert abs_out.dtype == qiarray.dtype
        assert inv.dtype == qiarray.dtype
        assert bit_and.dtype == qiarray.dtype
        assert bit_or.dtype == qiarray.dtype

        avals = np.array([1, -2, 3], dtype=np.int64)
        bvals = np.array([7, 4, 1], dtype=np.int64)
        np.testing.assert_array_equal(np.asarray(pos, dtype=np.int64), avals)
        np.testing.assert_array_equal(np.asarray(neg, dtype=np.int64), -avals)
        np.testing.assert_array_equal(
            np.asarray(abs_out, dtype=np.int64), np.abs(avals)
        )
        np.testing.assert_array_equal(np.asarray(inv, dtype=np.int64), np.invert(avals))
        np.testing.assert_array_equal(
            np.asarray(bit_and, dtype=np.int64), np.bitwise_and(avals, bvals)
        )
        np.testing.assert_array_equal(
            np.asarray(bit_or, dtype=np.int64), np.bitwise_or(avals, bvals)
        )
