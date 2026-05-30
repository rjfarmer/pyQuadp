#!/bin/bash
set -euo pipefail

python -m black pyquadp/ tests/
isort .
mypy pyquadp/
zizmor --fix=all --gh-token "$(gh auth token)" .github/**/*.yml