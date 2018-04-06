#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>

#include <platform/basis/platform/devices.h>

#include <regs/regs_crg.h>

int test2;
int test3;

void read_APB_CLKDIV()
{
	int a;
	rumboot_printf("Checking read CLKDIV");
	a = ioread32(CRG_SYS_BASE + CRG_CKDIVMODE0);
	rumboot_printf("Checking read CLKDIV end");
	rumboot_printf("CLKDIV= %x", a);
}

//void send_req(int a)
//{
//  set_req(a);
//}

//void send_req(int a)
//{
//	set_req(a);

//do{
//	test2  = check_req();
//	test3 = check_ack();

	//tubef_hex(test2);
	// tubef_hex(test3);

//	if (!(test2&a))
	//	{
	//		rumboot_printf("ARM FINISHED\n");
	//	}

//}while(test3|test2);


//}

int main(void)
{
	rumboot_printf("Start CRG divider in cluster test");
	test2=0;
	test3=0;
	read_APB_CLKDIV();

	iowrite32(0x1ACCE551, CRG_SYS_BASE + CRG_WR_LOCK);
	int j = 1;
	int ch_div = 0x0000000F;
	rumboot_printf("Start changing DIVMODE");
	while(1)
	{
	  j--;
	  //tubef_str("Counter load");
	  //rumboot_printf(j);

	  if(j==0)
	    {
	      ch_div++;
		  iowrite32(ch_div, CRG_SYS_BASE + CRG_CKDIVMODE2);
		  iowrite32(0x00000001, CRG_SYS_BASE + CRG_CKUPDATE);
		  rumboot_printf("Divider has changed\n, value %d", ch_div);
		  j=1;
		}

	  if (j==1){
			udelay(50);
	    }

	  if(ch_div > 0x000003F)
	    {
		  iowrite32(0x0000000F, CRG_SYS_BASE + CRG_CKDIVMODE2);
	      rumboot_printf("CLKDIV has been changed at F");
	      break;
	    }
	}

	return -1;
}
