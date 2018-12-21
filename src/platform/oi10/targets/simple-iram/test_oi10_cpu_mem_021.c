/*
 * test_oi10_cpu_mem_021.c
 *
 *  Created on: Dec 21, 2018
 *      Author: a.gurov
 */


#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RUMBOOT_ASSERT_WARN_ONLY

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/common_macros/common_macros.h>

#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>

#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>

/* Config */
#define CHECK_BASE                  SRAM1_BASE
#define CHECK_OFFSET                0x0000      //0x0200
#define L2C_LINE_BYTES              128
#define L2C_REQUEST_TRIES           0x80

/* Integer constant macros */
#define TEST_OK                     0x00000000
#define TEST_ERROR                  !TEST_OK
#define BYTE_MASK                   0xFF
#define NOTHING                     0x00000000
#define NO                          NOTHING
#define YES                         (!NO)
#define FAIL_VALUE                  ~NOTHING
#define PPC_EXT_ADDR_WIDTH              42
#define MACHINE_BITS                (sizeof(uintptr_t) << 3)
/* + L2C specific constants */
#define L2C_LINE_WORDS              (L2C_LINE_BYTES / sizeof(uintptr_t))
#define BUFFERID_LRU                L2C_L2ARRACCCTL_BUFFERID_LRU_i
#define BUFFERID_TAG                L2C_L2ARRACCCTL_BUFFERID_TAG_i
#define BUFFERID_DATA               L2C_L2ARRACCCTL_BUFFERID_DATA_i
#define REQTYPE_RD8WOECC            L2C_L2ARRACCCTL_REQTYPE_RD8WOECC
#define REQTYPE_WR8WTECC            L2C_L2ARRACCCTL_REQTYPE_WR8WTECC
#define REQTYPE_WR8WOECC            L2C_L2ARRACCCTL_REQTYPE_WR8WOECC
#define L2C_ARR_CTL_M_REQ           (1 << L2C_L2ARRACCCTL_REQUEST_i)
#define L2C_ARR_CTL_R_REQ           (1 << L2C_L2ARRACCCTL_READREQUESTCOMP_i)
#define L2C_ARR_CTL_W_REQ           (1 << L2C_L2ARRACCCTL_WRTREQUESTCOMP_e)
#define L2C_TAG_ADDR_SHIFT          6
#define L2C_TAG_STAT_BITS           3
#define L2C_TAG_STAT_MASK           0x07
#define L2C_TAG_ADDR_MASK           0x1FFFFFFF
#define L2C_TAG_STAT_SHIFT          (32 - L2C_TAG_STAT_BITS)
#define L2C_TAG_ENTRY_SHIFT_LO      ((PPC_EXT_ADDR_WIDTH       \
                                        - MACHINE_BITS)        \
                                        + L2C_TAG_STAT_BITS)
#define L2C_TAG_ENTRY_SHIFT_HI      (     L2C_TAG_STAT_BITS)
#define REQUEST_COMPLETE_MASK       (/* L2C_ARR_CTL_R_REQ | */ \
                                     /* L2C_ARR_CTL_W_REQ | */ \
                                        L2C_ARR_CTL_M_REQ )

/* ++ L2C Cache State (debug) Modes */
#define L2C_CSM_DEFAULT             0x00
#define L2C_CSM_NO_SL               0x01
#define L2C_CSM_NO_MU               0x02
#define L2C_CSM_NO_T                0x04
#define L2C_CSM_MESI_ONLY           0x07

/* ++ L2C Tag Array Cache State */
#define L2C_STATE_I                 0b000   /* Invalid              */
#define L2C_STATE_U                 0b001   /* Undefined (not used) */
#define L2C_STATE_S                 0b010   /* Shared               */
#define L2C_STATE_SL                0b011   /* Shared Last          */
#define L2C_STATE_E                 0b100   /* Exclusive            */
#define L2C_STATE_T                 0b101   /* Tagged               */
#define L2C_STATE_M                 0b110   /* Modified             */
#define L2C_STATE_MU                0b111   /* Modified Unsolicited */


#define MAY_NOT_BE_USED             __attribute__((unused))
#define ALWAYS_INLINE               __attribute__((always_inline))

/* Utility (function) macros */
#define BIT(N)                      (1 << (N))
#define CAST_ADDR(VAL)              ((void*)(VAL))
#define GEN_REQTYPE(REQTYPE)        (REQTYPE << L2C_L2ARRACCCTL_REQTYPE_i)
#define GEN_WAY_FIELD(WAY)          ((WAY) << L2C_L2ARRACCCTL_L2WAY_i)
#define CHECK_ADDR(OFFT)            (CHECK_BASE + CHECK_OFFSET + (OFFT))
#define GET_BYTE(VAL,BYTE)          (((VAL) >> ((BYTE) << 3)) & BYTE_MASK)
#define L2C_TAG_XTR_ST(TD)          (((TD) >> L2C_TAG_STAT_SHIFT)      \
                                        & L2C_TAG_STAT_MASK)
#define L2C_READ(REG)               l2c_l2_read(DCR_L2C_BASE,          \
                                        (L2C_L2REG)(REG))
#define L2C_WRITE(REG,VAL)          l2c_l2_write(DCR_L2C_BASE,         \
                                        (L2C_L2REG)(REG),(VAL))
#define L2C_ARR_RD_TAG_RQ(WAY)      (BIT(L2C_L2ARRACCCTL_REQUEST_i)  | \
                                     BIT(BUFFERID_TAG)               | \
                                     GEN_REQTYPE(REQTYPE_RD8WOECC)   | \
                                     GEN_WAY_FIELD(WAY))


union u64_u32x2_t
{
    uint32_t    u32[2];
    uint64_t    u64;
     int64_t    i64;
};

typedef union u64_u32x2_t u64_u32x2_t;

/* Global vars */
MAY_NOT_BE_USED
static tlb_entry em2_noexec_entries[] =
{
    {   /*            ERPN,RPN,    EPN,    DSIZ    */
        MMU_TLB_ENTRY(0x00,0x00000,0x00000,MMU_TLBE_DSIZ_1GB,
        /*  IL1I,IL1D,W,I,M,G,E */
            1,   1,   0,0,1,0,MMU_TLBE_E_BIG_END,
        /*  UX,UW,UR, SX,SW,SR,DULXE,IULXE,TS,TID  */
            0, 0, 0,  0, 1, 1, 0,    0,    0, MEM_WINDOW_0,
        /*  WAY,            BID,             V     */
            MMU_TLBWE_WAY_3,MMU_TLBWE_BE_UND,1)
    },
    {   /*            ERPN,RPN,    EPN,    DSIZ    */
        MMU_TLB_ENTRY(0x00,0x40000,0x40000,MMU_TLBE_DSIZ_1GB,
        /*  IL1I,IL1D,W,I,M,G,E */
            1,   1,   0,0,1,0,MMU_TLBE_E_BIG_END,
        /*  UX,UW,UR, SX,SW,SR,DULXE,IULXE,TS,TID  */
            0, 0, 0,  0, 1, 1, 0,    0,    0, MEM_WINDOW_0,
        /*  WAY,            BID,             V     */
            MMU_TLBWE_WAY_3,MMU_TLBWE_BE_UND,1)
    }
};
MAY_NOT_BE_USED
static uint32_t l2c_cache_sizes[4] =
    {256*1024, 512*1024, 1024*1024, 128*1024};
MAY_NOT_BE_USED
static uint32_t l2c_addr_masks[4] =
    /* 256     512     1024    128 */
    {0x03FE, 0x07FE, 0x0FFE, 0x01FE};
MAY_NOT_BE_USED
static uint32_t l2c_read_req[4] =
{
        L2C_ARR_RD_TAG_RQ(0),
        L2C_ARR_RD_TAG_RQ(1),
        L2C_ARR_RD_TAG_RQ(2),
        L2C_ARR_RD_TAG_RQ(3)
};

static volatile uint32_t g_cnfg0 = 0;
static volatile uint32_t g_cnfg1 = 0;


MAY_NOT_BE_USED
static uint32_t mem_read32(uint32_t ptr)
{
    uint32_t readed;
    rumboot_printf("Reading from 0x%X...\n", CHECK_ADDR(ptr));
    readed = ioread32(CHECK_ADDR(ptr));
    g_cnfg1 = L2C_READ(L2C_L2CNFG1);
    rumboot_printf("Readed: 0x%X (%d).\n", readed, readed);
    return readed;
}

MAY_NOT_BE_USED
static void mem_write32(uint32_t val, uint32_t ptr)
{
    rumboot_printf("Writing 0x%X (%d) to 0x%X...\n",
            val, val, CHECK_ADDR(ptr));
    iowrite32(val, CHECK_ADDR(ptr));
    g_cnfg1 = L2C_READ(L2C_L2CNFG1);
}

MAY_NOT_BE_USED
static int32_t l2c_read_lru(uintptr_t       addr,
                            u64_u32x2_t    *lru_addr,
                            int32_t        *way_out)
{
    uint32_t way, tries, result, l2c_addr32;
    int32_t  l2c_stat;
    int64_t  l2c_addr64;
    l2c_addr64  = rumboot_virt_to_phys(CAST_ADDR(addr));
    l2c_addr32  = (uint32_t)((l2c_addr64 >> L2C_TAG_ADDR_SHIFT)
                    & l2c_addr_masks[g_cnfg0 & 0x03]);
    L2C_WRITE(L2C_L2ARRACCADR, l2c_addr32);
    for (way = 0; way < 4; way++)
    {
        result = 0;
        l2c_stat = 0;
        L2C_WRITE(L2C_L2ARRACCCTL, l2c_read_req[way]);
        for(tries = L2C_REQUEST_TRIES;
            (L2C_READ(L2C_L2ARRACCCTL) & REQUEST_COMPLETE_MASK)
                    && tries;
            tries--) {;} /* No body for this 'for' */
        result = L2C_READ(L2C_L2ARRACCDO0);
        l2c_stat = L2C_TAG_XTR_ST(result);
        if(tries && l2c_stat)
        {
            *way_out = way;
            lru_addr->u64 = ((uint64_t)(result & L2C_TAG_ADDR_MASK))
                            << L2C_TAG_ENTRY_SHIFT_LO;
            return l2c_stat;
        } else if(!tries)
                rumboot_printf(
                    "L2C tag array request timeout! "
                    "(way%d, tag=0x%X)\n",
                    way, result);
    }
    return -1;
}

uint32_t main(void)
{
    uint32_t     status  = TEST_OK,
                 revid   = 0,
                 idx     = 0;

    rumboot_printf("Init EMI...\n");
    emi_init(DCR_EM2_EMI_BASE);
    msync();
    for(idx = 0; idx < (128 / sizeof(uint32_t)); idx++)
        iowrite32(0, CHECK_ADDR(idx * sizeof(uint32_t)));
    mem_write32(0x5555AAAA, 0);
    rumboot_printf("Init done.\n");
    rumboot_printf("Write TLB entries...\n");
    write_tlb_entries(em2_noexec_entries, 2);
    tlbsync();
    msync();
    isync();
    revid = L2C_READ(L2C_L2REVID);
    g_cnfg0 = L2C_READ(L2C_L2CNFG0);
    g_cnfg1 = L2C_READ(L2C_L2CNFG1);
    rumboot_printf("Start checking L2C at DCR base 0x%X (ver:%d, rev:%d)...\n",
            DCR_L2C_BASE, revid >> 8 & 0x0FFF, GET_BYTE(revid,0));
    rumboot_printf("L2C_L2CNFG0=0x%X, L2C_L2CNFG1=0x%X,%s\n", g_cnfg0, g_cnfg1);
    rumboot_printf("Cache size is %dKB\n",
            (l2c_cache_sizes[g_cnfg0 & 0x03]) >> 10);

    status |= 0;

    rumboot_printf("TEST %s!\n", !status ? "OK" : "ERROR");

    return status;
}





