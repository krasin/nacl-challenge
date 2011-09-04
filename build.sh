#!/bin/sh
set -e
x86_64-nacl-gcc -o lala_full.nexe graphics_2d_example.c -lppapi
cp lala_full.nexe lala.nexe
x86_64-nacl-strip lala.nexe