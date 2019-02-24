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


#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>

void mpic128_reset( uint32_t base_address ) {
    dcr_write( base_address + MPIC128_GCF0, dcr_read( base_address + MPIC128_GCF0 )
                                          | ( 1 << MPIC128_GCF0_R_i ) );
    while( dcr_read( base_address + MPIC128_GCF0 ) & ( 1 << MPIC128_GCF0_R_i ) ) {}
}

void mpic128_pass_through_disable( uint32_t base_address ) {
    dcr_write( base_address + MPIC128_GCF0, dcr_read( base_address + MPIC128_GCF0 )
                                          | ( 1 << MPIC128_GCF0_P_i ) );
}

void mpic128_set_interrupt_borders( uint32_t base_address, uint32_t mc_border, uint32_t cr_border ) {
    TEST_ASSERT( mc_border >= cr_border, "See MPIC documentation" );
    dcr_write( base_address + MPIC128_VITC, ( mc_border << MPIC128_VITC_MCB_i )
                                          | ( cr_border << MPIC128_VITC_CRB_i ) );
}

static int mpic128_init( const struct rumboot_irq_controller *dev ) {
    mpic128_reset( DCR_MPIC128_BASE );
    mpic128_pass_through_disable( DCR_MPIC128_BASE );
    mpic128_set_interrupt_borders( DCR_MPIC128_BASE, MPIC128_PRIOR_UND, MPIC128_PRIOR_UND ); /* make all interrupts non-critical */
    dcr_write( DCR_MPIC128_BASE + MPIC128_CTP_PR, MPIC128_PRIOR_0 ); /* enable all interrupts priority */
    return 0; /* We're good */
}

#define IRQ_TYPE_INDX 0
#define CSRR1_INDX    1
#define MSR_INDX      2

#define MC_TYPE  3
#define CR_TYPE  2
#define NCR_TYPE 1

static uint32_t mpic128_begin( const struct rumboot_irq_controller *dev, void *scratch ) {
    uint32_t ack, spv, *data;
    data = scratch;
    data[CSRR1_INDX] = spr_read(SPR_CSRR1);
    data[MSR_INDX] = msr_read();
    msync();
    spv = dcr_read( DCR_MPIC128_BASE + MPIC128_SPV );
    ack = dcr_read( DCR_MPIC128_BASE + MPIC128_IAR_PR );
    if(ack != spv) {
        data[IRQ_TYPE_INDX] = MC_TYPE;
        return ack;
    }
    ack = dcr_read( DCR_MPIC128_BASE + MPIC128_CIAR_PR );
    if(ack != spv) {
        data[IRQ_TYPE_INDX] = CR_TYPE;
        msr_write(data[CSRR1_INDX]);
        return ack;
    }
    ack = dcr_read( DCR_MPIC128_BASE + MPIC128_NCIAR_PR );
    data[IRQ_TYPE_INDX] = NCR_TYPE;
    return ack;
}

static void mpic128_end( const struct rumboot_irq_controller *dev, void *scratch, uint32_t irq ) {
    uint32_t *data = scratch;
    if( irq != dcr_read( DCR_MPIC128_BASE + MPIC128_SPV ) ) {
        /* signal the end of processing for non-spurious interrupt */
        if(data[IRQ_TYPE_INDX] == MC_TYPE) {
            dcr_write( DCR_MPIC128_BASE + MPIC128_EOI_PR, 0 );
            return;
        }
        if(data[IRQ_TYPE_INDX] == CR_TYPE) {
            msr_write(data[MSR_INDX]);
            dcr_write( DCR_MPIC128_BASE + MPIC128_CEOI_PR, 0 );
            return;
        }
        dcr_write( DCR_MPIC128_BASE + MPIC128_NCEOI_PR, 0 );
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

static uint32_t mpic128_setup_ext_interrupt( uint32_t const base_address, int const irq, uint32_t const sense_polarity_priority ) {
    uint32_t value = sense_polarity_priority;
    uint32_t sense_polarity  = CLEAR_BITS_BY_MASK( value, FIELD_MASK32( MPIC128_VP_PRI_i, MPIC128_VP_PRI_n ));
    value = sense_polarity_priority;
    mpic128_prior_t priority = (mpic128_prior_t)(CLEAR_BITS_BY_MASK( value, FIELD_MASK32( MPIC128_VP_S_i, MPIC128_VP_S_n )
                                                                          | FIELD_MASK32( MPIC128_VP_POL_i, MPIC128_VP_POL_n ) ) >> MPIC128_VP_PRI_i);
    if(priority == MPIC128_PRIOR_0) priority = MPIC128_PRIOR_1; /* set lowest priority to enable interrupts handling */
    uint32_t const interrupt_vpaddr = mpic128_get_interrupt_vpaddr_by_num( irq );
    if( interrupt_vpaddr != MPIC128_XADDR_ERR ) {
        uint32_t const vpaddr = base_address + interrupt_vpaddr;
        uint32_t vpvalue = enable_mpic128_vp_modify( vpaddr );

        uint32_t const interrupt_dstaddr = mpic128_get_interrupt_dstaddr_by_num( irq );
        if( interrupt_dstaddr != MPIC128_XADDR_ERR ) dcr_write( base_address + interrupt_dstaddr, ( 1 << Processor0 ) );

        vpvalue = mpic128_setup_vp( vpvalue, irq, priority ); /* set lowest priority to enable all interrupts handling */
        if( irq <= MPIC128_INTERRUPT_SOURCE_MAX ) vpvalue = mpic128_setup_interrupt_source( vpvalue, sense_polarity );
        dcr_write( vpaddr, vpvalue );

        return interrupt_vpaddr;
    }

    return MPIC128_XADDR_ERR;
}

void mpic128_generate_swint( const struct rumboot_irq_controller *dev, uint32_t irq ) {
    if( irq == MPIC128_IPI_0 ) {
        dcr_write( DCR_MPIC128_BASE + MPIC128_IPID_PR, ( 1 << Processor0 ) );
    } else {
        rumboot_platform_panic( "mpic: Can't generate sw irq on line %d", irq );
    }
}

static void mpic128_configure( const struct rumboot_irq_controller *dev, int const irq, uint32_t const flags, int const enable ) {
    uint32_t const interrupt_vpaddr = mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, irq, flags );
    if( (interrupt_vpaddr != MPIC128_XADDR_ERR) && enable ) mpic128_unmask_int( DCR_MPIC128_BASE + interrupt_vpaddr );
}

void mpic128_adjust_priority(const struct rumboot_irq_controller *dev, uint32_t irq, int priority)
{
    /* TODO: Implement me */
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
};

void rumboot_irq_register_mpic128() {
    rumboot_irq_register_controller( &irq_ctl );
}
