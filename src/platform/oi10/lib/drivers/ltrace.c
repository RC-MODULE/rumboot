#include <regs_ltrace.h>
#include <ltrace.h>
#include <devices.h>




static void dcr_set(const uint32_t base_address, const uint32_t reg_address, uint32_t value)
{
    uint32_t reg_value = dcr_read(base_address + reg_address);
    reg_value |= value;
    dcr_write(base_addr + reg_address, reg_value);
}

static void dcr_clear(const uint32_t base_address, const uint32_t reg_address, uint32_t value)
{
    uint32_t reg_value = dcr_read(base_address + reg_address);
    reg_value &= ~value;
    dcr_write(base_addr + reg_address, reg_value);
}




void ltrace_enable()
{
    dcr_set(DCR_LTRACE_BASE, LTR_TC, reg_field(0, 0x01));
}

void ltrace_disable()
{
    dcr_clear(DCR_LTRACE_BASE, LTR_TC, reg_field(0, 0x00));
}

void ltrace_init(LTRACE_CONDITION_TYPE condition_type,
                 LTRACE_TRIGGER_MODE trigger_mode,
                 LTRACE_COMPRESSION_MODE compression_mode,
                 uint32_t cond1_or,
                 uint32_t cond2_or,
                 uint32_t cond3_or,
                 uint32_t logic_array_power_down,
                 LTRACE_CONDITION_INVERT cond1_invert,
                 LTRACE_CONDITION_INVERT cond2_invert,
                 LTRACE_CONDITION_INVERT cond3_invert,
                 uint32_t complete_stop_mask,
                 uint32_t complete_enable)
{
    dcr_write(DCR_LTRACE_BASE + LTR_TC, (reg_field(31, (complete_enable & 0x1)) |
                                         reg_field(30, (complete_stop_mask & 0x1)) |
                                         reg_field(27, (cond3_invert & 0xF)) |
                                         reg_field(23, (cond2_invert & 0xF)) |
                                         reg_field(19, (cond1_invert & 0xF)) |
                                         reg_field(15, (logic_array_power_down & 0x1)) |
                                         reg_field(13, (cond3_or & 0x7)) |
                                         reg_field(10, (cond2_or & 0x7)) |
                                         reg_field(7, (cond1_or & 0x7)) |
                                         reg_field(4, compression_mode) |
                                         reg_field(3, trigger_mode) |
                                         reg_field(2, condition_type)));
}

uint32_t ltrace_get_status()
{
    return (dcr_read(DCR_LTRACE_BASE + LTR_TS) & 0xF8000000);
}

void ltrace_clear_status()
{
    dcr_clear(DCR_LTRACE_BASE + LTR_TS, 0xF8000000);
}

uint32_t ltrace_get_last_address1()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_LA1);
}

uint32_t ltrace_get_last_address2()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_LA2);
}

uint32_t ltrace_get_trace_window()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_WC);
}

void ltrace_set_trace_window(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_WC, value);
}

uint32_t ltrace_get_condition_timeout()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CT);
}

void ltrace_set_condition_timeout(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CT, value);
}

uint32_t ltrace_get_condcomp1_polarity_h()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC1PH);
}

void ltrace_set_condcomp1_polarity_h(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC1PH, value);
}

uint32_t ltrace_get_condcomp1_polarity_l()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC1PL);
}

void ltrace_set_condcomp1_polarity_l(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC1PL, value);
}

uint32_t ltrace_get_condcomp1_mask_h()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC1MH);
}

void ltrace_set_condcomp1_mask_h(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC1MH, value);
}

uint32_t ltrace_get_condcomp1_mask_l()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC1ML);
}

void ltrace_set_condcomp1_mask_l(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC1ML, value);
}


uint32_t ltrace_get_condcomp2_polarity_h()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC2PH);
}

void ltrace_set_condcomp2_polarity_h(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC2PH, value);
}

uint32_t ltrace_get_condcomp2_polarity_l()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC2PL);
}

void ltrace_set_condcomp2_polarity_l(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC2PL, value);
}

uint32_t ltrace_get_condcomp2_mask_h()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC2MH);
}

void ltrace_set_condcomp2_mask_h(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC2MH, value);
}

uint32_t ltrace_get_condcomp2_mask_l()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC2ML);
}

void ltrace_set_condcomp2_mask_l(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC2ML, value);
}


uint32_t ltrace_get_condcomp3_polarity_h()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC3PH);
}

void ltrace_set_condcomp3_polarity_h(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC3PH, value);
}

uint32_t ltrace_get_condcomp3_polarity_l()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC3PL);
}

void ltrace_set_condcomp3_polarity_l(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC3PL, value);
}

uint32_t ltrace_get_condcomp3_mask_h()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC3MH);
}

void ltrace_set_condcomp3_mask_h(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC3MH, value);
}

uint32_t ltrace_get_condcomp3_mask_l()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CC3ML);
}

void ltrace_set_condcomp3_mask_l(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CC3ML, value);
}


uint32_t ltrace_get_compression_mask_h()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CMH);
}

void ltrace_set_compression_mask_h(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CMH, value);
}

uint32_t ltrace_get_compression_mask_l()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_CML);
}

void ltrace_set_compression_mask_l(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_CML, value);
}


LTRACE_ADDRESS ltrace_get_address()
{
    LTRACE_ADDRESS address;

    uint32_t value = dcr_read(DCR_LTRACE_BASE + LTR_AI);
    address.range = (value & 0xC0000000) >> 30;
    address.address = (value & 0x00000FFF);
    return address;
}

void ltrace_set_address(const LTRACE_ADDRESS address)
{
    uint32_t value = (address.address & 0x00000FFF) | ((address.range & 0x00000003) << 30);
    dcr_write(DCR_LTRACE_BASE + LTR_AI, value);
}

uint32_t ltrace_get_data()
{
    return dcr_read(DCR_LTRACE_BASE + LTR_AD);
}

void ltrace_set_data(const uint32_t value)
{
    dcr_write(DCR_LTRACE_BASE + LTR_AD, value);
}
