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

import pyquad as pq
import pyquad.qmath as qm
import pyquad.qcmath as qcm


class TestQCMathFloat:
    @given(
        complex_numbers(
            allow_infinity=False, allow_nan=False, min_magnitude=0, max_magnitude=1e100
        )
    )
    def test_cabsq(self, x):
        assert float(qcm.cabsq(x)) == pytest.approx(abs(x))

    @given(
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=1e300),
        floats(allow_infinity=False, allow_nan=False, min_value=1, max_value=1e300),
    )
    def test_cargq(self, x, y):
        c = complex(x, y)
        assert float(qcm.cargq(c)) == pytest.approx(ccmath.phase(c))

    @given(complex_numbers(allow_infinity=False, allow_nan=False))
    def test_cimagq(self, x):
        assert float(qcm.cimagq(x)) == pytest.approx(x.imag)

    @given(complex_numbers(allow_infinity=False, allow_nan=False))
    def test_crealq(self, x):
        assert float(qcm.crealq(x)) == pytest.approx(x.real)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_cacoshq(self, x):
    #     assert complex(qcm.cacoshq(x)) == cmath.acosh(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_cacosq(self, x):
    #     assert complex(qcm.cacosq(x)) == cmath.acos(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_casinhq(self, x):
    #     assert complex(qcm.casinhq(x)) == cmath.asinh(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_casinq(self, x):
    #     assert complex(qcm.casinq(x)) == cmath.asin(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_catanhq(self, x):
    #     assert complex(qcm.catanhq(x)) == cmath.atanh(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_catanq(self, x):
    #     assert complex(qcm.catanq(x)) == cmath.atan(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_ccosq(self, x):
    #     assert complex(qcm.ccosq(x)) == cmath.cos(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_ccoshq(self, x):
    #     assert complex(qcm.ccoshq(x)) == cmath.cosh(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_cexpq(self, x):
    #     assert complex(qcm.cexpq(x)) == cmath.exp(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_cexpiq(self, x):
    #     assert complex(qcm.cexpiq(x)) == cmath.expi(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_clogq(self, x):
    #     assert complex(qcm.clogq(x)) == cmath.log(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_clog10q(self, x):
    #     assert complex(qcm.clog10q(x)) == cmath.log10(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_conjq(self, x):
    #     assert complex(qcm.conjq(x)) == cmath.onj(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_cpowq(self, x):
    #     assert complex(qcm.cpowq(x)) == cmath.pow(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_cprojq(self, x):
    #     assert complex(qcm.cprojq(x)) == cmath.proj(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_csinq(self, x):
    #     assert complex(qcm.csinq(x)) == cmath.sin(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_csinhq(self, x):
    #     assert complex(qcm.csinhq(x)) == cmath.sinh(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_csqrtq(self, x):
    #     assert complex(qcm.csqrtq(x)) == cmath.sqrt(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_ctanq(self, x):
    #     assert complex(qcm.ctanq(x)) == cmath.tan(x)

    # @given(complex_numbers(allow_infinity=False,allow_nan=False))
    # def test_ctanhq(self, x):
    #     assert complex(qcm.ctanhq(x)) == cmath.tanh(x)
