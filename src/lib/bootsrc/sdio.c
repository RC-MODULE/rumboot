#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include <rumboot/bootsrc/sdio.h>
#include <regs/regs_sdio.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>

#define BUFER_TRANS_START           0x04
#define DCDTR_1_VAL_512             0x200
#define DCCR_1_VAL                  0x00020F01

#define SDIO_TIMEOUT_US      2000000
#define SDIO_ATTEMPTS_NUMBER 5
#define DEBUG

/**
 * Card_type Type card enumeration
 */
enum Card_type {
	SDIO_CARD_UNKNOW = 0,
	SDIO_CARD_OLD,
	SDIO_CARD_SDHC,
	SDIO_CARD_SDSC,
	SDIO_CARD_SDXC,
};

struct sd_cid {
	unsigned char	mid;
	char		oid[2];
	char		pnm[5];
	unsigned char	prv_m : 4;
	unsigned char	prv_n : 4;
	uint32_t	psn;
	unsigned char	mdt_year_high : 4;
	unsigned char	reserved : 4;
	unsigned char	mdt_month : 4;
	unsigned char	mdt_year_low : 4;
	unsigned char	always1 : 1;
	unsigned char	crc : 7;
} __attribute__((packed));

enum Type_event {
	CH1_DMA_DONE_HANDLE = 0,
	CMD_DONE_HANDLE,
	TRAN_DONE_HANDLE,
	TRAN_FINISH_HANDLE,
};

struct Event {
	enum Type_event type;
	uint32_t	response;
	uint32_t	flag;
};

struct sd_private_data {
	const struct rumboot_bootsource *	src;
	enum Card_type				cardtype;
};

static inline uint32_t make_cmd(const uint32_t cmd, const uint32_t resp, const uint32_t crc, const uint32_t idx)
{
	return cmd << 16 | (idx << 12) | (crc << 13) | (resp << 10) | (0x11);
}

static bool wait(const uint32_t base, struct Event *event)
{
	uint32_t status;

	uint32_t start = rumboot_platform_get_uptime();

	do {
		status = ioread32(base + SDIO_INT_STATUS);

		if (status & SPISDIO_SDIO_INT_STATUS_CAR_ERR) {
			iowrite32(SDR_TRAN_SDC_ERR, base + SDIO_SDR_BUF_TRAN_RESP_REG);
			return false;
		} else if (status & event->flag) {
			(event->type == CH1_DMA_DONE_HANDLE) ?
			iowrite32(event->response, base + SDIO_DCCR_1) : iowrite32(event->response, base + SDIO_SDR_BUF_TRAN_RESP_REG);
			return true;
		}
	} while ((rumboot_platform_get_uptime() - start) < SDIO_TIMEOUT_US);

	return false;
}

static void debug_card_name(enum Card_type type)
{
	rumboot_printf("SD: ");
	switch (type) {
	case SDIO_CARD_SDHC:
		rumboot_printf("v2.0 SDHC");
		break;
	case SDIO_CARD_SDSC:
		rumboot_printf("v2.0 SDSC");
		break;
	case SDIO_CARD_OLD:
		rumboot_printf("v1.x SDSC");
		break;
	default:
		rumboot_printf("No or invalid card\n");
		break;
	}

	rumboot_printf("\n");
}

/*FIX THIS FUNC!*/
static void debug_product_name(const uint32_t base, struct sd_cid *cid)
{
	rumboot_printf("debug product name\n");

	uint32_t *cidptr = (uint32_t *)cid;

	cidptr[3] = ioread32(base + SDIO_SDR_RESPONSE1_REG);
	cidptr[2] = ioread32(base + SDIO_SDR_RESPONSE2_REG);
	cidptr[1] = ioread32(base + SDIO_SDR_RESPONSE3_REG);
	cidptr[0] = ioread32(base + SDIO_SDR_RESPONSE4_REG);

	size_t i;

	for (i = 0; i < 4; i++)
		rumboot_printf("%s", (char)cidptr[i]);

	for (i = 0; i < 2; i++)
		if (isalnum(cid->oid[i]))
			rumboot_printf("%s", cid->oid[i]);

	rumboot_printf(" ");

	for (i = 0; i < 5; i++)
		if (isalnum(cid->pnm[i]))
			rumboot_printf("%s", cid->pnm[i]);

	rumboot_printf(" detected \n");
}

static void debug_response(const uint32_t base)
{
	rumboot_printf("0x%x\n", ioread32(base + SDIO_SDR_RESPONSE1_REG));
	rumboot_printf("0x%x\n", ioread32(base + SDIO_SDR_RESPONSE2_REG));
	rumboot_printf("0x%x\n", ioread32(base + SDIO_SDR_RESPONSE3_REG));
	rumboot_printf("0x%x\n", ioread32(base + SDIO_SDR_RESPONSE4_REG));
}

static bool send_cmd(const uint32_t base, const uint32_t cmd_ctrl, const uint32_t arg)
{
#ifdef DEBUG
	rumboot_printf("SEND CMD 0x%x 0x%x\n", cmd_ctrl, arg);
#endif

	iowrite32(arg, base + SDIO_SDR_CMD_ARGUMENT_REG);
	iowrite32(cmd_ctrl, base + SDIO_SDR_CTRL_REG);

	struct Event cmd_event = {
		.type		= CMD_DONE_HANDLE,
		.flag		= SPISDIO_SDIO_INT_STATUS_CMD_DONE,
		.response	= SDR_TRAN_SDC_CMD_DONE
	};

	bool result = wait(base, &cmd_event);

#ifdef DEBUG
	debug_response(base);
#endif
	return result;
}

static inline uint32_t calc_divl(const uint32_t freq_khz)
{
	return (freq_khz / (0.4) / 2) - 1;
}

static inline uint32_t calc_divh(const uint32_t freq_khz)
{
	return (freq_khz / (10) / 2) - 1;
}

static inline struct sd_private_data *to_sd_private_data(void *pdata)
{
	return (struct sd_private_data *)pdata;
}

static void enable_interrupt(uint32_t base)
{
	iowrite32(0x7F, base + SPISDIO_SDIO_INT_MASKS);
	iowrite32(0x16F, base + SDIO_SDR_ERROR_ENABLE_REG);
}

bool sd_init(const struct rumboot_bootsource *src, void *pdata)
{
	size_t count = SDIO_ATTEMPTS_NUMBER;
	uint32_t resp;
	uint32_t arg;
	uint32_t divl = calc_divl(src->freq_khz);
	uint32_t divh = calc_divh(src->freq_khz);

	rumboot_printf("SD: init\n");

	iowrite32(divl, src->base + SPISDIO_SDIO_CLK_DIVIDE);
	iowrite32(0x1, src->base + SPISDIO_ENABLE);             //sdio-on, spi-off
	iowrite32((1 << 7), src->base + SDIO_SDR_CTRL_REG);     /* Hard reset damned hardware */

	while (ioread32(src->base + SDIO_SDR_CTRL_REG) & (1 << 7)) ;

	enable_interrupt(src->base);

	while (count--)
		send_cmd(src->base, make_cmd(0, SDIO_RESPONSE_NONE, 0, 0), 0);

	count = SDIO_ATTEMPTS_NUMBER;
	while (count--) {
		if (send_cmd(src->base, make_cmd(0, SDIO_RESPONSE_NONE, 0, 0), 0))
			break;

		mdelay(250);
	}

	struct sd_private_data *temp = to_sd_private_data(pdata);

	if (!send_cmd(src->base, make_cmd(8, SDIO_RESPONSE_R1367, 1, 1), 0x000001AA))
		temp->cardtype = SDIO_CARD_OLD;
	else if (ioread32(src->base + SDIO_SDR_RESPONSE1_REG) != 0x000001AA)
		temp->cardtype = SDIO_CARD_OLD;

	count = SDIO_ATTEMPTS_NUMBER;
	while (count--) {
		if (!send_cmd(src->base, make_cmd(55, SDIO_RESPONSE_R1367, 1, 1), 0x0))              /* CMD55 with RCA == 0 */
			continue;

		arg = 0x80100000;

		if (temp->cardtype != SDIO_CARD_OLD)             /* Do not set HC bit for old cards, it may confuse them */
			arg |= 0x40000000;

		if (!send_cmd(src->base, make_cmd(41, SDIO_RESPONSE_R1367, 1, 1), arg)) /* ACMD41, Tell the card we support SDHC */
			continue;

		resp = ioread32(src->base + SDIO_SDR_RESPONSE1_REG);

		if (resp & (1 << 31))
			continue; /* Card not ready, yet */

		if (temp->cardtype != SDIO_CARD_OLD)
			temp->cardtype = (resp & (1 << 30)) ? SDIO_CARD_SDHC : SDIO_CARD_SDSC;

		mdelay(250);
	}

	if (count == 0)
		return false;

	temp->src = src;

#ifdef DEBUG
	debug_card_name(temp->cardtype);
#endif

	if (!send_cmd(src->base, make_cmd(2, SDIO_RESPONSE_R2, 1, 1), 0x0)) /* Request CID */
		return false;

	struct sd_cid cid;
	debug_product_name(src->base, &cid);

	if (!send_cmd(src->base, make_cmd(3, SDIO_RESPONSE_R1367, 1, 1), 0x0))       /* Request RCA with CMD3 */
		return false;

	uint32_t rca = ioread32(src->base + SDIO_SDR_RESPONSE1_REG) >> 16;
	if (!send_cmd(src->base, make_cmd(7, SDIO_RESPONSE_R1367, 1, 1), (rca << 16))) /* Put SD card in transfer mode with CMD7 */
		return false;

	if (!send_cmd(src->base, make_cmd(55, SDIO_RESPONSE_R1367, 1, 1), (rca << 16))) /* CMD55 with real RCA */
		return false;

	if (!send_cmd(src->base, make_cmd(6, SDIO_RESPONSE_R1367, 1, 1), 0x2))     /* ACMD6 */
		return false;

	iowrite32(divh, src->base + SPISDIO_SDIO_CLK_DIVIDE);
	rumboot_printf("SD: Card ready!\n");

	return true;
}

void sd_deinit(void *pdata)
{
	rumboot_printf("SD: deinit\n");

	struct sd_private_data *sd_pdata = to_sd_private_data(pdata);
	const struct rumboot_bootsource *src = sd_pdata->src;

	uint32_t reg = ioread32(src->base + SPISDIO_ENABLE);

	iowrite32(reg & ~(1 << 1), src->base + SPISDIO_ENABLE);
}

static bool SD2buf512(const uint32_t base, const uint32_t buf_num, const uint32_t idx)
{
	iowrite32((buf_num << 2), base + SDIO_SDR_ADDRESS_REG);
	iowrite32(((1 << 8) | (1 << 0)), base + SDIO_SDR_CARD_BLOCK_SET_REG);
	iowrite32(idx, base + SDIO_SDR_CMD_ARGUMENT_REG);
	iowrite32(CMD17_CTRL, base + SDIO_SDR_CTRL_REG);

	struct Event cmd_event = {
		.type		= CMD_DONE_HANDLE,
		.flag		= SPISDIO_SDIO_INT_STATUS_CMD_DONE,
		.response	= SDR_TRAN_SDC_CMD_DONE
	};

	struct Event tran_event = {
		.type		= TRAN_DONE_HANDLE,
		.flag		= SPISDIO_SDIO_INT_STATUS_TRAN_DONE,
		.response	= SDR_TRAN_SDC_DAT_DONE
	};

	if (wait(base, &cmd_event) == false) {
		rumboot_printf("TIMEOUT:CMD DONE at function SD2buf512.\n");
		return false;
	}

	if (wait(base, &tran_event) == false) {
		rumboot_printf("TIMEOUT:TRANSFER DONE at function SD2buf512.\n");
		return false;
	}

	return true;
}

static int buf2axi(const uint32_t base, int buf_num, uint32_t dma_addr)
{
	const int size = DCDTR_1_VAL_512;

	iowrite32(BUFER_TRANS_START | buf_num, base + SDIO_BUF_TRAN_CTRL_REG);
	iowrite32(size, base + SDIO_DCDTR_1);
	iowrite32(dma_addr, base + SDIO_DCDSAR_1);
	iowrite32(DCCR_1_VAL, base + SDIO_DCCR_1);

	struct Event dma_event = {
		.type		= CH1_DMA_DONE_HANDLE,
		.flag		= SPISDIO_SDIO_INT_STATUS_CH1_FINISH,
		.response	= DSSR_CHANNEL_TR_DONE
	};

	struct Event event = {
		.type		= TRAN_FINISH_HANDLE,
		.response	= SDR_TRAN_FIFO_FINISH,
		.flag		= SPISDIO_SDIO_INT_STATUS_BUF_FINISH
	};

	if (!wait(base, &dma_event)) {
		rumboot_printf("TIMEOUT:DMA DONE at function buf2axi.\n");
		return -1;
	}

	if (!wait(base, &event)) {
		rumboot_printf("TIMEOUT:BUF TRAN FINISH at function buf2axi.\n");
		return -2;
	}

	return size;
}

int sd_read(void *pdata, void *ram, void *sd_addr)
{
	rumboot_printf("SD: read block\n");

	int ret;
	uint32_t buf_num = 0;
	struct sd_private_data *sd_pdata = to_sd_private_data(pdata);
	const struct rumboot_bootsource *src = sd_pdata->src;
	uint32_t full_addr = (uint32_t)sd_addr;
	uint32_t offset = full_addr - src->base;

	if (offset % 512 != 0)
		return -1;

	if (sd_pdata->cardtype == SDIO_CARD_SDHC)
		offset /= 512;

	if (!SD2buf512(src->base, buf_num, offset))
		return -2;

	ret = buf2axi(src->base, buf_num, (uint32_t)ram);

	return ret;
}

bool sd_load_again()
{
	return false;
}
