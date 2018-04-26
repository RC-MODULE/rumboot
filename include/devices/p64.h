/**
 *
 */

#ifndef P64_H
#define P64_H


#include <stdint.h>
#include <stdbool.h>
#include <platform/oi10/platform/reg_access/reg_access_p64.h>

typedef struct p64_cfg
{
    P64CR_PRI_N     pri;
    bool            spen;
    bool            owen;
    bool            ipc;
    uint8_t         ifmid0;
    uint8_t         ifmid1;
    bool            ife0;
    bool            ife1;
} p64_cfg;

void p64_get_configuration(uint32_t const base_addr, p64_cfg * cfg);
void p64_set_configuration(uint32_t const base_addr, p64_cfg * cfg);
void p64_set_split_range_address(uint32_t const base_addr, uint32_t const addrl1, uint32_t const addrh1);
#endif // P64_H
