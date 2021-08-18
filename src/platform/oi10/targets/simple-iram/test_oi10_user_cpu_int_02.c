#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <arch/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/ppc_476fp_timer_fields.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_asm.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <devices/ugly/plb6mcif2.h>
#include <devices/ugly/emi.h>
#include <devices/mpic128.h>
#include <regs/fields/emi.h>
#include <platform/interrupts.h>
#include <arch/dma2plb6.h>

#include <platform/regs/regs_ltrace.h>
#include <platform/devices/ltrace.h>
#include <platform/regs/regs_itrace.h>
#include <platform/devices/itrace.h>

#include <arch/regs/dma2plb6.h>
#include <platform/regs/regs_plb4plb6.h>
#include <platform/regs/regs_plb4ahb.h>
#include <arch/regs/fields/plb6bc.h>
#include <platform/regs/regs_dcrarb.h>
#include <arch/regs/regs_p64.h>

#include <rumboot/printf.h>

#define TEST_PACKED

struct endian_t {
  int a; /* 0x1112_1314 word */
  double b; /* 0x2122_2324_2526_2728 doubleword */ // 0x3FB9_9999_9999_999A
  char * c; /* 0x3132_3334 word */
  char d[7]; /* 'A', 'B', 'C', 'D', 'E', 'F', 'G' array of bytes */
  short e; /* 0x5152 halfword */
  int f; /* 0x6162_6364 word */
}
__attribute__((packed, aligned(8)))
;

struct endian_t s = { 0x11121314, 0.1, (char *) 0x31323334,  {'A', 'B', 'C', 'D', 'E', 'F', 'G'}, 0x5152, 0x61626364 };
struct endian_t d;

union union_t {
    double a;
    uint32_t b[2];
} u __attribute__((aligned(8)));

// 20, 21, 22, CR
asm (
ASM_TEXT(   .section ".text.rom","ax",@progbits                         )
ASM_TEXT(   .align 8                                                    )
ASM_TEXT(   ivor5_unaligned_handler2:                            )
// enable MSR[FP]
// save r20, r21, r22
ASM_TEXT(  mtspr  277, r20      ) // SPR_SPRG5, 0x105, 261, 277
ASM_TEXT(  mtspr  278, r21      ) // SPR_SPRG6, 0x106, 262, 278
ASM_TEXT(  mtspr  279, r22      ) // SPR_SPRG7, 0x107, 263, 279
// enable MSR[FP]
ASM_TEXT(  mfmsr r20            )
ASM_TEXT(  ori  r20, r20, 0x2000 )
ASM_TEXT(  mtmsr r20            )
ASM_TEXT(  mfspr 20, 26         ) // SPR_SRR0, 26
ASM_TEXT(  lwz   21, 0(20)      ) // cmd
ASM_TEXT(  andis. 21, 21, 0xFC00 ) // 
ASM_TEXT(  addi  22, 0, 26      ) //       
ASM_TEXT(  srw   21, 21, 22     ) // shift right on [r22]
ASM_TEXT(  cmplwi 7, 21, 0x32     ) 
ASM_TEXT(  beq    7, l_32       )
ASM_TEXT(  cmplwi 7, 21, 0x36     )
ASM_TEXT(  beq    7, l_36       )
ASM_TEXT(  b      restore       )
ASM_TEXT(  l_32:                )
ASM_TEXT(  mfspr 20, 61         ) // SPR_DEAR, 61
ASM_TEXT(  lwz   21, 0(20)      )
ASM_TEXT(  lwz   22, 4(20)      )
//ASM_TEXT(  addi  20,  0,  0     )
//ASM_TEXT(  add   20, 20, %0     )
ASM_TEXT(  addis 20,  0,  u@h   )
ASM_TEXT(  ori   20, 20,  u@l   )
ASM_TEXT(  stw   21, 0(20)      )
ASM_TEXT(  stw   22, 4(20)      )
ASM_TEXT(  lfd    0, 0(20)      )
// SRR0 = SRR0 + 4
ASM_TEXT(  mfspr 21, 26         ) // SPR_SRR0, 26
ASM_TEXT(  addi  21, 21, 4       )
ASM_TEXT(  mtspr 26, 21         ) // SPR_SRR0, 26
ASM_TEXT(  b restore            )
ASM_TEXT(  l_36:                )
//ASM_TEXT(  addi  20,  0,  0     )
//ASM_TEXT(  add   20, 20, %0     )
ASM_TEXT(  addis 20,  0,  u@h   )
ASM_TEXT(  ori   20, 20,  u@l   )
ASM_TEXT(  stfd   0, 0(20)      )
ASM_TEXT(  lwz   21, 0(20)      )
ASM_TEXT(  lwz   22, 4(20)      )
ASM_TEXT(  mfspr 20, 61         ) // SPR_DEAR, 61
ASM_TEXT(  stw   21, 0(20)      )
ASM_TEXT(  stw   22, 4(20)      )
// SRR0 = SRR0 + 4
ASM_TEXT(  mfspr 21, 26         ) // SPR_SRR0, 26
ASM_TEXT(  addi  21, 21, 4       )      
ASM_TEXT(  mtspr 26, 21         ) // SPR_SRR0, 26
ASM_TEXT(  restore:             )
// restore r20, r21, r22
ASM_TEXT(  mfspr  r20, 261      ) // 261
ASM_TEXT(  mfspr  r21, 262      ) // 262
ASM_TEXT(  mfspr  r22, 263      ) // 263
ASM_TEXT(  rfi                  )
);

void ivor5_unaligned_handler2();
/*
__attribute__((aligned(16))) void ivor5_unaligned_handler()
{
// lfd   0, 4(20)
// DEAR - address in memory of unaligned data
// SRR0 - address of an instruction that caused IVOR5 (UNALIGN)
//  double tmp __attribute__((aligned(8)));
  uint32_t cmd;
  uint32_t * cmd_ptr;
  volatile union union_t u;

  u.b[0] = 0; 
  u.b[1] = 0;

  cmd = u.b[0];

  cmd_ptr = (uint32_t * ) spr_read(SPR_SRR0);
  cmd = ((*cmd_ptr) & 0xFC000000) >> 26;
//dst_reg = (*cmd_ptr) & 0x03E00000) >> 21; // is not used in switch !
  switch (cmd) {
   case 0x32:  // lfd 0, xx; UNALIGN
     asm volatile (
       "  mfspr 20, 61        \n\t" // SPR_DEAR, 61
       "  lwz   21, 0(20)     \n\t"
       "  lwz   22, 4(20)     \n\t"
//       "  addis 20,  0,  %0@h  \n\t"
//       "  ori   20, 20,  %0@l  \n\t"
       "  addi  20,  0,  0     \n\t"
       "  add   20, 20, %0     \n\t"
       "  stw   21, 0(20)     \n\t"
       "  stw   22, 4(20)     \n\t"
       "  lfd    0, 0(20)     \n\t"
       "  mfspr 21, 26        \n\t" // SPR_SRR0, 26
       "  addi  21, 0, 4      \n\t"      
       "  mtspr 26, 21        \n\t" // SPR_SRR0, 26
       "  rfi                 \n\t"    
       :
       : "r" (u)
       :"r20","r21","r22"
     );        
     break;      
   case 0x36:  // stfd 0, xx; UNALIGN
     asm volatile (
//       "  addis 20,  0,  %0@h \n\t"
//       "  ori   20, 20,  %0@l \n\t"
       "  addi  20,  0,  0     \n\t"
       "  add   20, 20, %0     \n\t"
       "  stfd   0, 0(20)    \n\t"
       "  lwz   21, 0(20)    \n\t"
       "  lwz   22, 4(20)    \n\t"
       "  mfspr 20, 61       \n\t" // SPR_DEAR, 61
       "  stw   21, 0(20)    \n\t"
       "  stw   22, 4(20)    \n\t"
       "  mfspr 23, 26       \n\t" // SPR_SRR0, 26
       "  addi  23, 0, 4     \n\t"
       "  mtspr 26, 23       \n\t" // SPR_SRR0, 26
       "  rfi                \n\t"
       : 
       : "r" (u)
       :"r20","r21","r22","r23"
     );
     break;
   default: 
     asm volatile (
       "  rfi                \n\t"
     ); 
     break;
  };
};
*/

static void init_handlers() {
    rumboot_putstring( "Init handlers" );
    spr_write( SPR_IVOR5,  (uint32_t)&ivor5_unaligned_handler2 & ITRPT_IVORn_OFFSET_mask );
}


int main () {
// ld8, ld16, ld32, ld64
// st8, st16, st32, st64

  init_handlers();

// enable fpu
  msr_write(msr_read() | (1<<13) ); // 18, 31-18=13 

asm volatile (
"  addis 20, 0,  s@h \n\t"
"  ori   20, 20, s@l \n\t"
"  addis 21, 0,  d@h \n\t"
"  ori   21, 21, d@l \n\t"
"                        \n\t"
"  lwz   10, 0(20)     \n\t"
"  add   10, 10, 10   \n\t"
"  stw   10, 0(21)     \n\t"
"  msync                 \n\t"
"                        \n\t"
"  lfd   0, 4(20)      \n\t"
"  fadd  0, 0, 0       \n\t"
"  stfd  0, 4(21)      \n\t"
"  msync                 \n\t"
"                        \n\t"
"  lwz   10, 12(20)     \n\t"
"  addi  10, 10, 1     \n\t"
"  stw   10, 12(21)     \n\t"
"  msync                 \n\t"
"                        \n\t"
"  lbz   10, 16(20)     \n\t"
"  add   10, 10, 10   \n\t"
"  stb   10, 16(21)     \n\t"
"  lbz   10, 17(20)     \n\t"
"  add   10, 10, 10   \n\t"
"  stb   10, 17(21)     \n\t"
"  lbz   10, 18(20)     \n\t"
"  add   10, 10, 10   \n\t"
"  stb   10, 18(21)     \n\t"
"  lbz   10, 19(20)     \n\t"
"  add   10, 10, 10   \n\t"
"  stb   10, 19(21)     \n\t"
"  lbz   10, 20(20)     \n\t"
"  add   10, 10, 10   \n\t"
"  stb   10, 20(21)     \n\t"
"  lbz   10, 21(20)     \n\t"
"  add   10, 10, 10   \n\t"
"  stb   10, 21(21)     \n\t"
"  lbz   10, 22(20)     \n\t"
"  add   10, 10, 10   \n\t"
"  stb   10, 22(21)     \n\t"
"                        \n\t"
"  lhz   10, 23(20)     \n\t"
"  addi  10, 10, 1     \n\t"
"  sth   10, 23(21)     \n\t"
"  msync                 \n\t"
"                        \n\t"
"  lwz   10, 25(20)     \n\t"
"  add   10, 10, 10   \n\t"
"  stw   10, 25(21)     \n\t"

  :::"r20","r21","fr0","r10", "r0"
);



// 6 version
/*
  d.a = s.a + s.a;
  msync();
  d.b = s.b + s.b;
  msync();
  d.c = s.c + 1;
  msync();
  for(int k=0; k<7; k++) {d.d[k] = s.d[k] + s.d[k]; };
  msync();
  d.e = s.e + s.e;
  msync();
  d.f = s.f + s.f;
*/
//  rumboot_printf( "Source struct a %x b %x c %x e %x f %x\n", s.a, s.b, s.c, s.e, s.f );
//  rumboot_printf( "Source struct d0 %x d1 %x d2 %x d3 %x d4 %x d5 %x d6 %x\n", s.d[0], s.d[1], s.d[2], s.d[3], s.d[4], s.d[5], s.d[6]);

//  rumboot_printf( "Dst    struct a %x b %x c %x e %x f %x\n", d.a, d.b, d.c, d.e, d.f );
//  rumboot_printf( "Dst    struct d0 %x d1 %x d2 %x d3 %x d4 %x d5 %x d6 %x\n", d.d[0], d.d[1], d.d[2], d.d[3], d.d[4], d.d[5], d.d[6]);

  return 0;
}

