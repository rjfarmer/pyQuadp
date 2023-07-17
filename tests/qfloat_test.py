# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint

import numpy as np
import pytest

import pyquad as pq


class TestQFloat:
    def test_make(self):

        q=pq.qfloat(1)
        assert str(q) == '1.000000000000000000000000000000000000e+00'

        q=pq.qfloat(1.0)
        assert str(q) == '1.000000000000000000000000000000000000e+00'

        q=pq.qfloat('1')
        assert str(q) == '1.000000000000000000000000000000000000e+00'


        with pytest.raises(TypeError) as cm:
            q=pq.qfloat('abc')


    def test_add(self):
        q1=pq.qfloat(1)
        q2=pq.qfloat(2)
        assert str(q1+q1) == str(q2) 

    def test_sub(self):
        q1=pq.qfloat(1)
        q2=pq.qfloat(2)
        assert str(q2-q1) == str(q1) 