#ifndef NMC_DEVICES_H
#define NMC_DEVICES_H

/**  
    !!!BIG FAT WARNING!!! 
    ---------------------
    All NeuroMatrix docs have adresses specified in 4-byte words.
    This breaks many things, and in order to provide some degree of 
    compatibility ioread/iowrite functions accept BYTE adresses.

    This means that we have to specify adresses in bytes here for it
    to be happy.
*/

#define NMC_REGS_BASE 0xE0000

/* TODO: Internal blocks may have different order in different SoC's */
/* Prehaps we have to somehow accomodate this later ? */

#define INTC_BASE    (NMC_REGS_BASE + 0x0000)
#define SC_BASE      (NMC_REGS_BASE + 0x1000)
#define TIMERS_BASE  (NMC_REGS_BASE + 0x2000)
#define IFU_BASE     (NMC_REGS_BASE + 0x3000)
#define AXIBR_BASE   (NMC_REGS_BASE + 0x4000)
#define FIFO_BASE    (NMC_REGS_BASE + 0x5000)
#define DAP_BASE     (NMC_REGS_BASE + 0x6000)
#define DBG_BASE     (NMC_REGS_BASE + 0x7000)

/* nmc-gcc asm compiler don't understand hex numbers in it's patterns
   with math operations. */

#define NMC_REGS_BASE_DEC 917504

#define DAP_BASE_DEC (NMC_REGS_BASE_DEC + 24576)
#define DBG_BASE_DEC (NMC_REGS_BASE_DEC + 28672)

#include <platform/o32t/platform/devices.h>

#undef IM1_BASE
#define IM1_BASE 0x70000000

#undef IM2_BASE
#define IM2_BASE 0x70010000

#undef IM3_BASE
#define IM3_BASE 0x70018000



#endif