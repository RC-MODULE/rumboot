
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

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>

#include <platform/regs/regs_emi.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/fields/emi.h>

#include <platform/interrupts.h>


/* Integer constant macros */
#define EMI_BASE                    DCR_EM2_EMI_BASE
#define IRR_RST_ALL                 0x0001FFFF
#define HXADR_DFLT                  0xFFFFFFF0
#define EMI_RFC_RP_DIV_1            0b11110011110011
#define EMI_RFC_RP_DIV_2            0b01111001111001
#define EMI_RFC_RP_MASK             0x8FFF
#define EMI_RFC_TRFC_MASK           0x0007
#define ERR_ADDR_MASK               (~0x0F)
#define IMR_UNMASK_ALL              IRR_RST_ALL
#define IRQ_FLAGS                   (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
#define IRQ_LIST_END                (~0)
#define EVERYTHING                  (~0)
#define ALL_INTERRUPTS              EVERYTHING
#define SET_LATER                   0
#define EMI_BITS_D0_D7              EMI_WECR_BWE0_i
#define EMI_BITS_D8_D15             EMI_WECR_BWE1_i
#define EMI_BITS_D16_D23            EMI_WECR_BWE2_i
#define EMI_BITS_D24_D31            EMI_WECR_BWE3_i
#define EMI_BITS_ED0_ED7            EMI_WECR_BWE4_i

#define TEST_OK                     0x00000000
#define TEST_ERROR                  0x00000001

#define FIRST_BANK                  0
#define LAST_BANK                   5

#define MEMTYPE_ENABLE_SRAM         ( 1 << BTYP_SRAM    )
#define MEMTYPE_ENABLE_NOR          ( 1 << BTYP_NOR     )
#define MEMTYPE_ENABLE_SSRAM        ( 1 << BTYP_SSRAM   )
#define MEMTYPE_ENABLE_PIPELINE     ( 1 << BTYP_PIPERDY )
#define MEMTYPE_ENABLE_SDRAM        ( 1 << BTYP_SDRAM   )

#define CKLIST_END                  {0,0,0,NULL}
#define _GET_UPTIME                 rumboot_platform_get_uptime


/* Config */
#define CONST_1                     0xBABA0001
#define CONST_2                     0xF00D4BEE
#define MK_ERR_1                    0x00000001
#define MK_ERR_2                    0x00000012
#define BASE_OFFSET                 0x00000100
#define WECR_DFLT                   0x00000000
#define ENABLED_MEM_TYPES           (                                       \
                                         MEMTYPE_ENABLE_SRAM        |       \
                                         MEMTYPE_ENABLE_SSRAM       |       \
                                         MEMTYPE_ENABLE_PIPELINE    |       \
                                         MEMTYPE_ENABLE_SDRAM               \
                                    )

#define NOINLINE                    __attribute__((noinline))

/* Utility (function) macros */
#define EMI_TYPE(BANK)              (emi_get_bank_cfg_cached(BANK)          \
                                        -> ssx_cfg.BTYP)
#define EMIA(ADDR)                  (EMI_BASE + ADDR)
#define SEL_ECNT(BANK,ECNT1,ECNT2)  (((BANK) >= 0 && (BANK) <= 2) ? (ECNT1) \
                                        : ((BANK) >= 3 && (BANK) <= 5)      \
                                            ? (ECNT2) : 0                   \
                                    )
#define MASK_ECNT(BANK)             (0xFF >> emi_ecnt_b[BANK])
#define READ_ECNT(BANK)             ((emi_read(emi_ecnt[BANK])              \
                                        >> emi_ecnt_b[BANK]) & 0xFF         \
                                    )
#define GET_ECNT(BANK,ECNT1,ECNT2)  ((SEL_ECNT((BANK), (ECNT1), (ECNT2))    \
                                        >> emi_ecnt_b[BANK]) & 0xFF         \
                                    )
#define SINGLE_ERR_FLAG(BANK)       (1 << (((BANK) << 1) + 0))
#define DOUBLE_ERR_FLAG(BANK)       (1 << (((BANK) << 1) + 1))
#define ECC_ON(BANK)                emi_write(EMI_HSTSR, \
                                        emi_read(EMI_HSTSR) |  (1 << (BANK)));
#define ECC_OFF(BANK)               emi_write(EMI_HSTSR, \
                                        emi_read(EMI_HSTSR) & ~(1 << (BANK)));

#define CMP_EQ(A1,A2,RS,ST)         ((ST) |= (RS = !((A1) == (A2))))
#define CMP_NE(A1,A2,RS,ST)         ((ST) |= (RS = !((A1) != (A2))))
/* (B)ank, (F)irst, (L)ast */
#define FOREACH_BANK(B,F,L)         for(B = F; B <= L; B++)
#define FOREACH_IRQ(ITEM,LIST)      for(ITEM = LIST; ~*ITEM; ITEM++)

/* Custom types definitions */
typedef struct rumboot_irq_entry    rumboot_irq_entry;
typedef volatile uint32_t           irq_flags_t;

typedef struct
{
    uint8_t bnk;
    uint8_t ecc;
    uint8_t cnt;
    uint8_t bwe;
} mkerr_param_t;

typedef struct
{
    uint32_t    val, exp, err, idx;
} mke_info_t;

typedef struct
{
    uint32_t    eccval;
    uint32_t    cntval;
    uint32_t    ecnt20;
    uint32_t    ecnt53;
    uint32_t    h1addr;
    uint32_t    h2addr;
    uint32_t    h1cmrr;
    uint32_t    h2cmrr;
} emi_ckregs_t;

/* Global vars */
const    /* Memory banks addresses */
uintptr_t emi_bank_bases[8] =
{
    EM2_BANK0_BASE, EM2_BANK1_BASE,
    EM2_BANK2_BASE, EM2_BANK3_BASE,
    EM2_BANK4_BASE, EM2_BANK5_BASE, 0,0
};
/* Memory banks types descriptions */
char *btyp_descr[8] =
{
/*   0           1           2           3          */
    "SRAM",     "NOR",      "SSRAM",    "UNKNOWN3",
/*   4           5           6           7          */
    "PIPELINE", "UNKNOWN5", "SDRAM",    "UNKNOWN7"
};
  /* Check templates 32 bits */
uint32_t    long_tpl[] =
{
    0xDEBA0001 ^ 0xFF000000,
    0xDEDA0001 ^ 0x00FF0000,
    0xDEDA0101 ^ 0x0000FF00,
    0xDEDA0100 ^ 0x000000FF
};
uint32_t    mask_tpl[] =
{
    0xFF000000,
    0x00FF0000,
    0x0000FF00,
    0x000000FF
};
/* Check templates 8 bits */
uint8_t     byte_tpl[] = {0xDE, 0xDA, 0x01, 0x00};
static volatile const uint8_t emi_ecnt_b[8] = {0,8,16,0,8,16,0,0};
static volatile const uint8_t emi_ecnt[8] =
    {
        EMI_ECNT20, EMI_ECNT20, EMI_ECNT20,
        EMI_ECNT53, EMI_ECNT53, EMI_ECNT53, 0,0
    };
static irq_flags_t IRQ = 0;
static rumboot_irq_entry *irq_table = NULL;
static volatile uint32_t global_irr = 0;
static uint8_t const_2_ecc[2];
static tlb_entry em2_noexec_entries[] =
{
    {
        MMU_TLB_ENTRY(0,0x00000,0x00000,MMU_TLBE_DSIZ_1GB,
            1,1,0,1,0,0, MMU_TLBE_E_BIG_END,
        /*  UX,UW,UR, SX,SW,SR  */
            0, 0, 0,  0, 1, 1, 0,0,0, MEM_WINDOW_0,
            MMU_TLBWE_WAY_3, MMU_TLBWE_BE_UND,1)
    }
};
inline static uint32_t reverse_bytes32(uint32_t data)
{
    return  ((data & 0xFF000000) >> 0x18) |
            ((data & 0x00FF0000) >> 0x08) |
            ((data & 0x0000FF00) << 0x08) |
            ((data & 0x000000FF) << 0x18) ;
}

inline static void reset_interrupts(void)
{
    global_irr = 0x00000000;
    IRQ        = 0x00000000;
}

/* NOINLINE */
uint32_t emi_read(uint32_t emi_addr)
{
    return dcr_read(EMIA(emi_addr));
}

/* NOINLINE */
void emi_write(uint32_t emi_addr, uint32_t emi_data)
{
    dcr_write(EMIA(emi_addr),(emi_data));
}

uint32_t emi_wait_irq(uint32_t usecs, uint32_t irq_mask)
{
    volatile
    uint32_t    tmp;

    tmp = _GET_UPTIME();
    rumboot_printf("Wait for interrupt...\n");
    while(((_GET_UPTIME() - tmp) < (usecs)) && !(IRQ & irq_mask));
    if(!((_GET_UPTIME() - tmp) < (usecs)))
        rumboot_printf("IRQ TIMEOUT!\n");

    return usecs - (_GET_UPTIME() - tmp);
}

void emi_irq_handler( int irq, void *arg )
{
    rumboot_printf("IRQ #%d received (Return address: 0x%X).\n",
            irq, spr_read(SPR_SRR0));
    if(irq < 32) irq = 32;  /* trim IRQ number */
    if(irq > 63) irq = 63;  /* trim IRQ number */
    IRQ |= (1 << (irq - 32));
    global_irr = emi_read(EMI_IRR);
    emi_write(EMI_IRR_RST, IRR_RST_ALL);    /* Reset interrupts */
}

static void ex_handler(int exid, const char *exname)
{
    rumboot_printf(
            "\n >>> EXCEPTION #%d (%s) at 0x%X with MCSR=0x%X! <<<\n",
            exid, exname, spr_read(SPR_MCSRR0), spr_read(SPR_MCSR_RW));
    spr_write(SPR_MCSR_C, ~0);
    l2c_l2_write(DCR_L2C_BASE, L2C_L2PLBSTAT1, ~0);
}

void init_interrupts(void)
{
    uint32_t    *irq_item;
    static
    uint32_t    irq_list[] =
    {
        EMI_CNTR_INT_0,
        EMI_CNTR_INT_1,
        EMI_CNTR_INT_2,
        EMI_CNTR_INT_3,
        ~0
    };
    rumboot_putstring ("\tStart IRQ initialization...\n");

    IRQ = 0;
    rumboot_irq_cli();
    irq_table = rumboot_irq_create( NULL );
    FOREACH_IRQ(irq_item, irq_list)
        rumboot_irq_set_handler(irq_table, *irq_item,
            IRQ_FLAGS, emi_irq_handler, NULL);

    /* Activate the table */
    rumboot_irq_table_activate(irq_table);
    FOREACH_IRQ(irq_item, irq_list)
        rumboot_irq_enable(*irq_item);
    rumboot_irq_sei();
    /* Unmask all interrupts */
    emi_write(EMI_IMR, IMR_UNMASK_ALL);
    // spr_write(SPR_SPRG7, 0);
    rumboot_irq_set_exception_handler(ex_handler);

    rumboot_putstring ("\tIRQ have been initialized.\n");
}

NOINLINE
void emi_read_ckregs(volatile emi_ckregs_t *regs, uint32_t bank)
{
    regs->eccval = emi_read(EMI_ECCRDR);
    regs->ecnt20 = emi_read(EMI_ECNT20);
    regs->ecnt53 = emi_read(EMI_ECNT53);
    regs->h1addr = emi_read(EMI_H1ADR );
    regs->h2addr = emi_read(EMI_H2ADR );
    regs->h1cmrr = emi_read(EMI_H1CMR );
    regs->h2cmrr = emi_read(EMI_H2CMR );
    regs->cntval = GET_ECNT(bank, regs->ecnt20, regs->ecnt53);
}

NOINLINE
void emi_clear_ckregs(int bank)
{
    if(bank + 1) /* is not -1 */
        emi_write(emi_ecnt[bank], 0x00000000);
    emi_write(EMI_H1ADR, HXADR_DFLT);
    emi_write(EMI_H2ADR, HXADR_DFLT);
    emi_write(EMI_H1CMR, 0x00000000);
    emi_write(EMI_H2CMR, 0x00000000);

}

/* Make single error */
uint32_t check_emi_mkserr(  mke_info_t     *info,
                            mkerr_param_t   params)
{
    uint32_t     status   = TEST_OK,
                 result   = 0;
    uintptr_t    base32   = 0,
                 base8    = 0;
    volatile
    emi_ckregs_t regs;
    volatile
    uint32_t     readed   = 0,
                 tmp      = 0;

    base32  = emi_bank_bases[params.bnk];
    base32 += (BASE_OFFSET + (info->idx * sizeof(uintptr_t)));
    base8  += base32 + info->idx;
    tmp    ^= 0;  /* Prevent compiler unused var warning */

    reset_interrupts();

    rumboot_printf("Writing 0x%X to 0x%X...\n",
            info->val ^ info->err, base32);
    iowrite32(info->val ^ info->err, base32);
    msync();
    // tmp = emi_read(EMI_WECR);       /* EMI Sync */
    emi_write(EMI_WECR, emi_read(EMI_WECR) | (1 << params.bwe));
    rumboot_printf("Writing 0x%X to 0x%X...\n", info->val, base32);
    iowrite32(info->val, base32);
    msync();
    // tmp = emi_read(EMI_WECR);       /* EMI Sync */
    emi_write(EMI_WECR, WECR_DFLT);
    rumboot_printf("Reading from 0x%X...\n", base32);
    readed = ioread32(base32);
    msync();
    udelay(50);
    emi_wait_irq(5000, ALL_INTERRUPTS);
    emi_read_ckregs(&regs, (uint32_t)params.bnk);
    rumboot_printf("ECNT20=0x%X, ECNT53=0x%X, H1ADR=0x%X, H2ADR=0x%X, H1CMR=0x%X, H2CMR=0x%X\n",
            regs.ecnt20, regs.ecnt53, regs.h1addr, regs.h2addr, regs.h1cmrr, regs.h2cmrr);
    rumboot_printf("READED: 0x%X (ECC=0x%X, ECNT=%d) after single error.\n",
            readed, regs.eccval, regs.cntval);
    TEST_ASSERT(result = (readed == info->exp),
            "EMI: Single error repair fails.");
    status |= !result;
    TEST_ASSERT(result = (regs.eccval == (uint32_t)params.ecc),
            "EMI: Wrong ECC-code after single error.");
    status |= !result;
    TEST_ASSERT(result = (regs.cntval == (uint32_t)params.cnt),
            "EMI: Wrong single error counter value.");
    status |= !result;
    TEST_ASSERT(result = (global_irr & SINGLE_ERR_FLAG(params.bnk)),
            "EMI: Single error interrupt flag not risen.");
    status |= !result;
    TEST_ASSERT(result = (regs.h1addr == (base32 & ERR_ADDR_MASK)),
            "EMI: Wrong value of H1ADR after single error.");
    status |= !result;
    TEST_ASSERT(result = (regs.h2addr == HXADR_DFLT),
            "EMI: Wrong value of H2ADR after single error.");
    status |= !result;

    return status;
}

/* Make double error */
uint32_t check_emi_mkderr(  mke_info_t     *info,
                            mkerr_param_t   params)
{
    uint32_t     status   = TEST_OK,
                 result   = 0;
    uintptr_t    base32   = 0,
                 base8    = 0;
    volatile
    emi_ckregs_t regs;
    volatile
    uint32_t     readed   = 0,
                 tmp      = 0;

    base32  = emi_bank_bases[params.bnk];
    base32 += (BASE_OFFSET + (info->idx * sizeof(uintptr_t)));
    base8  += base32 + info->idx;
    tmp    ^= 0; /* Prevent compiler unused var warning */

    reset_interrupts();

    rumboot_printf("Writing 0x%X to 0x%X...\n",
            info->val ^ info->err, base32);
    iowrite32(info->val ^ info->err, base32);
    msync();
    // tmp = emi_read(EMI_WECR);       /* EMI Sync */
    emi_write(EMI_WECR, emi_read(EMI_WECR) | (1 << params.bwe));
    rumboot_printf("Writing 0x%X to 0x%X...\n", info->val, base32);
    iowrite32(info->val, base32);
    msync();
    // tmp = emi_read(EMI_WECR);       /* EMI Sync */
    emi_write(EMI_WECR, WECR_DFLT);
    rumboot_printf("Reading from 0x%X...\n", base32);
    readed = ioread32(base32);
    msync();
    udelay(50);
    emi_wait_irq(5000, ALL_INTERRUPTS);
    emi_read_ckregs(&regs, (uint32_t)params.bnk);
    rumboot_printf("ECNT20=0x%X, ECNT53=0x%X, H1ADR=0x%X, H2ADR=0x%X, H1CMR=0x%X, H2CMR=0x%X\n",
            regs.ecnt20, regs.ecnt53, regs.h1addr, regs.h2addr, regs.h1cmrr, regs.h2cmrr);
    rumboot_printf("READED: 0x%X (ECC=0x%X, ECNT=%d) after single error.\n",
            readed, regs.eccval, regs.cntval);
    TEST_ASSERT(result = (readed == info->exp),
            "EMI: Wrong value after double error.");
    status |= !result;
    TEST_ASSERT(result = (regs.eccval == (uint32_t)params.ecc),
            "EMI: Wrong ECC-code after double error.");
    status |= !result;
    TEST_ASSERT(result = (regs.cntval == (uint32_t)params.cnt),
            "EMI: Wrong single error counter value.");
    status |= !result;
    TEST_ASSERT(result = (!(global_irr & SINGLE_ERR_FLAG(params.bnk))),
            "EMI: Unexpected single error interrupt flag risen.");
    status |= !result;
    TEST_ASSERT(result = (global_irr & DOUBLE_ERR_FLAG(params.bnk)),
            "EMI: Double error interrupt flag not risen.");
    status |= !result;
    TEST_ASSERT(result = (regs.h1addr == HXADR_DFLT),
            "EMI: Wrong value of H1ADR after double error.");
    status |= !result;
    TEST_ASSERT(result = (regs.h2addr == (base32 & ERR_ADDR_MASK)),
            "EMI: Wrong value of H2ADR after double error.");
    status |= !result;

    return status;
}

uint32_t check_emi(uint32_t bank)
{
    int              i;
    uint32_t         status   = TEST_OK,
                     result   = 0,
                     hstsr    = 0,
                     data32   = 0;
    uintptr_t        base     = emi_bank_bases[bank];
    int              typ      = EMI_TYPE(bank);
    mke_info_t       mke_info = {0,0,0,0};

    mkerr_param_t   mke_d1[] =
    {
        {bank, const_2_ecc[0], 1, EMI_BITS_D0_D7  },
        {bank, const_2_ecc[0], 1, EMI_BITS_D8_D15 },
        {bank, const_2_ecc[0], 1, EMI_BITS_D16_D23},
        {bank, const_2_ecc[0], 1, EMI_BITS_D24_D31},
    },
                    mke_e1[] =
    {
        {bank, const_2_ecc[0], 2, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[0], 2, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[0], 2, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[0], 2, EMI_BITS_ED0_ED7},
    },
                     mke_d2[] =
    {
        {bank, const_2_ecc[0], 0, EMI_BITS_D0_D7  },
        {bank, const_2_ecc[0], 0, EMI_BITS_D8_D15 },
        {bank, const_2_ecc[0], 0, EMI_BITS_D16_D23},
        {bank, const_2_ecc[0], 0, EMI_BITS_D24_D31},
    },
                     mke_e2[] =
    {
        {bank, const_2_ecc[1], 0, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[1], 0, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[1], 0, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[1], 0, EMI_BITS_ED0_ED7},
    };

    if(typ & ~7) typ = 7;
    rumboot_printf(" *** CHECK BANK #%d (type %d=%s) *** \n",
            bank, typ, btyp_descr[typ]);

    /* Force bank switch */
    rumboot_printf("Writing 0x%X to 0x%X...\n", 0x00000000, base);
    iowrite32(0x00000000, base);
    rumboot_printf("Try to read from 0x%X...\n", base);
    data32 = ioread32(base);
    msync();

    emi_write(EMI_WECR,  WECR_DFLT);
    hstsr = emi_read(EMI_HSTSR);
    if(!GET_BIT(hstsr, bank))
    {
        ECC_ON(bank);
        iowrite32(0x00000000, base);
    }

    base += BASE_OFFSET;

    /* Reset single error counter */
    emi_write(emi_ecnt[bank], 0);

    /* Store32 */
    for(i = 0; i < 4; i++)
    {
        rumboot_printf(" +++ Store word 0x%X to 0x%X...\n",
                long_tpl[i], base + i * sizeof(uintptr_t));
        iowrite32(long_tpl[i], base + i * sizeof(uintptr_t));
    }

    /* Store8 */
    for(i = 0; i < 4; i++)
    {
        rumboot_printf(" +++ Store byte: 0x%X to 0x%X\n",
                byte_tpl[i], base + i * sizeof(uintptr_t) + i);
        iowrite8(byte_tpl[i], base + i * sizeof(uintptr_t) + i);
    }

    /* Load32 */
    for(i = 0; i < 4; i++)
    {
        uintptr_t   addr = (base + (i * sizeof(uintptr_t))),
                    expv = long_tpl[i] ^ mask_tpl[i];
        rumboot_printf(" +++ Load word from 0x%X...\n", long_tpl[i], addr);
        data32 = ioread32(addr);
        rumboot_printf("Readed 0x%X, from 0x%X\n", data32, addr);
        CMP_EQ(data32, expv, result, status);
        rumboot_printf("%d: At 0x%X, Expected: 0x%X, Actual: 0x%X -> %s\n",
                i, addr, expv, data32,
                !result ? "ok" : "error");
        TEST_ASSERT(data32 == expv, "READ ERROR");
    }
    /* --------------------------------------------------- */

    rumboot_printf("---------------------------------------------------\n");

    /* Mod1 Data */
    for(i = 0; i < 4; i++)
    {
        mke_info.idx = i;
        mke_info.val = CONST_2;
        mke_info.exp = CONST_2;
        mke_info.err = MK_ERR_1 << (i << 3);
        rumboot_printf(" *** A+%d Make single error in data... \n", i);
        emi_clear_ckregs(bank);
        status |= check_emi_mkserr(&mke_info,
                                    mke_d1[i]);
    }

    /* Mod2 Data */
    for(i = 0; i < 4; i++)
    {
        mke_info.idx = i;
        mke_info.val = CONST_2;
        mke_info.exp = CONST_2 ^ (MK_ERR_2 << (i << 3));
        mke_info.err = MK_ERR_2 << (i << 3);
        rumboot_printf(" *** A+%d Make single error in ECC...\n", i);
        emi_clear_ckregs(-1);
        status |= check_emi_mkserr(&mke_info,
                                    mke_d2[i]);
    }

    /* Mod1 ECC */
    for(i = 0; i < 4; i++)
    {
        mke_info.idx = i;
        mke_info.val = CONST_2;
        mke_info.exp = CONST_2 ^ MK_ERR_1;
        mke_info.err = MK_ERR_1;
        rumboot_printf(" *** A+%d Make double error in data...\n", i);
        emi_clear_ckregs(bank);
        status |= check_emi_mkderr(&mke_info,
                                    mke_e1[i]);
    }

    /* Mod2 ECC */
    for(i = 0; i < 4; i++)
    {
        mke_info.idx = i;
        mke_info.val = CONST_2;
        mke_info.exp = CONST_2;
        mke_info.err = MK_ERR_2;
        rumboot_printf(" *** A+%d Make double error in ECC...\n", i);
        emi_clear_ckregs(-1);
        status |= check_emi_mkderr(&mke_info,
                                    mke_e2[i]);
    }

    reset_interrupts();

    emi_write(EMI_HSTSR, hstsr);
    rumboot_printf("%s!\n", !status ? "success" : "failed");

    return status;
}

uint32_t main(void)
{
    uint32_t     test_result    = TEST_OK;
    int          mem_bank;

    rumboot_printf("Init EMI...\n");
    write_tlb_entries(em2_noexec_entries, 1);
    tlbsync();
    msync();
    isync();
    emi_init(DCR_EM2_EMI_BASE);
    /* emi_init_impl (DCR_EM2_EMI_BASE, DCR_EM2_PLB6MCIF2_BASE, 0x00); */
    init_interrupts();
    rumboot_printf("Init done.\n");

    rumboot_printf("Generate ECC codes...\n");
    const_2_ecc[0] = calc_hamming_ecc(reverse_bytes32(CONST_2           ));
    const_2_ecc[1] = calc_hamming_ecc(reverse_bytes32(CONST_2 ^ MK_ERR_2));
    rumboot_printf("Generated.\n");

    /* test_result |= check_emi(4); */
    FOREACH_BANK(mem_bank, FIRST_BANK, LAST_BANK)
    {
        rumboot_printf(" ------------ Bank #%d... ------------\n", mem_bank);
        if(ENABLED_MEM_TYPES & (1 << EMI_TYPE(mem_bank)))
            test_result |= check_emi(mem_bank);
        else
            rumboot_printf("This bank is disabled! Skipping it.\n");
        rumboot_printf(" ------------------------------------\n");
    }

    rumboot_putstring(!test_result ? "TEST OK!\n":"TEST_ERROR!\n");

    return test_result;
}

