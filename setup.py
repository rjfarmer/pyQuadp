from setuptools import setup, Extension


qimath = Extension(
    "qimath",
    sources=["pyquad/qimath.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qmath = Extension(
    "qmath",
    sources=["pyquad/qfloat.c", "pyquad/qmath.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qcmath = Extension(
    "qcmath",
    sources=["pyquad/qcmath.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qfloat = Extension(
    "qfloat",
    sources=["pyquad/qfloat.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qint = Extension(
    "qint",
    sources=["pyquad/qint.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qcmplx = Extension(
    "qcmplx",
    sources=["pyquad/qcmplx.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)


setup(
    ext_modules=[qmath, qimath, qcmath, qfloat, qint, qcmplx],
)
