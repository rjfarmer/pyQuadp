#!/bin/bash

export CC=gcc
export CXX=g++
export FC=gfortran

REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"

cd "$REPO_ROOT" || exit 1
rm -rf build dist
python -m build
python -m pip install dist/pyquadp*.whl --force-reinstall

# Keep cwd at repo root for tests/conftest.py, but remove source-tree package
# paths so imports resolve to the installed wheel.
python -c 'import os, sys, pytest; repo = os.getcwd(); sys.path = [p for p in sys.path if p not in ("", repo)]; raise SystemExit(pytest.main(["-v", "tests"]))'
