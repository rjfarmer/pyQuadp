from setuptools import setup, Extension

qmath = Extension(
    "qmathc",
    sources=["pyquad/qfloat.c", "pyquad/qmathc.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qcmath = Extension(
    "qcmathc",
    sources=["pyquad/qfloat.c", "pyquad/qcmplx.c","pyquad/qcmathc.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)

qfloat = Extension(
    "qfloat",
    sources=["pyquad/qfloat.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)


qcmplx = Extension(
    "qcmplx",
    sources=["pyquad/qfloat.c", "pyquad/qcmplx.c"],
    libraries=["quadmath"],
    extra_compile_args=["-ggdb"],
)


setup(
    ext_modules=[qmath, qcmath, qfloat, qcmplx],
)
