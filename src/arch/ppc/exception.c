#include <stdint.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <arch/ppc_476fp_config.h>
#include <rumboot/io.h>

void rumboot_arch_stacktrace(void)
{
   int depth;
   int size = 32;
   uint32_t stackptr, lr, *addr;
   // get link register
   asm volatile ("mflr %0"
      : "=r" (lr)
   );
   // link register is assigned to depth[0]
   rumboot_printf("frame[0] address 0x%x\n", (void *) (lr - 4));
   // get stackpointer
   asm volatile ("stw %%sp, 0(%0)" : : "b"((uint32_t) &stackptr));
   // assign stack ptr to address
   addr = (uint32_t *) stackptr;
   // get the frames
   if (*addr) {
      // skip first two frames because this function
      // does create a stackframe but doesn't set lr on
      // the previous one.
      addr = (uint32_t *) *addr;
      if (*addr) {
         addr = (uint32_t *) *addr;
      }
   }

   for (depth=1; (depth<size && *addr); ++depth) {
      uint32_t * next = (uint32_t *) *addr;
      rumboot_printf("frame[%d] address 0x%x\n", depth, (void *) (*(addr+1) - 4));
      addr = next;
   }
}

void rumboot_arch_exception(int id, const char *name)
{
        rumboot_printf("\n\n\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
        rumboot_printf("--- Guru Meditation ---\n");
        rumboot_printf("  -   Registers     -\n");
        rumboot_printf("MSR:     0x%x\n", msr_read());
        rumboot_printf("SRR0:    0x%x\n", spr_read(SPR_SRR0));
        rumboot_printf("SRR1:    0x%x\n", spr_read(SPR_SRR1));
        rumboot_printf("CSRR0:   0x%x\n", spr_read(SPR_CSRR0));
        rumboot_printf("CSRR1:   0x%x\n", spr_read(SPR_CSRR1));
        rumboot_printf("MCSRR0:  0x%x\n", spr_read(SPR_MCSRR0));
        rumboot_printf("MCSRR1:  0x%x\n", spr_read(SPR_MCSRR1));
        rumboot_printf("DEAR:    0x%x\n", spr_read(SPR_DEAR));
        rumboot_printf("ESR:     0x%x\n", spr_read(SPR_ESR));
        rumboot_printf("MCSR_RW: 0x%x\n", spr_read(SPR_MCSR_RW));
        rumboot_printf("  -   Stack trace   -\n");
        rumboot_arch_stacktrace();
        rumboot_printf("---       ---       ---\n");
        rumboot_platform_sv_event("EXCEPTION");
        rumboot_platform_panic("Please reset or power-cycle the board\n");
}
