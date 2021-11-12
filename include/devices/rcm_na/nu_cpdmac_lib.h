
#ifndef NU_CPDMAC_LIB_H
#define NU_CPDMAC_LIB_H

#include <stdint.h>

void nu_cpdmac_trn512_config       (uintptr_t base, void *addr, int size);
void nu_cpdmac_trn512_run          (uintptr_t base);
void nu_cpdmac_trn512_wait_complete(uintptr_t base);

void nu_cpdmac_rcv512_config       (uintptr_t base, void *addr, int size);
void nu_cpdmac_rcv512_run          (uintptr_t base);
void nu_cpdmac_rcv512_wait_complete(uintptr_t base);

void nu_cpdmac_trn256_config       (uintptr_t base, void *addr, int size);
void nu_cpdmac_trn256_run          (uintptr_t base);
void nu_cpdmac_trn256_wait_complete(uintptr_t base);

void nu_cpdmac_rcv256_config       (uintptr_t base, void *addr, int size);
void nu_cpdmac_rcv256_run          (uintptr_t base);
void nu_cpdmac_rcv256_wait_complete(uintptr_t base);


#endif // NU_CPDMAC_LIB_H
