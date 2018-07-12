
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_nic400.h>

#define PERIPHERAL_ID_BASE_32 0xC0100000
#define PERIPHERAL_ID_BASE_64 0xC0200000

static uint8_t AXI32_check_ID_regs()
{   
    rumboot_printf("Checking ID 32 registers default value...\n");
    /*peripheral_id*/
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR4) == 0x04), "NIC400_PIDR4 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR5) == 0x00), "NIC400_PIDR5 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR6) == 0x00), "NIC400_PIDR6 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR7) == 0x00), "NIC400_PIDR7 regi4ster default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR0) == 0x00), "NIC400_PIDR0 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR1) == 0xB4), "NIC400_PIDR1 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR2) == 0x5B), "NIC400_PIDR2 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR3) == 0x00), "NIC400_PIDR3 register default value incorrect");
    /*components_id*/
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_CIDR0) == 0x0D), "NIC400_CIDR0 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_CIDR1) == 0xF0), "NIC400_CIDR1 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_CIDR2) == 0x05), "NIC400_CIDR2 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_CIDR3) == 0xB1), "NIC400_CIDR3 register default value incorrect");
        
    return 1;
}

static uint8_t AXI64_check_ID_regs()
{
    rumboot_printf("Checking ID 64 registers default value...\n");
    /*peripheral_id*/
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR4) == 0x04), "PID_PERIPHERAL_ID4 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR5) == 0x00), "PID_PERIPHERAL_ID5 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR6) == 0x00), "PID_PERIPHERAL_ID6 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR7) == 0x00), "PID_PERIPHERAL_ID7 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR0) == 0x00), "PID_PERIPHERAL_ID0 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR1) == 0xB4), "PID_PERIPHERAL_ID1 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR2) == 0x5B), "PID_PERIPHERAL_ID2 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR3) == 0x00), "PID_PERIPHERAL_ID3 register default value incorrect");
    /*components_id read and compare reset value from table 3-5 in docs*/
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_CIDR0) == 0x0D), "PID_COMPONENT_ID0 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_CIDR1) == 0xF0), "PID_COMPONENT_ID1 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_CIDR2) == 0x05), "PID_COMPONENT_ID2 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_CIDR3) == 0xB1), "PID_COMPONENT_ID3 register default value incorrect");
    
    return 1;
}

static uint8_t AXI_check(uint32_t base_addr, uint32_t offset, uint8_t value)
{
    rumboot_putstring("Checking for read/write registers...\n"); 
    iowrite32(value, base_addr + offset);
    TEST_ASSERT((ioread32(base_addr + offset) == value), "Register default value incorrect");
    return 1;
}

int main(void)
{ 
   register uint8_t result;
   ///cpu_s
   result = AXI32_check_ID_regs() && AXI64_check_ID_regs()
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_ARM_S_RQOS, 0x4)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_ARM_S_WQOS, 0x5)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_ARM_S_FN_MOD, 0x0)
   ///
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_PCIE_S_RQOS, 0x2)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_PCIE_S_WQOS, 0x4)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_ARM_S_FN_MOD, 0x0)
   ///eth_1_s
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA0_S_RQOS, 0xc)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA0_S_WQOS, 0xd)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA0_S_FN_MOD, 0x0)
   ///sw_0_3_s
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA1_S_RQOS, 0xa)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA1_S_WQOS, 0xb)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA1_S_FN_MOD, 0x0)
   ///gspi_0_s
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA2_S_RQOS, 0x7)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA2_S_WQOS, 0x8)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA2_S_FN_MOD, 0x0)
   ///eth_0_s
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA3_S_RQOS, 0xf)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA3_S_WQOS, 0xd)
   && AXI_check(PERIPHERAL_ID_BASE_32, NIC400_DMA3_S_FN_MOD, 0x0)
   ///sw_0_s
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_ARM_S_RQOS, 0x4)
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_ARM_S_WQOS, 0x5)
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_ARM_S_FN_MOD, 0x0)
   ///sw_1_s
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_PCIE_S_RQOS, 0x2)
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_PCIE_S_WQOS, 0x4)
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_ARM_S_FN_MOD, 0x0)
   ///sw_2_s
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_DMA0_S_RQOS, 0xc)
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_DMA0_S_WQOS, 0xd)
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_DMA0_S_FN_MOD, 0x0)
   ///sw_3_s
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_DMA1_S_RQOS, 0xa)
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_DMA1_S_WQOS, 0xb)
   && AXI_check(PERIPHERAL_ID_BASE_64, NIC400_DMA1_S_FN_MOD, 0x0);

    if(result)
    {
	return 0;
    }
    
    return 1;
}


