#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/ltrace.h>
#include <platform/test_event_c.h>
#include <platform/test_assert.h>



#define EVENT_LTRACE0_CHECK_DATA    0x0000000099
#define IM0_ADDR_CELL               0x0004FFFC

#define LTRACE_RAW_ADDRESS(LTRACE_ADDRESS) (((uint32_t)((LTRACE_ADDRESS).address & 0x0FFF)) | ((uint32_t)(((LTRACE_ADDRESS).range & 0x0003) << 30)))

const uint32_t LOOP_COUNTER = 1;

void loop(uint32_t count)
{
    uint32_t i = count;
    while(i--)
    {
        nop();
        if (i%3 == 0) goto l1;
        nop();
l1:     nop();
    }
}


void call_IAC() {}

void IAC(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //rumboot_printf("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //rumboot_printf("mtspr SPR_IAC1, 0x00\n");
    spr_write(SPR_IAC1, (uint32_t)&call_IAC);

    //rumboot_printf("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //rumboot_printf("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("mtspr SPR_DBCR0, 0x00800000\n");
    spr_write(SPR_DBCR0, 0x80800000);

    //SPR_DBCR0_write(0x0FFF8000);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("call_IAC\n");
    call_IAC();
}

void IAC1(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //rumboot_printf("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //rumboot_printf("mtspr SPR_IAC1, 0x00\n");
    spr_write(SPR_IAC1, (uint32_t)&call_IAC);

    //rumboot_printf("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //rumboot_printf("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("mtspr SPR_DBCR0, 0x00800000\n");
    spr_write(SPR_DBCR0, 0x00800000);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("call_IAC\n");
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x00800000);

    spr_write(SPR_DBSR_RC, 0x00800000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_IAC again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x00800000);

        spr_write(SPR_DBSR_RC, 0x00800000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void IAC2(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //rumboot_printf("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //rumboot_printf("mtspr SPR_IAC2, 0x00\n");
    spr_write(SPR_IAC2, (uint32_t)&call_IAC);

    //rumboot_printf("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //rumboot_printf("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("mtspr SPR_DBCR0, 0x00400000\n");
    spr_write(SPR_DBCR0, 0x00400000);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("call_IAC\n");
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x00400000);

    spr_write(SPR_DBSR_RC, 0x00400000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_IAC again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x00400000);

        spr_write(SPR_DBSR_RC, 0x00400000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void IAC3(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //rumboot_printf("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //rumboot_printf("mtspr SPR_IAC3, 0x00\n");
    spr_write(SPR_IAC3, (uint32_t)&call_IAC);

    //rumboot_printf("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //rumboot_printf("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("mtspr SPR_DBCR0, 0x00200000\n");
    spr_write(SPR_DBCR0, 0x00200000);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("call_IAC\n");
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x00200000);

    spr_write(SPR_DBSR_RC, 0x00200000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_IAC again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x00200000);

        spr_write(SPR_DBSR_RC, 0x00200000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void IAC4(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //rumboot_printf("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //rumboot_printf("mtspr SPR_IAC4, 0x00\n");
    spr_write(SPR_IAC4, (uint32_t)&call_IAC);

    //rumboot_printf("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //rumboot_printf("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("mtspr SPR_DBCR0, 0x00100000\n");
    spr_write(SPR_DBCR0, 0x00100000);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("call_IAC\n");
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x00100000);

    spr_write(SPR_DBSR_RC, 0x00100000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_IAC again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x00100000);

        spr_write(SPR_DBSR_RC, 0x00100000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void DAC1R(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //rumboot_printf("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //rumboot_printf("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //rumboot_printf("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("mtspr SPR_DBCR0, 0x00080000\n");
    spr_write(SPR_DBCR0, 0x00080000);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("call_DAC1R\n");
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x00080000);

    spr_write(SPR_DBSR_RC, 0x00080000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_DAC1R again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x00080000);

        spr_write(SPR_DBSR_RC, 0x00080000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void DAC1W(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //rumboot_printf("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //rumboot_printf("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //rumboot_printf("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("mtspr SPR_DBCR0, 0x00040000\n");
    spr_write(SPR_DBCR0, 0x00040000);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("call_DAC1W\n");
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x00040000);

    spr_write(SPR_DBSR_RC, 0x00040000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_DAC1W again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x00040000);

        spr_write(SPR_DBSR_RC, 0x00040000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void DAC2R(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //rumboot_printf("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //rumboot_printf("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //rumboot_printf("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("mtspr SPR_DBCR0, 0x00020000\n");
    spr_write(SPR_DBCR0, 0x00020000);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("call_DAC2R\n");
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x00020000);

    spr_write(SPR_DBSR_RC, 0x00020000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_DAC2R again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x00020000);

        spr_write(SPR_DBSR_RC, 0x00020000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void DAC2W(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //rumboot_printf("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //rumboot_printf("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //rumboot_printf("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("mtspr SPR_DBCR0, 0x00010000\n");
    spr_write(SPR_DBCR0, 0x00010000);

    //rumboot_printf("isync\n");
    isync();

    //rumboot_printf("call_DAC2W\n");
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x00010000);

    spr_write(SPR_DBSR_RC, 0x00010000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_DAC2W again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x00010000);

        spr_write(SPR_DBSR_RC, 0x00010000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Trap(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x01000000\n");
    spr_write(SPR_DBCR0, 0x01000000);

    //rumboot_printf("call_TRAP\n");
    //call_IAC();
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x01000000);

    spr_write(SPR_DBSR_RC, 0x01000000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_TRAP again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x01000000);

        spr_write(SPR_DBSR_RC, 0x01000000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Branch(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x04000000\n");
    spr_write(SPR_DBCR0, 0x04000000);

    //rumboot_printf("call_BRANCH\n");
    //call_IAC();
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x04000000);

    spr_write(SPR_DBSR_RC, 0x04000000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_BRANCH again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x04000000);

        spr_write(SPR_DBSR_RC, 0x04000000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void InstructionComplete(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x08000000\n");
    spr_write(SPR_DBCR0, 0x08000000);

    //rumboot_printf("call_InstructionComplete\n");
    //call_IAC();
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x08000000);

    spr_write(SPR_DBSR_RC, 0x08000000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_InstructionComplete again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Interrupt(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x02000000\n");
    spr_write(SPR_DBCR0, 0x02000000);

    //rumboot_printf("call_Interrupt\n");
    //call_IAC();
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x02000000);

    spr_write(SPR_DBSR_RC, 0x02000000);


    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_Interrupt again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x02000000);

        spr_write(SPR_DBSR_RC, 0x02000000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Return(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x00000800\n");
    spr_write(SPR_DBCR0, 0x00008000);

    //rumboot_printf("call_Return\n");
    //call_IAC();
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x00008000);

    spr_write(SPR_DBSR_RC, 0x00008000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_Return again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x00008000);

        spr_write(SPR_DBSR_RC, 0x00008000);

        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Unconditional(uint8_t two_state, uint8_t bank_on_trigger)
{
    //rumboot_printf("mtspr SPR_DBCR0, 0x40000000\n");
    spr_write(SPR_DBCR0, 0x40000000);

    //rumboot_printf("call_Unconditional\n");
    //call_IAC();
    loop(LOOP_COUNTER);
    spr_write(SPR_DBSR_W, 0x40000000);

    spr_write(SPR_DBSR_RC, 0x40000000);

    if (two_state || bank_on_trigger)
    {
        //rumboot_printf("call_Unconditional again\n");
        loop(LOOP_COUNTER);
        spr_write(SPR_DBSR_W, 0x40000000);

        spr_write(SPR_DBSR_RC, 0x40000000);


        //rumboot_printf("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}


static int test_ltrace(LTRACE_CONDITION_TYPE condition_type,
                        LTRACE_TRIGGER_MODE trigger_mode,
                        LTRACE_COMPRESSION_MODE compression_mode,
                        uint32_t cond1_or,
                        uint32_t cond2_or,
                        uint32_t cond3_or,
                        LTRACE_CONDITION_INVERT cond1_invert,
                        LTRACE_CONDITION_INVERT cond2_invert,
                        LTRACE_CONDITION_INVERT cond3_invert,
                        uint32_t complete_stop_mask,
                        uint32_t complete_enable,
                        uint32_t trace_window_timeout,
                        uint32_t condcomp1_polarity_h,
                        uint32_t condcomp1_polarity_l,
                        uint32_t condcomp1_mask_h,
                        uint32_t condcomp1_mask_l,
                        uint32_t condcomp2_polarity_h,
                        uint32_t condcomp2_polarity_l,
                        uint32_t condcomp2_mask_h,
                        uint32_t condcomp2_mask_l,
                        uint32_t condcomp3_polarity_h,
                        uint32_t condcomp3_polarity_l,
                        uint32_t condcomp3_mask_h,
                        uint32_t condcomp3_mask_l,
                        uint32_t compression_mask_h,
                        uint32_t compression_mask_l,
                        void (*fn)(uint8_t, uint8_t))
{
    LTRACE_ADDRESS trace_address = {0};

    //rumboot_printf("LTRACE disable \n");
    ltrace_disable();

    //rumboot_printf("LTRACE clear \n");
    ltrace_clear_status();

    //rumboot_printf("LTRACE init \n");
    ltrace_init(condition_type,
                 trigger_mode,
                 compression_mode,
                 cond1_or,
                 cond2_or,
                 cond3_or,
                 0,
                 cond1_invert,
                 cond2_invert,
                 cond3_invert,
                 complete_stop_mask,
                 complete_enable);

    //rumboot_printf("LTRACE set trace window \n");
    ltrace_set_trace_window(trace_window_timeout);

    //rumboot_printf("LTRACE set condcomp1 \n");
    ltrace_set_condcomp1_polarity_h(condcomp1_polarity_h);
    ltrace_set_condcomp1_polarity_l(condcomp1_polarity_l);
    ltrace_set_condcomp1_mask_h(condcomp1_mask_h);
    ltrace_set_condcomp1_mask_l(condcomp1_mask_l);

    //rumboot_printf("LTRACE set condcomp2 \n");
    ltrace_set_condcomp2_polarity_h(condcomp2_polarity_h);
    ltrace_set_condcomp2_polarity_l(condcomp2_polarity_l);
    ltrace_set_condcomp2_mask_h(condcomp2_mask_h);
    ltrace_set_condcomp2_mask_l(condcomp2_mask_l);

    //rumboot_printf("LTRACE set condcomp3 \n");
    ltrace_set_condcomp3_polarity_h(condcomp3_polarity_h);
    ltrace_set_condcomp3_polarity_l(condcomp3_polarity_l);
    ltrace_set_condcomp3_mask_h(condcomp3_mask_h);
    ltrace_set_condcomp3_mask_l(condcomp3_mask_l);

    //rumboot_printf("LTRACE set compression mask \n");
    ltrace_set_compression_mask_h(compression_mask_h);
    ltrace_set_compression_mask_l(compression_mask_l);


    test_event(EVENT_LTRACE0_START);

    //rumboot_printf("LTRACE enable \n");
    ltrace_enable();

    //rumboot_printf("Fn \n");
    fn((condition_type == LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE) ||
       (condition_type == LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_ENABLE),
       (trigger_mode == LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER));

/*
    while (!(ltrace_get_status() & LTRACE_STATUS_COMPLETE))
        ;
 */

    //rumboot_printf("LTRACE get status (running) \n");
    TEST_ASSERT((ltrace_get_status() & LTRACE_STATUS_RUNNING) == 0x00, "Tracing is still running!");

    //rumboot_printf("LTRACE get status (complete) \n");
    TEST_ASSERT((ltrace_get_status() & LTRACE_STATUS_COMPLETE) == LTRACE_STATUS_COMPLETE, "Tracing does not terminated!");

    //rumboot_printf("LTRACE get status (address 1 wrapped) \n");
    TEST_ASSERT((ltrace_get_status() & LTRACE_STATUS_ADDRESS1_WRAPPED) == 0x00, "Trace address 1 wrapped!");

    if (trigger_mode == LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER)
    {
        //rumboot_printf("LTRACE get status (address 2 wrapped) \n");
        TEST_ASSERT((ltrace_get_status() & LTRACE_STATUS_ADDRESS2_WRAPPED) == 0x00, "Trace address 2 wrapped!");
    }

    test_event(EVENT_CHECK_LTRACE0_INT);

    //rumboot_printf("LTRACE disable \n");
    ltrace_disable();
    //test_event(EVENT_CHECK_LTRACE0_STOP);

    //rumboot_printf("LTRACE get last address 1 \n");
    uint32_t last_address = ltrace_get_last_address1();
    uint32_t last_address2 = ltrace_get_last_address2();

    last_address = (last_address != 0xFF)?last_address:1;
    last_address2 = ((last_address2 != 0xFF) && (last_address2 != 0x7F))?last_address2:1;

    uint8_t last_buffer_used = (ltrace_get_status() & (1 << IBM_BIT_INDEX(31, 4)));

//    rumboot_printf("Last address1 is ");
//    trace_hex(last_address);
//
//    rumboot_printf("Last address2 is ");
//    trace_hex(last_address2);

    last_address = (last_buffer_used == 0x1)?last_address2:last_address;

    uint32_t data[16384];

    uint32_t i = 0;

    do
    {
//            rumboot_printf("Address is (write)- ");
//            trace_hex(address);
//            rumboot_printf("Set address write\n");
//            itrace0_set_address(address);
//
//            data[0].value0.trace_branch_status = 0xFF;
//            data[0].value0.trace_execution_status = 0xFF;
//            data[0].value0.trace_status = 0xFF;
//            data[0].value0.stop_bit = 0xFF;
//            data[0].value1.trace_branch_status = 0xFF;
//            data[0].value1.trace_execution_status = 0xFF;
//            data[0].value1.trace_status = 0xFF;
//            data[0].value1.stop_bit = 0xFF;
//
//            itrace0_set_data(data[0]);

        if (LTRACE_RAW_ADDRESS(trace_address) == (last_address + 1))
            break;

        //rumboot_printf("LTRACE set address \n");
        ltrace_set_address(trace_address);

        //rumboot_printf("LTRACE get data \n");
        data[i] = ltrace_get_data();

        trace_address.range = 0x01;

        if (LTRACE_RAW_ADDRESS(trace_address) == (last_address + 1))
            break;

        //rumboot_printf("LTRACE set address \n");
        ltrace_set_address(trace_address);

        //rumboot_printf("LTRACE get data \n");
        data[i+1] = ltrace_get_data();

//        trace_hex(i);
//        trace_hex(data[i]);
//        trace_hex(data[i+1]);

        trace_address.range = 0x00;
        trace_address.address++;
        i += 2;

    } while (1);


    //TODO: How to verify trace data?
    //TEST_ASSERT(data.value0.stop_bit == 0x1, "Stop bit not equal to 0x1");


    iowrite32((uint32_t)&data, IM0_ADDR_CELL);
    isync();

//    rumboot_printf("data address =");
//    trace_hex(MEM32(IM0_ADDR_CELL));
//
//    trace_dump((uint32_t)&data, 1024);


    test_event(EVENT_LTRACE0_CHECK_DATA);


    return 0;
}

/*
static int test_ltrace0_stop_on_trigger_int_off(uint32_t cond1_or,
                                                  uint32_t cond2_or,
                                                  uint32_t cond3_or,
                                                  LTRACE_CONDITION_INVERT cond1_invert,
                                                  LTRACE_CONDITION_INVERT cond2_invert,
                                                  LTRACE_CONDITION_INVERT cond3_invert,
                                                  uint32_t condcomp1_polarity_h,
                                                  uint32_t condcomp1_polarity_l,
                                                  uint32_t condcomp1_mask_h,
                                                  uint32_t condcomp1_mask_l,
                                                  uint32_t condcomp2_polarity_h,
                                                  uint32_t condcomp2_polarity_l,
                                                  uint32_t condcomp2_mask_h,
                                                  uint32_t condcomp2_mask_l,
                                                  uint32_t condcomp3_polarity_h,
                                                  uint32_t condcomp3_polarity_l,
                                                  uint32_t condcomp3_mask_h,
                                                  uint32_t condcomp3_mask_l,
                                                  uint32_t compression_mask_h,
                                                  uint32_t compression_mask_l)
{
    test_ltrace0(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                0,
                0,
                0,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l);

    return TEST_OK;
}

static int test_ltrace0_stop_on_trigger_int_on(uint32_t cond1_or,
                                                 uint32_t cond2_or,
                                                 uint32_t cond3_or,
                                                 LTRACE_CONDITION_INVERT cond1_invert,
                                                 LTRACE_CONDITION_INVERT cond2_invert,
                                                 LTRACE_CONDITION_INVERT cond3_invert,
                                                 uint32_t condcomp1_polarity_h,
                                                 uint32_t condcomp1_polarity_l,
                                                 uint32_t condcomp1_mask_h,
                                                 uint32_t condcomp1_mask_l,
                                                 uint32_t condcomp2_polarity_h,
                                                 uint32_t condcomp2_polarity_l,
                                                 uint32_t condcomp2_mask_h,
                                                 uint32_t condcomp2_mask_l,
                                                 uint32_t condcomp3_polarity_h,
                                                 uint32_t condcomp3_polarity_l,
                                                 uint32_t condcomp3_mask_h,
                                                 uint32_t condcomp3_mask_l,
                                                 uint32_t compression_mask_h,
                                                 uint32_t compression_mask_l)
{
    test_ltrace0(LTRACE_CONDITION_TYPE_COND1,
                    LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                    LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                    cond1_or,
                    cond2_or,
                    cond3_or,
                    cond1_invert,
                    cond2_invert,
                    cond3_invert,
                    0,
                    0,
                    0,
                    condcomp1_polarity_h,
                    condcomp1_polarity_l,
                    condcomp1_mask_h,
                    condcomp1_mask_l,
                    condcomp2_polarity_h,
                    condcomp2_polarity_l,
                    condcomp2_mask_h,
                    condcomp2_mask_l,
                    condcomp3_polarity_h,
                    condcomp3_polarity_l,
                    condcomp3_mask_h,
                    condcomp3_mask_l,
                    compression_mask_h,
                    compression_mask_l);

    return TEST_OK;
}

static int test_ltrace0_bank_on_trigger_int_off(uint32_t cond1_or,
                                                  uint32_t cond2_or,
                                                  uint32_t cond3_or,
                                                  LTRACE_CONDITION_INVERT cond1_invert,
                                                  LTRACE_CONDITION_INVERT cond2_invert,
                                                  LTRACE_CONDITION_INVERT cond3_invert,
                                                  uint32_t condcomp1_polarity_h,
                                                  uint32_t condcomp1_polarity_l,
                                                  uint32_t condcomp1_mask_h,
                                                  uint32_t condcomp1_mask_l,
                                                  uint32_t condcomp2_polarity_h,
                                                  uint32_t condcomp2_polarity_l,
                                                  uint32_t condcomp2_mask_h,
                                                  uint32_t condcomp2_mask_l,
                                                  uint32_t condcomp3_polarity_h,
                                                  uint32_t condcomp3_polarity_l,
                                                  uint32_t condcomp3_mask_h,
                                                  uint32_t condcomp3_mask_l,
                                                  uint32_t compression_mask_h,
                                                  uint32_t compression_mask_l)
{
    test_ltrace0(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                        LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                        LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                        cond1_or,
                        cond2_or,
                        cond3_or,
                        cond1_invert,
                        cond2_invert,
                        cond3_invert,
                        0,
                        0,
                        0,
                        condcomp1_polarity_h,
                        condcomp1_polarity_l,
                        condcomp1_mask_h,
                        condcomp1_mask_l,
                        condcomp2_polarity_h,
                        condcomp2_polarity_l,
                        condcomp2_mask_h,
                        condcomp2_mask_l,
                        condcomp3_polarity_h,
                        condcomp3_polarity_l,
                        condcomp3_mask_h,
                        condcomp3_mask_l,
                        compression_mask_h,
                        compression_mask_l);

    return TEST_OK;
}

static int test_ltrace0_bank_on_trigger_int_on(uint32_t cond1_or,
                                                 uint32_t cond2_or,
                                                 uint32_t cond3_or,
                                                 LTRACE_CONDITION_INVERT cond1_invert,
                                                 LTRACE_CONDITION_INVERT cond2_invert,
                                                 LTRACE_CONDITION_INVERT cond3_invert,
                                                 uint32_t condcomp1_polarity_h,
                                                 uint32_t condcomp1_polarity_l,
                                                 uint32_t condcomp1_mask_h,
                                                 uint32_t condcomp1_mask_l,
                                                 uint32_t condcomp2_polarity_h,
                                                 uint32_t condcomp2_polarity_l,
                                                 uint32_t condcomp2_mask_h,
                                                 uint32_t condcomp2_mask_l,
                                                 uint32_t condcomp3_polarity_h,
                                                 uint32_t condcomp3_polarity_l,
                                                 uint32_t condcomp3_mask_h,
                                                 uint32_t condcomp3_mask_l,
                                                 uint32_t compression_mask_h,
                                                 uint32_t compression_mask_l)
{
    test_ltrace0(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                        LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                        LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                        cond1_or,
                        cond2_or,
                        cond3_or,
                        cond1_invert,
                        cond2_invert,
                        cond3_invert,
                        0,
                        0,
                        0,
                        condcomp1_polarity_h,
                        condcomp1_polarity_l,
                        condcomp1_mask_h,
                        condcomp1_mask_l,
                        condcomp2_polarity_h,
                        condcomp2_polarity_l,
                        condcomp2_mask_h,
                        condcomp2_mask_l,
                        condcomp3_polarity_h,
                        condcomp3_polarity_l,
                        condcomp3_mask_h,
                        condcomp3_mask_l,
                        compression_mask_h,
                        compression_mask_l);

    return TEST_OK;
}



static int test_ltrace1_stop_on_trigger_int_off(uint32_t cond1_or,
                                                  uint32_t cond2_or,
                                                  uint32_t cond3_or,
                                                  LTRACE_CONDITION_INVERT cond1_invert,
                                                  LTRACE_CONDITION_INVERT cond2_invert,
                                                  LTRACE_CONDITION_INVERT cond3_invert,
                                                  uint32_t condcomp1_polarity_h,
                                                  uint32_t condcomp1_polarity_l,
                                                  uint32_t condcomp1_mask_h,
                                                  uint32_t condcomp1_mask_l,
                                                  uint32_t condcomp2_polarity_h,
                                                  uint32_t condcomp2_polarity_l,
                                                  uint32_t condcomp2_mask_h,
                                                  uint32_t condcomp2_mask_l,
                                                  uint32_t condcomp3_polarity_h,
                                                  uint32_t condcomp3_polarity_l,
                                                  uint32_t condcomp3_mask_h,
                                                  uint32_t condcomp3_mask_l,
                                                  uint32_t compression_mask_h,
                                                  uint32_t compression_mask_l)
{
    test_ltrace1(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                        LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                        LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                        cond1_or,
                        cond2_or,
                        cond3_or,
                        cond1_invert,
                        cond2_invert,
                        cond3_invert,
                        0,
                        0,
                        0,
                        condcomp1_polarity_h,
                        condcomp1_polarity_l,
                        condcomp1_mask_h,
                        condcomp1_mask_l,
                        condcomp2_polarity_h,
                        condcomp2_polarity_l,
                        condcomp2_mask_h,
                        condcomp2_mask_l,
                        condcomp3_polarity_h,
                        condcomp3_polarity_l,
                        condcomp3_mask_h,
                        condcomp3_mask_l,
                        compression_mask_h,
                        compression_mask_l);

    return TEST_OK;
}

static int test_ltrace1_stop_on_trigger_int_on(uint32_t cond1_or,
                                                 uint32_t cond2_or,
                                                 uint32_t cond3_or,
                                                 LTRACE_CONDITION_INVERT cond1_invert,
                                                 LTRACE_CONDITION_INVERT cond2_invert,
                                                 LTRACE_CONDITION_INVERT cond3_invert,
                                                 uint32_t condcomp1_polarity_h,
                                                 uint32_t condcomp1_polarity_l,
                                                 uint32_t condcomp1_mask_h,
                                                 uint32_t condcomp1_mask_l,
                                                 uint32_t condcomp2_polarity_h,
                                                 uint32_t condcomp2_polarity_l,
                                                 uint32_t condcomp2_mask_h,
                                                 uint32_t condcomp2_mask_l,
                                                 uint32_t condcomp3_polarity_h,
                                                 uint32_t condcomp3_polarity_l,
                                                 uint32_t condcomp3_mask_h,
                                                 uint32_t condcomp3_mask_l,
                                                 uint32_t compression_mask_h,
                                                 uint32_t compression_mask_l)
{
    test_ltrace1(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                            LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                            LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                            cond1_or,
                            cond2_or,
                            cond3_or,
                            cond1_invert,
                            cond2_invert,
                            cond3_invert,
                            0,
                            0,
                            0,
                            condcomp1_polarity_h,
                            condcomp1_polarity_l,
                            condcomp1_mask_h,
                            condcomp1_mask_l,
                            condcomp2_polarity_h,
                            condcomp2_polarity_l,
                            condcomp2_mask_h,
                            condcomp2_mask_l,
                            condcomp3_polarity_h,
                            condcomp3_polarity_l,
                            condcomp3_mask_h,
                            condcomp3_mask_l,
                            compression_mask_h,
                            compression_mask_l);

    return TEST_OK;
}

static int test_ltrace1_bank_on_trigger_int_off(uint32_t cond1_or,
                                                  uint32_t cond2_or,
                                                  uint32_t cond3_or,
                                                  LTRACE_CONDITION_INVERT cond1_invert,
                                                  LTRACE_CONDITION_INVERT cond2_invert,
                                                  LTRACE_CONDITION_INVERT cond3_invert,
                                                  uint32_t condcomp1_polarity_h,
                                                  uint32_t condcomp1_polarity_l,
                                                  uint32_t condcomp1_mask_h,
                                                  uint32_t condcomp1_mask_l,
                                                  uint32_t condcomp2_polarity_h,
                                                  uint32_t condcomp2_polarity_l,
                                                  uint32_t condcomp2_mask_h,
                                                  uint32_t condcomp2_mask_l,
                                                  uint32_t condcomp3_polarity_h,
                                                  uint32_t condcomp3_polarity_l,
                                                  uint32_t condcomp3_mask_h,
                                                  uint32_t condcomp3_mask_l,
                                                  uint32_t compression_mask_h,
                                                  uint32_t compression_mask_l)
{
    test_ltrace1(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                            LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                            LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                            cond1_or,
                            cond2_or,
                            cond3_or,
                            cond1_invert,
                            cond2_invert,
                            cond3_invert,
                            0,
                            0,
                            0,
                            condcomp1_polarity_h,
                            condcomp1_polarity_l,
                            condcomp1_mask_h,
                            condcomp1_mask_l,
                            condcomp2_polarity_h,
                            condcomp2_polarity_l,
                            condcomp2_mask_h,
                            condcomp2_mask_l,
                            condcomp3_polarity_h,
                            condcomp3_polarity_l,
                            condcomp3_mask_h,
                            condcomp3_mask_l,
                            compression_mask_h,
                            compression_mask_l);

    return TEST_OK;
}

static int test_ltrace1_bank_on_trigger_int_on(uint32_t cond1_or,
                                                 uint32_t cond2_or,
                                                 uint32_t cond3_or,
                                                 LTRACE_CONDITION_INVERT cond1_invert,
                                                 LTRACE_CONDITION_INVERT cond2_invert,
                                                 LTRACE_CONDITION_INVERT cond3_invert,
                                                 uint32_t condcomp1_polarity_h,
                                                 uint32_t condcomp1_polarity_l,
                                                 uint32_t condcomp1_mask_h,
                                                 uint32_t condcomp1_mask_l,
                                                 uint32_t condcomp2_polarity_h,
                                                 uint32_t condcomp2_polarity_l,
                                                 uint32_t condcomp2_mask_h,
                                                 uint32_t condcomp2_mask_l,
                                                 uint32_t condcomp3_polarity_h,
                                                 uint32_t condcomp3_polarity_l,
                                                 uint32_t condcomp3_mask_h,
                                                 uint32_t condcomp3_mask_l,
                                                 uint32_t compression_mask_h,
                                                 uint32_t compression_mask_l)
{
    test_ltrace1(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                            LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                            LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                            cond1_or,
                            cond2_or,
                            cond3_or,
                            cond1_invert,
                            cond2_invert,
                            cond3_invert,
                            0,
                            0,
                            0,
                            condcomp1_polarity_h,
                            condcomp1_polarity_l,
                            condcomp1_mask_h,
                            condcomp1_mask_l,
                            condcomp2_polarity_h,
                            condcomp2_polarity_l,
                            condcomp2_mask_h,
                            condcomp2_mask_l,
                            condcomp3_polarity_h,
                            condcomp3_polarity_l,
                            condcomp3_mask_h,
                            condcomp3_mask_l,
                            compression_mask_h,
                            compression_mask_l);

    return TEST_OK;
}
*/


int main()
{
    uint32_t complete_stop_mask = 0x01;
    uint32_t complete_enable = 0x01;

    uint32_t trace_window_count = 0xAE0A2800;
    //uint32_t trace_window_count = 0x0028C000;

    uint32_t cond1_or = 0x00000004;
    uint32_t cond2_or = 0x00000004;
    uint32_t cond3_or = 0x00000001;
    LTRACE_CONDITION_INVERT cond1_invert = LTRACE_CONDITION_INVERT_NONE;
    LTRACE_CONDITION_INVERT cond2_invert = LTRACE_CONDITION_INVERT_NONE;
    LTRACE_CONDITION_INVERT cond3_invert = LTRACE_CONDITION_INVERT_NONE;
    uint32_t condcomp1_polarity_h = 0xFFFFFFFF;
    uint32_t condcomp1_polarity_l = 0xFFFFFFFF;
    uint32_t condcomp1_mask_h = 0x00000000;
    uint32_t condcomp1_mask_l = 0x00000000;
    uint32_t condcomp2_polarity_h = 0xFFFFFFFF;
    uint32_t condcomp2_polarity_l = 0xFFFFFFFF;
    uint32_t condcomp2_mask_h = 0x00000000;
    uint32_t condcomp2_mask_l = 0x00000000;
    uint32_t condcomp3_polarity_h = 0xFFFFFFFF;
    uint32_t condcomp3_polarity_l = 0xFFFFFFFF;
    uint32_t condcomp3_mask_h = 0x00000000;
    uint32_t condcomp3_mask_l = 0x00000000;
    uint32_t compression_mask_h = 0x00000000;
    uint32_t compression_mask_l = 0x00000000;

    //rumboot_printf("Configure L2C Debug\n");
    dcr_write(DCR_L2C_BASE + L2C_L2DBGSEL, 0x22222222);
    dcr_write(DCR_L2C_BASE + L2C_L2PLBDBG, 0x87654321);


    //LTRACE

    //IAC1
    rumboot_printf("IAC1 Core 0 stop on trigger, one state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                IAC1);

    rumboot_printf("IAC1 Core 0 stop on trigger, two state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                IAC1);

    rumboot_printf("IAC1 Core 0 bank on trigger ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                IAC1);
/*
    //DAC2W
    rumboot_printf("DAC2W Core 0 stop on trigger, one state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                DAC2W);

    rumboot_printf("DAC2W Core 0 stop on trigger, two state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                DAC2W);

    rumboot_printf("DAC2W Core 0 bank on trigger ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                DAC2W);


    //Trap
    rumboot_printf("Trap Core 0 stop on trigger, one state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                Trap);

    rumboot_printf("Trap Core 0 stop on trigger, two state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                Trap);

    rumboot_printf("Trap Core 0 bank on trigger ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                Trap);

    //InstructionComplete
    rumboot_printf("InstructionComplete Core 0 stop on trigger, one state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                InstructionComplete);

    rumboot_printf("InstructionComplete Core 0 stop on trigger, two state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                InstructionComplete);

    rumboot_printf("InstructionComplete Core 0 bank on trigger ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                InstructionComplete);

    //Interrupt
    rumboot_printf("Interrupt Core 0 stop on trigger, one state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                Interrupt);

    rumboot_printf("Interrupt Core 0 stop on trigger, two state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                Interrupt);

    rumboot_printf("Interrupt Core 0 bank on trigger ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                Interrupt);

    //Return
    rumboot_printf("Return Core 0 stop on trigger, one state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                Return);

    rumboot_printf("Return Core 0 stop on trigger, two state ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                Return);

    rumboot_printf("Return Core 0 bank on trigger ...\n");
    test_ltrace(LTRACE_CONDITION_TYPE_COND1,
                LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                LTRACE_COMPRESSION_MODE_NO_COMPRESSION,
                cond1_or,
                cond2_or,
                cond3_or,
                cond1_invert,
                cond2_invert,
                cond3_invert,
                complete_stop_mask,
                complete_enable,
                trace_window_count,
                condcomp1_polarity_h,
                condcomp1_polarity_l,
                condcomp1_mask_h,
                condcomp1_mask_l,
                condcomp2_polarity_h,
                condcomp2_polarity_l,
                condcomp2_mask_h,
                condcomp2_mask_l,
                condcomp3_polarity_h,
                condcomp3_polarity_l,
                condcomp3_mask_h,
                condcomp3_mask_l,
                compression_mask_h,
                compression_mask_l,
                Return);
*/















/*
    test_ltrace0_stop_on_trigger_int_off(cond1_or,
                                          cond2_or,
                                          cond3_or,
                                          cond1_invert,
                                          cond2_invert,
                                          cond3_invert,
                                          condcomp1_polarity_h,
                                          condcomp1_polarity_l,
                                          condcomp1_mask_h,
                                          condcomp1_mask_l,
                                          condcomp2_polarity_h,
                                          condcomp2_polarity_l,
                                          condcomp2_mask_h,
                                          condcomp2_mask_l,
                                          condcomp3_polarity_h,
                                          condcomp3_polarity_l,
                                          condcomp3_mask_h,
                                          condcomp3_mask_l,
                                          compression_mask_h,
                                          compression_mask_l);


    test_ltrace0_stop_on_trigger_int_on(cond1_or,
                                         cond2_or,
                                         cond3_or,
                                         cond1_invert,
                                         cond2_invert,
                                         cond3_invert,
                                         condcomp1_polarity_h,
                                         condcomp1_polarity_l,
                                         condcomp1_mask_h,
                                         condcomp1_mask_l,
                                         condcomp2_polarity_h,
                                         condcomp2_polarity_l,
                                         condcomp2_mask_h,
                                         condcomp2_mask_l,
                                         condcomp3_polarity_h,
                                         condcomp3_polarity_l,
                                         condcomp3_mask_h,
                                         condcomp3_mask_l,
                                         compression_mask_h,
                                         compression_mask_l);


    test_ltrace0_bank_on_trigger_int_off(cond1_or,
                                          cond2_or,
                                          cond3_or,
                                          cond1_invert,
                                          cond2_invert,
                                          cond3_invert,
                                          condcomp1_polarity_h,
                                          condcomp1_polarity_l,
                                          condcomp1_mask_h,
                                          condcomp1_mask_l,
                                          condcomp2_polarity_h,
                                          condcomp2_polarity_l,
                                          condcomp2_mask_h,
                                          condcomp2_mask_l,
                                          condcomp3_polarity_h,
                                          condcomp3_polarity_l,
                                          condcomp3_mask_h,
                                          condcomp3_mask_l,
                                          compression_mask_h,
                                          compression_mask_l);


    test_ltrace0_bank_on_trigger_int_on(cond1_or,
                                         cond2_or,
                                         cond3_or,
                                         cond1_invert,
                                         cond2_invert,
                                         cond3_invert,
                                         condcomp1_polarity_h,
                                         condcomp1_polarity_l,
                                         condcomp1_mask_h,
                                         condcomp1_mask_l,
                                         condcomp2_polarity_h,
                                         condcomp2_polarity_l,
                                         condcomp2_mask_h,
                                         condcomp2_mask_l,
                                         condcomp3_polarity_h,
                                         condcomp3_polarity_l,
                                         condcomp3_mask_h,
                                         condcomp3_mask_l,
                                         compression_mask_h,
                                         compression_mask_l);


    test_ltrace1_stop_on_trigger_int_off(cond1_or,
                                          cond2_or,
                                          cond3_or,
                                          cond1_invert,
                                          cond2_invert,
                                          cond3_invert,
                                          condcomp1_polarity_h,
                                          condcomp1_polarity_l,
                                          condcomp1_mask_h,
                                          condcomp1_mask_l,
                                          condcomp2_polarity_h,
                                          condcomp2_polarity_l,
                                          condcomp2_mask_h,
                                          condcomp2_mask_l,
                                          condcomp3_polarity_h,
                                          condcomp3_polarity_l,
                                          condcomp3_mask_h,
                                          condcomp3_mask_l,
                                          compression_mask_h,
                                          compression_mask_l);


    test_ltrace1_stop_on_trigger_int_on(cond1_or,
                                         cond2_or,
                                         cond3_or,
                                         cond1_invert,
                                         cond2_invert,
                                         cond3_invert,
                                         condcomp1_polarity_h,
                                         condcomp1_polarity_l,
                                         condcomp1_mask_h,
                                         condcomp1_mask_l,
                                         condcomp2_polarity_h,
                                         condcomp2_polarity_l,
                                         condcomp2_mask_h,
                                         condcomp2_mask_l,
                                         condcomp3_polarity_h,
                                         condcomp3_polarity_l,
                                         condcomp3_mask_h,
                                         condcomp3_mask_l,
                                         compression_mask_h,
                                         compression_mask_l);


    test_ltrace1_bank_on_trigger_int_off(cond1_or,
                                          cond2_or,
                                          cond3_or,
                                          cond1_invert,
                                          cond2_invert,
                                          cond3_invert,
                                          condcomp1_polarity_h,
                                          condcomp1_polarity_l,
                                          condcomp1_mask_h,
                                          condcomp1_mask_l,
                                          condcomp2_polarity_h,
                                          condcomp2_polarity_l,
                                          condcomp2_mask_h,
                                          condcomp2_mask_l,
                                          condcomp3_polarity_h,
                                          condcomp3_polarity_l,
                                          condcomp3_mask_h,
                                          condcomp3_mask_l,
                                          compression_mask_h,
                                          compression_mask_l);


    test_ltrace1_bank_on_trigger_int_on(cond1_or,
                                         cond2_or,
                                         cond3_or,
                                         cond1_invert,
                                         cond2_invert,
                                         cond3_invert,
                                         condcomp1_polarity_h,
                                         condcomp1_polarity_l,
                                         condcomp1_mask_h,
                                         condcomp1_mask_l,
                                         condcomp2_polarity_h,
                                         condcomp2_polarity_l,
                                         condcomp2_mask_h,
                                         condcomp2_mask_l,
                                         condcomp3_polarity_h,
                                         condcomp3_polarity_l,
                                         condcomp3_mask_h,
                                         condcomp3_mask_l,
                                         compression_mask_h,
                                         compression_mask_l);
*/



    return 0;
}
