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

    # def test_add(self):
    #     q1 = pq.qfloat(1)
    #     q2 = pq.qfloat(2)
    #     assert str(q1 + q1) == str(q2)

    # def test_sub(self):
    #     q1 = pq.qfloat(1)
    #     q2 = pq.qfloat(2)
    #     assert str(q2 - q1) == str(q1)

    # def test_comp(self):
    #     q1 = pq.qfloat(1)
    #     q2 = pq.qfloat(2)
    #     q3 = pq.qfloat(1)

    #     assert q1 <= q2
    #     assert not q1 == q2
    #     assert q1 == q3
    #     assert not q1 >= q2
    #     assert q1 <= q3
    #     assert q1 >= q3

    # def test_float(self):
    #     q1 = pq.qfloat(1.0)

    #     assert float(q1) == 1.0

    #     assert float(pq.qfloat("1e300")) == 1e300

    #     assert not np.isfinite(float(pq.qfloat("1e400")))

    # def test_int(self):
    #     q1 = pq.qfloat(1.0)
    #     assert int(q1) == 1

    #     q1 = pq.qfloat(100.0)
    #     assert int(q1) == 100

    #     q1 = pq.qfloat(1e300)
    #     assert int(q1) == 9223372036854775807

    # def test_nan(self):
    #     q = pq.qfloat("nan")

    #     assert not q == q

    # def test_mult(self):
    #     q1 = pq.qfloat(5.5)
    #     q2 = pq.qfloat(2)

    #     q3 = pq.qfloat(2 * 5.5)

    #     assert q1 * q2 == q3

    # def test_true_div(self):
    #     q1 = pq.qfloat(5.5)
    #     q2 = pq.qfloat(2)

    #     q3 = pq.qfloat(5.5 / 2)

    #     assert q1 / q2 == q3

    # def test_floor_div(self):
    #     q1 = pq.qfloat(5.5)
    #     q2 = pq.qfloat(2)

    #     q3 = pq.qfloat(5.5 // 2)

    #     assert q1 // q2 == q3

    # def test_remainder(self):
    #     q1 = pq.qfloat(4.4)
    #     q2 = pq.qfloat(2)

    #     q3 = pq.qfloat(4.4 % 2)

    #     assert q1 % q2 == q3

    # def test_pow(self):
    #     q1 = pq.qfloat(5.5)
    #     q2 = pq.qfloat(2)

    #     q3 = pq.qfloat(pow(5.5, 2))

    #     assert pow(q1, q2) == q3

    #     q1 = pq.qfloat(5)
    #     q2 = pq.qfloat(2)
    #     q4 = pq.qfloat(3)
    #     q3 = pq.qfloat(pow(5, 2, 3))

    #     assert pow(q1, q2, q4) == q3

    # def test_divmod(self):
    #     q1 = pq.qfloat(4.4)
    #     q2 = pq.qfloat(2)

    #     q3 = divmod(4.4, 2)

    #     assert divmod(q1, q2) == (pq.qfloat(q3[0]), pq.qfloat(q3[1]))

    # def test_negative(self):
    #     q1 = pq.qfloat(5.5)

    #     q3 = pq.qfloat(-5.5)

    #     assert -q1 == q3

    # def test_positive(self):
    #     q1 = pq.qfloat(5.5)

    #     q3 = pq.qfloat(5.5)

    #     assert +q1 == q3

    # def test_abs(self):
    #     q1 = pq.qfloat(-5.5)

    #     q3 = pq.qfloat(5.5)

    #     assert abs(q1) == q3

    #     q1 = pq.qfloat(5.5)

    #     q3 = pq.qfloat(5.5)

    #     assert abs(q1) == q3
