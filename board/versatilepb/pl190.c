#define VIC_IRQ_STATUS		(*((volatile unsigned int *) 0x10140000))
#define VIC_FIQ_STATUS 		(*((volatile unsigned int *) 0x10140004))
#define VIC_RAW_INTR 		(*((volatile unsigned int *) 0x10140008))
#define VIC_INT_SELECT 		(*((volatile unsigned int *) 0x1014000c))
#define VIC_INT_ENABLE 		(*((volatile unsigned int *) 0x10140010))
#define VIC_INT_CLEAR 		(*((volatile unsigned int *) 0x10140014))
#define VIC_SOFT_INT 		(*((volatile unsigned int *) 0x10140018))
#define VIC_SOFT_INT_CLEAR 	(*((volatile unsigned int *) 0x1014001c))
#define VIC_PROTECTION 		(*((volatile unsigned int *) 0x10140020))
#define VIC_VECT_ADDR 		(*((volatile unsigned int *) 0x10140030))
#define VIC_DEF_VECT_ADDR 	(*((volatile unsigned int *) 0x10140034))

void mask_irq(unsigned int hwirq)
{
	VIC_INT_ENABLE &= ~(1 << hwirq);
}

void unmask_irq(unsigned int hwirq)
{
	VIC_INT_ENABLE |= 1 << hwirq;
}
