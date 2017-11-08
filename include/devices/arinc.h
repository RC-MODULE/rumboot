
#define CHANNEL_EN			0x0004
#define CHANNEL_DIS			0x0008
#define CHANNEL_RST			0x000C
#define PARITY_BIT			0x0010
#define PARITY_ODD			0x0014
#define TEST_EN				0x0018

#define WAIT_SIG_RX			0x001C
#define TMR_MASK_RX			0x0020
#define SW_SIG_RX			0x0024
#define WAIT_TMR_TX			0x0028
#define SW_SIG_TX			0x002C
#define LABEL_EN_RX			0x0030
#define AXI_CTRL			0x0034
#define FIFO_EN_RX			0x0038
#define LAST_WD_TMR			0x003C
#define INT_TX				0x0040
#define INT_RX				0x0044

#define AG_E_RX				0x1880
#define AG_O_RX				0x1900
#define SZ_E_RX				0x1980
#define SZ_O_RX				0x1a00


#define TRF_E_TX			0x1380
#define TLF_E_TX			0x1400
#define TRL_E_TX			0x1480
#define TRF_O_TX			0x1500
#define TLF_O_TX			0x1580
#define TRL_O_TX			0x1600

#define AG_E_TX				0x1080
#define AG_O_TX				0x1100
#define SZ_E_TX				0x1180
#define SZ_O_TX				0x1200

#define STAT_E_TX			0x1280
#define STAT_O_TX			0x1300

#define STAT_E_RX			0x1A80
#define STAT_O_RX			0x1B00
#define TRF_E_RX			0x1B80
#define TLF_E_RX			0x1C00
#define TRL_E_RX			0x1C80
#define TRF_O_RX			0x1D00
#define TLF_O_RX			0x1D80
#define TRL_O_RX			0x1E00
#define LABEL_RX			0x1E80
#define RNUM_RX				0x1F00

#define FREQ_TX				0x0440
#define FREQ_RX				0x0c40
#define SELF_RX				0x0C80


#define ARINC_OK			0
#define ARINC_FAILED		1

#define TEST_OK				0
#define TEST_ERROR			1

#define arlock				0x0
#define arprot				0x0
#define arcache				0x0
#define awlock				0x0
#define awprot				0x0
#define awcache				0x0

#define status_success_bit  (1 << 31)
#define ARINC_ATTEMPT			10000