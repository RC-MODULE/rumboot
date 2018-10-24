#include <stdio.h>
#include <stdlib.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/can.h>
#include <platform/regs/sctl.h>
#include <platform/devices.h> 

extern int CCAN_LOOPBACK(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3);

/* The actual workhorse */
int run_can_loopback(uint32_t can_base_addr) {
    int test_state = 1;
    int ret = CCAN_LOOPBACK(can_base_addr, 0, 0, test_state);
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

    int ret0 = run_can_loopback(can0_base_addr);
    int ret1 = run_can_loopback(can1_base_addr);

    rumboot_printf("main run_can_loopback end\n");

    return ret0|ret1;
}

