#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/can_adapter.h>
#include <regs/regs_gp_timers.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <platform/devices.h> 


int main()
{
    unsigned int read_data;
	rumboot_printf("Hello world from IRAM! This is can_adapter_2_test! \n");
    rumboot_printf("It checks connection between both CAN nodes and GP_GLOBAL_TIMERS \n");
    
    //Enable GP_TIMERS
    iowrite32(1, GLOBAL_TIMERS + ENABLE);
        
    //Enable change TR Timing Register
    iowrite32(0 << TME |  1 << TRCE,                      CAN0_BASE + CAN_MODE_REG );
    iowrite32(0 << TME |  1 << TRCE,                      CAN1_BASE + CAN_MODE_REG );
    
    // Write and check TR Timing Register value
    iowrite32(1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV, CAN0_BASE + TR_TIMING_REG); // BR = 1000 kb/s ( 64 MHz) 
    read_data=ioread32(CAN0_BASE + TR_TIMING_REG);
    while (read_data ^ (1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV))
     {read_data=ioread32(CAN0_BASE + TR_TIMING_REG);}
    rumboot_printf("CAN0 TR timing is correct!\n");    
    iowrite32(1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV, CAN1_BASE + TR_TIMING_REG); // BR = 1000 kb/s ( 64 MHz)
    read_data=ioread32(CAN1_BASE + TR_TIMING_REG);
    while (read_data ^ (1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV))
     {read_data=ioread32(CAN1_BASE + TR_TIMING_REG);}
    rumboot_printf("CAN1 TR timing is correct!\n");

    //Disable change TR Timing Register
    iowrite32(0 << TME |  0 << TRCE,                      CAN0_BASE + CAN_MODE_REG );
    iowrite32(0 << TME |  0 << TRCE,                      CAN1_BASE + CAN_MODE_REG );
    
    // Init mode OFF
    iowrite32(2 << IMCR, CAN0_BASE + IMCR_REG); 
    iowrite32(2 << IMCR, CAN1_BASE + IMCR_REG);
    
      
    //Init TQ0 CAN0
    iowrite32(0x00000001,CAN0_BASE + DATA_TQ0); // TT[31: 0]; TT = 10 us
    iowrite32(0x00000000,CAN0_BASE + DATA_TQ0); // TT[63:32];
    iowrite32(0x0FA00000,CAN0_BASE + DATA_TQ0); // IDE[31], bID[30:20], eID[19:2], RTR[1], unused[0]
    iowrite32(0x82000000,CAN0_BASE + DATA_TQ0); // DLC[31:28], SISH[27], ROF[26], TR[25], unused[24:0]
    iowrite32(0x3F2F1F0F,CAN0_BASE + DATA_TQ0); // Byte3[31:24], Byte2[23:16], Byte1[15:8], Byte0[7:0]
    iowrite32(0x7F6F5F4F,CAN0_BASE + DATA_TQ0); // Byte7[31:24], Byte6[23:16], Byte5[15:8], Byte4[7:0]
    iowrite32(0xAABBCCDD,CAN0_BASE + DATA_TQ0); // PID[31:0];
    iowrite32(0x00000000,CAN0_BASE + DATA_TQ0); // unused[31:0];
    
    //Wait Bus Idle
    read_data=ioread32(CAN0_BASE + STATUS_REG);
    while (!(read_data & 1 << BI))
     {read_data=ioread32(CAN0_BASE + STATUS_REG);}
    rumboot_printf("CAN0 Bus Idle!\n");
    
    read_data=ioread32(CAN1_BASE + STATUS_REG);
    while (!(read_data & 1 << BI))
     {read_data=ioread32(CAN1_BASE + STATUS_REG);}
    rumboot_printf("CAN1 Bus Idle!\n");
    
    //Wait Transmission Report (TR) in EQ0 (CAN 0)
    read_data=ioread32(CAN0_BASE + FC_EQ0);
    while (!(read_data == 0x00000001))
     {read_data=ioread32(CAN0_BASE + FC_EQ0);}
    rumboot_printf("CAN0 EQ has 1 frame!\n");
    
    //Read from EQ0
    if (ioread32(CAN0_BASE + DATA_EQ0) != TR_REPORT_ID)
    {
        rumboot_printf("Frame from EQ0 (CAN0) has wrong EventID!\n");
        return -1;
    }
    else
    {rumboot_printf("Frame from EQ0 (CAN0) has Transmission Report element:\n");}

    if (ioread32(CAN0_BASE + DATA_EQ0)  != TIMEOUT)
    {
        rumboot_printf("Frame from EQ0 (CAN0) has wrong TRSL!\n");
        return -1;
    }
    else
    {rumboot_printf("Frame from EQ0 (CAN0) has TRSL == TIMEOUT\n");}

    for (int i = 0; i<6; i=i+1)
    {
        ioread32(CAN0_BASE + DATA_EQ0);
    }
    //End of Read from EQ0 (CAN0)


    //Init TQ0 CAN1
    iowrite32(0x00000002,CAN1_BASE + DATA_TQ0); // TT[31: 0]; TT = 20 us
    iowrite32(0x00000000,CAN1_BASE + DATA_TQ0); // TT[63:32];
    iowrite32(0x0FA00000,CAN1_BASE + DATA_TQ0); // IDE[31], bID[30:20], eID[19:2], RTR[1], unused[0]
    iowrite32(0x82000000,CAN1_BASE + DATA_TQ0); // DLC[31:28], SISH[27], ROF[26], TR[25], unused[24:0]
    iowrite32(0x3F2F1F0F,CAN1_BASE + DATA_TQ0); // Byte3[31:24], Byte2[23:16], Byte1[15:8], Byte0[7:0]
    iowrite32(0x7F6F5F4F,CAN1_BASE + DATA_TQ0); // Byte7[31:24], Byte6[23:16], Byte5[15:8], Byte4[7:0]
    iowrite32(0x11223344,CAN1_BASE + DATA_TQ0); // PID[31:0];
    iowrite32(0x00000000,CAN1_BASE + DATA_TQ0); // unused[31:0];
    
    
    
    //Wait Transmission Report (TR) in EQ0 (CAN 1)
    read_data=ioread32(CAN1_BASE + FC_EQ0);
    while (!(read_data == 0x00000001))
     {read_data=ioread32(CAN1_BASE + FC_EQ0);}
    rumboot_printf("CAN1 EQ has 1 frame!\n");
    
    //Read from EQ0
    if (ioread32(CAN1_BASE + DATA_EQ0) != TR_REPORT_ID)
    {
        rumboot_printf("Frame from EQ0 (CAN1) has wrong EventID!\n");
        return -1;
    }
    else
    {rumboot_printf("Frame from EQ0 (CAN1) has Transmission Report element:\n");}

    if (ioread32(CAN1_BASE + DATA_EQ0)  != TIMEOUT)
    {
        rumboot_printf("Frame from EQ0 (CAN1) has wrong TRSL!\n");
        return -1;
    }
    else
    {rumboot_printf("Frame from EQ0 (CAN1) has TRSL == TIMEOUT\n");}

    for (int i = 0; i<6; i=i+1)
    {
        ioread32(CAN1_BASE + DATA_EQ0);
    }
    //End of Read from EQ0 (CAN1)

  
    
    rumboot_printf("Goodbye from can_adapter_2_test!\n");
	return 0;
}
