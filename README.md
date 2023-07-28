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


### Routines from gcc's ``libquadthmath`` library

| Name     | Implemented |
|----------|-------------|
|acosq | :heavy_check_mark: | arc cosine function |
|acoshq | :heavy_check_mark: | inverse hyperbolic cosine function |
|asinq | :heavy_check_mark: | arc sine function |
|asinhq | :heavy_check_mark: | inverse hyperbolic sine function |
|atanq | :heavy_check_mark: | arc tangent function |
|atanhq | :heavy_check_mark: | inverse hyperbolic tangent function |
|atan2q | :heavy_check_mark: | arc tangent function |
|cbrtq | :heavy_check_mark: | cube root function |
|ceilq | :heavy_check_mark: | ceiling value function |
|copysignq |:heavy_check_mark: | copy sign of a number
|coshq | :heavy_check_mark: | hyperbolic cosine function |
|cosq | :heavy_check_mark: | cosine function |
|erfq | :heavy_check_mark: | error function |
|erfcq | :heavy_check_mark: | complementary error function |
|exp2q | :heavy_check_mark: | base 2 exponential function |
|expq | :heavy_check_mark: | exponential function |
|expm1q | :heavy_check_mark: | exponential minus 1 function |
|fabsq | :heavy_check_mark: | absolute value function |
|fdimq | :heavy_check_mark: | positive difference function |
|finiteq | :heavy_check_mark: | check finiteness of value
|floorq | :heavy_check_mark: | floor value function |
|fmaq | :heavy_check_mark: | fused multiply and add |
|fmaxq | :heavy_check_mark: | determine maximum of two values |
|fminq | :heavy_check_mark: | determine minimum of two values |
|fmodq | :heavy_check_mark: | remainder value function |
|frexpq | :heavy_check_mark: | extract mantissa and exponent |
|hypotq | :heavy_check_mark: | Eucledian distance function |
|ilogbq | :heavy_check_mark: | get exponent of the value |
|isinfq | :heavy_check_mark: | check for infinity |
|isnanq | :heavy_check_mark: | check for not a number |
|issignalingq | :heavy_check_mark: | check for signaling not a number |
|j0q | :heavy_check_mark: | Bessel function of the first kind, first order |
|j1q | :heavy_check_mark: | Bessel function of the first kind, second order |
|jnq | :heavy_check_mark: | Bessel function of the first kind, n-th order |
|ldexpq | :heavy_check_mark: | load exponent of the value |
|lgammaq | :heavy_check_mark: | logarithmic gamma function |
|llrintq | :heavy_check_mark: | round to nearest integer value |
|llroundq |:heavy_check_mark: | round to nearest integer value away from zero |
|logbq | :heavy_check_mark: | get exponent of the value |
|logq | :heavy_check_mark: | natural logarithm function |
|log10q | :heavy_check_mark: | base 10 logarithm function |
|log1pq | :heavy_check_mark: | compute natural logarithm of the value plus one |
|log2q | :heavy_check_mark: | base 2 logarithm function |
|lrintq | :heavy_check_mark: | round to nearest integer value |
|lroundq | :heavy_check_mark: | round to nearest integer value away from zero |
|modfq | :heavy_check_mark: | decompose the floating-point number |
|nanq | :heavy_check_mark: | return quiet NaN |
|nearbyintq | :heavy_check_mark: | round to nearest integer |
|nextafterq | ::heavy_check_mark: | next representable floating-point number |
|powq | :heavy_check_mark: | power function |
|remainderq | :heavy_check_mark: | remainder function |
|remquoq | :heavy_check_mark: | remainder and part of quotient |
|rintq | :heavy_check_mark: | round-to-nearest integral value |
|roundq | :heavy_check_mark: | round-to-nearest integral value, return __float128 |
|scalblnq | :heavy_check_mark: | compute exponent using FLT_RADIX |
|scalbnq | :heavy_check_mark: | compute exponent using FLT_RADIX |
|signbitq | :heavy_check_mark: | return sign bit |
|sincosq | :heavy_check_mark: | calculate sine and cosine simultaneously |
|sinhq | :heavy_check_mark: | hyperbolic sine function |
|sinq | :heavy_check_mark: | sine function |
|sqrtq | :heavy_check_mark: | square root function |
|tanq | :heavy_check_mark: | tangent function |
|tanhq | :heavy_check_mark: | hyperbolic tangent function |
|tgammaq | :heavy_check_mark: | true gamma function |
|truncq | :heavy_check_mark: | round to integer, towards zero |
|y0q | :heavy_check_mark: | Bessel function of the second kind, first order |
|y1q | :heavy_check_mark: | Bessel function of the second kind, second order |
|ynq | :heavy_check_mark: | Bessel function of the second kind, n-th order |



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
