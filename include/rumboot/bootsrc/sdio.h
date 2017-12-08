#ifndef SDIO__
#define SDIO__

/**
 * \defgroup bootsrc_sdio SDIO
 * \ingroup bootsrc
 *
 *  SDIO function libriary.
 *  Contains functions such as:
 *  - Init SDIO
 *  - Deinit SDIO
 *  - Read data from SDIO
 *
 * \code{.c}
 * #include <rumboot/bootsrc/sdio.h>
 * \endcode
 *
 * TODO: Insert usage example here
 *
 * \addtogroup bootsrc_sdio
 *
 * @{
 */

#include <rumboot/bootsource.h>

/**
 * Card_type Type card enumeration
 */
enum Card_type {

        SDIO_CARD_UNKNOW = 0,
        SDIO_CARD_OLD,
        SDIO_CARD_SDHC,
        SDIO_CARD_SDSC,

};

/**
 * Type_event Type event enumeration
 */
enum Type_event {

        CH1_DMA_DONE_HANDLE = 0,
        CMD_DONE_HANDLE,
        TRAN_DONE_HANDLE,
        TRAN_FINISH_HANDLE,
};

/**
 *  sd_cid            : Structure contains configuration parameters
 *  mid               - Manufacturer ID
 *  oid[2]            -OEM/Application ID, byte 1-2
 *  pnm[5]            - Product name, byte 3-7
 *  prv_m : 4         - Product revision least significant digit, byte 8
 *  prv_n  : 4        - Product revision most significant digit
 *  psn               - Product serial number
 *  mdt_year_high : 4 - Manufacturing date year low digit
 *  reserved : 4      - not used
 *  mdt_month : 4     - Manufacturing date month, byte 14
 *  mdt_year_low : 4  - Manufacturing date year low digit,
 *  always1 : 1       - not used always 1, byte 15
 *  crc : 7           - CRC7 checksum
 */
struct sd_cid {
        unsigned char mid;
        char oid[2];
        char pnm[5];
        unsigned char prv_m : 4;
        unsigned char prv_n : 4;
        uint32_t psn;
        unsigned char mdt_year_high : 4;
        unsigned char reserved : 4;
        unsigned char mdt_month : 4;
        unsigned char mdt_year_low : 4;
        unsigned char always1 : 1;
        unsigned char crc : 7;
} __attribute__((packed));

/**
  *
  * Event Event type structure
  * response  type of response
  * flag      type of flag
 */
struct Event {

        enum Type_event type;
        uint32_t response;
        uint32_t flag;
};

/**
 * SD card initialization
 * @param  base address of SDIO
 * @param  freq_in_mhz sys clock frequency in KHz
 * @return pointer to configuration structure of sd card
 */
bool sd_init(const struct rumboot_bootsource* src, void* pdata);
/**
 * Read SD card
 * @param  pdata     pointer to private data
 * @return           true if read ok
 */
bool sd_read(void* pdata);
/**
 * [sd_try_read description]
 * @param  pdata      base address of SDIO
 * @param  attempts  number of read attempts
 * @return           true if read ok
 */
bool sd_try_read(void* pdata, const uint32_t attempts);

/**
 * [sd_deinit description]
 * @param  base_addr [description]
 * @return           [description]
 */
void sd_deinit(void* pdata);

/**
 * Find another source for load
 * @return [description]
 */
bool sd_load_failed_should_i_try_again();


/**
 * @}
 * \defgroup registers
 * \ingroup bootsrc_sdio
 *
 * \addtogroup registers
 * @{
 */
#define SDIO_INT_STATUS   0x308
#define SPISDIO_ENABLE          0x300
#define SPISDIO_SDIO_CLK_DIVIDE 0x304
#define SPISDIO_SDIO_INT_MASKS  0x30C
#define SPISDIO_SPI_IRQMASKS    0x0D8

#define SDIO_SDR_CARD_BLOCK_SET_REG  0x00
#define SDIO_SDR_CTRL_REG            0x04
#define SDIO_SDR_CMD_ARGUMENT_REG    0x08
#define SDIO_SDR_ADDRESS_REG         0x0C
#define SPISDIO_SDIO_INT_STATUS_CH0_FINISH  0x01
#define SDIO_SDR_STATUS_REG          0x10
#define SDIO_SDR_ERROR_ENABLE_REG    0x14
#define SDIO_SDR_RESPONSE1_REG       0x18
#define SDIO_SDR_RESPONSE2_REG       0x1C
#define SDIO_SDR_RESPONSE3_REG       0x20
#define SDIO_SDR_RESPONSE4_REG       0x24
//from AXI
#define SDIO_DCCR_0                  0x28
#define SDIO_DCSSAR_0                0x2C
#define SDIO_DCDSAR_0                0x30
#define SDIO_DCDTR_0                 0x34
//to AXI
#define SDIO_DCCR_1                  0x38
#define SDIO_DCSSAR_1                0x3C
#define SDIO_DCDSAR_1                0x40
#define SDIO_DCDTR_1                 0x44

#define SDIO_SDR_BUF_TRAN_RESP_REG   0x48
#define SDIO_BUF_TRAN_CTRL_REG       0x50
#define SDIO_MULTI_BLK_CTRL_REG      0x54
#define SDIO_DMA_CHANEL_FINISH_REG   0x60

#define SDR_RESPONSE1_REG            0x18
#define SDR_RESPONSE2_REG            0x1C
/**
 * @}
 */

/**
 * @}
 * \defgroup commands
 * \ingroup bootsrc_sdio
 *
 * \addtogroup commands
 * @{
 */
 #define CMD0_CTRL             0x00000000
 #define CMD2_CTRL             0x00023411
 #define CMD3_CTRL             0x00033811
 #define CMD6_CTRL             0x00063811
 #define CMD7_CTRL             0x00073C11
 #define CMD8_CTRL             0x00083811
 #define CMD16_CTRL            0x00103811
 #define CMD17_CTRL            0x00117911
 #define CMD41_CTRL            0x00293811
 #define CMD55_CTRL            0x00373811
 #define CMD59_CTRL            0x003B3811
/**
 * @}
 */


/**
 * @}
 * \defgroup arguments
 * \ingroup bootsrc_sdio
 *
 * \addtogroup arguments
 * @{
 */
   #define CMD0_ARG              0x00000000
   #define CMD2_ARG              0x00000000
   #define CMD3_ARG              0x00000000
   #define CMD6_ARG              0x00000002
   #define CMD7_ARG              0x12340000
   #define CMD8_ARG              0x000001AA
   #define CMD16_ARG             0x200
   #define CMD41_ARG             0x80FF8000
   #define CMD55_ARG             0x12340000
   #define CMD55_ARG_0           0x00000000
   #define CMD59_ARG             0x00000001
/**
 * @}
 */

/**
 * @}
 * \defgroup responses
 * \ingroup bootsrc_sdio
 *
 * \addtogroup responses
 * @{
 */
 #define SDIO_RESPONSE_NONE  0
 #define SDIO_RESPONSE_R2    1
 #define SDIO_RESPONSE_R1367 2
 #define SDIO_RESPONSE_R1b   3
/**
 * @}
 */

/**
 * @}
 * \defgroup default_values
 * \ingroup bootsrc_sdio
 *
 * \addtogroup default_values
 * @{
 */
  #define SDIO_SDR_CARD_BLOCK_SET_REG_DFLT      0x0
  #define SDIO_SDR_CTRL_REG_DFLT                0x0
  #define SDIO_SDR_CMD_ARGUMENT_REG_DFLT        0x0
  #define SDIO_SDR_ADDRESS_REG_DFLT             0x0
  #define SDIO_SDR_STATUS_REG_DFLT              0x0
  #define SDIO_SDR_ERROR_ENABLE_REG_DFLT        0x0
  #define SDIO_SDR_RESPONSE1_REG_DFLT           0x0
  #define SDIO_SDR_RESPONSE2_REG_DFLT           0x0
  #define SDIO_SDR_RESPONSE3_REG_DFLT           0x0
  #define SDIO_SDR_RESPONSE4_REG_DFLT           0x0
/**
 * @}
 */

/**
 * @}
 * \defgroup flags
 * \ingroup bootsrc_sdio
 *
 * \addtogroup flags
 * @{
 */
 #define SPISDIO_SDIO_INT_STATUS_CAR_ERR     0x40
 #define SPISDIO_SDIO_INT_STATUS_CMD_DONE    0x20
 #define SPISDIO_SDIO_INT_STATUS_TRAN_DONE   0x10
 #define SPISDIO_SDIO_INT_STATUS_DATA_BOUND  0x08
 #define SPISDIO_SDIO_INT_STATUS_BUF_FINISH  0x04
 #define SPISDIO_SDIO_INT_STATUS_CH1_FINISH  0x02

 #define SDR_TRAN_FIFO_FINISH    0x00000001
 #define SDR_TRAN_SDC_DAT_DONE   0x00000002
 #define SDR_TRAN_SDC_CMD_DONE   0x00000004
 #define SDR_TRAN_DAT_BOUNT      0x00000008
 #define SDR_TRAN_SDC_ERR        0x00000010

 #define DSSR_CHANNEL_TR_DONE    0x00080000
/**
 * @}
 */

#endif
