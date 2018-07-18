#ifndef REGS_MKIO_H
#define REGS_MKIO_H

/**
 * \defgroup regs_mkio Gaisler MKIO (1553b)
 * \ingroup regs
 *
 * Register offsets and constants
 * \code{.c}
 * #include <regs/regs_mkio.h>
 * \endcode
 *
 * @{
 */
#define IRQ               0x000
#define IRQE              0x004
#define HWCFG             0x010

#define BCSL              0x040
#define BCA               0x044
#define BCTNP             0x048
#define BCANP             0x04C
#define BCT               0x050
#define BCTW              0x054
#define BCRD              0x058
#define BCBS              0x05C
#define BCTCP             0x068
#define BCACP             0x06C

#define RTS               0x080
#define RTC               0x084
#define RTBS              0x088
#define RTSW              0x08C
#define RTSY              0x090
#define RTSTBA            0x094
#define RTMCC             0x098
#define RTTTC             0x0A4
#define RTELM             0x0AC
#define RTELP             0x0B0
#define RTELIP            0x0B4

#define BMS               0x0C0
#define BMC               0x0C4
#define BMRTAF            0x0C8
#define BMRTSF            0x0CC
#define BMRTMC            0x0D0
#define BMLBS             0x0D4
#define BMLBE             0x0D8
#define BMLBP             0x0DC
#define BMTTC             0x0E0
/**
 * @}
 */

#define POLARITY          0x100

#endif /* end of include guard: REGS_MKIO_H */
