/**
 * \defgroup test_hello_iram Hello World
 * \ingroup tests_basis
 *
 * Description
 * ===========
 *
 * Prints hello world via gtube and exits with 0 exit code
 *
 * Verification environment/plusargs
 * ==============================================
 * None
 *
 * CFLAGS
 * ======
 * None
 *
 * Expected output
 * ===============
 *
 *
 * How to compile
 * ==============
 *
 * \code{cmake}
 *    add_rumboot_target(
 *       CONFIGURATION IRAM
 *       FILES common/tools/drom-check.c
 *       NAME "hello-world"
 *   )
 * \endcode
 *
 */

#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <stdlib.h>

int main()
{
	rumboot_printf("Hello world from IRAM!\n");
	return 0;
}
