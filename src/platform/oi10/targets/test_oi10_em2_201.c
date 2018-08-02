
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


#define EVENT_CHECK_SRAM0_TSOE_TCYC  0x0000003E
#define EVENT_CHECK_NOR_TSOE_TCYC    0x0000003F
#define EVENT_CHECK_SRAM1_TSOE_TCYC  0x00000040

void refresh_timings(int32_t bank_num)
{
    //TODO: have to add other banks
    //for refreshing timings at some bank we have to make access to some other bank
    switch (bank_num)
    {
        case 0:
            ioread32(NOR_BASE);
            break;
        case 4:
            ioread32(SRAM0_BASE);
            break;
        case 5:
            ioread32(SRAM0_BASE);
            break;
    }
}

void emi_update_tcyc_tsoe(uint32_t bank_num, ssx_tsoe_t tsoe, ssx_tcyc_t tcyc)
{
    emi_bank_cfg bn_cfg;
    emi_get_bank_cfg(bank_num, DCR_EM2_EMI_BASE, &bn_cfg);
    bn_cfg.ssx_cfg.T_SOE = tsoe;
    bn_cfg.ssx_cfg.T_CYC = tcyc;
    emi_set_bank_cfg(bank_num, DCR_EM2_EMI_BASE, &bn_cfg);
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
 * SRAM0 and NOR (2.1.1 and 2.1.2 of PPC_SRAM_SDRAM_slave0_testplan.docx)
 */
int check_sram_nor(uint32_t base_addr)
{
    #define SRAM_TSOE_SPACE  2
    #define SRAM_TCYC_SPACE  4
    #define TEST_ADDR_0  (base_addr +  0x4)
    #define TEST_ADDR_1  (base_addr + 0x10)

    const ssx_tsoe_t test_tsoe_arr[SRAM_TSOE_SPACE]       = {TSOE_1, TSOE_2};
    const ssx_tcyc_t test_tcyc_arr_sram0[SRAM_TCYC_SPACE] = {TCYC_2, TCYC_3, TCYC_4, TCYC_5};
    const ssx_tcyc_t test_tcyc_arr_nor[SRAM_TCYC_SPACE]   = {TCYC_3, TCYC_5, TCYC_9, TCYC_12};
    ssx_tcyc_t tcyc;

    uint32_t event_code = (base_addr==NOR_BASE)   ? EVENT_CHECK_NOR_TSOE_TCYC   :
                          (base_addr==SRAM1_BASE) ? EVENT_CHECK_SRAM1_TSOE_TCYC :
                                                    EVENT_CHECK_SRAM0_TSOE_TCYC;

    uint32_t bank       = (base_addr==NOR_BASE)   ? 5 :
                          (base_addr==SRAM1_BASE) ? 4 :
                                                    0;

    rumboot_printf("Checking SRAM/NOR (0x%X)\n", base_addr);

    for (int i=0; i<SRAM_TSOE_SPACE; i++)
        for (int j=0; j<SRAM_TCYC_SPACE; j++)
        {
            tcyc = (base_addr==NOR_BASE) ? test_tcyc_arr_nor[j] : test_tcyc_arr_sram0[j];
            emi_update_tcyc_tsoe(bank, test_tsoe_arr[i], tcyc);
            test_event(event_code);
            check_wrrd(TEST_ADDR_0, (i<<16) | j);
            check_wrrd(TEST_ADDR_1, ~((i<<16) | j));
        }
    return 0;
}

/*
 * SDRAM
 */
//TODO:
int check_sdram(uint32_t base_addr)
{
    rumboot_printf("Checking SDRAM (0x%X)\n", base_addr);
    iowrite32(0xBABADEDA, base_addr);
    rumboot_printf("[0x%x] = 0x%X\n", ioread32(base_addr));
    return 0;
}

/*
 * SSRAM
 */
//TODO:
int check_ssram(uint32_t base_addr)
{
    rumboot_printf("Checking SSRAM (0x%X) not yet implemented\n", base_addr);
    return 0;
}

/*
 * PIPELINED
 */
//TODO:
int check_pipelined(uint32_t base_addr)
{
    rumboot_printf("Checking SDRAM (0x%X) not yet implemented\n", base_addr);
    return 0;
}

int main()
{
    int ret;
    rumboot_printf("Start test_oi10_em2_201 (0x%X)\n", EXT_MEM_BASE);
    test_event_send_test_id("test_oi10_em2_201");
    emi_init();
    switch (EXT_MEM_BASE)
    {
        case SRAM0_BASE:
            ret = check_sram_nor(EXT_MEM_BASE);
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
            ret = check_sram_nor(EXT_MEM_BASE);
            break;
        case NOR_BASE:
            ret = check_sram_nor(EXT_MEM_BASE);
            break;
        default:
            rumboot_printf("Wrong base address 0x%X", EXT_MEM_BASE);
            return 1;
    }
    return ret;
}

