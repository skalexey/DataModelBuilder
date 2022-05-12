#!/usr/bin/bash

python clean.py
[ $? -ne 0 ] && echo "--- Error during clean process" || echo "--- Clean done"
python build.py "$@"

