#!/usr/bin/bash

export PYTEST_COVERAGE=1
python -m build
python -m pip install -e .
pytest --cov=pyquadp --cov-report html 
xdg-open htmlcov/index.html
