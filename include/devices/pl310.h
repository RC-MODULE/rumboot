/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 * Aneesh V <aneesh@ti.com>
 */
#ifndef _PL310_H_
#define _PL310_H_
#include <stdint.h>

/* Register bit fields */
#define PL310_AUX_CTRL_ASSOCIATIVITY_MASK	(1 << 16)
#define L2X0_DYNAMIC_CLK_GATING_EN		(1 << 1)
#define L2X0_STNDBY_MODE_EN			(1 << 0)
#define L2X0_CTRL_EN				1

#define L310_SHARED_ATT_OVERRIDE_ENABLE		(1 << 22)
#define L310_AUX_CTRL_DATA_PREFETCH_MASK	(1 << 28)
#define L310_AUX_CTRL_INST_PREFETCH_MASK	(1 << 29)
#define L310_LATENCY_CTRL_SETUP(n)		((n) << 0)
#define L310_LATENCY_CTRL_RD(n)			((n) << 4)
#define L310_LATENCY_CTRL_WR(n)			((n) << 8)

#define L2X0_CACHE_ID_PART_MASK     (0xf << 6)
#define L2X0_CACHE_ID_PART_L310     (3 << 6)
#define L2X0_CACHE_ID_RTL_MASK          0x3f
#define L2X0_CACHE_ID_RTL_R3P2          0x8

struct pl310_regs {
	uint32_t pl310_cache_id;
	uint32_t pl310_cache_type;
	uint32_t pad1[62];
	uint32_t pl310_ctrl;
	uint32_t pl310_aux_ctrl;
	uint32_t pl310_tag_latency_ctrl;
	uint32_t pl310_data_latency_ctrl;
	uint32_t pad2[60];
	uint32_t pl310_event_cnt_ctrl;
	uint32_t pl310_event_cnt1_cfg;
	uint32_t pl310_event_cnt0_cfg;
	uint32_t pl310_event_cnt1_val;
	uint32_t pl310_event_cnt0_val;
	uint32_t pl310_intr_mask;
	uint32_t pl310_masked_intr_stat;
	uint32_t pl310_raw_intr_stat;
	uint32_t pl310_intr_clear;
	uint32_t pad3[323];
	uint32_t pl310_cache_sync;
	uint32_t pad4[15];
	uint32_t pl310_inv_line_pa;
	uint32_t pad5[2];
	uint32_t pl310_inv_way;
	uint32_t pad6[12];
	uint32_t pl310_clean_line_pa;
	uint32_t pad7[1];
	uint32_t pl310_clean_line_idx;
	uint32_t pl310_clean_way;
	uint32_t pad8[12];
	uint32_t pl310_clean_inv_line_pa;
	uint32_t pad9[1];
	uint32_t pl310_clean_inv_line_idx;
	uint32_t pl310_clean_inv_way;
	uint32_t pad10[64];
	uint32_t pl310_lockdown_dbase;
	uint32_t pl310_lockdown_ibase;
	uint32_t pad11[190];
	uint32_t pl310_addr_filter_start;
	uint32_t pl310_addr_filter_end;
	uint32_t pad12[190];
	uint32_t pl310_test_operation;
	uint32_t pad13[3];
	uint32_t pl310_line_data;
	uint32_t pad14[7];
	uint32_t pl310_line_tag;
	uint32_t pad15[3];
	uint32_t pl310_debug_ctrl;
	uint32_t pad16[7];
	uint32_t pl310_prefetch_ctrl;
	uint32_t pad17[7];
	uint32_t pl310_power_ctrl;
};

void pl310_inval_all(uintptr_t base);
void pl310_clean_inval_all(uintptr_t base);
void pl310_inval_range(uintptr_t base, uint32_t start, uint32_t end);
void pl310_clean_inval_range(uintptr_t base, uint32_t start, uint32_t end);

#endif
