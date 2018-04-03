#ifndef _MPIC128_H_
#define _MPIC128_H_

#include <platform/oi10/platform/mpicOI10/mpicOI10_reg_access.h>
#include <platform/oi10/platform/boost/preprocessor/arithmetic/add.hpp>
#include <platform/oi10/platform/boost/preprocessor/arithmetic/mul.hpp>
#include <stdbool.h>


typedef enum{
    Mpic128Timer0 = 0,
    Mpic128Timer1 = 1,
    Mpic128Timer2 = 2,
    Mpic128Timer3 = 3
}mpicOI10_tim_num;

typedef enum{
    mpicOI10_IPI0 = 0,
    mpicOI10_IPI1 = 1,
    mpicOI10_IPI2 = 2,
    mpicOI10_IPI3 = 3
} mpicOI10_ipi_num;

typedef enum
{
    mpicOI10_interrupt_type_noncritical,
    mpicOI10_interrupt_type_critical,
    mpicOI10_interrupt_type_machine_check,
} mpicOI10_interrupt_type;

#define MPIC128_INTERRUPT_SOURCE_N  128
#define MPIC128_INTERRUPT_PROC_N    4
#define MPIC128_INTERRUPT_N         BOOST_PP_ADD(MPIC128_INTERRUPT_SOURCE_N, BOOST_PP_MUL(2,MPIC128_INTERRUPT_PROC_N))
typedef enum
{
    mpicOI10_int_src_SOURCE_begin    = 0,
    mpicOI10_int_src_SOURCE_end      = MPIC128_INTERRUPT_SOURCE_N,
    mpicOI10_int_src_TIMER_begin     = mpicOI10_int_src_SOURCE_end,
    mpicOI10_int_src_TIMER_end       = mpicOI10_int_src_TIMER_begin+MPIC128_INTERRUPT_PROC_N,
    mpicOI10_int_src_IPI_begin       = mpicOI10_int_src_TIMER_end,
    mpicOI10_int_src_IPI_end         = mpicOI10_int_src_IPI_begin+MPIC128_INTERRUPT_PROC_N,

    mpicOI10_int_src_begin           = mpicOI10_int_src_SOURCE_begin,
    mpicOI10_int_src_end             = mpicOI10_int_src_begin+MPIC128_INTERRUPT_N
} mpicOI10_int_src;
/*
typedef enum
{
    mpicOI10_int_src_SOURCE      = 0x00000000,   //   0-127
    mpicOI10_int_src_TIMER       = 0x00000080,   // 128-131
    mpicOI10_int_src_IPI         = 0x00000084    // 132-135
} mpicOI10_int_src_msk;
*/
typedef enum
{
    mpicOI10_timer_freq_SYS_CLK = 0,
    mpicOI10_timer_freq_SYS_TMRCLK = 1,
} mpicOI10_timer_freq;

////////////////////////////////////////////////////////////////////////////
void mpicOI10_reset(uint32_t base_address);
//pass through
void mpicOI10_pass_through_disable(uint32_t base_address);
void mpicOI10_pass_through_enable(uint32_t base_address);
//interrupt type border NONCR, CR, MCHK
void mpicOI10_set_interrupt_borders(uint32_t base_address, uint32_t mcb, uint32_t crb);
//setup TaskPriority
void mpicOI10_set_minimal_interrupt_priority_mask(uint32_t base_address, mpicOI10_proc_num processor_number, mpicOI10_prior_t prior_val);
//setup VP reg
void mpicOI10_setup_ext_interrupt(uint32_t base_address, uint8_t interrupt_number, mpicOI10_prior_t priority,bool sense, bool polarity, mpicOI10_proc_num proc);
static inline void mpicOI10_setup_timer_interrupt(uint32_t const base_address, uint8_t const interrupt_number, mpicOI10_prior_t const priority, mpicOI10_proc_num const proc)
{
    //polarity and sense type are not used for timer interrupts. Just set 0
    mpicOI10_setup_ext_interrupt(base_address, interrupt_number, priority, 0, 0, proc);
}
//mask interrupt in VP
void mpicOI10_mask_interrupt(uint32_t base_address, uint8_t interrupt_number);
//unmask interrupt in VP
void mpicOI10_unmask_interrupt(uint32_t base_address, uint8_t interrupt_number);
//get vector
static inline uint32_t mpicOI10_get_ncr_interrupt_vector(uint32_t const base_address, mpicOI10_proc_num const proc_number)
{
    return dcr_read(base_address + MPIC128x_NCIAR(proc_number));
}

static inline uint32_t mpicOI10_get_cr_interrupt_vector(uint32_t const base_address, mpicOI10_proc_num const proc_number)
{
    return dcr_read(base_address + MPIC128x_CIAR(proc_number));
}

static inline uint32_t mpicOI10_get_mc_interrupt_vector(uint32_t const base_address, mpicOI10_proc_num const proc_number)
{
    return dcr_read(base_address + MPIC128x_IAR(proc_number));
}


//end of interrupt handling
static inline void mpicOI10_end_ncr_interrupt(uint32_t const base_address, mpicOI10_proc_num const proc_number)
{
    dcr_write(base_address + MPIC128x_NCEOI(proc_number), 0);
}

static inline void mpicOI10_end_cr_interrupt(uint32_t const base_address, mpicOI10_proc_num const proc_number)
{
    dcr_write(base_address + MPIC128x_CEOI(proc_number), 0);
}

static inline void mpicOI10_end_mc_interrupt(uint32_t const base_address, mpicOI10_proc_num const proc_number)
{
    dcr_write(base_address + MPIC128x_EOI(proc_number), 0);
}


mpicOI10_interrupt_type mpicOI10_get_interrupt_type(uint32_t base_address, uint8_t interrupt_number);
void mpicOI10_set_interrupt_type(uint32_t base_address, uint8_t interrupt_number, mpicOI10_interrupt_type type);

static inline mpicOI10_proc_num mpicOI10_get_current_proc_num(uint32_t const base_address)
{
    return (mpicOI10_proc_num)((MPIC128_dcr_read_MPIC128x_WAI_PR(base_address) >> IBM_BIT_INDEX(32, 31)) & 0b11);
}


//Initialize MPIC timers
void mpicOI10_timer_init(uint32_t base_address, mpicOI10_timer_freq frequency);

//Check if interrupt number belongs to MPIC Timer
static inline bool mpicOI10_timer_check_interrupt_number(uint8_t const interrupt_number)
{
    return mpicOI10_int_src_TIMER_begin <= interrupt_number && interrupt_number < mpicOI10_int_src_TIMER_end;
}

#define MPIC128_TIMER_GET_BY_INTERRUPT_NUMBER( interrupt_number )   (interrupt_number & 0b11)
static inline mpicOI10_tim_num mpicOI10_timer_get_by_interrupt_number(uint8_t const interrupt_number)
{
    return (mpicOI10_tim_num)MPIC128_TIMER_GET_BY_INTERRUPT_NUMBER(interrupt_number);
}

#define MPIC128_IPI_GET_BY_INTERRUPT_NUMBER( interrupt_number )     (interrupt_number & 0b11)
static inline mpicOI10_ipi_num mpicOI10_ipi_get_by_interrupt_number(uint8_t const interrupt_number)
{
    return (mpicOI10_ipi_num)MPIC128_IPI_GET_BY_INTERRUPT_NUMBER(interrupt_number);
}

//timer: start
static inline void mpicOI10_start_timer(uint32_t const base_address, mpicOI10_tim_num const timer_num, uint32_t const base_count)
{
    dcr_write(base_address + MPIC128x_TBC(timer_num), (base_count & 0x7FFFFFFF) | 1 << 31);
    dcr_write(base_address + MPIC128x_TBC(timer_num), (base_count & 0x7FFFFFFF) | 0 << 31 );//generate transition
}
//timer:stop
static inline void mpicOI10_stop_timer(uint32_t const base_address, mpicOI10_tim_num const timer_num)
{
    dcr_write(base_address + MPIC128x_TBC(timer_num), (1 << 31));
}
//setup priority, target processor..
void mpicOI10_setup_timer_int(uint32_t base_address, mpicOI10_tim_num timer_num, uint8_t vector, mpicOI10_prior_t priority, uint32_t frequency, mpicOI10_proc_num proc);

#endif //_MPIC128_H_
