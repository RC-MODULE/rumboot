#ifndef DEVICES_H
#define DEVICES_H
#include <stdint.h>

extern uintptr_t DUT_BASE;
#define NPE_BASE DUT_BASE
#define NU_CPDMAC_ASM_BASE 0

 /**
  * \defgroup platform_native_register_map Register Map
  * \ingroup platform_native
  * This file contains physical addresses of devices present in SoC.
  * For developers: Please, keep the list sorted by device address
  * and formatted properly.
  *
  * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
  *
  * \addtogroup platform_native_register_map
  *
  *
  * @{
  *
  */

 /**
 * @}
 */


#endif /* end of include guard: DEVICES_H */
