#ifndef REGS_EMI_H_
#define REGS_EMI_H_

#include <arch/common_macros.h>

BEGIN_ENUM( EMI_REG )
DECLARE_ENUM_VAL( EMI_SS0,                      0x00 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SD0,                      0x04 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SS1,                      0x08 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SD1,                      0x0C )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SS2,                      0x10 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SD2,                      0x14 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SS3,                      0x18 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SD3,                      0x1C )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SS4,                      0x20 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SD4,                      0x24 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SS5,                      0x28 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_SD5,                      0x2C )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_RFC,                      0x30 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_HSTSR,                    0x34 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_ECNT20,                   0x38 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_ECNT53,                   0x3C )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_H1ADR,                    0x40 )    //WR N/A
DECLARE_ENUM_VAL( EMI_H2ADR,                    0x44 )    //WR N/A
DECLARE_ENUM_VAL( EMI_RREQADR,                  0x48 )    //WR N/A
DECLARE_ENUM_VAL( EMI_WREQADR,                  0x4C )    //WR N/A
DECLARE_ENUM_VAL( EMI_WDADR,                    0x50 )    //WR N/A
DECLARE_ENUM_VAL( EMI_BUSEN,                    0x54 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_WECR,                     0x58 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_FLCNTRL,                  0x5C )    //WR 0x0000001C
DECLARE_ENUM_VAL( EMI_IMR,                      0x60 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_IMR_SET,                  0x64 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_IMR_RST,                  0x68 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_IRR,                      0x70 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_IRR_RST,                  0x74 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_ECCWRR,                   0x78 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_ECCRDR,                   0x7C )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_H1CMR,                    0xC0 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_H2CMR,                    0xC4 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_RREQCMR,                  0xC8 )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_WREQCMR,                  0xCC )    //WR 0x00000000
DECLARE_ENUM_VAL( EMI_WDCMR,                    0xD0 )    //WR 0x00000000
END_ENUM( EMI_REG )

#endif /* REGS_EMI_H_ */
