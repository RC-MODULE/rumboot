
#include <platform/arch/ppc/ppc_476fp_asm.h>

.global _invalidate_tlb_entry
_invalidate_tlb_entry:
	tlbsx. r4, r0, r3      // [r3] - EA, [r4] - Way, Index
	bne    ite_end         // branch if not found
	oris   r4, r4, 0x8000  // r4[0]=1, r4[4]=0
	tlbwe  r3, r4, 0       // [r3] - EA[0:19], V[20], [r4]- Way[1:2], Index[8:15], index is NU
ite_end:
	or     r3, r4, r4      // return [r3]
	
	isync
	msync
	blr
	
	
.global _write_tlb_entry
_write_tlb_entry:
	addis r6, r0, 0x0000  // zero [r6]
	tlbwe r3, r6, 0
	tlbwe r4, r6, 1
	tlbwe r5, r6, 2
        
	isync
	msync
	blr
		
		
.global _read_tlb_entry
_read_tlb_entry:
	tlbsx. r5, r0, r3  // [r3] - EA, [r5] - Way, Index
	bne    rte_nf      // branch if not found
	
	tlbre r3, r5, 0
	stw   r3, 0(r4)
	tlbre r3, r5, 1
	stw   r3, 4(r4)
	tlbre r3, r5, 2
	stw   r3, 8(r4)

	add r3, r0, r5
	blr
	
rte_nf:
	addi r3, r0, 0
	blr

