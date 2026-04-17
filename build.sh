#!/bin/bash

export CC=gcc
export CXX=g++
export FC=gfortran

rm -rf build dist
python -m build
python -m pip install dist/pyquadp*.whl --force-reinstall
