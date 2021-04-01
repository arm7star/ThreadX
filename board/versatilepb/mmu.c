#define ENABLE_DCACHE_ICACHE  0

#define MMU_BASE              0x03000000


#define MMU_FULL_ACCESS       (3 << 10)  /* 访问权限 */
#define MMU_DOMAIN            (0 << 5)   /* 属于哪个域 */
#define MMU_SPECIAL           (1 << 4)   /* bit 4必须是1 */
#define MMU_CACHEABLE         (1 << 3)   /* 可以使用cache */
#define MMU_BUFFERABLE        (1 << 2)   /* 可以使用write buffer */
#define MMU_SECDESC           (2)        /* 表示这是段描述符 */

#define MMU_SECDESC_WB        (MMU_FULL_ACCESS | MMU_DOMAIN | \
                               MMU_SPECIAL | MMU_CACHEABLE | \
                               MMU_BUFFERABLE | MMU_SECDESC)

#define MMU_SECDESC_NCNB      (MMU_FULL_ACCESS | MMU_DOMAIN | \
                               MMU_SPECIAL | MMU_SECDESC)


void versatilepb_mmu_init(void)
{
    volatile unsigned long *table = (volatile unsigned long *)MMU_BASE;

    /* 建立页表项 */
    table[0x101] = 0x10100000 | MMU_SECDESC_NCNB; /* va: 0x10100000 => pa: 0x10100000 */

    /* va: 0 => pa: 0 */
    table[0x000] = 0x00000000 | MMU_SECDESC_WB; /* MMU使能前后的代码所在的空间, 保持它们的虚拟地址等于物理地址 */
    table[0x001] = 0x00100000 | MMU_SECDESC_WB; /* va: 0x00100000 => pa: 0x00100000 */
    table[0x002] = 0x00200000 | MMU_SECDESC_WB; /* va: 0x00200000 => pa: 0x00200000 */
    table[0x003] = 0x00300000 | MMU_SECDESC_WB; /* va: 0x00300000 => pa: 0x00300000 */

    /*
     To enable the MMU:
     1) Program the translation table base and domain access control registers.
     2) Program level 1 and level 2 page tables as required.
     3) Enable the MMU by setting bit 0 in the control register.
     */

    /* 把页表项的基地址告诉MMU */
    /* 启动MMU */
    __asm__ (
        "mov    r1, #0\n"
        "mcr    p15, 0, r1, c7, c7, 0\n"  /* 使无效ICaches和DCaches */

        "mcr    p15, 0, r1, c7, c10, 4\n" /* drain write buffer on v4 */
        "mcr    p15, 0, r1, c8, c7, 0\n"  /* 使无效指令、数据TLB */


        "mcr p15, 0, %0, c2, c0, 0\n"     /* write TTB register */
        "mrc p15, 0, r1, c3, c0, 0\n"     /* read domain 15:0 access permissions */
        "orr r1, r1, #3\n"                /* domain 0, Accesses are not checked */
        "mcr p15, 0, r1, c3, c0, 0\n"     /* write domain 15:0 access permissions */

        "mrc p15, 0, r1, c1, c0, 0\n"     /* Read control register */

#if ENABLE_DCACHE_ICACHE
        "orr r1, r1, #(1<<2)\n"           /* Data cache enable */
        "orr r1, r1, #(1<<12)\n"          /* Instruction cache enable */
        "orr r1, r1, #(1<<14)\n"          /* Round robin replacement */
#endif
        "orr r1, r1, #(1<<0)\n"           /* MMU enable */

        "mcr p15,0,r1,c1, c0,0\n"         /* write control register */
        :
        : "r" (table)
        : "r1"
    );
}
