# SPDX-License-Identifier: GPL-2.0+

from qfloat import _qfloat as qfloat
from qcmplx import _qcmplx as qcmplx
from qint import _qint as qint
import ctypes as _ctypes

# libquadmath constants

M_Eq = qfloat("2.718281828459045235360287471352662498")  #  /* e */
M_LOG2Eq = qfloat("1.442695040888963407359924681001892137")  #  /* log_2 e */
M_LOG10Eq = qfloat("0.434294481903251827651128918916605082")  #  /* log_10 e */
M_LN2q = qfloat("0.693147180559945309417232121458176568")  #  /* log_e 2 */
M_LN10q = qfloat("2.302585092994045684017991454684364208")  #  /* log_e 10 */
M_PIq = qfloat("3.141592653589793238462643383279502884")  #  /* pi */
M_PI_2q = qfloat("1.570796326794896619231321691639751442")  #  /* pi/2 */
M_PI_4q = qfloat("0.785398163397448309615660845819875721")  #  /* pi/4 */
M_1_PIq = qfloat("0.318309886183790671537767526745028724")  #  /* 1/pi */
M_2_PIq = qfloat("0.636619772367581343075535053490057448")  #  /* 2/pi */
M_2_SQRTPIq = qfloat("1.128379167095512573896158903121545172")  #  /* 2/sqrt(pi) */
M_SQRT2q = qfloat("1.414213562373095048801688724209698079")  #  /* sqrt(2) */
M_SQRT1_2q = qfloat("0.707106781186547524400844362104849039")  #  /* 1/sqrt(2) */


FLT128_MAX = qfloat("1.18973149535723176508575932662800702e4932")
FLT128_MIN = qfloat("3.36210314311209350626267781732175260e-4932")
FLT128_EPSILON = qfloat("1.92592994438723585305597794258492732e-34")
FLT128_DENORM_MIN = qfloat("6.475175119438025110924438958227646552e-4966")
FLT128_MANT_DIG = 113
FLT128_MIN_EXP = -16381
FLT128_MAX_EXP = 16384
FLT128_DIG = 33
FLT128_MIN_10_EXP = -4931
FLT128_MAX_10_EXP = 4932

INT128_MAX = qint("170141183460469231731687303715884105727")
INT128_MIN = -INT128_MAX

# Export python math library constant names

pi = M_PIq
e = M_Eq
tau = 2 * pi
inf = qfloat("inf")
nan = qfloat("nan")

infj = qcmplx(0.0, qfloat("inf"))
nanj = qcmplx(0.0, qfloat("nan"))


# Ctype data
c_qfloat = _ctypes.c_byte * 16


class c_qcmplx(_ctypes.Structure):
    _fields_ = [
        ("real", c_qfloat),
        ("imag", c_qfloat),
    ]
