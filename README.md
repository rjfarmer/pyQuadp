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

This package requires ``quadmath.h`` and ``libquadmath.so``. This might come installed with a installation of gcc/gfortran from your package manager. Or it might separate installation. This should be installed before trying to install the python package

###Fedora

````bash
sudo dnf install libquadmath libquadmath-devel
````


## Usage

### qfloat

A quad precision number is created by passing either a int, float, or strong to ``qfloat``:

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
````

### qmath

The ``qmath`` provides the union of math operations from Python's ``math`` library and the routines provided in ``libquadmath``.

This is still under development.