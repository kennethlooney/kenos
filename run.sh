#!/bin/bash

qemu-system-x86_64 -name KENOS -bios bios64.bin -net none -hda fat:rw:image