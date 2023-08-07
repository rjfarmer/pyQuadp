# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint

import numpy as np
import pytest
import pickle

import pyquadp as pq

from hypothesis import given, assume
from hypothesis.strategies import floats, integers


class Testqint:
    def test_make_basic(self):
        q = pq.qint(1)
        assert str(q) == "1"

        with pytest.raises(TypeError) as cm:
            q = pq.qint(1.0)

        q = pq.qint("1")
        assert str(q) == "1"

        q2 = pq.qint(q)
        assert str(q) == "1"

        with pytest.raises(TypeError) as cm:
            q = pq.qint("abc")

    @given(integers())
    def test_make(self, x):
        assume(x <= pq.INT128_MAX)
        assume(x >= pq.INT128_MIN)

        assert pq.qint(x) == x
        assert str(pq.qint(str(x))) == str(x)

    def test_add(self):
        q1 = pq.qint(1)
        q2 = pq.qint(2)
        assert str(q1 + q1) == str(q2)

    def test_sub(self):
        q1 = pq.qint(1)
        q2 = pq.qint(2)
        assert str(q2 - q1) == str(q1)

    def test_comp(self):
        q1 = pq.qint(1)
        q2 = pq.qint(2)
        q3 = pq.qint(1)

        assert q1 <= q2
        assert not q1 == q2
        assert q1 == q3
        assert not q1 >= q2
        assert q1 <= q3
        assert q1 >= q3

    def test_int(self):
        q1 = pq.qint(1)
        assert int(q1) == 1

        q1 = pq.qint(100)
        assert int(q1) == 100

    def test_mult(self):
        q1 = pq.qint(5)
        q2 = pq.qint(2)

        q3 = pq.qint(2 * 5)

        assert q1 * q2 == q3

    @pytest.mark.skip
    def test_true_div(self):
        q1 = pq.qint(5)
        q2 = pq.qint(2)

        assert q1 / q2 == 5 / 2

    def test_floor_div(self):
        q1 = pq.qint(5)
        q2 = pq.qint(2)

        q3 = pq.qint(5 // 2)

        assert q1 // q2 == q3

    def test_remainder(self):
        q1 = pq.qint(4)
        q2 = pq.qint(2)

        q3 = pq.qint(4 % 2)

        assert q1 % q2 == q3

    def test_pow(self):
        q1 = pq.qint(5)
        q2 = pq.qint(2)

        q3 = pq.qint(pow(5, 2))

        assert pow(q1, q2) == q3

        q1 = pq.qint(5)
        q2 = pq.qint(2)
        q4 = pq.qint(3)
        q3 = pq.qint(pow(5, 2, 3))

        assert pow(q1, q2, q4) == q3

    def test_divmod(self):
        q1 = pq.qint(4)
        q2 = pq.qint(2)

        q3 = divmod(4, 2)

        assert divmod(q1, q2) == (pq.qint(q3[0]), pq.qint(q3[1]))

    def test_negative(self):
        q1 = pq.qint(5)

        q3 = pq.qint(-5)

        assert -q1 == q3

    def test_positive(self):
        q1 = pq.qint(5)

        q3 = pq.qint(5)

        assert +q1 == q3

    def test_abs(self):
        q1 = pq.qint(-5)

        q3 = pq.qint(5)

        assert abs(q1) == q3

        q1 = pq.qint(5)

        q3 = pq.qint(5)

        assert abs(q1) == q3

    def test_inp_add(self):
        q1 = pq.qint(1)
        q2 = pq.qint(2)

        q1 += q1

        assert str(q1) == str(q2)

    def test_inp_sub(self):
        q1 = pq.qint(1)
        q2 = pq.qint(2)

        q2 -= q1

        assert q2 == q1

    def test_bool(self):
        q1 = pq.qint(1)

        assert bool(q1)

        q1 = pq.qint(0)

        assert not bool(q1)

    def test_inp_mult(self):
        q1 = pq.qint(5)
        q2 = pq.qint(2)

        q2 *= q1

        assert q2 == q1 * pq.qint(2)

    def test_inp_true_div(self):
        q1 = pq.qint(5)
        q2 = pq.qint(2)

        q1 /= q2

        assert q1 == pq.qint(5) / pq.qint(2)

    def test_floor_inp_div(self):
        q1 = pq.qint(5)
        q2 = pq.qint(2)

        q1 //= q2

        assert q1 == pq.qint(5) // pq.qint(2)

    def test_inp_remainder(self):
        q1 = pq.qint(4)
        q2 = pq.qint(2)

        q3 = pq.qint(4 % 2)

        q1 %= q2

        assert q1 == q3

    def test_inp_pow(self):
        q1 = pq.qint(5)
        q2 = pq.qint(2)

        q1 **= q2

        assert pq.qint(5) ** pq.qint(2) == q1

    def test_bytes(self):
        q1 = pq.qint("1234567890")

        b1 = q1.to_bytes()

        q2 = pq.qint.from_bytes(b1)

        assert q1 == q2

    def test_pickle(self):
        q1 = pq.qint("1234567890")
        pickled_value = pickle.dumps(q1)
        result = pickle.loads(pickled_value)

        assert result == q1

    def test_hash(self):
        q1 = pq.qint("1234567890")
        q2 = pq.qint("1234567899999")
        h1 = hash(q1)
        h2 = hash(q2)

        assert not h1 == h2

        # Check dict lookup
        x = {q1: "abc"}
        assert x[q1] == "abc"

    @pytest.mark.skip
    def test_hex(self):
        q = pq.qint("1234567899999")

        assert q.hex().lower() == "0x11f71fb2b5f"

        q2 = pq.qint.fromhex("0x11f71fb2b5f")

        assert q2 == q

    def test_shifts(self):
        q1 = pq.qint(5)
        assert q1 << 3 == 5 << 3
        q1 = pq.qint(5)
        assert q1 >> 3 == 5 >> 3
        q1 = pq.qint(5)
        assert ~q1 == ~5
        q1 = pq.qint(5)
        assert q1 & 8 == 5 & 8
        q1 = pq.qint(5)
        assert q1 | 8 == 5 | 8
        q1 = pq.qint(5)
        assert q1 ^ 3 == 5 ^ 3

    def test_inplace_shifts(self):
        q1 = pq.qint(5)
        q1 <<= 3
        assert q1 == 5 << 3
        q1 = pq.qint(5)
        q1 >>= 5
        assert q1 == 5 >> 3
        q1 = pq.qint(5)
        q1 &= 8
        assert q1 == 5 & 8
        q1 = pq.qint(5)
        q1 |= 8
        assert q1 == 5 | 8
        q1 = pq.qint(500)
        q1 ^= 3
        assert q1 == 500 ^ 3
