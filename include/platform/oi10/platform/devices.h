#ifndef OI10_DEVICES_H
#define OI10_DEVICES_H

#define BOOTROM_BASE            0xFFFC0000
#define BOOTROM_SIZE            0x00040000

#define BOOTROM_MIRROR_BASE     0x00000000
#define BOOTROM_MIRROR_SIZE     0x00040000

#define IM0_BASE                0x00040000
#define IM0_SIZE                0x00040000

//WD
#define WD_BASE                 0x38002000

#define PLB6PLB4_0_BASE         0x80000300
//PLB6 BUS CTRL
#define PLB6_BC_BASE            0x80000200

#endif /* end of include guard: OI10_DEVICES_H */
