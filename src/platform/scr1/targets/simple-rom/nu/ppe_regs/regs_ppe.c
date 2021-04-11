
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices.h>
#include <platform/devices/nu_lib.h>
#include <regs/regs_nu_ppe.h>

//ConfigPPE cfg;
ConfigREGPPE cfg_regs = {0};

 int nu_ppe_rd_regs(uintptr_t rbase,uintptr_t wbase, ConfigREGPPE* cfg) {
	  int res;
	  int res0,res1,res2,res3,res4,res5,res7,res8,res9,res10,res11,res12,
		  res13,res14,res15;
	  int resw0,resw1,resw2,resw3,resw4,resw5,resw6,resw7,resw8,resw9,resw10,resw11,resw12,
		  resw13,resw14,resw15,resw16,resw17,resw18,resw19,resw20,resw21,resw22,resw23,resw24,
		  resw25,resw26,resw27,resw28,resw29,resw30,resw31,resw32,resw33,resw34, resw35 ;
	  int resr;
	  int resw;
	  int reswro;
	  int reswro1,reswro2,reswro3;
	
	if ((0x00000000) !=(ioread32(rbase +NU_PPE_RDMA_STATUS) & 0x00000003)) 	
	{res0 =1;
	rumboot_printf("res0 error\n");}	
		else 
		res0=0;	

	if ((cfg-> rOpEn) !=(ioread32(rbase +NU_PPE_RDMA_OP_ENABLE)& 0x00000001)) 	
	{res1 =1;
	rumboot_printf("res1 error\n");}
		else 
		res1=0;	
	if ((cfg-> rWi) != ioread32(rbase +NU_PPE_RDMA_DATA_W_IN))
	{res2 =1;
	rumboot_printf("res2 error\n");}
		else 
		res2=0;	
	if ((cfg->rHi) !=ioread32(rbase +NU_PPE_RDMA_DATA_H_IN))	
		{res3 =1;
		rumboot_printf("res3 error\n");}
		else 
		res3=0;	

	if ((cfg->rCi) != ioread32(rbase + NU_PPE_RDMA_DATA_C_IN))
		{res4 =1;
		rumboot_printf("res4 error\n");}
		else 
		res4=0;	
	if ((cfg->rBALs) != ioread32(rbase + NU_PPE_RDMA_SRC_BASE_ADDR_L))
		{res5 =1;
		rumboot_printf("res5 error\n");}
		else 
		res5=0;
	
//	if ((cfg->rBAHs) != ioread32(rbase + NU_PPE_RDMA_SRC_BASE_ADDR_H))
//		res6=1;
//		else 
//		res6=0;
	
	if ((cfg->rVSs) !=ioread32(rbase +NU_PPE_RDMA_SRC_VECTOR_STRIDE)) 	
		{res7 =1;
		rumboot_printf("res7 error\n");}
		else 
		res7=0;	
	if ((cfg->rLSs) != ioread32(rbase + NU_PPE_RDMA_SRC_LINE_STRIDE))
		{res8 =1;
		rumboot_printf("res8 error\n");}	
		else 
		res8=0;	
	if ((cfg->rESs) !=ioread32(rbase +NU_PPE_RDMA_SRC_ELEMENT_STRIDE)) 	
		{res9 =1;
		rumboot_printf("res9 error\n");}	
		else 
		res9=0;
			
	if ((cfg->rOpM) != (ioread32(rbase + NU_PPE_RDMA_OP_MODE)& 0x0060FF00) )
		{res10 =1;
		rumboot_printf("res10 error\n");}	
		else 
		res10=0;
	
	if ((cfg->rBSWi) != (ioread32(rbase + NU_PPE_RDMA_BOX_SIZE_W_IN)& 0x00001FFF))
		{res11 =1;
		rumboot_printf("res11 error\n");}
		else 
		res11=0;
	if ((cfg->rBSHi) != (ioread32(rbase + NU_PPE_RDMA_BOX_SIZE_H_IN)& 0x00001FFF))
		{res12 =1;
		rumboot_printf("res12 error\n");}
		else 
		res12=0;

	if ((cfg->rBSCi) !=(ioread32(rbase +NU_PPE_RDMA_BOX_SIZE_C_IN)& 0x00001FFF))	
		{res13 =1;
		rumboot_printf("res13 error\n");}
		else 
		{res13=0;}


	if ((cfg->rStWi) != (ioread32(rbase + NU_PPE_RDMA_BOX_START_W_IN)& 0x00001FFF))
		{res14 =1;
	  
		rumboot_printf("res14 error\n");}
		else 
		{res14=0;}	
	if ((cfg->rOfWi) != (ioread32(rbase + NU_PPE_RDMA_BOX_OFFSET_W_IN)& 0x00001FFF))
		{res15 =1;
		rumboot_printf("res15 error\n");}
		else 
		res15=0;

//	if ((cfg->rK) != (ioread32(rbase + NU_PPE_RDMA_KERNEL)& 0x000F0007))
//		{res16 =1;
//		rumboot_printf("res16 error\n");}
//		else 
//		res16=0;

//-----------------------------------------------------------------
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_STATUS)& 0x00000003)) 	
		{resw0 =1;
		rumboot_printf("resw0 error\n");}
		else 
		resw0=0;	  
	if ((cfg->wOpEn) !=(ioread32(wbase +NU_PPE_OP_ENABLE)& 0x00000001)) 	
		{resw1 =1;
		rumboot_printf("resw1 error\n");}	
		else 
		resw1=0;	
	if ((cfg->wWi) != (ioread32(wbase +NU_PPE_DATA_W_IN)& 0x00001FFF))
		{resw2 =1;
		rumboot_printf("resw2 error\n");}
		else 
		resw2=0;	
	if ((cfg->wHi) !=(ioread32(wbase +NU_PPE_DATA_H_IN)& 0x00001FFF)) 	
		{resw3 =1;
		rumboot_printf("resw3 error\n");}
		else 
		resw3=0;

	if ((cfg->wCi) != (ioread32(wbase + NU_PPE_DATA_C_IN)& 0x00001FFF))
		{resw4 =1;
		rumboot_printf("resw4 error\n");}
		else 
		resw4=0;
	
	
	if ((cfg->wWo) != ioread32(wbase + NU_PPE_DATA_W_OUT))
		{resw5 =1;
		rumboot_printf("resw5 error\n");}
		else 
		resw5=0;
	if ((cfg->wHo) != ioread32(wbase + NU_PPE_DATA_H_OUT))
		{resw6=1;
		rumboot_printf("resw6 error\n");}
		else 
		resw6=0;
	if ((cfg->wCo) != ioread32(wbase + NU_PPE_DATA_C_OUT))
		{resw7=1;
		rumboot_printf("resw7 error\n");}
		else 
		resw7=0;
	if ((cfg->wBALd) != ioread32(wbase + NU_PPE_DST_BASE_ADDR_L))
		{resw8=1;
		rumboot_printf("resw8 error\n");}
		else 
		resw8=0;
//	if ((cfg->wBAHd) != ioread32(wbase + NU_PPE_DST_BASE_ADDR_H))
//		resw9=1;
//		else 
//		resw9=0;
	if ((cfg->wVSd) !=ioread32(wbase +NU_PPE_DST_VECTOR_STRIDE)) 	
		{resw9 =1;
		rumboot_printf("resw9 error\n");}
		else 
		resw9=0;	
	if ((cfg->wLSd) != ioread32(wbase + NU_PPE_DST_LINE_STRIDE))
		{resw10 =1;
		rumboot_printf("resw10 error\n");}
		else 
		resw10=0;	
	if ((cfg->wESd) !=ioread32(wbase +NU_PPE_DST_ELEMENT_STRIDE)) 	
		{resw11 =1;
		rumboot_printf("resw11 error\n");}	
		else 
		resw11=0;
   	
	if ((cfg->wOpM) != (ioread32(wbase + NU_PPE_OP_MODE)& 0x0260FF33))
		{resw12 =1;
		rumboot_printf("resw12 error\n");}
		else 
		resw12=0;	
	if ((cfg->wBSWi) != (ioread32(wbase + NU_PPE_BOX_SIZE_W_IN)& 0x000001FFF))
		{resw13 =1;
		rumboot_printf("resw13 error\n");}
		else 
		resw13=0;
	if ((cfg->wBSHi) != (ioread32(wbase + NU_PPE_BOX_SIZE_H_IN)& 0x000001FFF))
		{resw14 =1;
		rumboot_printf("resw14 error\n");}
		else 
		resw14=0;
	
	if ((cfg->wBSCi) !=(ioread32(wbase +NU_PPE_BOX_SIZE_C_IN)& 0x000001FFF)) 	
		{resw15=1;
		rumboot_printf("resw15 error\n");}
		else 
		resw15=0;
	
	if ((cfg->wStWi) != (ioread32(wbase + NU_PPE_BOX_START_W_IN)& 0x000001FFF)) 
		{resw16 =1;
		rumboot_printf("resw16 error\n");}
		else 
		resw16=0;	
	if ((cfg->wOfWi) != (ioread32(wbase + NU_PPE_BOX_OFFSET_W_IN)& 0x000001FFF))
		{resw17 =1;
		rumboot_printf("resw17 error\n");}
		else 
		resw17=0;
	//------------------
	if ((cfg->wBSWo) != ioread32(wbase + NU_PPE_BOX_SIZE_W_OUT))
		{resw18 =1;
		rumboot_printf("resw13 error\n");}
		else 
		resw18=0;
	if ((cfg->wBSHo) != ioread32(wbase + NU_PPE_BOX_SIZE_H_OUT))
		{resw19 =1;
		rumboot_printf("resw14 error\n");}
		else 
		resw19=0;
	
	if ((cfg->wBSCo) !=ioread32(wbase +NU_PPE_BOX_SIZE_C_OUT)) 	
		{resw20=1;
		rumboot_printf("resw15 error\n");}
		else 
		resw20=0;	
	if ((cfg->wStWo) != (ioread32(wbase + NU_PPE_BOX_START_W_OUT)& 0x000001FFF)) 
		{resw21 =1;
		rumboot_printf("resw16 error\n");}
		else 
		resw21=0;	
	if ((cfg->wOfWo) != (ioread32(wbase + NU_PPE_BOX_OFFSET_W_OUT)& 0x000001FFF))
		{resw22 =1;
		rumboot_printf("resw17 error\n");}
		else 
		resw22=0;


	if ((cfg->wK) != (ioread32(wbase + NU_PPE_KERNEL)& 0x00FF0707))
		{resw25 =1;
		rumboot_printf("resw25 error\n");}
		else 
		resw25=0;
	if ((cfg->wKWr) != (ioread32(wbase + NU_PPE_RECIP_KERNEL_W)& 0x00000FFFF))
		{resw26 =1;
		rumboot_printf("resw26 error\n");}
		else 
		resw26=0;	
	if ((cfg->wKHr) != (ioread32(wbase + NU_PPE_RECIP_KERNEL_H)& 0x00000FFFF))
		{resw27 =1;
		rumboot_printf("resw27 error\n");}
		else 
		resw27=0;
	if ((cfg->wP) != (ioread32(wbase + NU_PPE_PADDING) & 0x00007777))
		{resw28 =1;
		rumboot_printf("resw28 error\n");}
		else 
		resw28=0;
	if ((cfg->wPV1) != (ioread32(wbase + NU_PPE_PADDING_VALUE_1)& 0x0007FFFF))
		{resw29 =1;
		rumboot_printf("resw29 error\n");}
		else 
		resw29=0;

	if ((cfg->wPV2) != (ioread32(wbase + NU_PPE_PADDING_VALUE_2)& 0x0007FFFF))
		{resw30 =1;
		rumboot_printf("resw30 error\n");}
		else 
		resw30=0;
	if ((cfg->wPV3) != (ioread32(wbase + NU_PPE_PADDING_VALUE_3)& 0x0007FFFF))

		{resw31 =1;
		rumboot_printf("resw31 error\n");}
		else 
		resw31=0;
	if ((cfg->wPV4) != (ioread32(wbase + NU_PPE_PADDING_VALUE_4)& 0x0007FFFF))

		{resw32 =1;
		rumboot_printf("resw32 error\n");}
		else 
		resw32=0;
	if ((cfg->wPV5) != (ioread32(wbase + NU_PPE_PADDING_VALUE_5)& 0x0007FFFF))

		{resw33 =1;
		rumboot_printf("resw33 error\n");}
		else 
		resw33=0;
	if ((cfg->wPV6) != (ioread32(wbase + NU_PPE_PADDING_VALUE_6)& 0x0007FFFF))

		{resw34 =1;
		rumboot_printf("resw34 error\n");}
		else 
		resw34=0;
	if ((cfg->wPV7) != (ioread32(wbase + NU_PPE_PADDING_VALUE_7)& 0x0007FFFF))

		{resw35 =1;
		rumboot_printf("resw28 error\n");}
		else 
		{resw35=0;}
	
	if ((0x00000000) !=ioread32(wbase +NU_PPE_INF_NUM_IN)) 	
	{reswro1 =1;
	rumboot_printf("reswro1 error\n");}	
		else 
		reswro1=0;	
	if ((0x00000000) !=ioread32(wbase +NU_PPE_NAN_NUM_IN)) 	
	{reswro2 =1;
	rumboot_printf("reswro2 error\n");}	
		else 
		reswro2=0;
	if ((0x00000000) !=ioread32(wbase +NU_PPE_NAN_NUM_OUT)) 	
	{reswro3 =1;
	rumboot_printf("reswro3 error\n");}	
		else 
		reswro3=0;
	
	  resr = 	res0 || res1|| res2 || res3 || res4 || res5 || res7 || res8 ||
				res9 || res10 || res11 || res12 || res13 || res14 || res15 ;
	  resw = 	resw0 || resw1|| resw2 || resw3 || resw4 || resw5 || resw6 || resw7 || resw8 ||
				resw10 || resw11 || resw12 || resw13 || resw14 || resw15 || resw16 || resw17 ||
				resw18 || resw19 || resw20 || resw21 || resw22 || resw23 || resw24 || resw25 ||
				resw26 || resw27 || resw28 || resw29 || resw30 || resw31 || resw32 || resw33 ||
				resw34 ||resw35; 
	  reswro = reswro1 || reswro2 || reswro3;			
	
	res = resr  || resw  || reswro ;

	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
	

 return 0; 
 } 

int main() {
 
  int res;
  rumboot_printf("Test PPE regs run\n");
  rumboot_printf("Read REGS after reset\n");
  
   // Configure RDMA initial state
 // cfg_regs.rSt = 0x00000000;
  cfg_regs.rWi    = 0x00000000;
  cfg_regs.rHi    = 0x00000000;
  cfg_regs.rCi    = 0x00000000;
  cfg_regs.rBALs  = 0x00000000;
 // cfg_regs.rBAHs  = 0x00000000;
  cfg_regs.rVSs   = 0x00000000;
  cfg_regs.rLSs   = 0x00000000;
  cfg_regs.rESs   = 0x00000000;
  cfg_regs.rOpM   = 0x00000000;
  cfg_regs.rBSWi  = 0x00000000;
  cfg_regs.rBSHi  = 0x00000000;
  cfg_regs.rBSCi  = 0x00000000;
  cfg_regs.rStWi  = 0x00000000;
  cfg_regs.rOfWi  = 0x00000000;
  // Configure PPE+WDMA
//  cfg_regs.wSt = 0x00000000;
  cfg_regs.wWo    = 0x00000000;
  cfg_regs.wHo    = 0x00000000;
  cfg_regs.wCo    = 0x00000000;
  cfg_regs.wBALd  = 0x00000000;
 // cfg_regs.wBAHd  = 0x00000000;
  cfg_regs.wVSd   = 0x00000000;
  cfg_regs.wLSd   = 0x00000000;
  cfg_regs.wESd   = 0x00000000;
  cfg_regs.wOpM   = 0x00000000;
  cfg_regs.wBSWo  = 0x00000000;
  cfg_regs.wBSHo  = 0x00000000;
  cfg_regs.wBSCo  = 0x00000000;
  cfg_regs.wStWo  = 0x00000000;
  cfg_regs.wOfWo  = 0x00000000;
  

   res = nu_ppe_rd_regs(NU_PPE_RDMA_BASE, NU_PPE_STANDALONE_BASE, &cfg_regs);

  if ( res !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
   //write zero's 
   rumboot_printf("Read REGS after  WRITE REGs\n");
   nu_ppe_setup_reg(NU_PPE_RDMA_BASE, NU_PPE_STANDALONE_BASE, &cfg_regs);
   res = nu_ppe_rd_regs(NU_PPE_RDMA_BASE, NU_PPE_STANDALONE_BASE, &cfg_regs);
   
  // Configure RDMA

  cfg_regs.rWi    = 0x00000000;
  cfg_regs.rHi    = 0x00001000;
  cfg_regs.rCi    = 0x00000020;
  cfg_regs.rBALs  = 0x13572468;
 // cfg_regs.rBAHs  = 0x00000000;
  cfg_regs.rVSs   = 0x00000020;
  cfg_regs.rLSs   = 0x00001000;
  cfg_regs.rESs   = 0x00000020;
  cfg_regs.rOpM   = 0x00200000;
  cfg_regs.rBSWi  = 0x0000007F;
  cfg_regs.rBSHi  = 0x00000000;
  cfg_regs.rBSCi  = 0x0000000F;
  cfg_regs.rStWi  = 0x0000007F;
  cfg_regs.rOfWi  = 0x00000000;
  // Configure PPE+WDMA
//  cfg_regs.wSt = 0x00000000;
  cfg_regs.wWo    = 0x00001000;
  cfg_regs.wHo    = 0x00001000;
  cfg_regs.wCo    = 0x00000020;
  cfg_regs.wBALd  = 0x12345678;
 // cfg_regs.wBAHd  = 0x00000000;
  cfg_regs.wVSd   = 0x00000020;
  cfg_regs.wLSd   = 0x00001000;
  cfg_regs.wESd   = 0x00000020;
  cfg_regs.wOpM   = 0x00200000;
  cfg_regs.wBSWo  = 0x0000007F;
  cfg_regs.wBSHo  = 0x00000000;
  cfg_regs.wBSCo  = 0x0000000F;
  cfg_regs.wStWo  = 0x0000007F;
  cfg_regs.wOfWo  = 0x00000000;
  
	//rumboot_printf("Read REGS after WRITE REGs\n");
	// write reg config data
   nu_ppe_setup_reg(NU_PPE_RDMA_BASE, NU_PPE_STANDALONE_BASE, &cfg_regs);
   res = nu_ppe_rd_regs(NU_PPE_RDMA_BASE, NU_PPE_STANDALONE_BASE, &cfg_regs);

  if ( res !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  
  // Configure RDMA with changed config

  cfg_regs.rWi    = 0xFFFFFFFF;
  cfg_regs.rHi    = 0xFFFFFFFF;
  cfg_regs.rCi    = 0xFFFFFFFF;
  cfg_regs.rBALs  = 0xedcda987;
 // cfg_regs.rBAHs  = 0x00000000;
  cfg_regs.rVSs   = 0xFFFFFFDF;
  cfg_regs.rLSs   = 0xFFFFEFFF;
  cfg_regs.rESs   = 0xFFFFFFDF;
  cfg_regs.rOpM   = 0x0040FF00;
  cfg_regs.rBSWi  = 0x00001F80;
  cfg_regs.rBSHi  = 0x00001FFF;
  cfg_regs.rBSCi  = 0x00001FF0;
  cfg_regs.rStWi  = 0x00001F80;
  cfg_regs.rOfWi  = 0x00001FFF;
  // Configure PPE+WDMA
//  cfg_regs.wSt = 0x00000000;
  cfg_regs.wWo    = 0xFFFFEFFF;
  cfg_regs.wHo    = 0xFFFFEFFF;
  cfg_regs.wCo    = 0xFFFFEFDF;
  cfg_regs.wBALd  = 0xedcda987;
 // cfg_regs.wBAHd  = 0x00000000;
  cfg_regs.wVSd   = 0xFFFFFFDF;
  cfg_regs.wLSd   = 0xFFFFEFFF;
  cfg_regs.wESd   = 0xFFFFFFDF;
  cfg_regs.wOpM   = 0x0240FF23; 
  cfg_regs.wBSWo  = 0x00001F80;
  cfg_regs.wBSHo  = 0x00001FFF;
  cfg_regs.wBSCo  = 0x00001FF0;
  cfg_regs.wStWo  = 0x00001F80;
  cfg_regs.wOfWo  = 0x00001FFF;
  
	//rumboot_printf("Read REGS after WRITE REGs\n");
	// write reg config data
   nu_ppe_setup_reg(NU_PPE_RDMA_BASE, NU_PPE_STANDALONE_BASE, &cfg_regs);
   res = nu_ppe_rd_regs(NU_PPE_RDMA_BASE, NU_PPE_STANDALONE_BASE, &cfg_regs);

  if ( res !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  
   rumboot_printf("Test PASSED\n");

   return 0;

}
