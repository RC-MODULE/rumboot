
//SPI-SDIO control regs
#define SDIO_ENABLE                           0x300
#define SDIO_CLK_DIVIDE                       0x304
#define SDIO_INT_STATUS                       0x308
#define SDIO_INT_MASKS                        0x30c
#define SDIO_CLK_POLARITY                     0x310

//BASE 
#define eSRAM1_BASE		              0x00060000

//Test addresses
#define DMA_SDIO_SRC_IM0                      eSRAM1_BASE + 0x10000
#define DMA_SDIO_DST_SRAM                     0x30030000
#define DMA_SDIO_DST_SRAM1                    0x37f40000
#define DMA_SDIO_DST_DDR                      0x40000000
#define DMA_SDIO_DST_DDR1                     0x7f010000
#define DMA_SDIO_DST_DDR_HIGH                 0x00000000
#define DMA_SDIO_DST_DDR1_HIGH                0x3f000000
#define DMA_SDIO_DST_IM0                      eSRAM1_BASE + 0x30000

#define SPI_DMAWSTART     GSPI0_BASE+0x90
#define SPI_DMAWEND       GSPI0_BASE+0x94
#define SPI_DMAWCNTRL       GSPI0_BASE+0x98
#define SPI_DMAWCUR       GSPI0_BASE+0x9c
#define SPI_AXIW_BRESP      GSPI0_BASE+0xa0
#define SPI_DMARSTART       GSPI0_BASE+0xa4
#define SPI_DMAREND       GSPI0_BASE+0xa8
#define SPI_DMARCNTRL       GSPI0_BASE+0xac
#define SPI_DMARCUR       GSPI0_BASE+0xb0

#define Size                      33