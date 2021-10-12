
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices/nu_lib.h>
#include <regs/regs_na.h>
#include <platform/devices.h>

int na_cu_rd_regs() {

     int res;	 
	 int res1,res2, res3,/*res4,*/res5,res6,res7,res8,res9,res91,res92,
		 res93,res94,res95,res96,res10,res11,res12,
		 res13,res14,res15,res16,res17,res18,res19,res20,res21,res22;	 
  
		res1 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_UNITS_MODE) & 0x00000007;
		if(res1 != 0x00000000)
		{res1 =1; 
		rumboot_printf("Unexpected NA_UNITS_MODE =0x%x\n",res1);}	
		{res1 =0;} 
  
  
		res2 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_ID);
		if(res2 != 0xface2021) 
		{res2 =1;  
		rumboot_printf("Unexpected NA_ID =0x%x\n",res2);}
		{res2 =0;} 
 	
		res3 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_STAT);
		if ((0x00000000) !=(ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_STAT))& 0x000F0000) 	
		{res3 =1;
		rumboot_printf("Unexpected NA_STAT=0x%x\n",res3);}
		else 
		{res3=0;}
	/*
		res4 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_PAUSE);
		if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE +NA_PAUSE)& 0x0000000F))
		{res4 =1;
		rumboot_printf("Unexpected NA_PAUSE=0x%x\n",res4);}
		else 
		{res4=0;}
*/
	///////////////////////////////////////////////////////////////////////////////////////
	//
	// Register NA_PAUSE is not ended in RTL. The bits 16,18 19 are BROKEN as INPUTs     //
	//
	//////////////////////////////////////////////////////////////////////////////////////
		res5 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_SOFT_RESET);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_SOFT_RESET)& 0x00000001)) 
		{res5 =1;
		rumboot_printf("Unexpected NA_SOFT_RESET =0x%x\n",res5);}
		else 
		{res5=0;}
	
	res6 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_PPE_SOFT_RESET);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_PPE_SOFT_RESET)& 0x00000001))
		{res6 =1;
		rumboot_printf("Unexpected NA_PPE_SOFT_RESET =0x%x\n",res6);}
		else 
		{res6=0;}
	res7= ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_VPE_SOFT_RESET); 
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_VPE_SOFT_RESET)& 0x00000001))
		{res7 =1;
		rumboot_printf("Unexpected NA_VPE_SOFT_RESET =0x%x\n",res7);}
		else 
		{res7=0;}
	
	res8 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_SOFT_RESET);
	if((0x00000000)  !=(ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_SOFT_RESET)& 0x00000001)) 	
		{res8 =1;
		rumboot_printf("Unexpected NA_MPE_SOFT_RESET =0x%x\n",res8);}
		else 
		{res8=0;}
	
	
	res9 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_CMD_FIFO_STAT);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_CMD_FIFO_STAT)& 0x000000FF))
		{res9 =1;
		rumboot_printf("Unexpected NA_CMD_FIFO_STAT =0x%x\n",res9);}	
		else 
		{res9=0;}
	
	res91 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_D_CMD_FIFO_STAT);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_D_CMD_FIFO_STAT)& 0x000F00FF))
		{res91 =1;
		rumboot_printf("Unexpected NA_MPE_D_CMD_FIFO_STAT =0x%x\n",res91);}	
		else 
		{res91=0;}
	res92 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_W_CMD_FIFO_STAT);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_W_CMD_FIFO_STAT)& 0x000F00FF))
		{res92 =1;
		rumboot_printf("Unexpected NA_MPE_W_CMD_FIFO_STAT =0x%x\n",res92);}	
		else 
		{res92=0;}
	
		res93 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_MA_CMD_FIFO_STAT);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_MA_CMD_FIFO_STAT)& 0x000000FF))
		{res93 =1;
		rumboot_printf("Unexpected NA_MPE_MA_CMD_FIFO_STAT =0x%x\n",res93);}	
		else 
		{res93=0;}
	
		res94 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_VPE_CMD_FIFO_STAT);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_VPE_CMD_FIFO_STAT)& 0x000F00FF))
		{res94 =1;
		rumboot_printf("Unexpected NA_VPE_CMD_FIFO_STAT =0x%x\n",res94);}	
		else 
		{res94=0;}
	
		res95 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_PPE_R_CMD_FIFO_STAT);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_PPE_R_CMD_FIFO_STAT)& 0x000F00FF))
		{res95 =1;
		rumboot_printf("Unexpected NA_PPE_R_CMD_FIFO_STAT =0x%x\n",res95);}	
		else 
		{res95=0;}
	
		res95 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_PPE_W_CMD_FIFO_STAT);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_PPE_W_CMD_FIFO_STAT)& 0x000F00FF))
		{res96 =1;
		rumboot_printf("Unexpected NA_PPE_W_CMD_FIFO_STAT =0x%x\n",res96);}	
		else 
		{res96=0;}
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Register NA_CMD_FIFO_STAT is not ended in RTL. The bits 32-16; 7-0  are BROKEN as INPUTs //
	//
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	res10 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_CMD_FIFO_LEVEL);
	if ((0x000000FF)  !=(ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_CMD_FIFO_LEVEL) & 0x000000FF)) 	
		{res10 =1;
		rumboot_printf("NA_CMD_FIFO_LEVEL =0x%x\n",(ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_CMD_FIFO_LEVEL) & 0x000000FF));
		rumboot_printf("Unexpectedd NA_CMD_FIFO_LEVEL =0x%x\n",((ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_CMD_FIFO_LEVEL)) & 0x000000FF));}	
		else 
		{res10=0;}
	
	res11 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS);		
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_STATUS) &  0xFFEFFFFF))
		{res11 =1;
		rumboot_printf("Unexpected NA_INT_UNITS_STATUS =0x%x\n",res11);}	
		else 
		{res11=0;}
	
	res12 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_RESET)&  0x00FF00FF))
		{res12 =1;
		rumboot_printf("Unexpected NA_INT_UNITS_RESET =0x%x\n",res12);}
		else 
		{res12=0;}
	
	res13 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_SET);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_SET)&  0x00FF00FF))
		{res13 =1;
		rumboot_printf("Unexpected NA_INT_UNITS_SET =0x%x\n",res13);}
		else 
		{res13=0;}
	
	res14 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_MASK);
	if (( 0x00000000)  !=(ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_MASK) & 0xFFEFFFFF))	
		{res14 =1;
		rumboot_printf("Unexpected NA_INT_UNITS_MASK =0x%x\n",res14);}
		else 
		{res14=0;}

	res15 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_STATUS);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_STATUS)& 0x0000007F))
		{res15 =1;	  
		rumboot_printf("Unexpected NA_INT_STATUS =0x%x\n",res15);}
		else 
		{res15=0;}
	
	res16 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_RESET);
	//rumboot_printf("Unexpected NA_INT_RESET =0x%x\n",res16);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_RESET)&  0xFFEFFFFF))//!!!!
		{res16 =1;
		rumboot_printf("Unexpected NA_INT_RESET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_RESET));}
		else 
		{res16=0;}
		
	res17 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_SET);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_SET)&  0xFFEFFFFF))
		{res17 =1;
		rumboot_printf("Unexpected NA_INT_SET =0x%x\n",res17);}
		else 
		{res17=0;}
	
	res18 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_MASK);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_MASK)& 0x0000007F))
		{res18 =1;	  
		rumboot_printf("Unexpected NA_INT_MASK =0x%x\n",res18);}
		else 
		{res18=0;}
	
	res19 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_AXI_STATUS);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_AXI_STATUS)& 0x07FFFFFF))
		{res19 =1;
		rumboot_printf("Unexpected NQA_INT_AXI_STATUS =0x%x\n",res19);}
		else 
		{res19=0;}
	
	res20 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_AXI_RESET);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_AXI_RESET)& 0x07FFFFFF))
		{res20 =1;
		rumboot_printf("Unexpected NA_INT_AXI_RESET =0x%x\n",res20);}
		else 
		{res20=0;}
	
	res21 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_AXI_SET);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_AXI_SET)& 0x07FFFFFF))
		{res21 =1;
		rumboot_printf("Unexpected NA_INT_AXI_SET =0x%x\n",res21);}
		else 
		{res21=0;}

		res22 = ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_AXI_MASK);
  if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_AXI_MASK)& 0x07FFFFFF))
		{res22 =1;
		rumboot_printf("Unexpected NA_INT_AXI_MASK =0x%x\n",res22);}
		else 
		{res22=0;}
	
		res = res1  || res2 || res3 /*|| res4 */ ||res5  || res6  || res7  || res8  || res9 ||
			res91 || res92 || res93 || res94 || res95 || res96 || res10 ||
			res11 || res12 ||res13 || res14 ||res15 || res16 || res17  || res18 || res19 ||
			 res20 ||res21 || res22;
	
	if( res ==1) {
	 rumboot_printf("READ_REGS_CU ERROR\n");
		return -1;}
			

 return 0; 
 } 
 int na_cu_wr_rd_regs(uint32_t data) {

     int res;	 
	
		iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_UNITS_MODE);
		if((data & 0x00000007) != ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_UNITS_MODE)) 
		{rumboot_printf("Unexpected NA_UNITS_MODE =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_ID));	
		return 1;} 
  	
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_PPE_SOFT_RESET); // THERE INSERT  DELAY BEFORE READ SOFT RESET
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_VPE_SOFT_RESET);  // THERE INSERT  DELAY BEFORE READ SOFT RESET
	///////////////////////////////////////////////////////////////////////////////////////
	//
	// Register NA_PAUSE is not ended in RTL. The bits 16,18 19 are BROKEN as INPUTs     //
	//
	//////////////////////////////////////////////////////////////////////////////////////
		iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_SOFT_RESET);
		iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_MPE_SOFT_RESET);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_SOFT_RESET)& 0x00000001))
		{rumboot_printf("Unexpected NA_SOFT_RESET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_SOFT_RESET));
		return 1;}
	
	//	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_PPE_SOFT_RESET);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_PPE_SOFT_RESET)& 0x00000001))
		{rumboot_printf("Unexpected NA_PPE_SOFT_RESET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_PPE_SOFT_RESET));
		return 1;}
	
	//	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_VPE_SOFT_RESET); 
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_VPE_SOFT_RESET)& 0x00000001))		
		{rumboot_printf("Unexpected NA_VPE_SOFT_RESET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_VPE_SOFT_RESET));
		return 1;}
		
	//iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_MPE_SOFT_RESET);
	if((0x00000000)  !=(ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_SOFT_RESET)& 0x00000001)) 	
		{rumboot_printf("Unexpected NA_MPE_SOFT_RESE=0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_MPE_SOFT_RESET));
		return 1;}
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Register NA_CMD_FIFO_STAT is not ended in RTL. The bits 32-16; 7-0  are BROKEN as INPUTs //
	//
	//////////////////////////////////////////////////////////////////////////////////////////////
	
		iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_CMD_FIFO_LEVEL);
	if ((data & 0x0000007F)  !=(ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_CMD_FIFO_LEVEL) & 0x0000007F))  // why 7F	?????
		{rumboot_printf("Unexpectedd NA_CMD_FIFO_LEVEL =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_CMD_FIFO_LEVEL));
		return 1;}
	

	
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_RESET);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_RESET)&  0x00FF00FF))
		{rumboot_printf("Unexpected NA_INT_UNITS_RESET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_RESET));
		return 1;}
	
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_SET);
	if ((0x00000000)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_SET)&  0x00FF00FF))
		{rumboot_printf("Unexpected NA_INT_UNITS_SET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_SET));
		return 1;}
	
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_MASK);
	if ((data & 0xFFEFFFFF)  !=(ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_MASK)& 0xFFEFFFFF))	
		{rumboot_printf("Unexpected NA_INT_UNITS_MASK =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_UNITS_MASK));
		return 1;}
	
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_INT_RESET);
	if ((0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_RESET)&  0x0000007F))
		{rumboot_printf("Unexpected NA_INT_RESET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_RESET));
		return 1;}
		
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_INT_SET);
	if ((/*data & 0x0000007F*/0x00000000) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_SET)&  0x0000007F))
		{rumboot_printf("Unexpected NA_INT_SET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_SET));
		return 1;}
	
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + NA_INT_MASK);
	if ((data & 0x0000007F)  != (ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_MASK)& 0x0000007F))  
		{rumboot_printf("Unexpected NA_INT_MASK =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + NA_INT_MASK));
		return 1;}
	//Not finished in RTL
/*	
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + AXI_INT_RESET);
	if ((data & 0x07FFFFFF) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + AXI_INT_RESET)& 0x07FFFFFF))
		{res =1;
		rumboot_printf("Unexpected AXI_INT_RESET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + AXI_INT_RESET));}
		else 
		{res=0;}
	
	//Not finished in RTL
	
	iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + AXI_INT_SET);
	if ((data & 0x07FFFFFF) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + AXI_INT_SET)& 0x07FFFFFF))
		{res =1;
		rumboot_printf("Unexpected AXI_INT_SET =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + AXI_INT_SET));}
		else 
		{res=0;}

		iowrite32(data,NPE_BASE + NA_CU_REGS_BASE + AXI_INT_MASK);
  if ((data & 0x07FFFFFF) != (ioread32(NPE_BASE + NA_CU_REGS_BASE + AXI_INT_MASK)& 0x07FFFFFF))
		{res =1;
		rumboot_printf("Unexpected AXI_INT_MASK =0x%x\n",ioread32(NPE_BASE + NA_CU_REGS_BASE + AXI_INT_MASK));}
		else 
		{res=0;}
	*/	
/*	if( res ==1) {
	 rumboot_printf("READ_REGS_CU ERROR\n");
		return -1;} */
			

 return 0; 
 } 
int main() {
  uint32_t temp;
  int res,res1,res2,res3;
  
  rumboot_printf("Hello npe_regs\n");
  temp = ioread32(NPE_BASE + NA_VPE_BASE + NU_VPE + NU_VPE_DEV_ID);
  if(temp != 0xabcd2021) {
    rumboot_printf("Unexpected NU_VPE_DEV_ID=0x%x\n",temp);
    return 1;
  }
 
  res1 = na_cu_rd_regs();
  if (res1 !=0)
  {rumboot_printf("Test state after reset not passed\n");
	return 1;
  }
  {rumboot_printf("Test state after reset  passed\n");}
  res2 = na_cu_wr_rd_regs(0xFFFFFFFF);
  if (res2 !=0)
  {rumboot_printf("Test write-read ones not passed \n");
	return 1;
  }
  {rumboot_printf("Test state write-read ONEs passed\n");}
  
   res3 = na_cu_wr_rd_regs( 0x00000000);
  if (res3 !=0)
  {rumboot_printf("Test write-read ZEROs not passed \n");
	return 1;
  }
  {rumboot_printf("Test state write-read ZEROs passed\n");}
   res = res1 || res2;
  if (res !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  
  rumboot_printf("Test Passed\n");
  return 0;
}
