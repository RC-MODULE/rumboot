#ifndef REGS_MKIO_H
#define REGS_MKIO_H

#include <platform/common_macros/common_macros.h>

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

BEGIN_ENUM( MKIO_IRQE_FIELD )
    DECLARE_ENUM_VAL( MKIO_IRQE_BCEVE_i,      0 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BCEVE_n,      1 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BCEVE_mask,    FIELD_MASK32( MKIO_IRQE_BCEVE_i, MKIO_IRQE_BCEVE_n ) )

    DECLARE_ENUM_VAL( MKIO_IRQE_BCDE_i,       1 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BCDE_n,       1 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BCDE_mask,    FIELD_MASK32( MKIO_IRQE_BCDE_i, MKIO_IRQE_BCDE_n ) )

    DECLARE_ENUM_VAL( MKIO_IRQE_BCWKE_i,      2 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BCWKE_n,      1 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BCWKE_mask,    FIELD_MASK32( MKIO_IRQE_BCWKE_i, MKIO_IRQE_BCWKE_n ) )

    //7:3 RESERVED

    DECLARE_ENUM_VAL( MKIO_IRQE_RTEVE_i,      8 )
    DECLARE_ENUM_VAL( MKIO_IRQE_RTEVE_n,      1 )
    DECLARE_ENUM_VAL( MKIO_IRQE_RTEVE_mask,    FIELD_MASK32( MKIO_IRQE_RTEVE_i, MKIO_IRQE_RTEVE_n ) )

    DECLARE_ENUM_VAL( MKIO_IRQE_RTDE_i,       9 )
    DECLARE_ENUM_VAL( MKIO_IRQE_RTDE_n,       1 )
    DECLARE_ENUM_VAL( MKIO_IRQE_RTDE_mask,    FIELD_MASK32( MKIO_IRQE_RTDE_i, MKIO_IRQE_RTDE_n ) )

    DECLARE_ENUM_VAL( MKIO_IRQE_RTEE_i,      10 )
    DECLARE_ENUM_VAL( MKIO_IRQE_RTEE_n,       1 )
    DECLARE_ENUM_VAL( MKIO_IRQE_RTEE_mask,    FIELD_MASK32( MKIO_IRQE_RTEE_i, MKIO_IRQE_RTEE_n ) )

    //15:11 RESERVED

    DECLARE_ENUM_VAL( MKIO_IRQE_BMDE_i,      16 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BMDE_n,       1 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BMDE_mask,    FIELD_MASK32( MKIO_IRQE_BMDE_i, MKIO_IRQE_BMDE_n ) )

    DECLARE_ENUM_VAL( MKIO_IRQE_BMTOE_i,     17 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BMTOE_n,      1 )
    DECLARE_ENUM_VAL( MKIO_IRQE_BMTOE_mask,    FIELD_MASK32( MKIO_IRQE_BMTOE_i, MKIO_IRQE_BMTOE_n ) )

    //31:18 RESERVED

END_ENUM( MKIO_IRQE_FIELD )

BEGIN_ENUM( MKIO_BCSL_FIELD )
    DECLARE_ENUM_VAL( MKIO_BCSL_SCST_i,      0 )
    DECLARE_ENUM_VAL( MKIO_BCSL_SCST_n,      3 )
    DECLARE_ENUM_VAL( MKIO_BCSL_SCST_mask,   FIELD_MASK32( MKIO_BCSL_SCST_i, MKIO_BCSL_SCST_n ) )

    DECLARE_ENUM_VAL( MKIO_BCSL_SCADL_i,      3 )
    DECLARE_ENUM_VAL( MKIO_BCSL_SCADL_n,      3 )
    DECLARE_ENUM_VAL( MKIO_BCSL_SCADL_mask,   FIELD_MASK32( MKIO_BCSL_SCADL_i, MKIO_BCSL_SCADL_n ) )

    DECLARE_ENUM_VAL( MKIO_BCSL_ASST_i,      8 )
    DECLARE_ENUM_VAL( MKIO_BCSL_ASST_n,      2 )
    DECLARE_ENUM_VAL( MKIO_BCSL_ASST_mask,   FIELD_MASK32( MKIO_BCSL_ASST_i, MKIO_BCSL_ASST_n ) )

    //10 RESERVED

    DECLARE_ENUM_VAL( MKIO_BCSL_ASADL_i,      11 )
    DECLARE_ENUM_VAL( MKIO_BCSL_ASADL_n,      5 )
    DECLARE_ENUM_VAL( MKIO_BCSL_ASADL_mask,   FIELD_MASK32( MKIO_BCSL_ASADL_i, MKIO_BCSL_ASADL_n ) )

    DECLARE_ENUM_VAL( MKIO_BCSL_BCCHK_i,      16 )
    DECLARE_ENUM_VAL( MKIO_BCSL_BCCHK_n,      1 )
    DECLARE_ENUM_VAL( MKIO_BCSL_BCCHK_mask,   FIELD_MASK32( MKIO_BCSL_BCCHK_i, MKIO_BCSL_BCCHK_n ) )

    //27:17 RESERVED

    DECLARE_ENUM_VAL( MKIO_BCSL_BCFEAT_i,      28 )
    DECLARE_ENUM_VAL( MKIO_BCSL_BCFEAT_n,      3 )
    DECLARE_ENUM_VAL( MKIO_BCSL_BCFEAT_mask,   FIELD_MASK32( MKIO_BCSL_BCFEAT_i, MKIO_BCSL_BCFEAT_n ) )

    DECLARE_ENUM_VAL( MKIO_BCSL_BCSUP_i,      31 )
    DECLARE_ENUM_VAL( MKIO_BCSL_BCSUP_n,      1 )
    DECLARE_ENUM_VAL( MKIO_BCSL_BCSUP_mask,   FIELD_MASK32( MKIO_BCSL_BCSUP_i, MKIO_BCSL_BCSUP_n ) )
END_ENUM( MKIO_BCSL_FIELD )


#endif /* end of include guard: REGS_MKIO_H */
