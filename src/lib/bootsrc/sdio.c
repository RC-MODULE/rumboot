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
#include <rumboot/boot.h>

#define BUFER_TRANS_START           0x04
#define DCDTR_1_VAL_512             0x200
#define DCCR_1_VAL                  0x00020F01

#define SDIO_TIMEOUT_US      80000000
#define SDIO_ATTEMPTS_NUMBER 4
//#define DEBUG


#ifdef DEBUG
#define sd_dbg(src, msg, ...) rumboot_printf("SDIO DEBUG: " msg "\n", ## __VA_ARGS__);
#else
#define sd_dbg(src, msg, ...)
#endif

#ifdef __PPC__
#define format_response(x) (x)
#else
#define format_response(x) (((x) & 0xff) << 24 | ((x) & 0xff00) << 8 | ((x) & 0xff0000) >> 8 | (((x) >> 24) & 0xff))
#endif

enum sd_card_type {
        SDIO_CARD_UNKNOWN = 0,
        SDIO_CARD_OLD,
        SDIO_CARD_MMC, /* TODO: These need CMD1 to correctly initialize: http://patrickleyman.be/blog/sd-card-c-driver-init/ */
        SDIO_CARD_SDSC,
        SDIO_CARD_SDHC,
        SDIO_CARD_SDXC, /* TODO: Needs moar testing. TODO: Find out how to tell the difference between these two */
        SDIO_CARD_SDUC, /* TODO: Reserved. No cards on the market. Yet. */
        SDIO_CARD_MAX,
};

const char *const sd_card_type_names[] = {
        [SDIO_CARD_UNKNOWN] = "Unknown/Invalid",
        [SDIO_CARD_OLD] = "Ancient Relic (Where did u dig it up?)",
        [SDIO_CARD_MMC] = "MMC Card (Blast from the past?)",
        [SDIO_CARD_SDHC] = "SDHC Card",
        [SDIO_CARD_SDSC] = "SDSC Card",
        [SDIO_CARD_SDXC] = "SDXC Card",
        [SDIO_CARD_SDUC] = "SDUC Card",
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


struct sdio_request {
        /* Params */
        uint32_t	cmd;
        uint32_t	resp;
        uint32_t	crc;
        uint32_t	idx;
        uint32_t	arg;
        uint32_t	cmd55_arg;

        void *		buf;
        bool		is_acmd;
        bool		is_read;
        int		int_buf_num;
        int             data_lanes;
};

struct sdio_private_data {
        const struct rumboot_bootsource *	src;
        enum sd_card_type			cardtype;
        char					ident[16];
        uint32_t                                ocr;
        union {
                struct sd_cid	cid;
                uint32_t	regs[4];
                unsigned char   bytes[16];
        } cid;
};



static bool sdio_request_execute(uintptr_t base, struct sdio_request *rq);

static bool card_reset(uintptr_t base)
{
        struct sdio_request rq = {
                .cmd	= 0,
                .resp	= SDIO_RESPONSE_NONE,
                .crc	= 0,
                .idx	= 0,
                .data_lanes = 0,
        };

        /* Send cmd0 a few times */
        sdio_request_execute(base, &rq);
        return sdio_request_execute(base, &rq);
}

static bool card_start_acmd(uintptr_t base, uint32_t arg)
{
        struct sdio_request rq = {
                .cmd	= 55,         /* CMD 55 */
                .resp	= SDIO_RESPONSE_NONE,
                .crc	= 1,
                .idx	= 1,
                .arg	= arg,
                .data_lanes = 0,
        };

        /* Send cmd0 a few times */
        return sdio_request_execute(base, &rq);
}

static bool sdio_request_start(uint32_t base, struct sdio_request *rq)
{
        uint32_t lanes = (rq->data_lanes == 4) ? 1 : 0;
        uint32_t cmd_ctrl = (rq->cmd << 16) | (rq->idx << 12) | (rq->crc << 13) | (rq->resp << 10) | 1 << 4 | lanes;

        /* Do we have data transfer ? */
        if (rq->buf) {
                iowrite32((rq->int_buf_num << 2), base + SDIO_SDR_ADDRESS_REG);
                /* 1 block, 512 bytes */
                iowrite32(((1 << 8) | (1 << 0)), base + SDIO_SDR_CARD_BLOCK_SET_REG);
                cmd_ctrl |= 1 << 14; /* Enable data transfer for this cmd */
                cmd_ctrl |= rq->is_read ? (1 << 8) : 0;
        }

        iowrite32(rq->arg, base + SDIO_SDR_CMD_ARGUMENT_REG);
        iowrite32(cmd_ctrl, base + SDIO_SDR_CTRL_REG);
        return true;
}

static uint32_t sdio_get_events(uintptr_t base)
{
        uint32_t status;

        status = ioread32(base + SDIO_INT_STATUS);

        if (status & SPISDIO_SDIO_INT_STATUS_CAR_ERR) {
                sd_dbg(NULL, "sdio event: Card error");
                iowrite32(SDR_TRAN_SDC_ERR, base + SDIO_SDR_BUF_TRAN_RESP_REG);
        }
        ;

        if (status & SPISDIO_SDIO_INT_STATUS_CMD_DONE) {
                sd_dbg(NULL, "sdio event: command complete");
                iowrite32(SDR_TRAN_SDC_CMD_DONE, base + SDIO_SDR_BUF_TRAN_RESP_REG);
        }

        if (status & SPISDIO_SDIO_INT_STATUS_CH0_FINISH) {
                sd_dbg(NULL, "sdio event: ch0 dma done");
                iowrite32(DSSR_CHANNEL_TR_DONE, base + SDIO_DCCR_0);
        }

        if (status & SPISDIO_SDIO_INT_STATUS_CH1_FINISH) {
                sd_dbg(NULL, "sdio event: ch1 dma done");
                iowrite32(DSSR_CHANNEL_TR_DONE, base + SDIO_DCCR_1);
        }

        if (status & SPISDIO_SDIO_INT_STATUS_BUF_FINISH) {
                sd_dbg(NULL, "sdio event: buf finish");
                iowrite32((1 << 0), base + SDIO_SDR_BUF_TRAN_RESP_REG);
        }

        if (status & SPISDIO_SDIO_INT_STATUS_TRAN_DONE) {
                sd_dbg(NULL, "sdio event: transfer done");
                iowrite32(SDR_TRAN_SDC_DAT_DONE, base + SDIO_SDR_BUF_TRAN_RESP_REG);
        }

        return status;
}

static bool wait(uintptr_t base, uint32_t flag)
{
        bool ret = false;
        uint32_t status = 0;
        uint32_t start = rumboot_platform_get_uptime();

        do {
                uint32_t tmp = sdio_get_events(base);
                status |= tmp;
                if (status & SPISDIO_SDIO_INT_STATUS_CAR_ERR) {
                        ret = false;
                        break;
                }

                if ((status & flag) == flag) {
                        ret = true;
                        break;
                }
        } while ((rumboot_platform_get_uptime() - start) < SDIO_TIMEOUT_US);

        if (((rumboot_platform_get_uptime() - start) > SDIO_TIMEOUT_US)) {
                return false;
        }
        return ret;
}

static bool sdio_request_execute(uintptr_t base, struct sdio_request *rq)
{
        bool ret = false;

        sd_dbg(NULL, "SDIO START %sCMD: %d idx %x resp %x crc %x arg %x",
               rq->is_acmd ? "A" : "",
               rq->cmd, rq->idx, rq->resp, rq->crc, rq->arg);

        if (rq->is_acmd) {
                if (!card_start_acmd(base, rq->cmd55_arg)) {
                        return false;
                }
        }

        if (!sdio_request_start(base, rq)) {
                return false;
        }

        ret = wait(base, SPISDIO_SDIO_INT_STATUS_CMD_DONE);
        if (!rq->buf || !ret) {
                sd_dbg(NULL, "RESULT (COMMAND): %d", ret);
                return ret;
        }

        sd_dbg(NULL, "Command complete");

        ret = wait(base, SPISDIO_SDIO_INT_STATUS_TRAN_DONE);
        if (!ret) {
                sd_dbg(NULL, "RESULT (TRANSFER): %d", ret);
        }
        sd_dbg(NULL, "Transfer complete, starting");

        /* Now, copy data to buffer */
        const int size = DCDTR_1_VAL_512;
        iowrite32(BUFER_TRANS_START | rq->int_buf_num, base + SDIO_BUF_TRAN_CTRL_REG);
        iowrite32(size, base + SDIO_DCDTR_1);
        iowrite32((uintptr_t)rq->buf, base + SDIO_DCDSAR_1);
        iowrite32(DCCR_1_VAL, base + SDIO_DCCR_1);

        ret = wait(base, SPISDIO_SDIO_INT_STATUS_CH1_FINISH | SPISDIO_SDIO_INT_STATUS_BUF_FINISH);
        if (!ret) {
                sd_dbg(NULL, "RESULT (DMA2AXI): %d", ret);
                return false;
        }

        return ret;
}




static bool isprintable(const char s)
{
    if((s>='A' && s<='Z') ||
       (s>='a' && s<='z') ||
       (s>='!' && s<='?'))
    {
       return true;
    }

    return false;
}

static void parse_cid(const struct rumboot_bootsource *src, void *pdata)
{
        struct sdio_private_data *pd = pdata;
        struct sd_cid *cid = &pd->cid.cid;
        char *ptr = pd->ident;

        size_t i;

        for (i = 0; i < 2; i++)
                if (isprintable(cid->oid[i])) {
                        *ptr++ = cid->oid[i];
                }

        *ptr++ = ' ';
        for (i = 0; i < 5; i++)
                if (isprintable(cid->pnm[i])) {
                        *ptr++ = cid->pnm[i];
                }
        *ptr++ = 0;
}

static uint32_t calc_div(uint32_t freq_base_khz, uint32_t freq_sdio_khz)
{
        return (freq_base_khz * 1000) / 2 / (freq_sdio_khz * 1000) - 1;
}

static int sd_read_block(const struct rumboot_bootsource *src, void *pdata, void *ram, uint64_t offset)
{
        struct sdio_private_data *sd_pdata = pdata;

        if (offset % 512 != 0) {
                return -1;
        }

        uint64_t real_offset = offset;

        if (sd_pdata->cardtype == SDIO_CARD_SDHC) {
                real_offset /= 512;
        }

        struct sdio_request rq = {
                .cmd		= 17, /* CMD 17 */
                .resp		= SDIO_RESPONSE_R1367,
                .crc		= 1,
                .idx		= 1,
                .arg		= (uint32_t)real_offset,
                .buf		= ram,
                .is_read	= true,
                .cmd55_arg	= 0,
                .is_acmd	= 0,
                .int_buf_num	= 0,
                /* MMC cards don't work in 4-lane mode */
                .data_lanes     = (sd_pdata->cardtype == SDIO_CARD_MMC) ? 1 : 4,
        };

        if (!sdio_request_execute(src->base, &rq)) {
                return -1;
        }

        return 512;
}


static bool card_init_sd(const struct rumboot_bootsource *src, void *pdata)
{
        struct sdio_private_data *temp = pdata;
        bool initialized = false;
        struct sdio_request rq = {
                .is_acmd = 0,
                .is_read = 0,
                .data_lanes = 0,
        };

        /* First, try CMD41 to initialize the card */
        rq.cmd = 41;
        rq.resp = SDIO_RESPONSE_R1367;
        rq.is_acmd = 1;
        rq.crc = 1;
        rq.idx = 1;
        rq.cmd55_arg = 0;

        rq.arg = 0x80ff8000;
        if (temp->cardtype != SDIO_CARD_OLD) {                 /* Do not set HC bit for old cards, it may confuse them */
                rq.arg |= 0x40000000;
        }

        for (int i = 0; i < SDIO_ATTEMPTS_NUMBER; i++) {
                if (!sdio_request_execute(src->base, &rq)) {
                        continue;
                }
                temp->cid.regs[3] = format_response(ioread32(src->base + SDIO_SDR_RESPONSE1_REG));
                temp->cid.regs[2] = format_response(ioread32(src->base + SDIO_SDR_RESPONSE2_REG));
                temp->cid.regs[1] = format_response(ioread32(src->base + SDIO_SDR_RESPONSE3_REG));
                temp->cid.regs[0] = format_response(ioread32(src->base + SDIO_SDR_RESPONSE4_REG));
                sd_dbg(src, "SDIO_SDR_RESPONSE1_REG[1]=0x%x", temp->cid.regs[3]);
                sd_dbg(src, "SDIO_SDR_RESPONSE1_REG[2]=0x%x", temp->cid.regs[2]);
                sd_dbg(src, "SDIO_SDR_RESPONSE1_REG[3]=0x%x", temp->cid.regs[1]);
                sd_dbg(src, "SDIO_SDR_RESPONSE1_REG[4]=0x%x", temp->cid.regs[0]);

                temp->ocr = ioread32(src->base + SDIO_SDR_RESPONSE1_REG);
                if ((temp->ocr & (1 << 31))) {
                        initialized = true;
                        break;
                }
                mdelay(250);
        }

        if (!(temp->ocr & (1 << 31))) {
                dbg_boot(src, "Card didn't leave busy state: OCR %x", temp->ocr);
        }

        if (temp->cardtype != SDIO_CARD_OLD) {
                temp->cardtype = (temp->ocr & (1 << 30)) ? SDIO_CARD_SDHC : SDIO_CARD_SDSC;
        }

        return initialized;
}

static bool card_init_mmc(const struct rumboot_bootsource *src, void *pdata)
{
        struct sdio_private_data *temp = pdata;
        bool initialized = false;
        struct sdio_request rq = {
                .is_acmd = 0,
                .is_read = 0,
                .data_lanes = 0,
        };

        /* Try CMD1 for MMC cards */
        /* Perhaps it's an MMC card? Try CMD1 */
        rq.cmd = 1;
        rq.resp = SDIO_RESPONSE_R1367;
        rq.is_acmd = 0;
        rq.crc = 0;
        rq.idx = 1;
        rq.arg = 0x80ff8000;
        for (int i = 0; i < SDIO_ATTEMPTS_NUMBER * 2; i++) {
                if (!sdio_request_execute(src->base, &rq)) {
                        continue;
                }
                temp->ocr = ioread32(src->base + SDIO_SDR_RESPONSE1_REG);
                if ((temp->ocr & (1 << 31))) {
                        initialized = true;
                        temp->cardtype = SDIO_CARD_MMC;
                        break;
                }
                mdelay(250);
        }

        if (!(temp->ocr & (1 << 31))) {
                dbg_boot(src, "Card didn't leave busy state");
        }

        return initialized;
}



static bool sdio_init(const struct rumboot_bootsource *src, void *pdata)
{
        struct sdio_private_data *temp = pdata;

        bool initialized = 0;

        uint32_t divl = calc_div(src->freq_khz, 400);
        uint32_t divh = calc_div(src->freq_khz, 10000);

#ifdef CMAKE_BUILD_TYPE_DEBUG
        /* simulation speed up */
        divl = 0;
        divh = 0;
#endif

        iowrite32(divl, src->base + SPISDIO_SDIO_CLK_DIVIDE);
        iowrite32(0x1, src->base + SPISDIO_ENABLE);             //sdio-on, spi-off

        iowrite32((1 << 7), src->base + SDIO_SDR_CTRL_REG);     /* Hard reset damned hardware */
        while (ioread32(src->base + SDIO_SDR_CTRL_REG) & (1 << 7));

        iowrite32(0x7F, src->base + SPISDIO_SDIO_INT_MASKS);
        iowrite32(0x0, src->base + SPISDIO_SPI_IRQMASKS);

        iowrite32(0x16F, src->base + SDIO_SDR_ERROR_ENABLE_REG);

        struct sdio_request rq = {
                .cmd	= 0,
                .resp	= SDIO_RESPONSE_NONE,
                .crc	= 0,
                .idx	= 0,
                .data_lanes = 0,
        };

        if (!card_reset(src->base)) {
                return false;
        }

        rq.cmd = 8;
        rq.arg = 0x000001AA;
        rq.resp = SDIO_RESPONSE_R1367;
        rq.idx = 1;
        rq.crc = 1;

        if (!sdio_request_execute(src->base, &rq)) {
                temp->cardtype = SDIO_CARD_OLD; /* Invalid check pattern, assume old card */
        } else if (ioread32(src->base + SDIO_SDR_RESPONSE1_REG) != 0x000001AA) {
                temp->cardtype = SDIO_CARD_OLD; /* No response, assume it's an old non-2.0 card for now */
        } else {
                /* We'll sort out SDSC/SDHC later */
                temp->cardtype = SDIO_CARD_UNKNOWN;
        }


        initialized = card_init_sd(src, pdata);
        if (!initialized) {
                if (!card_reset(src->base)) {
                        return false;
                }
                initialized = card_init_mmc(src, pdata);
        }

        if (!initialized) {
                return false;
        }

        rq.cmd = 2;
        rq.is_acmd = 0;
        rq.resp = SDIO_RESPONSE_R2;
        rq.idx = 1;
        rq.crc = 1;
        rq.arg = 0;

        if (!sdio_request_execute(src->base, &rq)) {
                dbg_boot(src, "CID request failed");
                return false;
        }

        temp->cid.regs[3] = format_response(ioread32(src->base + SDIO_SDR_RESPONSE1_REG));
        temp->cid.regs[2] = format_response(ioread32(src->base + SDIO_SDR_RESPONSE2_REG));
        temp->cid.regs[1] = format_response(ioread32(src->base + SDIO_SDR_RESPONSE3_REG));
        temp->cid.regs[0] = format_response(ioread32(src->base + SDIO_SDR_RESPONSE4_REG));
        parse_cid(src, temp);

        sd_dbg(src, "SDIO_SDR_RESPONSE1_REG[1]=0x%x", temp->cid.regs[3]);
        sd_dbg(src, "SDIO_SDR_RESPONSE1_REG[2]=0x%x", temp->cid.regs[2]);
        sd_dbg(src, "SDIO_SDR_RESPONSE1_REG[3]=0x%x", temp->cid.regs[1]);
        sd_dbg(src, "SDIO_SDR_RESPONSE1_REG[4]=0x%x", temp->cid.regs[0]);
        sd_dbg(src, "Card CID %08x%08x%08x%08x", temp->cid.regs[0],temp->cid.regs[1],temp->cid.regs[2],temp->cid.regs[3]);


        if ((temp->cardtype >= SDIO_CARD_MAX) || (temp->cardtype < 0)) {
                temp->cardtype = SDIO_CARD_UNKNOWN;
        }

        dbg_boot(src, "Detected: %s [%s]", sd_card_type_names[temp->cardtype], temp->ident);

        rq.cmd = 3;
        rq.is_acmd = 0;
        rq.resp = SDIO_RESPONSE_R1367;
        rq.idx = 1;
        rq.crc = 1;
        rq.arg = 0;

        if (!sdio_request_execute(src->base, &rq)) {
                dbg_boot(src, "RCA request failed");
                return false;
        }

        uint32_t rca = ioread32(src->base + SDIO_SDR_RESPONSE1_REG);

        rca = rca >> 16;

        rq.cmd = 7;
        rq.is_acmd = 0;
        rq.resp = SDIO_RESPONSE_R1367;
        rq.idx = 1;
        rq.crc = 1;
        rq.arg = rca << 16;

        if (!sdio_request_execute(src->base, &rq)) {
                dbg_boot(src, "Failed to put SD card into transfer mode, rca %x", (rca << 16));
                return false;
        }

        /* Switch the card into 512 byte blocks mode */
        rq.cmd = 16;
        rq.is_acmd = 0;
        rq.resp = SDIO_RESPONSE_R1367;
        rq.idx = 1;
        rq.crc = 1;
        rq.arg = 512;


        if (!sdio_request_execute(src->base, &rq)) {
                dbg_boot(src, "CMD16 Failed ");
                return false;
        }

        if (temp->cardtype != SDIO_CARD_MMC) {
                rq.cmd = 6;
                rq.is_acmd = 1;
                rq.resp = SDIO_RESPONSE_R1367;
                rq.idx = 1;
                rq.crc = 1;
                rq.arg = 2;
                rq.cmd55_arg = rca << 16;

                if (!sdio_request_execute(src->base, &rq)) {
                        dbg_boot(src, "ACMD6 Failed ");
                        return false;
                }
        }

        iowrite32(divh, src->base + SPISDIO_SDIO_CLK_DIVIDE);
        return true;
}

static void sdio_deinit(const struct rumboot_bootsource *src, void *pdata)
{
        /* reset the card */
        struct sdio_request rq = {
                .cmd	= 0,
                .resp	= SDIO_RESPONSE_NONE,
                .crc	= 0,
                .idx	= 0,
                .data_lanes = 0,
        };

        sdio_request_execute(src->base, &rq);

        /* Disable SDIO */
        uint32_t reg = ioread32(src->base + SPISDIO_ENABLE);
        iowrite32(reg & ~(1 << 1), src->base + SPISDIO_ENABLE);
}

static size_t sdio_read(const struct rumboot_bootsource *src, void *pdata, void *to, size_t offset, size_t length)
{
        size_t total = 0;

        while (length) {
                if (512 != sd_read_block(src, pdata, to, offset)) {
                        return total;
                }

                if (length >= 512) {
                        length -= 512;
                } else {
                        length = 0;
                }

                to += 512;
                offset += 512;
                total += 512;
        }

        return total;
}

const struct rumboot_bootmodule g_bootmodule_sdio = {
        .init		= sdio_init,
        .deinit		= sdio_deinit,
        .read		= sdio_read,
        .align		= 512,
        .privdatalen	= sizeof(struct sdio_private_data)
};
