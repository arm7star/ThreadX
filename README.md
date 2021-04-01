# AzureRTOS-ThreadX
```
ThreadX 6.1.2 for ARM Versatile/PB (ARM926EJ-S)
https://github.com/azure-rtos/threadx/releases/tag/v6.1.2_rel
```

# Build
```
toolchain: 
	gcc-arm-none-eabi-4_9-2015q3-20150921-linux
	gcc-arm-none-eabi-4_9-2015q3-20150921-win32(Cygwin, make.exe for windows)

clean project:
	make clean; make distclean; make clobber
build project:
	make all
```

# Run
qemu-system-arm -nographic -M versatilepb -m 128M -kernel threadx

```
**** ThreadX Win32 Demonstration **** (c) 1996-2004 Express Logic, Inc.

           thread 0 events sent:          1
           thread 1 messages sent:        0
           thread 2 messages received:    0
           thread 3 obtained semaphore:   0
           thread 4 obtained semaphore:   0
           thread 5 events received:      0
**** ThreadX Win32 Demonstration **** (c) 1996-2004 Express Logic, Inc.

           thread 0 events sent:          2
           thread 1 messages sent:        1738181
           thread 2 messages received:    1738097
           thread 3 obtained semaphore:   26
           thread 4 obtained semaphore:   25
           thread 5 events received:      1
**** ThreadX Win32 Demonstration **** (c) 1996-2004 Express Logic, Inc.

           thread 0 events sent:          3
           thread 1 messages sent:        3480735
           thread 2 messages received:    3480648
           thread 3 obtained semaphore:   51
           thread 4 obtained semaphore:   50
           thread 5 events received:      2
```
