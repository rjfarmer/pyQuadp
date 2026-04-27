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
import sys

repo_root = os.path.realpath(os.environ["PYQUADP_REPO_ROOT"])
checkout_entries = {
    repo_root,
    os.path.join(repo_root, "pyquadp"),
    os.path.join(repo_root, "tests"),
}

# Ensure this subprocess resolves pyquadp from installed site-packages,
# not from the checkout tree that may not contain built extension artifacts.
sys.path[:] = [
    entry
    for entry in sys.path
    if entry
    and os.path.realpath(entry) not in checkout_entries
]

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
    env["PYQUADP_REPO_ROOT"] = os.path.realpath(
        os.path.join(os.path.dirname(__file__), "..")
    )

    if "PYTHONPATH" in env:
        cleaned = []
        repo_root = env["PYQUADP_REPO_ROOT"]
        checkout_entries = {
            repo_root,
            os.path.join(repo_root, "pyquadp"),
            os.path.join(repo_root, "tests"),
        }
        for entry in env["PYTHONPATH"].split(os.pathsep):
            if not entry:
                continue
            real_entry = os.path.realpath(entry)
            if real_entry in checkout_entries:
                continue
            cleaned.append(entry)
        env["PYTHONPATH"] = os.pathsep.join(cleaned)

    result = subprocess.run(
        [sys.executable, "-c", _IMPORT_CHECK_SCRIPT],
        check=False,
        capture_output=True,
        text=True,
        env=env,
    )

    assert result.returncode == 0, result.stderr
