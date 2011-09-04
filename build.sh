#!/bin/sh
set -e

build_nexe() {
  ARCH=$1 # i686 or x86_64 or arm
  $ARCH-nacl-gcc -g -o $ARCH-dbg-demo.nexe $ARCH/*.S graphics_2d_example.c -Wl,--defsym,__nacl_rodata_start=0x10000000 -nostdlib -Wl,-T,$ARCH/elf_nacl.x
  cp $ARCH-dbg-demo.nexe $ARCH-opt-demo.nexe
  $ARCH-nacl-strip $ARCH-opt-demo.nexe  
}

build_nexe x86_64
build_nexe i686

