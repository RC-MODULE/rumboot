#ifndef OI10_DEVICES_H
#define OI10_DEVICES_H

#define BOOTROM_BASE            0xFFFF0000
#define BOOTROM_SIZE            0x00010000

#define IM0_BASE                0xС0010000
#define IM0_SIZE                0x00020000

#define IM1_BASE                0xD0000000
#define IM1_SIZE                0x00020000

#define EM0_BASE                0x80000000
#define EM0_SIZE                0x20000000

#define EM1_BASE                0xA0000000
#define EM1_SIZE                0x20000000

#define EM2_BASE                0x00000000
#define EM2_SIZE                0x80000000

//WD
#define WD_BASE                 0x38002000

#define PLB6PLB4_0_BASE         0x80000300
//PLB6 BUS CTRL
#define PLB6_BC_BASE            0x80000200

#endif /* end of include guard: OI10_DEVICES_H */
