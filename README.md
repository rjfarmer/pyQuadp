[![Continuous Integration](https://github.com/rjfarmer/pyQuadp/actions/workflows/ci.yml/badge.svg)](https://github.com/rjfarmer/pyQuadp/actions/workflows/ci.yml)
[![Coverage Status](https://coveralls.io/repos/github/rjfarmer/pyQuadp/badge.svg?branch=main)](https://coveralls.io/github/rjfarmer/pyQuadp?branch=main)
[![Python versions](https://img.shields.io/pypi/pyversions/gfort2py.svg)](https://img.shields.io/pypi/pyversions/gfort2py.svg)
[![gfortran versions](https://img.shields.io/badge/gfortran-8%7C9%7C10%7C11%7C12-blue)](https://img.shields.io/badge/gfortran-8%7C9%7C10%7C11%7C12-blue)


# pyQuadp

Python interface to gcc's libquadmath for quad (128-bit) precision maths.

## Build
````bash
python -m build
python -m pip install . 
````

Package is not yet available on pypi.

This package requires ``quadmath.h`` and ``libquadmath.so``. This might come installed with your installation of gcc/gfortran from your package manager. Or it might require a separate installation. This should be installed before trying to install the Python package.

### Fedora

````bash
sudo dnf install libquadmath libquadmath-devel
````


## Usage

### qfloat

A quad precision number is created by passing either a int, float, or string to ``qfloat``:

````python
import pyquadp

q = pyquadp.qfloat(1)
q = pyquadp.qfloat(1.0)
q = pyquadp.qfloat('1')
````

A ``qfloat`` implements Python's NumberProtocol, thus it can be used like any other number, either with basic math operations or in rich comparisons:

````python

q1 = pyquadp.qfloat(1)
q2 = pyquadp.qfloat(2)

q1+q2 # pyquadp.qfloat(3)
q1*q2  # pyquadp.qfloat(2)
q1+=q2 # pyquadp.qfloat(3)

q1 <= q2 # True
q1 == q2 # False

str(q) # "1.000000000000000000000000000000000000e+00"
````

### qcmplx

A quad precision number is created by passing either a complex variable or two ints, floats, strs, or qfloats to ``qcmplx``:

````python
import pyquadp

q = pyquadp.qcmplx(complex(1,1))
q = pyquadp.qcmplx(1,1.0)
q = pyquadp.qcmplx('1',1.0)
q = pyquadp.qcmplx('1','1')
q = pyquadp.qcmplx(pyquadp.qfloat(1), pyquadp.qfloat('1'))
````

Note that strings must be split into two components. There is no support for ``1+1j`` (unless passed via complex('1+1j'))


<!-- A ``qcmplx`` implements Python's NumberProtocol, thus it can be used like any other number, either with basic math operations or in rich comparisons:

````python

q1 = pyquadp.qfloat(1)
q2 = pyquadp.qfloat(2)

q1+q2 # pyquadp.qfloat(3)
q1*q2  # pyquadp.qfloat(2)
q1+=q2 # pyquadp.qfloat(3)

q1 <= q2 # True
q1 == q2 # False

str(q) # "1.000000000000000000000000000000000000e+00"
```` -->

## Math libraries

The ``qmath`` provides the union of math operations from Python's ``math`` library and the routines provided in ``libquadmath``. ``qmath`` provides routines for ``qfloat``, while complex numbers are handled by ``qcmath`` versions.

Where possible functions accessed via the Python name follows Python's conventions
regarding behavior of exceptional values. While routines from ``libquadmath`` (those ending in ``q``) follows ``libquadmath``'s conventions.


### Routines from Python's ``math`` library

| Name     | Implemented | Descritpion |
|----------|-------------|-------------|
| ceil | :heavy_check_mark: |  |
| comb | :x: |  |
| copysign | :heavy_check_mark: |  |
| fabs |:heavy_check_mark: |  |
| factorial | :x: |  |
| floor | :heavy_check_mark: |  |
| fmod | :heavy_check_mark: |  |
| frexp | :heavy_check_mark: |  |
| fsum | :x: |  |
| gcd | :x: |  |
| isclose | :x: |  |
| isfinite | :x: |  |
| isinf | :heavy_check_mark: |  |
| isnan | :heavy_check_mark: |  |
| isqrt | :x: |  |
| lcm | :x: |  |
| ldexp | :heavy_check_mark: |  |
| modf | :heavy_check_mark: |  |
| nextafter | :heavy_check_mark: |  |
| perm | :x: |  |
| prod | :x: |  |
| remainder | :heavy_check_mark: |  |
| trunc | :heavy_check_mark: |  |
| ulp | :x: |  |
| cbrt | :heavy_check_mark: |  |
| exp | :heavy_check_mark: |  |
| exp2 | :heavy_check_mark: |  |
| expm1 | :heavy_check_mark: |  |
| log | :heavy_check_mark: |  |
| log1p | :heavy_check_mark: |  |
| log2 | :heavy_check_mark: |  |
| log10 | :heavy_check_mark: |  |
| pow | :heavy_check_mark: |  |
| sqrt | :heavy_check_mark: |  |
| acos | :heavy_check_mark: |  |
| asin | :heavy_check_mark: |  |
| atan | :heavy_check_mark: |  |
| atan2 | :heavy_check_mark: |  |
| cos | :heavy_check_mark: |  |
| dist | :x: |  |
| hypot | :heavy_check_mark: |  |
| sin | :heavy_check_mark: |  |
| tan | :heavy_check_mark: |  |
| degress | :x: |  |
| radians | :x: |  |
| acosh | :heavy_check_mark: |  |
| asinh| :heavy_check_mark: |  |
| atanh| :heavy_check_mark: |  |
| cosh| :heavy_check_mark: |  |
| sinh| :heavy_check_mark: |  |
| tanh| :heavy_check_mark: |  |
| erf| :heavy_check_mark: |  |
| erfc| :heavy_check_mark: |  |
| gamma| :heavy_check_mark: |  |
| lgamma| :heavy_check_mark: |  |
| pi | :heavy_check_mark: |  |  
| e | :heavy_check_mark: |  |  
| tau | :heavy_check_mark: |  |  
| inf | :heavy_check_mark: |  |
| nan | :heavy_check_mark: |  |  


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
| phase | :heavy_check_mark: | |
| polar | :heavy_check_mark: | |
| rect | :heavy_check_mark: | |
| exp | :heavy_check_mark: | |
| log | :heavy_check_mark: | |
| log10 | :heavy_check_mark: | |
| sqrt | :heavy_check_mark: | |
| acos | :heavy_check_mark: | |
| asin | :heavy_check_mark: | |
| atan | :heavy_check_mark: | |
| cos | :heavy_check_mark: | |
| sin | :heavy_check_mark: | |
| tan | :heavy_check_mark: | |
| acosh | :heavy_check_mark: | |
| asinh | :heavy_check_mark: | |
| atanh | :heavy_check_mark: | |
| cosh | :heavy_check_mark: | |
| sinh | :heavy_check_mark: | |
| tanh | :heavy_check_mark: | |
| isfinite | :heavy_check_mark: | |
| isinf | :heavy_check_mark: | |
| isnan | :heavy_check_mark: | |
| isclose | :x: | |
| pi | :heavy_check_mark: | |
| e | :heavy_check_mark: | |
| tau | :heavy_check_mark: | |
| inf | :heavy_check_mark: | |
| infj | :heavy_check_mark: | |
| nan | :heavy_check_mark: | |
| nanj | :heavy_check_mark: | |


### Routines from complex math ``libquadthmath`` library

These are available from ``qcmath``

| Name     | Implemented | Descritpion |
|----------|-------------|-------------|
| cabsq | :heavy_check_mark: |  complex absolute value function |
| cargq | :heavy_check_mark: | calculate the argument |
| cimagq | :heavy_check_mark: | imaginary part of complex number |
| crealq | :heavy_check_mark: | real part of complex number | 
| cacoshq | :heavy_check_mark: | complex arc hyperbolic cosine function |
| cacosq | :heavy_check_mark: | complex arc cosine function |
| casinhq | :heavy_check_mark: | complex arc hyperbolic sine function |
| casinq | :heavy_check_mark: | complex arc sine function |
| catanhq | :heavy_check_mark: | complex arc hyperbolic tangent function |
| catanq | :heavy_check_mark: | complex arc tangent function |
| ccosq: | :heavy_check_mark: | complex cosine function |
| ccoshq | :heavy_check_mark: | complex hyperbolic cosine function |
| cexpq | :heavy_check_mark: | complex exponential function |
| cexpiq | :heavy_check_mark: | computes the exponential function of ``i`` times a real value |
| clogq | :heavy_check_mark: | complex natural logarithm |
| clog10q | :heavy_check_mark: | complex base 10 logarithm |
| conjq | :heavy_check_mark: | complex conjugate function |
| cpowq | :heavy_check_mark: | complex power function |
| cprojq | :heavy_check_mark: | project into Riemann Sphere |
| csinq | :heavy_check_mark: | complex sine function |
| csinhq | :heavy_check_mark: | complex hyperbolic sine function |
| csqrtq | :heavy_check_mark: | complex square root |
| ctanq | :heavy_check_mark: | complex tangent function |
| ctanhq | :heavy_check_mark: | complex hyperbolic tangent function |


### Constants from  ``libquadthmath`` library.

The following constants are availbe both in ``qmath`` and ``qcmath``

| Name     | Implemented | Descritpion |
|----------|-------------|-------------|
| FLT128_MAX | :heavy_check_mark: | largest finite number | 
| FLT128_MIN| :heavy_check_mark: | smallest positive number with full precision |
| FLT128_EPSILON | :heavy_check_mark: |  difference between 1 and the next larger representable number | 
| FLT128_DENORM_MIN | :heavy_check_mark: | smallest positive denormalized number |
| FLT128_MANT_DIG | :heavy_check_mark: |  number of digits in the mantissa (bit precision) |
| FLT128_MIN_EXP | :heavy_check_mark: |  maximal negative exponent |
| FLT128_MAX_EXP | :heavy_check_mark: |  maximal positive exponent |
| FLT128_DIG | :heavy_check_mark: |  number of decimal digits in the mantissa |
| FLT128_MIN_10_EXP | :heavy_check_mark: |  maximal negative decimal exponent |
| FLT128_MAX_10_EXP | :heavy_check_mark: |  maximal positive decimal exponent |
| M_Eq | :heavy_check_mark: | the constant e (Euler’s number) |
| M_LOG2Eq | :heavy_check_mark: | binary logarithm of 2 |
| M_LOG10Eq | :heavy_check_mark: | common, decimal logarithm of 2 |
| M_LN2q | :heavy_check_mark: | natural logarithm of 2 |
| M_LN10q | :heavy_check_mark: | natural logarithm of 10 |
| M_PIq | :heavy_check_mark: | pi |
| M_PI_2q | :heavy_check_mark: | pi divided by two |
| M_PI_4q | :heavy_check_mark: | pi divided by four |
| M_1_PIq | :heavy_check_mark: | one over pi |
| M_2_PIq | :heavy_check_mark: | one over two pi |
| M_2_SQRTPIq | :heavy_check_mark: | two over square root of pi |
| M_SQRT2q | :heavy_check_mark: | square root of 2 |
| M_SQRT1_2q | :heavy_check_mark: | one over square root of 2 |
