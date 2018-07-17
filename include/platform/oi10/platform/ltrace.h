#ifndef LTRACE_H_
#define LTRACE_H_

#include <stdint.h>
#include <stdbool.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


typedef enum
{
    LTRACE_CONDITION_TYPE_UNUSED  = 0x0,
    LTRACE_CONDITION_TYPE_COND1 = 0x1,
    LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_ENABLE = 0x2,
    LTRACE_CONDITION_TYPE_CONDITIONS_TIMEOUT_DISABLE = 0x3
} LTRACE_CONDITION_TYPE;

typedef enum
{
    LTRACE_TRIGGER_MODE_STOP_ON_TRIGGER  = 0,
    LTRACE_TRIGGER_MODE_BANK_ON_TRIGGER = 1
} LTRACE_TRIGGER_MODE;

typedef enum
{
    LTRACE_COMPRESSION_MODE_NO_COMPRESSION      = 0x0,
    LTRACE_COMPRESSION_MODE_COMPRESSION_ENABLE  = 0x1
} LTRACE_COMPRESSION_MODE;

typedef enum
{
    LTRACE_CONDITION_INVERT_CONDCOMP1      = 0x8,
    LTRACE_CONDITION_INVERT_CONDCOMP2      = 0x4,
    LTRACE_CONDITION_INVERT_CONDCOMP3      = 0x2,
    LTRACE_CONDITION_INVERT_OR_OUTPUT      = 0x1,
    LTRACE_CONDITION_INVERT_NONE           = 0x0,
} LTRACE_CONDITION_INVERT;

typedef enum
{
    LTRACE_STATUS_RUNNING               = (1 << IBM_BIT_INDEX(32, 0)),
    LTRACE_STATUS_COMPLETE              = (1 << IBM_BIT_INDEX(32, 1)),
    LTRACE_STATUS_ADDRESS1_WRAPPED      = (1 << IBM_BIT_INDEX(32, 2)),
    LTRACE_STATUS_ADDRESS2_WRAPPED      = (1 << IBM_BIT_INDEX(32, 3)),
    LTRACE_STATUS_LAST_BUFFER_USED      = (1 << IBM_BIT_INDEX(32, 4)),
    LTRACE_STATUS_TIMEOUT_OCCURRED      = (1 << IBM_BIT_INDEX(32, 5))
} LTRACE_STATUS;


typedef struct
{
    uint16_t range;
    uint16_t address;
} LTRACE_ADDRESS;

typedef struct
{
    uint64_t low_data;
    uint8_t high_data;
} LTRACE_DATA;


void ltrace_enable();
void ltrace_disable();

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
                  uint32_t complete_enable);

uint32_t ltrace_get_status();
void ltrace_clear_status();

uint32_t ltrace_get_last_address1();
uint32_t ltrace_get_last_address2();

uint32_t ltrace_get_trace_window();
void ltrace_set_trace_window(const uint32_t value);

uint32_t ltrace_get_condition_timeout();
void ltrace_set_condition_timeout(const uint32_t value);

uint32_t ltrace_get_condcomp1_polarity_h();
void ltrace_set_condcomp1_polarity_h(const uint32_t value);

uint32_t ltrace_get_condcomp1_polarity_l();
void ltrace_set_condcomp1_polarity_l(const uint32_t value);

uint32_t ltrace_get_condcomp1_mask_h();
void ltrace_set_condcomp1_mask_h(const uint32_t value);

uint32_t ltrace_get_condcomp1_mask_l();
void ltrace_set_condcomp1_mask_l(const uint32_t value);

uint32_t ltrace_get_condcomp2_polarity_h();
void ltrace_set_condcomp2_polarity_h(const uint32_t value);

uint32_t ltrace_get_condcomp2_polarity_l();
void ltrace_set_condcomp2_polarity_l(const uint32_t value);

uint32_t ltrace_get_condcomp2_mask_h();
void ltrace_set_condcomp2_mask_h(const uint32_t value);

uint32_t ltrace_get_condcomp2_mask_l();
void ltrace_set_condcomp2_mask_l(const uint32_t value);

uint32_t ltrace_get_condcomp3_polarity_h();
void ltrace_set_condcomp3_polarity_h(const uint32_t value);

uint32_t ltrace_get_condcomp3_polarity_l();
void ltrace_set_condcomp3_polarity_l(const uint32_t value);

uint32_t ltrace_get_condcomp3_mask_h();
void ltrace_set_condcomp3_mask_h(const uint32_t value);

uint32_t ltrace_get_condcomp3_mask_l();
void ltrace_set_condcomp3_mask_l(const uint32_t value);

uint32_t ltrace_get_compression_mask_h();
void ltrace_set_compression_mask_h(const uint32_t value);

uint32_t ltrace_get_compression_mask_l();
void ltrace_set_compression_mask_l(const uint32_t value);

LTRACE_ADDRESS ltrace_get_address();
void ltrace_set_address(const LTRACE_ADDRESS value);

uint32_t ltrace_get_data();
void ltrace_set_data(const uint32_t value);

#endif /* LTRACE_H_ */
