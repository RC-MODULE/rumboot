/*
 *  Created on: Jul 23, 2015
 *      Author: a.korablinov
 */

#ifndef PPC470S_MMU_H_
#define PPC470S_MMU_H_

#include <arch/ppc/ppc_476fp_mmu.h>

/*Invalidate all tlb entries, including bolted. All entries are invalidated locally without broadcasting
snoop signals through PLB6 to other CPUs.
You must take care of tlb entires being currently used, i.e. they must be cached in
shadow TLBs (ITLB and DTLB) before invalidating. Otherwise TLB miss exception will occure.
TLB entries are cached in ITLB and DTLB during read operations from any address corresponding
to that TLB entry.*/
void ppc470s_invalidate_all_tlb_entries_nosnoop();

#define ppc470s_set_mem_window(window) set_mem_window(window)

#define ppc470s_get_mem_window(window) get_mem_window()

#define ppc470s_write_tlb_entries( entries, n ) write_tlb_entries( entries, n )

#endif /* PPC470S_MMU_H_ */
