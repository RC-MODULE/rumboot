#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>

#include <platform/basis/platform/devices.h>

#include <regs/regs_crg.h>

void read_APB_CLKDIV()
{
	int a;
	rumboot_printf("Checking read PLL PRDIV");
	a = ioread32(CRG_SYS_BASE + CRG_PLL_PRDIV);
	//rumboot_printf("Checking read CLKDIV end");
	rumboot_printf("CLKDIV= %x", a);
}

void change_pll(int fbdiv, int prdiv, int psdiv)
{
	rumboot_printf("Change settings PLL\n");
	iowrite32(fbdiv, CRG_SYS_BASE + CRG_PLL_FBDIV);
	iowrite32(prdiv, CRG_SYS_BASE + CRG_PLL_PRDIV);
	iowrite32(psdiv, CRG_SYS_BASE + CRG_PLL_PSDIV);
	rumboot_printf("Changes is done\n");
}

void change_mode_pll(int mode)
{
	rumboot_printf("Change mode of PLL\n");
	iowrite32(mode, CRG_SYS_BASE + CRG_PLL_CTRL);
	rumboot_printf("Mode of PLL was changed\n");

}

void change_divmode(int ch_div)
{
	iowrite32(ch_div, CRG_SYS_BASE + CRG_CKDIVMODE0);
	iowrite32(0x00000001, CRG_SYS_BASE + CRG_CKUPDATE);
}


void wait_ready_pll()
{
	int pll_r, pll_ch;
	do{
		pll_r = ioread32(CRG_SYS_BASE + CRG_PLL_STAT);
		pll_ch = pll_r  & 0xFFFFFFEF;
		rumboot_printf("reg pll_r= %x\n", pll_ch);
		if(pll_ch == 0x0)
			rumboot_printf("Wait ready from PLL\n");
		else
		if(pll_ch == 0x1){
			break;
		}
	} while(1);
	rumboot_printf("PLL is ready, next iteration\n");
}

int main(void)
{
	rumboot_printf("Start CRG PLL reconfiguration test\n");
	iowrite32(0x1ACCE551, CRG_SYS_BASE + CRG_WR_LOCK);


	rumboot_printf("Changing PRDIV and PSDIV\n");

	change_mode_pll(0x3);
	change_pll(499, 3, 0);	//Change freq 16*(500/4*1)=2000MHz , PRDIV =4, PSDIV=1;
	change_divmode(0x3);	//Change freq 2000 / 4 = 500MHz
	change_mode_pll(0x0);
	wait_ready_pll();
	
	//change_mode_pll(0x3);
	//change_pll(1119, 7, 0);		//Change freq 16*(1120/8*1)=2240Mhz , PRDIV =8, PSDIV=1;
	//change_mode_pll(0x0);
	//wait_ready_pll();
	
	//change_mode_pll(0x3);
	//change_pll(779, 5, 0);		//Change freq 16*(780/6*1)=2080Mhz , PRDIV =6, PSDIV=1;
	//change_mode_pll(0x0);
	//wait_ready_pll();

	change_mode_pll(0x3);
	change_pll(299, 2, 0);		//Change freq 16*(300/3*1)=1600Mhz , PRDIV =3, PSDIV=1;
	change_divmode(0x3);		//Change freq 1600 / 4 = 400MHz
	change_mode_pll(0x0);
	wait_ready_pll();
	
	//change_mode_pll(0x3);
	//change_pll(359, 2, 0);		//Change freq 16*(360/1*3)=1920Mhz , PRDIV =1, PSDIV=3;
	//change_mode_pll(0x0);
	//wait_ready_pll();
	
	change_mode_pll(0x3);
	change_pll(224, 1, 0);		//Change freq 16*(225/2*1)=1800Mhz , PRDIV =2, PSDIV=1;
	change_divmode(0x8);		//Change freq 1800 / 9 = 200MHz
	change_mode_pll(0x0);
	wait_ready_pll();

	//change_mode_pll(0x3);		//Change freq default value;
	//change_pll(99, 0, 0);
	//change_mode_pll(0x0);
	//wait_ready_pll();

	return 0;
}
