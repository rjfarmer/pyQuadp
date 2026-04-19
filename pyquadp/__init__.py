# SPDX-License-Identifier: GPL-2.0+

from .qmfloat import qfloat
from .qmint import qint
from .qmcmplx import qcmplx
from . import qarray
from . import qcarray
from . import qiarray

from . import constant as _constant
from .constant import *

import builtins as _builtins


def _constant_exports() -> tuple[str, ...]:
    if hasattr(_constant, "__all__"):
        return tuple(getattr(_constant, "__all__"))

    return tuple(name for name in dir(_constant) if not name.startswith("_"))


__all__ = (
    "qint",
    "qfloat",
    "qcmplx",
    "qarray",
    "qcarray",
    "qiarray",
    *_constant_exports(),
)


def _register_pickle_builtins() -> None:
    # Keep scalar types pickle-importable for historical compatibility.
    _builtins.qint = qint
    _builtins.qfloat = qfloat
    _builtins.qcmplx = qcmplx


_register_pickle_builtins()
