from ctypes import Structure
from typing import Any

from .qmcmplx import qcmplx
from .qmfloat import qfloat
from .qmint import qint

M_Eq: qfloat
M_LOG2Eq: qfloat
M_LOG10Eq: qfloat
M_LN2q: qfloat
M_LN10q: qfloat
M_PIq: qfloat
M_PI_2q: qfloat
M_PI_4q: qfloat
M_1_PIq: qfloat
M_2_PIq: qfloat
M_2_SQRTPIq: qfloat
M_SQRT2q: qfloat
M_SQRT1_2q: qfloat

FLT128_MAX: qfloat
FLT128_MIN: qfloat
FLT128_EPSILON: qfloat
FLT128_DENORM_MIN: qfloat
FLT128_MANT_DIG: int
FLT128_MIN_EXP: int
FLT128_MAX_EXP: int
FLT128_DIG: int
FLT128_MIN_10_EXP: int
FLT128_MAX_10_EXP: int

INT128_MAX: qint
INT128_MIN: qint

pi: qfloat
e: qfloat
tau: qfloat
inf: qfloat
nan: qfloat

infj: qcmplx
nanj: qcmplx

c_qfloat: Any

class c_qcmplx(Structure):
    real: Any
    imag: Any
