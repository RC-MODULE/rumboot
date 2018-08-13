
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/trace.S.h>
#include <platform/test_assert.S.h>
#include <platform/test_event_asm.h>
#include <platform/devices.h>
#include <platform/regs/regs_l2c_l2.h>

/* Registers aliases */
#define R_ARG0                  r3
#define R_ARG1                  r4
#define R_RETV                  R_ARG0
#define R_DCRA                  r20     /* DCR Address channel   */
#define R_DCRD                  r21     /* DCR Data channel      */
#define R_ADDR                  r22     /* Address of I/O Reg    */
#define R_DATA                  r23     /* Readed data for check */
#define R_EXPV                  r24     /* Expected value        */
#define R_SHFT                  r25     /* Shift value           */
#define R_TERM                  r26     /* Termination template  */
#define R_MASK                  r27     /* Register mask         */
#define R_BACK                  r28     /* Common backup         */
#define R_ONES                  r31     /* Holds 0xFFFFFFFF      */
#define R_ZERO                  r30     /* Holds 0x00000000      */


#define L2CA(OFFSET)			(DCR_L2C_BASE + (OFFSET))

#define I_M_SELF_ID				0x00
#define L2C_DCRAI_VAL			0x00000000
/* from trunk/cpu/units/ppc476l2c_v001/verilog/rtl/PPC476L2C.parameters.v */
#define L2C_VERSION_NUMBER		0x001
#define L2C_REVISION_ID			0x02
/* from dut */
#define L2C_L2SIZE              0x0
#define L2C_PLB_CLK_RATIO       0x0
#define L2C_TSNOOP              0x0
#define L2C_MASTER_ID           I_M_SELF_ID


/*	1 - initialization complete */
#define L2C_L2ISTAT_VAL			0x00000001
/*	see trunk/cpu/units/ppc476l2c_v001/verilog/rtl/PPC476L2C_DCR_LFSR.v.
	Depended I_M_SELF_ID strap. */
#define L2C_L2PNCR_VAL			0x00000069
#define L2C_L2REVID_VAL			reg_field(31,L2C_REVISION_ID)    |\
								reg_field(23,L2C_VERSION_NUMBER)
#define L2C_L2CNFG0_VAL     	reg_field(31,L2C_L2SIZE)         |\
								reg_field(27,L2C_PLB_CLK_RATIO)  |\
								reg_field(23,L2C_TSNOOP)         |\
								reg_field(19,L2C_MASTER_ID)
#define L2C_L2CNFG1_VAL			reg_field(26,0x1)
#define L2C_L2DBGSEL_VAL		0x00000000
#define L2C_L2DBGDATA0_VAL		0x00000000
#define L2C_L2DBGDATA1_VAL		0x00000000
#define L2C_L2SLEEPSTAT_VAL		0x00000000
#define L2C_L2SLEEPREQ_VAL		0x00000000
#define L2C_L2MCK_VAL			0x00000000
#define L2C_L2MCKEN_VAL			reg_field(30,0x7FF)
#define L2C_L2FERR_VAL			0x00000000
#define L2C_L2INT_VAL			0x00000000
#define L2C_L2INTEN_VAL			reg_field(30,0x3FF)
#define L2C_L2LOG0_VAL			0x00000000
#define L2C_L2LOG1_VAL			0x00000000
#define L2C_L2LOG2_VAL			0x00000000
#define L2C_L2LOG3_VAL			0x00000000
#define L2C_L2LOG4_VAL			0x00000000
#define L2C_L2LOG5_VAL			0x00000000
#define L2C_L2PLBCFG_VAL		0x00000000
#define L2C_L2PLBDBG_VAL		0x00000000
#define L2C_L2PLBERAP_VAL		0x00000000
#define L2C_L2PLBSTAT0_VAL		0x00000000
#define L2C_L2PLBSTAT1_VAL		0x00000000
#define L2C_L2PLBFRC0_VAL		0x00000000
#define L2C_L2PLBFRC1_VAL		0x00000000
#define L2C_L2PLBMCKEN0_VAL		0xFFFFFFFF
#define L2C_L2PLBMCKEN1_VAL		0xFFFFFFFF
#define L2C_L2PLBFERR0_VAL		0x00000000
#define L2C_L2PLBFERR1_VAL		0x00000000
#define L2C_L2PLBINTEN0_VAL		0x00000000
#define L2C_L2PLBINTEN1_VAL		0x00000000
#define L2C_L2ARRCFG_VAL		0x00000000
#define L2C_L2ARRDBG0_VAL		0x00000000
#define L2C_L2ARRDBG1_VAL		0x00000000
#define L2C_L2ARRDBG2_VAL		0x00000000
#define L2C_L2ARRDBG3_VAL		0x00000000
#define L2C_L2ARRACCCTL_VAL		0x00000000
#define L2C_L2ARRACCADR_VAL		0x00000000
#define L2C_L2ARRACCDI0_VAL		0x00000000
#define L2C_L2ARRACCDI1_VAL		0x00000000
#define L2C_L2ARRACCDI2_VAL		0x00000000
#define L2C_L2ARRACCDO0_VAL		0x00000000
#define L2C_L2ARRACCDO1_VAL		0x00000000
#define L2C_L2ARRACCDO2_VAL		0x00000000
#define L2C_L2ARRSTAT0_VAL		0x00000000
#define L2C_L2ARRSTAT1_VAL		0x00000000
#define L2C_L2ARRSTAT2_VAL		0x00000000
#define L2C_L2ARRFRC0_VAL		0x00000000
#define L2C_L2ARRFRC1_VAL		0x00000000
#define L2C_L2ARRFRC2_VAL		0x00000000
#define L2C_L2ARRMCKEN0_VAL		0xFFFFFFFF
#define L2C_L2ARRMCKEN1_VAL		0x00001FFF
#define L2C_L2ARRMCKEN2_VAL		0x00000000
#define L2C_L2ARRFERR0_VAL		0x00000000
#define L2C_L2ARRFERR1_VAL		0x00000000
#define L2C_L2ARRFERR2_VAL		0x00000000
#define L2C_L2ARRINTEN0_VAL		0x00000000
#define L2C_L2ARRINTEN1_VAL		0x00000000
#define L2C_L2ARRINTEN2_VAL		0x00FFFFFF
#define L2C_L2CPUCFG_VAL		0x00000000
#define L2C_L2CPUDBG_VAL		0x00000000
#define L2C_L2CPUSTAT_VAL		0x00000000
#define L2C_L2CPUFRC_VAL		0x00000000
#define L2C_L2CPUMCKEN_VAL		0x00000FFF
#define L2C_L2CPUFERR_VAL		0x00000000
#define L2C_L2CPUINTEN_VAL		0x00000000
#define L2C_L2RACCFG_VAL		0x00000000
#define L2C_L2RACDBG0_VAL		0x00000000
#define L2C_L2RACDBG1_VAL		0x00000000
#define L2C_L2RACSTAT0_VAL		0x00000000
#define L2C_L2RACFRC0_VAL		0x00000000
#define L2C_L2RACMCKEN0_VAL		0x03FFFFFF
#define L2C_L2RACFERR0_VAL		0x00000000
#define L2C_L2RACINTEN0_VAL		0x00000000
#define L2C_L2WACCFG_VAL		0x00000000
#define L2C_L2WACDBG0_VAL		0x00000000
#define L2C_L2WACDBG1_VAL		0x00000000
#define L2C_L2WACDBG2_VAL		0x00000000
#define L2C_L2WACSTAT0_VAL		0x00000000
#define L2C_L2WACSTAT1_VAL		0x00000000
#define L2C_L2WACSTAT2_VAL		0x00000000
#define L2C_L2WACFRC0_VAL		0x00000000
#define L2C_L2WACFRC1_VAL		0x00000000
#define L2C_L2WACFRC2_VAL		0x00000000
#define L2C_L2WACMCKEN0_VAL		0x00FFFFFF
#define L2C_L2WACMCKEN1_VAL		0x00FFFFFF
#define L2C_L2WACMCKEN2_VAL		0x00FFFFFF
#define L2C_L2WACFERR0_VAL		0x00000000
#define L2C_L2WACFERR1_VAL		0x00000000
#define L2C_L2WACFERR2_VAL		0x00000000
#define L2C_L2WACINTEN0_VAL		0x00000000
#define L2C_L2WACINTEN1_VAL		0x00000000
#define L2C_L2WACINTEN2_VAL		0x00000000

/* Default masks */
#define DEFAULT_MASK            0xFFFFFFFF
#define LO4_MASK                0x0000000F
#define LO8_MASK                0x000000FF
#define LO12_MASK               0x00000FFF
#define LO16_MASK               0x0000FFFF
#define LO20_MASK               0x000FFFFF
#define LO24_MASK               0x00FFFFFF
#define LO28_MASK               0x0FFFFFFF
#define HI4_MASK                0xF0000000
#define HI8_MASK                0xFF000000
#define HI12_MASK               0xFFF00000
#define HI16_MASK               0xFFFF0000
#define HI20_MASK               0xFFFFF000
#define HI24_MASK               0xFFFFFF00
#define HI28_MASK               0xFFFFFFF0

/* Regs masks */
#define L2C_L2ISTAT_MASK        0x00000001
#define L2C_L2PNCR_MASK         LO8_MASK
#define L2C_L2REVID_MASK        LO20_MASK
#define L2C_L2CNFG0_MASK        0x0001F733
#define L2C_L2CNFG1_MASK        0x00000037
#define L2C_L2DBGSEL_MASK       DEFAULT_MASK
#define L2C_L2DBGDATA0_MASK     DEFAULT_MASK
#define L2C_L2DBGDATA1_MASK     DEFAULT_MASK
#define L2C_L2SLEEPSTAT_MASK    DEFAULT_MASK
#define L2C_L2SLEEPREQ_MASK     DEFAULT_MASK
#define L2C_L2MCK_MASK          0x00000FFE
#define L2C_L2MCKEN_MASK        L2C_L2MCK_MASK
#define L2C_L2FERR_MASK         L2C_L2MCK_MASK
#define L2C_L2INT_MASK          L2C_L2MCK_MASK
#define L2C_L2INTEN_MASK        L2C_L2MCK_MASK
#define L2C_L2LOG0_MASK         0x00007FFF
#define L2C_L2LOG1_MASK         0x00FF00FF
#define L2C_L2LOG2_MASK         DEFAULT_MASK
#define L2C_L2LOG3_MASK         DEFAULT_MASK
#define L2C_L2LOG4_MASK         DEFAULT_MASK
#define L2C_L2LOG5_MASK         DEFAULT_MASK
#define L2C_L2PLBCFG_MASK       LO4_MASK
#define L2C_L2PLBDBG_MASK       DEFAULT_MASK
#define L2C_L2PLBERAP_MASK      0xFFFFFC00
#define L2C_L2PLBSTAT0_MASK     DEFAULT_MASK
#define L2C_L2PLBSTAT1_MASK     DEFAULT_MASK
#define L2C_L2PLBFRC0_MASK      DEFAULT_MASK
#define L2C_L2PLBFRC1_MASK      DEFAULT_MASK
#define L2C_L2PLBMCKEN0_MASK    DEFAULT_MASK
#define L2C_L2PLBMCKEN1_MASK    DEFAULT_MASK
#define L2C_L2PLBFERR0_MASK     DEFAULT_MASK
#define L2C_L2PLBFERR1_MASK     DEFAULT_MASK
#define L2C_L2PLBINTEN0_MASK    DEFAULT_MASK
#define L2C_L2PLBINTEN1_MASK    DEFAULT_MASK
#define L2C_L2ARRCFG_MASK       0x00000731
#define L2C_L2ARRDBG0_MASK      DEFAULT_MASK
#define L2C_L2ARRDBG1_MASK      DEFAULT_MASK
#define L2C_L2ARRDBG2_MASK      DEFAULT_MASK
#define L2C_L2ARRDBG3_MASK      DEFAULT_MASK
#define L2C_L2ARRACCCTL_MASK    0x0077F3FF
#define L2C_L2ARRACCADR_MASK    0x00001FFF
#define L2C_L2ARRACCDI0_MASK    DEFAULT_MASK
#define L2C_L2ARRACCDI1_MASK    DEFAULT_MASK
#define L2C_L2ARRACCDI2_MASK    LO8_MASK
#define L2C_L2ARRACCDO0_MASK    DEFAULT_MASK
#define L2C_L2ARRACCDO1_MASK    DEFAULT_MASK
#define L2C_L2ARRACCDO2_MASK    L2C_L2ARRACCDI2_MASK
#define L2C_L2ARRSTAT0_MASK     DEFAULT_MASK
#define L2C_L2ARRSTAT1_MASK     0x00001FFF
#define L2C_L2ARRSTAT2_MASK     LO24_MASK
#define L2C_L2ARRFRC0_MASK      L2C_L2ARRSTAT0_MASK
#define L2C_L2ARRFRC1_MASK      L2C_L2ARRSTAT1_MASK
#define L2C_L2ARRFRC2_MASK      L2C_L2ARRSTAT2_MASK
#define L2C_L2ARRMCKEN0_MASK    L2C_L2ARRSTAT0_MASK
#define L2C_L2ARRMCKEN1_MASK    L2C_L2ARRSTAT1_MASK
#define L2C_L2ARRMCKEN2_MASK    L2C_L2ARRSTAT2_MASK
#define L2C_L2ARRFERR0_MASK     L2C_L2ARRSTAT0_MASK
#define L2C_L2ARRFERR1_MASK     L2C_L2ARRSTAT1_MASK
#define L2C_L2ARRFERR2_MASK     L2C_L2ARRSTAT2_MASK
#define L2C_L2ARRINTEN0_MASK    L2C_L2ARRSTAT0_MASK
#define L2C_L2ARRINTEN1_MASK    L2C_L2ARRSTAT1_MASK
#define L2C_L2ARRINTEN2_MASK    L2C_L2ARRSTAT2_MASK
#define L2C_L2CPUCFG_MASK       0x00000007
#define L2C_L2CPUDBG_MASK       DEFAULT_MASK
#define L2C_L2CPUSTAT_MASK      LO12_MASK
#define L2C_L2CPUFRC_MASK       LO12_MASK
#define L2C_L2CPUMCKEN_MASK     LO12_MASK
#define L2C_L2CPUFERR_MASK      LO12_MASK
#define L2C_L2CPUINTEN_MASK     LO12_MASK
#define L2C_L2RACCFG_MASK       0x00000073
#define L2C_L2RACDBG0_MASK      DEFAULT_MASK
#define L2C_L2RACDBG1_MASK      DEFAULT_MASK
#define L2C_L2RACSTAT0_MASK     0x03FFFFFF
#define L2C_L2RACFRC0_MASK      L2C_L2RACSTAT0_MASK
#define L2C_L2RACMCKEN0_MASK    L2C_L2RACSTAT0_MASK
#define L2C_L2RACFERR0_MASK     L2C_L2RACSTAT0_MASK
#define L2C_L2RACINTEN0_MASK    L2C_L2RACSTAT0_MASK
#define L2C_L2WACCFG_MASK       0x000331FF
#define L2C_L2WACDBG0_MASK      DEFAULT_MASK
#define L2C_L2WACDBG1_MASK      DEFAULT_MASK
#define L2C_L2WACDBG2_MASK      DEFAULT_MASK
#define L2C_L2WACSTAT0_MASK     LO24_MASK
#define L2C_L2WACSTAT1_MASK     LO24_MASK
#define L2C_L2WACSTAT2_MASK     LO24_MASK
#define L2C_L2WACFRC0_MASK      LO24_MASK
#define L2C_L2WACFRC1_MASK      LO24_MASK
#define L2C_L2WACFRC2_MASK      LO24_MASK
#define L2C_L2WACMCKEN0_MASK    LO24_MASK
#define L2C_L2WACMCKEN1_MASK    LO24_MASK
#define L2C_L2WACMCKEN2_MASK    LO24_MASK
#define L2C_L2WACFERR0_MASK     LO24_MASK
#define L2C_L2WACFERR1_MASK     LO24_MASK
#define L2C_L2WACFERR2_MASK     LO24_MASK
#define L2C_L2WACINTEN0_MASK    LO24_MASK
#define L2C_L2WACINTEN1_MASK    LO24_MASK
#define L2C_L2WACINTEN2_MASK    LO24_MASK


.macro check_value_l2c_init
    load_const  R_DCRA,  L2CA(L2C_L2CDCRAI)
    load_const  R_DCRD,  L2CA(L2C_L2CDCRDI)
.endm

.macro check_value_l2c vAddr, vExp, vMask, sName
	rumboot_putstring "Checking \sName ... "
    load_const  R_ADDR, \vAddr
    load_const  R_EXPV, \vExp
    .if \vMask != DEFAULT_MASK
        load_const      R_MASK, \vMask
    .endif
    mtdcrx      R_DCRA, R_ADDR
    mfdcrx      R_DATA, R_DCRD
    .if \vMask != DEFAULT_MASK
        and     R_DATA, R_DATA, R_MASK
    .endif
    cmpw cr7,   R_DATA, R_EXPV
    TEST_ASSERT(eq, cr7, "\nDCR check error: In \sName was expected value \vExp (address \vAddr) ")
    beq+ cr7,   61f
    b           62f
61:
    rumboot_putstring   "done.\n"
62:
.endm


.section ".text","ax",@progbits

.global main

main:
    set_cr      cr6,    eq
    load_const  R_ZERO, 0x00000000
    not         R_ONES, R_ONES
    mr          R_RETV, R_ZERO

check_cpu_ppc_l2c:
    check_value_l2c_init
    check_value_l2c L2C_L2ISTAT,        L2C_L2ISTAT_VAL,        DEFAULT_MASK, "L2C_L2ISTAT"
    check_value_l2c L2C_L2PNCR,         L2C_L2PNCR_VAL,         DEFAULT_MASK, "L2C_L2PNCR"
    check_value_l2c L2C_L2REVID,        L2C_L2REVID_VAL,        DEFAULT_MASK, "L2C_L2REVID"
    check_value_l2c L2C_L2CNFG0,        L2C_L2CNFG0_VAL,        DEFAULT_MASK, "L2C_L2CNFG0"
    check_value_l2c L2C_L2CNFG1,        L2C_L2CNFG1_VAL,        DEFAULT_MASK, "L2C_L2CNFG1"
    check_value_l2c L2C_L2DBGSEL,       L2C_L2DBGSEL_VAL,       DEFAULT_MASK, "L2C_L2DBGSEL"
    check_value_l2c L2C_L2DBGDATA0,     L2C_L2DBGDATA0_VAL,     DEFAULT_MASK, "L2C_L2DBGDATA0"
    check_value_l2c L2C_L2DBGDATA1,     L2C_L2DBGDATA1_VAL,     DEFAULT_MASK, "L2C_L2DBGDATA1"
    check_value_l2c L2C_L2SLEEPSTAT,    L2C_L2SLEEPSTAT_VAL,    DEFAULT_MASK, "L2C_L2SLEEPSTAT"
    check_value_l2c L2C_L2SLEEPREQ,     L2C_L2SLEEPREQ_VAL,     DEFAULT_MASK, "L2C_L2SLEEPREQ"
    check_value_l2c L2C_L2MCK,          L2C_L2MCK_VAL,          DEFAULT_MASK, "L2C_L2MCK"
    check_value_l2c L2C_L2MCKEN,        L2C_L2MCKEN_VAL,        DEFAULT_MASK, "L2C_L2MCKEN"
    check_value_l2c L2C_L2FERR,         L2C_L2FERR_VAL,         DEFAULT_MASK, "L2C_L2FERR"
    check_value_l2c L2C_L2INT,          L2C_L2INT_VAL,          DEFAULT_MASK, "L2C_L2INT"
    check_value_l2c L2C_L2INTEN,        L2C_L2INTEN_VAL,        DEFAULT_MASK, "L2C_L2INTEN"
    check_value_l2c L2C_L2LOG0,         L2C_L2LOG0_VAL,         DEFAULT_MASK, "L2C_L2LOG0"
    check_value_l2c L2C_L2LOG1,         L2C_L2LOG1_VAL,         DEFAULT_MASK, "L2C_L2LOG1"
    check_value_l2c L2C_L2LOG2,         L2C_L2LOG2_VAL,         DEFAULT_MASK, "L2C_L2LOG2"
    check_value_l2c L2C_L2LOG3,         L2C_L2LOG3_VAL,         DEFAULT_MASK, "L2C_L2LOG3"
    check_value_l2c L2C_L2LOG4,         L2C_L2LOG4_VAL,         DEFAULT_MASK, "L2C_L2LOG4"
    check_value_l2c L2C_L2LOG5,         L2C_L2LOG5_VAL,         DEFAULT_MASK, "L2C_L2LOG5"
    check_value_l2c L2C_L2PLBCFG,       L2C_L2PLBCFG_VAL,       DEFAULT_MASK, "L2C_L2PLBCFG"
    check_value_l2c L2C_L2PLBDBG,       L2C_L2PLBDBG_VAL,       DEFAULT_MASK, "L2C_L2PLBDBG"
    check_value_l2c L2C_L2PLBERAP,      L2C_L2PLBERAP_VAL,      DEFAULT_MASK, "L2C_L2PLBERAP"
    check_value_l2c L2C_L2PLBSTAT0,     L2C_L2PLBSTAT0_VAL,     DEFAULT_MASK, "L2C_L2PLBSTAT0"
    check_value_l2c L2C_L2PLBSTAT1,     L2C_L2PLBSTAT1_VAL,     DEFAULT_MASK, "L2C_L2PLBSTAT1"
    /* L2C_L2PLBFRC0, L2C_L2PLBFRC1 are W/O regs */
/*  check_value_l2c L2C_L2PLBFRC0,      L2C_L2PLBFRC0_VAL,      DEFAULT_MASK, "L2C_L2PLBFRC0"     */
/*  check_value_l2c L2C_L2PLBFRC1,      L2C_L2PLBFRC1_VAL,      DEFAULT_MASK, "L2C_L2PLBFRC1"     */
    check_value_l2c L2C_L2PLBMCKEN0,    L2C_L2PLBMCKEN0_VAL,    DEFAULT_MASK, "L2C_L2PLBMCKEN0"
    check_value_l2c L2C_L2PLBMCKEN1,    L2C_L2PLBMCKEN1_VAL,    DEFAULT_MASK, "L2C_L2PLBMCKEN1"
    check_value_l2c L2C_L2PLBFERR0,     L2C_L2PLBFERR0_VAL,     DEFAULT_MASK, "L2C_L2PLBFERR0"
    check_value_l2c L2C_L2PLBFERR1,     L2C_L2PLBFERR1_VAL,     DEFAULT_MASK, "L2C_L2PLBFERR1"
    check_value_l2c L2C_L2PLBINTEN0,    L2C_L2PLBINTEN0_VAL,    DEFAULT_MASK, "L2C_L2PLBINTEN0"
    check_value_l2c L2C_L2PLBINTEN1,    L2C_L2PLBINTEN1_VAL,    DEFAULT_MASK, "L2C_L2PLBINTEN1"
    check_value_l2c L2C_L2ARRCFG,       L2C_L2ARRCFG_VAL,       DEFAULT_MASK, "L2C_L2ARRCFG"
    check_value_l2c L2C_L2ARRDBG0,      L2C_L2ARRDBG0_VAL,      DEFAULT_MASK, "L2C_L2ARRDBG0"
    check_value_l2c L2C_L2ARRDBG1,      L2C_L2ARRDBG1_VAL,      DEFAULT_MASK, "L2C_L2ARRDBG1"
    check_value_l2c L2C_L2ARRDBG2,      L2C_L2ARRDBG2_VAL,      DEFAULT_MASK, "L2C_L2ARRDBG2"
    check_value_l2c L2C_L2ARRDBG3,      L2C_L2ARRDBG3_VAL,      DEFAULT_MASK, "L2C_L2ARRDBG3"
    check_value_l2c L2C_L2ARRACCCTL,    L2C_L2ARRACCCTL_VAL,    DEFAULT_MASK, "L2C_L2ARRACCCTL"
    check_value_l2c L2C_L2ARRACCADR,    L2C_L2ARRACCADR_VAL,    DEFAULT_MASK, "L2C_L2ARRACCADR"
    check_value_l2c L2C_L2ARRACCDI0,    L2C_L2ARRACCDI0_VAL,    DEFAULT_MASK, "L2C_L2ARRACCDI0"
    check_value_l2c L2C_L2ARRACCDI1,    L2C_L2ARRACCDI1_VAL,    DEFAULT_MASK, "L2C_L2ARRACCDI1"
    check_value_l2c L2C_L2ARRACCDI2,    L2C_L2ARRACCDI2_VAL,    DEFAULT_MASK, "L2C_L2ARRACCDI2"
    check_value_l2c L2C_L2ARRACCDO0,    L2C_L2ARRACCDO0_VAL,    DEFAULT_MASK, "L2C_L2ARRACCDO0"
    check_value_l2c L2C_L2ARRACCDO1,    L2C_L2ARRACCDO1_VAL,    DEFAULT_MASK, "L2C_L2ARRACCDO1"
    check_value_l2c L2C_L2ARRACCDO2,    L2C_L2ARRACCDO2_VAL,    DEFAULT_MASK, "L2C_L2ARRACCDO2"
    check_value_l2c L2C_L2ARRSTAT0,     L2C_L2ARRSTAT0_VAL,     DEFAULT_MASK, "L2C_L2ARRSTAT0"
    check_value_l2c L2C_L2ARRSTAT1,     L2C_L2ARRSTAT1_VAL,     DEFAULT_MASK, "L2C_L2ARRSTAT1"
    check_value_l2c L2C_L2ARRSTAT2,     L2C_L2ARRSTAT2_VAL,     DEFAULT_MASK, "L2C_L2ARRSTAT2"
    /* L2C_L2ARRFRCx are W/O regs */
/*  check_value_l2c L2C_L2ARRFRC0,      L2C_L2ARRFRC0_VAL,      DEFAULT_MASK, "L2C_L2ARRFRC0"     */
/*  check_value_l2c L2C_L2ARRFRC1,      L2C_L2ARRFRC1_VAL,      DEFAULT_MASK, "L2C_L2ARRFRC1"     */
/*  check_value_l2c L2C_L2ARRFRC2,      L2C_L2ARRFRC2_VAL,      DEFAULT_MASK, "L2C_L2ARRFRC2"     */
    check_value_l2c L2C_L2ARRMCKEN0,    L2C_L2ARRMCKEN0_VAL,    DEFAULT_MASK, "L2C_L2ARRMCKEN0"
    check_value_l2c L2C_L2ARRMCKEN1,    L2C_L2ARRMCKEN1_VAL,    DEFAULT_MASK, "L2C_L2ARRMCKEN1"
    check_value_l2c L2C_L2ARRMCKEN2,    L2C_L2ARRMCKEN2_VAL,    DEFAULT_MASK, "L2C_L2ARRMCKEN2"
    check_value_l2c L2C_L2ARRFERR0,     L2C_L2ARRFERR0_VAL,     DEFAULT_MASK, "L2C_L2ARRFERR0"
    check_value_l2c L2C_L2ARRFERR1,     L2C_L2ARRFERR1_VAL,     DEFAULT_MASK, "L2C_L2ARRFERR1"
    check_value_l2c L2C_L2ARRFERR2,     L2C_L2ARRFERR1_VAL,     DEFAULT_MASK, "L2C_L2ARRFERR2"
    check_value_l2c L2C_L2ARRINTEN0,    L2C_L2ARRINTEN0_VAL,    DEFAULT_MASK, "L2C_L2ARRINTEN0"
    check_value_l2c L2C_L2ARRINTEN1,    L2C_L2ARRINTEN1_VAL,    DEFAULT_MASK, "L2C_L2ARRINTEN1"
    check_value_l2c L2C_L2ARRINTEN2,    L2C_L2ARRINTEN2_VAL,    DEFAULT_MASK, "L2C_L2ARRINTEN2"
    check_value_l2c L2C_L2CPUCFG,       L2C_L2CPUCFG_VAL,       DEFAULT_MASK, "L2C_L2CPUCFG"
    check_value_l2c L2C_L2CPUDBG,       L2C_L2CPUDBG_VAL,       DEFAULT_MASK, "L2C_L2CPUDBG"
    check_value_l2c L2C_L2CPUSTAT,      L2C_L2CPUSTAT_VAL,      DEFAULT_MASK, "L2C_L2CPUSTAT"
    /* L2C_L2CPUFRC is W/O reg */
/*  check_value_l2c L2C_L2CPUFRC,       L2C_L2CPUFRC_VAL,       DEFAULT_MASK, "L2C_L2CPUFRC"      */
    check_value_l2c L2C_L2CPUMCKEN,     L2C_L2CPUMCKEN_VAL,     DEFAULT_MASK, "L2C_L2CPUMCKEN"
    check_value_l2c L2C_L2CPUFERR,      L2C_L2CPUFERR_VAL,      DEFAULT_MASK, "L2C_L2CPUFERR"
    check_value_l2c L2C_L2CPUINTEN,     L2C_L2CPUINTEN_VAL,     DEFAULT_MASK, "L2C_L2CPUINTEN"
    check_value_l2c L2C_L2RACCFG,       L2C_L2RACCFG_VAL,       DEFAULT_MASK, "L2C_L2RACCFG"
    check_value_l2c L2C_L2RACDBG0,      L2C_L2RACDBG0_VAL,      DEFAULT_MASK, "L2C_L2RACDBG0"
    check_value_l2c L2C_L2RACDBG1,      L2C_L2RACDBG1_VAL,      DEFAULT_MASK, "L2C_L2RACDBG1"
    check_value_l2c L2C_L2RACSTAT0,     L2C_L2RACSTAT0_VAL,     DEFAULT_MASK, "L2C_L2RACSTAT0"
    /* L2C_L2RACFRC0 is W/O reg */
/*  check_value_l2c L2C_L2RACFRC0,      L2C_L2RACFRC0_VAL,      DEFAULT_MASK, "L2C_L2RACFRC0"     */
    check_value_l2c L2C_L2RACMCKEN0,    L2C_L2RACMCKEN0_VAL,    DEFAULT_MASK, "L2C_L2RACMCKEN0"
    check_value_l2c L2C_L2RACFERR0,     L2C_L2RACFERR0_VAL,     DEFAULT_MASK, "L2C_L2RACFERR0"
    check_value_l2c L2C_L2RACINTEN0,    L2C_L2RACINTEN0_VAL,    DEFAULT_MASK, "L2C_L2RACINTEN0"
    check_value_l2c L2C_L2WACCFG,       L2C_L2WACCFG_VAL,       DEFAULT_MASK, "L2C_L2WACCFG"
    check_value_l2c L2C_L2WACDBG0,      L2C_L2WACDBG0_VAL,      DEFAULT_MASK, "L2C_L2WACDBG0"
    check_value_l2c L2C_L2WACDBG1,      L2C_L2WACDBG1_VAL,      DEFAULT_MASK, "L2C_L2WACDBG1"
    check_value_l2c L2C_L2WACDBG2,      L2C_L2WACDBG2_VAL,      DEFAULT_MASK, "L2C_L2WACDBG2"
    check_value_l2c L2C_L2WACSTAT0,     L2C_L2WACSTAT0_VAL,     DEFAULT_MASK, "L2C_L2WACSTAT0"
    check_value_l2c L2C_L2WACSTAT1,     L2C_L2WACSTAT1_VAL,     DEFAULT_MASK, "L2C_L2WACSTAT1"
    check_value_l2c L2C_L2WACSTAT2,     L2C_L2WACSTAT2_VAL,     DEFAULT_MASK, "L2C_L2WACSTAT2"
    /* L2C_L2WACFRCx are W/O regs */
/*  check_value_l2c L2C_L2WACFRC0,      L2C_L2WACFRC0_VAL,      DEFAULT_MASK, "L2C_L2WACFRC0"     */
/*  check_value_l2c L2C_L2WACFRC1,      L2C_L2WACFRC1_VAL,      DEFAULT_MASK, "L2C_L2WACFRC1"     */
/*  check_value_l2c L2C_L2WACFRC2,      L2C_L2WACFRC2_VAL,      DEFAULT_MASK, "L2C_L2WACFRC2"     */
    check_value_l2c L2C_L2WACMCKEN0,    L2C_L2WACMCKEN0_VAL,    DEFAULT_MASK, "L2C_L2WACMCKEN0"
    check_value_l2c L2C_L2WACMCKEN1,    L2C_L2WACMCKEN1_VAL,    DEFAULT_MASK, "L2C_L2WACMCKEN1"
    check_value_l2c L2C_L2WACMCKEN2,    L2C_L2WACMCKEN2_VAL,    DEFAULT_MASK, "L2C_L2WACMCKEN2"
    check_value_l2c L2C_L2WACFERR0,     L2C_L2WACFERR0_VAL,     DEFAULT_MASK, "L2C_L2WACFERR0"
    check_value_l2c L2C_L2WACFERR1,     L2C_L2WACFERR1_VAL,     DEFAULT_MASK, "L2C_L2WACFERR1"
    check_value_l2c L2C_L2WACFERR2,     L2C_L2WACFERR2_VAL,     DEFAULT_MASK, "L2C_L2WACFERR2"
    check_value_l2c L2C_L2WACINTEN0,    L2C_L2WACINTEN0_VAL,    DEFAULT_MASK, "L2C_L2WACINTEN0"
    check_value_l2c L2C_L2WACINTEN1,    L2C_L2WACINTEN1_VAL,    DEFAULT_MASK, "L2C_L2WACINTEN1"
    check_value_l2c L2C_L2WACINTEN2,    L2C_L2WACINTEN2_VAL,    DEFAULT_MASK, "L2C_L2WACINTEN2"

check_cpu_ppc_l2c_reg:
    rumboot_putstring   "Check running <1> ...\n"
    load_const  R_EXPV, 0x00000000
    load_const  R_SHFT, 0x00000001
    load_const  R_ADDR, L2C_L2PLBDBG
    mtdcrx      R_DCRA, R_ADDR          /* Select address */
    mfdcrx      R_BACK, R_DCRD          /* Backup previous value */
    mtdcrx      R_DCRD, R_EXPV
    mfdcrx      R_DATA, R_DCRD
    cmpw cr7,   R_DATA, R_EXPV
    TEST_ASSERT(eq, cr7, "Check value failed (init 0)")

running_1:
    load_const  R_EXPV, 0x00000001
    load_const  R_TERM, 0x80000000
loop_1:
    mtdcrx      R_DCRD, R_EXPV
    mfdcrx      R_DATA, R_DCRD
    cmpw cr7,   R_DATA, R_EXPV
    TEST_ASSERT(eq, cr7, "Check value failed (running 1)") /* FAIL */
    cmpw cr0,   R_EXPV, R_TERM
    beq- cr0,   running_1_end
    slw         R_EXPV, R_EXPV, R_SHFT
    b loop_1
running_1_end:
    rumboot_putstring "done!\n"

running_0:
    rumboot_putstring "Check running <0> ...\n"
    load_const  R_EXPV, 0xFFFFFFFF
    load_const  R_ADDR, L2C_L2DBGSEL
    mtdcrx      R_DCRD, R_EXPV
    mfdcrx      R_DATA, R_DCRD
    cmpw cr7,   R_DATA, R_EXPV
    TEST_ASSERT(eq, cr7, "Check value failed (running 0)")

    load_const  R_EXPV, 0xFFFFFFFE
    load_const  R_TERM, 0x7FFFFFFF
loop_0:
    mtdcrx      R_DCRD, R_EXPV
    mfdcrx      R_DATA, R_DCRD
    cmpw cr7,   R_DATA, R_EXPV
    TEST_ASSERT(eq, cr7, "Check value failed (running 0)") /* FAIL */
    cmpw cr0,   R_EXPV, R_TERM
    beq- cr0,   running_0_end
    slw         R_EXPV, R_EXPV, R_SHFT
    ori         R_EXPV, R_EXPV, 0x0001
    b           loop_0
running_0_end:
    mtdcrx      R_DCRD, R_BACK          /* Restore previous value */
    rumboot_putstring "done!\n"

test_end:
    bne- cr6,   test_error

test_ok:
    rumboot_putstring "TEST OK\n"
    load_const R_RETV, RESULT_OK
    b finish

test_error:
    rumboot_putstring "TEST ERROR\n"
    load_const R_RETV, RESULT_ERROR

finish:
	blr


