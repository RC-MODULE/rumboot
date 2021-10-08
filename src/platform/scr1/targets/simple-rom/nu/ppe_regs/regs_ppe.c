
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices/nu_cpdmac_lib.h>
#include <platform/devices.h>
#include <platform/devices/nu_lib.h>
#include <regs/regs_nu_ppe.h>

#include <platform/devices/nu_test_macro.h> 

//ConfigPPE cfg;

 int nu_ppe_rd_regs(uintptr_t rbase,uintptr_t wbase) {
	  int res;
	  int res0,res1,res2,res3,res4,res5,res6,res7,res8,res9,res10,res11,res12,
		  res13,res14,res15,res16,res17,res18,res19,res20,res21,res22,res23,res24,res25,res26;
	  int resw0,resw1,resw2,resw3,resw4,resw5,resw6,resw7,resw8,resw9,resw10,resw11,resw12,
		  resw13,resw14,resw15,resw16,resw17,resw18,resw19,resw20,resw21,resw22,resw23,resw24,
		  resw25,resw26,resw27,resw28,resw29,resw30,resw31,resw32,resw33,resw34, resw35,resw36,
		  resw37, resw38, resw39, resw40,resw41,resw42,resw43,resw44,resw45, resw46,resw47,resw48,
		  resw49,resw50, resw51;

	  int resr;
	  int resw;
	 
	//--------------------------RDMA--------------------------------------
	
		if ((0x00000000) !=(ioread32(rbase +NU_PPE_OP_ENABLE)& 0x00000001)) 
		{res0 =1;
		rumboot_printf("res0 error\n");}	
		else 
		{res0=0;}
	
	if ((0x00000000) !=(ioread32(rbase +NU_PPE_POINTER))) 	
		{res1 =1;
		rumboot_printf("res1 error\n");}	
		else 
		{res1=0;}
	if ((0x00000000) !=(ioread32(rbase +NU_PPE_STATUS) & 0x00000003)) 	
		{res2 =1;
		rumboot_printf("res2 error\n");}
		else 
		{res2=0;}
	if ((0x00020000) !=(ioread32(rbase +NU_PPE_RDMA_AXI_PARAM) & 0x0F07730F)) 	
		{res3 =1;
		rumboot_printf("res3 error\n");}
		else 
		{res3=0;}
	
	if ((0x00000000) != (ioread32(rbase +NU_PPE_RDMA_INT_STATUS)& 0x00000170)) //!!!! 0-bit does not correspond doc
	{res4 =1;
		rumboot_printf("res4 error\n");}
		else 
		{res4=0;}	
	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_INT_RESET)& 0x00000171)) 
		{res5 =1;
		rumboot_printf("res5 error\n");}
		else 
		{res5=0;}	

	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_INT_SET)& 0x00000171))
		{res6 =1;
		rumboot_printf("res6 error\n");}
		else 
		{res6=0;}	
	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_INT_MASK)& 0x00000171))
		{res7 =1;
		rumboot_printf("res7 error\n");}
		else 
		{res7=0;}	

	if((0x00000000)  !=ioread32(rbase + NU_PPE_RDMA_BASE_ADDR)) 	
		{res8 =1;
		rumboot_printf("res8 error\n");}
		else 
		{res8=0;}	
	if ((0x00000000)  != ioread32(rbase + NU_PPE_RDMA_BORDER_X))
		{res9 =1;
		rumboot_printf("res9 error\n");}	
		else 
		{res9=0;}	
	if ((0x00000000)  !=ioread32(rbase +NU_PPE_RDMA_BORDER_Y)) 	
		{res10 =1;
		rumboot_printf("res10 error\n");}	
		else 
		{res10=0;}
			
	if ((0x00000000)  != ioread32(rbase + NU_PPE_RDMA_BORDER_Z) )
		{res11 =1;
		rumboot_printf("res11 error\n");}	
		else 
		{res11=0;}
	
	if ((0x00000000)  != ioread32(rbase + NU_PPE_RDMA_STRIDE_X))
		{res12 =1;
		rumboot_printf("res12 error\n");}
		else 
		{res12=0;}
	if ((0x00000000)  != ioread32(rbase + NU_PPE_RDMA_STRIDE_Y))
		{res13 =1;
		rumboot_printf("res13 error\n");}
		else 
		{res13=0;}

	if ((0x00000000)  !=ioread32(rbase +NU_PPE_RDMA_STRIDE_Z))	
		{res14 =1;
		rumboot_printf("res14 error\n");}
		else 
		{res14=0;}


	if ((0x00000000)  != (ioread32(rbase + NU_PPE_RDMA_FRAG_SIZE)& 0x00FFFFFF))
		{res15 =1;
	  
		rumboot_printf("res15 error\n");}
		else 
		{res15=0;}	
	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_FRAG_LAST_SIZE)& 0x00FFFFFF))
		{res16 =1;
		rumboot_printf("res16 error\n");}
		else 
		{res16=0;}

	if ((0x00000002) != (ioread32(rbase + NU_PPE_RDMA_XYZ_DRCT)& 0x00000003))
		{res17 =1;
		rumboot_printf("NU_PPE_RDMA_XYZ_DRCT =%d\n",ioread32(rbase + NU_PPE_RDMA_XYZ_DRCT));	
		rumboot_printf("res17 error\n");}
		else 
		{res17=0;}
	

	if ((0x00000000)  != (ioread32(rbase + NU_PPE_RDMA_BOX_ST_SIZE_X)& 0x00001FFF))
		{res18 =1;	  
		rumboot_printf("res18 error\n");}
		else 
		{res18=0;}
	
	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_BOX_ST_SIZE_Y)& 0x00001FFF))
		{res19 =1;
		rumboot_printf("res19 error\n");}
		else 
		{res19=0;}

	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_BOX_ST_SIZE_Z)& 0x00001FFF))
		{res20 =1;
		rumboot_printf("res20 error\n");}
		else 
		{res20=0;}

	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_BOX_SIZE_X)& 0x00001FFF))
		{res21 =1;
		rumboot_printf("res21 error\n");}
		else 
		{res21=0;}
	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_BOX_SIZE_Y)& 0x00001FFF))
		{res22 =1;
		rumboot_printf("res22 error\n");}
		else 
		{res22=0;}

	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_BOX_SIZE_Z)& 0x00001FFF))
		{res23 =1;
		rumboot_printf("res23 error\n");}
		else 
		{res23=0;}	
	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_BOX_OFFSET_X)& 0x00001FFF))
		{res24 =1;
		rumboot_printf("res24 error\n");}
		else 
		{res24=0;}		
	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_BOX_OFFSET_Y)& 0x00001FFF))
		{res25 =1;
		rumboot_printf("res25 error\n");}
		else 
		{res25=0;}	
	if ((0x00000000) != (ioread32(rbase + NU_PPE_RDMA_BOX_OFFSET_Z)& 0x00001FFF))
		{res26 =1;
		rumboot_printf("res26 error\n");}
		else 
		{res26=0;}	
//------------------PPE+WDMA-------------------------------------------
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_OP_ENABLE)& 0x00000001)) 	
		{resw0 =1;
		rumboot_printf("resw0 error\n");}
		else 
		{resw0=0;}
	if ((0x00000000) !=ioread32(wbase +NU_PPE_POINTER)) 	
		{resw1 =1;
		rumboot_printf("resw1 error\n");}
		else 
		{resw1=0;}	  
	
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_STATUS)& 0x00000003)) 	
		{resw2 =1;
		rumboot_printf("resw2 error\n");}	
		else 
		{resw2=0;}
	if ((0x00020000) !=(ioread32(wbase +NU_PPE_WDMA_AXI_PARAM) & 0x0F07730F)) 	
		{resw3 =1;
	    rumboot_printf("data_resw3 ==%x\n",(ioread32(wbase +NU_PPE_WDMA_AXI_PARAM)) );
		rumboot_printf("resw3 error\n");}
		else 
		{resw3=0;}
	
	if ((0x00000000) != (ioread32(wbase +NU_PPE_WDMA_INT_STATUS)& 0x00000171))		
		{resw4 =1;
		rumboot_printf("resw4 error\n");}
		else 
		{resw4=0;}	
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_WDMA_INT_RESET)&  0x00000171)) 	
		{resw5 =1;
		rumboot_printf("resw5 error\n");}
		else 
		{resw5=0;}

	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_INT_SET)&  0x00000171))
		{resw6 =1;
		rumboot_printf("resw6 error\n");}
		else 
		{resw6=0;}
		
	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_INT_MASK)& 0x00000171))
		{resw7 =1;
		rumboot_printf("resw7 error\n");}
		else 
		{resw7=0;}
	if ((0x00000000) != ioread32(wbase +  NU_PPE_WDMA_BASE_ADDR))
		{resw8=1;
		rumboot_printf("resw8 error\n");}
		else 
		{resw8=0;}
	if ((0x00000000) != ioread32(wbase + NU_PPE_WDMA_BORDER_X))
		{resw9=1;
		rumboot_printf("resw9 error\n");}
		else 
		{resw9=0;}
	if ((0x00000000) != ioread32(wbase + NU_PPE_WDMA_BORDER_Y))
		{resw10=1;
		rumboot_printf("resw10 error\n");}
		else 
		{resw10=0;}
	if ((0x00000000) != ioread32(wbase + NU_PPE_WDMA_BORDER_Z))
		{resw11=1;
		rumboot_printf("resw11 error\n");}
		else 
		{resw11=0;}
		
	
	if ((0x00000000) != ioread32(wbase + NU_PPE_WDMA_STRIDE_X))
		{resw12=1;
		rumboot_printf("resw12 error\n");}
		else 
		{resw12=0;}
	if ((0x00000000) !=ioread32(wbase +NU_PPE_WDMA_STRIDE_Y)) 	
		{resw13 =1;
		rumboot_printf("resw13 error\n");}
		else 
		{resw13=0;}	
	if ((0x00000000) != ioread32(wbase + NU_PPE_WDMA_STRIDE_Z))
		{resw14 =1;
		rumboot_printf("resw14 error\n");}
		else 
		{resw14=0;}	
	if ((0x00000000) !=(ioread32(wbase + NU_PPE_WDMA_FRAG_SIZE)& 0x00FFFFFF)) 	
		{resw15 =1;
		rumboot_printf("resw15 error\n");}	
		else 
		{resw15=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_FRAG_LAST_SIZE)& 0x00FFFFFF))
		{resw16 =1;
		rumboot_printf("resw16 error\n");}
		else 
		{resw16=0;}	
	if ((0x00000002) != (ioread32(wbase + NU_PPE_WDMA_XYZ_DRCT)& 0x00000003))
		{resw17 =1;
		rumboot_printf("resw17 error\n");}
		else 
		{resw17=0;}
	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_BOX_ST_SIZE_X)& 0x000001FFF))
		{resw18 =1;
		rumboot_printf("resw18 error\n");}
		else 
		{resw18=0;}
	
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_WDMA_BOX_ST_SIZE_Y)& 0x000001FFF)) 	
		{resw19=1;
		rumboot_printf("resw19 error\n");}
		else 
		{resw19=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_BOX_ST_SIZE_Z)& 0x000001FFF)) 
		{resw20 =1;
		rumboot_printf("resw20 error\n");}
		else 
		{resw20=0;}	

	//------------------
	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_BOX_SIZE_X)& 0x000001FFF))
		{resw21 =1;
		rumboot_printf("resw21 error\n");}
		else 
		{resw21=0;}
	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_BOX_SIZE_Y)& 0x000001FFF))
		{resw22 =1;
		rumboot_printf("resw22 error\n");}
		else 
		{resw22=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_BOX_SIZE_Z)& 0x000001FFF)) 	
		{resw23=1;
		rumboot_printf("resw23 error\n");}
		else 
		{resw23=0;}	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_BOX_OFFSET_X)& 0x000001FFF)) 
		{resw24 =1;
		rumboot_printf("resw24 error\n");}
		else 
		{resw24=0;}	
	if ((0x00000000)!= (ioread32(wbase + NU_PPE_WDMA_BOX_OFFSET_Y)& 0x000001FFF))
		{resw25 =1;
		rumboot_printf("resw25 error\n");}
		else 
		{resw25=0;}

	if ((0x00000000) != (ioread32(wbase + NU_PPE_WDMA_BOX_OFFSET_Z)& 0x000001FFF))
		{resw26 =1;
		rumboot_printf("resw26 error\n");}
		else 
		{resw26=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_INP_BOX_ST_SIZE_X)& 0x000001FFF))
		{resw27 =1;
		rumboot_printf("resw27 error\n");}
		else 
		{resw27=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_INP_BOX_SIZE_X)& 0x000001FFF))
		{resw28 =1;
		rumboot_printf("resw28 error\n");}
		else 
		{resw28=0;}


		if ((0x00000000) != (ioread32(wbase + NU_PPE_INP_BOX_OFFSET_X)& 0x000001FFF))
		{resw29 =1;
		rumboot_printf("resw29 error\n");}
		else 
		{resw29=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_DATA_W_IN)&0x000001FFF))
		{resw30 =1;
		rumboot_printf("resw30 error\n");}
		else 
		{resw30=0;}	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_DATA_H_IN)& 0x000001FFF))
		{resw31 =1;
		rumboot_printf("resw31 error\n");}
		else 
		{resw31=0;}
	if ((0x00000000) != (ioread32(wbase + NU_PPE_DATA_C_IN) & 0x000001FFF))
		{resw32 =1;
		rumboot_printf("resw32 error\n");}
		else 
		{resw32=0;}
	if ((0x00000000) != (ioread32(wbase + NU_PPE_DATA_W_OUT )& 0x000001FFF))
		{resw33 =1;
		rumboot_printf("resw33 error\n");}
		else 
		{resw33=0;}

	if ((0x00000000) != (ioread32(wbase + NU_PPE_DATA_H_OUT)& 0x000001FFF))
		{resw34 =1;
		rumboot_printf("resw34 error\n");}
		else 
		{resw34=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_DATA_C_OUT)&  0x000001FFF))
		{resw35 =1;
		rumboot_printf("resw35 error\n");}
		else 
		{resw35=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_OP_MODE)& 0x01030703))
		{resw36 =1;
		rumboot_printf("resw36error\n");}
		else 
		{resw36=0;}
	if ((0x00000000) != (ioread32(wbase + NU_PPE_KERNEL)& 0x0007FFFF))

		{resw37 =1;
		rumboot_printf("resw37 error\n");}
		else 
		{resw37=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_RECIP_KERNEL_W)& 0x00FF0707))
		{resw38 =1;
		rumboot_printf("resw38 error\n");}
		else 
		{resw38=0;}
	
	if ((0x00000000) != (ioread32(wbase + NU_PPE_RECIP_KERNEL_H)& 0x00FFFFFF))
		{resw39 =1;
		rumboot_printf("resw39 error\n");}
		else 
		{resw39=0;}
	
	if ((0x00000000) != (ioread32(wbase +NU_PPE_PADDING)& 0x00077777)) 	
	{resw40 =1;
	rumboot_printf("resw40 error\n");}	
		else 
		{resw40=0;}	
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_1)& 0x0007FFFF)) 	
	{resw41 =1;
	rumboot_printf("resw41 error\n");}	
	else 
	{resw41=0;}

	if ((0x00000000) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_2)& 0x0007FFFF)) 	
	{resw42 =1;
	rumboot_printf("resw42 error\n");}	
	else 
	{resw42=0;}
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_3)& 0x0007FFFF)) 	
	{resw43 =1;
	rumboot_printf("resw43 error\n");}	
		else 
		{resw43=0;}
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_4)& 0x0007FFFF)) 	
	{resw44 =1;
	rumboot_printf("resw44 error\n");}	
		else 
		{resw44=0;}
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_5)& 0x0007FFFF)) 	
	{resw45 =1;
	rumboot_printf("resw45 error\n");}	
		else 
		{resw45=0;}
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_6)& 0x0007FFFF)) 	
	{resw46 =1;
	rumboot_printf("resw46 error\n");}	
		else 
		{resw46=0;}
	if ((0x00000000) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_7)& 0x0007FFFF)) 	
	{resw47 =1;
	rumboot_printf("resw47 error\n");}	
		else 
		{resw47=0;}
	
	if ((0x00000000) !=ioread32(wbase +NU_PPE_INF_NUM_IN)) 	
	{resw48 =1;
	rumboot_printf("resw48 error\n");}	
		else 
		{resw48=0;}
	
	if ((0x00000000) !=ioread32(wbase +NU_PPE_NAN_NUM_IN)) 	
	{resw49 =1;
	rumboot_printf("resw49 error\n");}	
		else 
		{resw49=0;}	
	
	if ((0x00000000) !=ioread32(wbase + NU_PPE_NAN_NUM_OUT)) 	
	{resw50 =1;
	rumboot_printf("resw50 error\n");}	
		else 
		{resw50=0;}		
	if ((0x00000000) !=(ioread32(wbase + NU_PPE_STATUS_DONE) & 0x00000001)) 	
	{resw51 =1;
	rumboot_printf("resw51 error\n");}	
		else 
		{resw51=0;}			
	
	
	  resr = 	res0 || res1|| res2 || res3 || res4 || res5 || res6 || res7 || res8 ||
				res9 || res10 || res11 || res12 || res13 || res14 || res15  || res16 || res17 ||
				res18 || res19 || res20 || res21  || res22 || res23  || res24  ||res25 || res26;
	  resw = 	resw0 || resw1|| resw2 || resw3 || resw4 || resw5 || resw6 || resw7 || resw8 || resw9 ||
				resw10 || resw11 || resw12 || resw13 || resw14 || resw15 || resw16 || resw17 ||
				resw18 || resw19 || resw20  || resw21 || resw22 || resw23 || resw24 || resw25 ||
				resw26 || resw27 || resw28 || resw29 || resw30 || resw31 || resw32 || resw33 ||
				resw34 ||resw35 || resw36 || resw37 || resw38 || resw39 || resw40 || resw41 ||
				resw42 || resw43  || resw44 ||resw45 ||resw46 || resw47 || resw48 || resw49 || resw50 || resw51; 
	   		
	res = resr  || resw;
  
	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
			

 return 0; 
 } 
 
int nu_ppe_wr_rd_regs(uintptr_t rbase,uintptr_t wbase, int32_t data) {
 	  int res;
	  int res0,res3,res4,res5,res6, res7,res8,res9,res10,res11,res12,
		  res13,res14,res15,res16,res17,res18,res19,res20,res21,res22,res23,res24,res25;
	  int resw0,resw3,resw4,resw5,resw6,resw7,resw8,resw9,resw10,resw11,resw12,
		  resw13,resw14,resw15,resw16,resw17,resw18,resw19,/*resw20,*/resw21,resw22,resw23,resw24,
		  resw25,resw26,resw27,resw28,resw29,resw30,resw31,resw32,resw33,resw34, resw35,resw36,
		  resw37, resw38, resw39, resw40,resw41,resw42,resw43,resw44,resw45, resw46, resw47;

	  int resr;
	  int resw;
	  
	  int32_t tmpr;
	  tmpr = data;
	  
	    iowrite32((0x00000000), rbase + NU_PPE_OP_ENABLE);
		if ((0x00000000)  !=(ioread32(rbase +NU_PPE_OP_ENABLE)& 0x00000001)) 	
		{res0 =1;
		rumboot_printf("res0wr error\n");
		rumboot_printf("res0wr_tmp =%x\n",(ioread32(rbase +NU_PPE_OP_ENABLE)));
		rumboot_printf("tmpr_=%x\n",(tmpr &  0x00000001));
		}
		else 
		{res0=0;}	

		iowrite32(tmpr, rbase + NU_PPE_RDMA_AXI_PARAM);
		if ((tmpr &  0x0F07730F)  !=(ioread32(rbase +NU_PPE_RDMA_AXI_PARAM)& 0x0F07730F)) 	
		{res3 =1;
		rumboot_printf("res3 error\n");}
		else 
		{res3=0;}	
		
	
	    iowrite32(tmpr, rbase + NU_PPE_RDMA_INT_RESET);
	if ((0x00000000 )!= (ioread32(rbase  + NU_PPE_RDMA_INT_RESET)& 0x00000171)) 
		{res4 =1;
		rumboot_printf("res4 error\n");}
		else 
		{res4=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_INT_SET);
	if   ((0x00000000 ) != (ioread32(rbase + NU_PPE_RDMA_INT_SET)& 0x00000171))
		{res5 =1;
		rumboot_printf("res5 error\n");}
		else 
		{res5=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_INT_MASK);	
	if ((tmpr &0x00000171 ) != (ioread32(rbase + NU_PPE_RDMA_INT_MASK)& 0x00000171))
		{res6 =1;
		rumboot_printf("res6 error\n");}
		else 
		{res6=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_BASE_ADDR);
	if(tmpr  !=ioread32(rbase + NU_PPE_RDMA_BASE_ADDR)) 	
		{res7 =1;
		rumboot_printf("res7 error\n");}
		else 
		{res7=0;}

		iowrite32(tmpr, rbase + NU_PPE_RDMA_BORDER_X);
	if (tmpr  != ioread32(rbase + NU_PPE_RDMA_BORDER_X))
		{res8 =1;
		rumboot_printf("res8 error\n");}	
		else 
		{res8=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_BORDER_Y);
	if (tmpr !=ioread32(rbase + NU_PPE_RDMA_BORDER_Y)) 	
		{res9 =1;
		rumboot_printf("res9 error\n");}	
		else 
		{res9=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_BORDER_Z);			
	if (tmpr   != ioread32(rbase + NU_PPE_RDMA_BORDER_Z) )
		{res10 =1;
		rumboot_printf("res10 error\n");}	
		else 
		{res10=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_STRIDE_X);
	if (tmpr   != ioread32(rbase + NU_PPE_RDMA_STRIDE_X))
		{res11 =1;
		rumboot_printf("res11 error\n");}
		else 
		{res11=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_STRIDE_Y);	
	if (tmpr  != ioread32(rbase + NU_PPE_RDMA_STRIDE_Y))
		{res12 =1;
		rumboot_printf("res12 error\n");}
		else 
		{res12=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_STRIDE_Z);
	if (tmpr   !=ioread32(rbase +NU_PPE_RDMA_STRIDE_Z))	
		{res13 =1;
		rumboot_printf("res13 error\n");}
		else 
		{res13=0;}

		iowrite32(tmpr, rbase + NU_PPE_RDMA_FRAG_SIZE);
	if ((tmpr & 0x00FFFFFF)  != (ioread32(rbase + NU_PPE_RDMA_FRAG_SIZE)& 0x00FFFFFF))
		{res14 =1;	  
		rumboot_printf("res14 error\n");}
		else 
		{res14=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_FRAG_LAST_SIZE);	
	if ((tmpr & 0x00FFFFFF) != (ioread32(rbase + NU_PPE_RDMA_FRAG_LAST_SIZE)& 0x00FFFFFF))
		{res15 =1;
		rumboot_printf("res15 error\n");}
		else 
		{res15=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_XYZ_DRCT);
	if ((tmpr &0x00000003) != (ioread32(rbase + NU_PPE_RDMA_XYZ_DRCT)& 0x00000003))
		{res16 =1;
		rumboot_printf("res16 error\n");}
		else 
		{res16=0;}
	
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_BOX_ST_SIZE_X);
	if  ((tmpr & 0x00001FFF)  != (ioread32(rbase + NU_PPE_RDMA_BOX_ST_SIZE_X)& 0x00001FFF))
		{res17 =1;	  
		rumboot_printf("res17 error\n");}
		else 
		{res17=0;}

		iowrite32(tmpr, rbase + NU_PPE_RDMA_BOX_ST_SIZE_Y);
	if ((tmpr & 0x00001FFF)!= (ioread32(rbase + NU_PPE_RDMA_BOX_ST_SIZE_Y)& 0x00001FFF))
		{res18 =1;
		rumboot_printf("res18 error\n");}
		else 
		{res18=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_BOX_ST_SIZE_Z);
	if ((tmpr & 0x00001FFF) != (ioread32(rbase + NU_PPE_RDMA_BOX_ST_SIZE_Z)& 0x00001FFF))
		{res19 =1;
		rumboot_printf("res19 error\n");}
		else 
		{res19=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_BOX_SIZE_X);
	if ((tmpr & 0x00001FFF) != (ioread32(rbase + NU_PPE_RDMA_BOX_SIZE_X)& 0x00001FFF))
		{res20 =1;
		rumboot_printf("res20 error\n");}
		else 
		{res20=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_BOX_SIZE_Y);
	if ((tmpr & 0x00001FFF) != (ioread32(rbase + NU_PPE_RDMA_BOX_SIZE_Y)& 0x00001FFF))
		{res21 =1;
		rumboot_printf("res21 error\n");}
		else 
		{res21=0;}
		
		iowrite32(tmpr, rbase + NU_PPE_RDMA_BOX_SIZE_Z);
	if ((tmpr & 0x00001FFF) != (ioread32(rbase + NU_PPE_RDMA_BOX_SIZE_Z)& 0x00001FFF))
		{res22 =1;
		rumboot_printf("res22 error\n");}
		else 
		{res22=0;}
	
		iowrite32(tmpr, rbase + NU_PPE_RDMA_BOX_OFFSET_X);	
	if ((tmpr & 0x00001FFF) != (ioread32(rbase + NU_PPE_RDMA_BOX_OFFSET_X)& 0x00001FFF))
		{res23 =1;
		rumboot_printf("res23 error\n");}
		else 
		{res23=0;}

		iowrite32(tmpr, rbase + NU_PPE_RDMA_BOX_OFFSET_Y);
	if ((tmpr & 0x00001FFF) != (ioread32(rbase + NU_PPE_RDMA_BOX_OFFSET_Y)& 0x00001FFF))
		{res24 =1;
		rumboot_printf("res24 error\n");}
		else 
		{res24=0;}

		iowrite32(tmpr, rbase + NU_PPE_RDMA_BOX_OFFSET_Z);
	if ((tmpr & 0x00001FFF) != (ioread32(rbase + NU_PPE_RDMA_BOX_OFFSET_Z)& 0x00001FFF))
		{res25 =1;
		rumboot_printf("res25 error\n");}
		else 
		{res25=0;} 
	
//------------------PPE+WDMA-------------------------------------------
		iowrite32((0x00000000), wbase + NU_PPE_OP_ENABLE);	
		if (( 0x000000000) !=(ioread32(wbase + NU_PPE_OP_ENABLE)& 0x00000001)) 	
		{resw0 =1;
	    rumboot_printf("data_resw0 = %x\n",ioread32(wbase + NU_PPE_OP_ENABLE));
		rumboot_printf("resw0 error\n");}	
		else 
		{resw0=0;}
	
		iowrite32(tmpr, wbase + NU_PPE_WDMA_AXI_PARAM);	
		if ((tmpr& 0x0F07730F) !=(ioread32(wbase + NU_PPE_WDMA_AXI_PARAM)& 0x0F07730F)) 	
		{resw3 =1;
		rumboot_printf("resw3 error\n");}	
		else 
		{resw3=0;}
		iowrite32(tmpr, wbase + NU_PPE_WDMA_INT_RESET);	
		
	if ((tmpr & 0x00000000) !=(ioread32(wbase +NU_PPE_WDMA_INT_RESET)& 0x00000171))//doc 0, 8 bit??
		{resw4 =1;
		rumboot_printf("resw4 error\n");}
		else 
		{resw4=0;}
	
		iowrite32(tmpr, wbase + NU_PPE_WDMA_INT_SET);	
		if ((tmpr & 0x00000000) != (ioread32(wbase + NU_PPE_WDMA_INT_SET)& 0x00000171))//doc 0, 8 bit
		{resw5 =1;
		rumboot_printf("resw5 error\n");}
		else 
		{resw5=0;}
	
		iowrite32(tmpr, wbase + NU_PPE_WDMA_INT_MASK);	
	if ((tmpr & 0x00000171) != (ioread32(wbase + NU_PPE_WDMA_INT_MASK)& 0x00000171))
		{resw6 =1;
		rumboot_printf("resw6 error\n");}
		else 
		{resw6=0;}
	
		iowrite32(tmpr, wbase + NU_PPE_WDMA_BASE_ADDR);	
	if (tmpr != ioread32(wbase + NU_PPE_WDMA_BASE_ADDR))
		{resw7=1;
		rumboot_printf("resw7 error\n");}
		else 
		{resw7=0;}

		iowrite32(tmpr,wbase + NU_PPE_WDMA_BORDER_X);
	if (tmpr != ioread32(wbase + NU_PPE_WDMA_BORDER_X))
		{resw8=1;
		rumboot_printf("resw8 error\n");}
		else 
		{resw8=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_WDMA_BORDER_Y);
	if (tmpr != ioread32(wbase + NU_PPE_WDMA_BORDER_Y))
		{resw9=1;
		rumboot_printf("resw9 error\n");}
		else 
		{resw9=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_WDMA_BORDER_Z);
	if (tmpr != ioread32(wbase + NU_PPE_WDMA_BORDER_Z))
		{resw10=1;
		rumboot_printf("resw10 error\n");}
		else 
		{resw10=0;}
		
		iowrite32(tmpr,wbase + NU_PPE_WDMA_STRIDE_X);
	if (tmpr != ioread32(wbase + NU_PPE_WDMA_STRIDE_X))
		{resw11=1;
		rumboot_printf("resw11 error\n");}
		else 
		{resw11=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_WDMA_STRIDE_Y);	
	if (tmpr !=ioread32(wbase + NU_PPE_WDMA_STRIDE_Y)) 	
		{resw12 =1;
		rumboot_printf("resw12 error\n");}
		else 
		{resw12=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_WDMA_STRIDE_Z);	
	if (tmpr != ioread32(wbase + NU_PPE_WDMA_STRIDE_Z))
		{resw13 =1;
		rumboot_printf("resw13 error\n");}
		else 
		{resw13=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_WDMA_FRAG_SIZE);
	if ((tmpr & 0x00FFFFFF) !=(ioread32(wbase + NU_PPE_WDMA_FRAG_SIZE)& 0x00FFFFFF)) 	
		{resw14 =1;
		rumboot_printf("resw14 error\n");}	
		else 
		{resw14=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_WDMA_FRAG_LAST_SIZE);   	
	if ((tmpr & 0x00FFFFFF)  != (ioread32(wbase + NU_PPE_WDMA_FRAG_LAST_SIZE)& 0x00FFFFFF))
		{resw15 =1;
		rumboot_printf("resw15 error\n");}
		else 
		{resw15=0;}

		iowrite32(tmpr,wbase +  NU_PPE_WDMA_XYZ_DRCT);
	if ((tmpr & 0x00000003)  != (ioread32(wbase + NU_PPE_WDMA_XYZ_DRCT)& 0x00000003))
		{resw16 =1;
		rumboot_printf("resw16 error\n");}
		else 
		{resw16=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_WDMA_BOX_ST_SIZE_X);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_WDMA_BOX_ST_SIZE_X)& 0x000001FFF))
		{resw17 =1;
		rumboot_printf("resw17 error\n");}
		else 
		{resw17=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_WDMA_BOX_ST_SIZE_Y);	
	if ((tmpr & 0x000001FFF) !=(ioread32(wbase +NU_PPE_WDMA_BOX_ST_SIZE_Y)& 0x000001FFF)) 	
		{resw18=1;
		rumboot_printf("resw18 error\n");}
		else 
		{resw18=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_WDMA_BOX_ST_SIZE_Z);	
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_WDMA_BOX_ST_SIZE_Z)& 0x000001FFF)) 
		{resw19 =1;
		rumboot_printf("resw19 error\n");}
		else 
		{resw19=0;}
	
	

		iowrite32(tmpr,wbase +  NU_PPE_WDMA_BOX_SIZE_X);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_WDMA_BOX_SIZE_X)& 0x000001FFF))
		{resw21 =1;
		rumboot_printf("resw21 error\n");}
		else 
		{resw21=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_WDMA_BOX_SIZE_Y);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_WDMA_BOX_SIZE_Y)& 0x000001FFF))
		{resw22 =1;
		rumboot_printf("resw22 error\n");}
		else 
		{resw22=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_WDMA_BOX_SIZE_Z);
	if ((tmpr & 0x000001FFF)!= (ioread32(wbase + NU_PPE_WDMA_BOX_SIZE_Z)& 0x000001FFF)) 	
		{resw23=1;
		rumboot_printf("resw23 error\n");}
		else 
		{resw23=0;}

		iowrite32(tmpr,wbase +  NU_PPE_WDMA_BOX_OFFSET_X);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_WDMA_BOX_OFFSET_X)& 0x000001FFF)) 
		{resw24 =1;
		rumboot_printf("resw24 error\n");}
		else 
		{resw24=0;}

		iowrite32(tmpr,wbase +  NU_PPE_WDMA_BOX_OFFSET_Y);
	if ((tmpr & 0x000001FFF)!= (ioread32(wbase + NU_PPE_WDMA_BOX_OFFSET_Y)& 0x000001FFF))
		{resw25 =1;
		rumboot_printf("resw25 error\n");}
		else 
		{resw25=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_WDMA_BOX_OFFSET_Z);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_WDMA_BOX_OFFSET_Z)& 0x000001FFF))
		{resw26 =1;
		rumboot_printf("resw26 error\n");}
		else 
		{resw26=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_INP_BOX_ST_SIZE_X);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_INP_BOX_ST_SIZE_X)& 0x000001FFF))
		{resw27 =1;
		rumboot_printf("resw27 error\n");}
		else 
		{resw27=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_INP_BOX_SIZE_X);	
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_INP_BOX_SIZE_X)& 0x000001FFF))
		{resw28 =1;
		rumboot_printf("resw28 error\n");}
		else 
		{resw28=0;}

		iowrite32(tmpr,wbase +  NU_PPE_INP_BOX_OFFSET_X);
		if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_INP_BOX_OFFSET_X)& 0x000001FFF))
		{resw29 =1;
		rumboot_printf("resw29 error\n");}
		else 
		{resw29=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_DATA_W_IN);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_DATA_W_IN)& 0x000001FFF))
		{resw30 =1;
		rumboot_printf("resw30 error\n");}
		else 
		{resw30=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_DATA_H_IN);	
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_DATA_H_IN)& 0x000001FFF))
		{resw31 =1;
		rumboot_printf("resw31 error\n");}
		else 
		{resw31=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_DATA_C_IN);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_DATA_C_IN) & 0x000001FFF))
		{resw32 =1;
		rumboot_printf("resw32 error\n");}
		else 
		{resw32=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_DATA_W_OUT);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_DATA_W_OUT )& 0x000001FFF))
		{resw33 =1;
		rumboot_printf("resw33 error\n");}
		else 
		{resw33=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_DATA_H_OUT);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_DATA_H_OUT)& 0x000001FFF))
		{resw34 =1;
		rumboot_printf("resw34 error\n");}
		else 
		{resw34=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_DATA_C_OUT);
	if ((tmpr & 0x000001FFF) != (ioread32(wbase + NU_PPE_DATA_C_OUT)&  0x000001FFF))
		{resw35 =1;
		rumboot_printf("resw35 error\n");}
		else 
		{resw35=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_OP_MODE);
	if ((tmpr & 0x01030703) != (ioread32(wbase + NU_PPE_OP_MODE)& 0x01030703))
		{resw36 =1;
		rumboot_printf("resw36 error\n");}
		else 
		{resw36=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_KERNEL);
	if ((tmpr & 0x00FF0707) != (ioread32(wbase + NU_PPE_KERNEL)& 0x00FF0707))
		{resw37 =1;
		rumboot_printf("resw37 error\n");}
		else 
		{resw37=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_RECIP_KERNEL_W);
	if ((tmpr & 0x00FF0707) != (ioread32(wbase + NU_PPE_RECIP_KERNEL_W)& 0x00FF0707))
		{resw38 =1;
		rumboot_printf("resw38 error\n");}
		else 
		{resw38=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_RECIP_KERNEL_H);
	if ((tmpr & 0x00FFFFFF) != (ioread32(wbase + NU_PPE_RECIP_KERNEL_H)& 0x00FFFFFF))
		{resw39 =1;
		rumboot_printf("resw39 error\n");}
		else 
		{resw39=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_PADDING);
	if ((tmpr & 0x00007777) != (ioread32(wbase + NU_PPE_PADDING)& 0x00007777)) 	
		{resw40 =1;
		rumboot_printf("resw40 error\n");}	
		else 
		{resw40=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_PADDING_VALUE_1);	
	if ((tmpr & 0x0007FFFF) !=(ioread32(wbase + NU_PPE_PADDING_VALUE_1)& 0x0007FFFF)) 	
		{resw41 =1;
		rumboot_printf("resw41 error\n");}	
		else 
		{resw41=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_PADDING_VALUE_2);	
	if ((tmpr & 0x0007FFFF) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_2)& 0x0007FFFF)) 	
		{resw42 =1;
		rumboot_printf("resw42 error\n");}	
		else 
		{resw42=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_PADDING_VALUE_3);	
	if ((tmpr & 0x0007FFFF) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_3)& 0x0007FFFF)) 	
		{resw43 =1;
		rumboot_printf("resw43 error\n");}	
		else 
		{resw43=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_PADDING_VALUE_4);	
	if ((tmpr & 0x0007FFFF) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_4)& 0x0007FFFF)) 	
		{resw44 =1;
		rumboot_printf("resw44 error\n");}	
		else 
		{resw44=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_PADDING_VALUE_5);	
	if ((tmpr & 0x0007FFFF) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_5)& 0x0007FFFF)) 	
		{resw45 =1;
		rumboot_printf("resw45 error\n");}	
		else 
		{resw45=0;}
	
		iowrite32(tmpr,wbase +  NU_PPE_PADDING_VALUE_6);	
	if ((tmpr & 0x0007FFFF) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_6)& 0x0007FFFF)) 	
		{resw46 =1;
		rumboot_printf("resw46 error\n");}	
		else 
		{resw46=0;}
	
		iowrite32(tmpr,wbase + NU_PPE_PADDING_VALUE_7);	
	if ((tmpr & 0x0007FFFF) !=(ioread32(wbase +NU_PPE_PADDING_VALUE_7)& 0x0007FFFF)) 	
		{resw47 =1;
		rumboot_printf("resw47 error\n");}	
		else 
		{resw47=0;}
	
     resr = 	res0 ||  res3 || res4 || res5 || res6 || res7 || res8 ||
				res9 || res10 || res11 || res12 || res13 || res14 || res15  || res16 || res17 ||
				res18 || res19 || res20 || res21  || res22 || res23  || res24 || res25;
	  resw = 	resw0 || resw3 || resw4 || resw5 || resw6 || resw7 || resw8 || resw9 ||
				resw10 || resw11 || resw12 || resw13 || resw14 || resw15 || resw16 || resw17 ||
				resw18 || resw19 || /*resw20 || */ resw21 || resw22 || resw23 || resw24 || resw25 ||
				resw26 || resw27 || resw28 || resw29 || resw30 || resw31 || resw32 || resw33 ||
				resw34 ||resw35 || resw36 ||resw37 ||resw38 || resw39 || resw40 || resw41 ||
				resw42 || resw43  || resw44 ||resw45 ||resw46 || resw47 ; 
	   		
	res = resr  || resw;

 
 
	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
	

 return 0; 
 } 
 
int nu_running_one(uintptr_t base, uint32_t ppe_offset, uint32_t mask, int end_number )
{
    uint32_t data = 1;

	
    for ( int i = 0; i< end_number; i++)
    {
        iowrite32((data & mask)    ,(base + ppe_offset));
        if (ioread32(base + ppe_offset) != (data & mask))
        {
            rumboot_printf("PPE running one ERROR data=%x\n", data);
            return 1;
        }
        data = (data << 1);
    }
    return 0;
}

int main() {
 
  int res,res1,res2,res3,res4;
  rumboot_printf("Test PPE regs run\n");
  rumboot_printf("Read REGS after reset\n");
    rumboot_printf("NA_CU_PPE_UNIT_MODE = %x\n",NA_CU_PPE_UNIT_MODE);
	rumboot_printf("NPE_BASE = %x\n",NPE_BASE);
	rumboot_printf("NA_CU_REGS_BASE = %x\n",NA_CU_REGS_BASE);

	rumboot_printf("MY_PPE_RDMA_BASE = %x\n",MY_PPE_RDMA_BASE);
	rumboot_printf("MY_PPE_REGS_BASE = %x\n",MY_PPE_REGS_BASE);
	
	#if DUT_IS_NPE
	 na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_PPE_UNIT_MODE);
	#endif
   // Configure RDMA initial state
   res1 = nu_ppe_rd_regs(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE);

  if ( res1 !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
   rumboot_printf("Check READs after reset PASSED \n");
   //write zero's 
   {rumboot_printf("Read REGS after  WRITE REGs ZERO's\n");}
   res2 = nu_ppe_wr_rd_regs(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, 0x00000000);
   
  if ( res2 !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  { rumboot_printf("Check READs after write ZERO's PASSED \n");}
    {rumboot_printf("Read REGS after  WRITE REGs ONE's\n");} 
	  //write one's 
    res3 =  nu_ppe_wr_rd_regs(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE,  0xFFFFFFFF);

 if ( res3 !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  { rumboot_printf("Read REGS after  WRITE REGs ONE's PASSED\n");};

  { rumboot_printf("Read REGS running ONE\n");}
  res4 = nu_running_one(  MY_PPE_RDMA_BASE,NU_PPE_RDMA_BASE_ADDR,0xFFFFFFFF,32);	
	if  (res4 !=0)
	{rumboot_printf( "Check running ONE FAILED\n");
	return 1;
	}
	 else 
	{rumboot_printf( "Check 'running ONE' PASSED\n");	}
 
   res = res1 || res2  || res3 || res4;

  if ( res !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  
   rumboot_printf("Test PASSED\n");

   return 0;

}
