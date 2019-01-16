#include <stdint.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
static inline int get_cpsr()
{
    int result = 0;
    asm volatile (
    "mrs %0, cpsr\n" : "=r" (result) );
    return result;
}

__attribute__( ( always_inline ) ) static inline uint32_t __get_APSR(void)
{
  uint32_t result;

  asm volatile ("MRS %0, apsr" : "=r" (result) );
  return(result);
}

__attribute__( ( always_inline ) ) static inline  void set_cpsr(uint32_t cpsr)
{
  asm volatile ("MSR cpsr, %0" : : "r" (cpsr) : "memory");
}

static __attribute__( ( always_inline ) ) uint32_t __get_FP_usr(void)
{
  uint32_t cpsr = get_cpsr();
  uint32_t result;
  asm volatile(
//    "CPS     #0x13  \n"
    "MOV     %0, fp   " : "=r"(result) : : "memory"
   );
  set_cpsr(cpsr);
  asm volatile ("isb 0xF":::"memory");
  return result;
}


void rumboot_arch_stacktrace(void)
{
  uint32_t topfp = (int) __get_FP_usr();
    for (int i=0; i < 32; i++) {
        uint32_t pos;
        uint32_t fp = *(((uint32_t*)topfp) -3);
        uint32_t sp = *(((uint32_t*)topfp) -2);
        uint32_t lr = *(((uint32_t*)topfp) -1);
        uint32_t pc = *(((uint32_t*)topfp) -0);


        pos = lr - 4;


        rumboot_printf("frame[%d] address: 0x%x (FP: 0x%x SP: 0x%x LR: 0x%x PC: 0x%x)\n",
        i, pos, fp, sp, lr, pc);
        if (fp == 0)
          break;
        if ((fp < (uint32_t)&rumboot_platform_stack_area_start) ||
            (fp > (uint32_t)&rumboot_platform_stack_area_end)) {
                rumboot_printf("Next frame looks invalid, we'll stop here, sorry\n");
                break;
        }
        topfp = fp;
    }
}

void rumboot_arch_exception(int id, const char *name)
{
        rumboot_printf("\n\n\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
        rumboot_printf("--- Guru Meditation ---\n");
        rumboot_printf("  -   Registers     -\n");
        rumboot_printf("CPSR: 0x%x \n", get_cpsr());
        rumboot_printf("APSR: 0x%x \n", __get_APSR());
        rumboot_printf("VBAR: 0x%x \n", arm_vbar_get());
        rumboot_printf("  -   Stack trace   -\n");
        rumboot_arch_stacktrace();
        rumboot_printf("---       ---       ---\n");
        rumboot_platform_sv_event("EXCEPTION");
        rumboot_platform_panic("Please reset or power-cycle the board\n");
}
