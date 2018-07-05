#ifndef L2C_CONFIG_H
#define L2C_CONFIG_H


//L2 Cache Core Architected DCR List
#define L2C_DCRAI   0x00
#define L2C_DCRDI   0x04
#define L2CDCRAI(l2c_dcr_base)      (l2c_dcr_base + L2C_DCRAI)
#define L2CDCRDI(l2c_dcr_base)      (l2c_dcr_base + L2C_DCRDI)
//L2 Cache Core Indirect DCR List
#define L2ISTAT         0x000
#define L2PNCR          0x004
#define L2REVID         0x00C
#define L2CNFG0         0x010
#define L2CNFG1         0x014
#define L2DBGSEL        0x020
#define L2DBGDATA0      0x024
#define L2DBGDATA1      0x028
#define L2SLEEPSTAT     0x080
#define L2SLEEPREQ      0x084
#define L2MCK           0x120
#define L2MCKEN         0x130
#define L2FERR          0x140
#define L2INT           0x150
#define L2INTEN         0x160
#define L2LOG0          0x180
#define L2LOG1          0x184
#define L2LOG2          0x188
#define L2LOG3          0x18C
#define L2LOG4          0x190
#define L2LOG5          0x194
#define L2PLBCFG        0x210
#define L2PLBDBG        0x220
#define L2PLBERAP       0x230
#define L2PLBSTAT0      0x300
#define L2PLBSTAT1      0x304
#define L2PLBFRC0       0x310
#define L2PLBFRC1       0x314
#define L2PLBMCKEN0     0x330
#define L2PLBMCKEN1     0x334
#define L2PLBFERR0      0x340
#define L2PLBFERR1      0x344
#define L2PLBINTEN0     0x360
#define L2PLBINTEN1     0x364
#define L2ARRCFG        0x410
#define L2ARRDBG0       0x420
#define L2ARRDBG1       0x424
#define L2ARRDBG2       0x428
#define L2ARRDBG3       0x42C
#define L2ARRACCCTL     0x480
#define L2ARRACCADR     0x484
#define L2ARRACCDI0     0x490
#define L2ARRACCDI1     0x494
#define L2ARRACCDI2     0x498
#define L2ARRACCDO0     0x4A0
#define L2ARRACCDO1     0x4A4
#define L2ARRACCDO2     0x4A8
#define L2ARRSTAT0      0x500
#define L2ARRSTAT1      0x504
#define L2ARRSTAT2      0x508
#define L2ARRFRC0       0x510
#define L2ARRFRC1       0x514
#define L2ARRFRC2       0x518
#define L2ARRMCKEN0     0x530
#define L2ARRMCKEN1     0x534
#define L2ARRMCKEN2     0x538
#define L2ARRFERR0      0x540
#define L2ARRFERR1      0x544
#define L2ARRFERR2      0x548
#define L2ARRINTEN0     0x560
#define L2ARRINTEN1     0x564
#define L2ARRINTEN2     0x568
#define L2CPUCFG        0x610
#define L2CPUDBG        0x620
#define L2CPUSTAT       0x700
#define L2CPUFRC        0x710
#define L2CPUMCKEN      0x730
#define L2CPUFERR       0x740
#define L2CPUINTEN      0x760
#define L2RACCFG        0x810
#define L2RACDBG0       0x820
#define L2RACDBG1       0x824
#define L2RACSTAT0      0x900
#define L2RACFRC0       0x910
#define L2RACMCKEN0     0x930
#define L2RACFERR0      0x940
#define L2RACINTEN0     0x960
#define L2WACCFG        0xC10
#define L2WACDBG0       0xC20
#define L2WACDBG1       0xC24
#define L2WACDBG2       0xC28
#define L2WACSTAT0      0xD00
#define L2WACSTAT1      0xD04
#define L2WACSTAT2      0xD08
#define L2WACFRC0       0xD10
#define L2WACFRC1       0xD14
#define L2WACFRC2       0xD18
#define L2WACMCKEN0     0xD30
#define L2WACMCKEN1     0xD34
#define L2WACMCKEN2     0xD38
#define L2WACFERR0      0xD40
#define L2WACFERR1      0xD44
#define L2WACFERR2      0xD48
#define L2WACINTEN0     0xD60
#define L2WACINTEN1     0xD64
#define L2WACINTEN2     0xD68

#define L2INTEN_EXTINT_BIT

//PMU Architected DCR List
#define PMU_DCRAI   0x80
#define PMU_DCRDI   0x84
#define PMUDCRAI(pmu_dcr_base)      (pmu_dcr_base + PMU_DCRAI)
#define PMUDCRDI(pmu_dcr_base)      (pmu_dcr_base + PMU_DCRDI)
//PMU Indirect DCR List
#define PMUGS0          0x000
#define PMUGC0          0x004
#define PMUIS0          0x010
#define PMUIE0          0x014
#define PMUREVID        0xC00
#define PMULC0          0x800
#define PMUC0           0x808
#define PMULC1          0x810
#define PMUC1           0x818
#define PMULC2          0x820
#define PMUC2           0x828
#define PMULC3          0x830
#define PMUC3           0x838
#define PMULC4          0x900
#define PMUC4           0x908
#define PMULC5          0x910
#define PMUC5           0x918
#define PMULC6          0x920
#define PMUC6           0x928
#define PMULC7          0x930
#define PMUC7           0x938
#define PMULC8          0xA00
#define PMUC8           0xA08
#define PMULC9          0xA10
#define PMUC9           0xA18
#define PMULC10         0xA20
#define PMUC10          0xA28
#define PMULC11         0xA30
#define PMUC11          0xA38
#define PMULC12         0xB00
#define PMUC12          0xB08
#define PMULC13         0xB10
#define PMUC13          0xB18
#define PMULC14         0xB20
#define PMUC14          0xB28
#define PMULC15         0xB30
#define PMUC15          0xB38


#define I_M_SELF_ID(base) ( (base == L2C0_DCR_BASE) ? 0x00 : 0x01 )
//from trunk/cpu/units/ppc476l2c_v001/verilog/rtl/PPC476L2C.parameters.v
#define  L2C_VERSION_NUMBER   0x001
#define  L2C_REVISION_ID      0x02
//from dut
#define L2C_L2SIZE              0x1
#define L2C_PLB_CLK_RATIO       0x0
#define L2C_TSNOOP             0x0 //acc. to L2C_PLBTSNOOP[0:2] strap pin
#define L2C_MASTER_ID(base)     I_M_SELF_ID(base)


#define L2C_L2ISTAT_VAL     0x00000001 //1- initialization complete

#define L2C_L2PNCR_VAL_BASE(base) ( (base == L2C0_DCR_BASE) ? 0x00000069 : 0x00000075 ) //see trunk/cpu/units/ppc476l2c_v001/verilog/rtl/PPC476L2C_DCR_LFSR.v. Dependent from I_M_SELF_ID strap.
#define L2C_L2PNCR_VAL      L2C_L2PNCR_VAL_BASE(L2C0_DCR_BASE)

#define L2C_L2REVID_VAL     ( reg_field(31,L2C_REVISION_ID) | reg_field(23,L2C_VERSION_NUMBER) )
#define L2C_L2CNFG0_VAL_BASE(base)     ( reg_field(31,L2C_L2SIZE) | reg_field(27,L2C_PLB_CLK_RATIO) | reg_field(23,L2C_TSNOOP) | reg_field(19,L2C_MASTER_ID(base)) )
#define L2C_L2CNFG0_VAL     L2C_L2CNFG0_VAL_BASE(L2C0_DCR_BASE)

#define L2C_L2CNFG1_VAL     ( reg_field(26,0x1) )
#define L2C_L2DBGSEL_VAL    0x00000000
#define L2C_L2DBGDATA0_VAL  0x00000000
#define L2C_L2DBGDATA1_VAL  0x00000000
#define L2C_L2SLEEPSTAT_VAL 0x00000000
#define L2C_L2SLEEPREQ_VAL  0x00000000
#define L2C_L2MCK_VAL       0x00000000
#define L2C_L2MCKEN_VAL     reg_field(30,0x7FF)
#define L2C_L2FERR_VAL      0x00000000
#define L2C_L2INT_VAL       0x00000000
#define L2C_L2INTEN_VAL     reg_field(30,0x3FF)
#define L2C_L2LOG0_VAL      0x00000000
#define L2C_L2LOG1_VAL      0x00000000
#define L2C_L2LOG2_VAL      0x00000000
#define L2C_L2LOG3_VAL      0x00000000
#define L2C_L2LOG4_VAL      0x00000000
#define L2C_L2LOG5_VAL      0x00000000
#define L2C_L2PLBCFG_VAL    0x00000000
#define L2C_L2PLBDBG_VAL    0x00000000
#define L2C_L2PLBERAP_VAL   0x00000000
#define L2C_L2PLBSTAT0_VAL  0x00000000
#define L2C_L2PLBSTAT1_VAL  0x00000000
#define L2C_L2PLBFRC0_VAL   0x00000000
#define L2C_L2PLBFRC1_VAL   0x00000000
#define L2C_L2PLBMCKEN0_VAL 0xFFFFFFFF
#define L2C_L2PLBMCKEN1_VAL 0xFFFFFFFF
#define L2C_L2PLBFERR0_VAL  0x00000000
#define L2C_L2PLBFERR1_VAL  0x00000000
#define L2C_L2PLBINTEN0_VAL 0x00000000
#define L2C_L2PLBINTEN1_VAL 0x00000000
#define L2C_L2ARRCFG_VAL    0x00000000
#define L2C_L2ARRDBG0_VAL   0x00000000
#define L2C_L2ARRDBG1_VAL   0x00000000
#define L2C_L2ARRDBG2_VAL   0x00000000
#define L2C_L2ARRDBG3_VAL   0x00000000
#define L2C_L2ARRACCCTL_VAL 0x00000000
#define L2C_L2ARRACCADR_VAL 0x00000000
#define L2C_L2ARRACCDI0_VAL 0x00000000
#define L2C_L2ARRACCDI1_VAL 0x00000000
#define L2C_L2ARRACCDI2_VAL 0x00000000
#define L2C_L2ARRACCDO0_VAL 0x00000000
#define L2C_L2ARRACCDO1_VAL 0x00000000
#define L2C_L2ARRACCDO2_VAL 0x00000000
#define L2C_L2ARRSTAT0_VAL  0x00000000
#define L2C_L2ARRSTAT1_VAL  0x00000000
#define L2C_L2ARRSTAT2_VAL  0x00000000
#define L2C_L2ARRFRC0_VAL   0x00000000
#define L2C_L2ARRFRC1_VAL   0x00000000
#define L2C_L2ARRFRC2_VAL   0x00000000
#define L2C_L2ARRMCKEN0_VAL 0xFFFFFFFF
#define L2C_L2ARRMCKEN1_VAL 0x00001FFF
#define L2C_L2ARRMCKEN2_VAL 0x00000000
#define L2C_L2ARRFERR0_VAL  0x00000000
#define L2C_L2ARRFERR1_VAL  0x00000000
#define L2C_L2ARRFERR2_VAL  0x00000000
#define L2C_L2ARRINTEN0_VAL 0x00000000
#define L2C_L2ARRINTEN1_VAL 0x00000000
#define L2C_L2ARRINTEN2_VAL 0x00FFFFFF
#define L2C_L2CPUCFG_VAL    0x00000000
#define L2C_L2CPUDBG_VAL    0x00000000
#define L2C_L2CPUSTAT_VAL   0x00000000
#define L2C_L2CPUFRC_VAL    0x00000000
#define L2C_L2CPUMCKEN_VAL  0x00000FFF
#define L2C_L2CPUFERR_VAL   0x00000000
#define L2C_L2CPUINTEN_VAL  0x00000000
#define L2C_L2RACCFG_VAL    0x00000000
#define L2C_L2RACDBG0_VAL   0x00000000
#define L2C_L2RACDBG1_VAL   0x00000000
#define L2C_L2RACSTAT0_VAL  0x00000000
#define L2C_L2RACFRC0_VAL   0x00000000
#define L2C_L2RACMCKEN0_VAL 0x03FFFFFF
#define L2C_L2RACFERR0_VAL  0x00000000
#define L2C_L2RACINTEN0_VAL 0x00000000
#define L2C_L2WACCFG_VAL    0x00000000
#define L2C_L2WACDBG0_VAL   0x00000000
#define L2C_L2WACDBG1_VAL   0x00000000
#define L2C_L2WACDBG2_VAL   0x00000000
#define L2C_L2WACSTAT0_VAL  0x00000000
#define L2C_L2WACSTAT1_VAL  0x00000000
#define L2C_L2WACSTAT2_VAL  0x00000000
#define L2C_L2WACFRC0_VAL   0x00000000
#define L2C_L2WACFRC1_VAL   0x00000000
#define L2C_L2WACFRC2_VAL   0x00000000
#define L2C_L2WACMCKEN0_VAL 0x00FFFFFF
#define L2C_L2WACMCKEN1_VAL 0x00FFFFFF
#define L2C_L2WACMCKEN2_VAL 0x00FFFFFF
#define L2C_L2WACFERR0_VAL  0x00000000
#define L2C_L2WACFERR1_VAL  0x00000000
#define L2C_L2WACFERR2_VAL  0x00000000
#define L2C_L2WACINTEN0_VAL 0x00000000
#define L2C_L2WACINTEN1_VAL 0x00000000
#define L2C_L2WACINTEN2_VAL 0x00000000

#define L2C_L2ARRACCADR_MSK             0x1FFF
#define L2C_L2ARRACCADR_ADR_MSK         0x7FF
#define L2C_TIMEOUT							0x00000020
#define	L2C_WAY0_NUM						0x0
#define	L2C_WAY1_NUM						0x1
#define	L2C_WAY2_NUM						0x2
#define	L2C_WAY3_NUM						0x3
#define L2C_WAY_MSK							0x3
#define L2C_CACHE_LINE_ADDR_MSK						0x3FF
#define L2C_CACHE_HALFLINE_ADDR_MSK					0x7FF //L2C_L2ARRACCADR_ADR_MSK
#define L2C_ARRACCCTL_WAY_MSK							reg_field(23,0x3)
#define L2C_ARRACCCTL_DWORD_MSK							reg_field(31,0xFF)
#define L2C_ARRACCCTL_MSK							  ( L2C_ARRACCCTL_REQUEST_FIELD |\
														L2C_ARRACCCTL_LRU_ARRAY_SELECT_FIELD |\
														L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD |\
														L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD |\
														L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD |\
														L2C_ARRACCCTL_WAY_MSK |\
														L2C_ARRACCCTL_DWORD_MSK \
													  )
//bits [0:8] are reserved
#define L2C_ARRACCCTL_REQUEST_FIELD						reg_field(9,0x1)
#define L2C_ARRACCCTL_READ_REQUEST_COMP_FIELD			reg_field(10,0x1)
#define L2C_ARRACCCTL_WRITE_REQUEST_COMP_FIELD			reg_field(11,0x1)
//bit 12 is reserved
#define L2C_ARRACCCTL_LRU_ARRAY_SELECT_FIELD			reg_field(13,0x1)
#define L2C_ARRACCCTL_TAG_ARRAY_SELECT_FIELD			reg_field(14,0x1)
#define L2C_ARRACCCTL_DATA_ARRAY_SELECT_FIELD			reg_field(15,0x1)
#define L2C_ARRACCCTL_READ_8BYTE_WO_ECC_FIELD			reg_field(19,0xF)
#define L2C_ARRACCCTL_WRITE_8BYTE_GENERATE_ECC_FIELD	reg_field(19,0x6)
#define L2C_ARRACCCTL_WRITE_8BYTE_ECC_AS_IS_FIELD		reg_field(19,0x7)
#define L2C_ARRACCCTL_WAY_FIELD(way_number)				reg_field(23,((way_number) & 0x3))
#define L2C_ARRACCCTL_MSK_DWORD_FIELD(addr)				reg_field(31,(0x1 << (0x7 - ( ((addr) >> 0x3) & 0x7) )))
#define L2C_ARRACCADR_TAG_LRU_ADDR_FIELD(addr)			reg_field( 30,( ( (addr) >> 7) & L2C_CACHE_LINE_ADDR_MSK ) )
#define L2C_ARRACCADR_DATA_ARRAY_ADDR_FIELD(addr)		reg_field( 31,( ( (addr) >> 6) & L2C_L2ARRACCADR_ADR_MSK ) )

#define L2C_TAG_ADDRESS_OFFSET							(17)
#define L2C_TAG_ADDRESS_MSK								0x1FFFFFF
#define L2C_TAG_CACHE_STATE_MSK							0x7
#define L2C_TAG_CACHE_STATE_INVALID						0x0
#define L2C_TAG_ADDRESS_FROM_64BIT(addr)				(((addr) & ( L2C_TAG_ADDRESS_MSK << L2C_TAG_ADDRESS_OFFSET ) ) >> L2C_TAG_ADDRESS_OFFSET)
#define L2C_TAG_ADDRESS_FROM_ARRACCDO0(val)				( ( (val) & (L2C_TAG_ADDRESS_MSK << 4) ) >> 4)
#define L2C_TAG_CACHE_STATE_FROM_ARRACCDO0(val)			( ((val) >> IBM_BIT_INDEX(32, 2)) & L2C_TAG_CACHE_STATE_MSK )
#define L2C_TAG_CACHE_STATE_INVALID_VAL					0x0

#define EXT_ADDR_MSK									0x3FF

#define DEFINE_L2MCKEN_EXTINT      (1 << 11)
#define DEFINE_L2MCKEN_PLBINT0     (1 << 10)
#define DEFINE_L2MCKEN_PLBINT1     (1 << 9)
#define DEFINE_L2MCKEN_L2AINT0     (1 << 8)
#define DEFINE_L2MCKEN_L2AINT1     (1 << 7)
#define DEFINE_L2MCKEN_L2AINT2     (1 << 6)
#define DEFINE_L2MCKEN_L1CINT0     (1 << 5)
#define DEFINE_L2MCKEN_RACINT0     (1 << 4)
#define DEFINE_L2MCKEN_WACINT0     (1 << 3)
#define DEFINE_L2MCKEN_WACINT1     (1 << 2)
#define DEFINE_L2MCKEN_WACINT2     (1 << 1)


#endif  // L2C_CONFIG_H
