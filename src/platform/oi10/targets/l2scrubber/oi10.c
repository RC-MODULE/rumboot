#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/devices/l2c.h>
#include <stdio.h>
#include <inttypes.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/interrupts.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <math.h>
#include <arch/l2scrub.h>
#include "../src/platform/oi10/targets/l2scrubber/glue.h"
#include <platform/devices/emi.h>


/* ********************************* */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>

typedef enum 
{
	TLBSID_4K   = 0x00,
	TLBSID_16K  = 0x01,
	TLBSID_64K  = 0x03,
	TLBSID_1M   = 0x07,
	TLBSID_16M  = 0x0F,
	TLBSID_256M = 0x1F,
	TLBSID_1G   = 0x3F,
	TLBSID_ERR  = 0xFF
} tlb_size_id;

typedef enum
{
  TLB_MODE_NONE = 0x0,
  TLB_MODE_R  = 0x1,
  TLB_MODE_W  = 0x2,
  TLB_MODE_RW = 0x3,
  TLB_MODE_X  = 0x4,
  TLB_MODE_RX = 0x5,
  TLB_MODE_WX  = 0x6,
  TLB_MODE_RWX = 0x7
} tlb_rwx_mode;

typedef struct 
{
  uint32_t epn   : 20;  // [0:19]
  uint32_t v     : 1;   // [20]
  uint32_t ts    : 1;   // [21]
  uint32_t dsiz  : 6;   // [22:27]  set 0x0, 0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F (4K, 16K, 64K, 1M, 16M, 256M, 1GB) 
  uint32_t blank : 4;   // [28:31]
} tlb_entr_0;

typedef union
{
	tlb_entr_0 entr;
	uint32_t   data;
} tlb_entr_data_0;

typedef struct 
{
  uint32_t rpn   : 20;  // [0:19]
  uint32_t blank : 2;   // [20:21]
  uint32_t erpn  : 10;  // [22:31]
} tlb_entr_1;

typedef union
{
	tlb_entr_1 entr;
	uint32_t   data;
} tlb_entr_data_1;

typedef struct 
{
  uint32_t blank1 : 14;  // [0:13]
  uint32_t il1i   : 1;   // [14]
  uint32_t il1d   : 1;   // [15]
  uint32_t u      : 4;   // [16:19]
  uint32_t wimg   : 4;   // [20:23]
  uint32_t e      : 1;   // [24]  0-BE, 1-LE
  uint32_t blank2 : 1;   // [25]
  uint32_t uxwr   : 3;   // [26:28]
  uint32_t sxwr   : 3;   // [29:31]
} tlb_entr_2;

typedef union
{
	tlb_entr_2 entr;
	uint32_t   data;
} tlb_entr_data_2;




uint32_t _invalidate_tlb_entry(uint32_t tlb);
void _write_tlb_entry(uint32_t tlb0, uint32_t tlb1, uint32_t tlb2, uint32_t zero);
int _read_tlb_entry(uint32_t ea, uint32_t * tlb, uint32_t dummy);


void tlb47x_inval(uint32_t cpu_adr, tlb_size_id tlb_sid)
{
	tlb_entr_data_0 tlb_0;
	tlb_0.data = 0x00000000;
	tlb_0.entr.epn = (cpu_adr >> 12);
	tlb_0.entr.v = 0;
	tlb_0.entr.ts = 0;
	tlb_0.entr.dsiz = tlb_sid;
		
	_invalidate_tlb_entry(tlb_0.data);
}


typedef struct 
{
	tlb_size_id sid;
	const char * name;
} tlb_sid_name;

static const tlb_sid_name tlb_sid_names[] = 
{
	{ TLBSID_4K,   "4k"   },
	{ TLBSID_16K,  "16k"  },
	{ TLBSID_64K,  "64k"  },
	{ TLBSID_1M,   "1m"   },
	{ TLBSID_16M,  "16m"  },
	{ TLBSID_256M, "256m" },
	{ TLBSID_1G,   "1g"   },
	{ TLBSID_ERR,  ""     }
};	

static const char * get_tlb_sid_name(tlb_size_id tlb_sid)
{
	uint i;
	for ( i = 0; tlb_sid_names[i].sid != TLBSID_ERR; ++ i ) 
		if ( tlb_sid_names[i].sid == tlb_sid )
			break;
	return tlb_sid_names[i].name;
}

////////////////////////////////////////////////////////////////

/* Current:
  epn  v ts  sz   _  rpn  _ erpn  __  ili ild u wimg  e  _ uxwr sxwr
 00000 1  0   1m  f 00000 2  010 2000  1   1  0   4  BE  0   0    7
 10000 1  0 256m  b 10000 0  010 0000  1   1  0   5  LE  0   0    3
 20000 1  0 256m  b 20000 0  010 0000  1   1  0   5  LE  0   0    3
 30000 1  0 256m  f 30000 2  010 0000  1   1  0   5  LE  0   0    3
 40000 1  0 256m  3 00000 3  000 0000  1   1  0   4  BE  0   7    7
 fff00 1  0   1m  f fff00 3  3ff 2000  1   1  0   5  BE  0   0    5
*/

static void print_tlb_entr_0(tlb_entr_0 * tlb, bool header)
{
	if ( ! header ) rumboot_printf(" %05x %d  %d %4s  %01x", tlb->epn, tlb->v, tlb->ts, get_tlb_sid_name((tlb_size_id) tlb->dsiz), tlb->blank);
	else            rumboot_printf("  epn  v ts  sz   _");
}

static void print_tlb_entr_1(tlb_entr_1 * tlb, bool header)
{
	if ( ! header ) rumboot_printf(" %05x %01x  %03x", tlb->rpn, tlb->blank, tlb->erpn);
	else            rumboot_printf("  rpn  _ erpn"); 
}

static void print_tlb_entr_2(tlb_entr_2 * tlb, bool header)
{
	if ( ! header ) 
		rumboot_printf(" %04x  %d   %d  %01x   %01x  %s  %d   %01x    %01x", 
		       tlb->blank1, tlb->il1i, tlb->il1d, tlb->u, tlb->wimg, (tlb->e ? "LE" : "BE"), tlb->blank2, tlb->uxwr, tlb->sxwr);
	else
		rumboot_printf("  __  ili ild u wimg  e  _ uxwr sxwr");
}

typedef struct 
{
	uint32_t tlb_0;
	uint32_t tlb_1;
	uint32_t tlb_2;
} tlb_entr;

static tlb_entr s_tlb_arr[1024];
static int s_tlb_cnt = -1;

#define countof(arr)  (sizeof(arr) / sizeof((arr)[0]))

static int mem_map_tlbs(void) 
{
	print_tlb_entr_0(NULL, true);
	print_tlb_entr_1(NULL, true);
	print_tlb_entr_2(NULL, true);
	rumboot_printf("\n");
	
	s_tlb_cnt = -1;
	uint32_t ea;
	const uint32_t dlt = 0x1 << 12;
	for ( ea = 0x00000000; ea + dlt > ea; ea += dlt ) {
		uint32_t tlb[3] = { 0, 0, 0 };	
		uint32_t res = _read_tlb_entry(ea, tlb, 0);
		if ( res != 0 && (s_tlb_cnt == -1 || s_tlb_arr[s_tlb_cnt].tlb_0 != tlb[0]) ) {
			if ( ++ s_tlb_cnt == countof(s_tlb_arr) )
				return -1;
			s_tlb_arr[s_tlb_cnt].tlb_0 = tlb[0];
			s_tlb_arr[s_tlb_cnt].tlb_1 = tlb[1];
			s_tlb_arr[s_tlb_cnt].tlb_2 = tlb[2];
			print_tlb_entr_0((tlb_entr_0 *) & tlb[0], false);
			print_tlb_entr_1((tlb_entr_1 *) & tlb[1], false);
			print_tlb_entr_2((tlb_entr_2 *) & tlb[2], false);
			rumboot_printf("\n");
		}			
	}
	return 0;
}


/* ******************************** */

/* This file is the OI10 glue for scrubber framework */

static const tlb_entry sram0_tlb_entry[] = {
//        MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        { MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 ) },
        { MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1MB,     0b1,    0b1,    0b0,    0b0,    0b1,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 ) }
};


static emi_bank_cfg bank_cfg =
{
    //SS0
    {
        BTYP_SRAM,
        PTYP_NO_PAGES,
        SRDY_EXT_RDY_NOT_USE,
        TWR_0,
        SST_Flow_Through,
        TSSOE_1,
        TSOE_1,
        TCYC_3,
        0, //T_RDY
        TDEL_1
    },
    //SD0
    {
        CSP_256,
        SDS_2M,
        CL_3,
        TRDL_1,
        SI_EXT_INIT,
        TRCD_5,
        TRAS_9
    }
};


struct l2_scrubber *l2scrubber_tests_init(int mode) 
{
    dcr_write(0x80000600, 0xC10);
    dcr_write(0x80000604, 0x1);
    rumboot_platform_runtime_info->silent = 0;
    emi_init_ext_cfg(DCR_EM2_EMI_BASE, &bank_cfg);
    write_tlb_entries(sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry));
    struct l2c_mem_layout layout;
    l2c_get_mem_layout(DCR_L2C_BASE, &layout);
    rumboot_printf("l2c: size %d bytes, data %d elements\n", 
        layout.l2size_bytes, layout.data_array_size);

    memset(l2scrubber_tests_get_cachable_area(), 0xa5, layout.l2size_bytes);
//    rumboot_memfill32(l2scrubber_tests_get_cachable_area(), layout.l2size_bytes, 0, 1);
    asm("msync");
    asm("isync"); 
    for (int j=0; j < layout.l2size_bytes; j = j + 8) {
        ioread64(j);
    }
    rumboot_printf("l2c: Init complete\n");
//    mem_map_tlbs();
    rumboot_irq_sei();

    struct l2_scrubber *scr = l2_scrubber_create(DCR_L2C_BASE, (mode == 0) ? L2C0_INTERRUPTOUT : mode);
    return scr;
}

void *l2scrubber_tests_get_cachable_area()
{
    return (void *) SRAM0_BASE;
}
