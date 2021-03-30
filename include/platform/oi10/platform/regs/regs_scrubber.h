#define L2C_SCRUB_BASE                  0x800E0000
#define IM0_SCRUB_BASE                  0x800E1000
// Common
#define SCRUB_ID                        0x00000000       // Read only
#define SCRUB_VER                       0x00000004       // Read only
#define SCRUB_SW_RESET                  0x00000008
#define SCRUB_ECC_CNT_RST               0x0000000C
#define SCRUB_SETTINGS                  0x00000010
#define SCRUB_PERIOD                    0x00000014       
// Individual   
#define SCRUB_X_NUMER                   0x00000000
#define SCRUB_X_TIME                    0x00000004
#define SCRUB_X_BUS_ACCESSES            0x00000008
#define SCRUB_X_BUS_ACCESSES_TIME       0x0000000C
#define SCRUB_X_BUS_ACCESSES_RATE       0x00000010
    
#define SCRUB_X_ECC_ERRORS_0            0x00000020
#define SCRUB_X_ECC_ERRORS_1            0x00000024
#define SCRUB_X_ECC_ERRORS_2            0x00000028
#define SCRUB_X_ECC_ERRORS_3            0x0000002C
#define SCRUB_X_ECC_ERRORS_4            0x00000030
#define SCRUB_X_ECC_ERRORS_5            0x00000034
#define SCRUB_X_ECC_ERRORS_6            0x00000038
#define SCRUB_X_ECC_ERRORS_7            0x0000003C
#define SCRUB_X_ECC_ERRORS_8            0x00000040
#define SCRUB_X_ECC_ERRORS_9            0x00000044
#define SCRUB_X_ECC_ERRORS_10           0x00000048
#define SCRUB_X_ECC_ERRORS_11           0x0000004C
#define SCRUB_X_ECC_ERRORS_12           0x00000050
#define SCRUB_X_ECC_ERRORS_13           0x00000054
#define SCRUB_X_ECC_ERRORS_14           0x00000058
#define SCRUB_X_ECC_ERRORS_15           0x0000005C

// Scrubber local addresses
#define SCRUB_L2C_0                     0x800
#define SCRUB_L2C_1                     0x900
#define SCRUB_L2C_2                     0xA00
#define SCRUB_L2C_3                     0xB00
#define SCRUB_L2C_4                     0xC00
#define SCRUB_L2C_5                     0xD00
#define SCRUB_L2C_6                     0xE00
#define SCRUB_L2C_7                     0xF00

#define SCRUB_IM0_0_1                   0x800
#define SCRUB_IM0_2_3                   0xC00


// Constants
#define SCRUB_IDENTIFYER                0x62726353
#define SCRUB_VERSION                   0x00000100

#define SCRUB_L2C_0_NUMBER              0x00
#define SCRUB_L2C_1_NUMBER              0x01
#define SCRUB_L2C_2_NUMBER              0x02
#define SCRUB_L2C_3_NUMBER              0x03
#define SCRUB_L2C_4_NUMBER              0x04
#define SCRUB_L2C_5_NUMBER              0x05
#define SCRUB_L2C_6_NUMBER              0x06
#define SCRUB_L2C_7_NUMBER              0x07

#define SCRUB_IM0_0_1_NUMBER            0x0001
#define SCRUB_IM0_2_3_NUMBER            0x0203
