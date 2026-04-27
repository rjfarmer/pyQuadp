# SPDX-License-Identifier: GPL-2.0+

import builtins as _builtins
from importlib import import_module
from types import ModuleType

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


def _bootstrap_core_modules() -> None:
    if _BOOTSTRAPPED:
        return

    # Load extension modules in dependency order and bind each dependency
    # before importing dependents to avoid circular import failures.
    _qmfloat = import_module(".qmfloat", __name__)
    globals().update({"qmfloat": _qmfloat, "qfloat": _qmfloat.qfloat})

    _qmint = import_module(".qmint", __name__)
    globals().update({"qmint": _qmint, "qint": _qmint.qint})

    _qmcmplx = import_module(".qmcmplx", __name__)
    globals().update({"qmcmplx": _qmcmplx, "qcmplx": _qmcmplx.qcmplx})

    globals().update(
        {
            "qarray": import_module(".qarray", __name__),
            "qcarray": import_module(".qcarray", __name__),
            "qiarray": import_module(".qiarray", __name__),
        }
    )

    globals()["_BOOTSTRAPPED"] = True


def _constant_exports() -> tuple[str, ...]:
    if hasattr(_constant, "__all__"):
        return tuple(getattr(_constant, "__all__"))

    return tuple(name for name in dir(_constant) if not name.startswith("_"))


def _register_pickle_builtins() -> None:
    # Keep scalar types pickle-importable for historical compatibility.
    setattr(_builtins, "qint", qint)
    setattr(_builtins, "qfloat", qfloat)
    setattr(_builtins, "qcmplx", qcmplx)


_bootstrap_core_modules()

from . import constant as _constant
from .constant import *

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

_register_pickle_builtins()
