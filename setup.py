from setuptools import setup, Extension


qimath = Extension('qimath',
                    sources = ['pyquad/qdef.c','pyquad/qimath.c'],
                    libraries = ['quadmath'],
                     )

qmath = Extension('qmath',
                    sources = ['pyquad/qdef.c','pyquad/qfloat.c','pyquad/qmath.c'],
                    libraries = ['quadmath'],
                     )

qcmath = Extension('qcmath',
                    sources = ['pyquad/qdef.c','pyquad/qcmath.c'],
                    libraries = ['quadmath'],
                     )

qfloat = Extension('qfloat',
                    sources = ['pyquad/qdef.c','pyquad/qfloat.c'],
                    libraries = ['quadmath'],
                     )

qint = Extension('qint',
                    sources = ['pyquad/qdef.c','pyquad/qint.c'],
                    libraries = ['quadmath'],
                     )

qcmplx = Extension('qcmplx',
                    sources = ['pyquad/qdef.c','pyquad/qcmplx.c'],
                    libraries = ['quadmath'],
                     )


setup(
       ext_modules = [qmath, qimath, qcmath, qfloat, qint, qcmplx],
       )

