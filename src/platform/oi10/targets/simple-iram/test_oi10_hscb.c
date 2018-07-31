/*
 * test_oi10_hscb.c
 *
 *  Created on: Jul 23, 2018
 *      Author: r.galiulin
 */


#include <stdint.h>
#include <stdbool.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <regs/regs_hscb.h>


#define GPIO_REG_MASK   0xFFFFFFFF
#define CHECK_REGS
#define HSCB_X_BASE     SW0_BASE

#ifdef CHECK_REGS
static uint32_t check_hscb_default_val( uint32_t base_addr ) {
    rumboot_printf("Check the default values of the registers:\n");

    struct regpoker_checker check_default_array[] = {
        {   "HSCB_ID",              REGPOKER_READ32,    HSCB_ID,                HSCB_ID_DFLT,               HSCB_ID_MASK   },
        {   "HSCB_VERSION",         REGPOKER_READ32,    HSCB_VERSION,           HSCB_VERSION_DFLT,          HSCB_VERSION_MASK   },
        {   "HSCB_SW_RESET",        REGPOKER_READ32,    HSCB_SW_RESET,          HSCB_SW_RESET_DFLT,         HSCB_SW_RESET_MASK   },
        {   "HSCB_SETTINGS",        REGPOKER_READ32,    HSCB_SETTINGS,          HSCB_SETTINGS_DFLT,         HSCB_SETTINGS_MASK   },
        {   "HSCB_STATUS",          REGPOKER_READ32,    HSCB_STATUS,            HSCB_STATUS_DFLT,           HSCB_STATUS_MASK   },
        {   "HSCB_IRQ_MASK",        REGPOKER_READ32,    HSCB_IRQ_MASK,          HSCB_IRQ_MASK_DFLT,         HSCB_IRQ_MASK_MASK   },
        {   "HSCB_TRANS_CLK",       REGPOKER_READ32,    HSCB_TRANS_CLK,         HSCB_TRANS_CLK_DFLT,        HSCB_TRANS_CLK_MASK   },
        {   "HSCB_TIMINGS",         REGPOKER_READ32,    HSCB_TIMINGS,           HSCB_TIMINGS_DFLT,          GPIO_REG_MASK   },
        {   "HSCB_INTERVAL",        REGPOKER_READ32,    HSCB_INTERVAL,          HSCB_INTERVAL_DFLT,         GPIO_REG_MASK   },
        {   "HSCB_TIME_CODE_INNER", REGPOKER_READ32,    HSCB_TIME_CODE_INNER,   HSCB_TIME_CODE_INNER_DFLT,  GPIO_REG_MASK   },
        {   "HSCB_TIME_CODE_OUTER", REGPOKER_READ32,    HSCB_TIME_CODE_OUTER,   HSCB_TIME_CODE_OUTER_DFLT,  GPIO_REG_MASK   },
        {   "HSCB_RMAP_PARAMS",     REGPOKER_READ32,    HSCB_RMAP_PARAMS,       HSCB_RMAP_PARAMS_DFLT,      GPIO_REG_MASK   },
        {   "HSCB_RMAP_STATUS",     REGPOKER_READ32,    HSCB_RMAP_STATUS,       HSCB_RMAP_STATUS_DFLT,      GPIO_REG_MASK   },
        {   "HSCB_RMAP_C_STATUS",   REGPOKER_READ32,    HSCB_RMAP_C_STATUS,     HSCB_RMAP_C_STATUS_DFLT,    GPIO_REG_MASK   },
        {   "HSCB_ADMA_ID",         REGPOKER_READ32,    HSCB_ADMA_ID,           HSCB_ADMA_ID_DFLT,          GPIO_REG_MASK   },
        {   "HSCB_ADMA_REV",        REGPOKER_READ32,    HSCB_ADMA_REV,          HSCB_ADMA_REV_DFLT,         GPIO_REG_MASK   },
        {   "HSCB_ADMA_SW_RESET",   REGPOKER_READ32,    HSCB_ADMA_SW_RESET,     HSCB_ADMA_SW_RESET_DFLT,    GPIO_REG_MASK   },
        {   "HSCB_ADMA_CH_STATUS",  REGPOKER_READ32,    HSCB_ADMA_CH_STATUS,    HSCB_ADMA_CH_STATUS_DFLT,   GPIO_REG_MASK   },
        {   "HSCB_ARLEN",           REGPOKER_READ32,    HSCB_ARLEN,             HSCB_ARLEN_DFLT,            GPIO_REG_MASK   },
        {   "HSCB_AWLEN",           REGPOKER_READ32,    HSCB_AWLEN,             HSCB_AWLEN_DFLT,            GPIO_REG_MASK   },
        {   "HSCB_BRESP",           REGPOKER_READ32,    HSCB_BRESP,             HSCB_BRESP_DFLT,            GPIO_REG_MASK   },
        {   "HSCB_RAXI_ERR_ADDR",   REGPOKER_READ32,    HSCB_RAXI_ERR_ADDR,     HSCB_RAXI_ERR_ADDR_DFLT,    GPIO_REG_MASK   },
        {   "HSCB_ARCACHE",         REGPOKER_READ32,    HSCB_ARCACHE,           HSCB_ARCACHE_DFLT,          GPIO_REG_MASK   },
        {   "HSCB_AWCACHE",         REGPOKER_READ32,    HSCB_AWCACHE,           HSCB_AWCACHE_DFLT,          GPIO_REG_MASK   },
        {   "HSCB_ARPROT",          REGPOKER_READ32,    HSCB_ARPROT,            HSCB_ARPROT_DFLT,           GPIO_REG_MASK   },
        {   "HSCB_AWPROT",          REGPOKER_READ32,    HSCB_AWPROT,            HSCB_AWPROT_DFLT,           GPIO_REG_MASK   },
        {   "HSCB_ARLOCK",          REGPOKER_READ32,    HSCB_ARLOCK,            HSCB_ARLOCK_DFLT,           GPIO_REG_MASK   },
        {   "HSCB_AWLOCK",          REGPOKER_READ32,    HSCB_AWLOCK,            HSCB_AWLOCK_DFLT,           GPIO_REG_MASK   },
        {   "HSCB_ARBURST",         REGPOKER_READ32,    HSCB_ARBURST,           HSCB_ARBURST_DFLT,          GPIO_REG_MASK   },
        {   "HSCB_AWBURST",         REGPOKER_READ32,    HSCB_AWBURST,           HSCB_AWBURST_DFLT,          GPIO_REG_MASK   },
        {   "HSCB_RDMA_SETTINGS",   REGPOKER_READ32,    HSCB_RDMA_SETTINGS,     HSCB_RDMA_SETTINGS_DFLT,    GPIO_REG_MASK   },
        {   "HSCB_RDMA_STATUS",     REGPOKER_READ32,    HSCB_RDMA_STATUS,       HSCB_RDMA_STATUS_DFLT,      GPIO_REG_MASK   },
        {   "HSCB_RDMA_SYS_ADDR",   REGPOKER_READ32,    HSCB_RDMA_SYS_ADDR,     HSCB_RDMA_SYS_ADDR_DFLT,    GPIO_REG_MASK   },
        {   "HSCB_RDMA_TBL_SIZE",   REGPOKER_READ32,    HSCB_RDMA_TBL_SIZE,     HSCB_RDMA_TBL_SIZE_DFLT,    GPIO_REG_MASK   },
        {   "HSCB_RDMA_ADDR",       REGPOKER_READ32,    HSCB_RDMA_ADDR,         HSCB_RDMA_ADDR_DFLT,        GPIO_REG_MASK   },
        {   "HSCB_RDMA_LEN",        REGPOKER_READ32,    HSCB_RDMA_LEN,          HSCB_RDMA_LEN_DFLT,         GPIO_REG_MASK   },
        {   "HSCB_RDMA_DESC_ADDR",  REGPOKER_READ32,    HSCB_RDMA_DESC_ADDR,    HSCB_RDMA_DESC_ADDR_DFLT,   GPIO_REG_MASK   },
        {   "HSCB_WDMA_SETTINGS",   REGPOKER_READ32,    HSCB_WDMA_SETTINGS,     HSCB_WDMA_SETTINGS_DFLT,    GPIO_REG_MASK   },
        {   "HSCB_WDMA_STATUS",     REGPOKER_READ32,    HSCB_WDMA_STATUS,       HSCB_WDMA_STATUS_DFLT,      GPIO_REG_MASK   },
        {   "HSCB_WDMA_SYS_ADDR",   REGPOKER_READ32,    HSCB_WDMA_SYS_ADDR,     HSCB_WDMA_SYS_ADDR_DFLT,    GPIO_REG_MASK   },
        {   "HSCB_WDMA_TBL_SIZE",   REGPOKER_READ32,    HSCB_WDMA_TBL_SIZE,     HSCB_WDMA_TBL_SIZE_DFLT,    GPIO_REG_MASK   },
        {   "HSCB_WDMA_ADDR",       REGPOKER_READ32,    HSCB_WDMA_ADDR,         HSCB_WDMA_ADDR_DFLT,        GPIO_REG_MASK   },
        {   "HSCB_WDMA_LEN",        REGPOKER_READ32,    HSCB_WDMA_LEN,          HSCB_WDMA_LEN_DFLT,         GPIO_REG_MASK   },
        {   "HSCB_WDMA_DESC_ADDR",  REGPOKER_READ32,    HSCB_WDMA_DESC_ADDR,    HSCB_WDMA_DESC_ADDR_DFLT,   GPIO_REG_MASK   },
        { /* Sentinel */ }
    };

    if( rumboot_regpoker_check_array( check_default_array, base_addr ) == 0 ) {
        rumboot_printf( " OK\n" );
        return 0;
    }

    rumboot_printf( " ERROR\n" );
    return 1;
}

//static uint32_t check_gpio_regs( uint32_t base_addr ) {
//    rumboot_printf("Check WRITE/READ registers:");
//
//    struct regpoker_checker check_rw_array[] = {
//        {   "GPIO_DIR",         REGPOKER_WRITE32,   GPIO_DIR,                   ~GPIO_DIR_DEFAULT,      GPIO_REG_MASK   },
//        {   "GPIO_DATAFF",      REGPOKER_WRITE32,   GPIO_DATA+GPIO_ADDR_MASK,   GPIO_DATA_DEFAULT,      GPIO_REG_MASK   },
////      {   "GPIO_DATA00",      REGPOKER_WRITE32,   GPIO_DATA,                  GPIO_DATA_DEFAULT,      0x00            },
//        {   "GPIO_DIR",         REGPOKER_WRITE32,   GPIO_DIR,                   GPIO_DIR_DEFAULT,       0x00            },
//        {   "GPIO_IS",          REGPOKER_WRITE32,   GPIO_IS,                    GPIO_IS_DEFAULT,        GPIO_REG_MASK   },
//        {   "GPIO_IBE",         REGPOKER_WRITE32,   GPIO_IBE,                   GPIO_IBE_DEFAULT,       GPIO_REG_MASK   },
//        {   "GPIO_IEV",         REGPOKER_WRITE32,   GPIO_IEV,                   GPIO_IEV_DEFAULT,       GPIO_REG_MASK   },
//        {   "GPIO_IE",          REGPOKER_WRITE32,   GPIO_IE,                    GPIO_IE_DEFAULT,        GPIO_REG_MASK   },
//        {   "GPIO_AFSEL",       REGPOKER_WRITE32,   GPIO_AFSEL,                 GPIO_AFSEL_DEFAULT,     GPIO_REG_MASK   },
//        { /* Sentinel */ }
//    };
//
//    if( rumboot_regpoker_check_array( check_rw_array, base_addr ) == 0 ) {
//        rumboot_printf( " OK\n" );
//        return 0;
//    }
//
//    rumboot_printf( " ERROR\n" );
//    return 1;
//}
#endif


int main() {
    uint32_t result = 0x0;

    rumboot_printf( "Check HSCB (0x%x) \n", HSCB_X_BASE );

#ifdef CHECK_REGS
    result += check_hscb_default_val( HSCB_X_BASE );
    //result += check_gpio_regs( GPIO_X_BASE );
#endif
    //result += check_gpio_func( GPIO_X_BASE, 0x2A );

    return result;
}
