#!/bin/sh
set -e
x86_64-nacl-gcc -g -o lala_full.nexe startup/*.S graphics_2d_example.c -Wl,--defsym,__nacl_rodata_start=0x10000000 -nostdlib -Wl,-T,elf64_nacl.x #-lnacl #-lc
cp lala_full.nexe lala.nexe
x86_64-nacl-strip lala.nexe
