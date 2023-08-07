#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <quadmath.h>
#include <stdbool.h>

#pragma once

#include "qcmplx.h"
#include "qfloat.h"


#define OP_cabsq  0
#define OP_cargq  1
#define OP_cimagq  2
#define OP_crealq  3

#define OP_cacoshq  4
#define OP_cacosq  5
#define OP_casinhq  6
#define OP_casinq  7
#define OP_catanhq  8
#define OP_catanq  9
#define OP_ccosq  10
#define OP_ccoshq  11
#define OP_cexpq  12
#define OP_cexpiq  13
#define OP_clogq  14
#define OP_clog10q  15
#define OP_conjq  16
#define OP_cpowq  17
#define OP_cprojq  18
#define OP_csinq  19
#define OP_csinhq  20
#define OP_csqrtq  21
#define OP_ctanq  22
#define OP_ctanhq  23

#define OP_finitecq 24
#define OP_isinfcq 25
#define OP_isnancq 26

#define OP_phase 27
