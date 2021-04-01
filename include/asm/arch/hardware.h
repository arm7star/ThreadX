#ifndef _ARCH_HARDWARE_H_
#define _ARCH_HARDWARE_H_

#ifndef __ASSEMBLY__
#define __REG(x)	(*(unsigned long *)(x))
#define __REGl(x)	(*(unsigned long *)(x))
#define __REGw(x)	(*(unsigned short *)(x))
#define __REGb(x)	(*(unsigned char *)(x))
#define __REG2(x,y)	(*(unsigned long *)((x) + (y)))
#else
#define __REG(x)	(x)
#define __REGl(x)	(x)
#define __REGw(x)	(x)
#define __REGb(x)	(x)
#define __REG2(x,y)	((x) + (y))
#endif

#endif /* _ARCH_HARDWARE_H_ */

