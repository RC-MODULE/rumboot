
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/test_event_c.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>

#define EVENT_CHECK_SRAM0_TSOE_TCYC       0x0000003E
#define EVENT_CHECK_NOR_TSOE_TCYC         0x0000003F
#define EVENT_CHECK_SRAM1_TSOE_TCYC       0x00000040
#define EVENT_CHECK_SSRAM_SST_TSSOE_FT    0x00000041
#define EVENT_CHECK_SSRAM_SST_TSSOE_PIPE  0x00000042

#define TEST_ADDR_0  (base_addr +  0x4)
#define TEST_ADDR_1  (base_addr + 0x10)

void refresh_timings(emi_bank_num bank_num)
{
    //for refreshing timings at some bank we have to make access to some other bank
    switch (bank_num)
    {
        case emi_b0_sram0:
            ioread32(SRAM1_BASE);
            break;
        default:
            ioread32(SRAM0_BASE);
            break;
    }
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

    rumboot_printf("Checking SRAM0 (0x%X)\n", base_addr);

    for (int i=0; i<SRAM0_TSOE_SPACE; i++)
        for (int j=0; j<SRAM0_TCYC_SPACE; j++)
        {
            emi_update_tcyc_tsoe(emi_b0_sram0, test_tsoe_arr[i], test_tcyc_arr[j]);
            rumboot_printf("Setting tcyc / tsoe: %d / %d\n", test_tcyc_arr[j], test_tsoe_arr[i]);
            test_event(EVENT_CHECK_SRAM0_TSOE_TCYC);
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
    const ssx_tcyc_t test_tcyc_arr[NOR_TCYC_SPACE] = {TCYC_5, TCYC_9, TCYC_13};

    rumboot_printf("Checking NOR (0x%X)\n", base_addr);

    for (int i=0; i<NOR_TSOE_SPACE; i++)
        for (int j=0; j<NOR_TCYC_SPACE; j++)
        {
            emi_update_tcyc_tsoe(emi_b5_nor, test_tsoe_arr[i], test_tcyc_arr[j]);
            rumboot_printf("Setting tcyc / tsoe: %d / %d\n", test_tcyc_arr[j], test_tsoe_arr[i]);
            test_event(EVENT_CHECK_NOR_TSOE_TCYC);
            check_wrrd(TEST_ADDR_0, (i<<16) | j);
            check_wrrd(TEST_ADDR_1, ~((i<<16) | j));
        }
    return 0;
}

/*
 * SDRAM (2.1.3 and 2.1.5 PPPC_SRAM_SDRAM_slave0_testplan.docx)
 */
//TODO:
int check_sdram(uint32_t base_addr)
{
    rumboot_printf("Checking SDRAM (0x%X)\n", base_addr);
    iowrite32(0xBABADEDA, base_addr);
    rumboot_printf("[0x%x] = 0x%X\n", base_addr, ioread32(base_addr));
    return 0;
}

/*
 * SSRAM (2.1.4 PPPC_SRAM_SDRAM_slave0_testplan.docx)
 */
int check_ssram(uint32_t base_addr)
{
    #define SSRAM_SST_TSSOE_SPACE 2

    const uint32_t bank = 2;
    uint32_t event_code = EVENT_CHECK_SSRAM_SST_TSSOE_FT;

    ssx_sst_t sst_arr[SSRAM_SST_TSSOE_SPACE] = {SST_Flow_Through, SST_Pipelined};
    ssx_tssoe_t tssoe_arr[SSRAM_SST_TSSOE_SPACE] = {TSSOE_1, TSSOE_2};

    rumboot_printf("Checking SSRAM (0x%X)\n", base_addr);

    for (int i=0; i<SSRAM_SST_TSSOE_SPACE; i++)
    {
        emi_update_sst_tssoe(bank, sst_arr[i], tssoe_arr[i]);
        test_event(event_code++);
        check_wrrd(TEST_ADDR_0, 0x55555555<<i     );
        check_wrrd(TEST_ADDR_1, 0x55555555<<(i+1) );
    }
    return 0;
}

/*
 * PIPELINED (2.1.6 PPPC_SRAM_SDRAM_slave0_testplan.docx)
 */
int check_pipelined(uint32_t base_addr)
{
    rumboot_printf("Checking PIPELINED (0x%X)\n", base_addr);
    check_wrrd(TEST_ADDR_0, 0x55555555 );
    check_wrrd(TEST_ADDR_1, 0xAAAAAAAA );
    return 0;
}

int main()
{
    int ret;
    rumboot_printf("Start test_oi10_em2_201 (0x%X)\n", EXT_MEM_BASE);
    test_event_send_test_id("test_oi10_em2_201");
    emi_init(DCR_EM2_EMI_BASE);
    switch (EXT_MEM_BASE)
    {
        case SRAM0_BASE:
            ret = check_sram0(EXT_MEM_BASE);
            break;
        case SDRAM_BASE:
            ret = check_sdram(EXT_MEM_BASE);
            break;
        case SSRAM_BASE:
            ret = check_ssram(EXT_MEM_BASE);
            break;
        case PIPELINED_BASE:
            ret = check_pipelined(EXT_MEM_BASE);
            break;
        case SRAM1_BASE:
            ret = check_sram0(EXT_MEM_BASE);
            break;
        case NOR_BASE:
            ret = check_nor(EXT_MEM_BASE);
            break;
        default:
            rumboot_printf("Wrong base address 0x%X", EXT_MEM_BASE);
            return 1;
    }
    return ret;
}

