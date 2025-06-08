#!/bin/bash

rm -rf build dist
python -m build
python -m pip install dist/pyquadp*.whl --force-reinstall
