#ifndef __WD_REGS__
#define __WD_REGS__

#include <stdint.h>
#include <platform/oi10/platform/mivem_regs_access.h>

typedef enum{
    WDOGLOAD        = 0x00,// R/W 32 0xFFFFFFFF See Watchdog Load Register on page 4-15
    WDOGVALUE       = 0x04,// RO 32 0xFFFFFFFF See Watchdog Value Register on page 4-15
    WDOGCONTROL     = 0x08,// R/W 2 0x0 See Watchdog Control Register on page 4-15
    WDOGINTCLR      = 0x0C,// WO - - See Watchdog Clear Interrupt Register on page 4-15
    WDOGRIS         = 0x10,// RO,// 1 0x0 See Watchdog Raw Interrupt Status Register on page 4-16
    WDOGMIS         = 0x14,// RO,// 1 0x0 See Watchdog Interrupt Status Register on page 4-16
    WDOGLOCK        = 0xC00,// R/W 32 0x0 See Watchdog Lock Register on page 4-17
    WDOGITCR        = 0xF00,// R/W 1 0x0 See Watchdog Integration Test Control Register on page 4-18
    WDOGITOP        = 0xF04,// WO 2 0x0 See Watchdog Integration Test Output Set Register on     page 4-18
    WDOGPERIPHID0   = 0xFE0,// RO 8 0x05 See Peripheral Identification Register 0 on page 4-20
    WDOGPERIPHID1   = 0xFE4,// RO 8 0x18 See Peripheral Identification Register 1 on page 4-20
    WDOGPERIPHID2   = 0xFE8,// RO 8 0x04 See Peripheral Identification Register 2 on page 4-21
    WDOGPERIPHID3   = 0xFEC,// RO 8 0x00 See Peripheral Identification Register 3 on page 4-21
    WDOGPCELLID0    = 0xFF0,// RO 8 0x0D See PrimeCell Identification Register 0 on page 4-22
    WDOGPCELLID1    = 0xFF4,// RO 8 0xF0 See PrimeCell Identification Register 1 on page 4-22
    WDOGPCELLID2    = 0xFF8,// RO 8 0x05 See PrimeCell Identification Register 2 on page 4-22
    WDOGPCELLID3    = 0xFFC,// RO 8 0xB1 See PrimeCell Identification Register 3 on page 4-23
}WD_REGS;

typedef enum{
    WDOGLOAD_VALUE        = 0xFFFFFFFF,// R/W 32 0xFFFFFFFF See Watchdog Load Register on page 4-15
    WDOGVALUE_VALUE       = 0xFFFFFFFF,// RO 32 0xFFFFFFFF See Watchdog Value Register on page 4-15
    WDOGCONTROL_VALUE     = 0x0,// R/W 2 0x0 See Watchdog Control Register on page 4-15
    WDOGRIS_VALUE         = 0x0,// RO,// 1 0x0 See Watchdog Raw Interrupt Status Register on page 4-16
    WDOGMIS_VALUE         = 0x0,// RO,// 1 0x0 See Watchdog Interrupt Status Register on page 4-16
    WDOGLOCK_VALUE        = 0x0,// R/W 32 0x0 See Watchdog Lock Register on page 4-17
    WDOGITCR_VALUE        = 0x0,// R/W 1 0x0 See Watchdog Integration Test Control Register on page 4-18
    WDOGPERIPHID0_VALUE   = 0x05,// RO 8 0x05 See Peripheral Identification Register 0 on page 4-20
    WDOGPERIPHID1_VALUE   = 0x18,// RO 8 0x18 See Peripheral Identification Register 1 on page 4-20
    WDOGPERIPHID2_VALUE   = 0x14,// RO 8 0x04 See Peripheral Identification Register 2 on page 4-21
    WDOGPERIPHID3_VALUE   = 0x00,// RO 8 0x00 See Peripheral Identification Register 3 on page 4-21
    WDOGPCELLID0_VALUE    = 0x0D,// RO 8 0x0D See PrimeCell Identification Register 0 on page 4-22
    WDOGPCELLID1_VALUE    = 0xF0,// RO 8 0xF0 See PrimeCell Identification Register 1 on page 4-22
    WDOGPCELLID2_VALUE    = 0x05,// RO 8 0x05 See PrimeCell Identification Register 2 on page 4-22
    WDOGPCELLID3_VALUE    = 0xB1,// RO 8 0xB1 See PrimeCell Identification Register 3 on page 4-23
}WD_VALUES;

typedef enum{
    WDOGLOAD_MASK        = 0xFFFFFFFF,// R/W 32 0xFFFFFFFF See Watchdog Load Register on page 4-15
    WDOGVALUE_MASK       = 0xFFFFFFFF,// RO 32 0xFFFFFFFF See Watchdog Value Register on page 4-15
    WDOGCONTROL_MASK     = 0x00000003,// R/W 2 0x0 See Watchdog Control Register on page 4-15
    WDOGRIS_MASK         = 0x00000001,// RO,// 1 0x0 See Watchdog Raw Interrupt Status Register on page 4-16
    WDOGMIS_MASK         = 0x00000001,// RO,// 1 0x0 See Watchdog Interrupt Status Register on page 4-16
    WDOGLOCK_MASK        = 0xFFFFFFFF,// R/W 32 0x0 See Watchdog Lock Register on page 4-17
    WDOGITCR_MASK        = 0x00000001,// R/W 1 0x0 See Watchdog Integration Test Control Register on page 4-18
    WDOGPERIPHID0_MASK   = 0x000000FF,// RO 8 0x05 See Peripheral Identification Register 0 on page 4-20
    WDOGPERIPHID1_MASK   = 0x000000FF,// RO 8 0x18 See Peripheral Identification Register 1 on page 4-20
    WDOGPERIPHID2_MASK   = 0x000000FF,// RO 8 0x04 See Peripheral Identification Register 2 on page 4-21
    WDOGPERIPHID3_MASK   = 0x000000FF,// RO 8 0x00 See Peripheral Identification Register 3 on page 4-21
    WDOGPCELLID0_MASK    = 0x000000FF,// RO 8 0x0D See PrimeCell Identification Register 0 on page 4-22
    WDOGPCELLID1_MASK    = 0x000000FF,// RO 8 0xF0 See PrimeCell Identification Register 1 on page 4-22
    WDOGPCELLID2_MASK    = 0x000000FF,// RO 8 0x05 See PrimeCell Identification Register 2 on page 4-22
    WDOGPCELLID3_MASK    = 0x000000FF,// RO 8 0xB1 See PrimeCell Identification Register 3 on page 4-23
}WD_MASKS;


REG_READ(wd,WDOGLOAD,32)
REG_WRITE(wd,WDOGLOAD,32)

REG_READ(wd,WDOGVALUE,32)

REG_READ(wd,WDOGCONTROL,32)
REG_WRITE(wd,WDOGCONTROL,32)

REG_WRITE(wd,WDOGINTCLR,32)

REG_READ(wd,WDOGRIS,32)

REG_READ(wd,WDOGMIS,32)

REG_READ(wd,WDOGLOCK,32)
REG_WRITE(wd,WDOGLOCK,32)

REG_READ(wd,WDOGITCR,32)
REG_WRITE(wd,WDOGITCR,32)

REG_WRITE(wd,WDOGITOP,32)

REG_READ(wd,WDOGPERIPHID0,32)
REG_READ(wd,WDOGPERIPHID1,32)
REG_READ(wd,WDOGPERIPHID2,32)
REG_READ(wd,WDOGPERIPHID3,32)

REG_READ(wd,WDOGPCELLID0,32)
REG_READ(wd,WDOGPCELLID1,32)
REG_READ(wd,WDOGPCELLID2,32)
REG_READ(wd,WDOGPCELLID3,32)

//unlock value
#define WD_UNLOCK_CODE  0x1ACCE551

//fields
//CONTROL
#define INTEN   0
#define RESEN   1


#endif //__WD_REGS__
