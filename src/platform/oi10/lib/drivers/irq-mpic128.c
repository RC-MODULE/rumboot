#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/devices/mpic128.h>
#include <platform/regs/regs_mpic128.h>
#include <platform/regs/sctl.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>

#define MC_TYPE  3
#define CR_TYPE  2
#define NCR_TYPE 1

struct mpic128_scratchdata {
    uint32_t irq_type;
};

void mpic128_reset( uint32_t base0 ) {
    dcr_write( base0 + MPIC128_GCF0, dcr_read( base0 + MPIC128_GCF0 )
                                          | ( 1 << MPIC128_GCF0_R_i ) );
    while( dcr_read( base0 + MPIC128_GCF0 ) & ( 1 << MPIC128_GCF0_R_i ) ) {}
}

void mpic128_pass_through_disable( uint32_t base0 ) {
    dcr_write( base0 + MPIC128_GCF0, dcr_read( base0 + MPIC128_GCF0 )
                                          | ( 1 << MPIC128_GCF0_P_i ) );
}

void mpic128_set_interrupt_borders( uint32_t base0, uint32_t mc_border, uint32_t cr_border ) {
    TEST_ASSERT( mc_border >= cr_border, "See MPIC documentation" );
    dcr_write( base0 + MPIC128_VITC, ( mc_border << MPIC128_VITC_MCB_i )
                                          | ( cr_border << MPIC128_VITC_CRB_i ) );
}

static int mpic128_init( const struct rumboot_irq_controller *dev ) {
    mpic128_reset( dev->base0 );
    mpic128_pass_through_disable( dev->base0 );
    mpic128_set_interrupt_borders( dev->base0, MPIC128_PRIOR_UND, MPIC128_PRIOR_UND ); /* make all interrupts non-critical */
    dcr_write( dev->base0 + MPIC128_CTP_PR, MPIC128_PRIOR_0 ); /* enable all interrupts priority */
    return 0; /* We're good */
}

static uint32_t mpic128_begin( const struct rumboot_irq_controller *dev, void *scratch ) {
    uint32_t ack, spv;
    struct mpic128_scratchdata *priv = scratch;
    spv = dcr_read( dev->base0 + MPIC128_SPV );
    ack = dcr_read( dev->base0 + MPIC128_IAR_PR );
    if(ack != spv) {
        priv->irq_type = MC_TYPE;
        return ack;
    }
    ack = dcr_read( dev->base0 + MPIC128_CIAR_PR );
    if(ack != spv) {
        priv->irq_type = CR_TYPE;
        msr_write(spr_read(SPR_CSRR1));
        return ack;
    }
    ack = dcr_read( dev->base0 + MPIC128_NCIAR_PR );
    priv->irq_type = NCR_TYPE;
    return ack;
}

static void mpic128_end( const struct rumboot_irq_controller *dev, void *scratch, uint32_t irq ) {
    struct mpic128_scratchdata *priv = scratch;
    if( irq != dcr_read( dev->base0 + MPIC128_SPV ) ) {
        /* signal the end of processing for non-spurious interrupt */
        if(priv->irq_type == MC_TYPE) {
            dcr_write( dev->base0 + MPIC128_EOI_PR, 0 );
            return;
        }
        if(priv->irq_type == CR_TYPE) {
            dcr_write( dev->base0 + MPIC128_CEOI_PR, 0 );
            return;
        }
        dcr_write( dev->base0 + MPIC128_NCEOI_PR, 0 );
    }
}

#define MPIC128_XADDR_ERR               0xFFFFFFFF
#define MPIC128_INTERRUPT_SOURCE_N      128
#define MPIC128_INTERRUPT_SOURCE_MAX    (MPIC128_INTERRUPT_SOURCE_N-1)
#define MPIC128_INTERRUPT_TIMER_N       4
#define MPIC128_INTERRUPT_TIMER_MAX     (MPIC128_INTERRUPT_SOURCE_MAX+MPIC128_INTERRUPT_TIMER_N)
#define MPIC128_INTERRUPT_IPID_N        4
#define MPIC128_INTERRUPT_IPID_MAX      (MPIC128_INTERRUPT_TIMER_MAX+MPIC128_INTERRUPT_IPID_N)
#define MPIC128_INTERRUPT_MAX           MPIC128_INTERRUPT_IPID_MAX

#define MPIC128_GET_TI_BY_INTERRUPT_NUMBER( interrupt_number )  (interrupt_number & 0b11)


static inline __attribute__((always_inline)) uint32_t mpic128_get_interrupt_vpaddr_by_num( int const irq ) {
    if( irq <= MPIC128_INTERRUPT_SOURCE_MAX )   return MPIC128_VP( irq );
    if( irq <= MPIC128_INTERRUPT_TIMER_MAX )    return MPIC128_TVP( MPIC128_GET_TI_BY_INTERRUPT_NUMBER(irq) );
    if( irq <= MPIC128_INTERRUPT_IPID_MAX )     return MPIC128_IVP( MPIC128_GET_TI_BY_INTERRUPT_NUMBER(irq) );

    return MPIC128_XADDR_ERR;
}

static inline __attribute__((always_inline)) uint32_t mpic128_get_interrupt_dstaddr_by_num( int const irq ) {
    if( irq <= MPIC128_INTERRUPT_SOURCE_MAX )   return MPIC128_DST( irq );
    if( irq <= MPIC128_INTERRUPT_TIMER_MAX )    return MPIC128_TDST( MPIC128_GET_TI_BY_INTERRUPT_NUMBER(irq) );

    return MPIC128_XADDR_ERR;
}

static inline __attribute__((always_inline)) void mpic128_mask_int( uint32_t const vp_addr ) {
    dcr_write( vp_addr, dcr_read( vp_addr )
                      | (1 << MPIC128_VP_MSK_i) );
}

static inline __attribute__((always_inline)) void mpic128_unmask_int( uint32_t const vp_addr ) {
    dcr_write( vp_addr, dcr_read( vp_addr )
                      & ~(1 << MPIC128_VP_MSK_i) );
}

static inline __attribute__((always_inline)) int mpic128_is_unmasked_int( uint32_t const vp_addr ) {
    uint32_t vpvalue = dcr_read( vp_addr );
    if( vpvalue & ( 1 << MPIC128_VP_MSK_i ) ) return 0;
    return 1;
}

static inline uint32_t enable_mpic128_vp_modify( uint32_t const vp_addr ) {
    uint32_t vpvalue = dcr_read( vp_addr );
    if( !( vpvalue & ( 1 << MPIC128_VP_MSK_i ) ) || ( vpvalue & ( 1 << MPIC128_VP_ACT_i ) ) ) {
        mpic128_mask_int( vp_addr ); /* set MSK=1 ACT=0 for correct modifying */
        do {
            vpvalue = dcr_read( vp_addr );
        } while( vpvalue & ( 1 << MPIC128_VP_ACT_i ) );
    }

    return vpvalue;
}

static inline __attribute__((always_inline)) uint32_t mpic128_setup_interrupt_source( uint32_t vpvalue, uint32_t const sense_polarity ) {
    CLEAR_BITS_BY_MASK( vpvalue, FIELD_MASK32( MPIC128_VP_S_i, MPIC128_VP_S_n )
                               | FIELD_MASK32( MPIC128_VP_POL_i, MPIC128_VP_POL_n ) );
    SET_BITS_BY_MASK( vpvalue, sense_polarity );
    return vpvalue;
}

static inline __attribute__((always_inline)) uint32_t mpic128_setup_vp( uint32_t vpvalue, int const interrupt_number, mpic128_prior_t const priority ) {
    CLEAR_BITS_BY_MASK( vpvalue, FIELD_MASK32( MPIC128_VP_PRI_i, MPIC128_VP_PRI_n )
                               | FIELD_MASK32( MPIC128_VP_VEC_i, MPIC128_VP_VEC_n ) );
    SET_BITS_BY_MASK( vpvalue, (priority << MPIC128_VP_PRI_i)
                             | (interrupt_number << MPIC128_VP_VEC_i) );
    return vpvalue;
}

static uint32_t mpic128_setup_ext_interrupt( uint32_t const base0, int const irq, uint32_t const sense_polarity ) {
    uint32_t const interrupt_vpaddr = mpic128_get_interrupt_vpaddr_by_num( irq );
    if( interrupt_vpaddr != MPIC128_XADDR_ERR ) {
        uint32_t const vpaddr = base0 + interrupt_vpaddr;
        uint32_t vpvalue = enable_mpic128_vp_modify( vpaddr );

        uint32_t const interrupt_dstaddr = mpic128_get_interrupt_dstaddr_by_num( irq );
        if( interrupt_dstaddr != MPIC128_XADDR_ERR ) dcr_write( base0 + interrupt_dstaddr, ( 1 << Processor0 ) );

        vpvalue = mpic128_setup_vp( vpvalue, irq, MPIC128_PRIOR_1 ); /* set lowest priority to enable all interrupts handling */
        if( irq <= MPIC128_INTERRUPT_SOURCE_MAX ) vpvalue = mpic128_setup_interrupt_source( vpvalue, sense_polarity );
        dcr_write( vpaddr, vpvalue );

        return interrupt_vpaddr;
    }

    return MPIC128_XADDR_ERR;
}

void mpic128_generate_swint( const struct rumboot_irq_controller *dev, uint32_t irq ) {
    if( irq == MPIC128_IPI_0 ) {
        dcr_write( dev->base0 + MPIC128_IPID_PR, ( 1 << Processor0 ) );
    } else {
        rumboot_platform_panic( "mpic: Can't generate sw irq on line %d", irq );
    }
}

static void mpic128_configure( const struct rumboot_irq_controller *dev, int const irq, uint32_t const flags, int const enable ) {
    uint32_t const interrupt_vpaddr = mpic128_setup_ext_interrupt( dev->base0, irq, flags );
    if( (interrupt_vpaddr != MPIC128_XADDR_ERR) && enable ) mpic128_unmask_int( dev->base0 + interrupt_vpaddr );
}

void mpic128_adjust_priority(const struct rumboot_irq_controller *dev, uint32_t irq, int priority)
{
    uint32_t const interrupt_vpaddr = mpic128_get_interrupt_vpaddr_by_num( irq );
    if( interrupt_vpaddr != MPIC128_XADDR_ERR ) {
        uint32_t const vpaddr = dev->base0 + interrupt_vpaddr;
        int enabled = mpic128_is_unmasked_int(vpaddr);
        uint32_t vpvalue = enable_mpic128_vp_modify( vpaddr );

        uint32_t const interrupt_dstaddr = mpic128_get_interrupt_dstaddr_by_num( irq );
        if( interrupt_dstaddr != MPIC128_XADDR_ERR ) dcr_write( dev->base0 + interrupt_dstaddr, ( 1 << Processor0 ) );

        vpvalue = mpic128_setup_vp( vpvalue, irq, priority );
        dcr_write( vpaddr, vpvalue );

        if(enabled) mpic128_unmask_int(vpaddr);
    }

}

static const struct rumboot_irq_controller irq_ctl = {
    .name  = "MPIC",
    .first = 0,
    .last = MPIC128_INTERRUPT_MAX,
    .init = mpic128_init,
    .begin = mpic128_begin,
    .end = mpic128_end,
    .generate_swint = mpic128_generate_swint,
    .configure = mpic128_configure,
    .priority_min = 0,
    .priority_max = 15,
    .priority_default = 1,
    .adjust_priority = mpic128_adjust_priority,
    .scratch_size = sizeof(struct mpic128_scratchdata),
    .base0 = DCR_MPIC128_BASE,
};

void rumboot_irq_register_mpic128() {
    rumboot_irq_register_controller( &irq_ctl );
}

//timer: init
void mpic128_timer_init(uint32_t base_address, mpic128_timer_freq frequency)
{

    uint32_t sys_conf0_reg = dcr_read(DCR_SCTL_BASE + SCTL_PPC_SYS_CONF);
    sys_conf0_reg &= ~(1 << 0); //clear SYS_CONF0[MPIC_TMRCLK_SEL];
    sys_conf0_reg |= (frequency << 0);//set SYS_CONF0[MPIC_TMRCLK_SEL];
    dcr_write(DCR_SCTL_BASE + SCTL_PPC_SYS_CONF, sys_conf0_reg);

    switch (frequency)
    {
        case mpic128_timer_freq_SYS_CLK:
        {
            //TODO: copy paste from mm7705
            dcr_write(base_address + MPIC128_TFR, 100*1000*1000); //100MHz;
        }
        break;

        case mpic128_timer_freq_SYS_TMRCLK:
        {
            //TODO: copy paste from mm7705
            dcr_write(base_address + MPIC128_TFR, 50*1000*1000); //100MHz;
        }
        break;

        default:
            TEST_ASSERT(0, "Unexpected");
            break;
    }
}

//timer: start
void mpic128_start_timer(uint32_t const base_address, mpic128_tim_num const timer_num, uint32_t const base_count)
{
    dcr_write(base_address + MPIC128_TBC(timer_num), (base_count & 0x7FFFFFFF) | 1 << 31);
    dcr_write(base_address + MPIC128_TBC(timer_num), (base_count & 0x7FFFFFFF) | 0 << 31 );//generate transition
}
//timer:stop
void mpic128_stop_timer(uint32_t const base_address, mpic128_tim_num const timer_num)
{
    dcr_write(base_address + MPIC128_TBC(timer_num), (1 << 31));
}
