from .constant import *
import qcmathc as _qcmathc


def cabsq(x):
    return _qcmathc._cabs(x)


def cargq(x):
    return _qcmathc._carg(x)


def cimagq(x):
    return _qcmathc._cimag(x)


def crealq(x):
    return _qcmathc._creal(x)


def cacoshq(x):
    return _qcmathc._cacosh(x)


def cacosq(x):
    return _qcmathc._cacos(x)


def casinhq(x):
    return _qcmathc._casinh(x)


def casinq(x):
    return _qcmathc._casin(x)


def catanhq(x):
    return _qcmathc._catanh(x)


def catanq(x):
    return _qcmathc._catan(x)


def ccosq(x):
    return _qcmathc._ccos(x)


def ccoshq(x):
    return _qcmathc._ccosh(x)


def cexpq(x):
    return _qcmathc._cexp(x)


def cexpiq(x):
    return _qcmathc._cexpi(x)


def clogq(x):
    return _qcmathc._clog(x)


def clog10q(x):
    return _qcmathc._clog10(x)


def conjq(x):
    return _qcmathc._conj(x)


def cpowq(x, y):
    return _qcmathc._cpow(x, y)


def cprojq(x):
    return _qcmathc._cproj(x)


def csinq(x):
    return _qcmathc._csin(x)


def csinhq(x):
    return _qcmathc._csinh(x)


def csqrtq(x):
    return _qcmathc._csqrt(x)


def ctanq(x):
    return _qcmathc._ctan(x)


def ctanhq(x):
    return _qcmathc._ctanh(x)


##################################
# Python cmath library functions #
##################################


def phase(x):
    return _qcmathc._phase(x)


def polar(x):
    return _qcmathc._polar(x)


def rect(r, phi):
    return _qcmathc._polar(r, phi)


def acos(x):
    res = _qcmathc._cacos(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def acosh(x):
    res = _qcmathc._cacosh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def asin(x):
    res = _qcmathc._casin(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def asinh(x):
    res = _qcmathc._casinh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def atan(x):
    res = _qcmathc._catan(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def atanh(x):
    res = _qcmathc._catanh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def cosh(x):
    res = _qcmathc._ccosh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def cos(x):
    res = _qcmathc._ccos(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def exp(x):
    res = _qcmathc._cexp(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def log(x, base=None):
    res = _qcmathc._clog(x)
    if not isfinite(res):
        raise ValueError("math domain error")

    if base is not None:
        res = res / _qcmathc._clog(base)

    if not isfinite(res):
        raise ValueError("math domain error")

    return res


def log10(x):
    res = _qcmathc._clog10(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def pow(x, y):
    res = _qcmathc._cpow(x, y)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def sinh(x):
    res = _qcmathc._csinh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def sin(x):
    res = _qcmathc._csin(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def sqrt(x):
    res = _qcmathc._csqrt(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def tan(x):
    res = _qcmathc._ctan(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def tanh(x):
    res = _qcmathc._ctanh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def isfinite(x):
    return _qcmathc._finitecq(x)


def isnan(x):
    return _qcmathc._isnancq(x)


def isinf(x):
    return _qcmathc._isinfcq(x)


def isclose(a, b, *, rel_tol=1e-09, abs_tol=0.0):
    raise NotImplementedError
