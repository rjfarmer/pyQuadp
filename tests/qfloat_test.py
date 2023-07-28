# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint

import numpy as np
import pytest

import pyquad as pq


class TestQFloat:
    def test_make(self):
        q = pq.qfloat(1)
        assert str(q) == "1.000000000000000000000000000000000000e+00"

        q = pq.qfloat(1.0)
        assert str(q) == "1.000000000000000000000000000000000000e+00"

        q = pq.qfloat("1")
        assert str(q) == "1.000000000000000000000000000000000000e+00"

        q2 = pq.qfloat(q)
        assert str(q) == "1.000000000000000000000000000000000000e+00"

        with pytest.raises(TypeError) as cm:
            q = pq.qfloat("abc")

    def test_add(self):
        q1 = pq.qfloat(1)
        q2 = pq.qfloat(2)
        assert str(q1 + q1) == str(q2)

    def test_sub(self):
        q1 = pq.qfloat(1)
        q2 = pq.qfloat(2)
        assert str(q2 - q1) == str(q1)

    def test_comp(self):
        q1 = pq.qfloat(1)
        q2 = pq.qfloat(2)
        q3 = pq.qfloat(1)

        assert q1 <= q2
        assert not q1 == q2
        assert q1 == q3
        assert not q1 >= q2
        assert q1 <= q3
        assert q1 >= q3

    def test_float(self):
        q1 = pq.qfloat(1.0)

        assert float(q1) == 1.0

        assert float(pq.qfloat("1e300")) == 1e300

    def test_int(self):
        q1 = pq.qfloat(1.0)
        assert int(q1) == 1

        q1 = pq.qfloat(100.0)
        assert int(q1) == 100

        q1 = pq.qfloat(1e300)
        assert int(q1) == 9223372036854775807

    def test_nan(self):
        q = pq.qfloat("nan")

        assert not q == q

        q = pq.qfloat("inf")
        q = pq.qfloat("-inf")

    def test_mult(self):
        q1 = pq.qfloat(5.5)
        q2 = pq.qfloat(2)

        q3 = pq.qfloat(2 * 5.5)

        assert q1 * q2 == q3

    def test_true_div(self):
        q1 = pq.qfloat(5.5)
        q2 = pq.qfloat(2)

        q3 = pq.qfloat(5.5 / 2)

        assert q1 / q2 == q3

    def test_floor_div(self):
        q1 = pq.qfloat(5.5)
        q2 = pq.qfloat(2)

        q3 = pq.qfloat(5.5 // 2)

        assert q1 // q2 == q3

    def test_remainder(self):
        q1 = pq.qfloat(4.4)
        q2 = pq.qfloat(2)

        q3 = pq.qfloat(4.4 % 2)

        assert q1 % q2 == q3

    def test_pow(self):
        q1 = pq.qfloat(5.5)
        q2 = pq.qfloat(2)

        q3 = pq.qfloat(pow(5.5, 2))

        assert pow(q1, q2) == q3

        q1 = pq.qfloat(5)
        q2 = pq.qfloat(2)
        q4 = pq.qfloat(3)
        q3 = pq.qfloat(pow(5, 2, 3))

        assert pow(q1, q2, q4) == q3

    def test_divmod(self):
        q1 = pq.qfloat(4.4)
        q2 = pq.qfloat(2)

        q3 = divmod(4.4, 2)

        assert divmod(q1, q2) == (pq.qfloat(q3[0]), pq.qfloat(q3[1]))

    def test_negative(self):
        q1 = pq.qfloat(5.5)

        q3 = pq.qfloat(-5.5)

        assert -q1 == q3

    def test_positive(self):
        q1 = pq.qfloat(5.5)

        q3 = pq.qfloat(5.5)

        assert +q1 == q3

    def test_abs(self):
        q1 = pq.qfloat(-5.5)

        q3 = pq.qfloat(5.5)

        assert abs(q1) == q3

        q1 = pq.qfloat(5.5)

        q3 = pq.qfloat(5.5)

        assert abs(q1) == q3

    def test_inp_add(self):
        q1 = pq.qfloat(1)
        q2 = pq.qfloat(2)

        q1 += q1

        assert str(q1) == str(q2)

    def test_inp_sub(self):
        q1 = pq.qfloat(1)
        q2 = pq.qfloat(2)

        q2 -= q1

        assert q2 == q1

    def test_bool(self):
        q1 = pq.qfloat(1.0)

        assert bool(q1)

        q1 = pq.qfloat(0.0)

        assert not bool(q1)

    def test_inp_mult(self):
        q1 = pq.qfloat(5.5)
        q2 = pq.qfloat(2)

        q2 *= q1

        assert q2 == q1 * pq.qfloat(2)

    def test_inp_true_div(self):
        q1 = pq.qfloat(5.5)
        q2 = pq.qfloat(2)

        q1 /= q2

        assert q1 == pq.qfloat(5.5) / pq.qfloat(2)

    def test_floor_inp_div(self):
        q1 = pq.qfloat(5.5)
        q2 = pq.qfloat(2)

        q1 //= q2

        assert q1 == pq.qfloat(5.5) // pq.qfloat(2)

    def test_inp_remainder(self):
        q1 = pq.qfloat(4.4)
        q2 = pq.qfloat(2)

        q3 = pq.qfloat(4.4 % 2)

        q1 %= q2

        assert q1 == q3

    def test_inp_pow(self):
        q1 = pq.qfloat(5.5)
        q2 = pq.qfloat(2)

        q1 **= q2

        assert pq.qfloat(5.5) ** pq.qfloat(2) == q1
