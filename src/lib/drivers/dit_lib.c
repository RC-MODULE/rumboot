#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>
#include <devices/dit.h>
#include <rumboot/dit_lib.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>


#define DIT_CTRL_ENABLE     1<<7
#define DIT_CTRL_PERIODIC   1<<6 
#define DIT_CTRL_INTEN      1<<5
#define DIT_CTRL_DIV1       1<<3
#define DIT_CTRL_DIV0       1<<2
#define DIT_CTRL_SIZE32     1<<1
#define DIT_CTRL_ONESHOT    1


void oneshot_n_cycles_0(int cycles, int count_to)
{
    int cntrl ;
    cntrl = DIT_CTRL_ENABLE|(DIT_CTRL_INTEN&~DIT_CTRL_DIV1)|DIT_CTRL_DIV0|DIT_CTRL_SIZE32|DIT_CTRL_ONESHOT;

    iowrite8(cntrl,DIT0_BASE+DIT0_REG_CONTROL0);
    for (int i = 0; i < cycles; i++)
    {
        iowrite32(count_to,DIT0_BASE+DIT0_REG_LOAD0);
        while (ioread32(DIT0_BASE+DIT0_REG_VALUE0)){};
        rumboot_printf("Timer 0 Cycle# %d \n",i);
    };
    iowrite32(cntrl&~(DIT_CTRL_ENABLE),DIT0_BASE+DIT0_REG_CONTROL0);
}

void oneshot_n_cycles_1(int cycles, int count_to)
{
    int cntrl ;
    cntrl = DIT_CTRL_ENABLE|(DIT_CTRL_INTEN&~DIT_CTRL_DIV1)|DIT_CTRL_DIV0|DIT_CTRL_SIZE32|DIT_CTRL_ONESHOT;

    iowrite8(cntrl,DIT0_BASE+DIT0_REG_CONTROL1);
    for (int i = 0; i < cycles; i++)
    {
        iowrite32(count_to,DIT0_BASE+DIT0_REG_LOAD1);
        while (ioread32(DIT0_BASE+DIT0_REG_VALUE1)){rumboot_printf("Value = %d \n",ioread32(DIT0_BASE+DIT0_REG_VALUE1 ));};
        rumboot_printf("Timer 1 Cycle# %d \n",i);
    };
    iowrite32(cntrl&~(DIT_CTRL_ENABLE),DIT0_BASE+DIT0_REG_CONTROL1);
}


void freerun_0(int optional_load)
{
    int cntrl ;
    cntrl = (DIT_CTRL_ENABLE&~DIT_CTRL_PERIODIC)|((DIT_CTRL_INTEN&~DIT_CTRL_DIV1)&~DIT_CTRL_DIV0)|(DIT_CTRL_SIZE32&~DIT_CTRL_ONESHOT);

    iowrite8(cntrl,DIT0_BASE+DIT0_REG_CONTROL0);
    if (optional_load)
        iowrite32(optional_load,DIT0_BASE+DIT0_REG_LOAD0);
    while (ioread32(DIT0_BASE+DIT0_REG_VALUE0)){rumboot_printf("Value = %d \n",ioread32(DIT0_BASE+DIT0_REG_VALUE0 ));};

}

void freerun_1(int optional_load)
{
    int cntrl ;
    cntrl = (DIT_CTRL_ENABLE&~DIT_CTRL_PERIODIC)|((DIT_CTRL_INTEN&~DIT_CTRL_DIV1)&~DIT_CTRL_DIV0)|(DIT_CTRL_SIZE32&~DIT_CTRL_ONESHOT);

    iowrite8(cntrl,DIT0_BASE+DIT0_REG_CONTROL1);
    if (optional_load)
        iowrite32(optional_load,DIT0_BASE+DIT0_REG_LOAD1);
    while (ioread32(DIT0_BASE+DIT0_REG_VALUE1)){rumboot_printf("Value = %d \n",ioread32(DIT0_BASE+DIT0_REG_VALUE1 ));};

}


void periodic_0(int load, int optional_bgload)
{
    int cntrl ;
    cntrl = (DIT_CTRL_ENABLE|DIT_CTRL_PERIODIC)|((DIT_CTRL_INTEN&~DIT_CTRL_DIV1)&~DIT_CTRL_DIV0)|(DIT_CTRL_SIZE32&~DIT_CTRL_ONESHOT);

    iowrite8(cntrl,DIT0_BASE+DIT0_REG_CONTROL0);
    iowrite32(load,DIT0_BASE+DIT0_REG_LOAD0);
    if (optional_bgload)
        iowrite32(optional_bgload,DIT0_BASE+DIT0_REG_BGLOAD0);
    while (ioread32(DIT0_BASE+DIT0_REG_VALUE0)){rumboot_printf("Value = %d \n",ioread32(DIT0_BASE+DIT0_REG_VALUE0 ));};

}

void periodic_1(int load, int optional_bgload)
{
    int cntrl ;
    cntrl = (DIT_CTRL_ENABLE|DIT_CTRL_PERIODIC)|((DIT_CTRL_INTEN&~DIT_CTRL_DIV1)&~DIT_CTRL_DIV0)|(DIT_CTRL_SIZE32&~DIT_CTRL_ONESHOT);

    iowrite8(cntrl,DIT0_BASE+DIT0_REG_CONTROL1);
    iowrite32(load,DIT0_BASE+DIT0_REG_LOAD1);
    if (optional_bgload)
        iowrite32(optional_bgload,DIT0_BASE+DIT0_REG_BGLOAD1);
    while (ioread32(DIT0_BASE+DIT0_REG_VALUE1)){rumboot_printf("Value = %d \n",ioread32(DIT0_BASE+DIT0_REG_VALUE1 ));};

}










