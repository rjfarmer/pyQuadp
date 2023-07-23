#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <quadmath.h>
#include <stdbool.h>

#pragma once

#include "qfloat.h"


#define OP_acosq 0
#define OP_acoshq 1
#define OP_asinq 2
#define OP_asinhq 3
#define OP_atanq 4
#define OP_atanhq 5
#define OP_atan2q 6
#define OP_cbrtq 7
#define OP_ceilq 8
#define OP_copysignq 9
#define OP_coshq 10
#define OP_cosq 11
#define OP_erfq 12
#define OP_erfcq 13
#define OP_exp2q 14
#define OP_expq 15
#define OP_expm1q 16
#define OP_fabsq 17
#define OP_fdimq 18
#define OP_finiteq 19
#define OP_floorq 20
#define OP_fmaq 21
#define OP_fmaxq 22
#define OP_fminq 23
#define OP_fmodq 24
#define OP_frexpq 25
#define OP_hypotq 26
#define OP_ilogbq 27
#define OP_isinfq 28
#define OP_isnanq 29
#define OP_issignalingq 30
#define OP_j0q 31
#define OP_j1q 32
#define OP_jnq 33
#define OP_ldexpq 34
#define OP_lgammaq 35
#define OP_llrintq 36
#define OP_llroundq 37
#define OP_logbq 38
#define OP_logq 39
#define OP_log10q 40
#define OP_log1pq 41
#define OP_log2q 42
#define OP_lrintq 43
#define OP_lroundq 44
#define OP_modfq 45
#define OP_nanq 46
#define OP_nearbyintq 47
#define OP_nextafterq 48
#define OP_powq 49
#define OP_remainderq 50
#define OP_remquoq 51
#define OP_rintq 52
#define OP_roundq 53
#define OP_scalblnq 54
#define OP_scalbnq 55
#define OP_signbitq 56
#define OP_sincosq 57
#define OP_sinhq 58
#define OP_sinq 59
#define OP_sqrtq 60
#define OP_tanq 61
#define OP_tanhq 62
#define OP_tgammaq 63
#define OP_truncq 64
#define OP_y0q 65
#define OP_y1q 66
#define OP_ynq 67
