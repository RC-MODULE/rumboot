#include <stdio.h>
#include <stdlib.h>

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <regs/regs_global_timers.h>
#include <regs/regs_mgeth.h>

#include <devices/mgeth.h>

struct rumboot_irq_entry *tbl;
volatile int int_f0 = 0, int_f1 = 0;

static void isr0(int irq, void *arg)
{
	rumboot_printf("isr0 start; irq = %d; arg = %d.\n", irq, arg);

	rumboot_printf("GLOBAL_STATUS (ETH0) = 0x%X\n", ioread32(ETH0_BASE + 0xC));
	rumboot_printf("MGETH_STATUS (ETH0) = 0x%X\n", ioread32(ETH0_BASE + 0x10));
	rumboot_printf("STATUS_R (ETH0) = 0x%X\n", ioread32(ETH0_BASE + 0xA18));

	int_f0 = 1;

	rumboot_printf("isr0 end.\n");
}

static void isr1(int irq, void *arg)
{
	rumboot_printf("isr1 start; irq = %d; arg = %d.\n", irq, arg);

	rumboot_printf("GLOBAL_STATUS (ETH1) = 0x%X\n", ioread32(ETH1_BASE + 0xC));
	rumboot_printf("MGETH_STATUS (ETH1) = 0x%X\n", ioread32(ETH1_BASE + 0x10));
	rumboot_printf("STATUS_W (ETH1) = 0x%X\n", ioread32(ETH1_BASE + 0x318));

	int_f1 = 1;

	rumboot_printf("isr1 end.\n");
}

int main()
{
	volatile uint8_t *frame_o, *frame_i;
	volatile uint32_t *desc_o, *desc_i;
	int i;

	rumboot_printf("================================================================================\n");

	rumboot_printf("Waiting SGMII initialization...\n");

    
    #ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
        init_gpio_and_en_eth_phy(MGPIO0_BASE, MDIO0_BASE, AN_EN);
    #endif
	if (mgeth_init_sgmii(SGMII_PHY, SCTL_BASE, AN_EN))
	{
		rumboot_printf("ERROR: SGMII initialization ERROR!\n");
		return 1;
	}

	rumboot_printf("SGMII initialized.\n");

	tbl = rumboot_irq_create(NULL);
	rumboot_irq_set_handler(tbl, MGETH0_IRQ, 0x0, isr0, (void *)990);
	rumboot_irq_set_handler(tbl, MGETH1_IRQ, 0x0, isr1, (void *)991);
	rumboot_irq_table_activate(tbl);
	rumboot_irq_enable(MGETH0_IRQ);
	rumboot_irq_enable(MGETH1_IRQ);
	rumboot_irq_sei();

	// Start timer
	iowrite32(0x1, GLOBAL_TIMERS + ENABLE);

	mgeth_reset(ETH0_BASE);
	mgeth_reset(ETH1_BASE);

	frame_o = rumboot_malloc_from_heap_aligned(0, 64, 8);
	if (!frame_o)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		return 1;
	}

	frame_i = rumboot_malloc_from_heap_aligned(0, 64, 8);
	if (!frame_i)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		rumboot_free((void *)frame_o);
		return 1;
	}

	desc_o = rumboot_malloc_from_heap_aligned(0, 16, 8);
	if (!desc_o)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		return 1;
	}

	desc_i = rumboot_malloc_from_heap_aligned(0, 16, 8);
	if (!desc_i)
	{
		rumboot_printf("ERROR: Insufficient memory!\n");
		rumboot_free((void *)frame_o);
		rumboot_free((void *)frame_i);
		rumboot_free((void *)desc_o);
		return 1;
	}

	for (i = 0; i < 12; i++)
		frame_o[i] = 0xFF;

	frame_o[12] = 0x08;
	frame_o[13] = 0x0;

	frame_o[14] = 0x45;
	frame_o[15] = 0x0;
	frame_o[16] = 0;//50;
	frame_o[17] = 50;//0;
	frame_o[18] = 0x0;
	frame_o[19] = 0x0;
	frame_o[20] = 0x0;
	frame_o[21] = 0x0;
	frame_o[22] = 64;
	frame_o[23] = 0x0;
	frame_o[24] = 0x0;
	frame_o[25] = 0x0;
	frame_o[26] = 0xFF;
	frame_o[27] = 0xFF;
	frame_o[28] = 0xFF;
	frame_o[29] = 0xFF;
	frame_o[30] = 0xFF;
	frame_o[31] = 0xFF;
	frame_o[32] = 0xFF;
	frame_o[33] = 0xFF;

	for (i = 34; i < 64; i++)
		frame_o[i] = i - 34;

	for (i = 0; i < 64; i++)
		frame_i[i] = 0x0;

	for (i = 0; i < 4; i++)
	{
		desc_o[i] = 0x0;
		desc_i[i] = 0x0;
	}

	desc_o[2] = (uint32_t)frame_o;
	desc_o[3] = 0x12000040;

	desc_i[2] = (uint32_t)frame_i;
	desc_i[3] = 0x10000040;

	iowrite32(0x00100012, ETH1_BASE + 0x310); // SETTINGS_W
	iowrite32(0x10, ETH1_BASE + 0x314); // IRQ_MASK_W
	iowrite32((uint32_t)desc_i, ETH1_BASE + 0x320); // DESC_ADDR_W

	iowrite32(0x00100012, ETH0_BASE + 0xA10); // SETTINGS_R
	iowrite32(0x10, ETH0_BASE + 0xA14); // IRQ_MASK_R
	iowrite32((uint32_t)desc_o, ETH0_BASE + 0xA20); // DESC_ADDR_R

	iowrite32(0x1, ETH1_BASE + 0x300); // ENABLE_W
	iowrite32(0x1, ETH0_BASE + 0xA00); // ENABLE_R

	rumboot_printf("Waiting of sending of output packet...\n");

	while (!int_f0);

	rumboot_printf("Output packet sended.\n");
	rumboot_printf("Waiting of receiving of input packet...\n");

	while (!int_f1);

	rumboot_printf("Input packet received.\n");

	rumboot_printf("desc_o:\n");
	for (i = 0; i < 4; i++)
		rumboot_printf("%X ", desc_o[i]);
	rumboot_printf("\n");

	rumboot_printf("desc_i:\n");
	for (i = 0; i < 4; i++)
		rumboot_printf("%X ", desc_i[i]);
	rumboot_printf("\n");

	rumboot_printf("frame_o:\n");
	for (i = 0; i < 64; i++)
		rumboot_printf("%X ", frame_o[i]);
	rumboot_printf("\n");

	rumboot_printf("frame_i:\n");
	for (i = 0; i < 64; i++)
		rumboot_printf("%X ", frame_i[i]);
	rumboot_printf("\n");

	rumboot_printf("GLOBAL_STATUS (ETH0) = 0x%X\n", ioread32(ETH0_BASE + 0xC));
	rumboot_printf("ENABLE_R (ETH0) = 0x%X\n", ioread32(ETH0_BASE + 0xA00));
	rumboot_printf("STATUS_R (ETH0) = 0x%X\n", ioread32(ETH0_BASE + 0xA18));
	rumboot_printf("CUR_DESC_ADDR_R (ETH0) = 0x%X\n", ioread32(ETH0_BASE + 0xA28));
	rumboot_printf("GLOBAL_STATUS (ETH1) = 0x%X\n", ioread32(ETH1_BASE + 0xC));
	rumboot_printf("ENABLE_W (ETH1) = 0x%X\n", ioread32(ETH1_BASE + 0x300));
	rumboot_printf("STATUS_W (ETH1) = 0x%X\n", ioread32(ETH1_BASE + 0x318));
	rumboot_printf("CUR_DESC_ADDR_W (ETH1) = 0x%X\n", ioread32(ETH1_BASE + 0x328));

	rumboot_printf("Common counters (ETH0):\n");
	for (i = 0x100; i <= 0x194; i += 4)
		if (!((i >= 0x15C) && (i <= 0x17C)))
			rumboot_printf("%X ", ioread32(ETH0_BASE + i));
	rumboot_printf("\n");

	rumboot_printf("Transmit counters (ETH0):\n");
	for (i = 0xAC0; i <= 0xAD4; i += 4)
		rumboot_printf("%X ", ioread32(ETH0_BASE + i));
	rumboot_printf("\n");

	rumboot_printf("Common counters (ETH1):\n");
	for (i = 0x100; i <= 0x194; i += 4)
		if (!((i >= 0x15C) && (i <= 0x17C)))
			rumboot_printf("%X ", ioread32(ETH1_BASE + i));
	rumboot_printf("\n");

	rumboot_printf("Receive counters (ETH1):\n");
	for (i = 0x3C0; i <= 0x3DC; i += 4)
		rumboot_printf("%X ", ioread32(ETH1_BASE + i));
	rumboot_printf("\n");

	for (i = 0; i < 64; i++)
		if ((i != 24) && (i != 25) && (frame_i[i] != frame_o[i]))
		{
			rumboot_printf("ERROR: Wrong byte %d!\n", i);
			rumboot_free((void *)frame_o);
			rumboot_free((void *)frame_i);
			rumboot_free((void *)desc_o);
			rumboot_free((void *)desc_i);
			return 1;
		}

	rumboot_free((void *)frame_o);
	rumboot_free((void *)frame_i);

	if (desc_i[1] < desc_o[1])
	{
		rumboot_printf("ERROR: Wrong timestamps!\n");
		rumboot_free((void *)desc_o);
		rumboot_free((void *)desc_i);
		return 1;
	}

	if ((desc_i[1] == desc_o[1]) && (desc_i[0] < desc_o[0]))
	{
		rumboot_printf("ERROR: Wrong timestamps!\n");
		rumboot_free((void *)desc_o);
		rumboot_free((void *)desc_i);
		return 1;
	}

	rumboot_free((void *)desc_o);
	rumboot_free((void *)desc_i);

	return 0;
}
