# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint

import numpy as np
import pytest

import pyquad as pq


class TestQFloat:
    def test_make(self):
        q = pq.qcmplx(complex(1, 1))
        assert (
            str(q)
            == "(1.000000000000000000000000000000000000e+00+1.000000000000000000000000000000000000e+00j)"
        )
        assert (
            repr(q)
            == "qcmplx('1.000000000000000000000000000000000000e+00+1.000000000000000000000000000000000000e+00j')"
        )
        assert str(q.real) == "1.000000000000000000000000000000000000e+00"
        assert str(q.imag) == "1.000000000000000000000000000000000000e+00"

        with pytest.raises(AttributeError) as cm:
            q.real = 2

        with pytest.raises(AttributeError) as cm:
            q.imag = 2

        q = pq.qcmplx(complex("1+1j"))
        assert (
            str(q)
            == "(1.000000000000000000000000000000000000e+00+1.000000000000000000000000000000000000e+00j)"
        )

        q = pq.qcmplx(pq.qfloat(5))
        assert (
            str(q)
            == "(5.000000000000000000000000000000000000e+00+0.000000000000000000000000000000000000e+00j)"
        )

        with pytest.raises(TypeError) as cm:
            q = pq.qfloat("abc")

    def test_make2(self):
        q = pq.qcmplx(1, 1)
        assert (
            str(q)
            == "(1.000000000000000000000000000000000000e+00+1.000000000000000000000000000000000000e+00j)"
        )

        q = pq.qcmplx(5.0, 5.0)
        assert (
            str(q)
            == "(5.000000000000000000000000000000000000e+00+5.000000000000000000000000000000000000e+00j)"
        )

        q = pq.qcmplx(pq.qfloat(5), pq.qfloat(5))
        assert (
            str(q)
            == "(5.000000000000000000000000000000000000e+00+5.000000000000000000000000000000000000e+00j)"
        )

        q = pq.qcmplx(pq.qfloat(5), 5)
        assert (
            str(q)
            == "(5.000000000000000000000000000000000000e+00+5.000000000000000000000000000000000000e+00j)"
        )

        q = pq.qcmplx("6.5", "1.5")
        assert (
            str(q)
            == "(6.500000000000000000000000000000000000e+00+1.500000000000000000000000000000000000e+00j)"
        )

    def test_get_cmplx(self):
        q = pq.qcmplx(pq.qfloat(5), 5)

        assert complex(q) == complex(5, 5)

        q = pq.qcmplx("1e400", "1e400")

        with pytest.raises(ValueError) as cm:
            complex(q)

    def test_add(self):
        q = pq.qcmplx(pq.qfloat(5), 5)
        assert str(q + q) == pq.qcmplx(pq.qfloat(10), 10)

    def test_sub(self):
        q = pq.qcmplx(pq.qfloat(5), 5)
        assert str(q - q) == pq.qcmplx(pq.qfloat(0), 0)

