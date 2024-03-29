/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************
 */
        .section .head.text
        .global _start
_start:
        ldr pc, ResetAddr
        ldr pc, _undefined_instruction
        ldr pc, _software_interrupt
        ldr pc, _prefetch_abort
        ldr pc, _data_abort
        ldr pc, _not_used
        ldr pc, _irq
        ldr pc, _fiq

ResetAddr:
        .word reset
_undefined_instruction:
        .word undefined_instruction
_software_interrupt:
        .word software_interrupt
_prefetch_abort:
        .word prefetch_abort
_data_abort:
        .word data_abort
_not_used:
        .word 0x12345678
_irq:
        .word __tx_irq_handler
_fiq:
        .word fiq
_pad:
        .word 0x12345678 /* now 16*4=64 */

.global _end_vect
_end_vect:
        .balignl 16,0xdeadbeef

.global _TEXT_BASE
_TEXT_BASE :
        .word   TEXT_BASE

.global _DATA_BASE
_DATA_BASE :
        .word   DATA_BASE


.global __start
__start :
        .word _start

/*
 * These are defined in the board-specific linker script.
 */
.global _data_start
_data_start :
        .word __data_start

.global _bss_start
_bss_start :
        .word __bss_start

.global _bss_end
_bss_end :
        .word __bss_end

/* Reset entry */
reset:
        /*
         * Setup a stack for SVC mode
         */
        msr cpsr_c, #(0x13 | 0x80 | 0x40) /* Supervisor Mode, IRQ/FIQ disabled */
        ldr sp, =__stack_svc_end__
        
        /* Clear bss Section */
clear_bss:
        ldr     r0, _bss_start              /* find start of bss segment        */
        ldr     r1, _bss_end                /* stop here                        */
        mov     r2, #0x00000000             /* clear                            */

clbss_l:
        str     r2, [r0]                    /* clear loop...                    */
        add     r0, r0, #4                  /* Set r0 to next bss address       */
        cmp     r0, r1                      /* Check if r0 to the _bss_end      */
        ble     clbss_l                     /* Continue to clear bss ram        */

        bl  _tx_initialize_low_level
        bl  versatilepb_initialize

        /*
         * Jump to C entry
         */
        mov     lr, pc
        ldr     pc, __main
        b       .

__main:
    .word main

/*
 *************************************************************************
 *
 * Interrupt Code
 *
 *************************************************************************
 */

/*
 * IRQ stack frame.
 */
#define S_FRAME_SIZE    72

#define S_OLD_R0        68
#define S_PSR           64
#define S_PC            60
#define S_LR            56
#define S_SP            52

#define S_IP            48
#define S_FP            44
#define S_R10           40
#define S_R9            36
#define S_R8            32
#define S_R7            28
#define S_R6            24
#define S_R5            20
#define S_R4            16
#define S_R3            12
#define S_R2            8
#define S_R1            4
#define S_R0            0

/*
 * use bad_save_user_regs for abort/prefetch/undef/swi ...
 * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling
 */
    .macro  bad_save_user_regs
        sub    sp, sp, #S_FRAME_SIZE
        stmia  sp, {r0 - r12}

        add    r0, sp, #S_FRAME_SIZE
        mov    r1, lr
        mov    r2, pc
        mrs    r3, spsr
        add    r5, sp, #S_SP
        stmia  r5, {r0 - r3}
        mov    r0, sp
    .endm


/*
 * Undefined Instruction interrupt
 */
        .align  5
undefined_instruction:
        b      undefined_instruction

/*
 * Software interrupt
 */
        .align  5
software_interrupt:
        b   software_interrupt

/*
 * Pre fetch abort interrupt
 */
        .align  5
prefetch_abort:
        b   prefetch_abort

/*
 * Data abort interrupt
 */
        .align  5
data_abort:
        b   data_abort

        .align  5
not_used:
        b   .

/*
 * IRQ interrupt
 */
        .align  5
irq:
        sub    lr, lr, #4
        stmfd  sp!, {r0-r3, r12, lr}
        @ bl   do_irq
        ldmfd  sp!, {r0-r3, r12, lr}
        movs   pc,  lr

/*
 * FIQ interrupt
 */
        .align  5
fiq:
        stmfd  sp!, {r0-r3, r12, lr}
        // bl     do_fiq
        ldmfd  sp!, {r0-r3, r12, lr}
        subs   pc,  lr,  #4

/*
 *************************************************************************
 *
 * IRQ/FIQ enable/disable
 *
 *************************************************************************
 */

#define LOCKOUT    0xC0                 // Interrupt lockout value
#define MODE_MASK  0x1F                 // Processor Mode Mask
#define MODE_SVC   0x13                 // Supervisor Mode (SVC)

#define I_BIT      0x80                 // Interrupt bit of CPSR and SPSR
#define F_BIT      0x40                 // Interrupt bit of CPSR and SPSR

/*
 * Enable IRQ interrupt
 */
        .align  5
.global irq_enable
irq_enable:
        mrs    r0, cpsr
        bic    r0, r0, #I_BIT
        msr    cpsr_c, r0
        mov    pc, lr

/*
 * Disable IRQ interrupt
 */
        .align  5
.global irq_disable
irq_disable:
        mrs    r0, cpsr
        orr    r0, r0, #I_BIT
        msr    cpsr_c, r0
        mov    pc, lr

/*
 * Disable FIQ interrupt
 */
        .align  5
fiq_disable:
        mrs    r0, spsr
        orr    r0, r0, #F_BIT
        msr    cpsr_c, r0
        mov    pc, lr

/*
 * Enable FIQ interrupt
 */
        .align  5
fiq_enable:
        mrs    r0, spsr
        bic    r0, r0, #F_BIT
        msr    cpsr_c, r0
        mov    pc, lr
