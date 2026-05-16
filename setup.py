#!/usr/bin/env python

import os
import shlex

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class BuildExtUsingCC(build_ext):
    def finalize_options(self):
        super().finalize_options()
        # On Windows, setuptools defaults to MSVC unless compiler type is set.
        # If CC is provided, prefer MinGW toolchain so GCC is actually used.
        if os.name == "nt" and os.environ.get("CC"):
            self.compiler = "mingw32"

    def build_extensions(self):
        cc = os.environ.get("CC")
        # On Windows, let mingw32 compiler templates remain intact. Distutils
        # already respects the environment compiler choice when available.
        if cc and os.name != "nt":
            cc_cmd = shlex.split(cc)

            def _override_executable(cmd_name):
                current = getattr(self.compiler, cmd_name, None)
                if current is None:
                    return
                if isinstance(current, str):
                    current = shlex.split(current)
                if not current:
                    return
                # Replace only the compiler executable, keep setuptools flags (e.g. -fPIC).
                updated = cc_cmd + current[1:]
                self.compiler.set_executable(cmd_name, updated)

            _override_executable("compiler")
            _override_executable("compiler_so")
            _override_executable("compiler_cxx")
            _override_executable("linker_exe")

            # Respect explicit LDSHARED if provided by the environment.
            if not os.environ.get("LDSHARED"):
                _override_executable("linker_so")

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
