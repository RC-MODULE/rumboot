#include <stdio.h>
#include <stdlib.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/can.h>
#include <devices/sctl.h>
#include <platform/devices.h> 

extern int CCAN0_REQUESTS_CCAN1(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3);

/* The actual workhorse */
int run_can0_requests_can1(uint32_t can0_base_addr, uint32_t can1_base_addr) {
    int ret = CCAN0_REQUESTS_CCAN1(can0_base_addr, 0, 0, can1_base_addr);
    if (ret) { rumboot_printf("return %08x test failed\n", ret); }
    return ret;
}

int main()
{
    unsigned can0_base_addr = CAN0_BASE ^ 0x6000;
    unsigned can1_base_addr = CAN1_BASE ^ 0x6000;

    // bypass rc_module part
    (*(volatile int*) (SCTL_BASE + SCTL_CAN0)) = 0x7;
    // bypass rc_module part
    (*(volatile int*) (SCTL_BASE + SCTL_CAN1)) = 0x7;

    int ret = run_can0_requests_can1(can0_base_addr, can1_base_addr);

    rumboot_printf("main run_can0_requests_can1 end\n");

    return ret;
}

