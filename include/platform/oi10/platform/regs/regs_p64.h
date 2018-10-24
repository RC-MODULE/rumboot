/**
 *
 */

#ifndef P64_REGS_H
#define P64_REGS_H

#include <platform/regs/fields/p64.h>
#include <platform/common_macros/common_macros.h>

BEGIN_ENUM( P64_REG )
DECLARE_ENUM_VAL(     P64_P64CR                           , 0x00)         /* Configuration Register                           0x00000203*/
DECLARE_ENUM_VAL(     P64_ESR                             , 0x01)         /* Error Status Register                            0x00000000*/
DECLARE_ENUM_VAL(     P64_P64REV                          , 0x02)         /* Revision ID Register                             0x00000101*/
DECLARE_ENUM_VAL(     P64_EARH                            , 0x03)         /* Error Address Register High                      0x00000000*/
DECLARE_ENUM_VAL(     P64_EARL                            , 0x04)         /* Error Address Register Low                       0x00000000*/
DECLARE_ENUM_VAL(     P64_ADDRL0                          , 0x05)         /* Lower PLB6 Slave Address Register                I_ADDRL0(0:31) straps*/
DECLARE_ENUM_VAL(     P64_ADDRH0                          , 0x06)         /* Upper PLB6 Slave Address Register                I_ADDRH0(0:31) straps*/
DECLARE_ENUM_VAL(     P64_ADDRL1                          , 0x07)         /* Lower Split Range PLB6 Slave Address Register    0x00000000*/
DECLARE_ENUM_VAL(     P64_ADDRH1                          , 0x08)         /* Upper Split Range PLB6 Slave Address Register    0x00000000*/
DECLARE_ENUM_VAL(     P64_TESR                            , 0x09)         /* Test Only: Error Status Register                 0x00000000*/
END_ENUM( P64_REG )

#endif /* P64_REGS_H*/
