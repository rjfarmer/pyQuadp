# SPDX-License-Identifier: GPL-2.0+

import os, sys
from pprint import pprint

import numpy as np
import scipy.special
import pytest
import math
import decimal

from hypothesis import given, assume
from hypothesis.strategies import floats, integers

import pyquadp.qmath as qm


class TestQMathFloat:
    @given(floats(min_value=-1, max_value=1))
    def test_acosq(self, x):
        assert float(qm.acosq(x)) == pytest.approx(math.acos(x), nan_ok=True)

    @given(floats(min_value=1))
    def test_acoshq(self, x):
        assert float(qm.acoshq(x)) == pytest.approx(math.acosh(x), nan_ok=True)

    @given(floats(min_value=-1, max_value=1))
    def test_asinq(self, x):
        assert float(qm.asinq(x)) == pytest.approx(math.asin(x), nan_ok=True)

    @given(floats())
    def test_asinhq(self, x):
        # Float cant handle -infinity
        assume(x > -np.inf)
        assert float(qm.asinhq(x)) == pytest.approx(math.asinh(x), nan_ok=True)

    @given(floats(min_value=-1, max_value=1))
    def test_atanq(self, x):
        assert float(qm.atanq(x)) == pytest.approx(math.atan(x), nan_ok=True)

    @given(
        floats(
            min_value=-1,
            max_value=1,
            exclude_min=True,
            exclude_max=True,
        )
    )
    def test_atanhq(self, x):
        assert float(qm.atanhq(x)) == pytest.approx(math.atanh(x), nan_ok=True)

    @given(
        floats(),
        floats(),
    )
    def test_atan2q(self, x, y):
        assert float(qm.atan2q(x, y)) == pytest.approx(math.atan2(x, y), nan_ok=True)

    @given(floats())
    def test_cbrtq(self, x):
        assume(x > -np.inf)
        assert float(qm.cbrtq(x)) == pytest.approx(np.cbrt(x), nan_ok=True)

    @given(floats(allow_nan=False, allow_infinity=False))
    def test_ceilq(self, x):
        assert float(qm.ceilq(x)) == pytest.approx(math.ceil(x), nan_ok=True)

    @given(
        floats(allow_nan=False, allow_infinity=False),
        floats(allow_nan=False, allow_infinity=False),
    )
    def test_copysignq(self, x, y):
        assert float(qm.copysignq(x, y)) == pytest.approx(
            math.copysign(x, y), nan_ok=True
        )

    @given(floats(min_value=1, max_value=10))
    def test_coshq(self, x):
        assert float(qm.coshq(x)) == pytest.approx(math.cosh(x), nan_ok=True)

    @given(floats())
    def test_cosq(self, x):
        try:
            assert float(qm.cosq(x)) == pytest.approx(math.cos(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_erfq(self, x):
        assert float(qm.erfq(x)) == pytest.approx(math.erf(x), nan_ok=True)

    @given(floats())
    def test_erfcq(self, x):
        assert float(qm.erfcq(x)) == pytest.approx(math.erfc(x), nan_ok=True)

    @pytest.mark.skipif(
        sys.version_info < (3, 11), reason="requires python3.11 or higher"
    )
    @given(floats(max_value=100))
    def test_exp2q(self, x):
        assert float(qm.exp2q(x)) == pytest.approx(math.exp2(x), nan_ok=True)

    @given(floats(max_value=100))
    def test_expq(self, x):
        assert float(qm.expq(x)) == pytest.approx(math.exp(x), nan_ok=True)

    @given(floats(max_value=100))
    def test_expm1q(self, x):
        assert float(qm.expm1q(x)) == pytest.approx(math.expm1(x), nan_ok=True)

    @given(floats())
    def test_fabsq(self, x):
        assert float(qm.fabsq(x)) == pytest.approx(math.fabs(x), nan_ok=True)

    @given(
        floats(min_value=-1e300, max_value=1e300),
        floats(min_value=-1e300, max_value=1e300),
    )
    def test_fdimq(self, x, y):
        z = float(qm.fdimq(x, y))
        if x <= y:
            assert z == 0
        else:
            z == pytest.approx(x - y)

    @given(floats())
    def test_finiteq(self, x):
        assert qm.finiteq(x) == np.isfinite(x)

    @given(floats(allow_nan=False, allow_infinity=False))
    def test_floorq(self, x):
        assert float(qm.floorq(x)) == pytest.approx(math.floor(x), nan_ok=True)

    @given(
        floats(min_value=-1e100, max_value=1e100),
        floats(min_value=-1e100, max_value=1e100),
        floats(min_value=-1e50, max_value=1e50),
    )
    def test_fmaq(self, x, y, z):
        a = x * y + z
        assert float(qm.fmaq(x, y, z)) == pytest.approx(a, nan_ok=True)

    @given(
        floats(),
        floats(),
    )
    def test_fmaxq(self, x, y):
        z = qm.fmaxq(x, y)
        if qm.isnan(x) and qm.isnan(y):
            assert qm.isnan(z)
        elif qm.isnan(x):
            assert z == y
        elif qm.isnan(y):
            assert z == x
        elif x == -np.inf and y == -np.inf:
            assert qm.fmaxq(x, y) == -np.inf
        elif x >= y:
            assert z == x
        else:
            assert z == y

    @given(
        floats(),
        floats(),
    )
    def test_fminq(self, x, y):
        z = qm.fminq(x, y)

        if x == -np.inf or y == -np.inf:
            assert qm.fminq(x, y) == -np.inf
        elif qm.isnan(x) and qm.isnan(y):
            assert qm.isnan(z)
        elif qm.isnan(x):
            assert z == y
        elif qm.isnan(y):
            assert z == x
        elif x <= y:
            assert z == x
        else:
            assert z == y

    @given(
        floats(min_value=1),
        floats(min_value=1),
    )
    def test_fmodq(self, x, y):
        try:
            assert float(qm.fmodq(x, y)) == pytest.approx(math.fmod(x, y), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x) or not np.isfinite(y)

    @given(floats())
    def test_frexpq(self, x):
        assume(x > -np.inf)
        y = qm.frexpq(x)
        y1 = float(y[0])
        y2 = y[1]

        z = math.frexp(x)
        z1 = z[0]
        z2 = z[1]

        assert float(y1) == pytest.approx(z1, nan_ok=True)
        assert y2 == z2

    @given(
        floats(min_value=-1e100, max_value=1e100),
        floats(min_value=-1e100, max_value=1e100),
    )
    def test_hypotq(self, x, y):
        assert float(qm.hypotq(x, y)) == pytest.approx(math.hypot(x, y), nan_ok=True)

    @given(floats(min_value=1, max_value=1e10))
    def test_ilogbq(self, x):
        assert qm.ilogbq(x) == int(math.log2(math.fabs(x)))

    @given(floats())
    def test_isinfq(self, x):
        assert qm.isinfq(x) == math.isinf(x)

    @given(floats())
    def test_isnanq(self, x):
        assert qm.isnanq(x) == math.isnan(x)

    def test_issignalingq(self):
        try:
            assert not qm.issignalingq("nan")
            assert not qm.issignalingq("0")
        except NotImplementedError:
            pass

    @given(floats(min_value=1, max_value=10))
    def test_j0q(self, x):
        assert float(qm.j0q(x)) == pytest.approx(
            scipy.special.j0(x), rel=1e-3, nan_ok=True
        )

    @given(floats(min_value=1, max_value=10))
    def test_j1q(self, x):
        assert float(qm.j1q(x)) == pytest.approx(
            scipy.special.j1(x), rel=1e-3, nan_ok=True
        )

    @given(
        integers(min_value=-2, max_value=2),
        floats(min_value=1, max_value=10),
    )
    def test_jnq(self, x, y):
        assert float(qm.jnq(x, y)) == pytest.approx(
            scipy.special.jn(x, y), rel=1e-3, nan_ok=True
        )

    @given(
        floats(max_value=1e100, min_value=-1e100),
        integers(min_value=-100, max_value=100),
    )
    def test_ldexpq(self, x, n):
        assert float(qm.ldexpq(x, n)) == pytest.approx(math.ldexp(x, n), nan_ok=True)

    @given(floats(min_value=1, max_value=1e100))
    def test_lgammaq(self, x):
        assume(x != 0)
        assert float(qm.lgammaq(x)) == pytest.approx(
            math.lgamma(math.fabs(x)), nan_ok=True
        )

    @given(floats(min_value=-1e10, max_value=1e10))
    def test_llrintq(self, x):
        assert qm.llrintq(x) == int(round(x, 0))

    @given(floats(min_value=-1e10, max_value=1e10))
    def test_llroundq(self, x):
        xd = decimal.Decimal(x)
        assert qm.llroundq(x) == int(
            xd.quantize(decimal.Decimal("1"), rounding=decimal.ROUND_HALF_UP)
        )

    @given(floats(min_value=1, max_value=1e10))
    def test_logbq(self, x):
        assert float(qm.logbq(x)) == int(math.log2(math.fabs(x)))

    @given(floats(min_value=0, exclude_min=True))
    def test_logq(self, x):
        assert float(qm.logq(x)) == pytest.approx(math.log(x), nan_ok=True)

    @given(floats(min_value=0, exclude_min=True))
    def test_log10q(self, x):
        assert float(qm.log10q(x)) == pytest.approx(math.log10(x), nan_ok=True)

    @given(floats(min_value=0, exclude_min=True))
    def test_log1pq(self, x):
        assert float(qm.log1pq(x)) == pytest.approx(math.log1p(x), nan_ok=True)

    @given(floats(min_value=0, exclude_min=True))
    def test_log2q(self, x):
        assert float(qm.log2q(x)) == pytest.approx(math.log2(x), nan_ok=True)

    @given(floats(min_value=-1e10, max_value=1e10))
    def test_lrintq(self, x):
        assert qm.lrintq(x) == int(round(x, 0))

    @given(floats(min_value=-1e10, max_value=1e10))
    def test_lroundq(self, x):
        xd = decimal.Decimal(x)
        assert qm.lroundq(x) == int(
            xd.quantize(decimal.Decimal("1"), rounding=decimal.ROUND_HALF_UP)
        )

    @given(floats())
    def test_modfq(self, x):
        y = qm.modfq(x)
        y1 = float(y[0])
        y2 = y[1]

        z = math.modf(x)
        z1 = z[0]
        z2 = z[1]

        if np.isnan(x):
            assert qm.isnan(y1) == np.isnan(z1)
            assert qm.isnan(y2) == np.isnan(z2)
        else:
            assert y1 == z1
            assert y2 == z2

    def test_nanq(self):
        assert np.isnan(float(qm.nanq()))

    @given(integers(min_value=-1e10, max_value=1e10))
    def test_nearbyintq(self, x):
        xd = decimal.Decimal(x)
        assert qm.nearbyintq(x) == x

    @given(floats(allow_infinity=False), floats())
    def test_nextafterq(self, x, y):
        assert float(qm.nextafterq(x, y)) == pytest.approx(
            math.nextafter(x, y), nan_ok=True
        )

    @given(
        floats(min_value=0, max_value=100),
        floats(min_value=0, max_value=100),
    )
    def test_powq(self, x, y):
        assert float(qm.powq(x, y)) == pytest.approx(math.pow(x, y), nan_ok=True)

    @given(
        floats(min_value=1),
        floats(min_value=1),
    )
    def test_remainderq(self, x, y):
        try:
            assert float(qm.remainderq(x, y)) == pytest.approx(
                math.remainder(x, y), nan_ok=True
            )
        except ValueError:
            assert not np.isfinite(x) or not np.isfinite(y)

    @given(
        floats(min_value=0.1),
        floats(min_value=0.1),
    )
    def test_remquoq(self, x, y):
        try:
            assert float(qm.remquoq(x, y)[0]) == pytest.approx(
                math.remainder(x, y), nan_ok=True
            )
        except ValueError:
            assert not np.isfinite(x) or not np.isfinite(y)

    @given(integers(min_value=-1e10, max_value=1e10))
    def test_rintq(self, x):
        xd = decimal.Decimal(x)
        assert float(qm.rintq(x)) == pytest.approx(
            float(xd.quantize(decimal.Decimal("1"), rounding=decimal.ROUND_HALF_DOWN)),
            nan_ok=True,
        )

    @given(floats(min_value=-1e10, max_value=1e10))
    def test_roundq(self, x):
        xd = decimal.Decimal(x)
        assert float(qm.roundq(x)) == pytest.approx(
            float(xd.quantize(decimal.Decimal("1"), rounding=decimal.ROUND_HALF_UP)),
            nan_ok=True,
        )

    @given(
        floats(max_value=1e100, min_value=-1e100),
        integers(min_value=-100, max_value=100),
    )
    def test_scalblnq(self, x, n):
        assert float(qm.scalblnq(x, n)) == pytest.approx(math.ldexp(x, n), nan_ok=True)

    @given(
        floats(max_value=1e100, min_value=-1e100),
        integers(min_value=-100, max_value=100),
    )
    def test_scalbnq(self, x, n):
        assert float(qm.scalbnq(x, n)) == pytest.approx(math.ldexp(x, n), nan_ok=True)

    @given(floats(allow_nan=False, allow_infinity=False))
    def test_signbitq(self, x):
        sb = qm.signbitq(x)
        if np.isinf(x):
            if x > 0:
                assert sb == 0
            else:
                assert sb == 8
        elif x == 0:
            if math.copysign(1, x) > 0:
                # +0
                assert sb == 0
            else:
                # -0
                assert sb != 0
        else:
            if x > 0:
                assert sb == 0
            else:
                assert sb != 0

    @given(floats())
    def test_sincosq(self, x):
        y = qm.sincosq(x)
        y1 = float(y[0])
        y2 = float(y[1])

        try:
            z1 = math.sin(x)
            z2 = math.cos(x)
        except ValueError:
            assert not np.isfinite(x)
            return

        assert y1 == pytest.approx(z1, nan_ok=True)
        assert y2 == pytest.approx(z2, nan_ok=True)

    @given(floats(min_value=-10, max_value=10))
    def test_sinhq(self, x):
        assert float(qm.sinhq(x)) == pytest.approx(math.sinh(x), nan_ok=True)

    @given(floats())
    def test_sinq(self, x):
        try:
            assert float(qm.sinq(x)) == pytest.approx(math.sin(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats(min_value=0, exclude_min=True))
    def test_sqrtq(self, x):
        assert float(qm.sqrtq(x)) == pytest.approx(math.sqrt(x), nan_ok=True)

    @given(floats())
    def test_tanq(self, x):
        try:
            assert float(qm.tanq(x)) == pytest.approx(math.tan(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_tanhq(self, x):
        assert float(qm.tanhq(x)) == pytest.approx(math.tanh(x), nan_ok=True)

    @given(
        floats(
            allow_infinity=False,
            allow_nan=False,
            min_value=1,
            exclude_min=True,
            max_value=100,
        )
    )
    def test_tgammaq(self, x):
        assert float(qm.tgammaq(x)) == pytest.approx(math.gamma(x), nan_ok=True)

    @given(floats(allow_infinity=False, allow_nan=False))
    def test_truncq(self, x):
        assert float(qm.truncq(x)) == pytest.approx(math.trunc(x), nan_ok=True)

    @given(floats(min_value=1, max_value=10))
    def test_y0q(self, x):
        assert float(qm.y0q(x)) == pytest.approx(
            scipy.special.y0(x), nan_ok=True, rel=1e-3
        )

    @given(floats(min_value=1, max_value=10))
    def test_y1q(self, x):
        assert float(qm.y1q(x)) == pytest.approx(
            scipy.special.y1(x), nan_ok=True, rel=1e-3
        )

    @given(
        integers(min_value=-2, max_value=2),
        floats(min_value=1, max_value=10),
    )
    def test_ynq(self, n, x):
        assert float(qm.ynq(n, x)) == pytest.approx(
            scipy.special.yn(n, x), nan_ok=True, rel=1e-3
        )


######################################################################################


class TestQMathPy:
    @given(floats(min_value=-1, max_value=1))
    def test_acos(self, x):
        assert float(qm.acos(x)) == pytest.approx(math.acos(x), nan_ok=True)

    @given(floats(min_value=1))
    def test_acosh(self, x):
        try:
            assert float(qm.acosh(x)) == pytest.approx(math.acosh(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats(min_value=-1, max_value=1))
    def test_asin(self, x):
        assert float(qm.asin(x)) == pytest.approx(math.asin(x), nan_ok=True)

    @given(floats())
    def test_asinh(self, x):
        try:
            assert float(qm.asinh(x)) == pytest.approx(math.asinh(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats(min_value=-1, max_value=1))
    def test_atan(self, x):
        assert float(qm.atan(x)) == pytest.approx(math.atan(x), nan_ok=True)

    @given(
        floats(
            min_value=-1,
            max_value=1,
            exclude_min=True,
            exclude_max=True,
        )
    )
    def test_atanh(self, x):
        assert float(qm.atanh(x)) == pytest.approx(math.atanh(x), nan_ok=True)

    @given(
        floats(),
        floats(),
    )
    def test_atan2(self, x, y):
        try:
            assert float(qm.atan2(x, y)) == pytest.approx(math.atan2(x, y), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x) or not np.isfinite(y)

    @given(floats())
    def test_cbrt(self, x):
        try:
            assert float(qm.cbrt(x)) == pytest.approx(np.cbrt(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_ceil(self, x):
        try:
            assert float(qm.ceil(x)) == pytest.approx(math.ceil(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(
        floats(),
        floats(),
    )
    def test_copysign(self, x, y):
        try:
            assert float(qm.copysign(x, y)) == pytest.approx(
                math.copysign(x, y), nan_ok=True
            )
        except ValueError:
            assert not np.isfinite(x)

    @given(floats(min_value=1, max_value=10))
    def test_cosh(self, x):
        assert float(qm.cosh(x)) == pytest.approx(math.cosh(x), nan_ok=True)

    @given(floats())
    def test_cos(self, x):
        try:
            assert float(qm.cos(x)) == pytest.approx(math.cos(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_erf(self, x):
        try:
            assert float(qm.erf(x)) == pytest.approx(math.erf(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_erfc(self, x):
        try:
            assert float(qm.erfc(x)) == pytest.approx(math.erfc(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @pytest.mark.skipif(
        sys.version_info < (3, 11), reason="requires python3.11 or higher"
    )
    @given(floats(max_value=100))
    def test_exp2(self, x):
        assert float(qm.exp2(x)) == pytest.approx(math.exp2(x), nan_ok=True)

    @given(floats(max_value=100))
    def test_exp(self, x):
        assert float(qm.exp(x)) == pytest.approx(math.exp(x), nan_ok=True)

    @given(floats(max_value=100))
    def test_expm1(self, x):
        assert float(qm.expm1(x)) == pytest.approx(math.expm1(x), nan_ok=True)

    @given(floats())
    def test_fabs(self, x):
        try:
            assert float(qm.fabs(x)) == pytest.approx(math.fabs(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_floor(self, x):
        try:
            assert float(qm.floor(x)) == pytest.approx(math.floor(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(
        floats(min_value=1),
        floats(min_value=1),
    )
    def test_fmod(self, x, y):
        try:
            assert float(qm.fmod(x, y)) == pytest.approx(math.fmod(x, y), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_frexp(self, x):
        try:
            y = qm.frexp(x)
        except ValueError:
            assert not np.isfinite(x)
            return
        y1 = float(y[0])
        y2 = y[1]

        z = math.frexp(x)
        z1 = z[0]
        z2 = z[1]

        assert float(y1) == pytest.approx(z1, nan_ok=True)
        assert y2 == z2

    @given(
        floats(min_value=-1e100, max_value=1e100),
        floats(min_value=-1e100, max_value=1e100),
    )
    def test_hypot(self, x, y):
        assert float(qm.hypot(x, y)) == pytest.approx(math.hypot(x, y), nan_ok=True)

    @given(floats())
    def test_isinf(self, x):
        assert qm.isinf(x) == math.isinf(x)

    @given(floats())
    def test_isnan(self, x):
        assert qm.isnan(x) == math.isnan(x)

    @given(
        floats(max_value=1e100, min_value=-1e100),
        integers(min_value=-100, max_value=100),
    )
    def test_ldexp(self, x, n):
        assert float(qm.ldexp(x, n)) == pytest.approx(math.ldexp(x, n), nan_ok=True)

    @given(floats(min_value=1, max_value=1e100))
    def test_lgamma(self, x):
        assume(x != 0)
        assert float(qm.lgamma(x)) == pytest.approx(
            math.lgamma(math.fabs(x)), nan_ok=True
        )

    @given(floats(min_value=0, exclude_min=True))
    def test_log(self, x):
        try:
            assert float(qm.log(x)) == pytest.approx(math.log(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats(min_value=0, exclude_min=True))
    def test_log10(self, x):
        try:
            assert float(qm.log10(x)) == pytest.approx(math.log10(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats(min_value=0, exclude_min=True))
    def test_log1p(self, x):
        try:
            assert float(qm.log1p(x)) == pytest.approx(math.log1p(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats(min_value=0, exclude_min=True))
    def test_log2(self, x):
        try:
            assert float(qm.log2(x)) == pytest.approx(math.log2(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_modf(self, x):
        try:
            y = qm.modf(x)
        except ValueError:
            assert not np.isfinite(x)
            return
        y1 = float(y[0])
        y2 = y[1]

        try:
            z = math.modf(x)
        except ValueError:
            assert not np.isfinite(x)
            return
        z1 = z[0]
        z2 = z[1]

        assert y1 == z1
        assert y2 == z2

    @given(
        floats(allow_infinity=False),
        floats(),
    )
    def test_nextafter(self, x, y):
        try:
            assert float(qm.nextafter(x, y)) == pytest.approx(
                math.nextafter(x, y), nan_ok=True
            )
        except ValueError:
            assert not np.isfinite(x) or not np.isfinite(y)

    @given(
        floats(min_value=0, max_value=100),
        floats(min_value=0, max_value=100),
    )
    def test_pow(self, x, y):
        assert float(qm.pow(x, y)) == pytest.approx(math.pow(x, y), nan_ok=True)

    @given(
        floats(min_value=1),
        floats(min_value=1),
    )
    def test_remainder(self, x, y):
        try:
            assert float(qm.remainder(x, y)) == pytest.approx(
                math.remainder(x, y), nan_ok=True
            )
        except ValueError:
            assert not np.isfinite(x)

    @given(floats(min_value=-10, max_value=10))
    def test_sinh(self, x):
        assert float(qm.sinh(x)) == pytest.approx(math.sinh(x), nan_ok=True)

    @given(floats())
    def test_sin(self, x):
        try:
            assert float(qm.sin(x)) == pytest.approx(math.sin(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats(min_value=0, exclude_min=True))
    def test_sqrt(self, x):
        try:
            assert float(qm.sqrt(x)) == pytest.approx(math.sqrt(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_tan(self, x):
        try:
            assert float(qm.tan(x)) == pytest.approx(math.tan(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_tanh(self, x):
        try:
            assert float(qm.tanh(x)) == pytest.approx(math.tanh(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)

    @given(floats())
    def test_trunc(self, x):
        try:
            assert float(qm.trunc(x)) == pytest.approx(math.trunc(x), nan_ok=True)
        except ValueError:
            assert not np.isfinite(x)
