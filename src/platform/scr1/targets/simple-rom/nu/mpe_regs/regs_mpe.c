
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
	  int res0,/*res1,res2,res3,res4,res5,res6,res7,*//*res8,res9,*/res10,res11,res12,
		  res13,res14,res15,/*res16,*/res17,/*res18,*/res19,/*res20,*/res21,/*res22,*/res23,/*res24,*/res25,/*res26,*/
		  res29, res31,res32,res34;
	  int resr;

	 
	//--------------------------MPE_MA--------------------------------------
	
	
	
/*	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_COMMON_WA)& 0x00000170))
	{res4 =1;
		rumboot_printf("res4 error\n");}
		else 
		{res4=0;}	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_MA_BASE + MPE_COMMON_DA)& 0x00000171)) 
		{res5 =1;
		rumboot_printf("res5 error\n");}
		else 
		{res5=0;}	

	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_VRA)& 0x00000171))
		{res6 =1;
		rumboot_printf("res6 error\n");}
		else 
		{res6=0;}	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_MA_BASE + MPE_COMMON_NR)& 0x00000171))
		{res7 =1;
		rumboot_printf("res7 error\n");}
		else 
		{res7=0;}	
*/
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
	
	if ((0x00000001)  != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS)& 0x0000000F))
		{res12 =1;
		rumboot_printf("MPE_COMMON_D_BIAS = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS)); 
		rumboot_printf("Unexpected MPE_COMMON_D_BIAS error\n");}	
		else 
		{res12=0;}
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


	if ((0x00000080)  != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountM)& 0x00000FFF))
		{res15 =1;
	  	rumboot_printf("MPE_ADD_CountM = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountM)); 
		rumboot_printf("Unexpected MPE_ADD_CountM error\n");}	
	
		else 
		{res15=0;}	
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountM)& 0x00FFFFFF))
//		{res16 =1;
//		rumboot_printf("res16 error\n");}
//		else 
//		{res16=0;}

	if ((0x00000001) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountY)& 0x00003FFF))
		{res17 =1;
		rumboot_printf("MPE_ADD_CountY = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountY)); 
		rumboot_printf("Unexpected MPE_ADD_CountY error\n");}
		else 
		{res17=0;}
	

//	if ((0x00000001)  != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountY)& 0x00001FFF))
//		{res18 =1;	  
//		rumboot_printf("res18 error\n");}
//		else 
//		{res18=0;}
	
	if ((0x00000080) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountX)& 0x00003FFF))
		{res19 =1;
		rumboot_printf("MPE_ADD_CountM = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountX)); 
		rumboot_printf("Unexpected MPE_ADD_CountX error\n");}
		else 
		{res19=0;}
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountX)& 0x00001FFF))
//		{res20 =1;
//		rumboot_printf("res20 error\n");}
//		else 
//		{res20=0;}
	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountPLC)& 0x00001FFF))
		{res21 =1;
		rumboot_printf("res21 error\n");}
		else 
		{res21=0;}

//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountPLC)& 0x00001FFF))
//		{res22 =1;
//		rumboot_printf("res22 error\n");}
//		else 
//		{res22=0;}	
	if ((0x00000001) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountLCZ)& 0x0000007F))
		{res23 =1;
		rumboot_printf("MPE_ADD_CountLCZ = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountLCZ)); 
		rumboot_printf("Unexpected MPE_ADD_CountLCZ error\n");}
		else 
		{res23=0;}		
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountLCZ)& 0x00001FFF))
//		{res24 =1;
//		rumboot_printf("res24 error\n");}
//		else 
//		{res24=0;}	
	if ((0x00000001) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountPPS)& 0x00000FFF))
		{res25 =1;
	    rumboot_printf("  MPE_ADD_CountPPS = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountPPS));
		rumboot_printf("Unexpected MPE_ADD_CountPPS error\n");}
		else 
		{res25=0;}
/*	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountPPS)& 0x00001FFF))
		{res26 =1;
		rumboot_printf("res26 error\n");}
		else 
		{res26=0;}	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountJ)& 0x00001FFF))
		{res27 =1;
		rumboot_printf("res27 error\n");}
		else 
		{res27=0;}
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_CMP_CountJ)& 0x00001FFF))
		{res28 =1;
		rumboot_printf("res28 error\n");}
		else 
		{res28=0;}
*/	
	if ((0x00000010) != (ioread32(rbase + MPE_MA_BASE +MPE_ADD_CountI1)& 0x00001FFF))
		{ res29=1;
		rumboot_printf("  MPE_ADD_CountI1 = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountI1));
		rumboot_printf("Unexpected MPE_ADD_CountI1 error\n");}
		else 
		{res29=0;}	
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_CMP_CountI1)& 0x00001FFF))
//		{res30 =1;
//		rumboot_printf("res30 error\n");}
//		else 
//		{res30=0;}
	if ((0x00000001) != (ioread32(rbase + MPE_MA_BASE +MPE_ADD_CountI0)& 0x00001FFF))
		{res31 =1;
		rumboot_printf("  MPE_ADD_CountI0 = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountI0));
		rumboot_printf("Unexpected MPE_ADD_CountI0 error\n");}
		else 
		{res31=0;}	
	if ((0x00000008) != (ioread32(rbase + MPE_MA_BASE +MPE_CMP_CountI0)& 0x00001FFF))
		{res32 =1;
		rumboot_printf("  MPE_CMP_CountI0 = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountI0));
		rumboot_printf("Unexpected MPE_CMP_CountI0 error\n");}
		else 
		{res32=0;}
	
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
	
	
//------------------MPE_DMA Control  	-------------------------------------------
	
//------------------MPE_DMA wr Channel  -------------------------------------------

	  resr = 	res0 || /*res1|| res2 || res3 ||  res4 || res5 || res6 || res7 || res8 ||
				res9 || */ res10 || res11 || res12 || res13 || res14 || res15  /*|| res16 */|| res17 ||
				/*res18 || */res19 || /*res20 ||*/ res21  ||/* res22 || */res23  || /*res24  ||*/res25 || //res26 
				res29 || res31 || res32;// || res34 ;
	
	res = resr ;
  
	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
			
 return 0; 
 } 

int nu_mpe_wr_rd_regs(uintptr_t rbase, int32_t data) {
 	  int res;
	  int /*res0,res1,res2,res3,res4,res5,res6,res7,*//*res8,res9,res10,*/res11,res12,
		  res13,res14,res15,/*res16,*/res17,/*res18,*/res19,/*res20,*/res21,/*res22,*/res23,/*res24,*/res25,/*res26,*/
			res29 , res31 , res32 , res34 ;
	  int resr;

	 
	//--------------------------MPE_MA--------------------------------------
		
	//if ((0x00000000) !=(ioread32(rbase +  MPE_MA_BASE + MPE_CMD_IRC))) 	
	//	{res0 =1;
	//	rumboot_printf("res1 error\n");}	
	//	else 
	//	{res0=0;}

	//if ((0x00000000) !=(ioread32(rbase +  MPE_MA_BASE + MPE_CMD_IRCW))) 	
	//	{res1 =1;
	//	rumboot_printf("res1 error\n");}	
	//	else 
	//	{res1=0;}
//	if ((0x00000000) !=(ioread32(rbase + MPE_MA_BASE + MPE_CMD_ICM) & 0x00000003)) 	
//		{res2 =1;
//		rumboot_printf("res2 error\n");}
//		else 
//		{res2=0;}
//	if ((0x00020000) !=(ioread32(rbase + MPE_CMD_ICMW) & 0x0F07730F)) 	
//		{res3 =1;
//		rumboot_printf("res3 error\n");}
//		else 
//		{res3=0;}
/*	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_COMMON_WA)& 0x00000170))
	{res4 =1;
		rumboot_printf("res4 error\n");}
		else 
		{res4=0;}	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_MA_BASE + MPE_COMMON_DA)& 0x00000171)) 
		{res5 =1;
		rumboot_printf("res5 error\n");}
		else 
		{res5=0;}	

	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_VRA)& 0x00000171))
		{res6 =1;
		rumboot_printf("res6 error\n");}
		else 
		{res6=0;}	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_MA_BASE + MPE_COMMON_NR)& 0x00000171))
		{res7 =1;
		rumboot_printf("res7 error\n");}
		else 
		{res7=0;}	
*/
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
			
	//if ((0x00000000)  != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_IN_BUF_CFG) & 0x000001FF))
	//	{res11 =1;
	//	rumboot_printf("res11 error\n");}	
	//	else 
	//	{res11=0;}
		iowrite32(data, rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS);
	if ((0x00000000)  != (ioread32(rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS)& 0x0000000F))
		{res12 =1;
		rumboot_printf("MPE_COMMON_D_BIAS = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_D_BIAS)); 
		rumboot_printf("Unexpected MPE_COMMON_D_BIAS error\n");}	
		else 
		{res12=0;}
	
        //rumboot_printf(" begin MPE_COMMON_BUF_FULL_SET \n");
		iowrite32(data, rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET);
		//rumboot_printf("MPE_COMMON_BUF_FULL_SET = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET)); 
		if ((data) !=((ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET))))
		{res13 =1;
		rumboot_printf("MPE_COMMON_BUF_FULL_SET = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET)); 
		rumboot_printf("Unexpected MPE_COMMON_BUF_FULL_SET error\n");}	
		else 
		{res13=0;}
		//rumboot_printf(" end MPE_COMMON_BUF_FULL_SET = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_SET)); 
	
		iowrite32(data, rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_RST);
	if ((0x00000000)  !=ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_RST))	
		{res14 =1;
		rumboot_printf("MPE_COMMON_BUF_FULL_RST = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_COMMON_BUF_FULL_RST)); 
		rumboot_printf("Unexpected MPE_COMMON_BUF_FULL_RST error\n");}	
		else 
		{res14=0;}

		iowrite32(data, rbase + MPE_MA_BASE + MPE_ADD_CountM);
	if ((0x00000000)  != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountM)& 0x00000FFF))
		{res15 =1;	  
		rumboot_printf("MPE_ADD_CountM = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountM)); 
		rumboot_printf("Unexpected MPE_ADD_CountM error\n");}	
		else 
		{res15=0;}	
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountM)& 0x00FFFFFF))
//		{res16 =1;
//		rumboot_printf("res16 error\n");}
//		else 
//		{res16=0;}


		iowrite32(data, rbase + MPE_MA_BASE + MPE_ADD_CountY);
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountY)& 0x00003FFF))
		{res17 =1;
		rumboot_printf("MPE_ADD_CountM = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountY)); 
		rumboot_printf("Unexpected MPE_ADD_CountM error\n");}	
		else 
		{res17=0;}
	

//	if ((0x00000001)  != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountY)& 0x00001FFF))
//		{res18 =1;	  
//		rumboot_printf("res18 error\n");}
//		else 
//		{res18=0;}

		iowrite32(data, rbase + MPE_MA_BASE + MPE_ADD_CountX);
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountX)& 0x00003FFF))
		{res19 =1;
		rumboot_printf("MPE_ADD_CountM = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountX)); 
		rumboot_printf("Unexpected MPE_ADD_CountX error\n");}
		else 
		{res19=0;}
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountX)& 0x00001FFF))
//		{res20 =1;
//		rumboot_printf("res20 error\n");}
//		else 
//		{res20=0;}

		iowrite32(data, rbase + MPE_MA_BASE + MPE_ADD_CountPLC);	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountPLC)& 0x00001FFF))
		{res21 =1;
		rumboot_printf("MPE_ADD_CountM = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountPLC)); 
		rumboot_printf("Unexpected MPE_ADD_CountPLC error\n");}
		else 
		{res21=0;}

//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountPLC)& 0x00001FFF))
//		{res22 =1;
//		rumboot_printf("res22 error\n");}
//		else 
//		{res22=0;}

		iowrite32(data, rbase + MPE_MA_BASE + MPE_ADD_CountLCZ);	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountLCZ)& 0x0000007F))
		{res23 =1;
		rumboot_printf("MPE_ADD_CountLCZ = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountLCZ)); 
		rumboot_printf("Unexpected MPE_ADD_CountLCZ error\n");}
		else 
		{res23=0;}		
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountLCZ)& 0x00001FFF))
//		{res24 =1;
//		rumboot_printf("res24 error\n");}
//		else 
//		{res24=0;}

		iowrite32(data, rbase + MPE_MA_BASE + MPE_ADD_CountPPS);	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountPPS)& 0x00000FFF))
		{res25 =1;
	    rumboot_printf("  MPE_ADD_CountPPS = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountPPS));
		rumboot_printf("Unexpected MPE_ADD_CountPPS error\n");}
		else 
		{res25=0;}
/*	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountPPS)& 0x00001FFF))
		{res26 =1;
		rumboot_printf("res26 error\n");}
		else 
		{res26=0;}	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountJ)& 0x00001FFF))
		{res27 =1;
		rumboot_printf("res27 error\n");}
		else 
		{res27=0;}
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_CMP_CountJ)& 0x00001FFF))
		{res28 =1;
		rumboot_printf("res28 error\n");}
		else 
		{res28=0;}
*/	
		iowrite32(data, rbase + MPE_MA_BASE + MPE_ADD_CountI1);	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountI1)& 0x00001FFF))
		{res29 =1;
	    rumboot_printf("  MPE_ADD_CountI1 = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountI1));
		rumboot_printf("Unexpected MPE_ADD_CountI1 error\n");}
		else 
		{res29=0;}	
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_CMP_CountI1)& 0x00001FFF))
//		{res30 =1;
//		rumboot_printf("res30 error\n");}
//		else 
//		{res30=0;}

		iowrite32(data, rbase + MPE_MA_BASE + MPE_ADD_CountI0);
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountI0)& 0x00001FFF))
		{res31 =1;
		rumboot_printf("  MPE_ADD_CountI0 = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_ADD_CountI0));
		rumboot_printf("Unexpected MPE_ADD_CountI0 error\n");}
		else 
		{res31=0;}

		iowrite32(data, rbase + MPE_MA_BASE + MPE_CMP_CountI0);	
	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountI0)& 0x00001FFF))
		{res32 =1;
		rumboot_printf("  MPE_CMP_CountI0 = %x\n",ioread32(rbase + MPE_MA_BASE + MPE_CMP_CountI0));
		rumboot_printf("Unexpected MPE_CMP_CountI0 error\n");}
		else 
		{res32=0;}
	
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE +MPE_NULL)& 0x00001FFF))
//		{res33 =1;
//		rumboot_printf("res33 error\n");}
//		else 
//		{res33=0;}

		
//	if ((0x00000000) != (ioread32(rbase + MPE_MA_BASE + MPE_STATUS)& 0x000FFFFF))
//		{res34 =1;
//		rumboot_printf("res34 error\n");}
//		else 
//		{res34=0;}
	
	
//------------------MPE_DMA Control  	-------------------------------------------
	
//------------------MPE_DMA wr Channel  -------------------------------------------

	  resr = 	/*res0 || res1|| res2 || res3 ||  res4 || res5 || res6 || res7 || res8 ||
				res9 ||  res10 ||*/ res11 || res12 || res13 || res14 || res15  /*|| res16 */|| res17 ||
				/*res18 || */res19 || /*res20 ||*/ res21  ||/* res22 || */res23  || /*res24  ||*/res25 || //res26 
				res29 || res31 || res32 ;//|| res34 ;
	
	res = resr ;
  
	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
			

 return 0; 
 } 
 
int nu_running_one(uintptr_t base, uint32_t mpe_offset, uint32_t mask, int end_number )
{
    uint32_t data = 1;

	
    for ( int i = 0; i< end_number; i++)
    {
        iowrite32((data & mask)    ,(base + mpe_offset));
        if (ioread32(base + mpe_offset) != (data & mask))
        {
            rumboot_printf("MPE running one ERROR data=%x\n", data);
            return 1;
        }
        data = (data << 1);
    }
    return 0;
}

int main() {
 
  int res,res1,res2,res3,res4;
  rumboot_printf("Test MPE regs run\n");
  rumboot_printf("Read REGS after reset\n");
  
	rumboot_printf("NA_CU_MPE_UNIT_MODE = %x\n",NA_CU_MPE_UNIT_MODE);
	rumboot_printf("NPE_BASE = %x\n",NPE_BASE);
	//rumboot_printf("NA_CU_REGS_BASE = %x\n",NA_CU_REGS_BASE);
	rumboot_printf("MPE_MA_BASE = %x\n",MPE_MA_BASE);
	rumboot_printf("MY_MPE_REGS_BASE = %x\n",MY_MPE_REGS_BASE);
	
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
   //write zero's 
   {rumboot_printf("Read REGS after  WRITE REGs ZERO's\n");}
   res2 = nu_mpe_wr_rd_regs(MY_MPE_REGS_BASE, 0x00000000);
   
  if ( res2 !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  else
 { rumboot_printf("Check READs after write ZERO's PASSED \n");}

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
 res4 = nu_running_one( (MY_MPE_REGS_BASE + MPE_MA_BASE),MPE_COMMON_BUF_FULL_SET,0xFFFFFFFF,32);	
	if  (res4 !=0)
	{rumboot_printf( "Check running ONE FAILED\n");
	return 1;
	}
	 else 
	{rumboot_printf( "Check 'running ONE' PASSED\n");	}
 
  res = res1 || res3 ||res4;// || res2  || res3 || res4;

  if ( res !=0)
  {rumboot_printf("Test FAILED\n");
	return 1;
  }
  
   rumboot_printf("Test PASSED\n");

   return 0;

}

