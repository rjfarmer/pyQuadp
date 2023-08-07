from setuptools import setup, Extension

qmath = Extension(
    "qmathc",
    sources=["pyquadp/qfloat.c", "pyquadp/qmathc.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qcmath = Extension(
    "qcmathc",
    sources=["pyquadp/qfloat.c", "pyquadp/qcmplx.c", "pyquadp/qcmathc.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qfloat = Extension(
    "qfloat",
    sources=["pyquadp/qfloat.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)


qcmplx = Extension(
    "qcmplx",
    sources=["pyquadp/qfloat.c", "pyquadp/qcmplx.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qint = Extension(
    "qint",
    sources=["pyquadp/qint.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)


setup(
    ext_modules=[qfloat, qcmplx, qint, qmath, qcmath],
)
