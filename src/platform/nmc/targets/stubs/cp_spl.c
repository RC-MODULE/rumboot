#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <platform/devices.h>

int main()
{
    rumboot_platform_runtime_info->magic = 0xb00bc0de;
    rumboot_platform_runtime_info->in.opcode  = 0;
    rumboot_platform_runtime_info->out.opcode = 0;
    /* Start event processing ! */
    rumboot_platform_runtime_info->in.magic  = RUMBOOT_SYNC_MAGIC_IN;
    rumboot_platform_runtime_info->out.magic = RUMBOOT_SYNC_MAGIC_OUT;
    /* TODO: Send a char via cp */
	uint32_t c = 8;
    rumboot_platform_event_raise(EVENT_TERM, &c, 1);
    return 0;
}