#ifndef GPIO__
#define GPIO__

#define GPIO_ID            0
#define GPIO_VERSION       0x004

#define GPIO_PAD_DIR       0x008
#define GPIO_WR_DATA       0x00C
#define GPIO_RD_DATA       0x010
#define GPIO_STATUS        0x014
#define GPIO_IRQ_MASK      0x018
#define GPIO_WR_DATA_SET1  0x01C
#define GPIO_WR_DATA_SET0  0x020
#define GPIO_SWITCH_SOURCE 0x024

// version and ID
#define GPIO_ID_VALUE      0x4F495047
#define GPIO_VERSION_VALUE 1

#endif
