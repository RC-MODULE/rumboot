#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <stdbool.h>
#include <rumboot/xmodem.h>

#ifndef SIZE
	#define SIZE 4096 * 1024
#endif
int main()
{
        	 			void *data;
								size_t maxsize;
#ifndef RUMBOOT_NATIVE
								struct rumboot_bootheader *hdr = rumboot_platform_get_spl_area(&maxsize);
#else
								struct rumboot_bootheader *hdr = malloc(SIZE);
#endif
								rumboot_irq_set_exception_handler(rumboot_arch_exception);
								struct rumboot_config conf;
								rumboot_platform_runtime_info->persistent[0] = 0;
								rumboot_platform_read_config(&conf);
								rumboot_platform_init_loader(&conf);
								rumboot_platform_sv_event("HOST");

								//rumboot_putchar('X');

								while (rumboot_platform_getchar(1000) != 'X');;
								int ret = xmodem_get((void *) hdr, maxsize);

								if (ret <= 0)
									return 1;

								ssize_t len = rumboot_bootimage_check_header(NULL, hdr, &data);
								if (len == -EBADMAGIC) {
																dbg_boot(NULL, "Bad magic");
																return 1;
								}

								if (len < 0) {
																dbg_boot(NULL, "Header error: %s\n", rumboot_strerror(len));
																hdr->magic = 0;
																return 1;
								}
								if (0 == rumboot_bootimage_check_data(NULL, hdr)) {
																hdr->magic = 0x0;
																ret = rumboot_platform_exec(hdr, 0);
								}
								/* If we're here, something didn't work out */
								return 1;
}
