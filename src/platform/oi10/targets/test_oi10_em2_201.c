
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/devices/emi.h>

int check_sram(uint32_t base_addr)
{
    emi_bank_cfg bn_cfg;

    rumboot_printf("Checking SRAM (0x%X)\n", base_addr);
    emi_init();

    iowrite32(0xBABADEDA, base_addr);
    rumboot_printf("MEM[0x%X] = 0x%x\n", base_addr, ioread32(base_addr));

    get_emi_bank_cfg(0, DCR_EM2_EMI_BASE, &bn_cfg);
    bn_cfg.ssx_cfg.T_CYC = TCYC_2;
    set_emi_bank_cfg(0, DCR_EM2_EMI_BASE, &bn_cfg);

    iowrite32(0xBABADEDA, base_addr);
    rumboot_printf("MEM[0x%X] = 0x%x\n", base_addr, ioread32(base_addr));

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

