#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <regs/regs_com.h>
#include <platform/devices.h>
#include <devices/com_simple.h>


#ifdef __PPC__
#include <arch/ppc/arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc/arch/ppc_476fp_mmu.h>
#endif

#ifdef __PPC__
static struct tlb_entry const em2_nospeculative_tlb_entries[] =
{
/*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
    {MMU_TLB_ENTRY(  0x020,  0xC0304,    0xC0304,    MMU_TLBE_DSIZ_1GB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_LITTLE_END,  0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_UND,   0b1 )}
};
#endif


com_cfg_t cfg = {
  .MainCounter_WR   	=0x0,
  .Address_WR       	=(uint32_t)0x0,
  .Bias_WR          	=0x0,
  .RowCounter_WR    	=0x0,
  .AddressMode_WR   	=ONE_DIMENSION,  
  .InterruptMask_WR 	=0x0,
  
  .MainCounter_RD   	=0x0,  
  .Address_RD      	=(uint32_t)0x0,
  .Bias_RD          	=0x0,
  .RowCounter_RD    	=0x0,
  .AddressMode_RD   	=ONE_DIMENSION,     
  .InterruptMask_RD	=0x0
};

int main()
{
	uint32_t result =0;
	uint32_t tmp =0x1;

#ifdef __PPC__ 
  write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
 #endif 
 	
	rumboot_printf("Start  check COMMPORT regs\n");	
	rumboot_printf("Check COMMPORT status after reset\n");
	
   tmp = ioread32(COM0_BASE+CSR_tr);
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT0_Cpl=%d\n",  tmp );
	return 1;
	}
	
	tmp =ioread32(COM1_BASE+CSR_tr);	
	
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT1_Cpl=%d\n",  tmp );
	return 1;
	}
	tmp =ioread32(COM0_BASE+CSR_rcv);		
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT0_Cpl=%d\n",  tmp );
	return 1;
	}	
	tmp = ioread32(COM1_BASE+CSR_rcv);
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT1_Cpl=%d\n",  tmp );
	return 1;
	}

	
	tmp = com_simple_write((uintptr_t)COM0_BASE,&cfg);
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT0_CFG=%d\n",  tmp );
	return 1;
	}
	tmp =com_apb_check((uintptr_t)COM0_BASE, &cfg);
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT0_check=%d\n",  tmp );
	return 1;
	}
	
	 rumboot_printf("Read COMMPORT0 after WRITE\n");
	 
	cfg.MainCounter_WR = 0xffff; 
	cfg.Address_WR = 0xfffffff8;
	cfg.Bias_WR = 0xfff8;
	cfg.RowCounter_WR = 0xffff;
	cfg.AddressMode_WR =0x1;
	cfg.InterruptMask_WR = 0x3;
	
	cfg.MainCounter_RD = 0xffff; 
	cfg.Address_RD = 0xfffffff8;
	cfg.Bias_RD = 0xfff8;
	cfg.RowCounter_RD = 0xffff;
	cfg.AddressMode_RD =0x1;
	cfg.InterruptMask_RD = 0x3;
	
	tmp =com_apb_check((uintptr_t)COM0_BASE, &cfg);
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT0_check=%d",  tmp );
	return 1;
	}
 	tmp = com_simple_write((uintptr_t)COM1_BASE,&cfg);
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT1_CFG=%d",  tmp );
	return 1;
	} 
	tmp =com_apb_check((uintptr_t)COM0_BASE, &cfg);
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT0_check=%d",  tmp );
	return 1;
	}
	 rumboot_printf("Read COMMPORT1 after WRITE\n");
	 
	cfg.MainCounter_WR = 0xffff; 
	cfg.Address_WR = 0xfffffff8;
	cfg.Bias_WR = 0xfff8;
	cfg.RowCounter_WR = 0xffff;
	cfg.AddressMode_WR =0x1;
	cfg.InterruptMask_WR = 0x3;
	
	cfg.MainCounter_RD = 0xffff; 
	cfg.Address_RD = 0xfffffff8;
	cfg.Bias_RD = 0xfff8;
	cfg.RowCounter_RD = 0xffff;
	cfg.AddressMode_RD =0x1;
	cfg.InterruptMask_RD = 0x3;
	
	tmp =com_apb_check((uintptr_t)COM1_BASE, &cfg);
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT1_check=%d",  tmp );
	return 1;
	}
 	tmp = com_simple_write((uintptr_t)COM1_BASE,&cfg);
	if  (tmp !=0)
	{	
	rumboot_printf( "COMMPORT1_CFG=%d",  tmp );
	return 1;
	} 
 
	tmp = com_apb_running_one( COM0_BASE, Address_tr, 0xfffffff8,32);
	if  (tmp !=0)
	{
	rumboot_printf( "COMMPORT0_running_one_ERROR\n");	
	rumboot_printf( "COM1_Address_tr_data=%d",  tmp );
	return 1;
	} 
	tmp = com_apb_running_one( COM0_BASE, MainCounter_tr, 0xFFFF,16);
	if  (tmp !=0)
	{
	rumboot_printf( "COMMPORT0_running_one_ERROR\n");	
	rumboot_printf( "COM1_Address_tr_data=%d",  tmp );
	return 1;
	} 
	tmp = com_apb_running_one( COM0_BASE, Address_rcv, 0xfffffff8,32);
	if  (tmp !=0)
	{
	rumboot_printf( "COMMPORT0_running_one_ERROR\n");	
	rumboot_printf( "COM1_Address_tr_data=%d",  tmp );
	return 1;
	} 
	tmp = com_apb_running_one( COM0_BASE, MainCounter_rcv,  0xFFFF,16);
	if  (tmp !=0)
	{
	rumboot_printf( "COMMPORT0_running_one_ERROR\n");	
	rumboot_printf( "COM1_Address_tr_data=%d",  tmp );
	return 1;
	} 

	tmp = com_apb_running_one( COM1_BASE, Address_tr,0xfffffff8,32);
	if  (tmp !=0)
	{
	rumboot_printf( "COMMPORT0_running_one_ERROR\n");	
	rumboot_printf( "COM1_Address_tr_data=%d",  tmp );
	return 1;
	} 
	tmp = com_apb_running_one( COM1_BASE, MainCounter_tr, 0xFFFF,16);
	if  (tmp !=0)
	{
	rumboot_printf( "COMMPORT0_running_one_ERROR\n");	
	rumboot_printf( "COM1_Address_tr_data=%d",  tmp );
	return 1;
	} 
	tmp = com_apb_running_one( COM1_BASE, Address_rcv,0xfffffff8,32);
	if  (tmp !=0)
	{
	rumboot_printf( "COMMPORT0_running_one_ERROR\n");	
	rumboot_printf( "COM1_Address_tr_data=%d",  tmp );
	return 1;
	} 
	tmp = com_apb_running_one( COM1_BASE, MainCounter_rcv, 0xFFFF,16);
	if  (tmp !=0)
	{
	rumboot_printf( "COMMPORT0_running_one_ERROR\n");	
	rumboot_printf( "COM1_Address_rcv_data=%d",  tmp );
	return 1;
	} 
	
	{
	rumboot_printf( "Check COMMPORTs Interrupt masks\n");	
	iowrite32(0x1,COM0_BASE+InterruptMask_tr);
	tmp = ioread32(COM0_BASE+InterruptMask_tr);
	if  (tmp !=1)
	{	
	rumboot_printf( "COM0_InterruptMask_1_tr=%d",  tmp );
	return 1;
	} 
	iowrite32(0x2,COM0_BASE+InterruptMask_tr);
	tmp = ioread32(COM0_BASE+InterruptMask_tr);
	if  (tmp !=0x2)
	{	
	rumboot_printf( "COM0_InterruptMask_2_tr=%d",  tmp );
	return 1;
	}
	
	iowrite32(0x1,COM0_BASE+InterruptMask_rcv);
	tmp = ioread32(COM0_BASE+InterruptMask_rcv);
	if  (tmp !=1)
	{	
	rumboot_printf( "COM0_InterruptMask_1_rcv=%d",  tmp );
	return 1;
	} 
	iowrite32(0x2,COM0_BASE+InterruptMask_rcv);
	tmp = ioread32(COM0_BASE+InterruptMask_rcv);
	if  (tmp !=0x2)
	{	
	rumboot_printf( "COM0_InterruptMask_2_rcv=%d",  tmp );
	return 1;
	}	

	
	iowrite32(0x1,COM1_BASE+InterruptMask_tr);
	tmp = ioread32(COM1_BASE+InterruptMask_tr);
	if  (tmp !=1)
	{	
	rumboot_printf( "COM1_InterruptMask_1_tr=%d",  tmp );
	return 1;
	} 
	iowrite32(0x2,COM1_BASE+InterruptMask_tr);
	tmp = ioread32(COM1_BASE+InterruptMask_tr);
	if  (tmp !=0x2)
	{	
	rumboot_printf( "COM1_InterruptMask_2_tr=%d",  tmp );
	return 1;
	}
	
	iowrite32(0x1,COM1_BASE+InterruptMask_rcv);
	tmp = ioread32(COM1_BASE+InterruptMask_rcv);
	if  (tmp !=1)
	{	
	rumboot_printf( "COM1_InterruptMask_1_rcv=%d",  tmp );
	return 1;
	} 
	iowrite32(0x2,COM1_BASE+InterruptMask_rcv);
	tmp = ioread32(COM1_BASE+InterruptMask_rcv);
	if  (tmp !=0x2)
	{	
	rumboot_printf( "COM1_InterruptMask_2_rcv=%d",  tmp );
	return 1;
	}	
	}

  return result;
}

