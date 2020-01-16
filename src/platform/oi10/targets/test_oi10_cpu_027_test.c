#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/test_event_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/itrace.h>


#define EVENT_ITRACE_CHECK_DATA                 0x0000000096
#define EVENT_ITRACE_CHECK_DATA_WITH_MIRROR     (TEST_EVENT_CODE_MIN + 0x1)

#define ADDR_CELL                               0x0008FFFC
#define DATA_ARRAY_ADDRESS_BITS                 15
#define DATA_ARRAY_SIZE                         (1 << DATA_ARRAY_ADDRESS_BITS)

const uint32_t loop_counter = 7010;

/*
uint32_t inline atomic_fetch_and_nop(uint32_t address)
{
    uint32_t volatile data;
    asm volatile
    (
        "label_loop%=:\n\t"
        "lwarx %0, 0, %1\n\t"
        "stwcx. %0, 0, %1\n\t"
        "bne- label_loop%="
        :"=&r"(data)
        :"r"(address)
    );
    return data;
}


uint32_t inline atomic_test_and_set(uint32_t address, uint32_t initially_expected_value, uint32_t value_to_be_set)
{
    uint32_t succeeded = 0;//false
    uint32_t intermediate_data = 0;

    asm volatile
    (
            "label_loop%=:\n\t"
            "    lwarx %1, 0, %2\n\t"
            "    cmpw %1, %3\n\t"
            "    bne- label_fail%=\n\t"
            "    stwcx. %4, 0, %2\n\t"
            "    bne- label_loop%=\n\t"
            "    lis %0, 0\n\t"
            "    ori %0, %0, 1\n\t"
            "    b label_continue%=\n\t"
            "label_fail%=: \n\t"
            "    stwcx. %1, 0, %2\n\t"
            "    bne- label_loop%=\n\t"
            "    lis %0, 0\n\t"
            "label_continue%=: \n\t"
            :"=&r"(succeeded), "=&r"(intermediate_data)
            : "r"(address), "r" (initially_expected_value), "r" (value_to_be_set)
    );
    return succeeded;
}
*/


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
    //trace_msg("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //trace_msg("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //trace_msg("mtspr SPR_IAC1, 0x00\n");
    spr_write(SPR_IAC1, (uint32_t)&call_IAC);

    //trace_msg("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //trace_msg("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //trace_msg("isync\n");
    isync();
    msync();

    //trace_msg("mtspr SPR_DBCR0, 0x00800000\n");
    spr_write(SPR_DBCR0, 0x00800000);
    //spr_write(SPR_DBCR0, 0x0FFF8000);

    //trace_msg("isync\n");
    isync();
    msync();

    //trace_msg("call_IAC\n");
    call_IAC();
}

void IAC1(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //trace_msg("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //trace_msg("mtspr SPR_IAC1, 0x00\n");
    spr_write(SPR_IAC1, (uint32_t)&call_IAC);

    //trace_msg("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //trace_msg("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //trace_msg("isync\n");
    isync();
    msync();

    //trace_msg("mtspr SPR_DBCR0, 0x00800000\n");
    spr_write(SPR_DBCR0, 0x00800000);

    //trace_msg("isync\n");
    isync();
    msync();

    //trace_msg("call_IAC\n");
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x00800000);
    spr_write(SPR_DBSR_RC, 0x00800000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_IAC again\n");
        loop(loop_counter);
        //spr_write(SPR_DBSR_W, 0x00800000);
        //spr_write(SPR_DBSR_RC, 0x00800000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);
        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void IAC2(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //trace_msg("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //trace_msg("mtspr SPR_IAC2, 0x00\n");
    spr_write(SPR_IAC2, (uint32_t)&call_IAC);

    //trace_msg("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //trace_msg("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //trace_msg("isync\n");
    isync();
    msync();

    //trace_msg("mtspr SPR_DBCR0, 0x00400000\n");
    spr_write(SPR_DBCR0, 0x00400000);

    //trace_msg("isync\n");
    isync();
    msync();

    //trace_msg("call_IAC\n");
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x00400000);

    spr_write(SPR_DBSR_RC, 0x00400000);


    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_IAC again\n");
        loop(loop_counter);
        //spr_write(SPR_DBSR_W, 0x00400000);
        //spr_write(SPR_DBSR_RC, 0x00400000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);
        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void IAC3(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //trace_msg("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //trace_msg("mtspr SPR_IAC3, 0x00\n");
    spr_write(SPR_IAC3, (uint32_t)&call_IAC);

    //trace_msg("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //trace_msg("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //trace_msg("isync\n");
    isync();

    //trace_msg("mtspr SPR_DBCR0, 0x00200000\n");
    spr_write(SPR_DBCR0, 0x00200000);

    //trace_msg("isync\n");
    isync();

    //trace_msg("call_IAC\n");
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x00200000);

    spr_write(SPR_DBSR_RC, 0x00200000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_IAC again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x00200000);

        spr_write(SPR_DBSR_RC, 0x00200000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void IAC4(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //trace_msg("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //trace_msg("mtspr SPR_IAC4, 0x00\n");
    spr_write(SPR_IAC4, (uint32_t)&call_IAC);

    //trace_msg("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //trace_msg("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //trace_msg("isync\n");
    isync();

    //trace_msg("mtspr SPR_DBCR0, 0x00100000\n");
    spr_write(SPR_DBCR0, 0x00100000);

    //trace_msg("isync\n");
    isync();

    //trace_msg("call_IAC\n");
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x00100000);

    spr_write(SPR_DBSR_RC, 0x00100000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_IAC again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x00100000);

        spr_write(SPR_DBSR_RC, 0x00100000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void DAC1R(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //trace_msg("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //trace_msg("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //trace_msg("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //trace_msg("isync\n");
    isync();

    //trace_msg("mtspr SPR_DBCR0, 0x00080000\n");
    spr_write(SPR_DBCR0, 0x00080000);

    //trace_msg("isync\n");
    isync();

    //trace_msg("call_DAC1R\n");
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x00080000);

    spr_write(SPR_DBSR_RC, 0x00080000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_DAC1R again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x00080000);

        spr_write(SPR_DBSR_RC, 0x00080000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void DAC1W(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //trace_msg("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //trace_msg("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //trace_msg("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //trace_msg("isync\n");
    isync();

    //trace_msg("mtspr SPR_DBCR0, 0x00040000\n");
    spr_write(SPR_DBCR0, 0x00040000);

    //trace_msg("isync\n");
    isync();

    //trace_msg("call_DAC1W\n");
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x00040000);

    spr_write(SPR_DBSR_RC, 0x00040000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_DAC1W again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x00040000);

        spr_write(SPR_DBSR_RC, 0x00040000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void DAC2R(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //trace_msg("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //trace_msg("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //trace_msg("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //trace_msg("isync\n");
    isync();

    //trace_msg("mtspr SPR_DBCR0, 0x00020000\n");
    spr_write(SPR_DBCR0, 0x00020000);

    //trace_msg("isync\n");
    isync();

    //trace_msg("call_DAC2R\n");
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x00020000);

    spr_write(SPR_DBSR_RC, 0x00020000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_DAC2R again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x00020000);

        spr_write(SPR_DBSR_RC, 0x00020000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void DAC2W(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x00\n");
    spr_write(SPR_DBCR0, 0x00);

    //trace_msg("mtspr SPR_DBSR_RC, 0xCFFF8003\n");
    spr_write(SPR_DBSR_RC, 0xCFFF8003);

    //trace_msg("mtspr SPR_DBCR1, 0x00\n");
    spr_write(SPR_DBCR1, 0x00);

    //trace_msg("mtspr SPR_DBCR2, 0x00\n");
    spr_write(SPR_DBCR2, 0x00);

    //trace_msg("isync\n");
    isync();

    //trace_msg("mtspr SPR_DBCR0, 0x00010000\n");
    spr_write(SPR_DBCR0, 0x00010000);

    //trace_msg("isync\n");
    isync();

    //trace_msg("call_DAC2W\n");
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x00010000);

    spr_write(SPR_DBSR_RC, 0x00010000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_DAC2W again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x00010000);

        spr_write(SPR_DBSR_RC, 0x00010000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Trap(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x01000000\n");
    spr_write(SPR_DBCR0, 0x01000000);

    //trace_msg("call_TRAP\n");
    //call_IAC();
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x01000000);

    spr_write(SPR_DBSR_RC, 0x01000000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_TRAP again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x01000000);

        spr_write(SPR_DBSR_RC, 0x01000000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Branch(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x04000000\n");
    spr_write(SPR_DBCR0, 0x04000000);

    //trace_msg("call_BRANCH\n");
    //call_IAC();
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x04000000);

    spr_write(SPR_DBSR_RC, 0x04000000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_BRANCH again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x04000000);

        spr_write(SPR_DBSR_RC, 0x04000000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void InstructionComplete(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x08000000\n");
    spr_write(SPR_DBCR0, 0x08000000);

    //trace_msg("call_InstructionComplete\n");
    //call_IAC();
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x08000000);

    spr_write(SPR_DBSR_RC, 0x08000000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_InstructionComplete again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Interrupt(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x02000000\n");
    spr_write(SPR_DBCR0, 0x02000000);

    //trace_msg("call_Interrupt\n");
    //call_IAC();
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x02000000);

    spr_write(SPR_DBSR_RC, 0x02000000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_Interrupt again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x02000000);

        spr_write(SPR_DBSR_RC, 0x02000000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Return(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x00000800\n");
    spr_write(SPR_DBCR0, 0x00008000);

    //trace_msg("call_Return\n");
    //call_IAC();
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x00008000);

    spr_write(SPR_DBSR_RC, 0x00008000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_Return again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x00008000);

        spr_write(SPR_DBSR_RC, 0x00008000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}

void Unconditional(uint8_t two_state, uint8_t bank_on_trigger)
{
    //trace_msg("mtspr SPR_DBCR0, 0x40000000\n");
    spr_write(SPR_DBCR0, 0x40000000);

    //trace_msg("call_Unconditional\n");
    //call_IAC();
    loop(loop_counter);
    spr_write(SPR_DBSR_W, 0x40000000);

    spr_write(SPR_DBSR_RC, 0x40000000);

    if (two_state || bank_on_trigger)
    {
        //trace_msg("call_Unconditional again\n");
        loop(loop_counter);
        spr_write(SPR_DBSR_W, 0x40000000);

        spr_write(SPR_DBSR_RC, 0x40000000);

        //trace_msg("emulate instruction completed\n");
        spr_write(SPR_DBSR_W, 0x08000000);

        spr_write(SPR_DBSR_RC, 0x08000000);
    }
}



uint32_t __attribute__((section(".data.test"))) data[DATA_ARRAY_SIZE] = {0};


//static inline uint32_t convert_itrace_address(uint32_t address)
//{
//    return ((address << 1) | (address >> 31));
//}

static inline uint32_t get_stop_address(uint32_t address)
{
    return (address & 0x80000000) ?
          ((address ^ 0x80000000) + 1) :
           (address ^ 0x80000000);
}



static int test_itrace(ITRACE_CONDITION_TYPE condition_type,
                       ITRACE_TRIGGER_MODE trigger_mode,
                       ITRACE_SOURCE_SELECTION source_selection,
                       uint32_t trace_interrupt_enable,
                       uint32_t trace_window_timeout,
                       int32_t cond1_mask,
                       int32_t cond2_mask,
                       void (*fn)(uint8_t, uint8_t))
{

    //rumboot_printf("Configure CCR0\n");

    //uint32_t ccr0 = spr_read(SPR_CCR0);
    //ccr0 |= 0x80000000;
    //ccr0 &= 0xFFFF7FFF;

    memset(data, 0x00, DATA_ARRAY_SIZE);

    spr_write(SPR_CCR0, (spr_read(SPR_CCR0) | (1 << IBM_BIT_INDEX(32, 0))) & ~(1 << IBM_BIT_INDEX(32, 16)));

    //rumboot_printf("Disable\n");
    itrace_disable(DCR_ITRACE_BASE);
    itrace_clear_status(DCR_ITRACE_BASE, 0xF8000000);

    //rumboot_printf("Init\n");
    itrace_init(DCR_ITRACE_BASE, condition_type, trigger_mode, source_selection, trace_interrupt_enable, 0);
    itrace_set_trace_window(DCR_ITRACE_BASE, trace_window_timeout);

    //rumboot_printf("Cond mask 1\n");
    if (cond1_mask != 0x0)
        itrace_set_cond1_mask_h(DCR_ITRACE_BASE, cond1_mask);

    //rumboot_printf("Cond mask 2\n");
    if (cond2_mask != 0x0)
        itrace_set_cond2_mask_h(DCR_ITRACE_BASE, cond2_mask);

    //rumboot_printf("Set address\n");
    //itrace_set_address(0x00000000);

    test_event(EVENT_ITRACE_START);

    //rumboot_printf("Enable\n");
    itrace_enable(DCR_ITRACE_BASE);
/*
#ifdef TEST_MPW_CPU_027_MULTICORE
        rumboot_printf("ITRACE enabled\n");
        atomic_test_and_set((uint32_t) &(active_core_flags[0]),
            (ppcX|0x1110),
            0x2222 );
    }
    else
    {
        rumboot_printf("ITRACE enabling started on other core. Waiting for ready...\n");
        while (atomic_fetch_and_nop((uint32_t) &(active_core_flags[0])) != 0x2222);
    }
#endif
*/
    //rumboot_printf("Fn\n");
    fn((condition_type == ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE) ||
       (condition_type == ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_ENABLE),
       (trigger_mode == ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER));

   // rumboot_printf("Get status running\n");
    /*
    while((itrace_get_status() & ITRACE_STATUS_RUNNING) == ITRACE_STATUS_RUNNING)
    {
        rumboot_printf("Running ...\n");
    }

    */

/*
#ifdef TEST_MPW_CPU_027_MULTICORE

    if(atomic_test_and_set((uint32_t) &(active_core_flags[0]),
            0x2222,
            (ppcX|0x3330) ))
    {
        rumboot_printf("Disabling ITRACE and collecting data\n");
#endif
*/
    TEST_ASSERT((itrace_get_status(DCR_ITRACE_BASE) & ITRACE_STATUS_RUNNING) == 0x00, "Tracing is still running!");

    //rumboot_printf("Get status terminated\n");
    /*
    while((itrace_get_status() & ITRACE_STATUS_TERMINATED) == 0x00)
    {
        rumboot_printf("Not terminated ...\n");
    }
    */

//    TEST_ASSERT((itrace_get_status(DCR_ITRACE_BASE) & ITRACE_STATUS_TERMINATED) == ITRACE_STATUS_TERMINATED, "Tracing does not terminated!");

    //rumboot_printf("Is trace address 1 wrapped\n");
    //TEST_ASSERT((itrace_get_status() & ITRACE_STATUS_ADDRESS1_WRAPPED) == 0x00, "Trace address 1 wrapped!");

//        if (trigger_mode == ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER)
//        {
//            //rumboot_printf("Is trace address 2 wrapped\n");
//            TEST_ASSERT((itrace_get_status() & ITRACE_STATUS_ADDRESS2_WRAPPED) == 0x00, "Trace address 2 wrapped!");
//        }

    if (trace_interrupt_enable)
        test_event(EVENT_CHECK_ITRACE_INT);

    //rumboot_printf("Disable\n");
    itrace_disable(DCR_ITRACE_BASE);

    uint32_t start_address = 0;
    uint32_t address = 0;
    uint32_t last_address[2] = {itrace_get_last_address1(DCR_ITRACE_BASE),
                                itrace_get_last_address2(DCR_ITRACE_BASE)};
    uint32_t stop_address[2] = {0};
    uint32_t i = 0;
    uint32_t j = 0;

    uint32_t itrace_status = itrace_get_status(DCR_ITRACE_BASE);

    //wrapping considering functionality, if not necessary, just comment it out
    if((trigger_mode == ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER))
        last_address[1] = ((( itrace_status >> ITC0_TS_ITA2W_i ) & 1) ?
            ((((ITRACE_BUFFER_SIZE/2) - 1) | ((1<<ITC0_ITLA2_VALID_i) & last_address[1])) ^ 0x80000000)
            : last_address[1]);
    last_address[0] = ((( itrace_status >> ITC0_TS_ITA1W_i ) & 1) ?
            (((ITRACE_BUFFER_SIZE/
                    ((trigger_mode == ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER)? 2 : 1)) - 1) ^ 0x80000000)
            : last_address[0]);
    //end of wrapping considering functionality, if not necessary, just comment it out
    rumboot_printf("last_address[0] == 0x%x\n", last_address[0]);
    rumboot_printf("last_address[1] == 0x%x\n", last_address[1]);

    for(i = 0; i<2; ++i){
        stop_address[i] = get_stop_address(last_address[i]);
        rumboot_printf("stop_address[%d] == 0x%x\n", i, stop_address[i]);
    }

#ifndef TEST_OI10_CPU_027_SRAM_MIRROR_FAST_DEBUG_NO_DCR
//        //debug event, in order to debug with internal SRAM mirror instead of DCR reads
//        test_event(EVENT_ITRACE_CHECK_DATA_WITH_MIRROR);
//#else //TEST_OI10_CPU_027_SRAM_MIRROR_FAST_DEBUG_NO_DCR
    for (j = 0; j <
        (((trigger_mode == ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER)
                && ((1<<ITC0_ITLA2_VALID_i) & last_address[1]))
                ? 2 : 2/*1*/)
                ; j++)
    {
        start_address = ((j & 1) << (DATA_ARRAY_ADDRESS_BITS - 2));
        address = start_address;
        i = ((j & 1) << (DATA_ARRAY_ADDRESS_BITS - 1 ));


        rumboot_printf("j == %d\n", j);
        rumboot_printf("i == %d\n", i);
        rumboot_printf("start_address == 0x%x\n", start_address);
        rumboot_printf("address == 0x%x\n", address);


        do
        {
//                    trace_msg("Address is (write)- ");
//                rumboot_printf("address = 0x%x\n", address);
//                rumboot_printf("Set address write\n");
//                itrace_set_address(address);
//
//                data[0].value0.trace_branch_status = 0xFF;
//                data[0].value0.trace_execution_status = 0xFF;
//                data[0].value0.trace_status = 0xFF;
//                data[0].value0.stop_bit = 0xFF;
//                data[0].value1.trace_branch_status = 0xFF;
//                data[0].value1.trace_execution_status = 0xFF;
//                data[0].value1.trace_status = 0xFF;
//                data[0].value1.stop_bit = 0xFF;
//
//                itrace_set_data(data[0]);


//            rumboot_printf("address = 0x%x, stop_address = 0x%x\n", address, stop_address[j]);
            if ((address >= 0x00004000 /*stop_address[j]*/) && ((stop_address[j] & 0x80000000) == 0x00) /*|| ((address ^ 0x80000000) >= stop_address[j])*/)
                break;


            //trace_msg("Set address\n");
            itrace_set_address(DCR_ITRACE_BASE, address);
            data[i] = itrace_get_raw_data(DCR_ITRACE_BASE);


//            rumboot_printf("dcr address = 0x%x\n", address);
//            rumboot_printf("data_array_address = 0x%x\n", (uint32_t)(&data[i]));
//            rumboot_printf("data[%d] = 0x%x\n", i, data[i]);



            address ^= 0x80000000;
//                rumboot_printf("address = 0x%x\n", address);



//            rumboot_printf("address = 0x%x, stop_address = 0x%x\n", address, stop_address[j]);
            if ((address >= 0x80004000 /*stop_address[j]*/) && ((stop_address[j] & 0x80000000) != 0x00) /*|| ((address ^ 0x80000000) >= stop_address[j])*/)
                break;


            //trace_msg("Set address\n");
            itrace_set_address(DCR_ITRACE_BASE, address);
            data[i+1] = itrace_get_raw_data(DCR_ITRACE_BASE);

//                trace_hex(i);
//                trace_hex(data[i]);
//                trace_hex(data[i+1]);
//
//                trace_msg("i+1=");
//                trace_hex(i+1);

//                msync();




//            rumboot_printf("dcr address = 0x%x\n", address);
//            rumboot_printf("data_arrray_address = 0x%x\n", (uint32_t)(&data[i+1]));
//            rumboot_printf("data[%d] = 0x%x\n", i+1, data[i+1]);



            address ^= 0x80000000;
//                if((i & 0x6) == 0x6)
//                {

            if (i%8 == 0)
                dcbf((uint32_t*)(&data[i-8]));

//                    trace_msg("last address for dump = ");
//                    trace_hex(address);
//                    trace_dump(&data[i & 0xFFFFFFF8],32);

//                }

            i += 2;

//            rumboot_printf("i = %d, dcr address = 0x%x\n", i, address);
            address = (address & 0x1) ? start_address | (1 << (i/4)) : address + 1;
//            rumboot_printf("new dcr address = 0x%x\n", address);

        } while (1);

        dcbf((uint32_t*)(&data[i]));
    }

    //TODO: How to verify trace data?
    //TEST_ASSERT(data.value0.stop_bit == 0x1, "Stop bit not equal to 0x1");


    rumboot_printf("ADDR_CELL = 0x%x\n", ADDR_CELL);
    memset((uint32_t*)(ADDR_CELL & 0xFFFFFF80), 0x00, 0x80);
    iowrite32((uint32_t)&data, ADDR_CELL);
    dcbf((uint32_t*)ADDR_CELL);
    isync();
    msync();

//    rumboot_printf("Store data address = 0x%x\n", ADDR_CELL);
//    rumboot_putdump(ADDR_CELL, 1024);

    test_event(EVENT_ITRACE_CHECK_DATA);
#endif //TEST_OI10_CPU_027_SRAM_MIRROR_FAST_DEBUG_NO_DCR

    return 0;
}


/*
static int test_core0_stop_on_trigger_one_state_int_off(int32_t cond1_mask, int32_t cond2_mask, void (*fn)(uint8_t, uint8_t))
{
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_VALUE,
                0,
                0x00000001,
                cond1_mask,
                cond2_mask,
                fn);

    return TEST_OK;
}

static int test_core0_stop_on_trigger_two_state_int_off(int32_t cond1_mask, int32_t cond2_mask, void (*fn)(uint8_t, uint8_t))
{
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_VALUE,
                0,
                0xA3000000,
                cond1_mask,
                cond2_mask,
                fn);

    return TEST_OK;
}


static int test_core0_stop_on_trigger_int_on(int32_t cond1_mask, int32_t cond2_mask, void (*fn)(uint8_t, uint8_t))
{
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_VALUE,
                1,
                0xA3000000,
                cond1_mask,
                cond2_mask,
                fn);

    return TEST_OK;
}

static int test_core1_stop_on_trigger_int_off(int32_t cond1_mask, int32_t cond2_mask, void (*fn)(uint8_t, uint8_t))
{
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_VALUE_CORE1,
                0,
                0xA3000000,
                cond1_mask,
                cond2_mask,
                fn);

    return TEST_OK;
}


static int test_core1_stop_on_trigger_int_on(int32_t cond1_mask, int32_t cond2_mask, void (*fn)(uint8_t, uint8_t))
{
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_VALUE_CORE1,
                1,
                0xA3000000,
                cond1_mask,
                cond2_mask,
                fn);

    return TEST_OK;
}


static int test_core0_bank_on_trigger_int_off(int32_t cond1_mask, int32_t cond2_mask, void (*fn)(uint8_t, uint8_t))
{
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_VALUE,
                0,
                0xA3000000,
                cond1_mask,
                cond2_mask,
                fn);

    return TEST_OK;
}

static int test_core0_bank_on_trigger_int_on(int32_t cond1_mask, int32_t cond2_mask, void (*fn)(uint8_t, uint8_t))
{
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_VALUE,
                1,
                0xA3000000,
                cond1_mask,
                cond2_mask,
                fn);

    return TEST_OK;
}

static int test_core1_bank_on_trigger_int_off(int32_t cond1_mask, int32_t cond2_mask, void (*fn)(uint8_t, uint8_t))
{
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_VALUE_CORE1,
                0,
                0x00014600,
                cond1_mask,
                cond2_mask,
                fn);

    return TEST_OK;
}

static int test_core1_bank_on_trigger_int_on(int32_t cond1_mask, int32_t cond2_mask, void (*fn)(uint8_t, uint8_t))
{
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_VALUE_CORE1,
                1,
                0x00014600,
                cond1_mask,
                cond2_mask,
                fn);

    return TEST_OK;
}
*/

int __attribute__((section(".text.test"))) main()
{

//    uint32_t atomic_fetch_and_nop(uint32_t const address)
//    uint32_t atomic_test_and_set(uint32_t address, uint32_t initially_expected_value, uint32_t value_to_be_set)

/*
    rumboot_printf("IAC1 stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x20002000, 0x20002000, IAC1);
    rumboot_printf("IAC2 stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x10001000, 0x10001000, IAC2);
    rumboot_printf("IAC3 stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x08000800, 0x08000800, IAC3);
    rumboot_printf("IAC4 stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x04000400, 0x04000400, IAC4);

    rumboot_printf("DAC1R stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x02000200, 0x02000200, DAC1R);
    rumboot_printf("DAC1W stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x01000100, 0x01000100, DAC1W);
    rumboot_printf("DAC2R stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x00800080, 0x00800080, DAC2R);
    rumboot_printf("DAC2W stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x00400040, 0x00400040, DAC2W);

    rumboot_printf("Trap stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x00200020, 0x00200020, Trap);
    rumboot_printf("Branch stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x00020002, 0x00020002, Branch);
    rumboot_printf("InstructionComplete stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x00010001, 0x00010001, InstructionComplete);
    rumboot_printf("Interrupt stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x00100010, 0x00100010, Interrupt);
    rumboot_printf("Return stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x00040004, 0x00040004, Return);
    rumboot_printf("Unconditional stop on trigger, interrupt disabled ...\n");
    test_core0_stop_on_trigger_int_on(0x00080008, 0x00080008, Unconditional);

    return TEST_OK;
*/

/*
    //Core 0
    //IAC1
    rumboot_printf("IAC1 stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x20002000,
                0x20002000,
                IAC1);

    rumboot_printf("IAC1 stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x20002000,
                0x20002000,
                IAC1);

    rumboot_printf("IAC1 stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x20002000,
                0x20002000,
                IAC1);
*/
    rumboot_printf("IAC1 stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x20002000,
                0x00010001,
                IAC1);

    rumboot_printf("IAC1 bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x20002000,
                0x00010001,
                IAC1);

/*
    rumboot_printf("IAC1 bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x20002000,
                0x00010001,
                IAC1);


    //IAC2
    rumboot_printf("IAC2 stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x10001000,
                0x10001000,
                IAC2);

    rumboot_printf("IAC2 stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x10001000,
                0x10001000,
                IAC2);

    rumboot_printf("IAC2 stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x10001000,
                0x10001000,
                IAC2);

    rumboot_printf("IAC2 stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x10001000,
                0x10001000,
                IAC2);

    rumboot_printf("IAC2 bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x10001000,
                0x00010001,
                IAC2);

    rumboot_printf("IAC2 bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x10001000,
                0x00010001,
                IAC2);


    //IAC3
    rumboot_printf("IAC3 stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x08000800,
                0x08000800,
                IAC3);

    rumboot_printf("IAC3 stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x08000800,
                0x08000800,
                IAC3);

    rumboot_printf("IAC3 stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x08000800,
                0x08000800,
                IAC3);

    rumboot_printf("IAC3 stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x08000800,
                0x08000800,
                IAC3);

    rumboot_printf("IAC3 bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x08000800,
                0x00010001,
                IAC3);

    rumboot_printf("IAC3 bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x08000800,
                0x00010001,
                IAC3);


    //IAC4
    rumboot_printf("IAC4 stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x04000400,
                0x04000400,
                IAC4);

    rumboot_printf("IAC4 stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x04000400,
                0x04000400,
                IAC4);

    rumboot_printf("IAC4 stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x04000400,
                0x04000400,
                IAC4);

    rumboot_printf("IAC4 stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x04000400,
                0x04000400,
                IAC4);

    rumboot_printf("IAC4 bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x04000400,
                0x00010001,
                IAC4);

    rumboot_printf("IAC4 bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x04000400,
                0x00010001,
                IAC4);



    //DAC1R
    rumboot_printf("DAC1R stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x02000200,
                0x02000200,
                DAC1R);

    rumboot_printf("DAC1R stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x02000200,
                0x02000200,
                DAC1R);

    rumboot_printf("DAC1R stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x02000200,
                0x02000200,
                DAC1R);

    rumboot_printf("DAC1R stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x02000200,
                0x02000200,
                DAC1R);

    rumboot_printf("DAC1R bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x02000200,
                0x00010001,
                DAC1R);

    rumboot_printf("DAC1R bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x02000200,
                0x00010001,
                DAC1R);



    //DAC1W
    rumboot_printf("DAC1W stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x01000100,
                0x01000100,
                DAC1W);

    rumboot_printf("DAC1W stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x01000100,
                0x01000100,
                DAC1W);

    rumboot_printf("DAC1W stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x01000100,
                0x01000100,
                DAC1W);

    rumboot_printf("DAC1W stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x01000100,
                0x01000100,
                DAC1W);

    rumboot_printf("DAC1W bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x01000100,
                0x00010001,
                DAC1W);

    rumboot_printf("DAC1W bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x01000100,
                0x00010001,
                DAC1W);



    //DAC2R
    rumboot_printf("DAC2R stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00800080,
                0x00800080,
                DAC2R);

    rumboot_printf("DAC2R stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00800080,
                0x00800080,
                DAC2R);

    rumboot_printf("DAC2R stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00800080,
                0x00800080,
                DAC2R);

    rumboot_printf("DAC2R stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00800080,
                0x00800080,
                DAC2R);

    rumboot_printf("DAC2R bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00800080,
                0x00010001,
                DAC2R);

    rumboot_printf("DAC2R bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00800080,
                0x00010001,
                DAC2R);



    //DAC2W
    rumboot_printf("DAC2W stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00400040,
                0x00400040,
                DAC2W);

    rumboot_printf("DAC2W stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00400040,
                0x00400040,
                DAC2W);

    rumboot_printf("DAC2W stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00400040,
                0x00400040,
                DAC2W);

    rumboot_printf("DAC2W stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00400040,
                0x00400040,
                DAC2W);

    rumboot_printf("DAC2W bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00400040,
                0x00010001,
                DAC2W);

    rumboot_printf("DAC2W bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00400040,
                0x00010001,
                DAC2W);



    //Trap
    rumboot_printf("Trap stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00200020,
                0x00200020,
                Trap);

    rumboot_printf("Trap stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00200020,
                0x00200020,
                Trap);

    rumboot_printf("Trap stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00200020,
                0x00200020,
                Trap);

    rumboot_printf("Trap stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00200020,
                0x00200020,
                Trap);

    rumboot_printf("Trap bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00200020,
                0x00010001,
                Trap);

    rumboot_printf("Trap bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00200020,
                0x00010001,
                Trap);



    //Branch
    rumboot_printf("Branch stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00020002,
                0x00020002,
                Branch);

    rumboot_printf("Branch stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00020002,
                0x00020002,
                Branch);

    rumboot_printf("Branch stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00020002,
                0x00020002,
                Branch);

    rumboot_printf("Branch stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00020002,
                0x00020002,
                Branch);

    rumboot_printf("Branch bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00020002,
                0x00010001,
                Branch);

    rumboot_printf("Branch bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00020002,
                0x00010001,
                Branch);



    //InstructionComplete
    rumboot_printf("InstructionComplete stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00010001,
                0x00010001,
                InstructionComplete);

    rumboot_printf("InstructionComplete stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00010001,
                0x00010001,
                InstructionComplete);

    rumboot_printf("InstructionComplete stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00010001,
                0x00010001,
                InstructionComplete);

    rumboot_printf("InstructionComplete stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00010001,
                0x00010001,
                InstructionComplete);

    rumboot_printf("InstructionComplete bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00010001,
                0x00010001,
                InstructionComplete);

    rumboot_printf("InstructionComplete bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00010001,
                0x00010001,
                InstructionComplete);



    //Interrupt
    rumboot_printf("Interrupt stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00100010,
                0x00100010,
                Interrupt);

    rumboot_printf("Interrupt stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00100010,
                0x00100010,
                Interrupt);

    rumboot_printf("Interrupt stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00100010,
                0x00100010,
                Interrupt);

    rumboot_printf("Interrupt stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00100010,
                0x00100010,
                Interrupt);

    rumboot_printf("Interrupt bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00100010,
                0x00010001,
                Interrupt);

    rumboot_printf("Interrupt bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00100010,
                0x00010001,
                Interrupt);



    //Return
    rumboot_printf("Return stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00040004,
                0x00040004,
                Return);

    rumboot_printf("Return stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00040004,
                0x00040004,
                Return);

    rumboot_printf("Return stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00040004,
                0x00040004,
                Return);

    rumboot_printf("Return stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00040004,
                0x00040004,
                Return);

    rumboot_printf("Return bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00040004,
                0x00010001,
                Return);

    rumboot_printf("Return bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00040004,
                0x00010001,
                Return);



    //Unconditional
    rumboot_printf("Unconditional stop on trigger, one state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00080008,
                0x00080008,
                Unconditional);

    rumboot_printf("Unconditional stop on trigger, one state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00080008,
                0x00080008,
                Unconditional);

    rumboot_printf("Unconditional stop on trigger, two state interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00080008,
                0x00080008,
                Unconditional);

    rumboot_printf("Unconditional stop on trigger, two state interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE,
                ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00080008,
                0x00080008,
                Unconditional);

    rumboot_printf("Unconditional bank on trigger, interrupt disabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                0,
                0x00000001,
                0x00080008,
                0x00010001,
                Unconditional);

    rumboot_printf("Unconditional bank on trigger, interrupt enabled ...\n");
    test_itrace(ITRACE_CONDITION_TYPE_COND1,
                ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER,
                ITRACE_SOURCE_SELECTION_CORE0,
                1,
                0x00000001,
                0x00080008,
                0x00010001,
                Unconditional);
*/

    return 0;
}
