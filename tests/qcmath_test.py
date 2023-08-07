# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint

import numpy as np
import scipy.special
import pytest
import math
import cmath
import decimal

from hypothesis import given, assume
from hypothesis.strategies import floats, complex_numbers

import pyquadp as pq
import pyquadp.qmath as qm
import pyquadp.qcmath as qcm


class TestQCMathFloat:
    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=1e100
        )
    )
    def test_cabs(self, x):
        assert float(qcm.cabsq(x)) == pytest.approx(abs(x))

    @given(
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=1e300),
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=1e300),
    )
    def test_cargq(self, x, y):
        c = complex(x, y)
        assert float(qcm.cargq(c)) == pytest.approx(cmath.phase(c))

    @given(complex_numbers(allow_infinity=False, allow_nan=False))
    def test_cimagq(self, x):
        assert float(qcm.cimagq(x)) == pytest.approx(x.imag)

    @given(complex_numbers(allow_infinity=False, allow_nan=False))
    def test_crealq(self, x):
        assert float(qcm.crealq(x)) == pytest.approx(x.real)

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_cacoshq(self, x):
        assert complex(qcm.cacoshq(x)) == pytest.approx(cmath.acosh(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_cacosq(self, x):
        assert complex(qcm.cacosq(x)) == pytest.approx(cmath.acos(x))

    @given(
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
    )
    def test_casinhq(self, x, y):
        c = complex(x, y)
        assert complex(qcm.casinhq(c)) == pytest.approx(cmath.asinh(c))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_casinq(self, x):
        assert complex(qcm.casinq(x)) == pytest.approx(cmath.asin(x))

    @given(
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
    )
    def test_catanhq(self, x, y):
        c = complex(x, y)
        assert complex(qcm.catanhq(c)) == pytest.approx(cmath.atanh(c))

    @given(
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
    )
    def test_catanq(self, x, y):
        c = complex(x, y)
        assert complex(qcm.catanq(c)) == pytest.approx(cmath.atan(c))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_ccosq(self, x):
        assert complex(qcm.ccosq(x)) == pytest.approx(cmath.cos(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_ccoshq(self, x):
        assert complex(qcm.ccoshq(x)) == pytest.approx(cmath.cosh(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_cexpq(self, x):
        assert complex(qcm.cexpq(x)) == pytest.approx(cmath.exp(x))

    @given(floats(allow_infinity=False, allow_nan=False))
    def test_cexpiq(self, x):
        s = qm.sin(x)
        c = qm.cos(x)

        assert complex(qcm.cexpiq(x)) == pytest.approx(complex(c, s))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=1, max_magnitude=10
        )
    )
    def test_clogq(self, x):
        assert complex(qcm.clogq(x)) == pytest.approx(cmath.log(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=1, max_magnitude=10
        )
    )
    def test_clog10q(self, x):
        assert complex(qcm.clog10q(x)) == pytest.approx(cmath.log10(x))

    # @given(complex_numbers(allow_infinity=False,allow_nan=False, min_magnitude=0, max_magnitude=100))
    # def test_conjq(self, x):
    #     assert complex(qcm.conjq(x)) == ~x

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=1, max_magnitude=10
        ),
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=1, max_magnitude=10
        ),
    )
    def test_cpowq(self, x, y):
        assert complex(qcm.cpowq(x, y)) == pytest.approx(x**y)

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_cprojq(self, x):
        assert complex(qcm.cprojq(x)) == x

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_csinq(self, x):
        assert complex(qcm.csinq(x)) == pytest.approx(cmath.sin(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_csinhq(self, x):
        assert complex(qcm.csinhq(x)) == pytest.approx(cmath.sinh(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_csqrtq(self, x):
        assert complex(qcm.csqrtq(x)) == pytest.approx(cmath.sqrt(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_ctanq(self, x):
        assert complex(qcm.ctanq(x)) == pytest.approx(cmath.tan(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_ctanhq(self, x):
        assert complex(qcm.ctanhq(x)) == pytest.approx(cmath.tanh(x))


class TestQCMathPy:
    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_cacosh(self, x):
        assert complex(qcm.acosh(x)) == pytest.approx(cmath.acosh(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_cacos(self, x):
        assert complex(qcm.acos(x)) == pytest.approx(cmath.acos(x))

    @given(
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
    )
    def test_casinh(self, x, y):
        c = complex(x, y)
        assert complex(qcm.asinh(c)) == pytest.approx(cmath.asinh(c))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_casin(self, x):
        assert complex(qcm.asin(x)) == pytest.approx(cmath.asin(x))

    @given(
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
    )
    def test_catanh(self, x, y):
        c = complex(x, y)
        assert complex(qcm.atanh(c)) == pytest.approx(cmath.atanh(c))

    @given(
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=100),
    )
    def test_catan(self, x, y):
        c = complex(x, y)
        assert complex(qcm.atan(c)) == pytest.approx(cmath.atan(c))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_ccos(self, x):
        assert complex(qcm.cos(x)) == pytest.approx(cmath.cos(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_ccosh(self, x):
        assert complex(qcm.cosh(x)) == pytest.approx(cmath.cosh(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_cexp(self, x):
        assert complex(qcm.exp(x)) == pytest.approx(cmath.exp(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=1, max_magnitude=10
        )
    )
    def test_clog(self, x):
        assert complex(qcm.log(x)) == pytest.approx(cmath.log(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=1, max_magnitude=10
        )
    )
    def test_clog10(self, x):
        assert complex(qcm.log10(x)) == pytest.approx(cmath.log10(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_csin(self, x):
        assert complex(qcm.sin(x)) == pytest.approx(cmath.sin(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_csinh(self, x):
        assert complex(qcm.sinh(x)) == pytest.approx(cmath.sinh(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_csqrt(self, x):
        assert complex(qcm.sqrt(x)) == pytest.approx(cmath.sqrt(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_ctan(self, x):
        assert complex(qcm.tan(x)) == pytest.approx(cmath.tan(x))

    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=100
        )
    )
    def test_ctanh(self, x):
        assert complex(qcm.tanh(x)) == pytest.approx(cmath.tanh(x))
