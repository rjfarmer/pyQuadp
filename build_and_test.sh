#!/bin/bash

python -m build && python -m pip install . && python -m pytest 
