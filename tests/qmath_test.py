# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint

import numpy as np
import pytest

import pyquad as pq

class TestQFloat:
    def test_cos(self):
        q = pq.qfloat(0.0)

        assert pq.qmath.cos(q) == pq.qfloat(1.0)
