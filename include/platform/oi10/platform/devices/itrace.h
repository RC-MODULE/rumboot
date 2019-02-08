#ifndef ITRACE_H_
#define ITRACE_H_

#include <stdint.h>

#include <platform/regs/regs_itrace.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


typedef enum
{
    ITRACE_CONDITION_TYPE_BIT0  = 0,
    ITRACE_CONDITION_TYPE_COND1 = 1,
    ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_ENABLE = 2,
    ITRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE = 3
} ITRACE_CONDITION_TYPE;

typedef enum
{
    ITRACE_TRIGGER_MODE_STOP_ON_TRIGGER  = 0,
    ITRACE_TRIGGER_MODE_BANK_ON_TRIGGER = 1
} ITRACE_TRIGGER_MODE;

typedef enum
{
    ITRACE_SOURCE_SELECTION_NO_TRACE    = 0x0,
    ITRACE_SOURCE_SELECTION_CORE0       = 0x8,
    ITRACE_SOURCE_SELECTION_CORE1       = 0x4
} ITRACE_SOURCE_SELECTION;

typedef enum
{
    ITRACE_STATUS_RUNNING               = (1 << IBM_BIT_INDEX(32, 0)),
    ITRACE_STATUS_TERMINATED            = (1 << IBM_BIT_INDEX(32, 1)),
    ITRACE_STATUS_ADDRESS1_WRAPPED      = (1 << IBM_BIT_INDEX(32, 2)),
    ITRACE_STATUS_ADDRESS2_WRAPPED      = (1 << IBM_BIT_INDEX(32, 3)),
    ITRACE_STATUS_LAST_BUFFER_USED      = (1 << IBM_BIT_INDEX(32, 4)),
    ITRACE_STATUS_TIMEOUT_OCCURRED      = (1 << IBM_BIT_INDEX(32, 5))
} ITRACE_STATUS;

DECLARE_CONST( ITRACE_BUFFER_SIZE,  (1 << 14) )//size in dwords

//typedef struct
//{
//    union
//    {
//        uint16_t raw_data;
//        struct
//        {
//            uint16_t trace_execution_status : 5;
//            uint16_t trace_branch_status : 3;
//            uint16_t trace_status : 7;
//            uint16_t stop_bit : 1;
//        };
//    };
//} ITRACE_DATA;

struct itrace_data
{
    uint32_t trace_execution_status;
    uint32_t trace_branch_status;
    uint32_t trace_status;
    uint32_t stop_bit;
};

struct itrace_data_2
{
    struct itrace_data value0;
    struct itrace_data value1;
};


void itrace_enable(uint32_t base_address);
void itrace_disable(uint32_t base_address);

void itrace_init(uint32_t base_address,
                  ITRACE_CONDITION_TYPE condition_type,
                  ITRACE_TRIGGER_MODE trigger_mode,
                  ITRACE_SOURCE_SELECTION source_selection,
                  uint32_t trace_interrupt_enable,
                  uint32_t power_down_eDRAM);

uint32_t itrace_get_status(uint32_t base_address);
void itrace_clear_status(uint32_t base_address, uint32_t value);

uint32_t itrace_get_last_address1(uint32_t base_address);
uint32_t itrace_get_last_address2(uint32_t base_address);

uint32_t itrace_get_trace_window(uint32_t base_address);
void itrace_set_trace_window(uint32_t base_address, uint32_t value);

uint32_t itrace_get_condition_timeout(uint32_t base_address);
void itrace_set_condition_timeout(uint32_t base_address, uint32_t value);

uint32_t itrace_get_cond1_mask_h(uint32_t base_address);
void itrace_set_cond1_mask_h(uint32_t base_address, uint32_t value);

uint32_t itrace_get_cond2_mask_h(uint32_t base_address);
void itrace_set_cond2_mask_h(uint32_t base_address, uint32_t value);

uint32_t itrace_get_address(uint32_t base_address);
void itrace_set_address(uint32_t base_address, uint32_t value);

uint32_t itrace_get_raw_data(uint32_t base_address);
void itrace_set_raw_data(uint32_t base_address, uint32_t value);

struct itrace_data_2 itrace_get_data(uint32_t base_address);
void itrace_set_data(uint32_t base_address, struct itrace_data_2 value);


#endif /* ITRACE_H_ */
