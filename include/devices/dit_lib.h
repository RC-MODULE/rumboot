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

struct sp804_conf{
    enum sp804_mode mode;
    int  interrupt_enable;
    int  clock_division;
    int  width;
    int  load;
    int  bgload;
  
};


void sp804_enable( int index, int base_addr);                                                                             ;

void sp804_stop( int index, int base_addr);                                                                             ;

int sp804_get_value( int index, int base_addr);                                                                             ; //

void sp804_clrint( int index, int base_addr);                                                                    ;

void sp804_config( struct sp804_conf config, int index, int base_addr);


#endif
