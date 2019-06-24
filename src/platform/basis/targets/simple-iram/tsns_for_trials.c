#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <regs/regs_tsns.h>
#include <platform/basis/platform/devices.h>
uint32_t temp;
uint32_t dat;
uint32_t result;

int count=0;

void initial_read_regs() 
{
	uint32_t pwdn, start, ir, im, clkdiv, lvl, data, enzcalib;
	pwdn = ioread32(TSNS_BASE + TSNS_PWDN);
	rumboot_printf("pwdn = %x\n", pwdn);
	start = ioread32(TSNS_BASE + TSNS_START);
	rumboot_printf("start = %x\n", start);
	ir = ioread32(TSNS_BASE + TSNS_IR);
	rumboot_printf("ir = %x\n", ir);
	im = ioread32(TSNS_BASE + TSNS_IM);
	rumboot_printf("im = %x\n", im);
	clkdiv = ioread32(TSNS_BASE + TSNS_CLKDIV);
	rumboot_printf("clkdiv = %x\n",clkdiv);
	lvl = ioread32(TSNS_BASE + TSNS_LEVEL);
	rumboot_printf("lvl = %x\n",lvl);
	data = ioread32 (TSNS_BASE + TSNS_DATA);
	rumboot_printf("data = %x\n", data);
	enzcalib = ioread32(TSNS_BASE + TSNS_ENZCALIB);
	rumboot_printf("enzcalib = %x\n", enzcalib);
	
	if(pwdn !=0x00000001)
	{
		result=1;
	}
	
	if(start !=0x00000000)
	{
		result=1;
	}
	
	if(ir !=0x00000000)
	{
		result=1;
	}
	
	if(im !=0x00000000)
	{
		result=1;
	}
	
	if(clkdiv !=0x00000FFF)
	{
		result=1;
	}
	
	if(lvl !=0x00000FFF)
	{
		result=1;
	}
	
	if(data !=0x00000000)
	{
		result=1;
	}
	
	if(enzcalib !=0x00000000)
	{
		result=1;
	}
}

void init_tsns()
{
	rumboot_printf("Initialize tempsensor\n");
	iowrite32(0x7CF, TSNS_BASE + TSNS_CLKDIV);
	iowrite32(0xD9F, TSNS_BASE + TSNS_LEVEL);
	iowrite32(0x0, TSNS_BASE + TSNS_PWDN);
	iowrite32(0x1, TSNS_BASE + TSNS_START);
}

void multiple_start_tsns()
{
	int start;
	for (int unsigned q=0;q<4;q++)
	{
		iowrite32(0x0, TSNS_BASE + TSNS_START);
		start = ioread32(TSNS_BASE + TSNS_START);
		for (int unsigned j=0;j<10;j++)
		{
			if (start !=0)
			{
				result = 0;
			}
		}
		
		iowrite32(0x1, TSNS_BASE + TSNS_START);
		start = ioread32(TSNS_BASE + TSNS_START);
		for (int unsigned k=0;k<10;k++)
		{
			if (start !=1)
			{
				result = 0;
			}
		}
	}
}

struct pair {
		double a, b;
	};

static struct pair lookup[] = {
		{3795, -40},
		{3787, -35},
		{3779, -30},
		{3769, -25},
		{3761, -20},
		{3751, -15},
		{3743, -10},
		{3733, -5},
		{3723,  0},
		{3713,  5},
		{3703,  10},
		{3693,  15},
		{3683,  20},
		{3673,  25},
		{3663,  30},
		{3651,  35},
		{3641,  40},
		{3629,  45},
		{3617,  50},
		{3605,  55},
		{3593,  60},
		{3581,  65},
		{3569,  70},
		{3555,  75},
		{3543,  80},
		{3529,  85},
		{3515,  90},
		{3501,  95},
		{3487, 100},
		{3471, 105},
		{3457, 110},
		{3441, 115},
		{3425, 120},
		{3409, 125}   //33
};
	
	
	
	
	
double get_temp(int adc)
{
    double max = lookup[0].a;
	int indexMax = 0;
	for(int i=0;i<34;i++)
	{
		//rumboot_printf("data = %d\n",adc);
		//rumboot_printf("max = %f\n",max);
		//rumboot_printf("indexMax = %d\n",indexMax);
		if(lookup[i].a <= max && adc <= lookup[i].a && adc != 0)
		{
			max = lookup[i].a;
			indexMax = i;
		}	
	}	
	
	double min = lookup[33].a;
	int indexMin = 0;
	for(int i=0; i<34;i++)
	{
		//rumboot_printf("data = %d\n",adc);
		//rumboot_printf("min = %f\n",min);
		//rumboot_printf("indexMin = %d\n",indexMin);
		if(lookup[i].a >= min && adc >= lookup[i].a && adc !=0)
		{
			min = lookup[i].a;
			indexMin = i;
		}
	}	
	
	double Y = (lookup[indexMin].b *((max - (double)adc) / (max - min))) + (lookup[indexMax].b *(((double)adc-min)/(max-min)));
	return Y;
};	

#include <math.h>
int main(void)
{       
	rumboot_printf("Hello TSNS!\n");
	result = 0;
	rumboot_printf("Start initial read registers tempsensor\n");
	initial_read_regs();
	
	rumboot_printf("Start multiple start and finish tempsensor\n");
	multiple_start_tsns();
	
	
	init_tsns();
	//tubef_str("Initialize tempsensor");
	  
	int data_adc;
		
	for(int unsigned i=0;i<20000;i++)
	{
		while(1)
		{
			udelay(500);
			data_adc = ioread32(TSNS_BASE + TSNS_DATA);
			double temp = get_temp(data_adc);
			int a = (int) ceil(temp);
			int b = (int) round( (ceil(temp) - temp)  * 100);
			printf("Value of temperature = %d.%d\n",a,b);
			udelay(100);
			continue;
		}
	}  
	  
	return result;
}
