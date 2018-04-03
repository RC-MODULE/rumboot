//#include <platform/oi10/platform/mivem_test.h>
#include <platform/oi10/platform/ppc_c.h>
//#include <platform/oi10/platform/mivem_mem_map.h>
#include <stdbool.h>
#include <platform/oi10/platform/devices.h>
#include <platform/oi10/platform/wdOI10/wdOI10.h>


//static uint32_t wd_unlock()
//{
//    wd_write_WDOGLOCK(WD_BASE,WD_UNLOCK_CODE);
//    if((wd_read_WDOGLOCK(WD_BASE) & 0x1) == 0x0)
//        return true;
//    else
//        return false;
//}

//static uint32_t wd_lock()
//{
//    wd_write_WDOGLOCK(WD_BASE,0);//any value != WD_UNLOCK_CODE
//    if((wd_read_WDOGLOCK(WD_BASE) & 0x1) == 0x0)
//        return false;
//    else
//        return true;
//}

//uint32_t inline static wd_get_value()
//{
//    return wd_read_WDOGVALUE(WD_BASE);
//}
//
void wd_restart(uint32_t lval)
{
    //wd_unlock();
    wd_write_WDOGLOAD(WD_BASE,lval);//writing to this register restarts counting with WDOGLOAD (default 0xFFFFFFFF)
    //wd_lock();
}

void wd_clear_int()
{
    //wd_unlock();
    wd_write_WDOGINTCLR(WD_BASE,0x1);//write any value clears the interrupt and reloads the counter
    //wd_lock();
}

/*
 * enable interrupt. counter restarts if interrupt was previously disabled
 */
void wd_int_enable()
{
   //wd_unlock();
   uint32_t tmp = wd_read_WDOGCONTROL(WD_BASE);
   tmp |= (1 << INTEN);
   wd_write_WDOGCONTROL(WD_BASE,tmp);
   //wd_lock();
}

void wd_rst_enable()
{
    //wd_unlock();
    uint32_t tmp = wd_read_WDOGCONTROL(WD_BASE);
    tmp |= (1 << RESEN);
    wd_write_WDOGCONTROL(WD_BASE,tmp);
    //wd_lock();
}

void wd_disable(){
    //wd_unlock();
    wd_write_WDOGCONTROL(WD_BASE,0x0);
    //wd_lock();
}

//uint32_t inline wd_get_int_status()
//{
//    //wd_unlock();
//    return wd_read_WDOGMIS(WD_BASE);
//}

/////// Integrated test control functions

/*
 * enable test control
 */
void wd_itc_enable()
{
    //wd_unlock();
    wd_write_WDOGITCR(WD_BASE,0x1);
    //wd_lock();
}

/*
 * disable test control
 */
void wd_itc_disable()
{
    //wd_unlock();
    wd_write_WDOGITCR(WD_BASE,0);
    //wd_lock();
}

void wd_itc_set_output(uint32_t test_reset, uint32_t test_int)
{
    //wd_unlock();
    wd_write_WDOGITOP(WD_BASE,(test_reset << 0) | (test_int << 1));
    //wd_lock();
}











