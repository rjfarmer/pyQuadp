# pyQuadp Copilot Instructions

This repository contains CPython C extensions and Python wrappers. Follow these rules for all changes.

## C Extension Safety Rules

- Never call `PyErr_Print()` in library/runtime code paths. Propagate existing exceptions instead.
- Do not use unguarded `PyErr_Clear()` in parse fallbacks. Clear only expected exceptions via `PyErr_ExceptionMatches(...)`.
- For functions returning `bool`, return `false`, not `NULL`.
- Always check nullable results from Python C API before reuse:
  - `PyUnicode_FromString` / `PyUnicode_FromFormat`
  - `PyBytes_FromStringAndSize`
  - `PyObject_Call*`
  - `PyLong_AsLong` (check `PyErr_Occurred()` when result is `-1`)
- `PyDict_GetItemWithError(...)` contract:
  - `NULL` with exception set -> propagate error
  - `NULL` with no exception -> missing key; set explicit `KeyError`
- `Py_BuildValue` ownership:
  - If passing a new reference temporary, use `"N"`
  - If using `"O"`, decref temporary explicitly after build
- Avoid inline temporary key creation inside dict lookups (creates leaks and hides NULL checks). Create key object, check it, use it, decref it.
- Do not pass temporaries directly into `PyObject_Hash(...)` when they are new references. Store, hash, decref.

## Module Init Ownership Rules

- For `PyModule_AddObject`, remember it steals references only on success.
- On failure after manual `Py_INCREF`, decref that object before returning.
- Prefer `PyModule_AddObjectRef` / `PyModule_AddType` when feasible for clearer ownership.

## Python Wrapper Parity Rules

- Keep wrapper signatures in sync with bound C functions (arity and names).
- Verify every wrapper target exists in the corresponding C method table.
- For renamed C symbols, update wrappers and tests in the same change.

## Required Validation For Relevant Changes

- Run affected tests when touching wrappers or C-extension conversion/pickling/hash paths.
- Add or update tests for:
  - wrapper arity/name mismatches
  - pickle round-trip (`__getstate__`/`__setstate__`)
  - hash path error handling
  - missing-key behavior in `__setstate__`
