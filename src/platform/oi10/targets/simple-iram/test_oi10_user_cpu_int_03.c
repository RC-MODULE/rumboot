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
  char a; /* 0x1112_1314 word */
  double b; /* 0x2122_2324_2526_2728 doubleword */ // 0x3FB9_9999_9999_999A
  char * c; /* 0x3132_3334 word */
  char d[7]; /* 'A', 'B', 'C', 'D', 'E', 'F', 'G' array of bytes */
  short e; /* 0x5152 halfword */
  int f; /* 0x6162_6364 word */
}
#ifdef TEST_PACKED
__attribute__((packed, aligned(8)))
#endif
;

struct endian_t s = { 0x11, 0.1, (char *) 0x31323334,  {'A', 'B', 'C', 'D', 'E', 'F', 'G'}, 0x5152, 0x61626364 };
struct endian_t d;

int main () {
// 1 version
// ld8, ld16, ld32, ld64
// st8, st16, st32, st64
/*
  d = s;
  d.a = d.a + 1;
  d.b = d.b + 1.0;
  d.c = d.c + 1;
  for(int k=0; k<7; k++) {d.d[k] = d.d[k] + 1; };
  d.e = d.e + 1;
  d.f = d.f + 1;
*/
// 2 version
// there is not st64!
/*
  d = s;
  if ((d.a != s.a) && (d.b != s.b) && (d.c != s.c) && (d.e != s.e) && (d.f != s.f)) return 1;
*/

// 3 version
/*
  d.a = s.a + 1;
  d.b = s.b + 1.0;
  d.c = s.c + 1;
  for(int k=0; k<7; k++) {d.d[k] = s.d[k] + 1; };
  d.e = s.e + 1;
  d.f = s.f + 1;
*/
// 4 version
//  d = s;
//  msync();
/*
  d.a = s.a + 1;
  msync();
  d.b = s.b + 1.0;
  msync();
  d.c = s.c + 1;
  msync();
  for(int k=0; k<7; k++) {d.d[k] = s.d[k] + 1; };
  msync();
  d.e = s.e + 1;
  msync();
  d.f = s.f + 1;
*/

// 5 version
/*
  d.a = s.a;
  msync();
  d.b = s.b;
  msync();
  d.c = s.c;
  msync();
  for(int k=0; k<7; k++) {d.d[k] = s.d[k]; };
  msync();
  d.e = s.e;
  msync();
  d.f = s.f;
*/
// 6 version
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

//  rumboot_printf( "Source struct a %x b %x c %x e %x f %x\n", s.a, s.b, s.c, s.e, s.f );
//  rumboot_printf( "Source struct d0 %x d1 %x d2 %x d3 %x d4 %x d5 %x d6 %x\n", s.d[0], s.d[1], s.d[2], s.d[3], s.d[4], s.d[5], s.d[6]);

//  rumboot_printf( "Dst    struct a %x b %x c %x e %x f %x\n", d.a, d.b, d.c, d.e, d.f );
//  rumboot_printf( "Dst    struct d0 %x d1 %x d2 %x d3 %x d4 %x d5 %x d6 %x\n", d.d[0], d.d[1], d.d[2], d.d[3], d.d[4], d.d[5], d.d[6]);

  return 0;
}

