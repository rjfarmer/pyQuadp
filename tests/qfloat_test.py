# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint

import numpy as np
import pytest

import pyquad as pq


class TestQFloat:
    def test_make(self):
        q=pq.qfloat(1)
        q=pq.qfloat(1.0)
        q=pq.qfloat('1')

        with pytest.raises(TypeError) as cm:
            q=pq.qfloat('abc')