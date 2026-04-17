#!/bin/bash

export CC=${CC:-gcc}
export CXX=${CXX:-g++}
export FC=${FC:-gfortran}

rm -rf build dist
python -m build
python -m pip install dist/pyquadp*.whl --force-reinstall
