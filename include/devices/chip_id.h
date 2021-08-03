/*
 *  List of chips
 */

enum chip_ip {
    CHIP_OI10V1,
    CHIP_OI10V2,
    CHIP_OI10V3,
    CHIP_O32T,
    CHIP_UNKNOWN
};

enum chip_ip get_chip_id();