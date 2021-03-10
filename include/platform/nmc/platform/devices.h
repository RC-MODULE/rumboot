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
#define FIFO_BASE    (NMC_REGS_BASE + 0x4000)
/* Not Sure. See NMPU.v for details */
#define AXIBR_BASE   (NMC_REGS_BASE + 0x5000)
//#define MPU_BASE     (NMC_REGS_BASE + 0x1400)
//#define DAP_BASE     (NMC_REGS_BASE + 0x1800)

#include <platform/o32t/platform/devices.h>

#endif