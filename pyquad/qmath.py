from .constant import *
import qmathc as _qmathc


def acos(x):
    return _qmathc._acos(x)


def acosq(x):
    return _qmathc._acos(x)


def acosh(x):
    return _qmathc._acosh(x)


def acoshq(x):
    return _qmathc._acosh(x)


def asin(x):
    return _qmathc._asin(x)


def asinq(x):
    return _qmathc._asin(x)


def asinh(x):
    return _qmathc._asinh(x)


def asinhq(x):
    return _qmathc._asinh(x)


def atan(x):
    return _qmathc._atan(x)


def atanq(x):
    return _qmathc._atan(x)


def atanh(x):
    return _qmathc._atanh(x)


def atanhq(x):
    return _qmathc._atanh(x)


def atan2(x, y):
    return _qmathc._atan2(x, y)


def atan2q(x, y):
    return _qmathc._atan2(x, y)


def cbrt(x):
    return _qmathc._cbrt(x)


def cbrtq(x):
    return _qmathc._cbrt(x)


def ceil(x):
    return _qmathc._ceil(x)


def ceilq(x):
    return _qmathc._ceil(x)


def copysign(x, y):
    return _qmathc._copysign(x, y)


def copysignq(x, y):
    return _qmathc._copysign(x, y)


def cosh(x):
    return _qmathc._cosh(x)


def coshq(x):
    return _qmathc._cosh(x)


def cos(x):
    return _qmathc._cos(x)


def cosq(x):
    return _qmathc._cos(x)


def erf(x):
    return _qmathc._erf(x)


def erfq(x):
    return _qmathc._erf(x)


def erfc(x):
    return _qmathc._erfc(x)


def erfcq(x):
    return _qmathc._erfc(x)


def exp2(x):
    return _qmathc._exp2(x)


def exp2q(x):
    return _qmathc._exp2(x)


def exp(x):
    return _qmathc._exp(x)


def expq(x):
    return _qmathc._exp(x)


def expm1(x):
    return _qmathc._expm1(x)


def expm1q(x):
    return _qmathc._expm1(x)


def fabs(x):
    return _qmathc._fabs(x)


def fabsq(x):
    return _qmathc._fabs(x)


def fdimq(x, y):
    return _qmathc._fdim(x, y)


def finiteq(x):
    return _qmathc._finite(x)


def floor(x):
    return _qmathc._floor(x)


def floorq(x):
    return _qmathc._floor(x)


def fmaq(x, y, z):
    return _qmathc._fma(x, y, z)


def fmaxq(x, y):
    return _qmathc._fmax(x, y)


def fminq(x, y):
    return _qmathc._fmin(x, y)


def fmod(x, y):
    return _qmathc._fmod(x, y)


def fmodq(x, y):
    return _qmathc._fmod(x, y)


def frexp(x):
    return _qmathc._frexp(x)


def frexpq(x):
    return _qmathc._frexp(x)


def hypot(x, y):
    return _qmathc._hypot(x, y)


def hypotq(x, y):
    return _qmathc._hypot(x, y)


def ilogbq(x):
    return _qmathc._ilogb(x)


def isinf(x):
    return _qmathc._isinf(x)


def isinfq(x):
    return _qmathc._isinf(x)


def isnan(x):
    return _qmathc._isnan(x)


def isnanq(x):
    return _qmathc._isnan(x)


def issignalingq(x):
    return _qmathc._issignaling(x)


def j0q(x):
    return _qmathc._j0(x)


def j1q(x):
    return _qmathc._j1(x)


def jnq(n, x):
    return _qmathc._jn(n, x)


def ldexp(x, i):
    return _qmathc._ldexp(x, i)


def ldexpq(x, i):
    return _qmathc._ldexp(x, i)


def lgamma(x):
    if x < 0.0:
        raise ValueError("math domain error")

    return _qmathc._lgamma(x)


def lgammaq(x):
    """_summary_

    Args:
        x (quad-like): _description_

    Returns:
        lgamma (qfloat): base e logarithm of the abs value of the gamma function of x
    """
    return _qmathc._lgamma(x)


def llrintq(x):
    return _qmathc._llrint(x)


def llroundq(x):
    return _qmathc._llround(x)


def logbq(x):
    return _qmathc._logb(x)


def log(x, base=None):
    return _qmathc._log(x)


def logq(x):
    return _qmathc._log(x)


def log10(x):
    return _qmathc._log10(x)


def log10q(x):
    return _qmathc._log10(x)


def log1p(x):
    return _qmathc._log1p(x)


def log1pq(x):
    return _qmathc._log1p(x)


def log2(x):
    return _qmathc._log2(x)


def log2q(x):
    return _qmathc._log2(x)


def lrintq(x):
    return _qmathc._lrint(x)


def lroundq(x):
    return _qmathc._lround(x)


def modf(x):
    return _qmathc._modf(x)


def modfq(x):
    return _qmathc._modf(x)


def nanq(x=None):
    return nan


def nearbyintq(x):
    return _qmathc._nearbyint(x)


def nextafter(x, y):
    return _qmathc._nextafter(x, y)


def nextafterq(x, y):
    return _qmathc._nextafter(x, y)


def pow(x, y):
    return _qmathc._pow(x, y)


def powq(x, y):
    return _qmathc._pow(x, y)


def remainder(x, y):
    return _qmathc._remainder(x, y)


def remainderq(x, y):
    return _qmathc._remainder(x, y)


def remquoq(x, y):
    return _qmathc._remquo(x, y)


def rintq(x):
    return _qmathc._rint(x)


def roundq(x):
    return _qmathc._round(x)


def scalbnq(x, n):
    return _qmathc._scalbn(x, n)


def scalblnq(x, n):
    return _qmathc._scalbln(x, n)


def signbitq(x):
    return _qmathc._signbit(x)


def sincosq(x):
    return _qmathc._sincos(x)


def sinh(x):
    return _qmathc._sinh(x)


def sinhq(x):
    return _qmathc._sinh(x)


def sin(x):
    return _qmathc._sin(x)


def sinq(x):
    return _qmathc._sin(x)


def sqrt(x):
    return _qmathc._sqrt(x)


def sqrtq(x):
    return _qmathc._sqrt(x)


def tan(x):
    return _qmathc._tan(x)


def tanq(x):
    return _qmathc._tan(x)


def tanh(x):
    return _qmathc._tanh(x)


def tanhq(x):
    return _qmathc._tanh(x)


def tgammaq(x):
    return _qmathc._tgamma(x)


def trunc(x):
    return _qmathc._trunc(x)


def truncq(x):
    return _qmathc._trunc(x)


def y0q(x):
    return _qmathc._y0(x)


def y1q(x):
    return _qmathc._y1(x)


def ynq(n, x):
    return _qmathc._yn(n, x)


#################################
# Python math library functions #
#################################


def comb(n, k):
    raise NotImplementedError


def factorial(n):
    raise NotImplementedError


def fsum(iterable):
    raise NotImplementedError


def gcd(integers):
    raise NotImplementedError


def isclose(a, b, *, rel_tol=1e-09, abs_tol=0.0):
    raise NotImplementedError


def isfinite(x):
    raise NotImplementedError


def isqrt(n):
    raise NotImplementedError


def lcm(integers):
    raise NotImplementedError


def perm(iterable, *, start=1):
    raise NotImplementedError


def prod(iterable, *, start=1):
    raise NotImplementedError


def ulp(x):
    raise NotImplementedError


def dist(p, q):
    raise NotImplementedError


def degrees(x):
    raise NotImplementedError


def radians(x):
    raise NotImplementedError
