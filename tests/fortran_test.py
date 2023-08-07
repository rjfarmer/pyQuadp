import ctypes
import os
import select
from pprint import pprint
import time
import platform

import pytest

import pyquadp as pq
import pyquadp.qmath as qm
import pyquadp.qcmath as qcm


def lib_ext():
    os = platform.system()
    if os == "Darwin":
        return "dylib"
    elif os == "Windows":
        return "dll"
    else:
        return "so"


libname = f"./tests/quad.{lib_ext()}"
mod_name = "__testq_MOD_"
lib = ctypes.CDLL(libname)


class _captureStdOut:
    def read_pipe(self, pipe_out):
        def more_data():
            r, _, _ = select.select([pipe_out], [], [], 0)
            return bool(r)

        out = b""
        while more_data():
            out += os.read(pipe_out, 1024)
        return out.decode()

    def __enter__(self):
        self.pipe_out, self.pipe_in = os.pipe()
        self.stdout = os.dup(1)
        os.dup2(self.pipe_in, 1)

    def __exit__(self, *args, **kwargs):
        os.dup2(self.stdout, 1)
        print(self.read_pipe(self.pipe_out))
        os.close(self.pipe_in)
        os.close(self.pipe_out)
        os.close(self.stdout)


class TestFortranPassing:
    def test_single_quad(self, capfd):
        func = getattr(lib, f"{mod_name}single_quad")
        func.restype = None
        func.argtypes = [ctypes.c_char_p]

        q = pq.qfloat("3.141592653590000061569753597723320127e+00")
        b1 = q.to_bytes()
        with _captureStdOut() as cs:
            func(b1)
        cap = capfd.readouterr()
        assert pq.qfloat(cap.out.strip()) == q

    def test_single_quad2(self, capfd):
        func = getattr(lib, f"{mod_name}single_quad")
        func.restype = None
        func.argtypes = [ctypes.c_char_p]

        q = pq.qfloat(1)
        b1 = q.to_bytes()
        with _captureStdOut() as cs:
            func(b1)
        cap = capfd.readouterr()
        assert pq.qfloat(cap.out.strip()) == q

    def test_single_quad3(self, capfd):
        func = getattr(lib, f"{mod_name}single_quad")
        func.restype = None

        q = pq.qfloat("3.141592653590000061569753597723320127e+00")
        with _captureStdOut() as cs:
            func(q)
        cap = capfd.readouterr()
        assert pq.qfloat(cap.out.strip()) == q

    def test_single_quad4(self, capfd):
        func = getattr(lib, f"{mod_name}single_quad")
        func.restype = None
        func.argtypes = [pq.qfloat]

        q = pq.qfloat("3.141592653590000061569753597723320127e+00")
        with _captureStdOut() as cs:
            func("3.141592653590000061569753597723320127e+00")
        cap = capfd.readouterr()
        assert pq.qfloat(cap.out.strip()) == q

    @pytest.mark.skip("No quad return support yet in ctypes")
    def test_ret_quad(self, capfd):
        func = getattr(lib, f"{mod_name}return_quad")
        func.restype = ctypes.c_char_p
        func.argtypes = None

        b1 = func()
        q1 = pq.qfloat.from_bytes(b1)
        assert q1 == "3.14000000000000000000000000000000010785207688568520777113e+0000"

    def test_single_cmplx(self, capfd):
        func = getattr(lib, f"{mod_name}single_quadc")
        func.restype = None
        func.argtypes = [ctypes.c_char_p]

        q = pq.qcmplx(
            "3.141592653590000061569753597723320127e+00",
            "3.141592653590000061569753597723320127e+00",
        )
        b1 = q.to_bytes()
        with _captureStdOut() as cs:
            func(b1)
        cap = capfd.readouterr()

        c = cap.out.strip().strip("()").split(",")
        assert pq.qcmplx(*c) == q

    def test_single_cmplx2(self, capfd):
        func = getattr(lib, f"{mod_name}single_quadc")
        func.restype = None
        func.argtypes = [ctypes.c_char_p]

        q = pq.qcmplx(
            "3.141592653590000061569753597723320127e+00",
            "3.141592653590000061569753597723320127e+00",
        )
        with _captureStdOut() as cs:
            func(q)
        cap = capfd.readouterr()

        c = cap.out.strip().strip("()").split(",")
        assert pq.qcmplx(*c) == q

    def test_single_cmplx3(self, capfd):
        func = getattr(lib, f"{mod_name}single_quadc")
        func.restype = None
        func.argtypes = [pq.qcmplx]

        c = complex(3.14, 3.14)

        q = pq.qcmplx(c)
        with _captureStdOut() as cs:
            func(c)
        cap = capfd.readouterr()

        c = cap.out.strip().strip("()").split(",")
        assert pq.qcmplx(*c) == q
