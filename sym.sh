#!/bin/bash

cd Debug
arm-none-eabi-objdump -t USBx.elf  | grep \.text | sort -k 6 | awk '{$1=""}1' >sym.lst

cd ../cmake-build-debug
arm-none-eabi-objdump -t USBx.elf  | grep \.text | sort -k 6 | awk '{$1=""}1' >sym.lst

cd
