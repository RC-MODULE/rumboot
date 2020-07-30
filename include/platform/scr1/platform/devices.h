#ifndef DEVICES_H
#define DEVICES_H

#define SCR1_TIMER_BASE 0x00490000
#define DUT_BASE        0x00498000

#define SPI_CONTROL_BASE    DUT_BASE
#define SPI_FIFO_BASE       (DUT_BASE + 0x00002000)

#define MDMA_BASE           DUT_BASE

#endif /* end of include guard: DEVICES_H */