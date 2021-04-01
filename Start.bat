@echo off
title Azure RTOS ThreadX
qemu-system-arm.exe -nographic -M versatilepb -m 128M -kernel threadx 2>nul
