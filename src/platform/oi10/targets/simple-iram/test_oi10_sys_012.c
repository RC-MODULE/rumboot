
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <rumboot/regpoker.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_nic400.h>

#define PERIPHERAL_ID_BASE_32 0xC0100000
#define PERIPHERAL_ID_BASE_64 0xC0200000

static uint8_t AXIx_check_ID_regs(uint32_t base_addr)
{   
    rumboot_printf("Checking ID registers default value...\n");
    //peripheral_id//
      struct regpoker_checker check_array[] = {
      {" NIC400_PIDR4", REGPOKER_READ32, NIC400_PIDR4, 0x04, 0xFFFFFFFF },
      { "NIC400_PIDR5", REGPOKER_READ32, NIC400_PIDR5, 0x00, 0xFFFFFFFF },
      { "NIC400_PIDR6", REGPOKER_READ32, NIC400_PIDR6, 0x00, 0xFFFFFFFF },
      { "NIC400_PIDR7", REGPOKER_READ32, NIC400_PIDR7, 0x00, 0xFFFFFFFF },
      { "NIC400_PIDR0", REGPOKER_READ32, NIC400_PIDR0, 0x00, 0xFFFFFFFF },
      { "NIC400_PIDR1", REGPOKER_READ32, NIC400_PIDR1, 0xB4, 0xFFFFFFFF },
      { "NIC400_PIDR2", REGPOKER_READ32, NIC400_PIDR2, 0x5B, 0xFFFFFFFF },
      { "NIC400_PIDR3", REGPOKER_READ32, NIC400_PIDR3, 0x00, 0xFFFFFFFF },
      { "NIC400_CIDR0", REGPOKER_READ32, NIC400_CIDR0, 0x0D, 0xFFFFFFFF },
      { "NIC400_CIDR1", REGPOKER_READ32, NIC400_CIDR1, 0xF0, 0xFFFFFFFF },
      { "NIC400_CIDR2", REGPOKER_READ32, NIC400_CIDR2, 0x05, 0xFFFFFFFF },
      { "NIC400_CIDR3", REGPOKER_READ32, NIC400_CIDR3, 0xB1, 0xFFFFFFFF },
      {  }
    };
    return rumboot_regpoker_check_array(check_array, base_addr); // if OK == 0
}

int main(void)
{
   rumboot_printf("Check write/read register values\n");
   // get write for base 32
    iowrite32(0x04, PERIPHERAL_ID_BASE_32 + NIC400_ARM_S_RQOS);
    iowrite32(0x05, PERIPHERAL_ID_BASE_32 + NIC400_ARM_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_32 + NIC400_ARM_S_FN_MOD);

    iowrite32(0x02, PERIPHERAL_ID_BASE_32 + NIC400_PCIE_S_RQOS);
    iowrite32(0x04, PERIPHERAL_ID_BASE_32 + NIC400_PCIE_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_32 + NIC400_PCIE_S_FN_MOD);

    iowrite32(0x0c, PERIPHERAL_ID_BASE_32 + NIC400_DMA0_S_RQOS);
    iowrite32(0x0d, PERIPHERAL_ID_BASE_32 + NIC400_DMA0_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_32 + NIC400_DMA0_S_FN_MOD);

    iowrite32(0x0a, PERIPHERAL_ID_BASE_32 + NIC400_DMA1_S_RQOS);
    iowrite32(0x0b, PERIPHERAL_ID_BASE_32 + NIC400_DMA1_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_32 + NIC400_DMA1_S_FN_MOD);

    iowrite32(0x07, PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S_RQOS);
    iowrite32(0x08, PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S_FN_MOD);

    iowrite32(0x0f, PERIPHERAL_ID_BASE_32 + NIC400_DMA3_S_RQOS);
    iowrite32(0x0d, PERIPHERAL_ID_BASE_32 + NIC400_DMA3_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_32 + NIC400_DMA3_S_FN_MOD);
    // get write for base 64
    iowrite32(0x04, PERIPHERAL_ID_BASE_64 + NIC400_ARM_S_RQOS);
    iowrite32(0x05, PERIPHERAL_ID_BASE_64 + NIC400_ARM_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_64 + NIC400_ARM_S_FN_MOD);

    iowrite32(0x02, PERIPHERAL_ID_BASE_64 + NIC400_PCIE_S_RQOS);
    iowrite32(0x04, PERIPHERAL_ID_BASE_64 + NIC400_PCIE_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_64 + NIC400_PCIE_S_FN_MOD);

    iowrite32(0x0c, PERIPHERAL_ID_BASE_64 + NIC400_DMA0_S_RQOS);
    iowrite32(0x0d, PERIPHERAL_ID_BASE_64 + NIC400_DMA0_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_64 + NIC400_DMA0_S_FN_MOD);

    iowrite32(0x0a, PERIPHERAL_ID_BASE_64 + NIC400_DMA1_S_RQOS);
    iowrite32(0x0b, PERIPHERAL_ID_BASE_64 + NIC400_DMA1_S_WQOS);
    iowrite32(0x00, PERIPHERAL_ID_BASE_64 + NIC400_DMA1_S_FN_MOD);

    struct regpoker_checker check_array_RWID32[] = {
      {" NIC400_ARM_S_RQOS", REGPOKER_READ32, NIC400_ARM_S_RQOS, 0x04, 0xffffffff },
      { "NIC400_ARM_S_WQOS", REGPOKER_READ32, NIC400_ARM_S_WQOS, 0x05, 0xffffffff },
      { "NIC400_ARM_S_FN_MOD", REGPOKER_READ32, NIC400_ARM_S_FN_MOD, 0x00, 0xffffffff },
      { "NIC400_PCIE_S_RQOS", REGPOKER_READ32, NIC400_PCIE_S_RQOS, 0x02, 0xffffffff },
      { "NIC400_PCIE_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_WQOS, 0x04, 0xffffffff },
      { "NIC400_PCIE_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_FN_MOD, 0x00, 0xffffffff },
      { "NIC400_DMA0_S_RQOS", REGPOKER_READ32, NIC400_DMA0_S_RQOS, 0x0c, 0xffffffff },
      { "NIC400_DMA0_S_WQOS", REGPOKER_READ32, NIC400_DMA0_S_WQOS, 0x0d, 0xffffffff },
      { "NIC400_DMA0_S_FN_MOD", REGPOKER_READ32, NIC400_DMA0_S_FN_MOD, 0x00, 0xffffffff },
      { "NIC400_DMA1_S_RQOS", REGPOKER_READ32, NIC400_DMA1_S_RQOS, 0x0a, 0xffffffff },
      { "NIC400_DMA1_S_WQOS", REGPOKER_READ32, NIC400_DMA1_S_WQOS, 0x0b, 0xffffffff },
      { "NIC400_DMA1_S_FN_MOD", REGPOKER_READ32, NIC400_DMA1_S_FN_MOD, 0x00, 0xffffffff },
      { "NIC400_DMA2_S_RQOS", REGPOKER_READ32, NIC400_DMA2_S_RQOS, 0x07, 0xffffffff },
      { "NIC400_DMA2_S_WQOS", REGPOKER_READ32, NIC400_DMA2_S_WQOS, 0x08, 0xffffffff },
      { "NIC400_DMA2_S_FN_MOD", REGPOKER_READ32, NIC400_DMA2_S_FN_MOD, 0x00, 0xffffffff },
      { "NIC400_DMA3_S_RQOS", REGPOKER_READ32, NIC400_DMA3_S_RQOS, 0x0f, 0xffffffff },
      { "NIC400_DMA3_S_WQOS", REGPOKER_READ32, NIC400_DMA3_S_WQOS, 0x0d, 0xffffffff },
      { "NIC400_DMA3_S_FN_MOD", REGPOKER_READ32, NIC400_DMA3_S_FN_MOD, 0x00, 0xffffffff},
      {  }
    };

    struct regpoker_checker check_array_RWID64[] = {
          {" NIC400_ARM_S_RQOS", REGPOKER_READ32, NIC400_ARM_S_RQOS, 0x04, 0xffffffff },
          { "NIC400_ARM_S_WQOS", REGPOKER_READ32, NIC400_ARM_S_WQOS, 0x05, 0xffffffff },
          { "NIC400_ARM_S_FN_MOD", REGPOKER_READ32, NIC400_ARM_S_FN_MOD, 0x00, 0xffffffff },
          { "NIC400_PCIE_S_RQOS", REGPOKER_READ32, NIC400_PCIE_S_RQOS, 0x02, 0xffffffff },
          { "NIC400_PCIE_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_WQOS, 0x04, 0xffffffff },
          { "NIC400_PCIE_S_WQOS", REGPOKER_READ32, NIC400_PCIE_S_FN_MOD, 0x00, 0xffffffff },
          { "NIC400_DMA0_S_RQOS", REGPOKER_READ32, NIC400_DMA0_S_RQOS, 0x0c, 0xffffffff },
          { "NIC400_DMA0_S_WQOS", REGPOKER_READ32, NIC400_DMA0_S_WQOS, 0x0d, 0xffffffff },
          { "NIC400_DMA0_S_FN_MOD", REGPOKER_READ32, NIC400_DMA0_S_FN_MOD, 0x00, 0xffffffff },
          { "NIC400_DMA1_S_RQOS", REGPOKER_READ32, NIC400_DMA1_S_RQOS, 0x0a, 0xffffffff },
          { "NIC400_DMA1_S_WQOS", REGPOKER_READ32, NIC400_DMA1_S_WQOS, 0x0b, 0xffffffff },
          { "NIC400_DMA1_S_FN_MOD", REGPOKER_READ32, NIC400_DMA1_S_FN_MOD, 0x00, 0xffffffff },
          {  }
        };

    if(AXIx_check_ID_regs(PERIPHERAL_ID_BASE_32) ||
      (AXIx_check_ID_regs(PERIPHERAL_ID_BASE_64) ||
       rumboot_regpoker_check_array(check_array_RWID32, PERIPHERAL_ID_BASE_32) ||
       rumboot_regpoker_check_array (check_array_RWID64, PERIPHERAL_ID_BASE_64)) == 0)
    {
        rumboot_printf("TEST_OK\n");
        return 0;
    }

    return 1;
}
