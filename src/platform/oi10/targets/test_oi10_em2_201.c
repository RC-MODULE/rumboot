
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/devices/emi.h>


void emi_update_tcyc_tsoe(uint32_t bank_num, ssx_tsoe_t tsoe, ssx_tcyc_t tcyc)
{
    emi_bank_cfg bn_cfg;
    emi_get_bank_cfg(bank_num, DCR_EM2_EMI_BASE, &bn_cfg);
    bn_cfg.ssx_cfg.T_SOE = tsoe;
    bn_cfg.ssx_cfg.T_CYC = tcyc;
    emi_set_bank_cfg(bank_num, DCR_EM2_EMI_BASE, &bn_cfg);
}

void check_wrrd(uint32_t addr)
{
    iowrite32(0xBABADEDA, addr);
    rumboot_printf("MEM[0x%X] = 0x%x\n", addr, ioread32(addr));
}

int check_sram(uint32_t base_addr)
{
    #define TSOE_SPACE  2
    #define TCYC_SPACE  4
    const ssx_tsoe_t test_tsoe_arr[TSOE_SPACE] = {TSOE_1, TSOE_2};
    const ssx_tcyc_t test_tcyc_arr[TCYC_SPACE] = {TCYC_2, TCYC_3, TCYC_4, TCYC_5};

    rumboot_printf("Checking SRAM (0x%X)\n", base_addr);
    emi_init();

    for (int i=0; i<TSOE_SPACE; i++)
        for (int j=0; j<TCYC_SPACE; j++)
        {
            emi_update_tcyc_tsoe(0, test_tsoe_arr[i], test_tcyc_arr[j]);
            check_wrrd(base_addr);
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

//TODO:
int check_nor(uint32_t base_addr)
{
    return 0;
}


int main()
{
    int ret;
    rumboot_printf("Start test_oi10_em2_201\n");
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

