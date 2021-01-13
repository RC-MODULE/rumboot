/*
 *  Created on: Aug 6, 2015
 *      Author: a.korablinov
 */

#ifndef PPC470S_MMU_S_H_
#define PPC470S_MMU_S_H_

#include <platform/arch/ppc/ppc_476fp_mmu.S.h>

#define ppc470s_write_tlb_entries    write_tlb_entries

.macro ppc470s_set_mem_window rT, window
    set_mem_window \rT, \window
.endm



#endif //PPC470S_MMU_S_H_
