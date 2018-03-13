#ifndef MDIO__
#define MDIO__

#define MDIO_ID            0x00
#define MDIO_VER           0x04
#define MDIO_STATUS        0x08
#define MDIO_IRQ_MASK      0x0C
#define MDIO_PHY_IRQ_STATE 0x10
#define MDIO_CONTROL       0x14
#define MDIO_ETH_RST_N     0x18
#define MDIO_FREQ_DIVIDER  0x1C
#define MDIO_EN            0x20

#include <stdbool.h>

bool mdio_test();

#endif
