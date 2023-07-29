# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint

import numpy as np
import scipy.special
import pytest
import math
import decimal

from hypothesis import given, assume
from hypothesis.strategies import complex_numbers

import pyquad as pq
import pyquad.qmath as qm
import pyquad.qcmath as qcm

@pytest.mark.skip
class TestQCMathFloat:

    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_cabsq(self, x):
        assert complex(qcm.cabsq(x)) == math.cabs(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_cargq(self, x):
        assert complex(qcm.cargq(x)) == math.carg(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_cimagq(self, x):
        assert complex(qcm.cimagq(x)) == math.cimag(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_crealq(self, x):
        assert complex(qcm.crealq(x)) == math.creal(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_cacoshq(self, x):
        assert complex(qcm.cacoshq(x)) == math.cacosh(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_cacosq(self, x):
        assert complex(qcm.cacosq(x)) == math.cacos(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_casinhq(self, x):
        assert complex(qcm.casinhq(x)) == math.casinh(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_casinq(self, x):
        assert complex(qcm.casinq(x)) == math.casin(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_catanhq(self, x):
        assert complex(qcm.catanhq(x)) == math.catanh(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_catanq(self, x):
        assert complex(qcm.catanq(x)) == math.catan(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_ccosq(self, x):
        assert complex(qcm.ccosq(x)) == math.ccos(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_ccoshq(self, x):
        assert complex(qcm.ccoshq(x)) == math.ccosh(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_cexpq(self, x):
        assert complex(qcm.cexpq(x)) == math.cexp(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_cexpiq(self, x):
        assert complex(qcm.cexpiq(x)) == math.cexpi(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_clogq(self, x):
        assert complex(qcm.clogq(x)) == math.clog(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_clog10q(self, x):
        assert complex(qcm.clog10q(x)) == math.clog10(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_conjq(self, x):
        assert complex(qcm.conjq(x)) == math.conj(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_cpowq(self, x):
        assert complex(qcm.cpowq(x)) == math.cpow(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_cprojq(self, x):
        assert complex(qcm.cprojq(x)) == math.cproj(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_csinq(self, x):
        assert complex(qcm.csinq(x)) == math.csin(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_csinhq(self, x):
        assert complex(qcm.csinhq(x)) == math.csinh(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_csqrtq(self, x):
        assert complex(qcm.csqrtq(x)) == math.csqrt(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_ctanq(self, x):
        assert complex(qcm.ctanq(x)) == math.ctan(x)
    
    @given(complex_numbers(allow_infinity=False,allow_nan=False))
    def test_ctanhq(self, x):
        assert complex(qcm.ctanhq(x)) == math.ctanh(x)
    