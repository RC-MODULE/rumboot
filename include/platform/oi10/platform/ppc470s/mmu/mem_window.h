#ifndef __PPC470_MMU_MEM_WINDOW_H__
#define __PPC470_MMU_MEM_WINDOW_H__


#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


BEGIN_ENUM( MEM_WINDOW )
DECLARE_ENUM_VAL( MEM_WINDOW_SHARED,    0x0000 )
DECLARE_ENUM_VAL( MEM_WINDOW_0,         ( 0x8000 | 0 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_1,         ( 0x8000 | 1 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_2,         ( 0x8000 | 2 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_3,         ( 0x8000 | 3 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_4,         ( 0x8000 | 4 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_5,         ( 0x8000 | 5 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_6,         ( 0x8000 | 6 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_7,         ( 0x8000 | 7 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_8,         ( 0x8000 | 8 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_9,         ( 0x8000 | 9 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_10,        ( 0x8000 | 10 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_11,        ( 0x8000 | 11 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_12,        ( 0x8000 | 12 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_13,        ( 0x8000 | 13 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_14,        ( 0x8000 | 14 ) )
DECLARE_ENUM_VAL( MEM_WINDOW_15,        ( 0x8000 | 15 ) )
END_ENUM( MEM_WINDOW )


#endif // __PPC470_MMU_MEM_WINDOW_H__
