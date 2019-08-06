#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>

#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/devices.h>
#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>

enum {
    MMU_TLBE_ATTR_SPPar_test_i = MMU_TLBE_ATTR_IL1I_i+1
};

#define  TEST_TLB_ENTRY(    EPN,    EPNp,   V,  TS, DSIZ,       DSIZp,  TID,    TIDp,   ERPN,   ERPNp,   RPN,    RPNp,   L1I,L1D,U,      W,  I,  M,  G,  E,  UX, UW, UR, SX, SW, SR,     SPp,    WAY )\
    (MMU_MMUCR( 0b0,    0b0,0b0,    TS,     TID )),\
\
    (MMU_TLBE_TAG( EPN,     V,  TS,     DSIZ,   0b0,    EPNp,DSIZp,TIDp )),\
\
    (MMU_TLBE_DATA( ERPN,RPN,   ERPNp,RPNp )),\
\
(   MMU_TLBWE_RA( (MMU_TLBWE_WAY_0|WAY),  MMU_TLBWE_BE_UND )\
|   (SPp << MMU_TLBE_ATTR_SPPar_test_i)\
|   MMU_TLBE_ATTR( L1I,L1D,    W,I,M,G,E,  UX,UW,UR,   SX,SW,SR,   U,  0b0 )  )


static struct tlb_entry const templates[][16] = {
    {
        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000000,   0b0,    0x0000, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    0 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b0,0b000000,   0b0,    0x0005, 0b1,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b0,0b000000,   0b0,    0x000A, 0b1,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b0,0b000000,   0b0,    0x000F, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    0 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000000,   0b0,    0x0000, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    1 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b0,0b000000,   0b0,    0x0005, 0b1,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b0,0b000000,   0b0,    0x000A, 0b1,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b0,0b000000,   0b0,    0x000F, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    1 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000000,   0b0,    0x0000, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    2 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b0,0b000000,   0b0,    0x0005, 0b1,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b0,0b000000,   0b0,    0x000A, 0b1,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b0,0b000000,   0b0,    0x000F, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    2 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000000,   0b0,    0x0000, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    3 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b0,0b000000,   0b0,    0x0005, 0b1,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b0,0b000000,   0b0,    0x000A, 0b1,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b0,0b000000,   0b0,    0x000F, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    3 )}
    },
    {
        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000001,   0b1,    0x5555, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    0 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b0,0b000001,   0b1,    0x5540, 0b0,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b0,0b000001,   0b1,    0x557F, 0b0,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b0,0b000001,   0b1,    0x556A, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    0 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000001,   0b1,    0x5555, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    1 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b0,0b000001,   0b1,    0x5540, 0b0,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b0,0b000001,   0b1,    0x557F, 0b0,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b0,0b000001,   0b1,    0x556A, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    1 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000001,   0b1,    0x5555, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    2 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b0,0b000001,   0b1,    0x5540, 0b0,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b0,0b000001,   0b1,    0x557F, 0b0,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b0,0b000001,   0b1,    0x556A, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    2 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000001,   0b1,    0x5555, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    3 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b0,0b000001,   0b1,    0x5540, 0b0,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b0,0b000001,   0b1,    0x557F, 0b0,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b0,0b000001,   0b1,    0x556A, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    3 )}
    },
    {
        {TEST_TLB_ENTRY(    0x00001,0b0,    0b1,0b0,0b000011,   0b0,    0xAA00, 0b1,    0x000,  0b1,    0x00001,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    0 )},
        {TEST_TLB_ENTRY(    0x55551,0b0,    0b1,0b0,0b000011,   0b0,    0xAAFF, 0b1,    0x155,  0b0,    0x55551,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xAAAA1,0b0,    0b1,0b0,0b000011,   0b0,    0xAAAA, 0b1,    0x2AA,  0b0,    0xAAAA1,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xFFFF1,0b0,    0b1,0b0,0b000011,   0b0,    0xAA55, 0b1,    0x3FF,  0b1,    0xAAAA1,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    0 )},

        {TEST_TLB_ENTRY(    0x00001,0b0,    0b1,0b0,0b000011,   0b0,    0xAA00, 0b1,    0x000,  0b1,    0x00001,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    1 )},
        {TEST_TLB_ENTRY(    0x55551,0b0,    0b1,0b0,0b000011,   0b0,    0xAAFF, 0b1,    0x155,  0b0,    0x55551,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xAAAA1,0b0,    0b1,0b0,0b000011,   0b0,    0xAAAA, 0b1,    0x2AA,  0b0,    0xAAAA1,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xFFFF1,0b0,    0b1,0b0,0b000011,   0b0,    0xAA55, 0b1,    0x3FF,  0b1,    0xAAAA1,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    1 )},

        {TEST_TLB_ENTRY(    0x00001,0b0,    0b1,0b0,0b000011,   0b0,    0xAA00, 0b1,    0x000,  0b1,    0x00001,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    2 )},
        {TEST_TLB_ENTRY(    0x55551,0b0,    0b1,0b0,0b000011,   0b0,    0xAAFF, 0b1,    0x155,  0b0,    0x55551,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xAAAA1,0b0,    0b1,0b0,0b000011,   0b0,    0xAAAA, 0b1,    0x2AA,  0b0,    0xAAAA1,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xFFFF1,0b0,    0b1,0b0,0b000011,   0b0,    0xAA55, 0b1,    0x3FF,  0b1,    0xAAAA1,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    2 )},

        {TEST_TLB_ENTRY(    0x00001,0b0,    0b1,0b0,0b000011,   0b0,    0xAA00, 0b1,    0x000,  0b1,    0x00001,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    3 )},
        {TEST_TLB_ENTRY(    0x55551,0b0,    0b1,0b0,0b000011,   0b0,    0xAAFF, 0b1,    0x155,  0b0,    0x55551,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xAAAA1,0b0,    0b1,0b0,0b000011,   0b0,    0xAAAA, 0b1,    0x2AA,  0b0,    0xAAAA1,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xFFFF1,0b0,    0b1,0b0,0b000011,   0b0,    0xAA55, 0b1,    0x3FF,  0b1,    0xAAAA1,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    3 )}
    },
    {
        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    0 )},
        {TEST_TLB_ENTRY(    0x50555,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x155,  0b0,    0x50555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xA0AAA,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x2AA,  0b0,    0xA0AAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xF0FFF,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x3FF,  0b1,    0xF0FFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    0 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    1 )},
        {TEST_TLB_ENTRY(    0x50555,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x155,  0b0,    0x50555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xA0AAA,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x2AA,  0b0,    0xA0AAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xF0FFF,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x3FF,  0b1,    0xF0FFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    1 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    2 )},
        {TEST_TLB_ENTRY(    0x50555,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x155,  0b0,    0x50555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xA0AAA,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x2AA,  0b0,    0xA0AAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xF0FFF,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x3FF,  0b1,    0xF0FFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    2 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    3 )},
        {TEST_TLB_ENTRY(    0x50555,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x155,  0b0,    0x50555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xA0AAA,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x2AA,  0b0,    0xA0AAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xF0FFF,0b1,    0b1,0b0,0b000111,   0b1,    0xFFFF, 0b1,    0x3FF,  0b1,    0xF0FFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    3 )}
    },
    {
        {TEST_TLB_ENTRY(    0x0000E,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x000,  0b1,    0x0000E,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    0 )},
        {TEST_TLB_ENTRY(    0x5555E,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x155,  0b0,    0x5555E,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xAAAAE,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x2AA,  0b0,    0xAAAAE,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xFFFFE,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x3FF,  0b1,    0xFFFFE,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    0 )},

        {TEST_TLB_ENTRY(    0x0000E,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x000,  0b1,    0x0000E,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    1 )},
        {TEST_TLB_ENTRY(    0x5555E,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x155,  0b0,    0x5555E,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xAAAAE,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x2AA,  0b0,    0xAAAAE,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xFFFFE,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x3FF,  0b1,    0xFFFFE,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    1 )},

        {TEST_TLB_ENTRY(    0x0000E,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x000,  0b1,    0x0000E,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    2 )},
        {TEST_TLB_ENTRY(    0x5555E,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x155,  0b0,    0x5555E,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xAAAAE,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x2AA,  0b0,    0xAAAAE,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xFFFFE,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x3FF,  0b1,    0xFFFFE,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    2 )},

        {TEST_TLB_ENTRY(    0x0000E,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x000,  0b1,    0x0000E,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    3 )},
        {TEST_TLB_ENTRY(    0x5555E,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x155,  0b0,    0x5555E,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xAAAAE,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x2AA,  0b0,    0xAAAAE,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xFFFFE,0b0,    0b1,0b1,0b001111,   0b1,    0x5555, 0b1,    0x3FF,  0b1,    0xFFFFE,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    3 )}
    },
    {
        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b1,0b011111,   0b0,    0xAAAA, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    0 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b1,0b011111,   0b0,    0xAAAF, 0b1,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b1,0b011111,   0b0,    0xAAA0, 0b1,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b1,0b011111,   0b0,    0xAAA5, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    0 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b1,0b011111,   0b0,    0xAAAA, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    1 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b1,0b011111,   0b0,    0xAAAF, 0b1,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b1,0b011111,   0b0,    0xAAA0, 0b1,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b1,0b011111,   0b0,    0xAAA5, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    1 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b1,0b011111,   0b0,    0xAAAA, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    2 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b1,0b011111,   0b0,    0xAAAF, 0b1,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b1,0b011111,   0b0,    0xAAA0, 0b1,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b1,0b011111,   0b0,    0xAAA5, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    2 )},

        {TEST_TLB_ENTRY(    0x00000,0b1,    0b1,0b1,0b011111,   0b0,    0xAAAA, 0b1,    0x000,  0b1,    0x00000,0b1,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    3 )},
        {TEST_TLB_ENTRY(    0x55555,0b1,    0b1,0b1,0b011111,   0b0,    0xAAAF, 0b1,    0x155,  0b0,    0x55555,0b1,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xAAAAA,0b1,    0b1,0b1,0b011111,   0b0,    0xAAA0, 0b1,    0x2AA,  0b0,    0xAAAAA,0b1,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xFFFFF,0b1,    0b1,0b1,0b011111,   0b0,    0xAAA5, 0b1,    0x3FF,  0b1,    0xFFFFF,0b1,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    3 )}
    },
    {
        {TEST_TLB_ENTRY(    0x00007,0b0,    0b1,0b1,0b111111,   0b1,    0xFFFF, 0b1,    0x000,  0b1,    0x00007,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    0 )},
        {TEST_TLB_ENTRY(    0x55557,0b0,    0b1,0b1,0b111111,   0b1,    0xFFEA, 0b0,    0x155,  0b0,    0x55557,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xAAAA7,0b0,    0b1,0b1,0b111111,   0b1,    0xFFD5, 0b0,    0x2AA,  0b0,    0xAAAA7,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xFFFF7,0b0,    0b1,0b1,0b111111,   0b1,    0xFFC0, 0b1,    0x3FF,  0b1,    0xFFFF7,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    0 )},

        {TEST_TLB_ENTRY(    0x00007,0b0,    0b1,0b1,0b111111,   0b1,    0xFFFF, 0b1,    0x000,  0b1,    0x00007,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    1 )},
        {TEST_TLB_ENTRY(    0x55557,0b0,    0b1,0b1,0b111111,   0b1,    0xFFEA, 0b0,    0x155,  0b0,    0x55557,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xAAAA7,0b0,    0b1,0b1,0b111111,   0b1,    0xFFD5, 0b0,    0x2AA,  0b0,    0xAAAA7,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xFFFF7,0b0,    0b1,0b1,0b111111,   0b1,    0xFFC0, 0b1,    0x3FF,  0b1,    0xFFFF7,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    1 )},

        {TEST_TLB_ENTRY(    0x00007,0b0,    0b1,0b1,0b111111,   0b1,    0xFFFF, 0b1,    0x000,  0b1,    0x00007,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    2 )},
        {TEST_TLB_ENTRY(    0x55557,0b0,    0b1,0b1,0b111111,   0b1,    0xFFEA, 0b0,    0x155,  0b0,    0x55557,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xAAAA7,0b0,    0b1,0b1,0b111111,   0b1,    0xFFD5, 0b0,    0x2AA,  0b0,    0xAAAA7,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xFFFF7,0b0,    0b1,0b1,0b111111,   0b1,    0xFFC0, 0b1,    0x3FF,  0b1,    0xFFFF7,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    2 )},

        {TEST_TLB_ENTRY(    0x00007,0b0,    0b1,0b1,0b111111,   0b1,    0xFFFF, 0b1,    0x000,  0b1,    0x00007,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    3 )},
        {TEST_TLB_ENTRY(    0x55557,0b0,    0b1,0b1,0b111111,   0b1,    0xFFEA, 0b0,    0x155,  0b0,    0x55557,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xAAAA7,0b0,    0b1,0b1,0b111111,   0b1,    0xFFD5, 0b0,    0x2AA,  0b0,    0xAAAA7,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xFFFF7,0b0,    0b1,0b1,0b111111,   0b1,    0xFFC0, 0b1,    0x3FF,  0b1,    0xFFFF7,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    3 )}
    }
};

static void prepare_utlb( void );
static void fill_utlb_entries( uint32_t const template_id );
static void check_utlb_entries( uint32_t const template_id );
static void restore_utlb( void );

int main() {
    test_event_send_test_id("test_oi10_cpu_mem_003");

    prepare_utlb();

    for( uint32_t template_id = 0; template_id < ARRAY_SIZE(templates); template_id++ ) {
        fill_utlb_entries( template_id );
        check_utlb_entries( template_id );
    }

    restore_utlb();

    return 0;
}


/* <<<<<<<<<<Implementation>>>>>>>>>> */

static uint32_t rom_erpn, rom_rpn, rom_rpn_new;
static uint32_t spr_sspcr;

static void prepare_utlb( void ) {
    rumboot_printf( "Prepare UTLB\n");

    spr_write( SPR_MMUCR, MMU_MMUCR( 0b0,  0b0,0b0,    0b0,     MEM_WINDOW_SHARED ) );
    int32_t const rom_ra            = tlbsx( BOOTROM_BASE );
    TEST_ASSERT( rom_ra >= 0, "ROM TLB not found" );
    uint32_t const rom_tlbe_data    = tlbre( rom_ra, MMU_TLB_ENTRY_DATA );
    rom_erpn                        = GET_BITS(rom_tlbe_data, MMU_TLBE_DATA_ERPN_i, MMU_TLBE_DATA_ERPN_n);
    rom_rpn                         = GET_BITS(rom_tlbe_data, MMU_TLBE_DATA_RPN_i, MMU_TLBE_DATA_RPN_n);
    rom_rpn_new                     = 0xFFF00 & rom_rpn;
    struct tlb_entry const rom_replace_tlb[] = {
//       MMU_TLB_ENTRY( ERPN,       RPN,        EPN,        DSIZ,               IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY( 0x01F,      0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB, 0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_0,     0b0 )},
        {MMU_TLB_ENTRY( rom_erpn,   rom_rpn_new,0xFFF00,    MMU_TLBE_DSIZ_1MB,  0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_0,     0b1 )}
    };
    msync();
    write_tlb_entries( rom_replace_tlb, ARRAY_SIZE(rom_replace_tlb) );

    spr_sspcr = spr_read( SPR_SSPCR );

    spr_write( SPR_SSPCR, ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1MB)  << MMU_SSPCR_ORD1_i)
                        | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_64KB) << MMU_SSPCR_ORD2_i)
                        | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_4KB)  << MMU_SSPCR_ORD3_i)
                        | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_16KB) << MMU_SSPCR_ORD4_i)
                        | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_16MB) << MMU_SSPCR_ORD5_i)
                        | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_256MB)<< MMU_SSPCR_ORD6_i)
                        | ((MMU_SUSPCR_ORD_SHARED | MMU_XSPCR_ORD_1GB)  << MMU_SSPCR_ORD7_i) );
}

static void restore_utlb( void ) {
    rumboot_printf( "Restore UTLB\n");

    struct tlb_entry const restore_tlb[] = {
// Invalidate test TLBs
//       TEST_TLB_ENTRY(    EPN,    EPNp,   V,  TS, DSIZ,       DSIZp,  TID,    TIDp,   ERPN,   ERPNp,  RPN,    RPNp,   L1I,L1D,U,      W,  I,  M,  G,  E,  UX, UW, UR, SX, SW, SR,     SPp,    WAY )
        {TEST_TLB_ENTRY(    0x00007,0b0,    0b0,0b1,0b111111,   0b0,    0xFFFF, 0b1,    0x000,  0b1,    0x00007,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    0 )},
        {TEST_TLB_ENTRY(    0x55557,0b0,    0b0,0b1,0b111111,   0b0,    0xFFEA, 0b0,    0x155,  0b0,    0x55557,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xAAAA7,0b0,    0b0,0b1,0b111111,   0b0,    0xFFD5, 0b0,    0x2AA,  0b0,    0xAAAA7,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    0 )},
        {TEST_TLB_ENTRY(    0xFFFF7,0b0,    0b0,0b1,0b111111,   0b0,    0xFFC0, 0b1,    0x3FF,  0b1,    0xFFFF7,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    0 )},

        {TEST_TLB_ENTRY(    0x00007,0b0,    0b0,0b1,0b111111,   0b0,    0xFFFF, 0b1,    0x000,  0b1,    0x00007,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    1 )},
        {TEST_TLB_ENTRY(    0x55557,0b0,    0b0,0b1,0b111111,   0b0,    0xFFEA, 0b0,    0x155,  0b0,    0x55557,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xAAAA7,0b0,    0b0,0b1,0b111111,   0b0,    0xFFD5, 0b0,    0x2AA,  0b0,    0xAAAA7,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    1 )},
        {TEST_TLB_ENTRY(    0xFFFF7,0b0,    0b0,0b1,0b111111,   0b0,    0xFFC0, 0b1,    0x3FF,  0b1,    0xFFFF7,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    1 )},

        {TEST_TLB_ENTRY(    0x00007,0b0,    0b0,0b1,0b111111,   0b0,    0xFFFF, 0b1,    0x000,  0b1,    0x00007,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    2 )},
        {TEST_TLB_ENTRY(    0x55557,0b0,    0b0,0b1,0b111111,   0b0,    0xFFEA, 0b0,    0x155,  0b0,    0x55557,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xAAAA7,0b0,    0b0,0b1,0b111111,   0b0,    0xFFD5, 0b0,    0x2AA,  0b0,    0xAAAA7,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    2 )},
        {TEST_TLB_ENTRY(    0xFFFF7,0b0,    0b0,0b1,0b111111,   0b0,    0xFFC0, 0b1,    0x3FF,  0b1,    0xFFFF7,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    2 )},

        {TEST_TLB_ENTRY(    0x00007,0b0,    0b0,0b1,0b111111,   0b0,    0xFFFF, 0b1,    0x000,  0b1,    0x00007,0b0,    0b0,0b0,0b0000, 0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,0b0,    0b1,    3 )},
        {TEST_TLB_ENTRY(    0x55557,0b0,    0b0,0b1,0b111111,   0b0,    0xFFEA, 0b0,    0x155,  0b0,    0x55557,0b0,    0b1,0b0,0b1010, 0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xAAAA7,0b0,    0b0,0b1,0b111111,   0b0,    0xFFD5, 0b0,    0x2AA,  0b0,    0xAAAA7,0b0,    0b1,0b1,0b0101, 0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,0b1,0b0,    0b0,    3 )},
        {TEST_TLB_ENTRY(    0xFFFF7,0b0,    0b0,0b1,0b111111,   0b0,    0xFFC0, 0b1,    0x3FF,  0b1,    0xFFFF7,0b0,    0b1,0b1,0b1111, 0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,0b1,    0b0,    3 )},
//  Restore ROM TLB
//       MMU_TLB_ENTRY( ERPN,       RPN,        EPN,        DSIZ,               IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY( rom_erpn,   rom_rpn_new,0xFFF00,    MMU_TLBE_DSIZ_1MB,  0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_0,     0b0 )},
        {MMU_TLB_ENTRY( rom_erpn,   rom_rpn,    0xFFFF0,    MMU_TLBE_DSIZ_64KB, 0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_0,     0b1 )}
    };
    msync();
    write_tlb_entries( restore_tlb, ARRAY_SIZE(restore_tlb) );

    spr_write( SPR_SSPCR, spr_sspcr );
}

static void fill_utlb_entries( uint32_t const template_id ) {
    rumboot_printf( "Fill UTLB entries for template %d\n", template_id );

    write_tlb_entries( templates[template_id], ARRAY_SIZE(templates[0]) );
}

static uint32_t const check_indexes[] = {
        0x00,                       0x55,                       0xAA,                       0xFF
};

#define INDEX_TEMPLATES( template_id, index_offset ) {\
    &templates[template_id][index_offset+0],\
    &templates[template_id][index_offset+4],\
    &templates[template_id][index_offset+8],\
    &templates[template_id][index_offset+12]\
}
static struct tlb_entry const * const templates_check_map[][4][4] = {
    {   INDEX_TEMPLATES( 0, 0 ),    INDEX_TEMPLATES( 0, 1 ),    INDEX_TEMPLATES( 0, 2 ),    INDEX_TEMPLATES( 0, 3 ) },
    {   INDEX_TEMPLATES( 1, 1 ),    INDEX_TEMPLATES( 1, 0 ),    INDEX_TEMPLATES( 1, 3 ),    INDEX_TEMPLATES( 1, 2 ) },
    {   INDEX_TEMPLATES( 2, 0 ),    INDEX_TEMPLATES( 2, 3 ),    INDEX_TEMPLATES( 2, 2 ),    INDEX_TEMPLATES( 2, 1 ) },
    {   INDEX_TEMPLATES( 3, 3 ),    INDEX_TEMPLATES( 3, 2 ),    INDEX_TEMPLATES( 3, 1 ),    INDEX_TEMPLATES( 3, 0 ) },
    {   INDEX_TEMPLATES( 4, 1 ),    INDEX_TEMPLATES( 4, 0 ),    INDEX_TEMPLATES( 4, 3 ),    INDEX_TEMPLATES( 4, 2 ) },
    {   INDEX_TEMPLATES( 5, 2 ),    INDEX_TEMPLATES( 5, 3 ),    INDEX_TEMPLATES( 5, 0 ),    INDEX_TEMPLATES( 5, 1 ) },
    {   INDEX_TEMPLATES( 6, 3 ),    INDEX_TEMPLATES( 6, 2 ),    INDEX_TEMPLATES( 6, 1 ),    INDEX_TEMPLATES( 6, 0 ) }
};

enum {
    TEC_CHECK_UTLB_ENTRIES = TEST_EVENT_CODE_MIN,
};

static void check_utlb_entries( uint32_t const template_id ) {
    rumboot_printf( "Check UTLB entries for template %d\n", template_id );

    uint32_t const event [] = { TEC_CHECK_UTLB_ENTRIES, template_id, };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event, ARRAY_SIZE(event) );

    struct tlb_entry const * template;
    uint32_t ra, tlbe_tag, tlbe_data, tlbe_attr, template_attributes;

    for( uint32_t i = 0; i < ARRAY_SIZE(check_indexes); i++ ) {
        for( uint32_t way = 0; way < MMU_UTLB_WAYS_N; way++ ) {
            template    = templates_check_map[template_id][i][way];

            ra          = MMU_TLBRE_RA( check_indexes[i], way );

            tlbe_tag    = tlbre( ra, MMU_TLB_ENTRY_TAG );
            tlbe_data   = tlbre( ra, MMU_TLB_ENTRY_DATA );
            tlbe_attr   = tlbre( ra, MMU_TLB_ENTRY_ATTR );

            template_attributes = (template->attributes & MMU_TLBE_ATTR_ONLY_MASK)
                                | (GET_BIT(template->attributes, MMU_TLBE_ATTR_SPPar_test_i) << MMU_TLBE_ATTR_SPPar_i);

            TEST_ASSERT( tlbe_tag == template->tag, "Wrong TAG");
            TEST_ASSERT( tlbe_data == template->data, "Wrong DATA");
            TEST_ASSERT( tlbe_attr == template_attributes, "Wrong ATTR");
        }
    }
}
