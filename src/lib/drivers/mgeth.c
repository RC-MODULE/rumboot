#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_mgeth.h>
#include <devices/mdma_chan_api.h>
#include <devices/mgeth.h>

void mgeth_init_sgmii(uint32_t sgmii_base_addr, uint32_t sctl_base_addr)
{
	uint32_t
		read_data,
		OFFSET_SPCS_0, OFFSET_SPCS_1, OFFSET_SPCS_2, OFFSET_SPCS_3,
		OFFSET_TXS_0, OFFSET_TXS_1, OFFSET_TXS_2, OFFSET_TXS_3,
		OFFSET_CM,
		OFFSET_RXS_0, OFFSET_RXS_1, OFFSET_RXS_2, OFFSET_RXS_3;

	DBG_print("Start.");

	OFFSET_SPCS_0 = sgmii_base_addr + 0x0200;
	OFFSET_SPCS_1 = sgmii_base_addr + 0x0600;
	OFFSET_SPCS_2 = sgmii_base_addr + 0x0A00;
	OFFSET_SPCS_3 = sgmii_base_addr + 0x0E00;
	OFFSET_TXS_0  = sgmii_base_addr + 0x0000;
	OFFSET_TXS_1  = sgmii_base_addr + 0x0400;
	OFFSET_TXS_2  = sgmii_base_addr + 0x0800;
	OFFSET_TXS_3  = sgmii_base_addr + 0x0C00;
	OFFSET_CM     = sgmii_base_addr + 0x1000;
	OFFSET_RXS_0  = sgmii_base_addr + 0x0100;
	OFFSET_RXS_1  = sgmii_base_addr + 0x0500;
	OFFSET_RXS_2  = sgmii_base_addr + 0x0900;
	OFFSET_RXS_3  = sgmii_base_addr + 0x0D00;

	DBG_print("=== config SGMII_PHY ===");
	DBG_print("=== config SGMII_PHY OFFSET_SPCS ===");

	iowrite32(0x00000140, OFFSET_SPCS_0 + 0x00);
	iowrite32(0x00000140, OFFSET_SPCS_1 + 0x00);
	iowrite32(0x00000140, OFFSET_SPCS_2 + 0x00);
	iowrite32(0x00000140, OFFSET_SPCS_3 + 0x00);

	DBG_print("=== config SGMII_PHY OFFSET_TXS ===");

	iowrite32(0x40803004, OFFSET_TXS_0 + 0x00);
	iowrite32(0x40803004, OFFSET_TXS_1 + 0x00);
	iowrite32(0x40803004, OFFSET_TXS_2 + 0x00);
	iowrite32(0x40803004, OFFSET_TXS_3 + 0x00);

	DBG_print("=== config SGMII_PHY OFFSET_CM ===");

	iowrite32(0x00130000, OFFSET_CM + 0x04);
	iowrite32(0x710001F0, OFFSET_CM + 0x08);
	iowrite32(0x00000002, OFFSET_CM + 0x0C);
	iowrite32(0x07000000, OFFSET_CM + 0x20);

	DBG_print("=== config SGMII_PHY OFFSET_RXS ===");

	iowrite32(0x0000CEA6, OFFSET_RXS_0 + 0x08);
	iowrite32(0x0000CEA6, OFFSET_RXS_1 + 0x08);
	iowrite32(0x0000CEA6, OFFSET_RXS_2 + 0x08);
	iowrite32(0x0000CEA6, OFFSET_RXS_3 + 0x08);

	DBG_print("=== SGMII_PHY enable ===");

	iowrite32(0x1, sctl_base_addr + 0x14);

	read_data = 0x0;

	DBG_print("=== wait SGMII PLL_LOCK & SGMII PHY_RDY ===");

	while (read_data != 0x000001F1)
	{
		read_data = ioread32(sctl_base_addr + 0x14);

		DBG_print("SCTL register SGMII_CTRL_STAT: 0x%X", read_data);
	}

	DBG_print("End.");
}

void mgeth_reset(uint32_t base_addr)
{
	DBG_print("Start; base_addr = 0x%X.", base_addr);

	iowrite32(0x1, base_addr + MGETH_SW_RST);

	while (ioread32(base_addr + MGETH_SW_RST) & 0x1);

	DBG_print("End.");
}

void mgeth_init(const uint32_t base, const struct mgeth_conf *conf)
{
	uint32_t ctrl, ctrl_set = 0x0;

	DBG_print("Start; base = 0x%X.", base);

	if (conf->is_full_duplex)
		ctrl_set |= 0x1; // full_duplex (0 bit)

	ctrl_set |= conf->speed << 1; // speed (2 - 1 bits)

	DBG_print("ctrl_set = 0x%X", ctrl_set);

	ctrl = ioread32(base + MGETH_CONTROL);

	DBG_print("ctrl = 0x%X", ctrl);

	ctrl &= ~0x7;
	ctrl |= ctrl_set;

	DBG_print("ctrl = 0x%X", ctrl);

	iowrite32(ctrl, base + MGETH_CONTROL);

	DBG_print("End.");
}

struct mdma_chan *mgeth_transfer(uint32_t base, void *data, size_t len, bool send)
{
	struct mdma_chan *chan;
	struct mdma_cfg cfg;
	uint32_t off = send ? MGETH_SEND_CH_0 : MGETH_RECV_CH_0;
	int ret;

	DBG_print("Start; base = 0x%X; send = %d.", base, send);

	cfg.mode = MDMA_CHAN_POLLING;
	cfg.desc_kind = LONG_DESC;
	cfg.heap_id = 0;
	cfg.burst_width = MDMA_BURST_WIDTH4; // Fixed for ETH
	cfg.event_indx = -1; // Fixed for ETH
	cfg.add_info = true; // Fixed for ETH (for custom)
	cfg.addr_inc = true;

	chan = mdma_chan_create((void *)(base + off), &cfg);
	if (!chan)
	{
		DBG_print("ERROR: Failed create DMA channel!");
		DBG_print("End (With ERROR!); Return: NULL!");
		return NULL;
	}

	ret = mdma_trans_create(chan, MDMA_TRANS_QUEUE);
	if (ret)
	{
		DBG_print("ERROR: Failed create DMA transaction (ret = %d)!", ret);
		mdma_chan_destroy(chan);
		DBG_print("End (With ERROR!); Return: NULL!");
		return NULL;
	}

	ret = mdma_trans_add_group(chan, data, len, 0x800, true);
	if (ret)
	{
		DBG_print("ERROR: Failed add group to DMA transfer (ret = %d)!", ret);
		mdma_chan_destroy(chan);
		DBG_print("End (With ERROR!); Return: NULL!");
		return NULL;
	}

	ret = mdma_chan_config(chan);
	if (ret)
	{
		DBG_print("ERROR: Failed DMA channel configuration (ret = %d)!", ret);
		mdma_chan_destroy(chan);
		DBG_print("End (With ERROR!); Return: NULL!");
		return NULL;
	}

	ret = mdma_chan_start(chan);
	if (ret)
	{
		DBG_print("ERROR: Failed start DMA channel (ret = %d)!", ret);
		mdma_chan_destroy(chan);
		DBG_print("End (With ERROR!); Return: NULL!");
		return NULL;
	}

	DBG_print("End; Return: 0x%X.", chan);

	return chan;
}

struct mdma_chan *mgeth_transmit(uint32_t base, void *data, size_t len)
{
	return mgeth_transfer(base, data, len, true);
}

struct mdma_chan *mgeth_receive(uint32_t base, void *data, size_t len)
{
	return mgeth_transfer(base, data, len, false);
}

int mgeth_wait_transfer_complete(struct mdma_chan *chan, int timeout)
{
	int ret;

	DBG_print("Start; chan = 0x%X; timeout = %d.", chan, timeout);

	ret = mdma_chan_wait_trans_end(chan, timeout);
	if (ret)
	{
		DBG_print("ERROR: Failed wait transfer complete (ret = %d)!", ret);
		DBG_print("End (With ERROR!); Return: %d!", ret);
		return ret;
	}

	DBG_print("End; Return: %d.", ret);

	return ret;
}

int mgeth_finish_transfer(struct mdma_chan *chan)
{
	int ret;

	DBG_print("Start; chan = 0x%X.", chan);

	ret = mdma_chan_reset(chan);
	if (ret)
		DBG_print("WARNING: Failed reset DMA channel (ret = %d)!", ret);

	mdma_chan_destroy(chan);

	DBG_print("End; Return: %d.", ret);

	return ret;
}
