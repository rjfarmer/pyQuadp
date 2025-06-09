from .constant import *
import pyquadp.qmathc as qmathc
import math as _math


def acosq(x):
    return qmathc._acos(x)


def acoshq(x):
    return qmathc._acosh(x)


def asinq(x):
    return qmathc._asin(x)


def asinhq(x):
    return qmathc._asinh(x)


def atanq(x):
    return qmathc._atan(x)


def atanhq(x):
    return qmathc._atanh(x)


def atan2q(x, y):
    return qmathc._atan2(x, y)


def cbrtq(x):
    return qmathc._cbrt(x)


def ceilq(x):
    return qmathc._ceil(x)


def copysignq(x, y):
    return qmathc._copysign(x, y)


def coshq(x):
    return qmathc._cosh(x)


def cosq(x):
    return qmathc._cos(x)


def erfq(x):
    return qmathc._erf(x)


def erfcq(x):
    return qmathc._erfc(x)


def exp2q(x):
    return qmathc._exp2(x)


def expq(x):
    return qmathc._exp(x)


def expm1q(x):
    return qmathc._expm1(x)


def fdimq(x, y):
    return qmathc._fdim(x, y)


def fabsq(x):
    return qmathc._fabs(x)


def finiteq(x):
    return qmathc._finite(x)


def floorq(x):
    return qmathc._floor(x)


def fmaq(x, y, z):
    return qmathc._fma(x, y, z)


def fmaxq(x, y):
    return qmathc._fmax(x, y)


def fminq(x, y):
    return qmathc._fmin(x, y)


def fmodq(x, y):
    return qmathc._fmod(x, y)


def frexpq(x):
    return qmathc._frexp(x)


def hypotq(x, y):
    return qmathc._hypot(x, y)


def ilogbq(x):
    return qmathc._ilogb(x)


def isinfq(x):
    return qmathc._isinf(x)


def isnanq(x):
    return qmathc._isnan(x)


def issignalingq(x):
    return qmathc._issignaling(x)


def j0q(x):
    return qmathc._j0(x)


def j1q(x):
    return qmathc._j1(x)


def jnq(n, x):
    return qmathc._jn(n, x)


def ldexpq(x, i):
    return qmathc._ldexp(x, i)


def lgammaq(x):
    return qmathc._lgamma(x)


def llrintq(x):
    return qmathc._llrint(x)


def llroundq(x):
    return qmathc._llround(x)


def logbq(x):
    return qmathc._logb(x)


def logq(x):
    return qmathc._log(x)


def log10q(x):
    return qmathc._log10(x)


def log1p(x):
    return qmathc._log1p(x)


def log1pq(x):
    return qmathc._log1p(x)


def log2q(x):
    return qmathc._log2(x)


def lrintq(x):
    return qmathc._lrint(x)


def lroundq(x):
    return qmathc._lround(x)


def modfq(x):
    return qmathc._modf(x)


def nanq(x=None):
    return qfloat("nan")


def nearbyintq(x):
    return qmathc._nearbyint(x)


def nextafterq(x, y):
    return qmathc._nextafter(x, y)


def powq(x, y):
    return qmathc._pow(x, y)


def remainderq(x, y):
    return qmathc._remainder(x, y)


def remquoq(x, y):
    return qmathc._remquo(x, y)


def rintq(x):
    return qmathc._rint(x)


def roundq(x):
    return qmathc._round(x)


def scalbnq(x, n):
    return qmathc._scalbn(x, n)


def scalblnq(x, n):
    return qmathc._scalbln(x, n)


def signbitq(x):
    return qmathc._signbit(x)


def sincosq(x):
    return qmathc._sincos(x)


def sinh(x):
    return qmathc._sinh(x)


def sinhq(x):
    return qmathc._sinh(x)


def sinq(x):
    return qmathc._sin(x)


def sqrtq(x):
    return qmathc._sqrt(x)


def tanq(x):
    return qmathc._tan(x)


def tanh(x):
    return qmathc._tanh(x)


def tanhq(x):
    return qmathc._tanh(x)


def tgammaq(x):
    return qmathc._tgamma(x)


def trunc(x):
    return qmathc._trunc(x)


def truncq(x):
    return qmathc._trunc(x)


def y0q(x):
    return qmathc._y0(x)


def y1q(x):
    return qmathc._y1(x)


def ynq(n, x):
    return qmathc._yn(n, x)


#################################
# Python math library functions #
#################################


def acos(x):
    res = qmathc._acos(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def acosh(x):
    res = qmathc._acosh(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def asin(x):
    res = qmathc._asin(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def asinh(x):
    res = qmathc._asinh(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def atan(x):
    res = qmathc._atan(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def atanh(x):
    res = qmathc._atanh(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def atan2(x, y):
    res = qmathc._atan2(x, y)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def cbrt(x):
    res = qmathc._cbrt(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def ceil(x):
    res = qmathc._ceil(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def copysign(x, y):
    res = qmathc._copysign(x, y)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def cosh(x):
    res = qmathc._cosh(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def cos(x):
    res = qmathc._cos(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def erf(x):
    res = qmathc._erf(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def erfc(x):
    res = qmathc._erfc(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def exp2(x):
    res = qmathc._exp2(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def exp(x):
    res = qmathc._exp(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def expm1(x):
    res = qmathc._expm1(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def fabs(x):
    res = qmathc._fabs(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def floor(x):
    res = qmathc._floor(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def fmax(x):
    res = qmathc._fmax(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def fmin(x):
    res = qmathc._fmin(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def fmod(x, y):
    res = qmathc._fmod(x, y)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def frexp(x):
    res = qmathc._frexp(x)
    if not qmathc._finite(res[0]):
        raise ValueError("math domain error")
    return res


def hypot(*iterable):
    # Naive way of doing things
    sum = qfloat(0)
    for i in iterable:
        sum += qfloat(i) ** 2

    res = qmathc._sqrt(sum)

    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def isinf(x):
    res = qmathc._isinf(x)
    return res


def isnan(x):
    res = qmathc._isnan(x)
    return res


def ldexp(x, y):
    res = qmathc._ldexp(x, y)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def lgamma(x):
    res = qmathc._lgamma(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def log(x, base=None):
    res = qmathc._log(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")

    if base is not None:
        res = res / qmathc._log(base)

    if not qmathc._finite(res):
        raise ValueError("math domain error")

    return res


def log10(x):
    res = qmathc._log10(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def log1p(x):
    res = qmathc._log1p(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def log2(x):
    res = qmathc._log2(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def modf(x):
    res = qmathc._modf(x)
    if not qmathc._finite(res[0]):
        raise ValueError("math domain error")
    return res


def nextafter(x, y):
    res = qmathc._nextafter(x, y)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def pow(x, y):
    res = qmathc._pow(x, y)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def remainder(x, y):
    res = qmathc._remainder(x, y)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def sinh(x):
    res = qmathc._sinh(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def sin(x):
    res = qmathc._sin(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def sqrt(x):
    res = qmathc._sqrt(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def tan(x):
    res = qmathc._tan(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def tanh(x):
    res = qmathc._tanh(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def trunc(x):
    res = qmathc._trunc(x)
    if not qmathc._finite(res):
        raise ValueError("math domain error")
    return res


def comb(n, k):
    return _math.comb(n, k)


def factorial(n):
    return _math.comb(n)


def fsum(*iterable):
    return _math.fsum(iterable)


def gcd(*integers):
    return _math.gcd(integers)


def isclose(a, b, *, rel_tol=1e-09, abs_tol=0.0):
    raise NotImplementedError


def isfinite(x):
    return qmathc._finitecq(x)


def isqrt(n):
    return floorq(sqrtq(n))


def lcm(*integers):
    return _math.lcm(integers)


def perm(iterable, *, start=1):
    return _math.perm(iterable, start=start)


def prod(iterable, *, start=1):
    return _math.perm(iterable, start=start)


def ulp(x):
    raise NotImplementedError


def dist(p, q):
    return _math.dist(p, q)


def degrees(x):
    return qmathc.qfloat(x) * qmathc.qfloat(180.0) / M_PIq


def radians(x):
    return qmathc.qfloat(x) * M_PIq / qmathc.qfloat(180.0)
