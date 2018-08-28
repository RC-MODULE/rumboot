 
#include <mivem_test.h>
#include <ppc_c.h>
#include <wd.h>
#include <mpic128.h>
#include <test_event_codes.h>
#include <mivem_regs_access.h>
#include "test_mpw_ctrl_005.h"
#include <trace.h>

uint32_t volatile WD_INT_HANDLED = false;

static int32_t check_array32[] = {
        0xFFFFFFFF,
        0x00000000,
        0xFFFF0000,
        0x0000FFFF,
        0xFF00FF00,
        0x00FF00FF,
        0xF0F0F0F0,
        0x0F0F0F0F,
        0xCCCCCCCC,
        0x33333333,
        0xAAAAAAAA,
        0x55555555
};

static uint32_t check_wd_default_val( uint32_t base_addr ) {
    rumboot_printf("Check the default values of the registers:");
/*
    struct regpoker_checker check_default_array[] = {
        {   "Timer1Load",       REGPOKER_READ32,    GPIO_DATA,      GPIO_DATA_DEFAULT,      GPIO_REG_MASK   }, // check only first of 256 //
        {   "Timer1Value",      REGPOKER_READ32,    GPIO_DIR,       GPIO_DIR_DEFAULT,       GPIO_REG_MASK   },
        {   "Timer1Control",    REGPOKER_READ32,    GPIO_IS,        GPIO_IS_DEFAULT,        GPIO_REG_MASK   },
        {   "Timer1IntClr",     REGPOKER_READ32,    GPIO_IBE,       GPIO_IBE_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_IEV",         REGPOKER_READ32,    GPIO_IEV,       GPIO_IEV_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_IE",          REGPOKER_READ32,    GPIO_IE,        GPIO_IE_DEFAULT,        GPIO_REG_MASK   },
        {   "GPIO_RIS",         REGPOKER_READ32,    GPIO_RIS,       GPIO_RIS_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_MIS",         REGPOKER_READ32,    GPIO_MIS,       GPIO_MIS_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_AFSEL",       REGPOKER_READ32,    GPIO_AFSEL,     GPIO_AFSEL_DEFAULT,     GPIO_REG_MASK   },
        {   "GPIO_PeriphID0",   REGPOKER_READ32,    GPIO_PeriphID0, GPIO_PeriphID0_DEFAULT, GPIO_REG_MASK   },
        {   "GPIO_PeriphID1",   REGPOKER_READ32,    GPIO_PeriphID1, GPIO_PeriphID1_DEFAULT, GPIO_REG_MASK   },
        {   "GPIO_PeriphID2",   REGPOKER_READ32,    GPIO_PeriphID2, GPIO_PeriphID2_DEFAULT, GPIO_REG_MASK   },
        {   "GPIO_PeriphID3",   REGPOKER_READ32,    GPIO_PeriphID3, GPIO_PeriphID3_DEFAULT, GPIO_REG_MASK   },
        {   "GPIO_CellID0",     REGPOKER_READ32,    GPIO_CellID0,   GPIO_CellID0_DEFAULT,   GPIO_REG_MASK   },
        {   "GPIO_CellID1",     REGPOKER_READ32,    GPIO_CellID1,   GPIO_CellID1_DEFAULT,   GPIO_REG_MASK   },
        {   "GPIO_CellID2",     REGPOKER_READ32,    GPIO_CellID2,   GPIO_CellID2_DEFAULT,   GPIO_REG_MASK   },
        {   "GPIO_CellID3",     REGPOKER_READ32,    GPIO_CellID3,   GPIO_CellID3_DEFAULT,   GPIO_REG_MASK   },
        {  }
    };
*/
    struct regpoker_checker check_default_array[] = {
          {   "TimerPeriphID0",   REGPOKER_READ8,    WD_REG_PERIPHID0,      0x05, 0xff   }, /* check only first of 256 */
          {   "TimerPeriphID1",   REGPOKER_READ8,    WD_REG_PERIPHID1,      0x18, 0xff   },
          {   "TimerPeriphID2",   REGPOKER_READ8,    WD_REG_PERIPHID2,      0x14, 0xff   },
          {   "TimerPeriphID3",   REGPOKER_READ8,    WD_REG_PERIPHID3,      0x00, 0xff   },
          {   "TimerPCellID0",    REGPOKER_READ8,    WD_REG_PCELLID0,       0x0D, 0xff   },
          {   "TimerPCellID1",    REGPOKER_READ8,    WD_REG_PCELLID1,       0xF0, 0xff   },
          {   "TimerPCellID2",    REGPOKER_READ8,    WD_REG_PCELLID2,       0x05, 0xff   },
          {   "TimerPCellID3",    REGPOKER_READ8,    WD_REG_PCELLID3,       0xB1, 0xff   },
          { /* Sentinel */ }
      };


    if( rumboot_regpoker_check_array( check_default_array, base_addr ) == 0 ) {
        rumboot_printf( " OK\n" );
        return 0;
    }

    rumboot_printf( " ERROR\n" );
    return 1;
}




uint32_t check_wd_regs()
{
   uint32_t i;
   trace_msg("check_wd_regs\n");
   COMPARE_VALUES( (wd_read_WDOGLOAD(WD_BASE)        &  WDOGLOAD_MASK),        
WDOGLOAD_VALUE,"WDOGLOAD value check failed");
   COMPARE_VALUES( (wd_read_WDOGVALUE(WD_BASE)       &  WDOGVALUE_MASK),       
WDOGVALUE_VALUE,"WDOGVALUE value check failed");
   COMPARE_VALUES( (wd_read_WDOGCONTROL(WD_BASE)     &  WDOGCONTROL_MASK),     
WDOGCONTROL_VALUE,"WDOGCONTROL value check failed");
   COMPARE_VALUES( (wd_read_WDOGRIS(WD_BASE)         &  WDOGRIS_MASK),         
WDOGRIS_VALUE,"WDOGRIS value check failed");
   COMPARE_VALUES( (wd_read_WDOGMIS(WD_BASE)         &  WDOGMIS_MASK),         
WDOGMIS_VALUE,"WDOGMIS value check failed");
   COMPARE_VALUES( (wd_read_WDOGLOCK(WD_BASE)        &  WDOGLOCK_MASK),        
WDOGLOCK_VALUE,"WDOGLOCK value check failed");
   COMPARE_VALUES( (wd_read_WDOGITCR(WD_BASE)        &  WDOGITCR_MASK),        
WDOGITCR_VALUE,"WDOGITCR value check failed");
   COMPARE_VALUES( (wd_read_WDOGPERIPHID0(WD_BASE)   &  WDOGPERIPHID0_MASK),   
WDOGPERIPHID0_VALUE,"WDOGPERIPHID0 value check failed");
   COMPARE_VALUES( (wd_read_WDOGPERIPHID1(WD_BASE)   &  WDOGPERIPHID1_MASK),   
WDOGPERIPHID1_VALUE,"WDOGPERIPHID1 value check failed");
   COMPARE_VALUES( (wd_read_WDOGPERIPHID2(WD_BASE)   &  WDOGPERIPHID2_MASK),   
WDOGPERIPHID2_VALUE,"WDOGPERIPHID2 value check failed");
   COMPARE_VALUES( (wd_read_WDOGPERIPHID3(WD_BASE)   &  WDOGPERIPHID3_MASK),   
WDOGPERIPHID3_VALUE,"WDOGPERIPHID3 value check failed");
   COMPARE_VALUES( (wd_read_WDOGPCELLID0(WD_BASE)    &  WDOGPCELLID0_MASK),    
WDOGPCELLID0_VALUE,"WDOGPCELLID0 value check failed");
   COMPARE_VALUES( (wd_read_WDOGPCELLID1(WD_BASE)    &  WDOGPCELLID1_MASK),    
WDOGPCELLID1_VALUE,"WDOGPCELLID1 value check failed");
   COMPARE_VALUES( (wd_read_WDOGPCELLID2(WD_BASE)    &  WDOGPCELLID2_MASK) ,   
WDOGPCELLID2_VALUE,"WDOGPCELLID2 value check failed");
   COMPARE_VALUES( (wd_read_WDOGPCELLID3(WD_BASE)    &  WDOGPCELLID3_MASK),    
WDOGPCELLID3_VALUE,"WDOGPCELLID3 value check failed");

   //check data bus
   //interrupt and reset are disabled by default
   for (i = 0; i< 12; i++){
           wd_write_WDOGLOAD(WD_BASE, check_array32[i]);
           COMPARE_VALUES(wd_read_WDOGLOAD(WD_BASE), check_array32[i],"WDOG DCR
data bus check failed");
       }

   wd_clear_int();//clear RIS if occurred (WDOGLOAD write)


   return true;
}

void NON_CR_INTERRUPT_HANDLER(void)
{
    MPIC128_dcr_read_MPIC128x_NCIAR0( MPICx_BASE );
    MPIC128_dcr_write_MPIC128x_NCEOI0( MPICx_BASE, 0);
    if(wd_read_WDOGITCR(WD_BASE) == 0x1)//if integrated test control is enabled
    {
        //then disable it
        wd_itc_disable();
        wd_itc_set_output(false,false);
    }
    else{
        wd_disable();
        wd_clear_int();
    }
    WD_INT_HANDLED = true;
}

uint32_t wait_wd_int_handled(){
    int t = 0;
    while(!WD_INT_HANDLED && (t++<WAIT_INT_TIMEOUT))
        {;}
    return WD_INT_HANDLED;
}

uint32_t check_wd_tcr()
{
    /*
     * set wd interrupt using ITC mode.
     * clear interrupt using ITC mode
     */
    trace_msg("check_wd_tcr\n");
    mpic128_reset( MPICx_BASE );
    mpic128_pass_through_disable( MPICx_BASE );
    //setup MPIC128 interrupt 36
    mpic128_setup_ext_interrupt( MPICx_BASE,
WD_INTERRUPT_NUMBER,MPIC128_PRIOR_1,int_sense_level,int_pol_pos,Processor0);
    SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
    ppc_noncr_int_enable();//MSR setup
    mpic128_set_interrupt_borders( MPICx_BASE, 10, 5);//MCK, CR borders
    mpic128_set_minimal_interrupt_priority_mask( MPICx_BASE, Processor0,
MPIC128_PRIOR_0);
    WD_INT_HANDLED = false;
    //mpic128 is ready for interrupt

    //enable integrated test control
    wd_itc_enable();
    //set wd interrupt, no reset
    wd_itc_set_output(false,true);
    if(wait_wd_int_handled())
        //itc is already disabled in interrupt handler
        return true;
    else
    {
        //disable integrated test control, clear value on output
        wd_itc_disable();
        wd_itc_set_output(false,false);
        return false;
    }
}

uint32_t check_wd_int()
{
    /*
     * set interrupt using WD counter (standard method)
     * clear interrupt using WD restart (clear interrupt register)
     */
    trace_msg("check_wd_int\n");
    mpic128_reset( MPICx_BASE );
    mpic128_pass_through_disable( MPICx_BASE );
    //setup MPIC128 interrupt 36
    mpic128_setup_ext_interrupt( MPICx_BASE,
WD_INTERRUPT_NUMBER,MPIC128_PRIOR_1,int_sense_edge,int_pol_pos,Processor0);
    SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
    ppc_noncr_int_enable();//MSR setup
    mpic128_set_interrupt_borders( MPICx_BASE, 10, 5);//MCK, CR borders
    mpic128_set_minimal_interrupt_priority_mask( MPICx_BASE, Processor0,
MPIC128_PRIOR_0);
    WD_INT_HANDLED = false;
    //mpic128 is ready for interrupt

    //setup WD
    wd_int_enable();
    //reload
    wd_restart(200);
    if(wait_wd_int_handled())
        return true;
    else{
        trace_msg("Wait WD_INT_HANDLED timeout\n");
        wd_clear_int();
        wd_disable();
        return false;
    }
}

uint32_t test_mpw_ctrl_005(){
    /*
     * test scenario:
     * - check APB connection
     * - generate and clear interrupts using ITC
     * - generate interrupt using WD counter and clear it using int clear
register
     */
    TEST_ASSERT(check_wd_regs() == true,"check_wd_regs failed");
    TEST_ASSERT(check_wd_tcr() == true,"check_wd_tcr failed");
    TEST_ASSERT(check_wd_int() == true,"check_wd_int failed");
    return TEST_OK;
}

DECLARE_TEST(test_mpw_ctrl_005)
