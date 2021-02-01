#ifndef O32T_INTERRUPTS_H
#define O32T_INTERRUPTS_H
#include <platform/oi10/platform/interrupts.h>
/* Place all O32-specific interrupts here. 
   Use #undef carefully where needed
*/
#define CP0_RCV_INT                    0x4E
#define CP0_TRM_INT                    0x4F
#define CP1_RCV_INT                    0x50
#define CP1_TRM_INT                    0x51

#define MKIO2_INT                   (54)
#define MKIO3_INT                   (55)

#endif // !O32T_INTERRUPTS_H

