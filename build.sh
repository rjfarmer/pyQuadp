#!/bin/bash

rm -rf *.so build dist pyquadp.egg-info/
python -m build && python -m pip install .
