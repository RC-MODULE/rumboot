#ifndef MUART__
#define MUART__

#define MUART_ID 0x000

#define MUART_VERSION 0x004

#define MUART_DTRANS 0x008

#define MUART_DREC 0x00c

#define MUART_FIFO_STATE 0x010

#define MUART_BDIV 0x014

#define MUART_CTRL 0x018

#define MUART_FIFOWM 0x01C

#define MUART_MASK 0x020

#define MUART_STATUS 0x024

#define MUART_TIMEOUT 0x028

#define MUART_MEN_i 0
#define MUART_LBE_i 1
#define MUART_MDS_i 2
#define MUART_PEN_i 7
#define MUART_EPS 8
#define MUART_STP2_i 10
#define MUART_WLEN_i 12

#define MUART_BAUD_DIV_i 0

#define MUART_RXRIS_i 0
#define MUART_TXRIS_i 1
#define MUART_RTSEn_i 4
#define MUART_CTSEn_i 5
#define MUART_RERIS_i 6
#define MUART_TERIS_i 7

#endif
