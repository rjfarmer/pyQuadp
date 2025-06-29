#!/usr/bin/env python

from setuptools import Extension, setup

# import numpy as np

extensions = [
    # Extension(
    #     name="pyquadp/qarray",
    #     sources=[
    #         "pyquadp/qfloatarray.c",
    #     ],
    #     depends=[
    #         "pyquadp/qfloat.c",
    #     ],
    #     include_dirs=[
    #         np.get_include(),
    #         "pyquadp"
    #     ],
    #     libraries=[
    #         'quadmath'
    #     ],
    #     extra_compile_args=['-O3']
    # ),
]


if __name__ == "__main__":
    setup(ext_modules=extensions)
