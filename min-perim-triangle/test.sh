#!/usr/bin/env bash

gcc brute.c -O3 -o brute
gcc triangles.c -O3 -o triangles

python test.py
