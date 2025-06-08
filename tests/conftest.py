# SPDX-License-Identifier: GPL-2.0+

import subprocess
import os
from packaging.version import Version

import pathlib
import _pytest.pathlib


resolve_pkg_path_orig = _pytest.pathlib.resolve_package_path

# we consider all dirs in repo/ to be namespace packages
rootdir = pathlib.Path(__file__).parent.resolve()
namespace_pkg_dirs = [str(d) for d in rootdir.iterdir() if d.is_dir()]


# patched method
def resolve_package_path(path):
    # call original lookup
    result = resolve_pkg_path_orig(path)
    if result is not None:
        return result
    # original lookup failed, check if we are subdir of a namespace package
    # if yes, return the namespace package we belong to
    for parent in path.parents:
        if str(parent) in namespace_pkg_dirs:
            return parent
    return None


# apply patch
_pytest.pathlib.resolve_package_path = resolve_package_path


def pytest_configure(config):
    """This is run when pytest is setting up in the controller process and in the workers too"""
    if hasattr(config, "workerinput"):
        # prevent workers to run the same code
        return

    subprocess.call(["make", "-f", "Makefile", "all"], cwd="tests")
