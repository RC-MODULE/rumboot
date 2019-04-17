#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/can_adapter.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>


int main()
{
    unsigned int read_data;
	rumboot_printf("Hello world from IRAM! This is can_adapter_0_test!: %d\n");

    //Bus status enable init
    iowrite32(1 <<   BI | 1 << BOR | 1 << BO   | 1 << EPEA | 1 << EAEP | 1 << EWRN | 1 << RER | 1 << TER | 1 << LARB, CAN0_BASE + BUS_STAT_EN_REG);
    iowrite32(1 <<   BI | 1 << BOR | 1 << BO   | 1 << EPEA | 1 << EAEP | 1 << EWRN | 1 << RER | 1 << TER | 1 << LARB, CAN1_BASE + BUS_STAT_EN_REG);

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

    /*//TR_TIMING_REG Read
    if (ioread32(CAN1_BASE + TR_TIMING_REG) != (1 << TS2 | 12 << TS1 | 0 << SJW | 3 << DIV))
    {rumboot_printf("Incorrect TR_TIMING_REG value!: %d\n");
        return -1;}*/

    //Init TQ0 CAN0
    iowrite32(0xFFFFFFFF,CAN0_BASE + DATA_TQ0); // TT[31: 0]; TT = max
    iowrite32(0xFFFFFFFF,CAN0_BASE + DATA_TQ0); // TT[63:32];
    iowrite32(0x0FA00000,CAN0_BASE + DATA_TQ0); // IDE[31], bID[30:20], eID[19:2], RTR[1], unused[0]
    iowrite32(0x80000000,CAN0_BASE + DATA_TQ0); // DLC[31:28], SISH[27], ROF[26], TR[25], unused[24:0]
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

    //Read from EQ0
    if (ioread32(CAN1_BASE + DATA_EQ0) != BUS_STATUS_ID)
    {
        rumboot_printf("EQ0 in CAN1 element has wrong EventID!\n");
        return -1;
    }
    else
    {rumboot_printf("EQ0 in CAN1 has Bus Status element:\n");}

    if ((ioread32(CAN1_BASE + DATA_EQ0)&0x0000000F) != BUS_IDLE)
    {
        rumboot_printf("EQ0 in CAN1 Bus status has wrong BSC!\n");
        return -1;
    }
    else
    {rumboot_printf("EQ0 in CAN1 has BSC == BUS_IDLE\n");}

    for (int i = 0; i<6; i=i+1)
    {
        ioread32(CAN1_BASE + DATA_EQ0);
    }
    //End of Read from EQ0

    //Wait Tx Ok
    read_data=ioread32(CAN0_BASE + STATUS_REG);
     //while (!(read_data & 1 << TXOK))
     while (!(read_data == (1 << BI | 1 << TXOK)))
     {read_data=ioread32(CAN0_BASE + STATUS_REG);}
    rumboot_printf("CAN0 Tx Ok!\n");

    //Wait Rx Ok
    read_data=ioread32(CAN1_BASE + STATUS_REG);
     //while (!(read_data & 1 << RXOK))
     while (!(read_data == (1 << BI | 1 << RXOK)))
     {read_data=ioread32(CAN1_BASE + STATUS_REG);}

    //Read Received Message from CAN1
    if (ioread32(CAN1_BASE + DATA_EQ0) != REC_MSG_ID)
    {
        rumboot_printf("EQ0 in CAN1 element has wrong EventID!\n");
        return -1;
    }
    else
    {rumboot_printf("EQ0 in CAN1 has Rec Msg element:\n");}

    ioread32(CAN1_BASE + DATA_EQ0);

    if (ioread32(CAN1_BASE + DATA_EQ0) != 0x0FA00000)
    {
        rumboot_printf("Rec Msg in CAN1 element has wrong baseID!\n");
        return -1;
    }
    else
    {rumboot_printf("Rec Msg in CAN1 has correct baseID.\n");}

    rumboot_printf("CAN1 Rx Ok!\n");

//read_data=ioread32(GSPI0_BASE + 0x0d4);
//rumboot_printf("example_ok\n");
// while (!(read_data&0x008))
// {read_data=ioread32(GSPI0_BASE + 0x0d4);}



    rumboot_printf("Goodbye from can_adapter_0_test!\n");
	return 0;
}
