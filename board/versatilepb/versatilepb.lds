OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
/*OUTPUT_FORMAT("elf32-arm", "elf32-arm", "elf32-arm")*/
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
    . = 0x00000000;

    . = ALIGN(4);
    .text      :
    {
      *(.head.text)
      *(.text)
      lib_arm/div0.o    (.text)
    }

    . = ALIGN(4);
    .rodata : { *(.rodata) }

    . = ALIGN(4);
    __exidx_start = .;
    PROVIDE(__exidx_start = __exidx_start);
    .ARM.exidx :
    {
        /* __exidx_start = .; */
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        /* __exidx_end = .; */
    }
    __exidx_end = .;

    . = ALIGN(4);
    __data_start = .;
    .data : { *(.data) }

    . = ALIGN(4);
    __bss_start = .;
    .bss : { *(.bss) }
    . = ALIGN(4);
    __bss_end = .;

    . = ALIGN(4);
    . = . + 0x1000; /* 4kB of stack memory */
    __stack_irq_end__ = .;

    . = ALIGN(4);
    . = . + 0x1000; /* 4kB of stack memory */
    __stack_svc_end__ = .;

    /* for threadx */
    . = ALIGN(8);
    _stack_bottom = .;
    . = . + 0x1000; /* 4kB of stack memory */
    _sp =  . - 16 ;
    _end = .;
}
