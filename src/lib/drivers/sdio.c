#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include <platform/sdio.h>
#include <devices/sdio.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>
#include <platform/devices.h>
#include <rumboot/bootsource.h>

#define SDIO_TIMEOUT_US      2000000 //in microseconds
#define SDIO_ATTEMPTS_NUMBER 5
#define SDIO_DEBUG


static inline uint32_t make_cmd(const uint32_t cmd, const uint32_t resp, const uint32_t crc, const uint32_t idx) {

  return (cmd << 16 | (idx << 12) | (crc << 13) | (resp << 10) | (0x11));
}

static bool wait(const uint32_t base, struct Event *event)
{
	uint32_t status;

	uint32_t start = ucurrent();

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
	} while ((ucurrent() - start) < SDIO_TIMEOUT_US);

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
		rumboot_printf("No or invalid card\r\n");
		break;
	}

	rumboot_printf(" ");
}

static void debug_product_name(const uint32_t base, struct sd_cid *cid)
{
	uint32_t *cidptr = (uint32_t *)cid;

	cidptr[3] = ioread32(base + SDIO_SDR_RESPONSE1_REG);
	cidptr[2] = ioread32(base + SDIO_SDR_RESPONSE2_REG);
	cidptr[1] = ioread32(base + SDIO_SDR_RESPONSE3_REG);
	cidptr[0] = ioread32(base + SDIO_SDR_RESPONSE4_REG);

	size_t i;
	for (i = 0; i < 2; i++)
		if (isalnum(cid->oid[i]))
			rumboot_printf("%s", cid->oid[i]);

	rumboot_printf(" ");

	for (i = 0; i < 5; i++)
		if (isalnum(cid->pnm[i]))
			rumboot_printf("%s", cid->pnm[i]);

	rumboot_printf(" detected \r\n");
}

static void debug_response(const uint32_t base)
{
	rumboot_printf("0x%x\r\n", ioread32(base + SDIO_SDR_RESPONSE1_REG));
	rumboot_printf("0x%x\r\n", ioread32(base + SDIO_SDR_RESPONSE2_REG));
	rumboot_printf("0x%x\r\n", ioread32(base + SDIO_SDR_RESPONSE3_REG));
	rumboot_printf("0x%x\r\n", ioread32(base + SDIO_SDR_RESPONSE4_REG));
}

static bool SD2buf512(const uint32_t base, int buf_num, const uint32_t idx)
{
	iowrite32((buf_num << 2), base + SDIO_SDR_ADDRESS_REG);
	iowrite32(BLOCK_512_DATA_TRANS, base + SDIO_SDR_CARD_BLOCK_SET_REG);
	iowrite32(idx, base + SDIO_SDR_CMD_ARGUMENT_REG);
	iowrite32(CMD17_CTRL, base + SDIO_SDR_CTRL_REG);

	struct Event cmd_event;
	cmd_event.type = CMD_DONE_HANDLE;
	cmd_event.flag = SPISDIO_SDIO_INT_STATUS_CMD_DONE;
	cmd_event.response = SDR_TRAN_SDC_CMD_DONE;

	if (wait(base, &cmd_event) == false) {
		rumboot_printf("TIMEOUT:CMD DONE at function SD2buf512.\r\n");
		return false;
	}

	struct Event tran_event;
	tran_event.type = TRAN_DONE_HANDLE;
	tran_event.flag = SPISDIO_SDIO_INT_STATUS_TRAN_DONE;
	tran_event.response = SDR_TRAN_SDC_DAT_DONE;

	if (wait(base, &tran_event) == false) {
		rumboot_printf("TIMEOUT:TRANSFER DONE at function SD2buf512.\r\n");
		return false;
	}

	return true;
}

static bool buf2axi(const uint32_t base, int buf_num, uint32_t dma_addr)
{
	iowrite32(BUFER_TRANS_START | buf_num, base + SDIO_BUF_TRAN_CTRL_REG);
	iowrite32(DCDTR_1_VAL_512, base + SDIO_DCDTR_1);
	iowrite32(dma_addr, base + SDIO_DCDSAR_1);
	iowrite32(DCCR_1_VAL, base + SDIO_DCCR_1);

	struct Event dma_event;
	dma_event.type = CH1_DMA_DONE_HANDLE;
	dma_event.flag = SPISDIO_SDIO_INT_STATUS_CH1_FINISH;
	dma_event.response = DSSR_CHANNEL_TR_DONE;

	if (!wait(base, &dma_event)) {
		rumboot_printf("TIMEOUT:DMA DONE at function buf2axi.\r\n");
		return false;
	}

	struct Event event;
	event.type = TRAN_FINISH_HANDLE;
	event.response = SDR_TRAN_FIFO_FINISH;
	event.flag = SPISDIO_SDIO_INT_STATUS_BUF_FINISH;
	if (!wait(base, &event)) {
		rumboot_printf("TIMEOUT:BUF TRAN FINISH at function buf2axi.\r\n");
		return false;
	}
	return true;
}

static bool send_cmd(const uint32_t base, const uint32_t cmd_ctrl, const uint32_t arg)
{
#ifdef SDIO_DEBUG
	rumboot_printf("SEND CMD 0x%x 0x%x\r\n", cmd_ctrl, arg);
#endif

	iowrite32(arg, base + SDIO_SDR_CMD_ARGUMENT_REG);
	iowrite32(cmd_ctrl, base + SDIO_SDR_CTRL_REG);

	struct Event cmd_event;
	cmd_event.type = CMD_DONE_HANDLE;
	cmd_event.flag = SPISDIO_SDIO_INT_STATUS_CMD_DONE;
	cmd_event.response = SDR_TRAN_SDC_CMD_DONE;

	bool result = wait(base, &cmd_event);

#ifdef SDIO_DEBUG
	debug_response(base);
#endif
	return result;
}

static inline uint32_t calc_divl(const uint32_t freq_in_mhz)
{
	return (freq_in_mhz / (0.4) / 2) - 1;
}

static inline uint32_t calc_divh(const uint32_t freq_in_mhz)
{
	return (freq_in_mhz / (10) / 2) - 1;
}

bool sd_init(struct bdata* cfg)
{
	size_t count = SDIO_ATTEMPTS_NUMBER;
	uint32_t resp;
	uint32_t arg;
	uint32_t divl = calc_divl(cfg->freq_in_mhz);
	uint32_t divh = calc_divh(cfg->freq_in_mhz);

	rumboot_printf("SD: init\r\n");

	iowrite32(divl, cfg->base + SPISDIO_SDIO_CLK_DIVIDE);
	iowrite32(0x1, cfg->base + SPISDIO_ENABLE);          //sdio-on, spi-off
	iowrite32((1 << 7), cfg->base + SDIO_SDR_CTRL_REG);  /* Hard reset damned hardware */

	while (ioread32(cfg->base + SDIO_SDR_CTRL_REG) & (1 << 7)) ;

	iowrite32(0x7F, cfg->base + SPISDIO_SDIO_INT_MASKS);         //enable  interrupts in SDIO
	iowrite32(0x16F, cfg->base + SDIO_SDR_ERROR_ENABLE_REG);     //enable interrupt and error flag

	while (count--) {
		if (send_cmd(cfg->base, make_cmd(0, SDIO_RESPONSE_NONE, 0, 0), 0))
			break;

		mdelay(250);
	}

	if (!send_cmd(cfg->base, make_cmd(8, SDIO_RESPONSE_R1367, 1, 1), 0x000001AA))
		cfg->out_param[0] = SDIO_CARD_OLD;
	else if (ioread32(cfg->base + SDIO_SDR_RESPONSE1_REG) != 0x000001AA)
		cfg->out_param[0] = SDIO_CARD_OLD;

	count = SDIO_ATTEMPTS_NUMBER;
	while (count--) {
		if (!send_cmd(cfg->base, make_cmd(55, SDIO_RESPONSE_R1367, 1, 1), 0x0))              /* CMD55 with RCA == 0 */
			continue;

		arg = 0x80100000;

		if (cfg->out_param[0] != SDIO_CARD_OLD)             /* Do not set HC bit for old cards, it may confuse them */
			arg |= 0x40000000;

		if (!send_cmd(cfg->base, make_cmd(41, SDIO_RESPONSE_R1367, 1, 1), arg)) /* ACMD41, Tell the card we support SDHC */
			continue;

		resp = ioread32(cfg->base + SDIO_SDR_RESPONSE1_REG);

		if ((resp >> 31) != 1)
			continue; /* Card not ready, yet */

		if (cfg->out_param[0] != SDIO_CARD_OLD)
			cfg->out_param[0] = ((resp & 0x40000000) >> 30) ? SDIO_CARD_SDHC : SDIO_CARD_SDSC;

		mdelay(250);
	}

	if (count == 0)
		return false;

#ifdef SDIO_DEBUG
	debug_card_name(cfg->out_param[0]);
#endif

	if (!send_cmd(cfg->base, make_cmd(2, SDIO_RESPONSE_R2, 1, 1), 0x0)) /* Request CID */
		return false;

	struct sd_cid cid;
	debug_product_name(cfg->base, &cid);

	if (!send_cmd(cfg->base, make_cmd(3, SDIO_RESPONSE_R1367, 1, 1), 0x0))       /* Request RCA with CMD3 */
		return false;

	uint32_t rca = ioread32(cfg->base + SDIO_SDR_RESPONSE1_REG) >> 16;
	if (!send_cmd(cfg->base, make_cmd(7, SDIO_RESPONSE_R1367, 1, 1), (rca << 16))) /* Put SD card in transfer mode with CMD7 */
		return false;

	if (!send_cmd(cfg->base, make_cmd(55, SDIO_RESPONSE_R1367, 1, 1), (rca << 16))) /* CMD55 with real RCA */
		return false;

	if (!send_cmd(cfg->base, make_cmd(6, SDIO_RESPONSE_R1367, 1, 1), 0x2))     /* ACMD6 */
		return false;

	iowrite32(divh, cfg->base + SPISDIO_SDIO_CLK_DIVIDE);
	rumboot_printf("SD: Card ready!\n");

	return true;
}

void sd_deinit(struct bdata* cfg)
{
  /*Dummy*/
}

bool sd_read(struct bdata* cfg)
{
	rumboot_printf("SD: read block\n");

	if (!SD2buf512(cfg->base, 0, (const uint32_t) cfg->from))
		return false;

	if (!buf2axi(cfg->base, 0, (uint32_t) cfg->to))
		return false;

	return true;
}

bool sd_try_read(struct bdata* cfg, uint32_t attempts)
{
	while (attempts--)
		if (sd_read(cfg))
			return true;

	rumboot_printf("SD: Failed to read block: 0x%x dest: 0x%x\r\n", cfg->from, cfg->to);
	return false;
}

bool test_sdio(uint32_t base_addr)
{
	rumboot_printf("TEST SDIO\n");


	enable_gpio_for_SDIO();

  struct bdata cfg;
  cfg.base = base_addr;
  cfg.freq_in_mhz = SDIO_CLK_FREQ;
  //cfg.out_param[0];

	if (sd_init(&cfg) == false)
		return false;

	sd_try_read(&cfg, 5);

	return true;
}

bool sd_load_failed_should_i_try_again() {

  return false;
}
