from .constant import *
import pyquadp.qcmathc as qcmathc


def cabsq(x):
    return qcmathc._cabs(x)


def cargq(x):
    return qcmathc._carg(x)


def cimagq(x):
    return qcmathc._cimag(x)


def crealq(x):
    return qcmathc._creal(x)


def cacoshq(x):
    return qcmathc._cacosh(x)


def cacosq(x):
    return qcmathc._cacos(x)


def casinhq(x):
    return qcmathc._casinh(x)


def casinq(x):
    return qcmathc._casin(x)


def catanhq(x):
    return qcmathc._catanh(x)


def catanq(x):
    return qcmathc._catan(x)


def ccosq(x):
    return qcmathc._ccos(x)


def ccoshq(x):
    return qcmathc._ccosh(x)


def cexpq(x):
    return qcmathc._cexp(x)


def cexpiq(x):
    return qcmathc._cexpi(x)


def clogq(x):
    return qcmathc._clog(x)


def clog10q(x):
    return qcmathc._clog10(x)


def conjq(x):
    return qcmathc._conj(x)


def cpowq(x, y):
    return qcmathc._cpow(x, y)


def cprojq(x):
    return qcmathc._cproj(x)


def csinq(x):
    return qcmathc._csin(x)


def csinhq(x):
    return qcmathc._csinh(x)


def csqrtq(x):
    return qcmathc._csqrt(x)


def ctanq(x):
    return qcmathc._ctan(x)


def ctanhq(x):
    return qcmathc._ctanh(x)


##################################
# Python cmath library functions #
##################################


def phase(x):
    return qcmathc._phase(x)


def polar(x):
    return qcmathc._polar(x)


def rect(r, phi):
    return qcmathc._polar(r, phi)


def acos(x):
    res = qcmathc._cacos(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def acosh(x):
    res = qcmathc._cacosh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def asin(x):
    res = qcmathc._casin(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def asinh(x):
    res = qcmathc._casinh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def atan(x):
    res = qcmathc._catan(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def atanh(x):
    res = qcmathc._catanh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def cosh(x):
    res = qcmathc._ccosh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def cos(x):
    res = qcmathc._ccos(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def exp(x):
    res = qcmathc._cexp(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def log(x, base=None):
    res = qcmathc._clog(x)
    if not isfinite(res):
        raise ValueError("math domain error")

    if base is not None:
        res = res / qcmathc._clog(base)

    if not isfinite(res):
        raise ValueError("math domain error")

    return res


def log10(x):
    res = qcmathc._clog10(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def pow(x, y):
    res = qcmathc._cpow(x, y)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def sinh(x):
    res = qcmathc._csinh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def sin(x):
    res = qcmathc._csin(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def sqrt(x):
    res = qcmathc._csqrt(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def tan(x):
    res = qcmathc._ctan(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def tanh(x):
    res = qcmathc._ctanh(x)
    if not isfinite(res):
        raise ValueError("math domain error")
    return res


def isfinite(x):
    return qcmathc._finitecq(x)


def isnan(x):
    return qcmathc._isnancq(x)


def isinf(x):
    return qcmathc._isinfcq(x)


def isclose(a, b, *, rel_tol=1e-09, abs_tol=0.0):
    raise NotImplementedError
