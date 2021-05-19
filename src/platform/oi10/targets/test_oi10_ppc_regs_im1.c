#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>

#include <platform/interrupts.h>
#include <regs/fields/mpic128.h>
#include <platform/regs/sctl.h>
#include <devices/ugly/emi.h>
#include <platform/test_assert.h>

#include <devices/ugly/greth.h>

struct regpoker_checker greth_check_array[] = {
    { "CTRL              ",  REGPOKER_READ32,  CTRL              , 0x9A000090, 0xFE007CFF },
    { "MDIO_CTRL         ",  REGPOKER_READ32,  MDIO_CTRL         , 0x01E10140, 0xFFFFFFCF },
    { "EDCL_IP           ",  REGPOKER_READ32,  EDCL_IP           , EDCLIP0, ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_READ32,  EDCL_MAC_MSB      , EDCLMAC_MSB, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_READ32,  EDCL_MAC_LSB      , EDCLMAC_LSB, ~0 },
    { "EDCL_IP           ",  REGPOKER_WRITE32, EDCL_IP           , 0, ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_WRITE32, EDCL_MAC_MSB      , 0, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_WRITE32, EDCL_MAC_LSB      , 0, ~0 }
};


void regs_check(uint32_t base_addr)
{
    if (base_addr==GRETH_1_BASE)
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 1, base_addr);
        greth_check_array[2].expected = EDCLIP1;
        greth_check_array[4].expected |= EDCLADDRL1;
    }
    else
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 0, base_addr);
    }
    TEST_ASSERT(rumboot_regpoker_check_array(greth_check_array, base_addr)==0, "Failed to check GRETH registers\n");
}



int main(void)
{
    uint32_t* src;
    uint32_t volatile __attribute__((used)) val = 0;
    uint32_t src_prnt_l, src_prnt_b;
    uint64_t src_p;
    //volatile __attribute__((used))  
    //uint32_t  val;
    
    regs_check(GRETH_0_BASE);    
    
    //src = (uint32_t*) rumboot_malloc_from_heap_aligned(rumboot_malloc_heap_by_name("IM1"), 512);
    //dst = (uint32_t*) rumboot_malloc_from_heap_aligned(1, 512);
    
    src = (uint32_t*)rumboot_malloc_from_named_heap_aligned("IM1", 512 * sizeof(uint32_t), 16);
    src_p = rumboot_virt_to_phys(src);
    src_prnt_l = src_p;
    src_prnt_b = src_p >> 32;
    
    rumboot_printf("Start IM1 delay checks, src_virt = 0x%x, src_phys = 0x%x%x\n", src, src_prnt_b, src_prnt_l);
    for (int i=0; i < 512; i++)  {
        src[i] = i;
    }
    
    for (int i=0; i < 512; i++)  {
        //ioread32(&src[i]);
        val = src[i];
    }
    
    msync();
    
    src = (uint32_t*)rumboot_malloc_from_named_heap_aligned("IM0", 512 * sizeof(uint32_t), 16);
    src_p = rumboot_virt_to_phys(src);
    src_prnt_l = src_p;
    src_prnt_b = src_p >> 32;
    rumboot_printf("Start IM0 delay checks, src_virt = 0x%x, src_phys = 0x%x%x\n", src, src_prnt_b, src_prnt_l);
    for (int i=0; i < 512; i++)  {
        src[i] = i;
    }
    
    for (int i=0; i < 512; i++)  {
        //ioread32(&src[i]);
            val = src[i];
    }
    
    msync();
    
    src = (uint32_t*)rumboot_malloc_from_named_heap_aligned("SRAM0", 512 * sizeof(uint32_t), 16);
    src_p = rumboot_virt_to_phys(src);
    src_prnt_l = src_p;
    src_prnt_b = src_p >> 32;
    rumboot_printf("Start SRAM0 delay checks, src_virt = 0x%x, src_phys = 0x%x%x\n", src, src_prnt_b, src_prnt_l);
    for (int i=0; i < 512; i++)  {
        src[i] = i;
    }
    
    for (int i=0; i < 512; i++)  {
        //ioread32(&src[i]);
            val = src[i];
    }    
    
    return 0;
}

//***********************************************************************************************************************************************************************************************************