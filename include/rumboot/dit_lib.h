#ifndef DIT_LIB_H
#define DIT_LIB_H

#define DIT_CTRL_ENABLE     (1<<7)
#define DIT_CTRL_PERIODIC   (1<<6) 
#define DIT_CTRL_INTEN      (1<<5)
#define DIT_CTRL_DIV1       (1<<3)
#define DIT_CTRL_DIV0       (1<<2)
#define DIT_CTRL_SIZE32     (1<<1)
#define DIT_CTRL_ONESHOT    1//static uint32_t config (int control, int inten, int clkdiv, int size32)                         ; 


enum sp804_mode{
    ONESHOT,
    PERIODIC,
    FREERUN
};

/**
 *  sp804_conf         : Structure contains configuration parameters
 *  sp804_mode         - Chose counting mode - Oneshot, Periodic or Freerun
 *  interrupt_enable   - Interrupts enabled
 *  width              - Width of the counter - 32 or 16
 *  load               - Load value to count from (won't be writen to corresponding reg if zero ) 
 *  bgload             - Background Load value to count from (won't be writen to corresponding reg if zero ) 
 */
struct sp804_conf{
    enum sp804_mode mode;
    int  interrupt_enable;
    int  clock_division;
    int  width;
    int  load;
    int  bgload;
  
};


void sp804_enable( int base_addr, int index);                                                         

void sp804_stop( int base_addr, int index);                                                           

int sp804_get_value( int base_addr, int index);                                                       

void sp804_clrint( int base_addr, int index);                                                         

void sp804_config( uint32_t base_addr, const struct sp804_conf * config, int index);


#endif
