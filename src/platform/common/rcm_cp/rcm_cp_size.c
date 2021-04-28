#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <devices/rcm_cp.h>
#include <platform/devices.h>

#ifndef COM1_BASE
    #error Please provide COM1_BASE
#endif

#ifndef COM0_BASE
    #error Please provide COM0_BASE
#endif

#ifndef CHUNK_SIZE
    #define CHUNK_SIZE 128
#endif

#ifndef BASE_FREQ
    #define BASE_FREQ 100000
#endif

char tx_buf[CHUNK_SIZE * 2] __attribute__((aligned (8))) = "HELLO WORLD"; 
char rx_buf[CHUNK_SIZE * 2] __attribute__((aligned (8))) = "XXXXXXXXXXX"; 

int main()
{
    memset(tx_buf, 0xa, CHUNK_SIZE * 2);
    memset(rx_buf, 0xb, CHUNK_SIZE * 2);
    struct rcm_cp_instance c0; 
    cp_instance_init(&c0, COM0_BASE, BASE_FREQ);
    struct rcm_cp_instance c1; 
    cp_instance_init(&c1, COM1_BASE, BASE_FREQ);

    rumboot_printf("Initialized instances\n");
    cp_start_tx(&c0, tx_buf, CHUNK_SIZE);
    cp_start_rx(&c1, rx_buf, CHUNK_SIZE);
    cp_wait(&c1, 1, 1, CHUNK_SIZE * 50);
    cp_wait(&c0, 1, 1, CHUNK_SIZE * 50);
    #ifdef __PPC__
    asm("msync");
    #endif

    int pos = 0; 
    while (rx_buf[pos] == 0xa) {
        pos++;
    }
    rumboot_printf("Written %d bytes\n", pos);
    return (pos == CHUNK_SIZE) ? 0 : 1;
}
