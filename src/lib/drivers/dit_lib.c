#include <basis/defs_c.h>

    void clear_int0()
    {
        rgTIMER0_CLEAR = 1;
    }
    
    void clear_int1()
    {
        rgTIMER1_CLEAR = 1;
    }
    
    void mask_int0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL&0b11011111;
    }
    
    void mask_int1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL&0b11011111;
    }
    
    int read_raw_int0()
    {
        return rgTIMER0_RIS;
    }
    
    int read_raw_int1()
    {
        return rgTIMER1_RIS;
    }
    
    int read_mask_int0()
    {
        return rgTIMER0_MIS;
    }
    
    int read_mask_int1()
    {
        return rgTIMER1_MIS;
    }
    
    void size_32b_0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL|0b00000010;
    }
    
    void size_32b_1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL|0b00000010;
    }
    
    
    void oneshot0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL|0b00000001;
    }
    
    void oneshot1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL|0b00000001;
    }
    
    void periodic0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL&0b11011110;             // check rtl
    }
    
    void periodic1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL&0b10111110;             // check rtl
    }
    
    void freerun0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL&0b11111110;             // check rtl
        rgTIMER0_CONTROL = rgTIMER0_CONTROL|0b01000000;
    }
    
    void freerun1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL&0b11111110;             // check rtl
        rgTIMER1_CONTROL = rgTIMER1_CONTROL|0b01000000;
    }
    
    void div1_0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL&0b11110011;
    }
    
    void div1_1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL&0b11110011;
    }
    
    void div16_0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL&0b11110111;
        rgTIMER0_CONTROL = rgTIMER0_CONTROL|0b00000100;
    }
    
    void div16_1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL&0b11110111;
        rgTIMER1_CONTROL = rgTIMER1_CONTROL|0b00000100;
    }
    
    void div256_0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL&0b11111011;
        rgTIMER0_CONTROL = rgTIMER0_CONTROL|0b00001000;
    }
    
    void div256_1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL&0b11111011;
        rgTIMER1_CONTROL = rgTIMER1_CONTROL|0b00001000;
    }
    
    void load_value_0(int value)
    {
        rgTIMER0_LOAD = value;
    }
    
    void load_value_1(int value)
    {
        rgTIMER1_LOAD = value;
    }
    
    void load_bgvalue_0(int value)
    {
        rgTIMER0_BGLOAD = value;
    }
    
    void load_bgvalue_1(int value)
    {
        rgTIMER1_BGLOAD = value;
    }
    
    int check_value_0()
    {
        return rgTIMER0_VALUE;
    }
    
    int check_value_1()
    {
        return rgTIMER1_VALUE;
    }
    
    void start_0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL|0b10000000;
    }
    
    void start_1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL|0b10000000;
    }
    
    void stop_0()
    {
        rgTIMER0_CONTROL = rgTIMER0_CONTROL&0b01111111;
    }
    
    void stop_1()
    {
        rgTIMER1_CONTROL = rgTIMER1_CONTROL&0b01111111;
    }
