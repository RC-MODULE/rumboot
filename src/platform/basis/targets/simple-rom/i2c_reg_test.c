
//-----------------------------------------------------------------------------
//  This program is for simple testing of I2C controller
//    Test includes:
//    - checking I2C_0 registers reset values (auto)
//    - checking I2C_1 registers reset values (auto)
//    
//    Test duration (RTL): < TODO
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>

#include <platform/devices.h>
#include <regs/regs_i2c.h>

#define reset_I2C_ID            0x012C012C
#define reset_I2C_ISR           0x00000034
#define reset_I2C_IER           0x00000000
#define reset_I2C_SOFTR         0x00000000
#define reset_I2C_TRANSMIT      0x00000000
#define reset_I2C_RECEIVE       0x00000000
#define reset_I2C_CTRL          0x00000000
#define reset_I2C_NUMBER        0x00000000
#define reset_I2C_STATUS        0x0780009C
#define reset_I2C_PRESCALE      0x00000000
#define reset_I2C_FIFOFIL       0x00000000
#define reset_I2C_STAT_RST      0x00000000
#define reset_I2C_FILTR         0x0000007F

#define reset_default           0x00000000

uint32_t i2c_0_test_registers_reset ()
{
    // uint32_t rdata;
    
    for (uint32_t i = 0; i <= 3; i++)
    {
        if (ioread32 (I2C0_BASE + I2C_ID      ) != reset_I2C_ID      ) return -1;
        if (i == 0)
        {
            if (ioread32 (I2C0_BASE + I2C_ISR     ) != reset_I2C_ISR     ) return -1;
        }
        else
        {
            if (ioread32 (I2C0_BASE + I2C_ISR     ) != reset_default     ) return -1;
        }
        if (ioread32 (I2C0_BASE + I2C_IER     ) != reset_I2C_IER     ) return -1;
        if (ioread32 (I2C0_BASE + I2C_SOFTR   ) != reset_I2C_SOFTR   ) return -1;
        if (ioread32 (I2C0_BASE + I2C_TRANSMIT) != reset_I2C_TRANSMIT) return -1;
        // if (ioread32 (I2C0_BASE + I2C_RECEIVE ) != reset_I2C_RECEIVE ) return -1;    //  return X on model, so commented
        if (ioread32 (I2C0_BASE + I2C_CTRL    ) != reset_I2C_CTRL    ) return -1;
        if (ioread32 (I2C0_BASE + I2C_NUMBER  ) != reset_I2C_NUMBER  ) return -1;
        if (ioread32 (I2C0_BASE + I2C_STATUS  ) != reset_I2C_STATUS  ) return -1;
        if (ioread32 (I2C0_BASE + I2C_PRESCALE) != reset_I2C_PRESCALE) return -1;
        if (ioread32 (I2C0_BASE + I2C_FIFOFIL ) != reset_I2C_FIFOFIL ) return -1;
        if (ioread32 (I2C0_BASE + I2C_STAT_RST) != reset_I2C_STAT_RST) return -1;
        if (ioread32 (I2C0_BASE + I2C_FILTR   ) != reset_I2C_FILTR   ) return -1;
        // if (ioread32 (I2C0_BASE + 0x200       ) != reset_default     ) return -1;    // these check doesnt pass, because address
        // if (ioread32 (I2C0_BASE + 0x400       ) != reset_default     ) return -1;    //   mirroring present
        // if (ioread32 (I2C0_BASE + 0x7FC       ) != reset_default     ) return -1;    // 

        asm (
            "PUSH {R0-R7}\n\t"
            
            "LDR R0, =0x01046000\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "ADD R0, #0x4\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "LDR R0, =0x01046000\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "ADD R0, #0x4\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "LDR R0, =0x01046000\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "ADD R0, #0x4\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "POP {R0-R7}\n\t"
        );
    }
    
    return 0;
}

uint32_t i2c_1_test_registers_reset ()
{
    // uint32_t rdata;
    
    for (uint32_t i = 0; i <= 3; i++)
    {
        if (ioread32 (I2C1_BASE + I2C_ID      ) != reset_I2C_ID      ) return -1;
        if (i == 0)
        {
            if (ioread32 (I2C1_BASE + I2C_ISR     ) != reset_I2C_ISR     ) return -1;
        }
        else
        {
            if (ioread32 (I2C1_BASE + I2C_ISR     ) != reset_default     ) return -1;
        }
        if (ioread32 (I2C1_BASE + I2C_IER     ) != reset_I2C_IER     ) return -1;
        if (ioread32 (I2C1_BASE + I2C_SOFTR   ) != reset_I2C_SOFTR   ) return -1;
        if (ioread32 (I2C1_BASE + I2C_TRANSMIT) != reset_I2C_TRANSMIT) return -1;
        // if (ioread32 (I2C1_BASE + I2C_RECEIVE ) != reset_I2C_RECEIVE ) return -1;    //  return X on model, so commented
        if (ioread32 (I2C1_BASE + I2C_CTRL    ) != reset_I2C_CTRL    ) return -1;
        if (ioread32 (I2C1_BASE + I2C_NUMBER  ) != reset_I2C_NUMBER  ) return -1;
        if (ioread32 (I2C1_BASE + I2C_STATUS  ) != reset_I2C_STATUS  ) return -1;
        if (ioread32 (I2C1_BASE + I2C_PRESCALE) != reset_I2C_PRESCALE) return -1;
        if (ioread32 (I2C1_BASE + I2C_FIFOFIL ) != reset_I2C_FIFOFIL ) return -1;
        if (ioread32 (I2C1_BASE + I2C_STAT_RST) != reset_I2C_STAT_RST) return -1;
        if (ioread32 (I2C1_BASE + I2C_FILTR   ) != reset_I2C_FILTR   ) return -1;
        // if (ioread32 (I2C1_BASE + 0x200       ) != reset_default     ) return -1;    // these check doesnt pass, because address
        // if (ioread32 (I2C1_BASE + 0x400       ) != reset_default     ) return -1;    //   mirroring present
        // if (ioread32 (I2C1_BASE + 0x7FC       ) != reset_default     ) return -1;    // 

        asm (
            "PUSH {R0-R7}\n\t"
            
            "LDR R0, =0x01047000\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "ADD R0, #0x4\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "LDR R0, =0x01047000\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "ADD R0, #0x4\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "LDR R0, =0x01047000\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "ADD R0, #0x4\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            "LDMIA R0!, {R1, R2, R3, R4}\n\t"
            
            "POP {R0-R7}\n\t"
        );
    }
    
    return 0;
}

//-------------------------------------------------------------------------
//  Test sequence
//-------------------------------------------------------------------------
uint32_t main ()
{
    if (i2c_0_test_registers_reset () != 0)
        return -1;

    if (i2c_1_test_registers_reset () != 0)
        return -2;

    return 0;
}

