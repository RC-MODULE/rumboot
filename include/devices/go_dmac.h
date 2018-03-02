#ifndef DEVICES_SDIO_SPI_REGS_H
#define DEVICES_SDIO_SPI_REGS_H

void go_dmac(unsigned int gspibase, unsigned int* send, unsigned int* rcv, unsigned int amount);

#define SPI_DMARCUR                           0xb0

#define SPI_AXIR_BUFTYPE                      0xb4
#define SPI_AXI_PARAMS_ADDR                   0xc4
#define SPI_AXIR_BUFENA_ADDR                  0xb8
#define SPI_STATUS                            0xd4


#define SPI_AXI_PARAMS                        0x1ef
#define SPI_AXIR_BUFENA                       0x1

#endif 