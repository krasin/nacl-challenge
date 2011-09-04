#!/bin/sh
x86_64-nacl-g++ -o lala.nexe graphics_2d_example.c -lppapi
x86_64-nacl-strip lala.nexe