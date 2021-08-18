
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>

#include <platform/devices.h>
#include <platform/test_event_c.h>
#include <devices/ugly/emi.h>
#include <platform/devices/nor_1636RR4.h>
#include <arch/dma2plb6.h>
#include <regs/regs_emi.h>

#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/mem_window.h>


#define EVENT_CHECK_SRAM0_TSOE_TCYC       0x0000003E
#define EVENT_CHECK_NOR_TSOE_TCYC         0x0000003F
#define EVENT_CHECK_SRAM1_TSOE_TCYC       0x00000040
#define EVENT_CHECK_SSRAM_SST_TSSOE_FT    0x00000041
#define EVENT_CHECK_SSRAM_SST_TSSOE_PIPE  0x00000042
#define EVENT_CHECK_SDRAM_2_1_3           0x00000043
#define EVENT_CHECK_SDRAM_2_1_5           0x00000044
#define EVENT_CHECK_PIPE                  0x00000045

#define TEST_ADDR_0  (base_addr +  0x4)
#define TEST_ADDR_1  (base_addr + 0x10)

#define SDRAM_TEST_ADDR (SDRAM_BASE + 0x4000)

/*                       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
#define TLB_ENTRY_SDRAM0 MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b1,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_SDRAM1 MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b1,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )


void refresh_timings(emi_bank_num bank_num)
{
    //for refreshing timings at some bank we have to make access to some other bank
    rumboot_putstring("Refresh timings\n");
    switch (bank_num)
    {
        case emi_b0_sram0:
            dcbi((void *)SRAM1_BASE);
            ioread32(SRAM1_BASE);
            break;
        default:
            dcbi((void *)SRAM0_BASE);
            ioread32(SRAM0_BASE);
            break;
    }
}

void report_bank_cfg(emi_bank_cfg* bank_cfg)
{
    rumboot_printf("Report emi bank configuration\n");
    rumboot_printf("EMI_SSI[BTYP  ] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.BTYP   );
    rumboot_printf("EMI_SSI[PTYP  ] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.PTYP   );
    rumboot_printf("EMI_SSI[SRDY  ] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.SRDY   );
    rumboot_printf("EMI_SSI[TWR   ] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.TWR    );
    rumboot_printf("EMI_SSI[SST   ] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.SST    );
    rumboot_printf("EMI_SSI[T_SSOE] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.T_SSOE );
    rumboot_printf("EMI_SSI[T_SOE ] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.T_SOE  );
    rumboot_printf("EMI_SSI[T_CYC ] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.T_CYC  );
    rumboot_printf("EMI_SSI[T_RDY ] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.T_RDY  );
    rumboot_printf("EMI_SSI[T_DEL ] = 0x%X\n", (uint8_t)bank_cfg->ssx_cfg.T_DEL  );
    rumboot_printf("EMI_SDI[CSP   ] = 0x%X\n", (uint8_t)bank_cfg->sdx_cfg.CSP    );
    rumboot_printf("EMI_SDI[SDS   ] = 0x%X\n", (uint8_t)bank_cfg->sdx_cfg.SDS    );
    rumboot_printf("EMI_SDI[CL    ] = 0x%X\n", (uint8_t)bank_cfg->sdx_cfg.CL     );
    rumboot_printf("EMI_SDI[T_RDL ] = 0x%X\n", (uint8_t)bank_cfg->sdx_cfg.T_RDL  );
    rumboot_printf("EMI_SDI[SI    ] = 0x%X\n", (uint8_t)bank_cfg->sdx_cfg.SI     );
    rumboot_printf("EMI_SDI[T_RCD ] = 0x%X\n", (uint8_t)bank_cfg->sdx_cfg.T_RCD  );
    rumboot_printf("EMI_SDI[T_RAS ] = 0x%X\n", (uint8_t)bank_cfg->sdx_cfg.T_RAS  );
}

void emi_update_tcyc_tsoe(emi_bank_num bank_num, ssx_tsoe_t tsoe, ssx_tcyc_t tcyc)
{
    emi_bank_cfg bn_cfg;
    emi_get_bank_cfg(DCR_EM2_EMI_BASE, bank_num, &bn_cfg);
    bn_cfg.ssx_cfg.T_SOE = tsoe;
    bn_cfg.ssx_cfg.T_CYC = tcyc;
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, bank_num, &bn_cfg);
    emi_get_bank_cfg(DCR_EM2_EMI_BASE, bank_num, &bn_cfg);
    TEST_ASSERT(bn_cfg.ssx_cfg.T_SOE==tsoe, "Unexpected TSOE value after update");
    TEST_ASSERT(bn_cfg.ssx_cfg.T_CYC==tcyc, "Unexpected TCYC value after update");
    refresh_timings(bank_num);
}

void emi_update_sst_tssoe(emi_bank_num bank_num, ssx_sst_t sst, ssx_tssoe_t tssoe)
{
    emi_bank_cfg bn_cfg;
    emi_get_bank_cfg(DCR_EM2_EMI_BASE, bank_num, &bn_cfg);
    bn_cfg.ssx_cfg.SST = sst;
    bn_cfg.ssx_cfg.T_SSOE = tssoe;
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, bank_num, &bn_cfg);
    emi_get_bank_cfg(DCR_EM2_EMI_BASE, bank_num, &bn_cfg);
    TEST_ASSERT(bn_cfg.ssx_cfg.SST==sst, "Unexpected SST value after update");
    TEST_ASSERT(bn_cfg.ssx_cfg.T_SSOE==tssoe, "Unexpected TSSOE value after update");
    refresh_timings(bank_num);
}

void check_wrrd(uint32_t addr, uint32_t data)
{
    rumboot_printf("Checking store/load data 0x%x to addr 0x%x\n", data, addr);
    if (addr>=NOR_BASE)
        nor_write32(data, addr);
    else
        iowrite32(data, addr);
    TEST_ASSERT(ioread32(addr)==data, "Data error!");
}

/*
 * SRAM0 (2.1.1 of PPC_SRAM_SDRAM_slave0_testplan.docx)
 */
int check_sram0(uint32_t base_addr)
{
    #define SRAM0_TSOE_SPACE  2
    #define SRAM0_TCYC_SPACE  4

    const ssx_tsoe_t test_tsoe_arr[SRAM0_TSOE_SPACE] = {TSOE_1, TSOE_2};
    const ssx_tcyc_t test_tcyc_arr[SRAM0_TCYC_SPACE] = {TCYC_2, TCYC_3, TCYC_4, TCYC_5};

#ifndef  SW_ASSERTS_ONLY
    uint32_t event_code = (base_addr==SRAM0_BASE) ? EVENT_CHECK_SRAM0_TSOE_TCYC : EVENT_CHECK_SRAM1_TSOE_TCYC;
#endif
    emi_bank_num bank = (base_addr==SRAM0_BASE) ? emi_b0_sram0 : emi_b4_sram1;

    rumboot_printf("Checking SRAM0/SRAM1 (0x%X)\n", base_addr);

    for (int i=0; i<SRAM0_TSOE_SPACE; i++)
        for (int j=0; j<SRAM0_TCYC_SPACE; j++)
        {
            emi_update_tcyc_tsoe(bank, test_tsoe_arr[i], test_tcyc_arr[j]);
            rumboot_printf("Setting tcyc / tsoe: %d / %d\n", test_tcyc_arr[j], test_tsoe_arr[i]);
#ifndef  SW_ASSERTS_ONLY
            test_event(event_code);
#endif
            check_wrrd(TEST_ADDR_0, (i<<16) | j);
            check_wrrd(TEST_ADDR_1, ~((i<<16) | j));
        }
    return 0;
}

/*
 * NOR (2.1.2 of PPC_SRAM_SDRAM_slave0_testplan.docx)
 */
int check_nor(uint32_t base_addr)
{
    #define NOR_TSOE_SPACE  2
    #define NOR_TCYC_SPACE  3

    const ssx_tsoe_t test_tsoe_arr[NOR_TSOE_SPACE] = {TSOE_1, TSOE_2};
    const ssx_tcyc_t test_tcyc_arr[NOR_TCYC_SPACE] = {TCYC_7, TCYC_9, TCYC_12};

    rumboot_printf("Checking NOR (0x%X)\n", base_addr);

    for (int i=0; i<NOR_TSOE_SPACE; i++)
        for (int j=0; j<NOR_TCYC_SPACE; j++)
        {
            emi_update_tcyc_tsoe(emi_b5_nor, test_tsoe_arr[i], test_tcyc_arr[j]);
            rumboot_printf("Setting tcyc / tsoe: %d / %d\n", test_tcyc_arr[j], test_tsoe_arr[i]);
#ifndef  SW_ASSERTS_ONLY
            test_event(EVENT_CHECK_NOR_TSOE_TCYC);
#endif
            check_wrrd(TEST_ADDR_0, (i<<16) | j);
            check_wrrd(TEST_ADDR_1, ~((i<<16) | j));
            nor_erase_sect(TEST_ADDR_0);
        }
    return 0;
}

/*
 * SDRAM (2.1.3 and 2.1.5 PPPC_SRAM_SDRAM_slave0_testplan.docx)
 */
void emi_update_sdx(emi_sdx_reg_cfg* sdx)
{
    emi_bank_cfg bank_cfg;
    emi_get_bank_cfg(DCR_EM2_EMI_BASE, emi_b1_sdram, &bank_cfg);
//    rumboot_printf("SDRAM bank configuration before updating\n");
//    report_bank_cfg(&bank_cfg);

    bank_cfg.sdx_cfg = *sdx;
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, emi_b1_sdram, &bank_cfg);
    refresh_timings(emi_b1_sdram);

    emi_get_bank_cfg(DCR_EM2_EMI_BASE, emi_b1_sdram, &bank_cfg);
//    rumboot_printf("SDRAM bank configuration after updating\n");
//    report_bank_cfg(&bank_cfg);
}

void mem_set32(uint32_t addr, uint32_t len_bytes, uint32_t val)
{
    uint32_t i;
    for (i=0; i<len_bytes; i+=4)
    {
        iowrite32(val, addr + i);
    }
}

void sdram_oper()
{
    uint32_t buf0;
    uint32_t buf1;

    iowrite32(0xBABADEDA, SDRAM_TEST_ADDR - 4);
    iowrite32(0xDEDABABA, SDRAM_TEST_ADDR - 0);
    msync();

    buf0 = ioread32(SDRAM_TEST_ADDR - 4);
    buf1 = ioread32(SDRAM_TEST_ADDR - 0);
    msync();

    TEST_ASSERT(((buf0==0xBABADEDA) && (buf1==0xDEDABABA)), "SDRAM data error!");
}

int check_sdram_2_1_3(uint32_t base_addr, sdx_csp_t csp, sdx_sds_t sds, sdx_sds_t cl)
{
#define SDRAM_TRDL_SPACE    2
#define SDRAM_TRCD_SPACE    2
#define SDRAM_TRAS_SPACE    2
#define SDRAM_CL_SPACE      2

    emi_bank_cfg cfg[6];

    sdx_trdl_t trdl_arr[SDRAM_TRDL_SPACE] = {TRDL_1, TRDL_2};
    sdx_trcd_t trcd_arr[SDRAM_TRCD_SPACE] = {TRCD_2, TRCD_5};
    sdx_tras_t tras_arr[SDRAM_TRAS_SPACE] = {TRAS_4, TRAS_11};
    emi_bank_cfg sdram_cfg;

    uint32_t k;
    uint32_t l;
    uint32_t m;

    void (*sdram_oper_from_emi)() = (void *)(SSRAM_BASE + 0x100);
    /*
    static const tlb_entry sdram_tlb_entry[2] = { {TLB_ENTRY_SDRAM0},
                                                  {TLB_ENTRY_SDRAM1} };
    */

    static const tlb_entry sdram_tlb_entry[1] = { {TLB_ENTRY_SDRAM1} };

    memset((void*) SRAM0_BASE, 0, 128);
    memset((void*) (SSRAM_BASE), 0, 0x2000);

    rumboot_printf("Checking SDRAM\n");

    prepare_default_emi_cfg(cfg);
    cfg[1].sdx_cfg.CSP   = csp;
    cfg[1].sdx_cfg.SDS   = sds;
    cfg[1].sdx_cfg.CL    = cl;
    emi_init_ext_cfg(DCR_EM2_EMI_BASE, cfg);
    emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);

    rumboot_putstring("Preinit test memory\n");
    mem_set32(SDRAM_TEST_ADDR - 0x80, 256, 0);//hier loader expects only one sdram configuration
    //memset((void*) (SDRAM_TEST_ADDR - 0x80), 0, 256);

    memcpy(sdram_oper_from_emi, sdram_oper, (uint32_t)check_sdram_2_1_3 - (uint32_t)sdram_oper);
    msync();

    write_tlb_entries(sdram_tlb_entry, 1);

    rumboot_putstring("Touching sdram_oper_from_emi()\n");
    icbt(0, sdram_oper_from_emi);

    emi_get_bank_cfg(DCR_EM2_EMI_BASE, emi_b1_sdram, &sdram_cfg);
    for (k=0; k<SDRAM_TRDL_SPACE ; k++)
        for (l=0; l<SDRAM_TRCD_SPACE ; l++)
            for (m=0; m<SDRAM_TRAS_SPACE ; m++)
            {
                rumboot_putstring("Update EMI_SDRAM\n");
                sdram_cfg.sdx_cfg.T_RDL = trdl_arr[k];
                sdram_cfg.sdx_cfg.T_RCD = trcd_arr[l];
                sdram_cfg.sdx_cfg.T_RAS = tras_arr[m];
                emi_set_bank_cfg(DCR_EM2_EMI_BASE, emi_b1_sdram, &sdram_cfg);
                refresh_timings(emi_b1_sdram);

                rumboot_putstring("Execute access to memory\n");
#ifndef  SW_ASSERTS_ONLY
                test_event(EVENT_CHECK_SDRAM_2_1_3);
#endif
                sdram_oper_from_emi();

                //dcbi((void *)SDRAM_TEST_ADDR - 4);
                //TEST_ASSERT(0xBABADEDA==ioread32(SDRAM_TEST_ADDR - 4), "Data error");
                //dcbi((void *)SDRAM_TEST_ADDR - 0);
                //TEST_ASSERT(0xBABADEDA==ioread32(SDRAM_TEST_ADDR - 0), "Data error");
            }

    return 0;
}

int check_sdram_2_1_5(uint32_t base_addr, sdx_sds_t sds)
{
    emi_bank_cfg sdram_cfg;

    void (*sdram_oper_from_emi)() = (void *)(SSRAM_BASE + 0x100);

    static const tlb_entry sdram_tlb_entry[2] = { {TLB_ENTRY_SDRAM0},
                                                  {TLB_ENTRY_SDRAM1} };


    memset((void*) SRAM0_BASE, 0, 128);
    memset((void*) (SSRAM_BASE), 0, 0x2000);

    rumboot_printf("Checking SDRAM\n");

    rumboot_putstring("Update EMI_SDRAM\n");
    emi_get_bank_cfg(DCR_EM2_EMI_BASE, emi_b1_sdram, &sdram_cfg);
    sdram_cfg.sdx_cfg.SDS   = sds;
    sdram_cfg.sdx_cfg.T_RDL = TRDL_2;
    sdram_cfg.sdx_cfg.T_RCD = TRCD_2;
    sdram_cfg.sdx_cfg.T_RAS = TRAS_4;
    emi_set_bank_cfg(DCR_EM2_EMI_BASE, emi_b1_sdram, &sdram_cfg);
    refresh_timings(emi_b1_sdram);

    rumboot_putstring("Preinit test memory\n");
    mem_set32(SDRAM_TEST_ADDR - 0x80, 256, 0);
    //memset((void*) (SDRAM_TEST_ADDR - 0x80), 0, 256);

    memcpy(sdram_oper_from_emi, sdram_oper, (uint32_t)check_sdram_2_1_3 - (uint32_t)sdram_oper);
    msync();

    write_tlb_entries(sdram_tlb_entry, 2);

    rumboot_putstring("Touching sdram_oper_from_emi()\n");
    icbt(0, sdram_oper_from_emi);

    rumboot_putstring("Execute access to memory\n");
#ifndef  SW_ASSERTS_ONLY
    test_event(EVENT_CHECK_SDRAM_2_1_5);
#endif
    sdram_oper_from_emi();
    return 0;
}

/*
 * SSRAM (2.1.4 PPPC_SRAM_SDRAM_slave0_testplan.docx)
 */
int check_ssram(uint32_t base_addr, ssx_sst_t sst_val, ssx_tssoe_t tssoe_val)
{
    rumboot_putstring("Start checking SSRAM\n");
    emi_update_sst_tssoe(emi_b2_ssram, sst_val, tssoe_val);
#ifndef  SW_ASSERTS_ONLY
    test_event((sst_val==SST_Flow_Through) ? EVENT_CHECK_SSRAM_SST_TSSOE_FT : EVENT_CHECK_SSRAM_SST_TSSOE_PIPE);
#endif
    check_wrrd(TEST_ADDR_0, 0x55555555      );
    check_wrrd(TEST_ADDR_1, 0x55555555 << 1 );
    return 0;
}

/*
 * PIPELINED (2.1.6 PPPC_SRAM_SDRAM_slave0_testplan.docx)
 */
int check_pipelined(uint32_t base_addr)
{
    rumboot_printf("Checking PIPELINED (0x%X)\n", base_addr);
#ifndef  SW_ASSERTS_ONLY
    test_event(EVENT_CHECK_PIPE);
#endif
    check_wrrd(TEST_ADDR_0, 0x55555555 );
    check_wrrd(TEST_ADDR_1, 0xAAAAAAAA );
    return 0;
}

int main()
{
    int ret = 0;
    rumboot_printf("Start test_oi10_em2_201 (0x%X)\n", EXT_MEM_BASE);
#ifndef  SW_ASSERTS_ONLY
    test_event_send_test_id("test_oi10_em2_201");
#endif

    memset((void *)SRAM0_BASE, 0, sizeof(uint32_t));
    memset((void *)SSRAM_BASE, 0, sizeof(uint32_t));

    switch (EXT_MEM_BASE)
    {
        case SRAM0_BASE:
            emi_init(DCR_EM2_EMI_BASE);
            emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);
            ret = check_sram0(EXT_MEM_BASE);
            break;
        case SDRAM_BASE:
#ifdef CHECK_2_1_3
            ret = check_sdram_2_1_3(EXT_MEM_BASE, CSP, SDS, CL);
#endif
#ifdef CHECK_2_1_5
            emi_init(DCR_EM2_EMI_BASE);
            emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);
            ret = check_sdram_2_1_5(EXT_MEM_BASE, SDS);
#endif

            break;
        case SSRAM_BASE:
#ifdef SSRAM_SST
            emi_init(DCR_EM2_EMI_BASE);
            emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);
            ret = check_ssram(EXT_MEM_BASE, SSRAM_SST, SSRAM_TSSOE);
#endif
            break;
        case PIPELINED_BASE:
            emi_init(DCR_EM2_EMI_BASE);
            emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);
            ret = check_pipelined(EXT_MEM_BASE);
            break;
        case SRAM1_BASE:
            emi_init(DCR_EM2_EMI_BASE);
            emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);
            ret = check_sram0(EXT_MEM_BASE);
            break;
        case NOR_BASE:
            emi_init(DCR_EM2_EMI_BASE);
            emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);
            ret = check_nor(EXT_MEM_BASE);
            break;
        default:
            rumboot_printf("Wrong base address 0x%X", EXT_MEM_BASE);
            return 1;
    }
    rumboot_putstring("\n\n>>>test_oi10_em2_201 is OK<<<\n");
    return ret;
}

