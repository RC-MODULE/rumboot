#include <stdint.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>

extern int main(int argc, const char *argv[]);

/* TODO: Handle 64-bits here */
#if defined(RUMBOOT_NOINIT)
 #define 	THE_ENTRY_POINT	((uint32_t) &main)
#elif defined(RUMBOOT_NOENTRY)
 #define 	THE_ENTRY_POINT	0
#else
 #define 	THE_ENTRY_POINT	((uint32_t) &rumboot_main)
#endif

static const __attribute__((used)) __attribute__((section(".header")))
__attribute__((packed)) struct rumboot_bootheader hdr  = {
	.magic		= RUMBOOT_HEADER_MAGIC,
#ifdef __NM__
	/* NeuroMatrix images always need decapsulation */
	.flags      = RUMBOOT_FLAG_DECAPS,
#endif
	.version	= RUMBOOT_HEADER_VERSION,
	.chip_id	= RUMBOOT_PLATFORM_CHIPID,
	.chip_rev	= RUMBOOT_PLATFORM_CHIPREV,
#if defined (__NM__) && !defined(RUMBOOT_TARGET_CPU_CLUSTER)
	.target_cpu_cluster = 1,
#elif defined (RUMBOOT_TARGET_CPU_CLUSTER)
	.target_cpu_cluster = RUMBOOT_TARGET_CPU_CLUSTER,
#endif
	.entry_point32	= { THE_ENTRY_POINT, 0x0 },
};
