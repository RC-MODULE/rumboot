
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices/nu_cpdmac_lib.h>
#include <platform/devices.h>
#include <platform/devices/nu_lib.h>
#include <regs/regs_nu_mpe.h>
#include <regs/regs_na.h>
#include <platform/devices/nu_test_macro.h> 

 int nu_mpe_rd_regs(uintptr_t rbase) {
	  int res;
	  int /*res8,res9,*/res10,/*res11,res12,*/
		  res13,res14,res34;

	  int resr;

	 
	//--------------------------MPE_MA--------------------------------------
	
	//if((0x00000001)  !=(ioread32(rbase + MPE_MA_BASE + MPE_COMMON_IN_FORMAT)& 0x00000003))	//NOT WORKS !!!!
	//	{res8 =1;
	//	rumboot_printf("MPE_COMMON_IN_FORMAT = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_IN_FORMAT));
	//	rumboot_printf("res8 error\n");}
	//	else 
	//	{res8=0;}

	//if ((0x00000000)  != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_NORM_PARAM)& 0x0003013F))
	//	{res9 =1;
	//	rumboot_printf("MPE_COMMON_NORM_PARAM = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_NORM_PARAM)); //NOT WORKS !!!! 
	//	rumboot_printf("res9 error\n");}	
	//	else 
	//	{res9=0;}	
	if ((0x00000000)  !=ioread32(rbase + MPE_MA_BASE +MPE_COMMON_OUT_SATURATION)) 	
		{res10 =1;
	    rumboot_printf("MPE_COMMON_OUT_SATURATION = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_OUT_SATURATION)); 
		rumboot_printf("Unexpected MPE_COMMON_OUT_SATURATION error\n");}	
		else 
		{res10=0;}
	
	//if ((0x00000000)  != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_IN_BUF_CFG) & 0x000001FF))
	//	{res11 =1;
	//	rumboot_printf("res11 error\n");}	
	//	else 
	//	{res11=0;}
	
//	if ((0x00000001)  != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS)& 0x0000000F))
//		{res12 =1;
//		rumboot_printf("MPE_COMMON_D_BIAS = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS)); 
//		rumboot_printf("Unexpected MPE_COMMON_D_BIAS error\n");}	
//		else 
//		{res12=0;}
	if ((0x00000000)  != ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET))
		{res13 =1;
		rumboot_printf("MPE_COMMON_BUF_FULL_SET = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET)); 
		rumboot_printf("Unexpected MPE_COMMON_BUF_FULL_SET error\n");}	
		else 
		{res13=0;}

	if ((0x00000000)  !=ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_RST))	
		{res14 =1;
		rumboot_printf("res14 error\n");}
		else 
		{res14=0;}

//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_NULL)& 0x00001FFF))
//		{res33 =1;
//		rumboot_printf("res33 error\n");}
//		else 
//		{res33=0;}	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_STATUS)& 0x000FFFFF))
		{res34 =1;
		rumboot_printf("  MPE_STATUS = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_STATUS));
		rumboot_printf("Unexpected MPE_STATUS error\n");}
		else 
		{res34=0;}
	
	  resr = 	/* res8 || res9 ||*/ res10 ||/* res11 || res12 ||*/ res13 || res14  || res34 ;	
	  res = resr ;
  
	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
			
 return 0; 
 } 

int nu_mpe_wr_rd_regs(uintptr_t rbase, int32_t data) {
 	  int res;
	  int /*res8,res9,res10,res11,res12, */
		  res13,res14;
		
	  int resr;

	 
	//--------------------------MPE_MA--------------------------------------
		

	//if((0x00000001)  !=(ioread32(rbase + MPE_MA_BASE + MPE_COMMON_IN_FORMAT)& 0x00000003))	//NOT WORKS !!!!
	//	{res8 =1;
	//	rumboot_printf("MPE_COMMON_IN_FORMAT = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_IN_FORMAT));
	//	rumboot_printf("res8 error\n");}
	//	else 
	//	{res8=0;}	
	//if ((0x00000000)  != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_NORM_PARAM)& 0x0003013F))
	//	{res9 =1;
	//	rumboot_printf("MPE_COMMON_NORM_PARAM = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_NORM_PARAM)); //NOT WORKS !!!! 
	//	rumboot_printf("res9 error\n");}	
	//	else 
	//	{res9=0;}
	
	//if ((0x00000000)  !=ioread32(rbase + MPE_MA_BASE +MPE_COMMON_OUT_SATURATION)) 	
	//	{res10 =1;
	//    rumboot_printf("MPE_COMMON_NORM_PARAM = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_OUT_SATURATION)); 
	//	rumboot_printf("Unexpected MPE_COMMON_NORM_PARAM error\n");}	
	//	else 
	//	{res10=0;}
			
	//if ((0x00000000)  != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_IN_BUF_CFG) & 0x000001FF)) //NOT WORKS !!!!
	//	{res11 =1;
	//	rumboot_printf("res11 error\n");}	
	//	else 
	//	{res11=0;}
//		iowrite32(data, rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS);
//	if ((data & 0x0000000F)  != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS)& 0x0000000F))
//		{res12 =1;
//		rumboot_printf("MPE_COMMON_D_BIAS = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS)); 
//		rumboot_printf("Unexpected MPE_COMMON_D_BIAS error\n");}	
//		else 
//		{res12=0;}
	
     
		iowrite32(data, rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET);	
		if ((data) !=((ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET))))
		{res13 =1;
		rumboot_printf("MPE_COMMON_BUF_FULL_SET = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET)); 
		rumboot_printf("Unexpected MPE_COMMON_BUF_FULL_SET error\n");}	
		else 
		{res13=0;}
		
	
		iowrite32(data, rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_RST);
	if ((0x00000000)  !=ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_RST))	
		{res14 =1;
		rumboot_printf("MPE_COMMON_BUF_FULL_RST = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_RST)); 
		rumboot_printf("Unexpected MPE_COMMON_BUF_FULL_RST error\n");}	
		else 
		{res14=0;}



	  resr = 	/*res0 || res1|| res2 || res3 ||  res4 || res5 || res6 || res7 || res8 ||
				res9 ||  res10 || res11 || res12 ||*/ res13 || res14;
	
	res = resr ;
  
	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
   return -1;}
			

 return 0; 
 } 
 
int nu_running_one(uintptr_t base, uint32_t mpe_offset,uint32_t mpe_offset_reset, uint32_t mask, int end_number )
{
    uint32_t data = 1;

	
    for ( int i = 0; i< end_number; i++)
    {

		iowrite32((data & mask)    ,(base + mpe_offset));
        if ((ioread32(base + mpe_offset) )!= (data & mask))
        {
            rumboot_printf("MPE running one ERROR data=%x\n", data);
		
           return 1;
        }
		 //rumboot_printf("MPE data_before=%x\n", data);
		iowrite32((data & mask)    ,(base + mpe_offset_reset)); 
        data = (data << 1);		
		//rumboot_printf("MPE data_after=%x\n", data);
    }
    return 0;
}
int nu_mpe_dma_rd_regs(uintptr_t rbase) {
	  int res;
	  int res0,res1, res2,res3;
	 int res20,res21, res22,res23;
	  int resr;
	  int resw;

	 
	//--------------------------MPE_RDMA D channel--------------------------------------
	
	if((0x00000000)  !=(ioread32(rbase + MPE_RDMA_D_BASE + BIASCtrl_MSha)& 0x0000003F))
		{res0 =1;
		rumboot_printf("BIASCtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_D_BASE + BIASCtrl_MSha));
		rumboot_printf("Unexpected BIASCtrl_MSha error\n");}
		else 
		{res0=0;}

	if ((0x00000000)  != (ioread32(rbase + MPE_RDMA_D_BASE + RD_THRECtrl_MSha)&  0x001FFFFF))
		{res1 =1;
		rumboot_printf("RD_THRECtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_D_BASE + RD_THRECtrl_MSha));
		rumboot_printf("Unexpected RD_THRECtrl_MSha error\n");}	
		else 
		{res1=0;}
	if((0x00000000)  !=(ioread32(rbase + MPE_RDMA_D_BASE + RD_DECCtrl_MSha)& 0x0003FFFF))
		{res2 =1;
		rumboot_printf("RD_DECCtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_D_BASE + RD_DECCtrl_MSha));
		rumboot_printf("Unexpected  RD_DECCtrl_MSha error\n");}
		else 
		{res2=0;}
	
	if ((0x00000000)  != (ioread32(rbase + MPE_RDMA_D_BASE + PADCtrl_MSha)& 0x0007FFFF))
		{res3 =1;
		rumboot_printf("PADCtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_D_BASE + PADCtrl_MSha)); 
		rumboot_printf("Unexpected PADCtrl_MSha error\n");}	
		else 
		{res3=0;}

//---------------------------------MPE_RDMA W Channel----------------------------
	if((0x00000011)  !=(ioread32(rbase + MPE_RDMA_W_BASE + X_Cfg_MSha)& 0x00030011))
		{res20 =1;
		rumboot_printf("X_Cfg_MSha = %x\n",ioread32(rbase + MPE_RDMA_W_BASE + X_Cfg_MSha));
		rumboot_printf("Unexpected X_Cfg_MSha error\n");}
		else 
		{res20=0;}

	if ((0x00000040)  != (ioread32(rbase + MPE_RDMA_W_BASE + WR_THRECtrl_MSha)& 0x000001FF))
		{res21 =1;
		rumboot_printf("WR_THRECtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_W_BASE + WR_THRECtrl_MSha));
		rumboot_printf("Unexpected MPE_RDMA_W_POINTER error\n");}	
		else 
		{res21=0;}
	if((0x00000000)  !=(ioread32(rbase + MPE_RDMA_W_BASE + WR_DECCtrl_MSha)& 0x00000007))
		{res22 =1;
		rumboot_printf("WR_DECCtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_W_BASE + WR_DECCtrl_MSha));
		rumboot_printf("Unexpected  WR_DECCtrl_MSha error\n");}
		else 
		{res22=0;}
	
	if ((0x00000005)  != (ioread32(rbase + MPE_RDMA_W_BASE + CHCfg_MSha)& 0x0000000F))
		{res23 =1;
		rumboot_printf("CHCfg_MSha = %x\n",ioread32(rbase + MPE_RDMA_W_BASE + CHCfg_MSha)); 
		rumboot_printf("Unexpected CHCfg_MSha error\n");}	
		else 
		{res23=0;}

	  resr = 	res0 || res1 || res2 || res3  ;	
	  resw = 	res20 || res21 || res22 || res23  ;
	  res = resr || resw ;
  
	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
			
 return 0; 
 } 

int nu_mpe_dma_wr_rd_regs(uintptr_t rbase, int32_t data) {
 	   int res;
	    int res0,res1, res2,res3;
		int res20,res21, res22,res23;
	  int resr;
	  int resw;
	 
//--------------------------MPE_RDMA D channel--------------------------------------
				
   		iowrite32(data, rbase + MPE_RDMA_D_BASE + BIASCtrl_MSha); 	
  	if((data & 0x0000003F)  !=(ioread32(rbase + MPE_RDMA_D_BASE + BIASCtrl_MSha)& 0x0000003F))
		{res0 =1;
		rumboot_printf("BIASCtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_D_BASE + BIASCtrl_MSha));
		rumboot_printf("Unexpected BIASCtrl_MSha error\n");}
		else 
		{res0=0;}
	
   		iowrite32(data, rbase + MPE_RDMA_D_BASE + BIASCtrl_MSha); 
	if ((data & 0x001FFFFF)  != (ioread32(rbase + MPE_RDMA_D_BASE + RD_THRECtrl_MSha)&  0x001FFFFF))
		{res1 =1;
		rumboot_printf("RD_THRECtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_D_BASE + RD_THRECtrl_MSha));
		rumboot_printf("Unexpected RD_THRECtrl_MSha error\n");}	
		else 
		{res1=0;}
	
   		iowrite32(data, rbase + MPE_RDMA_D_BASE + BIASCtrl_MSha); 	
	if((data & 0x0003FFFF)  !=(ioread32(rbase + MPE_RDMA_D_BASE + RD_DECCtrl_MSha)& 0x0003FFFF))
		{res2 =1;
		rumboot_printf("RD_DECCtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_D_BASE + RD_DECCtrl_MSha));
		rumboot_printf("Unexpected  RD_DECCtrl_MSha error\n");}
		else 
		{res2=0;}
	
   		iowrite32(data, rbase + MPE_RDMA_D_BASE + BIASCtrl_MSha); 	
	if ((data & 0x0007FFFF)  != (ioread32(rbase + MPE_RDMA_D_BASE + PADCtrl_MSha)& 0x0007FFFF))
		{res3 =1;
		rumboot_printf("PADCtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_D_BASE + PADCtrl_MSha)); 
		rumboot_printf("Unexpected PADCtrl_MSha error\n");}	
		else 
		{res3=0;}

//---------------------------------MPE_RDMA W Channel----------------------------
	   	iowrite32(data, rbase + MPE_RDMA_D_BASE + BIASCtrl_MSha); 
	if((data & 0x00030011)  !=(ioread32(rbase + MPE_RDMA_W_BASE + X_Cfg_MSha)& 0x00030011))
		{res20 =1;
		rumboot_printf("X_Cfg_MSha = %x\n",ioread32(rbase + MPE_RDMA_W_BASE + X_Cfg_MSha));
		rumboot_printf("Unexpected X_Cfg_MSha error\n");}
		else 
		{res20=0;}

	if ((data & 0x000001FF)  != (ioread32(rbase + MPE_RDMA_W_BASE + WR_THRECtrl_MSha)& 0x000001FF))
		{res21 =1;
		rumboot_printf("WR_THRECtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_W_BASE + WR_THRECtrl_MSha));
		rumboot_printf("Unexpected MPE_RDMA_W_POINTER error\n");}	
		else 
		{res21=0;}
	if((data & 0x00000007)  !=(ioread32(rbase + MPE_RDMA_W_BASE + WR_DECCtrl_MSha)& 0x00000007))
		{res22 =1;
		rumboot_printf("WR_DECCtrl_MSha = %x\n",ioread32(rbase + MPE_RDMA_W_BASE + WR_DECCtrl_MSha));
		rumboot_printf("Unexpected  WR_DECCtrl_MSha error\n");}
		else 
		{res22=0;}
	
	if ((data & 0x0000000F)  != (ioread32(rbase + MPE_RDMA_W_BASE + CHCfg_MSha)& 0x0000000F))
		{res23 =1;
		rumboot_printf("CHCfg_MSha = %x\n",ioread32(rbase + MPE_RDMA_W_BASE + CHCfg_MSha)); 
		rumboot_printf("Unexpected CHCfg_MSha error\n");}	
		else 
		{res23=0;}

	  resr = 	res0 || res1 || res2 || res3  ;	
	  resw = 	res20 || res21 || res22 || res23  ;
	  res = resr || resw ;
  
	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
			
 return 0; 
 } 

int main() {
 
  int res,res1,res2,res3,res4,res5;
  rumboot_printf("Test MPE regs run\n");
  rumboot_printf("Read REGS after reset\n");
  
	rumboot_printf("NA_CU_MPE_UNIT_MODE = %x\n",NA_CU_MPE_UNIT_MODE);
	rumboot_printf("NPE_BASE = %x\n",NPE_BASE);
	//rumboot_printf("NA_CU_REGS_BASE = %x\n",NA_CU_REGS_BASE);
	rumboot_printf("MPE_MA_BASE = %x\n",MPE_MA_BASE);
	rumboot_printf("MY_MPE_REGS_BASE = %x\n",MY_MPE_REGS_BASE);
	rumboot_printf(" \n");
	
	#if DUT_IS_NPE
	 na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_MPE_UNIT_MODE);
	#endif
   // Configure RDMA initial state
	res1 = nu_mpe_rd_regs((MY_MPE_REGS_BASE));
	 if ( res1 !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
 else
 {rumboot_printf("Check READs after reset PASSED \n");}
	rumboot_printf(" \n");
   //write zero's 
   {rumboot_printf("Read REGS after  WRITE REGs ZERO's\n");}
   res2 = nu_mpe_wr_rd_regs(MY_MPE_REGS_BASE, 0x00000000);
   
  if ( res2 !=0)
 {rumboot_printf("Test FAILED\n");
	return 1;
  }
 else
 { rumboot_printf("Check READs after write ZERO's PASSED \n");}
	rumboot_printf(" \n");
   {rumboot_printf("Read REGS after  WRITE REGs ONE's\n");} 
	  //write one's 
    res3 =  nu_mpe_wr_rd_regs(MY_MPE_REGS_BASE,  0xFFFFFFFF);

 if ( res3 !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
 }
  else
  { rumboot_printf("Read REGS after  WRITE REGs ONE's PASSED\n");};

  { rumboot_printf("Read REGS running ONE\n");}
 res4 = nu_running_one( (MY_MPE_REGS_BASE + MPE_MA_BASE),MPE_COMMON_BUF_FULL_SET,MPE_COMMON_BUF_FULL_RST,0xFFFFFFFF,32);	
	if  (res4 !=0)
	{rumboot_printf( "Check running ONE FAILED\n");
	return 1;
	}
	 else 
	{rumboot_printf( "Check 'running ONE' PASSED\n");	}
 
  {rumboot_printf("READs DMA after reset\n");} 
	res5 = nu_mpe_dma_rd_regs((MY_MPE_REGS_BASE));
	 if ( res5 !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  else
  {rumboot_printf("Check READs DMA after reset PASSED \n");}
	rumboot_printf(" \n");
 
 
  res = res1 || res2 ||res3 ||res4 || res5;

  if ( res !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  
   rumboot_printf("Test PASSED\n");

   return 0;

}

