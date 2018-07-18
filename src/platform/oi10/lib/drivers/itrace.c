#include <platform/regs/regs_itrace.h>
#include <platform/devices/itrace.h>
#include <arch/ppc_476fp_lib_c.h>



static void dcr_set(uint32_t const dcr_address, uint32_t const value)
{
    uint32_t dcr_value = dcr_read(dcr_address);
    dcr_value |= value;
    dcr_write(dcr_address, dcr_value);
}

static void dcr_clear(uint32_t const dcr_address, uint32_t const value)
{
    uint32_t dcr_value = dcr_read(dcr_address);
    dcr_value &= ~value;
    dcr_write(dcr_address, dcr_value);
}



void itrace_enable(uint32_t const base_address)
{
    dcr_set(base_address + ITC0_TC, reg_field(0, 0x01));
}

void itrace_disable(uint32_t const base_address)
{
    dcr_set(base_address + ITC0_TC, reg_field(0, 0x00));
}

void itrace_init(uint32_t const base_address, ITRACE_CONDITION_TYPE const condition_type,
                                                     ITRACE_TRIGGER_MODE const trigger_mode,
                                                     ITRACE_SOURCE_SELECTION const source_selection,
                                                     uint32_t const trace_interrupt_enable,
                                                     uint32_t const power_down_eDRAM)
{
    dcr_write(base_address + ITC0_TC, (reg_field(10, (power_down_eDRAM & 0x1)) |
                                      reg_field(8, (trace_interrupt_enable & 0x1)) |
                                      reg_field(7, source_selection) |
                                      reg_field(3, trigger_mode) |
                                      reg_field(2, condition_type)));
}


uint32_t itrace_get_status(uint32_t const base_address)
{
    return (dcr_read(base_address + ITC0_TS) & 0xF8000000);
}

void itrace_clear_status(uint32_t const base_address)
{
    dcr_clear(base_address + ITC0_TS, 0xF8000000);
}

uint32_t itrace_get_last_address1(uint32_t const base_address)
{
    return dcr_read(base_address + ITC0_TLA1);
}

uint32_t itrace_get_last_address2(uint32_t const base_address)
{
    return dcr_read(base_address + ITC0_TLA2);
}

uint32_t itrace_get_trace_window(uint32_t const base_address)
{
    return dcr_read(base_address + ITC0_WCNT);
}

void itrace_set_trace_window(uint32_t const base_address, uint32_t const value)
{
    dcr_write(base_address + ITC0_WCNT, value);
}

uint32_t itrace_get_condition_timeout(uint32_t const base_address)
{
    return dcr_read(base_address + ITC0_CTO);
}

void itrace_set_condition_timeout(uint32_t const base_address, uint32_t const value)
{
    dcr_write(base_address + ITC0_CTO, value);
}

uint32_t itrace_get_cond1_mask_h(uint32_t const base_address)
{
    return (dcr_read(base_address + ITC0_C1MH) & 0xFFFF3FFF);
}

void itrace_set_cond1_mask_h(uint32_t const base_address, uint32_t const value)
{
    dcr_write(base_address + ITC0_C1MH, (value & 0xFFFF3FFF));
}

uint32_t itrace_get_cond2_mask_h(uint32_t const base_address)
{
    return (dcr_read(base_address + ITC0_C2MH) & 0xFFFF3FFF);
}

void itrace_set_cond2_mask_h(uint32_t const base_address, uint32_t const value)
{
    dcr_write(base_address + ITC0_C2MH, (value & 0xFFFF3FFF));
}

uint32_t itrace_get_address(uint32_t const base_address)
{
    return dcr_read(base_address + ITC0_ITAI);
}

void itrace_set_address(uint32_t const base_address, uint32_t const value)
{
    dcr_write(base_address + ITC0_ITAI, value);
}

uint32_t itrace_get_raw_data(uint32_t const base_address)
{
    uint32_t value = dcr_read(base_address + ITC0_ITAD);
    return value;
}

struct itrace_data_2 itrace_get_data(uint32_t const base_address)
{
    struct itrace_data_2 data;
    uint32_t value = dcr_read(base_address + ITC0_ITAD);

    data.value0.trace_execution_status = (value & 0xF8000000) >> 27;
    data.value0.trace_branch_status = (value & 0x07000000) >> 24;
    data.value0.trace_status = (value & 0x00FE0000) >> 17;
    data.value0.stop_bit = (value & 0x00010000) >> 16;

    data.value1.trace_execution_status = (value & 0x0000F800) >> 11;
    data.value1.trace_branch_status = (value & 0x00000700) >> 8;
    data.value1.trace_status = (value & 0x000000FE) >> 1;
    data.value1.stop_bit = (value & 0x00000001);

    return data;
}

void itrace_set_raw_data(uint32_t const base_address, uint32_t const value)
{
    dcr_write(base_address + ITC0_ITAD, value);
}

void itrace_set_data(uint32_t const base_address, struct itrace_data_2 const value)
{
    uint32_t data = (reg_field(31, (value.value1.stop_bit & 0x1)) |
                    reg_field(30, (value.value1.trace_status & 0x7F)) |
                    reg_field(23, (value.value1.trace_branch_status & 0x7)) |
                    reg_field(20, (value.value1.trace_execution_status & 0x1F)) |
                    reg_field(15, (value.value0.stop_bit & 0x1)) |
                    reg_field(14, (value.value0.trace_status & 0x7F)) |
                    reg_field(7, (value.value0.trace_branch_status & 0x7)) |
                    reg_field(4, (value.value0.trace_execution_status & 0x1F)));

    dcr_write(base_address + ITC0_ITAD, data);
}
