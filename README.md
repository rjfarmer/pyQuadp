[![Continuous Integration](https://github.com/rjfarmer/pyQuad/actions/workflows/ci.yml/badge.svg)](https://github.com/rjfarmer/pyQuad/actions/workflows/ci.yml)
[![Coverage Status](https://coveralls.io/repos/github/rjfarmer/pyQuad/badge.svg?branch=main)](https://coveralls.io/github/rjfarmer/pyQuad?branch=main)
[![Python versions](https://img.shields.io/pypi/pyversions/gfort2py.svg)](https://img.shields.io/pypi/pyversions/gfort2py.svg)
[![gfortran versions](https://img.shields.io/badge/gfortran-8%7C9%7C10%7C11%7C12-blue)](https://img.shields.io/badge/gfortran-8%7C9%7C10%7C11%7C12-blue)


# pyQuad

Python interface to gcc's libquadmath for quad (128-bit) precision maths.

## Build
````bash
python -m build
python -m pip install . 
````

Package is not yet available on pypi.

This package requires ``quadmath.h`` and ``libquadmath.so``. This might come installed with your installation of gcc/gfortran from your package manager. Or it might require a separate installation. This should be installed before trying to install the python package.

### Fedora

````bash
sudo dnf install libquadmath libquadmath-devel
````


## Usage

### qfloat

A quad precision number is created by passing either a int, float, or string to ``qfloat``:

````python
import pyquad

q = pyquad.qfloat(1)
q = pyquad.qfloat(1.0)
q = pyquad.qfloat('1')
````

A ``qfloat`` implements Python's NumberProtocol, thus it can be used like any other number, either with basic math operations or in rich comparisons:

````python

q1 = pyquad.qfloat(1)
q2 = pyquad.qfloat(2)

q1+q2 # pyquad.qfloat(3)
q1*q2  # pyquad.qfloat(2)
q1+=q2 # pyquad.qfloat(3)

q1 <= q2 # True
q1 == q2 # False

str(q) # "1.000000000000000000000000000000000000e+00"
````

### qint


### qcmplx



## Math libraries

The ``qmath`` provides the union of math operations from Python's ``math`` library and the routines provided in ``libquadmath``. ``qmath`` provides routines for both ``qint``
and ``qfloat``. Complex numbers are handled by ``qcmath`` verions.

Where possible functions accessed via the Python name follows Python's conventions
regarding behaviour of exceptional values. While routines from ``libquadmath`` (those ending in ``q``) follows ``libquadmath``'s conventions.


### Routines from Python's ``math`` library

| Name     | Implemented | Descritpion |
|----------|-------------|-------------|
| ceil | :x: |  |
| comb | :x: |  |
| copysign | :x: |  |
| fabs | :x: |  |
| facorial | :x: |  |
| floor | :x: |  |
| fmod | :x: |  |
| frexp | :x: |  |
| fsum | :x: |  |
| gcd | :x: |  |
| isclose | :x: |  |
| isfinite | :x: |  |
| isinf | :x: |  |
| isnan | :x: |  |
| isqrt | :x: |  |
| lcm | :x: |  |
| ldexp | :x: |  |
| modf | :x: |  |
| nextafter | :x: |  |
| perm | :x: |  |
| prod | :x: |  |
| remainder | :x: |  |
| trunc | :x: |  |
| ulp | :x: |  |
| cbrt | :x: |  |
| exp | :x: |  |
| exp2 | :x: |  |
| expm1 | :x: |  |
| log | :x: |  |
| log1p | :x: |  |
| log2 | :x: |  |
| log10 | :x: |  |
| pow | :x: |  |
| sqrt | :x: |  |
| acos | :x: |  |
| asin | :x: |  |
| atan | :x: |  |
| atan2 | :x: |  |
| cos | :x: |  |
| dist | :x: |  |
| hypot | :x: |  |
| sin | :x: |  |
| tan | :x: |  |
| degress | :x: |  |
| radians | :x: |  |
| acosh | :x: |  |
| asinh| :x: |  |
| atanh| :x: |  |
| cosh| :x: |  |
| sinh| :x: |  |
| tanh| :x: |  |
| erf| :x: |  |
| erfc| :x: |  |
| gamma| :x: |  |
| lgamma| :x: |  |
| pi | :x: |  |  
| e | :x: |  |  
| tau | :x: |  |  
| inf | :x: |  |
| nan | :x: |  |  


### Routines from Python's ``math`` library

| Name     | Implemented |
|----------|-------------|
|acosq | :x: | arc cosine function |
|acoshq | :x: | inverse hyperbolic cosine function |
|asinq | :x: | arc sine function |
|asinhq | :x: | inverse hyperbolic sine function |
|atanq | :x: | arc tangent function |
|atanhq | :x: | inverse hyperbolic tangent function |
|atan2q | :x: | arc tangent function |
|cbrtq | :x: | cube root function |
|ceilq | :x: | ceiling value function |
|copysignq | :x: | copy sign of a number
|coshq | :x: | hyperbolic cosine function |
|cosq | :x: | cosine function |
|erfq | :x: | error function |
|erfcq | :x: | complementary error function |
|exp2q | :x: | base 2 exponential function |
|expq | :x: | exponential function |
|expm1q | :x: | exponential minus 1 function |
|fabsq | :x: | absolute value function |
|fdimq | :x: | positive difference function |
|finiteq | :x: | check finiteness of value
|floorq | :x: | floor value function |
|fmaq | :x: | fused multiply and add |
|fmaxq | :x: | determine maximum of two values |
|fminq | :x: | determine minimum of two values |
|fmodq | :x: | remainder value function |
|frexpq | :x: | extract mantissa and exponent |
|hypotq | :x: | Eucledian distance function |
|ilogbq | :x: | get exponent of the value |
|isinfq | :x: | check for infinity |
|isnanq | :x: | check for not a number |
|issignalingq | :x: | check for signaling not a number |
|j0q | :x: | Bessel function of the first kind, first order |
|j1q | :x: | Bessel function of the first kind, second order |
|jnq | :x: | Bessel function of the first kind, n-th order |
|ldexpq | :x: | load exponent of the value |
|lgammaq | :x: | logarithmic gamma function |
|llrintq | :x: | round to nearest integer value |
|llroundq | :x: | round to nearest integer value away from zero |
|logbq | :x: | get exponent of the value |
|logq | :x: | natural logarithm function |
|log10q | :x: | base 10 logarithm function |
|log1pq | :x: | compute natural logarithm of the value plus one |
|log2q | :x: | base 2 logarithm function |
|lrintq | :x: | round to nearest integer value |
|lroundq | :x: | round to nearest integer value away from zero |
|modfq | :x: | decompose the floating-point number |
|nanq | :x: | return quiet NaN |
|nearbyintq | :x: | round to nearest integer |
|nextafterq | :x: | next representable floating-point number |
|powq | :x: | power function |
|remainderq | :x: | remainder function |
|remquoq | :x: | remainder and part of quotient |
|rintq | :x: | round-to-nearest integral value |
|roundq | :x: | round-to-nearest integral value, return __float128 |
|scalblnq | :x: | compute exponent using FLT_RADIX |
|scalbnq | :x: | compute exponent using FLT_RADIX |
|signbitq | :x: | return sign bit |
|sincosq | :x: | calculate sine and cosine simultaneously |
|sinhq | :x: | hyperbolic sine function |
|sinq | :x: | sine function |
|sqrtq | :x: | square root function |
|tanq | :x: | tangent function |
|tanhq | :x: | hyperbolic tangent function |
|tgammaq | :x: | true gamma function |
|truncq | :x: | round to integer, towards zero |
|y0q | :x: | Bessel function of the second kind, first order |
|y1q | :x: | Bessel function of the second kind, second order |
|ynq | :x: | Bessel function of the second kind, n-th order |



### Routines from Python's complex math ``cmath`` library

These are available from ``qcmath``

| Name     | Implemented | Descritpion |
|----------|-------------|-------------|
| phase | :x: | |
| polar | :x: | |
| rect | :x: | |
| exp | :x: | |
| log | :x: | |
| log10 | :x: | |
| sqrt | :x: | |
| acos | :x: | |
| asin | :x: | |
| atan | :x: | |
| cos | :x: | |
| sin | :x: | |
| tan | :x: | |
| acosh | :x: | |
| asinh | :x: | |
| atanh | :x: | |
| cosh | :x: | |
| sinh | :x: | |
| tanh | :x: | |
| isfinite | :x: | |
| isinf | :x: | |
| isnan | :x: | |
| isclose | :x: | |
| pi | :x: | |
| e | :x: | |
| tau | :x: | |
| inf | :x: | |
| infj | :x: | |
| nan | :x: | |
| nanj | :x: | |


### Routines from complex math ``libquadthmath`` library

These are available from ``qcmath``

| Name     | Implemented | Descritpion |
|----------|-------------|-------------|
| cabsq | :x: |  complex absolute value function |
| cargq | :x: | calculate the argument |
| cimagq | :x: | imaginary part of complex number |
| crealq | :x: | real part of complex number | 
| cacoshq | :x: | complex arc hyperbolic cosine function |
| cacosq | :x: | complex arc cosine function |
| casinhq | :x: | complex arc hyperbolic sine function |
| casinq | :x: | complex arc sine function |
| catanhq | :x: | complex arc hyperbolic tangent function |
| catanq | :x: | complex arc tangent function |
| ccosq: | :x: | complex cosine function |
| ccoshq | :x: | complex hyperbolic cosine function |
| cexpq | :x: | complex exponential function |
| cexpiq | :x: | computes the exponential function of ``i`` times a real value |
| clogq | :x: | complex natural logarithm |
| clog10q | :x: | complex base 10 logarithm |
| conjq | :x: | complex conjugate function |
| cpowq | :x: | complex power function |
| cprojq | :x: | project into Riemann Sphere |
| csinq | :x: | complex sine function |
| csinhq | :x: | complex hyperbolic sine function |
| csqrtq | :x: | complex square root |
| ctanq | :x: | complex tangent function |
| ctanhq | :x: | complex hyperbolic tangent function |


### Constants from  ``libquadthmath`` library.

The following constants are availbe both in ``qmath`` and ``qcmath``

| Name     | Implemented | Descritpion |
|----------|-------------|-------------|
| FLT128_MAX | :x: | largest finite number | 
| FLT128_MIN| :x: | smallest positive number with full precision |
| FLT128_EPSILON | :x: |  difference between 1 and the next larger representable number | 
| FLT128_DENORM_MIN | :x: | smallest positive denormalized number |
| FLT128_MANT_DIG | :x: |  number of digits in the mantissa (bit precision) |
| FLT128_MIN_EXP | :x: |  maximal negative exponent |
| FLT128_MAX_EXP | :x: |  maximal positive exponent |
| FLT128_DIG | :x: |  number of decimal digits in the mantissa |
| FLT128_MIN_10_EXP | :x: |  maximal negative decimal exponent |
| FLT128_MAX_10_EXP | :x: |  maximal positive decimal exponent |
| M_Eq | :x: | the constant e (Euler’s number) |
| M_LOG2Eq | :x: | binary logarithm of 2 |
| M_LOG10Eq | :x: | common, decimal logarithm of 2 |
| M_LN2q | :x: | natural logarithm of 2 |
| M_LN10q | :x: | natural logarithm of 10 |
| M_PIq | :x: | pi |
| M_PI_2q | :x: | pi divided by two |
| M_PI_4q | :x: | pi divided by four |
| M_1_PIq | :x: | one over pi |
| M_2_PIq | :x: | one over two pi |
| M_2_SQRTPIq | :x: | two over square root of pi |
| M_SQRT2q | :x: | square root of 2 |
| M_SQRT1_2q | :x: | one over square root of 2 |
