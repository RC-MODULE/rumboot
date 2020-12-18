#include <rumboot/irq.h>
#include <arch/ppc_476fp_asm.h>

.section ".text.irq","ax",@progbits

.global rumboot_itrpt_hdr_base
.global rumboot_CI_hdr
.global rumboot_MC_hdr
.global rumboot_DS_hdr
.global rumboot_IS_hdr
.global rumboot_EI_hdr
.global rumboot_A_hdr
.global rumboot_P_hdr
.global rumboot_FPU_hdr
.global rumboot_SC_hdr
.global rumboot_APU_hdr
.global rumboot_DEC_hdr
.global rumboot_FIT_hdr
.global rumboot_WDT_hdr
.global rumboot_DTLBE_hdr
.global rumboot_ITLBE_hdr
.global rumboot_D_hdr

.align 4
rumboot_itrpt_hdr_base:
rumboot_CI_hdr:
    full_prologue   1

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_NORMAL
    load_const  r5, 0
    bl          rumboot_irq_core_dispatch

    full_epilogue   1

    rfci

.align 4
rumboot_MC_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_MACHINE_CHECK
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfmci

.align 4
rumboot_DS_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_DATA_STORAGE
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_IS_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_INST_STORAGE
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_EI_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_NORMAL
    load_const  r5, 0
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_A_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_ALIGNMENT
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_P_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_PROGRAM
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_FPU_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_FP_UNAVAILABLE
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_SC_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_SYSTEM_CALL
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_APU_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_AP_UNAVAILABLE
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_DEC_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_DECREMENTER
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_FIT_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_FIXED_INTERVAL_TIMER
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_WDT_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_WATCHDOG_TIMER
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfci

.align 4
rumboot_DTLBE_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_DATA_TLB_ERROR
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

.align 4
rumboot_ITLBE_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_INST_TLB_ERROR
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfi

 .align 4
rumboot_D_hdr:
    full_prologue

    load_const  r3, 0
    load_const  r4, RUMBOOT_IRQ_TYPE_EXCEPTION
    load_const  r5, RUMBOOT_IRQ_DEBUG
    bl          rumboot_irq_core_dispatch

    full_epilogue

    rfci