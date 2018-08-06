#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>


#define EMI_BASE                DCR_EM2_EMI_BASE
#define TEST_OK                 0x00000000
#define TEST_ERROR              0x00000000

#define CHECKLIST_END           {NULL,NULL}
#define EMI_READ(ADDR)          dcr_read  ((EMI_BASE) + (ADDR)        )
#define EMI_WRITE(ADDR,DATA)    dcr_write ((EMI_BASE) + (ADDR), (DATA))

uint32_t check_emi__crc_on(void);
uint32_t check_emi__crc_off(void);

typedef struct
{
    uint32_t   (*func)();
    char        *desc;
} ckinfo_t;

ckinfo_t    checklist =
{
    {check_emi__crc_on,     "CRC ON" },
    {check_emi__crc_off,    "CRC OFF"},
    CHECKLIST_END
};

uint32_t check_emi__crc_on(void)
{
    return TEST_OK;
}

uint32_t check_emi__crc_off(void)
{
    return TEST_OK;
}

int main(void)
{
    uint32_t     test_result = TEST_OK;
    ckinfo_t    *test;
    emi_init();

    test_event_send_test_id("test_oi10_em2_202");

    for(test = checklist; test->func; test++)
        test_result |= test->func();

    return 0;
}
