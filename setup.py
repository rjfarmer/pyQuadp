#!/usr/bin/env python

from setuptools import Extension, setup

extensions = [
    Extension(
        name="pyquadp.qmathc",
        sources=["pyquadp/qfloat.c", "pyquadp/qmathc.c"],
        libraries=["quadmath"],
    ),
    Extension(
        name="pyquadp.qcmathc",
        sources=["pyquadp/qfloat.c", "pyquadp/qcmplx.c", "pyquadp/qcmathc.c"],
        libraries=["quadmath"],
    ),
    Extension(
        name="pyquadp.qmfloat",
        sources=["pyquadp/qfloat.c"],
        libraries=["quadmath"],
    ),
    Extension(
        name="pyquadp.qmcmplx",
        sources=["pyquadp/qfloat.c", "pyquadp/qcmplx.c"],
        libraries=["quadmath"],
    ),
    Extension(
        name="pyquadp.qmint",
        sources=["pyquadp/qint.c"],
        libraries=["quadmath"],
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
            ),
            Extension(
                name="pyquadp.qcarray",
                sources=["pyquadp/qcarray.c"],
                include_dirs=["pyquadp", np.get_include()],
                libraries=["quadmath"],
            ),
            Extension(
                name="pyquadp.qiarray",
                sources=["pyquadp/qiarray.c"],
                include_dirs=["pyquadp", np.get_include()],
                libraries=["quadmath"],
            ),
        ]
    )

    setup(ext_modules=extensions)
