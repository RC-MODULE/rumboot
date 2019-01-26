#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <stdbool.h>
#include <alloca.h>
#include <rumboot/timer.h>

int main(int argc, char const *argv[]) {
    struct rumboot_config conf;
	rumboot_platform_read_config(&conf);
	rumboot_platform_init_loader(&conf);

    uint32_t pid = spr_read(0x30);
    rumboot_printf("PID: %d\n", pid);
    if (pid != 0) {
            rumboot_printf("PID must be 0\n");
            exit(1);
    }
    exit(0);
}
