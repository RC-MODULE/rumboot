
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <basis/defs_c.h>

    void clear_int0();

    
    void clear_int1();

    
    void mask_int0();

    
    void mask_int1();

    
    int read_raw_int0();

    
    int read_raw_int1();

    
    int read_mask_int0();

    
    int read_mask_int1();

    
    void size_32b_0();

    
    void size_32b_1();
    
    
    void oneshot0();

    
    void oneshot1();

    
    void periodic0();

    
    void periodic1();

    
    void freerun0();

    
    void freerun1();

    
    void div1_0();

    
    void div1_1();

    
    void div16_0();

    
    void div16_1();

    
    void div256_0();

    
    void div256_1();

    
    void load_value_0(int value);

    
    void load_value_1(int value);

    
    void load_bgvalue_0(int value);

    
    void load_bgvalue_1(int value);

    
    int check_value_0();

    
    int check_value_1();

    
    void start_0();

    
    void start_1();

    
    void stop_0();

    
    void stop_1();
#endif
