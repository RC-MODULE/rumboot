
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/regs/regs_lscb.h>
#include <platform/regs/fields/lscb_msp.h>

/* config */
#define test_read_ID
#define test_LSCB0
#define test_LSCB1
#define CHECK_MSP_REG_ADDR          MSP_RM_DSA
#define CHECK_MSP_REG_NAME          "rm_dsa"
#define DEBUG_MARK_SPR              SPR_SPRG8
/* ------ */

#define LSCB0_M0_IRQ_s              105
#define LSCB1_M1_IRQ_s              68

#define BIT(BN)                     (1 << (BN))

#define MKO_MEM_OFFSET              0x1000

#define LCSB_TIMEOUT                0x1000

#define TEST_OK                     0x0000
#define TEST_ERROR                  0x0001

#define MEM_FILL_END                {0xFFFF, 0xFFFF}

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

#define RTADP_BIT                   5
#define RTADP_MASK                  BIT(RTADP_BIT)
#define RTAD_MASK                   0x0000001F


typedef struct
{
    uint32_t    reg_access;
    uint32_t    reg_msp_signal;
    uint32_t    not_used[MKO_MEM_OFFSET / sizeof(uint32_t) - 2];
    uint32_t    mem[4096];
} lscb_t;

typedef struct
{
    uint16_t    addr;
    uint16_t    data;
} a16d16_t;             /* memory fill table entry */


#define MSP_CFG1_VALUE          BIT(CFG1_FAR_i)     | \
                                BIT(CFG1_IMGTE_i)   | \
                                BIT(CFG1_ITE_i)     | \
                                BIT(CFG1_ETE_i)     /* LAST */
#define MSP_CFG2_VALUE          BIT(CFG2_EI_i)
#define MSP_CFG3_VALUE          BIT(CFG3_EME_i)
#define MSP_BCFT_VALUE          0x0010
#define MSP_IMR__VALUE          BIT(IMR_M_BC_EOF_i) | \
                                BIT(IMR_M_EOM_i)    /* LAST */


#define LSCB0                   ((lscb_t*)(LSCB0_BASE))
#define LSCB1                   ((lscb_t*)(LSCB1_BASE))

#define DEBUG_MARK(MARK)        spr_write(DEBUG_MARK_SPR, (MARK))
#define IDX32(ADDR)             ((ADDR) / sizeof(uint32_t))

#define REG_WR_REQ(ADDR,DATA)   ((((ADDR) << REG_ADDR_BIT) & REG_ADDR_MASK) \
                                | ((DATA) & REG_DATA_MASK)                  \
                                | OP_RUN_WR)
#define REG_RD_REQ(ADDR)        ((((ADDR) << REG_ADDR_BIT) & REG_ADDR_MASK) \
                                | OP_RUN_RD)

static a16d16_t mem_fill_table[] =
{
/*      {0x0000,    0x8000}, */
/*      {0x0004,    0x0000}, */
        {0x0010,    0xA000},    /* ??? */

        {0x000C,    0x0154},
        {0x0008,    0x00DC},
        {0x001C,    0x0180},
        {0x0018,    0x00DC},

        {0x0550,    0x0082},    /* Msg block #2 */
        {0x0554,    0xF821},
        {0x0558,    0xA5A5},
        {0x055C,    0x0000},

        {0x0600,    0x0002},    /* Msg block #3 */
        {0x0604,    0xF821},
        {0x0608,    0x5A5A},
        {0x060C,    0x0000},

        {0x0100*4,  0x0000},    /* Cmd stack ptr A              */
        {0x0101*4,  0xFFFD},    /* Msg count A                  */
        {0x0102*4,  0x0000},    /* Init val of cmd stack ptr A  */
        {0x0103*4,  0xFFFD},    /* Init val of msg count A      */

        {0x0104*4,  0x0F00},    /* Cmd stack ptr A              */
        {0x0105*4,  0xFFFD},    /* Msg count A                  */
        {0x0106*4,  0x0000},    /* Init val of cmd stack ptr A  */
        {0x0107*4,  0xFFFD},    /* Init val of msg count A      */
        MEM_FILL_END
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

static volatile uint32_t IRQ;

/*
void NON_CR_INTERRUPT_HANDLER(void)
{
	rumboot_putstring ("NON_CR_INTERRUPT_HANDLER \n");
    uint32_t source_vector = mpic_get_ncr_interrupt_vector(Processor0);
    rumboot_putstring("NON_CRITICAL int handler message ");
    rumboot_puthex (source_vector);
    if (source_vector == LSCB0_M0_IRQ_s)
    {
        rumboot_putstring("source_vector == LSCB0_M0_IRQ_s \n");
        IRQ = 1;
    }
    if (source_vector == LSCB1_M1_IRQ_s)
    {
        rumboot_putstring("source_vector == LSCB1_M1_IRQ_s \n");
        IRQ = 1;
    }
	END_NONCR_INT_P0;
}
*/

int wait_lscb_int()
{
    int i=0;

    rumboot_putstring ("wait_lscb_int \n");

/*
    for (i = 1; i <= LCSB_TIMEOUT; i++)
    {
        if (IRQ == 1)
        {
            IRQ = 0;
            break;
        }
    }
    if ( i >=LCSB_TIMEOUT)
    {
        rumboot_putstring("Error! IRQ flag wait timeout!");
        return TEST_ERROR;
    }
*/

    return TEST_OK^i;
}

void lscb_irq_set()
{
    /* Initialization Interrupt controller */
	IRQ = 0;
    rumboot_putstring ("\tStart IRQ initialization...\n");
/*
    mpic_reset();
    mpic_pass_through_disable();
    mpic_setup_ext_interrupt(LSCB0_M0_IRQ_s, MPIC_PRIOR_1,int_sense_edge,int_pol_pos,Processor0);
    mpic_setup_ext_interrupt(LSCB1_M1_IRQ_s, MPIC_PRIOR_1,int_sense_edge,int_pol_pos,Processor0);
	SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
	ppc_noncr_int_enable();
	mpic_set_interrupt_borders(10, 5);  // MCK, CR borders
	mpic_set_minimal_interrupt_priority_mask(Processor0, MPIC_PRIOR_0);
*/
	rumboot_putstring ("\tIRQ have NOT been initialized (not implemented).\n");
}


uint32_t lscb_read_mem(lscb_t *lscb, uint32_t mem_addr)
{
    uint32_t mem_data = lscb->mem[IDX32(mem_addr)];
    DEBUG_MARK((mem_addr<<16 & 0x0FFF0000) | (mem_data&0xFFFF));
    rumboot_printf("Read mem 0x%X: 0x%X\n",
            (uint32_t)(&(lscb->mem[IDX32(mem_addr)])), mem_data);
    return mem_data;
}

void write_msp_reg(     lscb_t      *lscb,
                        MSP_REG      msp_addr,
                        uint32_t     msp_data,
                        char        *msp_reg_name )
{
    uint32_t reg_access;
    rumboot_printf("write to %s (A:0x%X; D:0x%X) \n",
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
    rumboot_printf("read reg %s ... \n", msp_reg_name);
    lscb->reg_access = REG_RD_REQ(msp_addr);
    while ((reg_access = lscb->reg_access) & OP_RUN_MASK)
        rumboot_printf("try reg_access: 0x%X\n", reg_access);
    rumboot_printf("%s = 0x%X\n", msp_reg_name, reg_access);
    return reg_access;
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

#if defined(test_LSCB0) && defined(test_LSCB1)
uint32_t test_lscb(lscb_t *lscb, int lscbn)
{
    a16d16_t    *mft = NULL; /* memory fill table */
    uint32_t     test_result = TEST_OK,
                 read_mem    = 0x0000,
                 read_config = 0x0000;

    rumboot_putstring("-------- check msp reg access -------- \n");

    if(!!(test_result = check_msp_reg(
            lscb,
            msp_fill_table,
            CHECK_MSP_REG_ADDR,
            CHECK_MSP_REG_NAME)))
        return TEST_ERROR;

    rumboot_printf("------ write config: 0x%X ----- \n",
            RTAD_MASK | MSP_SSFLAG_EXT_TRIG_MASK);
    lscb->reg_msp_signal = RTAD_MASK | MSP_SSFLAG_EXT_TRIG_MASK;
    msync();


    rumboot_putstring("------------ read config ------------ \n");
    read_config = lscb->reg_msp_signal;
    rumboot_printf(" read_config: 0x%X\n", read_config);

    rumboot_putstring("------------ write reg ------------- \n");
    write_msp_reg(lscb, MSP_SRR, BIT(SRR_RESET_i), "srr");
    rumboot_putstring("-- read reg cfg5_: test msp_rtad[4:0] & msp_rtadp -- \n");
    if(
        (read_msp_reg(lscb, MSP_CFG5_, "cfg5_") & (RTAD_MASK|RTADP_MASK)) !=
        (( (read_config << CFG5_RT_A_0_i) | (read_config & RTADP_MASK)
                ?BIT(CFG5_RT_A_P_i):0) & (RTAD_MASK|RTADP_MASK))
    ) test_result |= TEST_ERROR;
    write_msp_reg(lscb, MSP_CFG3_, MSP_CFG3_VALUE, "cfg3_");
    write_msp_reg(lscb, MSP_CFG1_, MSP_CFG1_VALUE, "cfg1_");
    write_msp_reg(lscb, MSP_CFG2_, MSP_CFG2_VALUE, "cfg2_");
    write_msp_reg(lscb, MSP_BCFT,  MSP_BCFT_VALUE, "bcft" );
    write_msp_reg(lscb, MSP_IMR,   MSP_IMR__VALUE, "imr"  );

    rumboot_putstring("------------ write mem ------------- \n");

    /* Fill memory */
    for(mft = mem_fill_table; (mft->addr) != 0xFFFF; mft++)
    {
        rumboot_printf("write mem 0x%X <- 0x%X\n",
                (uint32_t)(&(lscb->mem[IDX32(mft->addr)])), (uint32_t)mft->data);
        DEBUG_MARK(0xEEEE0000 | (0xFFFF & mft->data));
        lscb->mem[IDX32(mft->addr)] = (uint32_t)mft->data;
    }

    rumboot_putstring("------------ write reg ------------- \n");

    write_msp_reg(lscb, MSP_SRR, BIT(SRR_BC_MT_ST_i), "srr");

    /* !!!!!!!!!!!!!!!!! */
    rumboot_putstring("--- start: msp_ssflag_ext_trig 0->1 --- \n");
    DEBUG_MARK(RTAD_MASK);
    lscb->reg_msp_signal = RTAD_MASK;
    DEBUG_MARK(RTAD_MASK | MSP_SSFLAG_EXT_TRIG_MASK);
    lscb->reg_msp_signal = RTAD_MASK | MSP_SSFLAG_EXT_TRIG_MASK;
    DEBUG_MARK(0xFFFFFFFF);
    /* !!!!!!!!!!!!!!!!! */

    if (!!(test_result = wait_lscb_int())) return TEST_ERROR;

    DEBUG_MARK(0x7FFFFFFF);
    read_mem = lscb_read_mem(lscb, 0x0000);
    // if(read_mem != 0x8000) return TEST_ERROR;
    // if(read_mem != 0x8000) return TEST_ERROR;

    rumboot_printf("------------ read word loop_test LSCB%d ch A -------------- \n",
            lscbn);

    read_mem = lscb_read_mem(lscb, 0x0558);
    if(read_mem != 0xA5A5) return TEST_ERROR;
    read_mem = lscb_read_mem(lscb, 0x055C);
    if(read_mem != 0x0000) return TEST_ERROR;

    if(!!(test_result = wait_lscb_int())) return TEST_ERROR;

    /* WTF??? */
    read_mem = lscb_read_mem(lscb, 0x0004);

    read_mem = lscb_read_mem(lscb, 0x0010);
    if(read_mem != 0xA000) return TEST_ERROR;

    rumboot_printf("------------ read word loop_test LSCB%d ch B -------------- \n",
            lscbn);
    read_mem = lscb_read_mem(lscb, 0x0608);
    if(read_mem != 0x5A5A) return TEST_ERROR;
    read_mem = lscb_read_mem(lscb, 0x060C);
    if(read_mem != 0x0000) return TEST_ERROR;

    return !!test_result;
}
#endif

#ifdef test_read_ID
void test_read_id(lscb_t *lscb, int lscbn)
{
    rumboot_printf("---------- test_read_ID LSCB%d ---------- \n",
            lscbn);
    lscb->reg_msp_signal = RTAD_MASK;
    rumboot_printf("test_read_ID LSCB%d write=0x%X read=0x%X\n",
            RTAD_MASK, lscb->reg_msp_signal);
}
#endif

int main(void)
{
	uint32_t    test_result[2],
	            test_error;

    test_result[0] = TEST_OK;
    test_result[1] = TEST_OK;

#ifdef test_read_ID
    test_read_id(LSCB0, 0);
    test_read_id(LSCB1, 1);
#endif
		
	rumboot_putstring("---------- Start test LSCB ---------- \n");

	lscb_irq_set();

#ifdef test_LSCB0
    rumboot_putstring("------------ TEST LSCB0 ------------- \n");
	test_result[0] = test_lscb(LSCB0, 0);
	rumboot_printf("Test lscb%d - %s!\n", 0, test_result[0]?"ERROR":"OK");
#endif

#ifdef test_LSCB1
    rumboot_putstring("------------ TEST LSCB1 ------------- \n");
    test_result[1] = test_lscb(LSCB1, 1);
    rumboot_printf("Test lscb%d - %s!\n", 1, test_result[1]?"ERROR":"OK");
#endif
    test_error = test_result[0] || test_result[1];
	rumboot_putstring((!test_error)?
	        "TEST_OK\n":"TEST_ERROR\n");
    return test_error;
}

