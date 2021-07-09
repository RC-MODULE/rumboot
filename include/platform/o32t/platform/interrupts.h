#ifndef O32T_INTERRUPTS_H
#define O32T_INTERRUPTS_H
#include <platform/oi10/platform/interrupts.h>
/* Place all O32-specific interrupts here. 
   Use #undef carefully where needed
*/
#define CP0_RCV_INT                    78
#define CP0_TRM_INT                    79
#define CP1_RCV_INT                    80
#define CP1_TRM_INT                    81

#define AXI_MDMAC_ERR                  82
#define AXI_MDMAC_FINISH               83

#define MKIO2_INT                   (54)
#define MKIO3_INT                   (55)

#define NMC4_INT_0                  (84)
#define NMC4_INT_1                  (85)


#endif // !O32T_INTERRUPTS_H

