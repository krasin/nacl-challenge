#!/bin/sh
set -e
x86_64-nacl-gcc -g -o x86_64-dbg-demo.nexe x86_64/*.S graphics_2d_example.c -Wl,--defsym,__nacl_rodata_start=0x10000000 -nostdlib -Wl,-T,x86_64/elf_nacl.x #-lnacl #-lc
cp x86_64-dbg-demo.nexe x86_64-opt-demo.nexe
x86_64-nacl-strip x86_64-opt-demo.nexe
