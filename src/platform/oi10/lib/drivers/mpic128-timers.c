#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <devices/mpic128.h>
#include <regs/regs_mpic128.h>
#include <platform/regs/sctl.h>
#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>


void mpic128_timer_init( uint32_t const base_address, mpic128_timer_freq const frequency) {
    uint32_t sys_conf0_reg = dcr_read(DCR_SCTL_BASE + SCTL_PPC_SYS_CONF);
    sys_conf0_reg &= ~(1 << 0); //clear SYS_CONF0[MPIC_TMRCLK_SEL];
    sys_conf0_reg |= (frequency << 0);//set SYS_CONF0[MPIC_TMRCLK_SEL];
    dcr_write(DCR_SCTL_BASE + SCTL_PPC_SYS_CONF, sys_conf0_reg);

    switch (frequency) {
        case mpic128_timer_freq_SYS_CLK: {
            dcr_write(base_address + MPIC128_TFR, 25*1000*1000); //25MHz;
            break;
        }
        case mpic128_timer_freq_SYS_TMRCLK: {
            dcr_write(base_address + MPIC128_TFR, (25*1000*1000)/2);//12.5MHz;
            break;
        }
        default: {
            TEST_ASSERT(0, "Unexpected");
            break;
        }
    }
}

void mpic128_start_timer( uint32_t const base_address, mpic128_tim_num const timer_num, uint32_t const base_count ) {
    dcr_write(base_address + MPIC128_TBC(timer_num), (base_count & 0x7FFFFFFF) | 1 << 31);
    dcr_write(base_address + MPIC128_TBC(timer_num), (base_count & 0x7FFFFFFF) | 0 << 31 );//generate transition
}

void mpic128_stop_timer( uint32_t const base_address, mpic128_tim_num const timer_num ) {
    dcr_write(base_address + MPIC128_TBC(timer_num), (1 << 31));
}
