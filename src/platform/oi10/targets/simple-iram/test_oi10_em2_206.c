
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

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <devices/ugly/plb6mcif2.h>
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
#define COMMON_EMI_REGS             (-1)

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
#define SHIFT_ERR(ERR,IDX)          ((ERR) << ((IDX) << 3))
#define GEN_ECC(VAL)                calc_hamming_ecc(reverse_bytes32(VAL))
#define ECC_ON(BANK)                emi_write(EMI_HSTSR, \
                                        emi_read(EMI_HSTSR) |  (1 << (BANK)));
#define ECC_OFF(BANK)               emi_write(EMI_HSTSR, \
                                        emi_read(EMI_HSTSR) & ~(1 << (BANK)));

#define CMP_EQ(A1,A2,RS,ST)         ((ST) |= (RS = !((A1) == (A2))))
#define CMP_NE(A1,A2,RS,ST)         ((ST) |= (RS = !((A1) != (A2))))
/* (B)ank, (F)irst, (L)ast */
#define FOREACH_BANK(B,F,L)         for(B = F; B <= L; B++)
#define FOREACH_IRQ(ITEM,LIST)      for(ITEM = LIST; ~*ITEM; ITEM++)
#define MOD_BYTE(WORD,IDX,BYTE)     ((uint8_t*)(&(WORD)))[(IDX)] = (BYTE)

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
static char *btyp_descr[8] =
{
/*   0           1           2           3          */
    "SRAM",     "NOR",      "SSRAM",    "UNKNOWN3",
/*   4           5           6           7          */
    "PIPELINE", "UNKNOWN5", "SDRAM",    "UNKNOWN7"
};
  /* Check templates 32 bits */
static const uint32_t    long_tpl[] =
{
    0xDEBA0001 ^ 0xFF000000,
    0xDEDA0001 ^ 0x00FF0000,
    0xDEDA0101 ^ 0x0000FF00,
    0xDEDA0100 ^ 0x000000FF
};
static const uint32_t    mask_tpl[] =
{
    0xFF000000,
    0x00FF0000,
    0x0000FF00,
    0x000000FF
};
static const uint32_t mke1_4_ecc[4] =
{  /* ECC 0x11 (0x10 ^ 0x01) */
    0xF00D4B25,
    0xF00DBAEE,
    0xF0FC4BEE,
    0xF70D4BEE
};
static const uint32_t mke2_4_ecc[4] =
{  /* ECC 0x54 (0x10 ^ 0x44) */
    0xF00D4B2B,
    0xF00DC9EE,
    0xF0D04BEE,
    0xF71D4BEE
};
/* Check templates 8 bits */
uint8_t     byte_tpl1[] = {0xDE, 0xDA, 0x01, 0x00};
uint8_t     byte_tpl2[] = {0x55, 0xAA, 0x33, 0xCC};
static volatile const uint8_t emi_ecnt_b[8] = {0,8,16,0,8,16,0,0};
static volatile const uint8_t emi_ecnt[8] =
    {
        EMI_ECNT20, EMI_ECNT20, EMI_ECNT20,
        EMI_ECNT53, EMI_ECNT53, EMI_ECNT53, 0,0
    };
static irq_flags_t IRQ = 0;
static rumboot_irq_entry *irq_table = NULL;
static volatile uint32_t global_irr = 0;
static uint8_t const_2_ecc[16];
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

static void init_interrupts(void)
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
    rumboot_putstring ("Init IRQ...\n");

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

    rumboot_putstring ("IRQ initialized\n");
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
    uint32_t     readed   = 0,
                 tmp      = 0;
    volatile
    emi_ckregs_t regs1, regs2;

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
    rumboot_printf("Writing byte 0x%X to 0x%X...\n",
            byte_tpl2[info->idx], base8);
    iowrite8(byte_tpl2[info->idx], base8);
    msync();
    emi_wait_irq(200, ALL_INTERRUPTS);
    emi_read_ckregs(&regs1, (uint32_t)params.bnk);
    rumboot_printf("Reading from 0x%X...\n", base32);
    readed = ioread32(base32);
    // msync();
    emi_read_ckregs(&regs2, (uint32_t)params.bnk);
    rumboot_printf("ECNT20=0x%X, ECNT53=0x%X, H1ADR=0x%X, H2ADR=0x%X, H1CMR=0x%X, H2CMR=0x%X\n",
            regs1.ecnt20, regs1.ecnt53, regs1.h1addr, regs1.h2addr, regs1.h1cmrr, regs1.h2cmrr);
    rumboot_printf("READED: 0x%X, EXP: 0x%X (ECC=0x%X/0x%X, ECNT=%d) after single error.\n",
            readed, info->exp, regs1.eccval, regs2.eccval, regs1.cntval);
    TEST_ASSERT(result = (readed == info->exp),
            "EMI: Single error repair fails.");
    status |= !result;
    TEST_ASSERT(result = (regs1.eccval == (uint32_t)params.ecc),
            "EMI: Wrong ECC-code after single error.");
    status |= !result;
    TEST_ASSERT(result = (regs1.cntval == (uint32_t)params.cnt),
            "EMI: Wrong single error counter value.");
    status |= !result;
    TEST_ASSERT(result = (global_irr & SINGLE_ERR_FLAG(params.bnk)),
            "EMI: Single error interrupt flag not risen.");
    status |= !result;
    TEST_ASSERT(result = (regs1.h1addr == (base32 & ERR_ADDR_MASK)),
            "EMI: Wrong value of H1ADR after single error.");
    status |= !result;
    TEST_ASSERT(result = (regs1.h2addr == HXADR_DFLT),
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
    uint32_t     readed   = 0,
                 tmp      = 0;
    volatile
    emi_ckregs_t regs1, regs2;

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
    rumboot_printf("Writing byte 0x%X to 0x%X...\n",
            byte_tpl2[info->idx], base8);
    iowrite8(byte_tpl2[info->idx], base8);
    msync();
    emi_wait_irq(200, ALL_INTERRUPTS);
    emi_read_ckregs(&regs1, (uint32_t)params.bnk);
    rumboot_printf("Reading from 0x%X...\n", base32);
    readed = ioread32(base32);
    // msync();
    emi_read_ckregs(&regs2, (uint32_t)params.bnk);
    rumboot_printf("ECNT20=0x%X, ECNT53=0x%X, H1ADR=0x%X, H2ADR=0x%X, H1CMR=0x%X, H2CMR=0x%X\n",
            regs1.ecnt20, regs1.ecnt53, regs1.h1addr, regs1.h2addr, regs1.h1cmrr, regs1.h2cmrr);
    rumboot_printf("READED: 0x%X, EXP: 0x%X (ECC=0x%X/0x%X, ECNT=%d) after double error.\n",
            readed, info->exp, regs1.eccval, regs2.eccval, regs1.cntval);
    TEST_ASSERT(result = (readed == info->exp),
            "EMI: Wrong value after double error.");
    status |= !result;
    TEST_ASSERT(result = (regs1.eccval == (uint32_t)params.ecc),
            "EMI: Wrong ECC-code after double error.");
    status |= !result;
    TEST_ASSERT(result = (regs1.cntval == (uint32_t)params.cnt),
            "EMI: Wrong single error counter value.");
    status |= !result;
    TEST_ASSERT(result = (!(global_irr & SINGLE_ERR_FLAG(params.bnk))),
            "EMI: Unexpected single error interrupt flag risen.");
    status |= !result;
    TEST_ASSERT(result = (global_irr & DOUBLE_ERR_FLAG(params.bnk)),
            "EMI: Double error interrupt flag not risen.");
    status |= !result;
    TEST_ASSERT(result = (regs1.h1addr == HXADR_DFLT),
            "EMI: Wrong value of H1ADR after double error.");
    status |= !result;
    TEST_ASSERT(result = (regs1.h2addr == (base32 & ERR_ADDR_MASK)),
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

    mkerr_param_t   mke_d1[4] =
    {   /* Mod1 data */
        {bank, const_2_ecc[0x08], 1, EMI_BITS_D24_D31},
        {bank, const_2_ecc[0x08], 2, EMI_BITS_D16_D23},
        {bank, const_2_ecc[0x08], 3, EMI_BITS_D8_D15 },
        {bank, const_2_ecc[0x08], 4, EMI_BITS_D0_D7  },
    },
                    mke_d2[4] =
    {   /* Mod2 data */
        {bank, const_2_ecc[0x08], 0, EMI_BITS_D24_D31},
        {bank, const_2_ecc[0x08], 0, EMI_BITS_D16_D23},
        {bank, const_2_ecc[0x08], 0, EMI_BITS_D8_D15 },
        {bank, const_2_ecc[0x08], 0, EMI_BITS_D0_D7  },
    },
                    mke_e1[4] =
    {   /* Mod1 ECC */
        {bank, const_2_ecc[0x09], 1, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[0x09], 2, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[0x09], 3, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[0x09], 4, EMI_BITS_ED0_ED7},
    },
                     mke_e2[4] =
    {   /* Mod2 ECC */
        {bank, const_2_ecc[0x0A], 0, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[0x0A], 0, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[0x0A], 0, EMI_BITS_ED0_ED7},
        {bank, const_2_ecc[0x0A], 0, EMI_BITS_ED0_ED7},
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
                byte_tpl1[i], base + i * sizeof(uintptr_t) + i);
        iowrite8(byte_tpl1[i], base + i * sizeof(uintptr_t) + i);
    }

    /* Load32 */
    for(i = 0; i < 4; i++)
    {
        uintptr_t   addr = (base + (i * sizeof(uintptr_t))),
                    expv = long_tpl[i] ^ mask_tpl[i];
        rumboot_printf(" +++ Load word from 0x%X...\n", addr);
        data32 = ioread32(addr);
        rumboot_printf("Readed 0x%X, from 0x%X\n", data32, addr);
        CMP_EQ(data32, expv, result, status);
        rumboot_printf("%d: At 0x%X, Expected: 0x%X, Actual: 0x%X -> %s\n",
                i, addr, expv, data32,
                !result ? "ok" : "error");
        TEST_ASSERT(data32 == expv, "READ ERROR");
    }

    rumboot_printf("-----------------------------------\n");

    /* Mod1 Data */
    rumboot_printf(" === Make single error in data === \n");
    for(i = 0; i < 4; i++)
    {
        mke_info.idx = i;
        mke_info.val = CONST_2;
        mke_info.exp = CONST_2;
        mke_info.err = SHIFT_ERR(MK_ERR_1, i);
        MOD_BYTE(mke_info.exp, i, byte_tpl2[i]);
        rumboot_printf(" *** Mod1 data A+%d... \n", i);
        emi_clear_ckregs(COMMON_EMI_REGS);
        status |= check_emi_mkserr(&mke_info, mke_d1[i]);
        rumboot_printf(" ================================= \n");
    }

    /* Mod2 data */
    rumboot_printf(" === Make double error in data === \n");
    for(i = 0; i < 4; i++)
    {
        mke_info.idx = i;
        mke_info.val = CONST_2;
        mke_info.exp = CONST_2 ^ SHIFT_ERR(MK_ERR_2, i);
        mke_info.err = SHIFT_ERR(MK_ERR_2, i);
        MOD_BYTE(mke_info.exp, i, byte_tpl2[i]);
        rumboot_printf(" *** Mod2 data A+%d...\n", i);
        emi_clear_ckregs(bank);
        status |= check_emi_mkderr(&mke_info, mke_d2[i]);
        rumboot_printf(" ================================= \n");
    }

    /* Mod1 ECC */
    rumboot_printf(" === Make single error in ECC === \n");
    for(i = 0; i < 4; i++)
    {
        mke_info.idx = i;
        mke_info.val = CONST_2;
        mke_info.exp = CONST_2;
        mke_info.err = CONST_2 ^ mke1_4_ecc[i];
        MOD_BYTE(mke_info.exp, i, byte_tpl2[i]);
        rumboot_printf(" *** Mod1 ECC A+%d...\n", i);
        emi_clear_ckregs(COMMON_EMI_REGS);
        status |= check_emi_mkserr(&mke_info, mke_e1[i]);
        rumboot_printf(" ================================= \n");
    }

    /* Mod2 ECC */
    rumboot_printf(" === Make double error in ECC === \n");
    for(i = 0; i < 4; i++)
    {
        mke_info.idx = i;
        mke_info.val = CONST_2;
        mke_info.exp = CONST_2;
        mke_info.err = CONST_2 ^ mke2_4_ecc[i];
        MOD_BYTE(mke_info.exp, i, byte_tpl2[i]);
        rumboot_printf(" *** Mod2 ECC A+%d...\n", i);
        emi_clear_ckregs(bank);
        status |= check_emi_mkderr(&mke_info, mke_e2[i]);
        rumboot_printf(" ================================= \n");
    }

    reset_interrupts();

    emi_write(EMI_HSTSR, hstsr);
    rumboot_printf("%s!\n", !status ? "success" : "failed");

    return status;
}

static void init_emi() {
    rumboot_printf("Init EMI...\n");

    static struct tlb_entry const em2_nospeculative_tlb_entries[] = {
    /*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));

    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("EMI initialized\n");
}

static void generate_ecc_codes() {
    rumboot_printf("Generate ECC codes...\n");

    for(int i = 0; i < 4; i++)
    {
        const_2_ecc[0 + i] = GEN_ECC(
                mke1_4_ecc[i] | SHIFT_ERR(byte_tpl2[i], i));
        const_2_ecc[4 + i] = GEN_ECC(
                mke2_4_ecc[i] | SHIFT_ERR(byte_tpl2[i], i));
    }
    const_2_ecc[0x08] = GEN_ECC(CONST_2      );
    const_2_ecc[0x09] = GEN_ECC(mke1_4_ecc[0]);
    const_2_ecc[0x0A] = GEN_ECC(mke2_4_ecc[0]);
    const_2_ecc[0x0B] = 0x00;
    const_2_ecc[0x0C] = 0x00;
    const_2_ecc[0x0D] = 0x00;
    const_2_ecc[0x0E] = 0x00;
    const_2_ecc[0x0F] = 0xFF;

    rumboot_printf("ECC codes generated.\n");
}

uint32_t main(void)
{
    uint32_t     test_result    = TEST_OK;
    int          mem_bank;

    init_emi();
    init_interrupts();

    generate_ecc_codes();

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

    return test_result;
}

