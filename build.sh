#!/bin/bash

rm -rf *.so build pyquad.egg-info/
python -m build && python -m pip install .
