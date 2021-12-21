#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <rumboot/platform.h>
#include <rumboot/hexdump.h>
#include <platform/devices.h>
extern uintptr_t rumboot_native_request_device(int dev_id, uint32_t addr, size_t len);

int main()
{
    rumboot_printf("VPE ID: %x\n", ioread32(DUT_BASE + 0x1000 + 4));
	return (ioread32(DUT_BASE + 0x1000 + 4) == 0xabcd2021) ? 0 : 1;
}


