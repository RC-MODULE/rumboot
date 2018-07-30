
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

void emi_update_tcyc_tsoe(uint32_t bank_num, ssx_tsoe_t tsoe, ssx_tcyc_t tcyc)
{
    emi_bank_cfg bn_cfg;
    emi_get_bank_cfg(bank_num, DCR_EM2_EMI_BASE, &bn_cfg);
    bn_cfg.ssx_cfg.T_SOE = tsoe;
    bn_cfg.ssx_cfg.T_CYC = tcyc;
    emi_set_bank_cfg(bank_num, DCR_EM2_EMI_BASE, &bn_cfg);
    iowrite32_NOR(0xDEDABABA, NOR_BASE);
}

void check_wrrd(uint32_t addr, uint32_t data)
{
    rumboot_printf("Checking store/load data 0x%x to addr 0x%x", data, addr);
    iowrite32(data, addr);
    TEST_ASSERT(ioread32(addr)==data, "Data error!");
}

int check_sram(uint32_t base_addr)
{
    #define EVENT_CHECK_SRAM0_TSOE_TCYC  0x0000003E
    #define SRAM_TSOE_SPACE  2
    #define SRAM_TCYC_SPACE  4
    #define SRAM0_TEST_ADDR_0  (base_addr +  0x4)
    #define SRAM0_TEST_ADDR_1  (base_addr + 0x10)

    const ssx_tsoe_t test_tsoe_arr[SRAM_TSOE_SPACE] = {TSOE_1, TSOE_2};
    const ssx_tcyc_t test_tcyc_arr[SRAM_TCYC_SPACE] = {TCYC_2, TCYC_3, TCYC_4, TCYC_5};

    rumboot_printf("Checking SRAM (0x%X)\n", base_addr);
    for (int i=0; i<SRAM_TSOE_SPACE; i++)
        for (int j=0; j<SRAM_TCYC_SPACE; j++)
        {
            emi_update_tcyc_tsoe(0, test_tsoe_arr[i], test_tcyc_arr[j]);
            test_event(EVENT_CHECK_SRAM0_TSOE_TCYC);
            check_wrrd(SRAM0_TEST_ADDR_0, (i<<16) | j);
            check_wrrd(SRAM0_TEST_ADDR_1, ~((i<<16) | j));
        }
    return 0;
}

//TODO:
int check_sdram(uint32_t base_addr)
{
    rumboot_printf("Checking SDRAM (0x%X) not yet implemented\n", base_addr);
    return 0;
}

//TODO:
int check_ssram(uint32_t base_addr)
{
    rumboot_printf("Checking SSRAM (0x%X) not yet implemented\n", base_addr);
    return 0;
}

//TODO:
int check_pipelined(uint32_t base_addr)
{
    rumboot_printf("Checking SDRAM (0x%X) not yet implemented\n", base_addr);
    return 0;
}

int check_nor(uint32_t base_addr)
{
    rumboot_printf("Checking NOR (0x%X)\n", base_addr);
    iowrite32_NOR(0xDEDABABA, base_addr);
    rumboot_printf("NOR[0x%x] = 0x%X\n", base_addr, ioread32(base_addr));
    return 0;
}


int main()
{
    int ret;
    rumboot_printf("Start test_oi10_em2_201\n");
    test_event_send_test_id("test_oi10_em2_201");
    emi_init();
    switch (EXT_MEM_BASE)
    {
        case SRAM0_BASE:
            ret = check_sram(EXT_MEM_BASE);
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
            ret = check_sram(EXT_MEM_BASE);
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

