# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint
import pickle

import numpy as np
import pytest

import pyquadp as pq


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

    def test_add(self):
        q = pq.qcmplx(pq.qfloat(5), 5)
        assert str(q + q) == str(pq.qcmplx(pq.qfloat(10), 10))

    def test_sub(self):
        q = pq.qcmplx(pq.qfloat(5), 5)
        assert str(q - q) == str(pq.qcmplx(pq.qfloat(0), 0))

    def test_mult(self):
        c = complex(5, 5)
        q = pq.qcmplx(c)
        assert str(q * q) == str(pq.qcmplx(c * c))

    def test_mult(self):
        c = complex(5, 5)
        q = pq.qcmplx(c)
        assert str(q / q) == str(pq.qcmplx(c / c))

    def test_cmp(self):
        q1 = complex(2, 3)
        q1a = pq.qcmplx(2, 3)
        q2 = complex(3, 2)

        assert pq.qcmplx(q1) == q1a
        assert pq.qcmplx(q2) != q1a

        with pytest.raises(TypeError) as cm:
            q1 > q1a

    def test_abs(self):
        q1 = complex(2, 3)
        assert abs(q1) == abs(complex(q1))

    def test_conjugate(self):
        c1 = complex(1, -1)
        c2 = complex(1, 1)

        assert complex(pq.qcmplx(c1).conjugate()) == c2
        assert complex(pq.qcmplx(c2).conjugate()) == c1

    def test_bytes(self):
        q1 = pq.qcmplx("1.234567890", "9.87654321")

        b1 = q1.to_bytes()

        q2 = pq.qcmplx.from_bytes(b1)

        assert q1 == q2

    def test_pickle(self):
        q1 = pq.qcmplx("1.234567890", "9.87654321")
        pickled_value = pickle.dumps(q1)
        result = pickle.loads(pickled_value)

        assert result == q1

    def test_hash(self):
        q1 = pq.qcmplx("1.234567890", "9.87654321")
        q2 = pq.qcmplx("9.87654321", "1.234567890")
        h1 = hash(q1)
        h2 = hash(q2)

        assert not h1 == h2

        # Check dict lookup
        x = {q1: "abc"}
        assert x[q1] == "abc"
