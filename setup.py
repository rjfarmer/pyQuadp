#!/usr/bin/env python

import os
import shlex

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class BuildExtUsingCC(build_ext):
    def build_extensions(self):
        cc = os.environ.get("CC")
        if cc:
            cc_cmd = shlex.split(cc)
            self.compiler.set_executable("compiler", cc_cmd)
            self.compiler.set_executable("compiler_so", cc_cmd)
            self.compiler.set_executable("compiler_cxx", cc_cmd)

            # Keep link step aligned with CC unless explicitly overridden.
            if not os.environ.get("LDSHARED"):
                self.compiler.set_executable("linker_so", cc_cmd + ["-shared"])
            self.compiler.set_executable("linker_exe", cc_cmd)

        super().build_extensions()

extensions = [
    Extension(
        name="pyquadp.qmathc",
        sources=["pyquadp/qfloat.c", "pyquadp/qmathc.c"],
        libraries=["quadmath"],
        py_limited_api=True,
    ),
    Extension(
        name="pyquadp.qcmathc",
        sources=["pyquadp/qfloat.c", "pyquadp/qcmplx.c", "pyquadp/qcmathc.c"],
        libraries=["quadmath"],
        py_limited_api=True,
    ),
    Extension(
        name="pyquadp.qmfloat",
        sources=["pyquadp/qfloat.c"],
        libraries=["quadmath"],
        py_limited_api=True,
    ),
    Extension(
        name="pyquadp.qmcmplx",
        sources=["pyquadp/qfloat.c", "pyquadp/qcmplx.c"],
        libraries=["quadmath"],
        py_limited_api=True,
    ),
    Extension(
        name="pyquadp.qmint",
        sources=["pyquadp/qint.c"],
        libraries=["quadmath"],
        py_limited_api=True,
    ),
]


if __name__ == "__main__":
    import numpy as np

    extensions.extend(
        [
            Extension(
                name="pyquadp.qarray",
                sources=["pyquadp/qfloatarray.c"],
                include_dirs=["pyquadp", np.get_include()],
                libraries=["quadmath"],
                py_limited_api=True,
            ),
            Extension(
                name="pyquadp.qcarray",
                sources=["pyquadp/qcarray.c"],
                include_dirs=["pyquadp", np.get_include()],
                libraries=["quadmath"],
                py_limited_api=True,
            ),
            Extension(
                name="pyquadp.qiarray",
                sources=["pyquadp/qiarray.c"],
                include_dirs=["pyquadp", np.get_include()],
                libraries=["quadmath"],
                py_limited_api=True,
            ),
        ]
    )

    setup(
        ext_modules=extensions,
        cmdclass={"build_ext": BuildExtUsingCC},
        package_data={"pyquadp": ["*.pyi", "py.typed"]},
        options={"bdist_wheel": {"py_limited_api": "cp310"}},
    )
