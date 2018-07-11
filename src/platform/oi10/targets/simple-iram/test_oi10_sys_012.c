
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/test_assert.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_nic400.h>

#include "test_oi10_sys_012.h"

//static uint32_t res;

static uint8_t AXI32_check_ID_regs()
{
    //check reserved registers
    rumboot_printf("Checking reserved ID registers default value ...\n");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32) == 0x00), "Reserved register default value incorrect");
    rumboot_printf("Base address: 0x%x\n", AXI32_CTRL);
    
    /*
    for(i=0x001; i<0xFCD; i <<= 1)
    {
        TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE + i) == 0), "Reserved register default value incorrect");
    }
    */
    rumboot_printf("Checking ID registers default value ...\n");
    
    /*peripheral_id*/
    ///read operations
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR4) == 0x04), "NIC400_PIDR4 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR5) == 0x00), "NIC400_PIDR5 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR6) == 0x00), "NIC400_PIDR6 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR7) == 0x00), "NIC400_PIDR7 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR0) == 0x00), "NIC400_PIDR0 register default value incorrect");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR1) == 0xB4), "PID_PERIPHERAL_ID1 register default value incorrect");
  //  rumboot_putstring("PID_PERIPHERAL_ID2 = ");
  //  rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR2));
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR2) == 0x5B), "PID_PERIPHERAL_ID2 register default value incorrect");
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
    //check reserved registers
    rumboot_printf("Checking reserved ID registers default value ...\n");
    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64) == 0x00), "Reserved register default value incorrect");
    
    /*
    for(i=0x001; i<0xFCD; i <<= 1)
    {
        TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE + i) == 0), "Reserved register default value incorrect");
    }
    */
    
    rumboot_printf("Checking ID registers default value ...\n");
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

static uint32_t AXI32_check(uint32_t interface_number)
{
    //rumboot_printf("Checking AXI registers default value for interface 0x%x \n", interface_number);
    /// NIC400_ARM_S 0x42100
    /// NIC400_ARM_S_RQOS 
    /// #define NIC400_ARM_S_RQOS NIC400_ARM_S+RQOS_OFFSET
    
    rumboot_printf("Address 0x%x\n", interface_number);
    rumboot_printf("Address interface_number 0x%x\n", NIC400_ARM_S);
    
    iowrite64(0x20, interface_number + NIC400_ARM_S + AMIB_SYNC_MODE);
    TEST_ASSERT((ioread64(interface_number + NIC400_ARM_S + AMIB_SYNC_MODE) == 0x020), "NIC400_ARM_S_RQOS register value incorrect");
    //rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_PIDR2));
    rumboot_putstring("AMIB_SYNC_MODE = ");
  //  iowrite8(0x20, interface_number +  AMIB_SYNC_MODE);
    rumboot_puthex(ioread32(interface_number + AMIB_SYNC_MODE));
    
   //rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA0_S_RQOS));
      
    /*
    rumboot_printf("Address 0x%x\n", interface_number);
    rumboot_printf("Address interface_number 0x%x\n", NIC400_DMA0_S);
    
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + WQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA0_S_RQOS));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + NIC400_DMA0_S_RQOS = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA0_S + RQOS_OFFSET));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + FN_MOD_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA0_S + FN_MOD_OFFSET));
    
    rumboot_printf("Address 0x%x\n", interface_number);
    rumboot_printf("Address interface_number 0x%x\n", NIC400_DMA1_S);
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + WQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA1_S + WQOS_OFFSET));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + RQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA1_S + RQOS_OFFSET));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + RQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA1_S + FN_MOD_OFFSET));
    
    rumboot_printf("Address 0x%x\n", interface_number);
    rumboot_printf("Address interface_number 0x%x\n", NIC400_DMA2_S);
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + RQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S + WQOS_OFFSET));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + RQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S + RQOS_OFFSET));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + FN_MOD_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S + FN_MOD_OFFSET));
    
    rumboot_printf("Address 0x%x\n", interface_number);
    rumboot_printf("Address interface_number 0x%x\n", NIC400_DMA3_S);
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + WQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA3_S + WQOS_OFFSET));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + RQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S + RQOS_OFFSET));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + FN_MOD_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S + FN_MOD_OFFSET));
    
    rumboot_printf("Address 0x%x\n", interface_number);

    rumboot_printf("Address interface_number 0x%x\n", NIC400_ETH0_S);
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + WQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_ETH0_S + WQOS_OFFSET));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + RQOS_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S + RQOS_OFFSET));
    rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + FN_MOD_OFFSET = \n");
    rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + NIC400_DMA2_S + FN_MOD_OFFSET));
    
   //rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + RQOS_OFFSET = ");
   //rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + WQOS_OFFSET));
    
   //rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + WQOS_OFFSET = ");
   //rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + WQOS_OFFSET));
    
 //   rumboot_putstring("PERIPHERAL_ID_BASE_32 + interface_number + FN_MOD_OFFSET = ");
 //   rumboot_puthex(ioread32(PERIPHERAL_ID_BASE_32 + FN_MOD_OFFSET));
    
   // TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + interface_number + RQOS_OFFSET) == 0x00), "RQOS_OFFSET register default value incorrect");
    
    //TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + interface_number + WQOS_OFFSET) == 0x04), "WQOS_OFFSET register default value incorrect");
    //TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_32 + interface_number + FN_MOD_OFFSET) == 0x08), "FN_MOD_OFFSET register default value incorrect");
    
   // rumboot_printf("Checking AXI registers write value for interface 0x%x\n", interface_number);
    
    //iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x00;
    
*/
    return 0;
}
/*
static uint32_t AXI64_check(uint32_t interface_number)
{
    //rumboot_printf("Checking AXI registers default value for interface 0x%x \n", interface_number);
    
    rumboot_printf("Address 0x%x\n", interface_number);
    rumboot_printf("Address interface_number 0x%x\n", NIC400_DMA0_S);
    
    rumboot_printf("Address 0x%x\n", interface_number);
    rumboot_printf("Address interface_number 0x%x\n", NIC400_DMA1_S);
    return 0;
}
*/
/*    
static uint8_t check_ARC_regs(uint32_t security_slaves_count) //functia not used
{
  ///  uint8_t i;
    
    //TEST_ASSERT(security_slaves_count < 64, "Only 64 security registers are allowed");
    
//    rumboot_printf("Checking ARC registers ...\n");
    //iowrite8(ARC_BASE + ARC_REMAP) = ~0x01; //zakomentil
   
    // iowrite8(~0x01, ARC_BASE + ARC_REMAP); //zakomentil
        
    for (i = 0; i < security_slaves_count; i++)
    {
        iowrite16(~(i*4), ARC_BASE + 0x08 + i*4);
    }

    //test_event(EVENT_CHECK_NIC301_ARC);
*/
//    return 0;
//}
/*NIC400_ARM_DMA
static uint8_t check_AMIB_regs(uint8_t interface_number) //functia not used 
{
    rumboot_printf("Checking AMIB registers default value for interface %x", interface_number);
    rumboot_printf(interface_number);
    
    TEST_ASSERT(ioread8(PERIPHERAL_ID_BASE_32 + interface_number + AMIB_FN_MOD_BM_ISS) == 0x00, "AMIB_FN_MOD_BM_ISS register default value incorrect");
    TEST_ASSERT(ioread8(PERIPHERAL_ID_BASE_32 + interface_number + AMIB_SYNC_MODE) == 0x04, "AMIB_SYNC_MODE register default value incorrect");
    TEST_ASSERT(ioread8(PERIPHERAL_ID_BASE_32 + interface_number + AMIB_FN_MOD2) == 0x00, "AMIB_FN_MOD2 register default value incorrect");
    TEST_ASSERT(ioread8(PERIPHERAL_ID_BASE_32 + interface_number + AMIB_WR_TIDEMARK) == 0xFF, "AMIB_WR_TIDEMARK register default value incorrect");     //must be defined via AMBA Designer
    TEST_ASSERT(ioread8(PERIPHERAL_ID_BASE_32 + interface_number + AMIB_AHB_CNTL) == 0x00, "AMIB_AHB_CNTL register default value incorrect");           //default value doesn't exist in technical reference manual
    TEST_ASSERT(ioread8(PERIPHERAL_ID_BASE_32 + interface_number + AMIB_FN_MOD) == 0x00, "AMIB_FN_MOD register default value incorrect");
    ///write registers
    rumboot_printf("Checking AMIB registers write value for interface %x", interface_number);s
    rumboot_printf(interface_number);
    iowrite8(0x00, PERIPHERAL_ID_BASE_32 + interface_number + AMIB_WR_TIDEMARK);
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x00, "AMIB_WR_TIDEMARK register read value incorrect");
    
    iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x01;
    
    iowrite8(0x01, AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK); //zakomentil
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x01, "AMIB_WR_TIDEMARK register read value incorrect");
    
    iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x02;
    
    iowrite8(0x02, AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK); zakomentil
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x02, "AMIB_WR_TIDEMARK register read value incorrect");
    
    iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x04;
    
    iowrite8(0x04, AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK); zakomentil
    
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x04, "AMIB_WR_TIDEMARK register read value incorrect");
    
    iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x08;
    
    iowrite8(0x08, AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK); zakomentil
    
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x08, "AMIB_WR_TIDEMARK register read value incorrect");

    iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x0F;
    
    iowrite8(0x0F, AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK); zakomentil
    
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x0F, "AMIB_WR_TIDEMARK register read value incorrect");

    iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x0E;
    
    iowrite8(0x0E, AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK); zakomentil
    
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x0E, "AMIB_WR_TIDEMARK register read value incorrect");

    iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x0D;
    
    iowrite8(0x0D, AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK); zakomentil
    
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x0D, "AMIB_WR_TIDEMARK register read value incorrect");

    iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x0B;
    
    iowrite8(0x0B, AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK); zakomentil
    
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x0B, "AMIB_WR_TIDEMARK register read value incorrect");

    iowrite8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) = 0x07;
    
    iowrite8(0x07, AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK); zakomentil
    
    TEST_ASSERT(ioread8(AMIB_BASE(interface_number) + AMIB_WR_TIDEMARK) == 0x07, "AMIB_WR_TIDEMARK register read value incorrect");

    return 0;
}
*/
//static uint8_t check_ASIB_regs(uint8_t interface_number, uint8_t isAHBSlave)
//{
 ///   uint8_t i;
//  register uint32_t _number = 0; //dobavil vremenno
    //Reserved registers test
//    rumboot_printf("Checking reserved ASIB registers default value for interface 0%x", _number);//interface_number
    //rumboot_printf(interface_number);
    
    //TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + 0x000) == 0x0, "Reserved register default value incorrect (expected 0x00)");
    
    /*
    for (i = 0x001; i < 0x010; i += 4)
        //TEST_ASSERT(iowrite8(ASIB_BASE(interface_number) + i) == 0x0, "Reserved register default value incorrect (expected 0x00)");
	TEST_ASSERT(iowrite8(0x0, ASIB_BASE(interface_number) + i) "Reserved register default value incorrect (expected 0x00)");
    */
    
    /*
    for (i = 0x02C; i < 0x040; i += 4)
        TEST_ASSERT(MEM8(ASIB_BASE(interface_number) + i) == 0x0, "Reserved register default value incorrect (expected 0x00)");

    for (i = 0x044; i < 0x100; i += 4)
        TEST_ASSERT(MEM8(ASIB_BASE(interface_number) + i) == 0x0, "Reserved register default value incorrect (expected 0x00)");

    for (i = 0x10C; i <= 0xFFC; i += 4)
        TEST_ASSERT(MEM8(ASIB_BASE(interface_number) + i) == 0x0, "Reserved register default value incorrect (expected 0x00)");
*/

  //  rumboot_printf("Checking ASIB registers default value for interface %x", interface_number);
    //rumboot_printf(interface_number);

    //sync_mode has not been used
    //TEST_ASSERT(MEM8(ASIB_BASE(interface_number) + ASIB_SYNC_MODE) == 0x04, "ASIB_SYNC_MODE register default value incorrect");

    //TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_FN_MOD2) == 0x0, "ASIB_FN_MOD2 register default value incorrect");

 //   if (isAHBSlave) //zakomentil
      //  TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_FN_MOD_AHB) == 0x00, "ASIB_FN_MOD_AHB register default value incorrect");

    //Programmable FIFO has not been used
    //TEST_ASSERT(MEM8(ASIB_BASE(interface_number) + ASIB_WR_TIDEMARK) == 0xFF, "ASIB_WR_TIDEMARK register default value incorrect");         //must be defined via AMBA Designer

    //TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x00, "ASIB_READ_QOS register default value incorrect");
    //TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_WRITE_QOS) == 0x00, "ASIB_WRITE_QOS register default value incorrect");
    //TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_FN_MOD) == 0x00, "ASIB_FN_MOD register default value incorrect");

    //rumboot_printf("Checking ASIB registers read/write for interface %x", interface_number);
    //ne rabotaet rumboot_puthex?
    //rumboot_printf(interface_number);

   // iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x00; //zakomentil
    
 //   iowrite8(0x00, ASIB_BASE(interface_number) + ASIB_READ_QOS); //zakomentil
    
 //   TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x00, "ASIB_READ_QOS register read value incorrect (expected 0x00)");

   // iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x01; //zakomentil
    
   // iowrite8(0x01, ASIB_BASE(interface_number) + ASIB_READ_QOS); //zakomentil
    
  //  TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x01, "ASIB_READ_QOS register read value incorrect (expected 0x01)");

    //iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x02;
    
    //iowrite8(0x02, ASIB_BASE(interface_number) + ASIB_READ_QOS); zakomentil
    
   // TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x02, "ASIB_READ_QOS register read value incorrect (expected 0x02)");

    //iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x04;
    
    //iowrite8    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR4) == 0x00), "PID_PERIPHERAL_ID4 register default value incorrect");

    
    //TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x04, "ASIB_READ_QOS register read value incorrect (expected 0x04)");

    //iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x08;
    
    //iowrite8(0x08, ASIB_BASE(interface_number) + ASIB_READ_QOS); zakomentil
    
    //TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x08, "ASIB_READ_QOS register read value incorrect (expected 0x08)");
    
    //iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x0F;
    
    //iowrite8(0x0F, ASIB_BASE(interface_number) + ASIB_READ_QOS); zakomentil
    
   // TEST_ASSERT(ioread8(ASIB_tBASE(interface_number) + ASIB_READ_QOS) == 0x0F, "ASIB_READ_QOS register read value incorrect (expected 0x0F)");
    
    //iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x0E;
    
    //iowrite8(0x0E, ASIB_BASE(interface_number) + ASIB_READ_QOS); zakomentil
    
    //TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x0E, "ASIB_READ_QOS register read value incorrect (expected 0x0E)");
    
    //iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x0D;
    
    //iowrite8(0x0D, ASIB_BASE(interface_number) + ASIB_READ_QOS); zakomentil
   
    // TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x0D, "ASIB_READ_QOS register read value incorrect (expected 0x0D)");

    //iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x0B;
    
    //iowrite8(0x0B, ASIB_BASE(interface_number) + ASIB_READ_QOS); zakomentil
   
    // TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x0B, "ASIB_READ_QOS register read value incorrect (expected 0x0B)");

    //iowrite8(ASIB_BASE(interface_number) + ASIB_READ_QOS) = 0x07;
    
    //iowrite8(0x07, ASIB_BASE(interface_number) + ASIB_READ_QOS); zakomentil
   
    // TEST_ASSERT(ioread8(ASIB_BASE(interface_number) + ASIB_READ_QOS) == 0x07, "ASIB_READ_QOS register read value incorrect (expected 0x07)");

//    return 0;
//}
 
//{
 ///   uint8_t i;

    //Reserved registers test
 //   rumboot_printf("Checking reserved IB registers default value for interface %x", interface_number); //zakomentil
    //rumboot_printf();
/*
    TEST_ASSERT(ioread8(IB_BASE(interface_number) + 0x000) == 0x0, "Reserved register default value incorrect (expected 0x00)");
    for (i = 0x001; i < 0x008; i += 4)
        TEST_ASSERT(iowrite8(IB_BASE(interface_number) + i) == 0x0, "Reserved register default value incorrect (expected 0x00)");
*/
//    rumboot_printf("Checking IB registers default value for interface %x", interface_number);
    //rumboot_printf(interface_number);

  //  TEST_ASSERT(ioread8(IB_BASE(interface_number) + IB_FN_MOD_BM_ISS) == 0x00, "IB_FN_MOD_BM_ISS register default value incorrect");

    //sync_mode has not been used
    //TEST_ASSERT(MEM8(IB_BASE(interface_number) + IB_SYNC_MODE) == 0x04, "IB_SYNC_MODE register default value incorrect");

  //  TEST_ASSERT(ioread8(IB_BASE(interface_number) + IB_FN_MOD2) == 0x00, "IB_FN_MOD2 register default value incorrect");

    //Programm    TEST_ASSERT((ioread32(PERIPHERAL_ID_BASE_64 + NIC400_PIDR4) == 0x00), "PID_PERIPHERAL_ID4 register default value incorrect");
    //TEST_ASSERT(MEM8(IB_BASE(interface_number) + IB_WR_TIDEMARK) == 0xFF, "IB_WR_TIDEMARK register default value incorrect");         //must be defined via AMBA Designer

//    TEST_ASSERT(ioread8(IB_BASE(interface_number) + IB_FN_MOD) == 0x00, "IB_FN_MOD register default value incorrect");

//    rumboot_printf("Checking IB registers read/write for interface %x", interface_number);
    //rumboot_printf(interface_number);

    //iowrite8(IB_BASE(interface_number) + IB_FN_MOD) = 0x00; zakomentil
    
    //TEST_ASSERT(ioread8(IB_BASE(interface_number) + IB_FN_MOD) == 0x00, "IB_FN_MOD register read value incorrect");

    //iowrite8(IB_BASE(interface_number) + IB_FN_MOD) = 0x01; //zakomentil
    
    //TEST_ASSERT(ioread8(IB_BASE(interface_number) + IB_FN_MOD) == 0x01, "IB_FN_MOD register read value incorrect");

  //  iowrite8(IB_BASE(interface_number) + IB_FN_MOD) = 0x02; zakomentil
    
    //TEST_ASSERT(ioread8(IB_BASE(interface_number) + IB_FN_MOD) == 0x02, "IB_FN_MOD register read value incorrect");

/*
    MEM8(IB_BASE(interface_number) + IB_FN_MOD) = 0x04;
    TEST_ASSERT(MEM8(IB_BASE(interface_number) + IB_FN_MOD) == 0x04, "IB_FN_MOD register read value incorrect");

    MEM8(IB_BASE(interface_number) + IB_FN_MOD) = 0x08;
    TEST_ASSERT(MEM8(IB_BASE(interface_number) + IB_FN_MOD) == 0x08, "IB_FN_MOD register read value incorrect");
*/
/*
    MEM8(IB_BASE(interface_number) + IB_FN_MOD) = 0x0F;
    TEST_ASSERT(MEM8(IB_BASE(interface_number) + IB_FN_MOD) == 0x0F, "IB_FN_MOD register read value incorrect");

    MEM8(IB_BASE(interface_number) + IB_FN_MOD) = 0x0E;
    TEST_ASSERT(MEM8(IB_BASE(interface_number) + IB_FN_MOD) == 0x0E, "IB_FN_MOD register read value incorrect");

    MEM8(IB_BASE(interface_number) + IB_FN_MOD) = 0x0D;
    TEST_ASSERT(MEM8(IB_BASE(interface_number) + IB_FN_MOD) == 0x0D, "IB_FN_MOD register read value incorrect");

    MEM8(IB_BASE(interface_number) + IB_FN_MOD) = 0x0B;
    TEST_ASSERT(MEM8(IB_BASE(interface_number) + IB_FN_MOD) == 0x0B, "IB_FN_MOD register read value incorrect");

    MEM8(IB_BASE(interface_number) + IB_FN_MOD) = 0x07;
    TEST_ASSERT(MEM8(IB_BASE(interface_number) + IB_FN_MOD) == 0x07, "IB_FN_MOD register read value incorrect");
*/
    // iowrite8(IB_BASE(interface_number) + IB_FN_MOD) = 0x03; zakomentil
    
    //TEST_ASSERT(ioread8(IB_BASE(interface_number) + IB_FN_MOD) == 0x03, "IB_FN_MOD register read value incorrect");

//    return 0;
//}

int main(void)
{ 
   register uint8_t result;
   result = AXI32_check_ID_regs() && AXI64_check_ID_regs()
   && AXI32_check(PERIPHERAL_ID_BASE_32);
  // && AXI64_check(PERIPHERAL_ID_BASE_64);
   
  //&& AXI32_check(NIC400_DMA0_S); 
/*
   &&
             check_ARC_regs() &&
             check_AMIB_regs(0) &&
             check_ASIB_regs(66, 0) &&
             check_ASIB_regs(67, 0) &&
             check_ASIB_regs(68, 0) &&
             check_ASIB_regs(69, 0) &&
             check_ASIB_regs(70, 0) &&
             check_ASIB_regs(71, 0) &&
             check_ASIB_regs(72, 0) &&
             check_ASIB_regs(73, 0) &&
             check_ASIB_regs(74, 0) &&
             check_ASIB_regs(75, 0) &&
             check_ASIB_regs(76, 0) &&
             check_ASIB_regs(77, 0) &&
             check_ASIB_regs(78, 0) &&
             check_ASIB_regs(79, 0) &&
             check_ASIB_regs(80, 0) &&
             check_ASIB_regs(81, 0) &&
             check_ASIB_regs(82, 0) &&
             check_ASIB_regs(83, 0) &&
             check_ASIB_regs(84, 0) &&
             check_ASIB_regs(85, 0); &&
             check_IB_regs(194)        &&
             check_IB_regs(195)        &&
             check_IB_regs(196);
*/
    if(result)
    {
	return 0;
    }
    
    return 1;
    //return (result) ? 0 : 1;
}


