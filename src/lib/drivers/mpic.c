//#include <mivem_test.h>
#include <platform/oi10/platform/ppc_c.h>
#include <platform/oi10/platform/mpicOI10/mpicOI10.h>
#include <platform/oi10/platform/mpicOI10/mpicOI10_reg_fields.h>
#include <platform/oi10/platform/mpicOI10/mpicOI10_regs.h>
#include <platform/oi10/platform/mpicOI10/mpicOI10_reg_access.h>
#include <common_macros/boost/preprocessor/punctuation/comma_if.hpp>
#include <common_macros/boost/preprocessor/repetition/repeat.hpp>
#include <common_macros/boost/preprocessor/arithmetic/add.hpp>
#include <common_macros/boost/preprocessor/arithmetic/sub.hpp>
//#include <test_macro.h>
//#include <mivem_assert.h>
//#include <platform/oi10/platform/sctl.h>


#define MPIC128x_ADDR_N                     256
#define MPIC128x_XADDR_ERR                  0xFFFFFFFF
#define MPIC128x_WRONG_ADDR(z, n, dummy)    , MPIC128x_XADDR_ERR

#define MPIC128x_VP_ADDR(z, n, dummy)       BOOST_PP_COMMA_IF(n) MPIC128x_VP(n)
#define MPIC128x_TVP_ADDR(z, n, dummy)      , MPIC128x_TVP(n)
#define MPIC128x_IVP_ADDR(z, n, dummy)      , MPIC128x_IVP(n)
static inline uint32_t mpicOI10_get_interrupt_vpaddr_by_num(uint8_t const interrupt_number)
{
    static uint32_t const vpaddr[MPIC128x_ADDR_N] =
    {
        BOOST_PP_REPEAT( MPIC128_INTERRUPT_SOURCE_N, MPIC128x_VP_ADDR, dummy )
        BOOST_PP_REPEAT( MPIC128_INTERRUPT_PROC_N, MPIC128x_TVP_ADDR, dummy )
        BOOST_PP_REPEAT( MPIC128_INTERRUPT_PROC_N, MPIC128x_IVP_ADDR, dummy )
        BOOST_PP_REPEAT( BOOST_PP_SUB( MPIC128x_ADDR_N,MPIC128_INTERRUPT_N ), MPIC128x_WRONG_ADDR, dummy )
    };

    return vpaddr[interrupt_number];

//    if( interrupt_number < mpicOI10_int_src_SOURCE_end ) return MPIC128x_VP(interrupt_number);
//    if( interrupt_number < mpicOI10_int_src_TIMER_end )  return MPIC128x_TVP(mpicOI10_timer_get_by_interrupt_number(interrupt_number));
//    if( interrupt_number < mpicOI10_int_src_IPI_end )    return MPIC128x_IVP(mpicOI10_ipi_get_by_interrupt_number(interrupt_number));
//
//    TEST_ASSERT(0, "Wrong interrupt number");
//    return -1;
}

#define MPIC128x_DST_ADDR(z, n, dummy)      BOOST_PP_COMMA_IF(n) MPIC128x_DST(n)
#define MPIC128x_TDST_ADDR(z, n, dummy)     , MPIC128x_TDST(n)
static inline uint32_t mpicOI10_get_interrupt_dstaddr_by_num(uint8_t const interrupt_number)
{
    static uint32_t const dstaddr[MPIC128x_ADDR_N] =
    {
        BOOST_PP_REPEAT( MPIC128_INTERRUPT_SOURCE_N, MPIC128x_DST_ADDR, dummy )
        BOOST_PP_REPEAT( MPIC128_INTERRUPT_PROC_N, MPIC128x_TDST_ADDR, dummy )
        BOOST_PP_REPEAT( BOOST_PP_SUB( MPIC128x_ADDR_N, BOOST_PP_ADD( MPIC128_INTERRUPT_SOURCE_N, MPIC128_INTERRUPT_PROC_N )), MPIC128x_WRONG_ADDR, dummy )
    };

    return dstaddr[interrupt_number];

//    if( interrupt_number < mpicOI10_int_src_SOURCE_end ) return MPIC128x_DST(interrupt_number);
//    if( interrupt_number < mpicOI10_int_src_TIMER_end )  return MPIC128x_TDST(mpicOI10_timer_get_by_interrupt_number(interrupt_number));
//
//    TEST_ASSERT(0, "Wrong interrupt number");
//    return -1;
}

static inline void mpicOI10_mask_int(uint32_t const vp_addr)
{
    uint32_t tmp;
    tmp = dcr_read(vp_addr);
    SET_BIT(tmp, IBM_BIT_INDEX(32, MPIC128x_VPy_MSK));
    dcr_write(vp_addr, tmp);
}

static inline void mpicOI10_unmask_int(uint32_t const vp_addr)
{
    uint32_t tmp;
    tmp = dcr_read(vp_addr);
    CLEAR_BIT(tmp, IBM_BIT_INDEX(32, MPIC128x_VPy_MSK));
    dcr_write(vp_addr, tmp);
}

static inline uint32_t enable_mpicOI10_vp_modify(uint32_t const vp_addr)
{
    uint32_t vpvalue = dcr_read(vp_addr);
    if( !(vpvalue & (1 << IBM_BIT_INDEX(32, MPIC128x_VPy_MSK)))
     || (vpvalue & (1 << IBM_BIT_INDEX(32, MPIC128x_VPy_ACT))) ) {
        //set MSK=1 ACT=0 for correct modifying
        mpicOI10_mask_int(vp_addr);
        do {
            vpvalue = dcr_read(vp_addr);
        } while( vpvalue & (1 << IBM_BIT_INDEX(32, MPIC128x_VPy_ACT)) );
    }

    return vpvalue;
}

static inline uint32_t mpicOI10_setup_interrupt_source(uint32_t vpvalue, bool const sense, bool const polarity)
{
    CLEAR_BITS_BY_MASK(vpvalue, (0b1    << IBM_BIT_INDEX(32, MPIC128x_VPy_S))
                              | (0b1    << IBM_BIT_INDEX(32, MPIC128x_VPy_POL)));
    SET_BITS_BY_MASK(vpvalue, (sense    << IBM_BIT_INDEX(32, MPIC128x_VPy_S))
                            | (polarity << IBM_BIT_INDEX(32, MPIC128x_VPy_POL)));
    return vpvalue;
}

static inline uint32_t mpicOI10_setup_vp(uint32_t vpvalue, uint8_t const interrupt_number, mpicOI10_prior_t const priority)
{
    CLEAR_BITS_BY_MASK(vpvalue, (0xF            << IBM_BIT_INDEX(32, MPIC128x_VPy_PRI))
                              | (0xFF           << IBM_BIT_INDEX(32, MPIC128x_VPy_VEC)));
    SET_BITS_BY_MASK(vpvalue, (priority         << IBM_BIT_INDEX(32, MPIC128x_VPy_PRI))
                            | (interrupt_number << IBM_BIT_INDEX(32, MPIC128x_VPy_VEC)));
    return vpvalue;
}

static inline void mpicOI10_setup_tvp (uint32_t const address, mpicOI10_prior_t const priority, uint8_t const vector)
{
    uint32_t tmp;
    tmp = dcr_read(address);
    CLEAR_BITS_BY_MASK(tmp, 0xF << IBM_BIT_INDEX(32, MPIC128x_VPy_PRI));
    SET_BITS_BY_MASK(tmp, (priority << IBM_BIT_INDEX(32, MPIC128x_VPy_PRI))
                        | (vector << IBM_BIT_INDEX(32, MPIC128x_VPy_VEC)));
    dcr_write(address, tmp);
}

static inline void mpicOI10_set_interrupt_destination_processor(uint32_t const base_address, uint32_t const dstaddr, mpicOI10_proc_num const proc_number)
{
    dcr_write(base_address + dstaddr, (uint32_t) (1 << IBM_BIT_INDEX(32, (31-proc_number))));
}

static inline void mpicOI10_set_int_timer_destination_proc(uint32_t const base_address, mpicOI10_tim_num const timer, mpicOI10_proc_num const proc_number)
{
    dcr_write(base_address + MPIC128x_TDST(timer), (uint32_t) (1 << proc_number));
}

//
// PUBLIC part
//

void mpicOI10_reset(uint32_t base_address)
{
    uint32_t tmp;
    tmp = MPIC128_dcr_read_MPIC128x_GCF0(base_address);
    SET_BIT(tmp, IBM_BIT_INDEX(32, MPIC128x_GCF0_R));
    MPIC128_dcr_write_MPIC128x_GCF0( base_address, tmp );
    while(MPIC128_dcr_read_MPIC128x_GCF0(base_address) & (1 << IBM_BIT_INDEX(32, MPIC128x_GCF0_R)));
}

void mpicOI10_pass_through_disable(uint32_t base_address)
{
    uint32_t tmp;
    tmp = MPIC128_dcr_read_MPIC128x_GCF0(base_address);
    SET_BIT(tmp, IBM_BIT_INDEX(32, MPIC128x_GCF0_P));
    MPIC128_dcr_write_MPIC128x_GCF0( base_address, tmp );
}

void mpicOI10_pass_through_enable(uint32_t base_address)
/*
* interrupt source 0 is directly connected to the non-critical interrupt input pin of processor 0 (mpicOI10 data book 5.3.2 page )
*/
{
    uint32_t tmp;
    tmp = MPIC128_dcr_read_MPIC128x_GCF0(base_address);
    CLEAR_BIT(tmp, IBM_BIT_INDEX(32, MPIC128x_GCF0_P));

    MPIC128_dcr_write_MPIC128x_GCF0( base_address, tmp );
}

void mpicOI10_set_interrupt_borders(uint32_t base_address, uint32_t mcb, uint32_t crb)
{
//    TEST_ASSERT(mcb >= crb, "See MPIC documentation");
    MPIC128_dcr_write_MPIC128x_VITC( base_address, (mcb << IBM_BIT_INDEX(32, MPIC128x_VITC_MCB)) | (crb << IBM_BIT_INDEX(32, MPIC128x_VITC_CRB)));
}

void mpicOI10_set_minimal_interrupt_priority_mask(uint32_t base_address, mpicOI10_proc_num processor_number, mpicOI10_prior_t prior_val)
/*
* set minimal priority level which will not be masked
*/
{
    dcr_write(base_address + MPIC128x_CTP(processor_number), (uint32_t) prior_val);
}

void mpicOI10_mask_interrupt(uint32_t base_address, uint8_t interrupt_number)
{
    uint32_t interrupt_vpaddr = mpicOI10_get_interrupt_vpaddr_by_num(interrupt_number);

    mpicOI10_mask_int(base_address + interrupt_vpaddr);
}

void mpicOI10_unmask_interrupt(uint32_t base_address, uint8_t interrupt_number)
{
    uint32_t interrupt_vpaddr = mpicOI10_get_interrupt_vpaddr_by_num(interrupt_number);

    mpicOI10_unmask_int(base_address + interrupt_vpaddr);
}

void mpicOI10_setup_ext_interrupt(uint32_t const base_address, uint8_t const interrupt_number, mpicOI10_prior_t const priority, bool const sense, bool const polarity, mpicOI10_proc_num const proc)
{
    uint32_t const interrupt_vpaddr = mpicOI10_get_interrupt_vpaddr_by_num(interrupt_number);
    if( interrupt_vpaddr != MPIC128x_XADDR_ERR ) {
        uint32_t const interrupt_dstaddr    = mpicOI10_get_interrupt_dstaddr_by_num(interrupt_number);
        uint32_t const vpaddr               = base_address + interrupt_vpaddr;
        uint32_t       vpvalue              = enable_mpicOI10_vp_modify(vpaddr);

        if( interrupt_dstaddr != MPIC128x_XADDR_ERR ) {
            mpicOI10_set_interrupt_destination_processor(base_address, interrupt_dstaddr, proc);
        }

        //set vector, priority, sense type and polarity
        vpvalue = mpicOI10_setup_vp(vpvalue, interrupt_number, priority);
        if( interrupt_number < mpicOI10_int_src_SOURCE_end ) {
            vpvalue = mpicOI10_setup_interrupt_source(vpvalue, sense, polarity);
        }
        dcr_write(vpaddr, vpvalue);

        mpicOI10_unmask_int(vpaddr);

        return;
    }

//    TEST_ASSERT(0, "Wrong interrupt number");
}

mpicOI10_interrupt_type mpicOI10_get_interrupt_type(uint32_t base_address, uint8_t interrupt_number)
{
    uint32_t interrupt_vpaddr = mpicOI10_get_interrupt_vpaddr_by_num(interrupt_number);

    const uint32_t priority = (dcr_read(base_address + interrupt_vpaddr)
            >> IBM_BIT_INDEX(32, MPIC128x_VPy_PRI)) & 0b1111;
    const uint32_t borders = MPIC128_dcr_read_MPIC128x_VITC(base_address);
    const uint32_t mcb = (borders >> IBM_BIT_INDEX(32, MPIC128x_VITC_MCB)) & 0b11111;
    const uint32_t crb = (borders >> IBM_BIT_INDEX(32, MPIC128x_VITC_CRB)) & 0b11111;

    if (priority >= mcb)
        return mpicOI10_interrupt_type_machine_check;
    else if (priority >= crb)
        return mpicOI10_interrupt_type_critical;
    else
        return mpicOI10_interrupt_type_noncritical;
}

void mpicOI10_set_interrupt_type(uint32_t base_address, uint8_t interrupt_number, mpicOI10_interrupt_type type)
{
    const uint32_t borders =  MPIC128_dcr_read_MPIC128x_VITC(base_address);
    const uint32_t mcb = (borders >> IBM_BIT_INDEX(32, MPIC128x_VITC_MCB)) & 0b11111;
    const uint32_t crb = (borders >> IBM_BIT_INDEX(32, MPIC128x_VITC_CRB)) & 0b11111;

    uint32_t interrupt_vpaddr = mpicOI10_get_interrupt_vpaddr_by_num(interrupt_number);

    bool masked = (dcr_read(base_address + interrupt_vpaddr)
            >> IBM_BIT_INDEX(32, MPIC128x_VPy_MSK)) & 0b1;

    enable_mpicOI10_vp_modify(base_address + interrupt_vpaddr);

    uint32_t VPx = dcr_read(base_address + interrupt_vpaddr);

    VPx &= ~(0b1111 << IBM_BIT_INDEX(32, MPIC128x_VPy_PRI)); //clear priority bits

    switch (type)
    {
        case mpicOI10_interrupt_type_machine_check:
        {
//            TEST_ASSERT(MPIC128_PRIOR_1 <= mcb && mcb < 16, "See MPIC documentation");
            VPx |= (mcb << IBM_BIT_INDEX(32, MPIC128x_VPy_PRI));
        }
        break;

        case mpicOI10_interrupt_type_critical:
        {
//            TEST_ASSERT(MPIC128_PRIOR_1 <= crb && crb < mcb, "See MPIC documentation");
            VPx |= (crb << IBM_BIT_INDEX(32, MPIC128x_VPy_PRI));
        }
        break;

        case mpicOI10_interrupt_type_noncritical:
        {
//            TEST_ASSERT(MPIC128_PRIOR_1 < crb && crb <= mcb, "See MPIC documentation");
            VPx |= (MPIC128_PRIOR_1 << IBM_BIT_INDEX(32, MPIC128x_VPy_PRI));
        }
        break;

//        default: TEST_ASSERT(0, "Unexpected"); break;
    }
    if (!masked)
    {
        //Unmask
        VPx &= ~(1 << IBM_BIT_INDEX(32, MPIC128x_VPy_MSK));
    }
    dcr_write(base_address + interrupt_vpaddr, VPx);
}

////////TIMER PUBLIC

void mpicOI10_timer_init(uint32_t base_address, mpicOI10_timer_freq frequency)
{

//    uint32_t sys_conf0_reg = SCTL_read_SCTLx_SYS_CONF0(SCTL_BASE);
//    sys_conf0_reg &= ~(1 << 9); //clear SYS_CONF0[MPIC_TMRCLK_SEL];
//    sys_conf0_reg |= (frequency << 9);//set SYS_CONF0[MPIC_TMRCLK_SEL];
//    SCTL_write_SCTLx_SYS_CONF0(SCTL_BASE, sys_conf0_reg);

    switch (frequency)
    {
        case mpicOI10_timer_freq_SYS_CLK:
        {
            MPIC128_dcr_write_MPIC128x_TFR ( base_address, 100*1000*1000 ); //100MHz
        }
        break;

        case mpicOI10_timer_freq_SYS_TMRCLK:
        {
            MPIC128_dcr_write_MPIC128x_TFR ( base_address, 50*1000*1000 ); //50MHz
        }
        break;

        default:
//            TEST_ASSERT(0, "Unexpected");
            break;
    }
}

void __attribute__((deprecated)) mpicOI10_setup_timer_int(uint32_t base_address, mpicOI10_tim_num timer_num, uint8_t vector, mpicOI10_prior_t priority, uint32_t frequency, mpicOI10_proc_num proc_number)
{
    uint32_t address = base_address + MPIC128x_TVP(timer_num);
    enable_mpicOI10_vp_modify(address);//tvp and vp mask/act are identical
    mpicOI10_setup_tvp(address,priority,vector);
    mpicOI10_unmask_int(address);
    //setup frequency
    if(frequency < MPIC128_TIMER_MIN_FREQ)
        MPIC128_dcr_write_MPIC128x_TFR ( base_address, MPIC128_TIMER_MIN_FREQ );
    else
        MPIC128_dcr_write_MPIC128x_TFR ( base_address, frequency );
    mpicOI10_set_int_timer_destination_proc(base_address, timer_num,proc_number);
}
