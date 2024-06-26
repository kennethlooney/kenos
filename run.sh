#!/bin/bash

# From image
#qemu-system-x86_64 -drive format=raw,unit=0,file=kenos.img -name KENOS -bios bios64.bin -m 256M -vga std -machine q35 -net none

# From directory
qemu-system-x86_64 -name KENOS -bios bios64.bin -m 256M -vga std -machine q35 -net none -hda fat:rw:image