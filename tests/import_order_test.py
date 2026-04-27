# SPDX-License-Identifier: GPL-2.0+

import json
import os
import subprocess
import sys

import pytest

_IMPORT_CHECK_SCRIPT = """
import importlib
import json
import os

order = json.loads(os.environ["PYQUADP_IMPORT_ORDER"])
for module_name in order:
    importlib.import_module(module_name)

import pyquadp
assert hasattr(pyquadp, "qfloat")
assert hasattr(pyquadp, "qint")
assert hasattr(pyquadp, "qcmplx")
assert hasattr(pyquadp, "qarray")
assert hasattr(pyquadp, "qcarray")
assert hasattr(pyquadp, "qiarray")
assert hasattr(pyquadp, "qmfloat")
assert hasattr(pyquadp, "qmint")
assert hasattr(pyquadp, "qmcmplx")
"""


@pytest.mark.parametrize(
    "import_order",
    [
        ["pyquadp"],
        ["pyquadp.qmcmplx", "pyquadp.qmfloat", "pyquadp.qcarray"],
        ["pyquadp.qmfloat", "pyquadp.qmcmplx", "pyquadp.qcarray"],
        ["pyquadp.qarray", "pyquadp.qcarray", "pyquadp.qiarray", "pyquadp"],
        ["pyquadp.constant", "pyquadp.qmath", "pyquadp.qcmath", "pyquadp"],
    ],
)
def test_import_order_independent(import_order):
    env = os.environ.copy()
    env["PYQUADP_IMPORT_ORDER"] = json.dumps(import_order)

    result = subprocess.run(
        [sys.executable, "-c", _IMPORT_CHECK_SCRIPT],
        check=False,
        capture_output=True,
        text=True,
        env=env,
    )

    assert result.returncode == 0, result.stderr
