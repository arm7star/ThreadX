#define SP804_T23_LOAD       (*((volatile unsigned int *) 0x101e3000))
#define SP804_T23_VALUE      (*((volatile unsigned int *) 0x101e3004))
#define SP804_T23_CTRL       (*((volatile unsigned int *) 0x101e3008))
#define SP804_T23_INTCTRL    (*((volatile unsigned int *) 0x101e300c))
#define SP804_T23_RIS        (*((volatile unsigned int *) 0x101e3010))
#define SP804_T23_MIS        (*((volatile unsigned int *) 0x101e3014))
#define SP804_T23_BGLOAD     (*((volatile unsigned int *) 0x101e3018))

#define VIC_INT_ENABLE       (*((volatile unsigned int *) 0x10140010))

/*
;*****************************************************************************************************
;* 函数名称 : Timer23_Exception
;* 描    述 : 定时器0中断服务子程序 
;* 输　    入 : 无
;*        
;* 输　    出 : 无
;*****************************************************************************************************
;*/
void Timer23_Exception(void)
{
    SP804_T23_INTCTRL = 0;  /* clear interrupt flag MR0 */
}

/*
;*****************************************************************************************************
;* 函数名称 : sp804_init
;* 描    述 : 初始化定时器
;* 输　    入 : 无
;*
;* 输　    出 : 无
;*****************************************************************************************************
;*/
void sp804_init(void)
{
    SP804_T23_CTRL = 0;
    SP804_T23_BGLOAD = 0;
    SP804_T23_LOAD = 1000000 / 1000 * 10;
    SP804_T23_CTRL = 0xe2;
    
    unmask_irq(5);
    irq_enable();
}
