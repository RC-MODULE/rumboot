#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <regs/regs_tsns.h>
#include <regs/regs_gpio.h>
#include <platform/basis/platform/devices.h>
uint32_t temp;
uint32_t dat;
uint32_t result;



int count=0;
int unsigned check_max = 3795;
int unsigned check_min = 3409;


int unsigned array_gpio[6] = {241, 199, 157, 115, 73, 42};


void GPIO (uint8_t send_data, uint8_t direction, uint32_t base_gpio_addr)
{
	iowrite32(0xFF, base_gpio_addr + GPIO_SOURCE);
	iowrite32(direction, base_gpio_addr + GPIO_DIRECTION);
	iowrite32(send_data, base_gpio_addr + GPIO_WRITE_APBtoPAD);
}

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
	rumboot_printf("Initialize tempsensor");
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

int main(void)
{       
	rumboot_printf("Hello TSNS!");
	result = 0;
	initial_read_regs();
	//multiple_start_tsns();
	//rumboot_printf("Start multiple start and finish tempsensor");
	init_tsns();
	//tubef_str("Initialize tempsensor");
	
	// Check That Correct Value Got From rgTSNS_DATA after 7 50MHz Clocks
	
	for (count=0;count<6;count++)
	{
		GPIO(array_gpio[count], 0xFF, GPIO2_BASE); 
		if (count == 0)
		{
			for(int unsigned i=0;i<100;i++)
			{
				if( i & 0x000003FF )
				{
					temp= ioread32(TSNS_BASE + TSNS_DATA);
					rumboot_printf("temp = %x\n",temp);
					rumboot_printf("iteration = %x\n", i);
				}
			}
		}
		else
		{
			for(int unsigned i=0;i<20;i++)
			{
				if( i & 0x000000FF )
				{
					temp= ioread32(TSNS_BASE + TSNS_DATA);
					rumboot_printf("temp = %x\n",temp);
					rumboot_printf("iteration = %x\n", i);
				}
			}
		}	
		
		if (check_min < temp && temp < check_max)
		{
			rumboot_printf("Iteration OK");
			//rumboot_printf(count);
			continue;
		}
		else
		{
			result = 1;
			break;
		}		
	}	
  
	return result;
}
