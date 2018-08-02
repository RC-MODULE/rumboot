
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_lscb.h>
#include <platform/regs/fields/lscb_msp.h>

/* config */
#define USE_OFF_LINE_SELF_TEST      1
#define RTAD_VALUE                  0x001F
#define CHECK_MSP_REG_ADDR          MSP_RM_DSA
#define CHECK_MSP_REG_NAME          "rm_dsa"
#define LCSB_IRQ_TIMEOUT            10000 /* us */
#define DEBUG_MARK_SPR              SPR_SPRG8
#define IRQ_FLAGS                   (RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH)
/* ------ */

/* General constants, bits, and masks */
#define BIT(BN)                     (1 << (BN))

#define MKO_MEM_OFFSET              0x1000

#define TEST_OK                     0x0000
#define TEST_ERROR                  0x0001

#define MEM_TABLE_END               {0xFFFF, 0xFFFF}
#define REG_TABLE_END               {0xFFFF, 0xFFFF, NULL}

#define REG_DATA_BIT                0
#define REG_ADDR_BIT                16
#define REG_WR_RD_BIT               30
#define OP_RUN_BIT                  31
#define REG_DATA_MASK               0x0000FFFF
#define REG_ADDR_MASK               0x0FFF0000
#define REG_WR_RD_MASK              MASK_BIT(REG_WR_RD_BIT)
#define OP_RUN_MASK                 BIT(OP_RUN_BIT)
/* not in doc, for local usage only */
#define OP_RUN_RD                   (OP_RUN_MASK)
#define OP_RUN_WR                   (OP_RUN_MASK | BIT(REG_WR_RD_BIT))

#define MSP_SSFLAG_EXT_TRIG_BIT     8
#define MSP_SSFLAG_EXT_TRIG_MASK    BIT(MSP_SSFLAG_EXT_TRIG_BIT)

#define RTAD_BIT                    0
#define RTADP_BIT                   5
#define RTADP_MASK                  BIT(RTADP_BIT)
#define RTAD_MASK                   0x0000001F

/* Command word masks */
#define CMD_WORD_WC_MASK            0x001F
#define CMD_WORD_MC_MASK            0x001F
#define CMD_WORD_SA_MASK            0x03E0
#define CMD_WORD_TR_MASK            (1 << CMD_WORD_TR_i)
#define CMD_WORD_RT_A_MASK          0xF800
#define CW_RECV                     0x0000
#define CW_TRAN                     0x0001
#define CW_GRP_ADDR                 0x001F
#define CW_TEST_SUBADDR             0x001E

#define STACK_A_BASE                0x0000
#define STACK_B_BASE                0x0F00
#define MSG_BLOCK_BASE              0x0108
#define MSG_BLOCK_WORDS             ((0x012E)-(MSG_BLOCK_BASE))
#define CCSP_A                      0x0100
#define CMSG_A                      0x0101
#define ICSP_A                      0x0102
#define IMSG_A                      0x0103
#define CCSP_B                      0x0104
#define CMSG_B                      0x0105
#define ICSP_B                      0x0106
#define IMSG_B                      0x0107

typedef struct rumboot_irq_entry rumboot_irq_entry;

/* Data structures */

typedef struct
{
    uint32_t     reg_access;
    uint32_t     reg_msp_signal;
    uint32_t     not_used[MKO_MEM_OFFSET / sizeof(uint32_t) - 2];
    uint32_t     mem[4096];
} lscb_t;

typedef struct
{
    uint16_t     addr;
    uint16_t     data;
} a16d16_t;             /* memory init/check table entry */

typedef struct
{
    uint16_t     addr;
    uint16_t     data;
    char        *name;
} msp_reg_t;

/* Default values */

#define MSP_CFG1_VALUE          BIT(CFG1_IMGTE_i)       | \
                                BIT(CFG1_ETE_i)

#define MSP_CFG2_VALUE          BIT(CFG2_EI_i)          | \
                                BIT(CFG2_ISAC_i)        | \
                                BIT(CFG2_L_P_IR_i)

#define MSP_CFG3_VALUE          BIT(CFG3_EME_i)
#define MSP_CFG4_VALUE          BIT(CFG4_E_BC_CWE_i)
#define MSP_CFG6_VALUE          BIT(CFG6_ICOE_i)
#define MSP_IMR__VALUE          BIT(IMR_M_BC_EOF_i)     | \
                                BIT(IMR_M_EOM_i)

#define MSP_BCFT_VALUE          0x0010

#define MSP_CFG1_VALUE_A        MSP_CFG1_VALUE
#define MSP_CFG1_VALUE_B        MSP_CFG1_VALUE_A        | \
                                BIT(CFG1_CA_B_A_i)
                                /* Add more bits here */
#define CTL_WORD_VALUE          BIT(CTL_WORD_BF_i)      | \
                                (USE_OFF_LINE_SELF_TEST   \
                                ?BIT(CTL_WORD_OLST_i):0)

#define CTL_WORD_A_VALUE        CTL_WORD_VALUE          | \
                                BIT(CTL_WORD_BC_A_B_i)
#define CTL_WORD_B_VALUE        CTL_WORD_VALUE

#define LSCBX                   ((lscb_t*)(LSCB_X_BASE))

/* Macro functions (utilities) */

#define DEBUG_MARK(MARK)        spr_write(DEBUG_MARK_SPR, (MARK))
#define IDX32(ADDR)             ((ADDR) / sizeof(uint32_t))

#define LSCB_MEM_PHYSADDR(D,I)  /* (D)evice, (I)ndex of memory */           \
        (uint32_t)(&((D)->mem[(I)]))

#define MSG_COUNT(N)            (0xFFFE - (N))
#define STACK_A(IDX)            (STACK_A_BASE + (IDX))
#define STACK_B(IDX)            (STACK_B_BASE + (IDX))
#define MSGBLK(BLK,IDX)         (MSG_BLOCK_BASE + ((BLK) * MSG_BLOCK_WORDS + (IDX)))

#define REG_WR_REQ(ADDR,DATA)   ((((ADDR) << REG_ADDR_BIT) & REG_ADDR_MASK) \
                                | ((DATA) & REG_DATA_MASK)                  \
                                | OP_RUN_WR)
#define REG_RD_REQ(ADDR)        ((((ADDR) << REG_ADDR_BIT) & REG_ADDR_MASK) \
                                | OP_RUN_RD)
/* Command word builder:           A - Endpoint (A)ddr,
                                   T - (T)ransmit/recieve,
                                   S - (S)ubaddr/control mode,
                                   W - (W)ords of data count/control code */
#define CMD_WORD(A,T,S,W)       (((A) << CMD_WORD_RT_A_i) & CMD_WORD_RT_A_MASK) | \
                                (((T) << CMD_WORD_TR_i  ) & CMD_WORD_TR_MASK)   | \
                                (((S) << CMD_WORD_SA_i  ) & CMD_WORD_SA_MASK)   | \
                                (((W) << CMD_WORD_WC_i  ) & CMD_WORD_WC_MASK)

/* Message descriptor build */
#define MSG_DESC(B,I,T,A)       /* (B)ase, (I)ndex, (T)ime, (A)ddress */    \
                                {(B) + ((I) * 4 + 0),   0x0000},            \
                                {(B) + ((I) * 4 + 1),   0x0000},            \
                                {(B) + ((I) * 4 + 2),      (T)},            \
                                {(B) + ((I) * 4 + 3),      (A)}
#define START_OF_MESSAGE(DEV)   write_msp_reg(  (DEV), MSP_SRR,             \
                                                BIT(SRR_BC_MT_ST_i) |       \
                                                BIT(SRR_BT_SOM_i), "srr")

        /* MSP registers initialization table */
static msp_reg_t reg_init_table[]=
{
        {MSP_SRR,   BIT(SRR_RESET_i), "srr"  },
        {MSP_CFG3_, MSP_CFG3_VALUE,   "cfg3_"},
        {MSP_CFG4_, MSP_CFG4_VALUE,   "cfg4_"},
        {MSP_CFG1_, MSP_CFG1_VALUE_A, "cfg1_"},
        {MSP_CFG2_, MSP_CFG2_VALUE,   "cfg2_"},
        {MSP_CFG6_, MSP_CFG6_VALUE,   "cfg6_"},
        {MSP_BCFT,  MSP_BCFT_VALUE,   "bcft" },
        {MSP_IMR,   MSP_IMR__VALUE,   "imr"  },
        REG_TABLE_END
};

        /* LSCB memory initialization table */
static a16d16_t mem_init_table[] =
{
        /* Init stack pointers and other general stuff */
        {CCSP_A,        STACK_A_BASE},  /* Cmd stack ptr A              */
        {CMSG_A,        MSG_COUNT(2)},  /* Msg count A                  */
        {ICSP_A,        STACK_A_BASE},  /* Init val of cmd stack ptr A  */
        {IMSG_A,        MSG_COUNT(2)},  /* Init val of msg count A      */

        {CCSP_B,        STACK_B_BASE},  /* Cmd stack ptr B              */
        {CMSG_B,        MSG_COUNT(2)},  /* Msg count B                  */
        {ICSP_B,        STACK_B_BASE},  /* Init val of cmd stack ptr B  */
        {IMSG_B,        MSG_COUNT(2)},  /* Init val of msg count B      */

        /* Stack A */
        MSG_DESC(STACK_A_BASE, 0, 0x00DC, MSGBLK(2,0)),
        MSG_DESC(STACK_A_BASE, 1, 0x00DC, MSGBLK(3,0)),
        /* --- */

        /* Stack B */
        MSG_DESC(STACK_B_BASE, 0, 0x00DC, MSGBLK(4,0)),
        MSG_DESC(STACK_B_BASE, 1, 0x00DC, MSGBLK(5,0)),
        /* --- */

        /* Msg block #2 */
        {MSGBLK(2,0),   CTL_WORD_A_VALUE},
        {MSGBLK(2,1),   CMD_WORD(CW_GRP_ADDR,CW_RECV,1,1)},
        {MSGBLK(2,2),   0xA5A5},
        {MSGBLK(2,3),   0x0000},

        /* Msg block #3 */
        {MSGBLK(3,0),   CTL_WORD_A_VALUE},
        {MSGBLK(3,1),   CMD_WORD(CW_GRP_ADDR,CW_RECV,1,1)},
        {MSGBLK(3,2),   0x5A5A},
        {MSGBLK(3,3),   0x0000},

        /* Msg block #4 */
        {MSGBLK(4,0),   CTL_WORD_B_VALUE},
        {MSGBLK(4,1),   CMD_WORD(CW_GRP_ADDR,CW_RECV,1,1)},
        {MSGBLK(4,2),   0xCC33},
        {MSGBLK(4,3),   0x0000},

        /* Msg block #5 */
        {MSGBLK(5,0),   CTL_WORD_B_VALUE},
        {MSGBLK(5,1),   CMD_WORD(CW_GRP_ADDR,CW_RECV,1,1)},
        {MSGBLK(5,2),   0x33CC},
        {MSGBLK(5,3),   0x0000},

        MEM_TABLE_END
};

static a16d16_t mem_check_table_a_stack[] =
{
        /* Stack A */
        {STACK_A(0),    BIT(STATUS_WORD_EOM_i)},
        {STACK_A(2),    0x00DC},
        {STACK_A(3),    MSGBLK(2,0)},
        {STACK_A(4),    0x0000},
        {STACK_A(5),    0x0000},
        {STACK_A(6),    0x00DC},
        {STACK_A(7),    MSGBLK(3,0)},
        /* --- */

        /* Stack B */
        {STACK_B(0),    0x0000},
        {STACK_B(2),    0x00DC},
        {STACK_B(3),    MSGBLK(4,0)},
        {STACK_B(4),    0x0000},
        {STACK_B(5),    0x0000},
        {STACK_B(6),    0x00DC},
        {STACK_B(7),    MSGBLK(5,0)},
        /* --- */
        MEM_TABLE_END
};

static a16d16_t mem_check_table_b_stack[] =
{
        /* Stack A */
        {STACK_A(0),    BIT(STATUS_WORD_EOM_i)},
        {STACK_A(2),    0x00DC},
        {STACK_A(3),    MSGBLK(2,0)},
        {STACK_A(4),    0x0000},
        {STACK_A(5),    0x0000},
        {STACK_A(6),    0x00DC},
        {STACK_A(7),    MSGBLK(3,0)},
        /* --- */

        /* Stack B */
        {STACK_B(0),    BIT(STATUS_WORD_EOM_i) |
                        BIT(STATUS_WORD_CHANNEL_B_A_i)},
        {STACK_B(2),    0x00DC},
        {STACK_B(3),    MSGBLK(4,0)},
        {STACK_B(4),    0x0000},
        {STACK_B(5),    0x0000},
        {STACK_B(6),    0x00DC},
        {STACK_B(7),    MSGBLK(5,0)},
        /* --- */
        MEM_TABLE_END
};

static a16d16_t mem_check_table_a[] =
{
        /* Msg block #2 */
        {MSGBLK(2,0),   CTL_WORD_A_VALUE},
        {MSGBLK(2,1),   CMD_WORD(CW_GRP_ADDR,CW_RECV,1,1)},
        {MSGBLK(2,2),   0xA5A5},
        {MSGBLK(2,3),   0xA5A5},

        /* Msg block #3 */
        {MSGBLK(3,0),   CTL_WORD_A_VALUE},
        {MSGBLK(3,1),   CMD_WORD(CW_GRP_ADDR,CW_RECV,1,1)},
        {MSGBLK(3,2),   0x5A5A},
        {MSGBLK(3,3),   0x0000},

        MEM_TABLE_END
};

static a16d16_t mem_check_table_b[] =
{
        /* Msg block #4 */
        {MSGBLK(4,0),   CTL_WORD_B_VALUE},
        {MSGBLK(4,1),   CMD_WORD(CW_GRP_ADDR,CW_RECV,1,1)},
        {MSGBLK(4,2),   0xCC33},
        {MSGBLK(4,3),   0xCC33},

        /* Msg block #5 */
        {MSGBLK(5,0),   CTL_WORD_B_VALUE},
        {MSGBLK(5,1),   CMD_WORD(CW_GRP_ADDR,CW_RECV,1,1)},
        {MSGBLK(5,2),   0x33CC},
        {MSGBLK(5,3),   0x0000},

        MEM_TABLE_END
};

static uint32_t msp_fill_table[] =
{
        10,             /* items quantity */
        0xFFFF, 0x0000,
        0xFF00, 0x00FF,
        0xF0F0, 0x0F0F,
        0xCCCC, 0x3333,
        0xAAAA, 0x5555
};

static uint8_t rtad_fill_table[] =
{
        14,
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
        0x3F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F, 0x1F
};

static volatile uint32_t IRQ = 0;


void write_msp_reg(     lscb_t      *lscb,
                        MSP_REG      msp_addr,
                        uint32_t     msp_data,
                        char        *msp_reg_name )
{
    uint32_t reg_access;
    rumboot_printf("write to reg %s (A:0x%X; D:0x%X) \n",
        msp_reg_name, msp_addr, msp_data);
    lscb->reg_access = REG_WR_REQ(msp_addr, msp_data);
    while ((reg_access = lscb->reg_access) & OP_RUN_MASK)
        rumboot_printf("try reg_access: 0x%X\n", reg_access);
}

uint32_t read_msp_reg(  lscb_t      *lscb,
                        MSP_REG      msp_addr,
                        char        *msp_reg_name )
{
    uint32_t reg_access;
    lscb->reg_access = REG_RD_REQ(msp_addr);
    while ((reg_access = lscb->reg_access) & OP_RUN_MASK)
        rumboot_printf("try read %s: 0x%X\n",
                msp_reg_name ? msp_reg_name : "reg",
                reg_access);
    if(msp_reg_name)
        rumboot_printf("%s = 0x%X\n", msp_reg_name, reg_access);
    return reg_access;
}

void init_lscb_regs(lscb_t *lscb, msp_reg_t *rit_ptr)
{
    msp_reg_t    *rit = NULL; /* reg init table */
    for(rit = rit_ptr; (rit->addr) != 0xFFFF; rit++)
        write_msp_reg(lscb, rit->addr, rit->data, rit->name);
}


uint32_t check_msp_reg( lscb_t      *lscb,
                        uint32_t    *fill_tab,
                        MSP_REG      msp_addr,
                        char        *msp_reg_name )
{
    uint32_t    reg_access,
                reg_backup, /* Copy of previous content */
                ck_result,  /* Total check status       */
                ck_fail,    /* Single fail status       */
                tab_sz,
                idx;
    ck_result = TEST_OK;
    rumboot_printf("Checking reg %s ...\n", msp_reg_name);
    tab_sz = (*(fill_tab++)); /* First entry is an entries count */
    lscb->reg_access = REG_RD_REQ(msp_addr);
    while ((reg_access = lscb->reg_access) & OP_RUN_MASK)
        rumboot_printf("try read %s: 0x%X ...\n", msp_reg_name, reg_access);
    reg_backup = reg_access;
    for(idx = 0; idx < tab_sz; idx++)
    {
        lscb->reg_access = REG_WR_REQ(msp_addr, fill_tab[idx]);
        while ((reg_access = lscb->reg_access) & OP_RUN_MASK)
            rumboot_printf("try write to %s: 0x%X = 0x%X (0x%X) ...\n",
                    msp_reg_name, msp_addr, fill_tab[idx], reg_access);
        lscb->reg_access = REG_RD_REQ(msp_addr);
        while ((reg_access = lscb->reg_access) & OP_RUN_MASK)
            rumboot_printf("try read %s (0x%X): 0x%X ...\n",
                    msp_reg_name, reg_access);
        ck_result |= (ck_fail = (reg_access != fill_tab[idx]));
        if(ck_fail)
            rumboot_printf(
                    "fail: reg %s exp: 0x%X, val: 0x%X\n",
                    msp_reg_name, fill_tab[idx], reg_access);
    }
    lscb->reg_access = REG_WR_REQ(msp_addr, reg_backup);
    while ((reg_access = lscb->reg_access) & OP_RUN_MASK)
        rumboot_printf("try write (restore) to %s: 0x%X = 0x%X (0x%X) ...\n",
                msp_reg_name, msp_addr, reg_backup, reg_access);
    rumboot_putstring("done.\n");
    return ck_result;
}

uint32_t init_lscb_mem(lscb_t *lscb, a16d16_t *mit_ptr)
{
    uint32_t     wc  = 0;    /* words count */
    a16d16_t    *mit = NULL; /* memory init table */
    for(mit = mit_ptr; (mit->addr) != 0xFFFF; mit++,wc++)
        lscb->mem[mit->addr] = (uint32_t)mit->data;
    return wc;
}

uint32_t read_lscb_mem(lscb_t *lscb, uint32_t mem_addr)
{
    uint32_t mem_data = lscb->mem[mem_addr];
    rumboot_printf("Read mem 0x%X (0x%X): 0x%X\n",
            LSCB_MEM_PHYSADDR(lscb, mem_addr),
            mem_addr, mem_data);
    return mem_data;
}

uint32_t compare_lscb_mem(lscb_t *lscb, a16d16_t *mem_cmp_table)
{
    a16d16_t    *mct        = NULL;     /* Mem compare table */
    uint32_t     read_mem   = 0x0000,   /* Readed   value    */
                 expt_mem   = 0x0000,   /* Expected value    */
                 ck_result  = TEST_OK;

    for(mct = mem_cmp_table; (mct->addr) != 0xFFFF; mct++)
    {
        read_mem = lscb->mem[mct->addr];
        expt_mem = (uint32_t)mct->data;
        /*
        rumboot_printf("Check 0x%X at 0x%X...\n",
                (uint32_t)mct->data, (uint32_t)mct->addr);
        */
        if(read_mem != expt_mem)
        {
            ck_result |= TEST_ERROR;
            rumboot_printf("mem at 0x%X (0x%X) compare fail! EXP: 0x%X FACT: 0x%X\n",
                    LSCB_MEM_PHYSADDR(lscb, mct->addr),
                    (uint32_t)mct->addr,
                    expt_mem, read_mem);
        }
    }
    return ck_result;
}

uint32_t check_rms(lscb_t *lscb, uint8_t *ft)
{
    uint32_t     table_sz  = ((uint32_t)*ft++),
                 ck_result = TEST_OK,
                 reg_backup,
                 expv,
                 idx;
    volatile
    uint32_t    *reg = &(lscb->reg_msp_signal);

    reg_backup = *reg;
    for(idx = 0; idx < table_sz; idx++)
    {
        expv = (uint32_t)ft[idx];
        *reg = (uint32_t)ft[idx];
        ck_result |= (*reg != expv);
    }
    *reg = reg_backup;
    rumboot_printf("reg_msp_signal access test %s!\n",
            !ck_result?"SUCCESS":"FAILS");
    return ck_result;
}

uint32_t compare_cfg5_rtad(uint32_t cfg5_val, uint32_t signal_val)
{
    uint32_t    parsed_cfg5;
    parsed_cfg5 =   ((cfg5_val >> CFG5_RT_A_0_i << RTAD_BIT ) & RTAD_MASK ) |
                    ((cfg5_val >> CFG5_RT_A_P_i << RTADP_BIT) & RTADP_MASK);
    return (uint32_t)(parsed_cfg5 != (signal_val & (RTAD_MASK | RTADP_MASK)));
}

void lscb_irq_handler( int irq, void *arg )
{
    rumboot_printf("IRQ%d received!\n", irq);
    IRQ |= 0x00000001;
    read_msp_reg((lscb_t*)arg, MSP_ISR, "isr");
}

void init_lscb_irq(rumboot_irq_entry **irq_table, lscb_t *lscb)
{
    rumboot_putstring ("\tStart IRQ initialization...\n");

    IRQ = 0;
    rumboot_irq_cli();
    *irq_table = rumboot_irq_create( NULL );

    rumboot_irq_set_handler(*irq_table, LSCB_X_INT, IRQ_FLAGS, lscb_irq_handler, (void*)lscb);

    /* Activate the table */
    rumboot_irq_table_activate(*irq_table);
    rumboot_irq_enable( LSCB_X_INT );
    rumboot_irq_sei();
    rumboot_putstring ("\tIRQ have been initialized.\n");
}

void free_lscb_irq(rumboot_irq_entry **irq_table)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(*irq_table);
    *irq_table = NULL;
}

uint32_t wait_lscb_int(lscb_t *lscb, uint32_t *res)
{
    uint32_t    tbeg = rumboot_platform_get_uptime(),
                tend = 0;

    rumboot_putstring ("wait_lscb_int \n");
    while(
            (!IRQ) &&
            ((tend = rumboot_platform_get_uptime()) <= (tbeg + LCSB_IRQ_TIMEOUT))
    );

    if(tend > (tbeg + LCSB_IRQ_TIMEOUT))
    {
        rumboot_putstring("Error! IRQ wait timeout!\n");
        *res |= TEST_ERROR;
    }
    return *res;
}


/* ----------------- MAIN LSCB TEST --------------------- */

uint32_t test_lscb(lscb_t *lscb)
{
    uint32_t     test_result = TEST_OK;

#ifdef CHECK_REGS
    rumboot_putstring("------- check msp reg access -------- \n");
    if(!!(test_result = check_msp_reg(
            lscb,
            msp_fill_table,
            CHECK_MSP_REG_ADDR,
            CHECK_MSP_REG_NAME)))
        return TEST_ERROR;
#endif

    rumboot_printf("------- write config: 0x%X -------- \n",
            RTAD_VALUE);
    lscb->reg_msp_signal = RTAD_VALUE;

#ifdef CHECK_REGS
    rumboot_putstring("---- check reg_msp_signal access ---- \n");
    test_result |= check_rms(lscb, rtad_fill_table);
#endif

    rumboot_putstring("------------- init regs ------------- \n");
    init_lscb_regs(lscb, reg_init_table);

#ifdef CHECK_REGS
    rumboot_putstring("-------- read 'reg cfg5_' and ------- \n");
    rumboot_putstring("-------- compare msp_rtad[4:0] ------ \n");
    rumboot_putstring("-------- -- with msp_rtadp ---------- \n");
    test_result |= compare_cfg5_rtad(
            read_msp_reg(lscb, MSP_CFG5_, NULL),
            lscb->reg_msp_signal);
    rumboot_putstring(!test_result ? "Success!\n" : "Failed!\n");
#endif

    rumboot_putstring("------------- init mem -------------- \n");
    rumboot_printf("Total: %d words writen.\n",
            init_lscb_mem(lscb, mem_init_table));

    rumboot_putstring("------- Transfer message ch A ------- \n");
    START_OF_MESSAGE(lscb);

    wait_lscb_int(lscb, &test_result);

    rumboot_putstring("------- Check LSCB channel A -------- \n");

    test_result |= compare_lscb_mem(lscb, mem_check_table_a_stack);
    test_result |= compare_lscb_mem(lscb, mem_check_table_a);
    rumboot_printf("LSCB memory compare %s!\n",
            !test_result?"SUCCESS":"FAILED");

    rumboot_putstring("------- Check LSCB channel B -------- \n");

    /* Select channel B */
    write_msp_reg(lscb, MSP_CFG1_, MSP_CFG1_VALUE_B , "cfg1_");
    rumboot_putstring("------- Transfer message ch B ------- \n");
    START_OF_MESSAGE(lscb);

    if (wait_lscb_int(lscb, &test_result)) return TEST_ERROR;

    test_result |= compare_lscb_mem(lscb, mem_check_table_b_stack);
    test_result |= compare_lscb_mem(lscb, mem_check_table_b);
    rumboot_printf("LSCB memory compare %s!\n", !test_result?"SUCCESS":"FAILED");


    return !!test_result;
}

int main(void)
{
	uint32_t    test_result = TEST_OK;

    test_result = TEST_OK;

    rumboot_irq_entry *irq_table = NULL;
	init_lscb_irq(&irq_table, (void*)LSCBX);

    rumboot_putstring("------------ TEST LSCB -------------- \n");
	rumboot_putstring(!(test_result |= test_lscb(LSCBX)) ?
	        "TEST OK\n":"TEST ERROR\n");
    free_lscb_irq(&irq_table);
    return test_result;
}

