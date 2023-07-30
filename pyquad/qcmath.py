from .constant import *
import qcmathc as _qcmathc


def cabs(x):
    return _qcmathc._cabs(x)


def cabsq(x):
    return _qcmathc._cabs(x)


def carg(x):
    return _qcmathc._carg(x)


def cargq(x):
    return _qcmathc._carg(x)


def cimag(x):
    return _qcmathc._cimag(x)


def cimagq(x):
    return _qcmathc._cimag(x)


def creal(x):
    return _qcmathc._creal(x)


def crealq(x):
    return _qcmathc._creal(x)


def cacosh(x):
    return _qcmathc._cacosh(x)


def cacoshq(x):
    return _qcmathc._cacosh(x)


def cacos(x):
    return _qcmathc._cacos(x)


def cacosq(x):
    return _qcmathc._cacos(x)


def casinh(x):
    return _qcmathc._casinh(x)


def casinhq(x):
    return _qcmathc._casinh(x)


def casin(x):
    return _qcmathc._casin(x)


def casinq(x):
    return _qcmathc._casin(x)


def catanh(x):
    return _qcmathc._catanh(x)


def catanhq(x):
    return _qcmathc._catanh(x)


def catan(x):
    return _qcmathc._catan(x)


def catanq(x):
    return _qcmathc._catan(x)


def ccos(x):
    return _qcmathc._ccos(x)


def ccosq(x):
    return _qcmathc._ccos(x)


def ccosh(x):
    return _qcmathc._ccosh(x)


def ccoshq(x):
    return _qcmathc._ccosh(x)


def cexp(x):
    return _qcmathc._cexp(x)


def cexpq(x):
    return _qcmathc._cexp(x)


def cexpi(x):
    return _qcmathc._cexpi(x)


def cexpiq(x):
    return _qcmathc._cexpi(x)


def clog(x):
    return _qcmathc._clog(x)


def clogq(x):
    return _qcmathc._clog(x)


def clog10(x):
    return _qcmathc._clog10(x)


def clog10q(x):
    return _qcmathc._clog10(x)


def conj(x):
    return _qcmathc._conj(x)


def conjq(x):
    return _qcmathc._conj(x)


def cpow(x, y):
    return _qcmathc._cpow(x, y)


def cpowq(x, y):
    return _qcmathc._cpow(x, y)


def cproj(x):
    return _qcmathc._cproj(x)


def cprojq(x):
    return _qcmathc._cproj(x)


def csin(x):
    return _qcmathc._csin(x)


def csinq(x):
    return _qcmathc._csin(x)


def csinh(x):
    return _qcmathc._csinh(x)


def csinhq(x):
    return _qcmathc._csinh(x)


def csqrt(x):
    return _qcmathc._csqrt(x)


def csqrtq(x):
    return _qcmathc._csqrt(x)


def ctan(x):
    return _qcmathc._ctan(x)


def ctanq(x):
    return _qcmathc._ctan(x)


def ctanh(x):
    return _qcmathc._ctanh(x)


def ctanhq(x):
    return _qcmathc._ctanh(x)


def isfinite(x):
    return _qcmathc._finitecq(x)


def isnan(x):
    return _qcmathc._isnancq(x)


def isinf(x):
    return _qcmathc._isinfcq(x)


def phase(x):
    return _qcmathc._phase(x)


def polar(x):
    return _qcmathc._polar(x)


def rect(r, phi):
    return _qcmathc._polar(r, phi)
