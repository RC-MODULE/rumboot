#include <stdint.h>
#include <string.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

struct rumboot_runtime_info *rumboot_platform_runtime_info;

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_ANONYMOUS | MAP_SHARED;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, 0, 0);
}

int g_argc = 0;
char *g_argv[64];

void __attribute__((constructor)) rumboot_main()
{
    rumboot_platform_runtime_info = create_shared_memory(sizeof(rumboot_platform_runtime_info));

    /* Clean up everything beyound marker */
    memset(rumboot_platform_runtime_info, 0x0, sizeof(rumboot_platform_runtime_info));
    rumboot_platform_setup();
}
