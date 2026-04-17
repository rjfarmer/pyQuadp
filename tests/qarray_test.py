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
