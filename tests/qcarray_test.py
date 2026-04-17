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

        assert add_out.dtype == qcarray.dtype
        assert sub_out.dtype == qcarray.dtype
        assert mul_out.dtype == qcarray.dtype
        assert div_out.dtype == qcarray.dtype

        a128 = np.asarray(a, dtype=np.complex128)
        b128 = np.asarray(b, dtype=np.complex128)
        assert np.allclose(np.asarray(add_out, dtype=np.complex128), a128 + b128)
        assert np.allclose(np.asarray(sub_out, dtype=np.complex128), a128 - b128)
        assert np.allclose(np.asarray(mul_out, dtype=np.complex128), a128 * b128)
        assert np.allclose(np.asarray(div_out, dtype=np.complex128), a128 / b128)

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
