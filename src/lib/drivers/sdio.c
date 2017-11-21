#include <stdint.h>

#include <platform/defs_c.h>

#include <sdio.h>

#define PL061_AFSEL 0x420

#define SDIO_CLK_DIV_400KHz   124
#define SDIO_CLK_DIV_10MHz    4
#define SDIO_CLK_DIV_50MHz    0


//#define SDIO_DEBUG
//--------------------------SDIO_CMD------------------------
#define CMD0_CTRL           0x00000000
#define CMD2_CTRL           0x00023411
#define CMD3_CTRL           0x00033811
#define CMD6_CTRL           0x00063811
#define CMD7_CTRL           0x00073C11
#define CMD8_CTRL           0x00083811
#define CMD16_CTRL          0x00103811
#define CMD17_CTRL          0x00117911
#define CMD41_CTRL          0x00293811
#define CMD55_CTRL          0x00373811
#define CMD59_CTRL          0x003B3811
//--------------------------SDIO_CMD_ARG--------------------
#define CMD0_ARG            0x00000000
#define CMD2_ARG            0x00000000
#define CMD3_ARG            0x00000000
#define CMD6_ARG            0x00000002
#define CMD7_ARG            0x12340000
#define CMD8_ARG            0x000001AA
#define CMD16_ARG           0x200
#define CMD41_ARG           0x80FF8000
#define CMD55_ARG           0x12340000
#define CMD55_ARG_0         0x00000000
#define CMD59_ARG           0x00000001

#define SDR_TRAN_FIFO_FINISH    0x00000001
#define SDR_TRAN_SDC_DAT_DONE   0x00000002
#define SDR_TRAN_SDC_CMD_DONE   0x00000004
#define SDR_TRAN_DAT_BOUNT      0x00000008
#define SDR_TRAN_SDC_ERR        0x00000010

#define DSSR_CHANNEL_TR_DONE    0x00080000
//--------------------------SDIO_INT------------------------

#define BLOCK_512_DATA_TRANS        0x00000101
#define BUFER_TRANS_START           0x04
#define DCDTR_1_VAL_512             0x200
#define DCCR_1_VAL                  0x00020F01

#define SDIO_TIMEOUT        2000000 //in microseconds
#define SDIO_ATTEMPTS_NUMBER 5

struct bootheader  {
  /*Dummy*/

};

static void enable_gpio_for_SDIO()
{
    uint8_t afsel;
    afsel = ioread32(LSIF1_GPIO7__ + PL061_AFSEL);
    afsel |= 0b11110;
    iowrite32(afsel, LSIF1_GPIO7__ + PL061_AFSEL);
}

enum Card_type {
  SDIO_CARD_UNKNOW,
  SDIO_CARD_OLD,
  SDIO_CARD_SDHC,
  SDIO_CARD_SDSC,

};

struct bsrc_sd_card {
  enum Card_type cardtype;
  /*Dummy*/
};



enum Card_error {
  SDIO_CARD_ERROR,
  SDIO_CARD_OK,
};

struct sd_cid {
   // byte 0
   /** Manufacturer ID */
   unsigned char mid;
   // byte 1-2
   /** OEM/Application ID */
   char oid[2];
   // byte 3-7
   /** Product name */
   char pnm[5];
   // byte 8
   /** Product revision least significant digit */
   unsigned char prv_m : 4;
   /** Product revision most significant digit */
   unsigned char prv_n : 4;
   // byte 9-12
   /** Product serial number */
   uint32_t psn;
   // byte 13
   /** Manufacturing date year low digit */
   unsigned char mdt_year_high : 4;
   /** not used */
   unsigned char reserved : 4;
   // byte 14
   /** Manufacturing date month */
   unsigned char mdt_month : 4;
   /** Manufacturing date year low digit */
   unsigned char mdt_year_low :4;
   // byte 15
   /** not used always 1 */
   unsigned char always1 : 1;
   /** CRC7 checksum */
   unsigned char crc : 7;
}__attribute__((packed));

#define TAG "SD"

static
bool wait_cmd_done_handle()
{
    int status;

    uint32_t start = ucurrent();

    do
    {
        status = ioread32(SPISDIO__ + SDIO_INT_STATUS);
        if (status & SPISDIO_SDIO_INT_STATUS_CAR_ERR)
        {
            iowrite32(SDR_TRAN_SDC_ERR, SDIO__ + SDIO_SDR_BUF_TRAN_RESP_REG);
#ifdef SDIO_DEBUG
            rumboot_printf("CMD: ERROR\n0x%x\r\n", status);
#endif
            return false;
        }
        else if (status & SPISDIO_SDIO_INT_STATUS_CMD_DONE)
        {
            iowrite32(SDR_TRAN_SDC_CMD_DONE, SDIO__ + SDIO_SDR_BUF_TRAN_RESP_REG);
#ifdef SDIO_DEBUG
            rumboot_printf("CMD: DONE\n");
#endif
            return true;
        }
    }
    while((ucurrent() - start) < SDIO_TIMEOUT);

    return false;
}
//-------------------------------------
static
bool wait_ch1_dma_done_handle()
{
    int status;

    uint32_t start = ucurrent();

    do
    {
        status = ioread32(SPISDIO__ + SDIO_INT_STATUS);
        if (status & SPISDIO_SDIO_INT_STATUS_CAR_ERR)
        {
            iowrite32(SDR_TRAN_SDC_ERR, SDIO__ + SDIO_SDR_BUF_TRAN_RESP_REG);
            return false;
        }
        else if (status & SPISDIO_SDIO_INT_STATUS_CH1_FINISH)
        {
            iowrite32(DSSR_CHANNEL_TR_DONE, SDIO__ + SDIO_DCCR_1);
            return true;
        }
    }
    while((ucurrent() - start) < SDIO_TIMEOUT);

    return false;
}
//-------------------------------------
static
bool wait_buf_tran_finish_handle()
{

    int status;

    uint32_t start = ucurrent();

    do
    {
        status = ioread32(SPISDIO__ + SDIO_INT_STATUS);
        if (status & SPISDIO_SDIO_INT_STATUS_CAR_ERR)
        {
            iowrite32(SDR_TRAN_SDC_ERR, SDIO__ + SDIO_SDR_BUF_TRAN_RESP_REG);
            return false;
        }
        else if (status & SPISDIO_SDIO_INT_STATUS_BUF_FINISH)
        {
            iowrite32(SDR_TRAN_FIFO_FINISH, SDIO__ + SDIO_SDR_BUF_TRAN_RESP_REG);
            return true;
        }
    }
    while((ucurrent() - start) < SDIO_TIMEOUT);

    return false;
}
//-------------------------------------
static
bool wait_tran_done_handle()
{

    int status;

    uint32_t start = ucurrent();

    do
    {
        status = ioread32(SPISDIO__ + SDIO_INT_STATUS);
        if (status & SPISDIO_SDIO_INT_STATUS_CAR_ERR)
        {
            iowrite32(SDR_TRAN_SDC_ERR, SDIO__ + SDIO_SDR_BUF_TRAN_RESP_REG);
            return false;
        }
        else if (status & SPISDIO_SDIO_INT_STATUS_TRAN_DONE)
        {
            iowrite32( SDR_TRAN_SDC_DAT_DONE, SDIO__ + SDIO_SDR_BUF_TRAN_RESP_REG);
            return true;
        }
    }
    while((ucurrent() - start) < SDIO_TIMEOUT);

    return false;
}


#define SDIO_RESPONSE_NONE  0
#define SDIO_RESPONSE_R2    1
#define SDIO_RESPONSE_R1367 2
#define SDIO_RESPONSE_R1b   3

#define SDIO_CTRL_NODATA(_cmd, _resp, _crc, _idx)          \
   ( _cmd << 16 | (_idx << 12) | (_crc << 13)  | (_resp << 10) | (0x11))

static bool sd_send_cmd(uint32_t cmd_ctrl, uint32_t arg)
{

#ifdef SDIO_DEBUG
    rumboot_printf("SEND CMD 0x%x 0x%x\r\n", cmd_ctrl, arg);
#endif
    iowrite32(arg, SDIO__ + SDIO_SDR_CMD_ARGUMENT_REG);
    iowrite32(cmd_ctrl, SDIO__ + SDIO_SDR_CTRL_REG);
    bool result = wait_cmd_done_handle();
#ifdef SDIO_DEBUG
    rumboot_printf("0x%x\r\n", ioread32(SDIO__ + SDIO_SDR_RESPONSE1_REG));
    rumboot_printf("0x%x\r\n", ioread32(SDIO__ + SDIO_SDR_RESPONSE2_REG));
    rumboot_printf("0x%x\r\n", ioread32(SDIO__ + SDIO_SDR_RESPONSE3_REG));
    rumboot_printf("0x%x\r\n", ioread32(SDIO__ + SDIO_SDR_RESPONSE4_REG));
#endif
    return result;
}

static bool isprintable(const char s)
{
    return ((s>='A' && s<='Z') ||
       (s>='a' && s<='z') ||
       (s>='!' && s<='?')) ? true : false;
}

int sd_core_init(int divl, int divh)
{
    int i;
    int result = SDIO_CARD_ERROR;
    int type = SDIO_CARD_UNKNOW;

    rumboot_printf("SD: init\r\n");

    iowrite32( divl, SPISDIO__ + SPISDIO_SDIO_CLK_DIVIDE);


    iowrite32( 0x1, SPISDIO__ + SPISDIO_ENABLE);//sdio-on, spi-off

    /* Hard reset damned hardware */
    iowrite32((1<<7), SDIO__ + SDIO_SDR_CTRL_REG);

    while (ioread32(SDIO__ + SDIO_SDR_CTRL_REG) & (1<<7));

    iowrite32(0x0, GSPI__ + PL022_IMSC);//disable interrupts in SSP

    iowrite32(0x7F, SPISDIO__ + SPISDIO_SDIO_INT_MASKS);//enable  interrupts in SDIO

    iowrite32(0x00, SPISDIO__ + SPISDIO_SPI_IRQMASKS);//disable interrupts from ssp

    iowrite32(0x16F, SDIO__ + SDIO_SDR_ERROR_ENABLE_REG);//enable interrupt and error flag

/* Try several times, card may be slow to respond on powerup */
    for (i=0; i<SDIO_ATTEMPTS_NUMBER; ++i)
    {


        if (sd_send_cmd(
                SDIO_CTRL_NODATA(0, SDIO_RESPONSE_NONE, 0, 0),
                0))
        {
            break;
        }
        mdelay(250);

    }

    if (sd_send_cmd(SDIO_CTRL_NODATA(8, SDIO_RESPONSE_R1367, 1, 1), 0x000001AA))
    {
        /* Check pattern incorrect, unusable card */
        if (ioread32(SDIO__ + SDIO_SDR_RESPONSE1_REG) != 0x000001AA)
        {
            type = SDIO_CARD_OLD; /* Invalid check pattern, assume old card */
        }
    }
    else
    {
        type = SDIO_CARD_OLD; /* No response, assume it's an old non-2.0 card for now */
    }

    for (i=0; i<SDIO_ATTEMPTS_NUMBER; ++i)
    {
        uint32_t resp;
        uint32_t arg;
        if (i)
            mdelay(250);
        /* CMD55 with RCA == 0 */
        if (!sd_send_cmd(SDIO_CTRL_NODATA(55, SDIO_RESPONSE_R1367, 1, 1), 0x0))
            continue;

        arg = ((uint32_t) 0x80100000);

        /* Do not set HC bit for old cards, it may confuse them */

        if (type != SDIO_CARD_OLD)
            arg |= ((uint32_t) 0x40000000);

        /* ACMD41, Tell the card we support SDHC */
        if (!sd_send_cmd(SDIO_CTRL_NODATA(41, SDIO_RESPONSE_R1367, 1, 1),
                 arg))
            continue;

        resp = ioread32(SDIO__ + SDIO_SDR_RESPONSE1_REG);

        if ((resp >> 31) != 1)
            continue; /* Card not ready, yet */

        if (type != SDIO_CARD_OLD)
        {
            if ((resp & 0x40000000) >> 30)
                type = SDIO_CARD_SDHC;
            else
                type = SDIO_CARD_SDSC;
        }

        break;
    }

    if (i == SDIO_ATTEMPTS_NUMBER)
        result = SDIO_CARD_ERROR;

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
        return SDIO_CARD_ERROR;
        break;
    }

    rumboot_printf(" ");

    /* Request CID */
    if (!sd_send_cmd(SDIO_CTRL_NODATA(2, SDIO_RESPONSE_R2, 1, 1), 0x0))
        return SDIO_CARD_ERROR;

    struct sd_cid cid;
    uint32_t *cidptr = (uint32_t *) &cid;
    cidptr[3] = ioread32(SDIO__ + SDIO_SDR_RESPONSE1_REG);
    cidptr[2] = ioread32(SDIO__ + SDIO_SDR_RESPONSE2_REG);
    cidptr[1] = ioread32(SDIO__ + SDIO_SDR_RESPONSE3_REG);
    cidptr[0] = ioread32(SDIO__ + SDIO_SDR_RESPONSE4_REG);

    for (i=0; i<2; i++)
        if (isprintable(cid.oid[i]))
            rumboot_printf("%s", cid.oid[i]);

    rumboot_printf(" ");

    for (i=0; i<5; i++)
        if (isprintable(cid.pnm[i]))
            rumboot_printf("%s", cid.pnm[i]);

    rumboot_printf(" detected \r\n");

    /* Request RCA with CMD3 */
    if (!sd_send_cmd(SDIO_CTRL_NODATA(3, SDIO_RESPONSE_R1367, 1, 1), 0x0))
        return SDIO_CARD_ERROR;


    uint32_t rca = ioread32(SDIO__ + SDIO_SDR_RESPONSE1_REG) >> 16;

    /* Put SD card in transfer mode with CMD7 */
    if (!sd_send_cmd(SDIO_CTRL_NODATA(7, SDIO_RESPONSE_R1367, 1, 1), (rca <<16)))
        return SDIO_CARD_ERROR;

    /* In transfer mode, we can now change bus width and clock */

    /* CMD55 with real RCA */
    if (!sd_send_cmd(SDIO_CTRL_NODATA(55, SDIO_RESPONSE_R1367, 1, 1), (rca <<16)))
        return SDIO_CARD_ERROR;

    /* ACMD6 */
    if (!sd_send_cmd(SDIO_CTRL_NODATA(6, SDIO_RESPONSE_R1367, 1, 1), (0x2)))
        return SDIO_CARD_ERROR;


    iowrite32( divh, SPISDIO__ + SPISDIO_SDIO_CLK_DIVIDE);
    rumboot_printf("SD: Card ready!\r\n");

    return result;
}
//----------------------------
static
bool SD2buf512(int buf_num, int idx)
{

    iowrite32((buf_num << 2), SDIO__ + SDIO_SDR_ADDRESS_REG);
    iowrite32( BLOCK_512_DATA_TRANS, SDIO__ + SDIO_SDR_CARD_BLOCK_SET_REG);
    iowrite32(idx, SDIO__ + SDIO_SDR_CMD_ARGUMENT_REG);
    iowrite32( CMD17_CTRL, SDIO__ + SDIO_SDR_CTRL_REG);

    if (wait_cmd_done_handle()==false)
    {
        rumboot_printf("TIMEOUT:CMD DONE at function SD2buf512.\r\n");
        return false;
    }

    if (wait_tran_done_handle()==false)
    {
        rumboot_printf("TIMEOUT:TRANSFER DONE at function SD2buf512.\r\n");
        return false;
    }
    return true;
}
//----------------------------
//dma_addr is a physical address
//Hi [35:32] must be set through LSIF1 reg
static
bool buf2axi(int buf_num, int dma_addr)
{

    iowrite32(BUFER_TRANS_START | buf_num, SDIO__ + SDIO_BUF_TRAN_CTRL_REG);
    iowrite32(DCDTR_1_VAL_512, SDIO__ + SDIO_DCDTR_1);
    iowrite32(dma_addr, SDIO__ + SDIO_DCDSAR_1);
    iowrite32(DCCR_1_VAL, SDIO__ + SDIO_DCCR_1);

    if (!wait_ch1_dma_done_handle())
    {
        rumboot_printf("TIMEOUT:DMA DONE at function buf2axi.\r\n");
        return false;
    }

    if (!wait_buf_tran_finish_handle())
    {
        rumboot_printf("TIMEOUT:BUF TRAN FINISH at function buf2axi.\r\n");
        return false;
    }
    return true;
}
//------------------------------------------

//dest_addr is a physical address
//Hi [35:32] must be set through LSIF1 reg

bool sd_read_block(uint32_t* srcaddr, uint32_t* destaddr)
{
  rumboot_printf("SD: read block\n");

    if (!SD2buf512(0, srcaddr))
       return false;

    if (!buf2axi(0, destaddr))
       return false;

     return true;
}

static bool sd_try_read_block(uint32_t block, uint32_t dest_addr, uint32_t attempts)
{

    while (attempts--)
    {
        if (!sd_read_block(block, dest_addr))
        {
            rumboot_printf("SD: Failed to read block: 0x%x dest: 0x%x\r\n", block, dest_addr);
        }
        else
        {
            return true;
        }
    }

    return false;
}


bool sd_core_read_image(void* bdata, volatile struct bootheader* dst, int dstlen)
{
    struct bsrc_sd_card* sd_card = (struct bsrc_sd_card*)bdata;

    uint32_t image_src = 8192;
    uint32_t increment = 512;
    uint32_t image_address = (uint32_t)dst;
    int image_length;
    uint32_t image_blocks_count;

    DBG_ASSERT(dstlen >= increment, "Unexpected dstlen value");

    rumboot_printf("SD: Loading image @ 0x%x\r\n", image_src);

    /* 512 byte blocks addressing for SDHC cards */
    if (sd_card->cardtype == SDIO_CARD_SDHC)
    {
        image_src /= 512;
        increment = 1;
    }

    if (!sd_try_read_block(image_src, image_address, SDIO_ATTEMPTS_NUMBER))
        return false;
    image_src += increment;
    image_address += 512;

    //First 512 bytes have been loaded
    //Let's check magic is valid

    image_length = bootimage_check_header_log(dst, TAG, dstlen);

    if (image_length < 0)
        return false;

    image_length += sizeof(struct bootheader);

    if (image_length > dstlen)
    {
        rumboot_printf("SD: Image too big: %d > %d\n", image_length, dstlen);
        return false;
    }

    rumboot_printf("SD: Reading %d bytes\n", image_length);

    image_blocks_count = image_length/512 +
                    ((image_length % 512) ? 1 : 0);
    //First block already loaded, so decrement
    --image_blocks_count;

    while (image_blocks_count)
    {
        if (!sd_try_read_block(image_src, image_address, SDIO_ATTEMPTS_NUMBER))
            return false;

        image_src += increment;
        image_address += 512;
        --image_blocks_count;
    }

   return bootimage_check_data_log(dst, TAG);;
}



bool test_sdio() {

  rumboot_printf("TEST SDIO\n");

  int divl, divh;
  uint32_t dest_addr;
  uint32_t block;

  enable_gpio_for_SDIO();

  if(sd_core_init(SDIO_CLK_DIV_400KHz, SDIO_CLK_DIV_10MHz) == SDIO_CARD_ERROR)
    return false;

  sd_read_block(block, dest_addr);

  return true;
}
