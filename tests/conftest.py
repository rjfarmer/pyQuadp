# SPDX-License-Identifier: GPL-2.0+

import subprocess
import os
from packaging.version import Version


def pytest_configure(config):
    """This is run when pytest is setting up in the controller process and in the workers too"""
    if hasattr(config, "workerinput"):
        # prevent workers to run the same code
        return

    subprocess.call(["make", "-f", "Makefile", "all"], cwd="tests")
