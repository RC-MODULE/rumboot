#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <devices/gic.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>

static inline void gic_dist_write(uint32_t reg, uint32_t value)
{
    iowrite32(value, GIC_DIST_BASE + reg);
}

static inline void gic_cpuif_write(uint32_t reg, uint32_t value)
{
    iowrite32(value, GIC_CPUIF_BASE + reg);
}

static inline uint32_t gic_dist_read(uint32_t reg)
{
    return ioread32(GIC_DIST_BASE + reg);
}

static inline uint32_t gic_cpuif_read(uint32_t reg)
{
    return ioread32(GIC_CPUIF_BASE + reg);
}


uint32_t rumboot_platform_irq_begin()
{
    return gic_cpuif_read(GICC_REG_IAR);
}

void rumboot_platform_irq_end(uint32_t id)
{
    gic_cpuif_write(GICC_REG_EOIR, id);
}


void rumboot_platform_irq_configure(int irq, uint32_t flags, int enable)
{
    gic_dist_write(GICD_REG_CTLR, 0);
    gic_cpuif_write(GICC_REG_CTLR, 0);

    uint32_t reg = enable ? GICD_REG_ISENABLER1 : GICD_REG_ICENABLER1;
    /* TODO: Edge/Level sensivity */
    if ((irq > 31) && (irq < 64)) {
        reg = enable ? GICD_REG_ISENABLER2 : GICD_REG_ICENABLER2;
    } else if ((irq > 63) && (irq < 96)) {
        reg = enable ? GICD_REG_ISENABLER3 : GICD_REG_ICENABLER3;
    }

    gic_dist_write(reg, 1 << irq);

    gic_cpuif_write(GICC_REG_PMR, 0xff);
    gic_cpuif_write(GICC_REG_CTLR, 1);
    gic_dist_write(GICD_REG_CTLR, 1);
}

#if 0
typedef struct{
  u32 SGI_Type;   // Bit Mask, Each Bit: 1 - FIQ, 0 - IRQ
  u64 SPI_Type;   // Bit Mask, Each Bit: 1 - FIQ, 0 - IRQ
  u64 SPI_Enable; // Bit Mask, Each Bit: 1 - Enable, 0 - Disable
} T_gic_configuration;

void gic_config(const T_gic_configuration gic_configuration)
{
  rgGICD_CTLR = 0;
  rgGICC_CTLR = 0;
  tubef_hex(rgGICD_TYPER);
  rgGICD_ICFGR2 = rgGICD_ICFGR2 | 0x00008000;  // WKUPINT is Edge-Sensitive
  rgGICD_IGROUPR0  = 0u;  //  ~       (gic_configuration .SGI_Type);   // FIQs Not Implemented
  rgGICD_IGROUPR1  = 0u;  //  ~((u32) (gic_configuration .SPI_Type));
  rgGICD_IGROUPR2  = 0u;  //  ~((u32) (gic_configuration .SPI_Type >> 32));
  rgGICD_ISENABLER1=   (u32) (gic_configuration .SPI_Enable);
  rgGICD_ISENABLER2=   (u32) (gic_configuration .SPI_Enable >> 32);
  rgGICC_PMR = 0xFF;
  rgGICC_CTLR = 1;
  rgGICD_CTLR = 1;
  tubef_str("gic_config end");
  tubef_hex(rgGICC_CTLR);
  tubef_hex(rgGICD_CTLR);
  tubef_hex(rgGICD_IGROUPR0);
}

T_gic_configuration gic_configuration =
{
    .SGI_Type  =0u,       // All SGIs Of IRQ Type
    .SPI_Type  =0ul,      // All SPIs Of IRQ Type
    .SPI_Enable=NOGICSPI  // All SPIs Disabled // Use Special Constants Defined In defs.h
};

int main(void)
{
  tubef_str("Hello GIC!");
    // Config GIC (Try Software IRQs)
  gic_config(gic_configuration);
    // Generate 8 SGIs One-By-One
  rgGICD_SGIR = GENSWINT0; // GENSWINT0 - GENSWINT7 - Are Special Constants Defined In defs.h
  rgGICD_SGIR = GENSWINT1;
  rgGICD_SGIR = GENSWINT2;
  rgGICD_SGIR = GENSWINT3;
  rgGICD_SGIR = GENSWINT4;
  rgGICD_SGIR = GENSWINT5;
  rgGICD_SGIR = GENSWINT6;
  rgGICD_SGIR = GENSWINT7;
  while (
      sync_cell_irq[0]+sync_cell_irq[1]+sync_cell_irq[2]+sync_cell_irq[3]+
      sync_cell_irq[4]+sync_cell_irq[5]+sync_cell_irq[6]+sync_cell_irq[7] < 8
  ){};

  tubef_str  ("Once more");
  sync_cell_irq[0]=0;
  sync_cell_irq[1]=0;
  sync_cell_irq[2]=0;
  sync_cell_irq[3]=0;
  sync_cell_irq[4]=0;
  sync_cell_irq[5]=0;
  sync_cell_irq[6]=0;
  sync_cell_irq[7]=0;

  rgGICD_SGIR = GENSWINT7;
  rgGICD_SGIR = GENSWINT6;
  rgGICD_SGIR = GENSWINT5;
  rgGICD_SGIR = GENSWINT4;
  rgGICD_SGIR = GENSWINT3;
  rgGICD_SGIR = GENSWINT2;
  rgGICD_SGIR = GENSWINT1;
  rgGICD_SGIR = GENSWINT0;
  while (
    sync_cell_irq[0]+sync_cell_irq[1]+sync_cell_irq[2]+sync_cell_irq[3]+
    sync_cell_irq[4]+sync_cell_irq[5]+sync_cell_irq[6]+sync_cell_irq[7] < 8
  ){};
  //========================================================================================
/*  tubef_str  ("Hello FIQ");
  gic_configuration.SGI_Type = 0xFF;
  gic_config(gic_configuration);


  rgGICD_SGIR = GENSWINT0; // GENSWINT0 - GENSWINT7 - Are Special Constants Defined In defs.h
  rgGICD_SGIR = GENSWINT1;
  rgGICD_SGIR = GENSWINT2;
  rgGICD_SGIR = GENSWINT3;
  rgGICD_SGIR = GENSWINT4;
  rgGICD_SGIR = GENSWINT5;
  rgGICD_SGIR = GENSWINT6;
  rgGICD_SGIR = GENSWINT7;
  while (
      sync_cell_fiq[0]+sync_cell_fiq[1]+sync_cell_fiq[2]+sync_cell_fiq[3]+
      sync_cell_fiq[4]+sync_cell_fiq[5]+sync_cell_fiq[6]+sync_cell_fiq[7] < 8
  ){};

  tubef_str  ("Once more");
  sync_cell_fiq[0]=0;
  sync_cell_fiq[1]=0;
  sync_cell_fiq[2]=0;
  sync_cell_fiq[3]=0;
  sync_cell_fiq[4]=0;
  sync_cell_fiq[5]=0;
  sync_cell_fiq[6]=0;
  sync_cell_fiq[7]=0;

  rgGICD_SGIR = GENSWINT0;
  rgGICD_SGIR = GENSWINT1;
  rgGICD_SGIR = GENSWINT2;
  rgGICD_SGIR = GENSWINT3;
  rgGICD_SGIR = GENSWINT4;
  rgGICD_SGIR = GENSWINT5;
  rgGICD_SGIR = GENSWINT6;
  rgGICD_SGIR = GENSWINT7;
  while (
    sync_cell_fiq[0]+sync_cell_fiq[1]+sync_cell_fiq[2]+sync_cell_fiq[3]+
    sync_cell_fiq[4]+sync_cell_fiq[5]+sync_cell_fiq[6]+sync_cell_fiq[7] < 8
  ){};*/
  tubef_str  ("OK GIC");
//   tubef_die();
  return -1;
}


#endif
