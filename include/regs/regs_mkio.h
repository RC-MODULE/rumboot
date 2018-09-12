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

#define POLARITY                0x100
#define SYNC_CTRL_ADDR          0x104
#define VALIDCMD_FIRST_L_ADDR   0x108
#define VALIDCMD_FIRST_H_ADDR   0x10C
#define VALIDCMD_LAST_L_ADDR    0x110
#define VALIDCMD_LAST_H_ADDR    0x114
#define RTSYNC_LAST_L_ADDR      0x118
#define RTSYNC_LAST_H_ADDR      0x11C

#define MKIO_IRQE_BCEVE_i      ( 0 )
#define MKIO_IRQE_BCEVE_n      ( 1 )
#define MKIO_IRQE_BCEVE_mask   (1 << MKIO_IRQE_BCEVE_i)

#define MKIO_IRQE_BCDE_i       ( 1 )
#define MKIO_IRQE_BCDE_n       ( 1 )
#define MKIO_IRQE_BCDE_mask    (1 << MKIO_IRQE_BCDE_i)

#define MKIO_IRQE_BCWKE_i      ( 2 )
#define MKIO_IRQE_BCWKE_n      ( 1 )
#define MKIO_IRQE_BCWKE_mask   (1 << MKIO_IRQE_BCWKE_i)

//7:3 RESERVED

#define MKIO_IRQE_RTEVE_i      ( 8 )
#define MKIO_IRQE_RTEVE_n      ( 1 )
#define MKIO_IRQE_RTEVE_mask   ( 1 << MKIO_IRQE_RTEVE_i)

#define MKIO_IRQE_RTDE_i      ( 9 )
#define MKIO_IRQE_RTDE_n      ( 1 )
#define MKIO_IRQE_RTDE_mask   ( 1 << MKIO_IRQE_RTDE_i)

#define MKIO_IRQE_RTEE_i     ( 10 )
#define MKIO_IRQE_RTEE_n     (  1 )
#define MKIO_IRQE_RTEE_mask  ( 1 << MKIO_IRQE_RTEE_i )

//15:11 RESERVED

#define MKIO_IRQE_BMDE_i     ( 16 )
#define MKIO_IRQE_BMDE_n     (  1 )
#define MKIO_IRQE_BMDE_mask  ( 1 << MKIO_IRQE_BMDE_i )

#define MKIO_IRQE_BMTOE_i    ( 17 )
#define MKIO_IRQE_BMTOE_n    (  1 )
#define MKIO_IRQE_BMTOE_mask ( 1 << MKIO_IRQE_BMTOE_i )

    //31:18 RESERVED

#define MKIO_BCSL_SCST_i      ( 0 )
#define MKIO_BCSL_SCST_n      ( 3 )
#define MKIO_BCSL_SCST_mask   (0x7 << MKIO_BCSL_SCST_i)

#define MKIO_BCSL_SCADL_i      ( 3 )
#define MKIO_BCSL_SCADL_n      ( 3 )
#define MKIO_BCSL_SCADL_mask   (0x7 << MKIO_BCSL_SCADL_i)

#define MKIO_BCSL_ASST_i       ( 8 )
#define MKIO_BCSL_ASST_n       ( 2 )
#define MKIO_BCSL_ASST_mask    (0x3 << MKIO_BCSL_ASST_i)

    //10 RESERVED

#define MKIO_BCSL_ASADL_i      ( 11 )
#define MKIO_BCSL_ASADL_n      (  5 )
#define MKIO_BCSL_ASADL_mask   ( 0x1F << MKIO_BCSL_ASADL_i)

#define MKIO_BCSL_BCCHK_i      ( 16 )
#define MKIO_BCSL_BCCHK_n      ( 1 )
#define MKIO_BCSL_BCCHK_mask   ( 1 << MKIO_BCSL_BCCHK_i )

    //27:17 RESERVED

#define MKIO_BCSL_BCFEAT_i      ( 28 )
#define MKIO_BCSL_BCFEAT_n      (  3 )
#define MKIO_BCSL_BCFEAT_mask   ( 0x7 << MKIO_BCSL_BCFEAT_i)

#define MKIO_BCSL_BCSUP_i      ( 31 )
#define MKIO_BCSL_BCSUP_n      (  1 )
#define MKIO_BCSL_BCSUP_mask   ( 1 << MKIO_BCSL_BCSUP_i)

#endif /* end of include guard: REGS_MKIO_H */
