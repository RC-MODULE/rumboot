#ifndef TEST_EVENT_ASM_H_
#define TEST_EVENT_ASM_H_

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/test_event_codes.h>


/* TODO: remove repeated declaration */
#define EVENT_NOP               0
#define EVENT_TERM              1
#define EVENT_STDOUT            2
#define EVENT_TESTEVENT         17
#define EVENT_RELOCATE_RUNTIME  18


.macro gtube_init tmp_reg=r5
    load_const          \tmp_reg, 0x00000000  // make r0 "all zeros" source
    mtspr               SPR_SPRG0, \tmp_reg
    mtspr               SPR_SPRG1, \tmp_reg
    mtspr               SPR_SPRG2, \tmp_reg
    mtspr               SPR_SPRG3, \tmp_reg
    mtspr               SPR_SPRG4, \tmp_reg
    mtspr               SPR_SPRG5, \tmp_reg
    mtspr               SPR_SPRG6, \tmp_reg
    mtspr               SPR_SPRG7, \tmp_reg
    mtspr               SPR_SPRG8, \tmp_reg
    load_const          \tmp_reg, 0x0003c0de
    mtspr               SPR_SPRG0, \tmp_reg
.endm


.macro disable_ext_irq tmp_reg=r5, reg_old_msr=r6
    mfmsr               \reg_old_msr
    load_const          \tmp_reg, ~( 1 << ITRPT_XSR_EE_i )
    and                 \tmp_reg, \reg_old_msr, \tmp_reg
    mtmsr               \tmp_reg
.endm


.macro rumboot_event_arg1_ event_code, arg0_reg, tmp_reg=r5
    mtspr               SPR_SPRG2, \arg0_reg
    load_const          \tmp_reg, \event_code
    mtspr               SPR_SPRG1, \tmp_reg
    load_const          \tmp_reg, EVENT_NOP
    mtspr               SPR_SPRG1, \tmp_reg
.endm


.macro exit exit_code, tmp_reg=r5, tmp_reg_msr=r6
    disable_ext_irq     \tmp_reg, \tmp_reg_msr

    load_const          \tmp_reg, TEST_EVENT_NEW_TEST_FINISHED
    rumboot_event_arg1_ EVENT_TESTEVENT, \tmp_reg

    load_const          \tmp_reg, \exit_code
    rumboot_event_arg1_ EVENT_TERM, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


.macro gtube_init_runtime tmp_reg=r5, tmp_reg_msr=r6
    disable_ext_irq     \tmp_reg, \tmp_reg_msr

    load_addr           \tmp_reg, rumboot_platform_runtime_info
    rumboot_event_arg1_ EVENT_RELOCATE_RUNTIME, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


.macro test_event test_event_code, tmp_reg=r5, tmp_reg_msr=r6
    disable_ext_irq     \tmp_reg, \tmp_reg_msr

    load_const          \tmp_reg, \test_event_code
    rumboot_event_arg1_ EVENT_TESTEVENT, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


.macro test_event_send_test_id test_id, tmp_reg=r5, tmp_reg_msr=r6
    b 4f

3:  .ascii "\test_id"
    .byte  0x0 /*end of string*/

.align 2, 0
4:  disable_ext_irq     \tmp_reg, \tmp_reg_msr

    load_addr           \tmp_reg, 3b
    mtspr               SPR_SPRG3, \tmp_reg
    load_const          \tmp_reg, TEST_EVENT_NEW_TEST_STARTED
    rumboot_event_arg1_ EVENT_TESTEVENT, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


#endif /* TEST_EVENT_ASM_H_ */
