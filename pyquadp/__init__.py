# SPDX-License-Identifier: GPL-2.0+

from importlib import import_module
from types import ModuleType

from . import constant as _constant
from .constant import *

import builtins as _builtins

_BOOTSTRAPPED = False

qmfloat: ModuleType
qmint: ModuleType
qmcmplx: ModuleType
qarray: ModuleType
qcarray: ModuleType
qiarray: ModuleType

qfloat: type
qint: type
qcmplx: type


def _constant_exports() -> tuple[str, ...]:
    if hasattr(_constant, "__all__"):
        return tuple(getattr(_constant, "__all__"))

    return tuple(name for name in dir(_constant) if not name.startswith("_"))


_CONSTANT_EXPORTS = _constant_exports()


__all__ = [
    "qint",
    "qfloat",
    "qcmplx",
    "qmint",
    "qmfloat",
    "qmcmplx",
    "qarray",
    "qcarray",
    "qiarray",
]
__all__.extend(_CONSTANT_EXPORTS)  # pyright: ignore[reportUnsupportedDunderAll]


def _bootstrap_core_modules() -> None:
    if _BOOTSTRAPPED:
        return

    # Load extension modules in dependency order.
    _qmfloat = import_module(".qmfloat", __name__)
    _qfloat = _qmfloat.qfloat

    _qmint = import_module(".qmint", __name__)
    _qint = _qmint.qint

    _qmcmplx = import_module(".qmcmplx", __name__)
    _qcmplx = _qmcmplx.qcmplx

    globals().update(
        {
            "qmfloat": _qmfloat,
            "qfloat": _qfloat,
            "qmint": _qmint,
            "qint": _qint,
            "qmcmplx": _qmcmplx,
            "qcmplx": _qcmplx,
            "qarray": import_module(".qarray", __name__),
            "qcarray": import_module(".qcarray", __name__),
            "qiarray": import_module(".qiarray", __name__),
        }
    )

    globals()["_BOOTSTRAPPED"] = True


def _register_pickle_builtins() -> None:
    # Keep scalar types pickle-importable for historical compatibility.
    setattr(_builtins, "qint", qint)
    setattr(_builtins, "qfloat", qfloat)
    setattr(_builtins, "qcmplx", qcmplx)


_bootstrap_core_modules()
_register_pickle_builtins()
