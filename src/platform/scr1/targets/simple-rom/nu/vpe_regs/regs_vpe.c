
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices/nu_cpdmac_lib.h>
#include <platform/devices/nu_lib.h>
#include <regs/regs_nu_vpe.h>
#include <platform/devices.h>


 int nu_vpe_rd_regs(uintptr_t base) {
	
	  
	  int resw0,resw1,resw2,resw3,resw4,resw5,resw6,resw7,resw8,resw9,resw10,resw11,resw12,
		  resw13,resw14,resw15,resw16,resw17,resw18,resw19,resw20, resw21,resw22,resw23,resw24,
		  resw25,resw26,resw27,resw28,resw29,resw30,resw31,resw32,resw33,resw34,resw35,resw36,
		  resw37,resw38,resw39,resw40, resw41,resw42,resw43,resw44,resw45,resw46,resw47,resw48,
		  resw49,resw50, resw51, resw52, resw53, resw54, resw55, resw56,resw57, resw58, resw59,
		  resw60,resw61,resw62,resw63;
	 int  src_res0,src_res1,src_res2,src_res3,src_res4,src_res5,src_res6,src_res7,src_res8,src_res9,src_res10,src_res11,src_res12,
		  src_res13,src_res14,src_res15,src_res16,src_res17,src_res18,src_res19,src_res20, src_res21,src_res22,src_res23,src_res24, src_res25, 
		  src_res26,src_res27;
	  		  
	  int resr,res;
	  int resw;
	  int32_t tmp;
	//----------------------NU_VPE---------------------------------------
	if ((0x00000000) !=(ioread32(base + NU_VPE +NU_VPE_NEXT_CNTX)& 0x00011131)) 	
		{resw0 =1;
	    tmp = ioread32(base + NU_VPE + NU_VPE_NEXT_CNTX);
		rumboot_printf("resw0_tmp =%d\n",tmp);
		rumboot_printf("resw0 error\n");}
		else 
		{resw0=0;}
	
	if ((0xabcd2021) !=(ioread32(base + NU_VPE + NU_VPE_DEV_ID))) 	
		{resw1 =1;
	    tmp = ioread32(base + NU_VPE +NU_VPE_DEV_ID);
		rumboot_printf("resw1_tmp =%d\n",tmp);
		rumboot_printf("resw1 error\n");}
		else 
		{resw1=0;}	
	
	if ((0x00000000) !=(ioread32(base + NU_VPE +NU_VPE_AXI_PARAM)& 0x0F07030F)) 	
		{resw2 =1;
	    tmp = ioread32(base + NU_VPE +NU_VPE_AXI_PARAM);
		rumboot_printf("resw1_tmp =%d\n",tmp);
		rumboot_printf("resw1 error\n");}
		else 
		{resw2=0;}
	
		if ((0x00000000) !=(ioread32(base + NU_VPE +NU_VPE_CUBE_SIZE))) 	
		{resw3 =1;
	    tmp = ioread32(base + NU_VPE +NU_VPE_CUBE_SIZE);
		rumboot_printf("resw3_tmp =%d\n",tmp);
		rumboot_printf("resw3 error\n");}
		else 
		{resw3=0;}
/*	
	if ((0x00000000) !=(ioread32(base + NU_VPE +NU_VPE_INT_STATUS)& 0x00001101)) 	
		{resw4 =1;
	    tmp = ioread32(base + NU_VPE +NU_VPE_STATUS);
		rumboot_printf("resw4_tmp =%d\n",tmp);
		rumboot_printf("resw4 error\n");}
		else 
		{resw4=0;}

	if ((0x00000000) !=(ioread32(base + NU_VPE +NU_VPE_INT_RESET)&  0x00001101)) 	
		{resw5 =1;
	    tmp= ioread32(base + NU_VPE +NU_VPE_INT_RESET);
		rumboot_printf("resw5_tmp =%d\n",tmp);
		rumboot_printf("resw5 error\n");}
		else 
		{resw5=0;}
 
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_INT_SET) & 0x00001101))	
		{resw6 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_INT_SET);
		rumboot_printf("resw6_tmp =%d\n",tmp);
		rumboot_printf("resw6 error\n");}
		else 
		{resw6=0;}
 
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_INT_MASK) & 0x00001101))	
		{resw7 =1;
	    tmp = ioread32(base + NU_VPE + NU_VPE_INT_MASK);
		rumboot_printf("resw7_tmp =%d\n",tmp);
		rumboot_printf("resw7 error\n");}
		else 
		{resw7=0;}

	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE_C) & 0x00001FFF))	
		{resw8 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE_C);
		rumboot_printf("resw8_tmp =%d\n",tmp);
		rumboot_printf("resw8 error\n");}
		else 
		resw8=0;

	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE_W) & 0x00001FFF))	
		{resw9 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE_W);
		rumboot_printf("resw9_tmp =%d\n",tmp);	
		rumboot_printf("resw9 error\n");}
		else 
		resw9=0;	
	if ((0x00000000) != ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE_H))	
		{resw10 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE_H);
		rumboot_printf("resw10_tmp =%d\n",tmp);	
		rumboot_printf("resw10 error\n");}
		else 
		resw10=0;	
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP_MODE) & 0x71F3FF13))	
		{resw11 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP_MODE);
		rumboot_printf("resw11_tmp =%d\n",tmp);	
		rumboot_printf("resw11 error\n");}
		else 
		resw11=0;
	//---------------------------------OP0----------------------------
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP0_CFG) & 0x00001FFF))	
		{resw12 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_CFG);
		rumboot_printf("resw12_tmp =%d\n",tmp);	
		rumboot_printf("resw12 error\n");}
		else 
		resw12=0;
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP0_ALU_CFG) & 0x00001F01))	
		{resw13 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_ALU_CFG);
		rumboot_printf("resw13_tmp =%d\n",tmp);	
		rumboot_printf("resw13 error\n");}
		else 
		{resw13=0;}
	
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL) & 0x0000FFFF))	
		{resw14 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL);
		rumboot_printf("resw14_tmp =%d\n",tmp);	
		rumboot_printf("resw14 error\n");}
		else 
		{resw14=0;}	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP0_MUL_CFG) & 0x00000001))	
		{resw15 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_MUL_CFG);
		rumboot_printf("resw15_tmp =%d\n",tmp);	
		rumboot_printf("resw15 error\n");}
		else 
		{resw15=0;}
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL)& 0x00001F01))	
		{resw16 =1;
		tmp = ioread32(base + NU_VPE  + NU_VPE_OP0_MUL_SRC_VAL);
		rumboot_printf("resw16_tmp =%d\n",tmp);	
		rumboot_printf("resw16 error\n");}
		else 
		{resw16=0;}
			
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP0_NORM_PARAM)& 0x0003013F))		
		{resw17 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_NORM_PARAM);
		rumboot_printf("resw17_tmp =%d\n",tmp);	
		rumboot_printf("resw17 error\n");}
		else 
		resw17=0;
	//-----------------------------------OP1---------------------------
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP1_CFG)& 0x0000007F))		
		{resw18 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_CFG);
		rumboot_printf("resw18_tmp =%d\n",tmp);	
		rumboot_printf("resw18 error\n");}
		else 
		resw18=0;
	if ((0x00000000) != (ioread32(base + NU_VPE +  NU_VPE_OP1_ALU_CFG)& 0x00001F01))	
		{resw19 =1;	
		tmp = ioread32(base + NU_VPE +  NU_VPE_OP1_ALU_CFG);
		rumboot_printf("resw19_tmp =%d\n",tmp);	
		rumboot_printf("resw19 error\n");}
		else 
		{resw19=0;	
		tmp = ioread32(base + NU_VPE +  NU_VPE_OP1_ALU_CFG);
		rumboot_printf("resw21_tmp =%d\n",tmp);	
	    }
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP1_ALU_SRC_VAL)& 0x0000FFFF))	
		{resw20 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_ALU_SRC_VAL);
		rumboot_printf("resw20_tmp =%d\n",tmp);		
		rumboot_printf("resw20 error\n");}
		else 
		{resw20=0;}		
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP1_MUL_CFG)& 0x00000001))	
		{resw21 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_MUL_CFG);
		rumboot_printf("resw21_tmp =%d\n",tmp);	
		rumboot_printf("resw21 error\n");}
		else 
		{resw21=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP1_MUL_SRC_VAL)& 0x0000FFFF))	
		{resw22 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_MUL_SRC_VAL);
		rumboot_printf("resw22_tmp =%d\n",tmp);	
		rumboot_printf("resw22 error\n");}
		else 
		{resw22=0;}

	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP1_NORM_PARAM)& 0x00031F01))	
		{resw23 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_NORM_PARAM);
		rumboot_printf("resw23_tmp =%d\n",tmp);	
		rumboot_printf("resw23 error\n");}
		else 
		{resw23=0;}	
	//---------------------------------OP2-----------------------------------
	if ((0x00000000) != (ioread32(base + NU_VPE +NU_VPE_OP2_CFG)& 0x000000FF))	
		{resw24 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP2_CFG);
		rumboot_printf("resw24_tmp =%d\n",tmp);	
		rumboot_printf("resw24 error\n");}
		else 
		{resw24=0;}
	
	if ((0x00000000)!=(ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CFG ) & 0x01F3FF33))
		{resw25 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CFG  );
		rumboot_printf("resw25_tmp =%d\n",tmp);
		rumboot_printf("resw25 error\n");}
		else 
		{resw25=0;}
	
	if ((0x00000053)!=(ioread32(base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL) & 0x0000007F)) 	//DOC not corresp
		{resw26 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL);
		rumboot_printf("resw26_tmp =%x\n",tmp);
		rumboot_printf("resw26 error\n");}		
		else 
		{resw26=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL);
		rumboot_printf("resw26_tmp =%x\n",tmp);}
		
	
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL)& 0x00003F01)) 	
		{resw27 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
		rumboot_printf("resw27_tmp =%d\n",tmp);	
		rumboot_printf("resw27 error\n");}
		else 
		{resw27=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
		rumboot_printf("resw27_tmp =%d\n",tmp);
	     }

	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL)& 0x0000FFFF))
		{resw28 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL);
		rumboot_printf("resw28_tmp =%d\n",tmp);	
		rumboot_printf("resw28 error\n");}
		else 
		{resw28=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL);
		rumboot_printf("resw28_tmp =%d\n",tmp);
		}
	if ((0x00000000) !=  (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL)& 0x00000001))
		{resw29 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL);
		rumboot_printf("resw29_tmp =%d\n",tmp);	
		rumboot_printf("resw29 error\n");}
		else 
		{resw29=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL);
		rumboot_printf("resw29_tmp =%d\n",tmp);
		}
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CMP_RESULT)& 0x0000FFFF))	
		{resw30 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CMP_RESULT);
		rumboot_printf("resw30 =%d\n",tmp);	
		rumboot_printf(resw30 error\n");}
		else 
		{resw30=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CMP_RESULT);
		rumboot_printf("resw30 =%d\n",tmp);
		}	

	if ((0x00000000)!=(ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CFG ) & 0x01F3FF33))
		{resw31 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CFG  );
		rumboot_printf("resw31_tmp =%d\n",tmp);
		rumboot_printf("resw31 error\n");}
		else 
		{resw31=0;}
 	
	if ((0x00000053)!=(ioread32(base + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL) & 0x0000007F)) 	//DOC not corresp
		{resw32 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL);
		rumboot_printf("resw32_tmp =%x\n",tmp);
		rumboot_printf("resw32 error\n");}
		
		else 
		{resw32=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL);
		rumboot_printf("resw32tmp =%x\n",tmp);}
		
	/*
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL)& 0x00003F01)) 	
		{resw33 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
		rumboot_printf("resw33_tmp =%d\n",tmp);	
		rumboot_printf("resw33 error\n");}
		else 
		{resw33=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
		rumboot_printf("resw33_tmp =%d\n",tmp);
	     }

	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL)& 0x0000FFFF))
		{resw34 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL);
		rumboot_printf("resw34_tmp =%d\n",tmp);	
		rumboot_printf("resw34 error\n");}
		else 
		{resw34=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL);
		rumboot_printf("resw34_tmp =%d\n",tmp);
		}
	if ((0x00000000) !=  (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL)& 0x00000001))
		{resw35 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL);
		rumboot_printf("resw35_tmp =%d\n",tmp);	
		rumboot_printf("resw35 error\n");}
		else 
		{resw35=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL);
		rumboot_printf("resw35_tmp =%d\n",tmp);
		}
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP2_NORM_PARAM)& 0x0000FFFF))	
		{resw36 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_NORM_PARAM);
		rumboot_printf("resw36_tmp =%d\n",tmp);	
		rumboot_printf(resw36 error\n");}
		else 
		{resw36=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_NORM_PARAM);
		rumboot_printf("resw36_tmp =%d\n",tmp);
		}	
//---------------------------LUT---------------------------------
	if ((0x00000000)!= (ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_CFG & 0x000301ff))
		{resw37 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_ACCESS_CFG);
		rumboot_printf("resw37_tmp =%d\n",tmp);
		rumboot_printf("resw37 error\n");}
		else 
		{resw37=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_CFG);
		rumboot_printf("resw37_tmp =%d\n",tmp);
		}
		
	if ((0x00000053)!=(ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_DATA) & 0x0000007F))
		{resw38 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_ACCESS_DATA);
		rumboot_printf("resw38_tmp =%x\n",tmp);	
		rumboot_printf("resw38 error\n");}	
		else 
		{resw38=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_ACCESS_DATA);
		rumboot_printf("cp1_res0_tmp =%x\n",tmp);
		}
		
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_LUT_CFG)&  0x00003F01)) 	
		{resw39 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_CFG);
		rumboot_printf("resw39_tmp =%d\n",tmp);	
		rumboot_printf("resw39 error\n");}
		else 
		{resw39=0;	
	    tmp= ioread32(base + NU_VPE + NU_VPE_LUT_CFG);
		rumboot_printf("resw39_tmp =%d\n",tmp);}			
		
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_LUT_SYM_POINT)&  0x00003F01)) 	
		{resw40 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_SYM_POINT);
		rumboot_printf("resw40_tmp =%d\n",tmp);	
		rumboot_printf("resw40 error\n");}
		else 
		{resw40=0;	
	    tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_SYM_POINT);
		rumboot_printf("resw40_tmp =%d\n",tmp);}	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_SHIFT)& 0x0000FFFF))
		{resw41 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_X_SHIFT);
		rumboot_printf("resw41_tmp =%d\n",tmp);
		rumboot_printf("resw41 error\n");}
		else 
		{resw41=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_X_SHIFT);
		rumboot_printf("resw41_tmp =%d\n",tmp);	
		}
	if ((0x00000000) !=  (ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT)& 0x00000001))
		{resw42 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB2_X_SHIFT);
		rumboot_printf("resw42_tmp =%d\n",tmp);	
		rumboot_printf("resw42 error\n");}
		else 
		{resw42=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB2_X_SHIFT);
		rumboot_printf("resw42_tmp =%d\n",tmp);
		}
	if ((0x00000000 =1;
		{resw43 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_X_START);
		rumboot_printf("resw43_tmp =%d\n",tmp);	
		rumboot_printf("resw48 error\n");}
		else 
		{resw43=0;	
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_X_START);
		rumboot_printf("resw43_tmp =%d\n",tmp);	
		}
		
	if ((0x00000000)!= (ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_END)& 0x000301ff))
		{resw44 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_X_END);
		rumboot_printf("resw44_tmp =%d\n",tmp);	
		rumboot_printf("resw44 error\n");}
		else 
		{resw44=0;	
        tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_X_END);
		rumboot_printf("resw44_tmp =%d\n",tmp);
		}
	
	if ((0x00000053)!=(ioread32(base + NU_VPE +  NU_VPE_LUT_TAB2_X_START) & 0x0000007F)) 		
		{resw45 =1;
		tmp= ioread32(base + NU_VPE +   NU_VPE_LUT_TAB2_X_START);
		rumboot_printf("resw45_tmp =%x\n",tmp);
		rumboot_printf("resw45 error\n");}	
		else 
		{resw45=0;		
		tmp= ioread32(base + NU_VPE +   NU_VPE_LUT_TAB2_X_START);
		rumboot_printf("resw45_tmp =%x\n",tmp);}
		
	if ((0x00000002)!=(ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_END) &  0x00000003))		
		{resw46 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_END);
		rumboot_printf("resw46_tmp =%d\n",tmp);
		rumboot_printf("resw46 error\n");}	
		else 
		resw46=0;	
	
	if ((0x00000000) != ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE))	
		{resw47 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALEL);
		rumboot_printf("resw47_tmp =%d\n",tmp);
		rumboot_printf("resw47 error\n");}
		else 
		{resw47=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE);
		rumboot_printf("resw47_tmp =%d\n",tmp);}
		
	if ((0x00000000) != ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT))	
		{resw48 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT);
		rumboot_printf("resw48_tmp =%d\n",tmp);
		rumboot_printf("resw48 error\n");}
		else 
		{resw48=0;	
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE;
		rumboot_printf("resw43_tmp =%d\n",tmp);}
		
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE)& 0x0000FFFF))	
		{resw49 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE);
		rumboot_printf("resw49_tmp =%d\n",tmp);	
		rumboot_printf("resw49 error\n");}
		else 
		{resw49=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE);
		rumboot_printf("resw49_tmp =%d\n",tmp);
	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT)& 0x0000003F))		
		{resw50 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT);
		rumboot_printf("resw50_tmp =%d\n",tmp);
		rumboot_printf("resw50 error\n");}
		else 
		{resw50=0;		
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT);
		rumboot_printf("resw50_tmp =%d\n",tmp);
		}
	if ((0x00000002) != (ioread32(base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL)& 0x00000003))		
		{resw51 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OUT_CVT_OFFSET_VAL);
		rumboot_printf("resw51_tmp =%d\n",tmp);
		rumboot_printf("resw51 error\n");}
		else 
		{resw51=0;			
		tmp= ioread32(base + NU_VPE +  NU_VPE_OUT_CVT_OFFSET_VAL));
		rumboot_printf("resw51_tmp =%d\n",tmp);
		}
	if ((0x00000000) != ioread32(base + NU_VPE_OUT_CVT_SCALE_VAL + ))		
		{resw52 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OUT_CVT_SCALE_VAL);
		rumboot_printf("resw52_tmp =%d\n",tmp);
		rumboot_printf("resw52 error\n");}
		else 
		{resw52=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OUT_CVT_SCALE_VAL);
		rumboot_printf("resw52_tmp =%d\n",tmp);		
		}
	if ((0x00000000) != ioread32(base + NU_VPE_OUT_CVT_TRUNC_VAL + ))		
		{resw53 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OUT_CVT_TRUNC_VAL);
		rumboot_printf("resw53_tmp =%d\n",tmp);
		rumboot_printf("resw53 error\n");}
		else 
		{resw53=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OUT_CVT_TRUNC_VAL);
		rumboot_printf("resw53_tmp =%d\n",tmp);		
		}		
		
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_INF_NUM_IN)))	
		{resw54 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_INF_NUM_IN);
		rumboot_printf("resw54_tmp =%d\n",tmp);		
		rumboot_printf("resw54 error\n");}
		else 
		{resw54=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_INF_NUM_IN);
		rumboot_printf("resw54_tmp =%d\n",tmp);
		}
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_NAN_NUM_IN)& 0x0000FFFF))	
		{resw55 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_NAN_NUM_IN);
		rumboot_printf("resw55_tmp =%d\n",tmp);	
		rumboot_printf("resw55 error\n");}
		else 
		{resw55=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_NAN_NUM_IN);
		rumboot_printf("resw55_tmp =%d\n",tmp);}
		
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_NAN_NUM_OUT)& 0x0000003F))		
		{resw56 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_NAN_NUM_OUT);
		rumboot_printf("resw56_tmp =%d\n",tmp);	
		rumboot_printf("resw56 error\n");}
		else 
		{resw56=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_NAN_NUM_OUT);
		rumboot_printf("resw56_tmp =%d\n",tmp);}

	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_SATUR_NUM_OUT)& 0x0000003F))		
		{resw57 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_SATUR_NUM_OUT);
		rumboot_printf("resw57_tmp =%d\n",tmp);	
		rumboot_printf("resw57 error\n");}
		else 
		{resw57=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_SATUR_NUM_OUT);
		rumboot_printf("resw56_tmp =%d\n",tmp);}
		
   if ((0x00000000) !=(ioread32(base + NU_VPE + LUT_CNT_OVRF)& 0x000301ff))		
		{resw58 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_OVRF);
		rumboot_printf("resw58_tmp =%d\n",tmp);
		rumboot_printf("resw58 error\n");}
		else 
		{resw58=0;			
	     tmp= ioread32(base + NU_VPE +  LUT_CNT_OVRFM);
		rumboot_printf("resw58_tmp =%d\n",tmp);}
		
   if ((0x00000000) !=(ioread32(base + NU_VPE + LUT_CNT_UNDF)& 0x000301ff))		
		{resw59 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_UNDF);
		rumboot_printf("resw59_tmp =%d\n",tmp);
		rumboot_printf("resw59 error\n");}
		else 
		{resw59=0;			
	     tmp= ioread32(base + NU_VPE +  LUT_CNT_UNDF);
		rumboot_printf("resw59_tmp =%d\n",tmp);}
*/		
if ((0x00000000) != (ioread32(base + NU_VPE + LUT_CNT_HIT_TAB0)& 0x0000003F))		
		{resw60 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_HIT_TAB0);
		rumboot_printf("resw60_tmp =%d\n",tmp);	
		rumboot_printf("resw60 error\n");}
		else 
		{resw60=0;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_HIT_TAB0);
		rumboot_printf("resw60_tmp =%d\n",tmp);}
	
   if ((0x00000000) !=(ioread32(base + NU_VPE + LUT_CNT_HIT_TAB1)& 0x000301ff))		
		{resw61 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_HIT_TAB1);
		rumboot_printf("resw61_tmp =%d\n",tmp);
		rumboot_printf("resw61 error\n");}
		else 
		{resw61=0;			
	     tmp= ioread32(base + NU_VPE +  LUT_CNT_HIT_TAB1);
		rumboot_printf("resw61_tmp =%d\n",tmp);}

   if ((0x00000000) !=(ioread32(base + NU_VPE + LUT_CNT_HIT_HYBR)& 0x000301ff))		
		{resw62 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_HIT_HYBR);
		rumboot_printf("resw62_tmp =%d\n",tmp);
		rumboot_printf("resw62 error\n");}
		else 
		{resw62=0;			
	     tmp= ioread32(base + NU_VPE +  LUT_CNT_HIT_HYBR);
		rumboot_printf("resw62_tmp =%d\n",tmp);}		
	//*/	
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_SOFT_RESET)& 0x000301ff))		
		{resw63 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_SOFT_RESET);
		rumboot_printf("resw63_tmp =%d\n",tmp);
		rumboot_printf("resw63 error\n");}
		else 
		{resw63=0;			
	     tmp= ioread32(base + NU_VPE +  NU_VPE_SOFT_RESET);
		rumboot_printf("resw63_tmp =%d\n",tmp);}			
	
	//----------------------Configuration VPE_DMA and Interrupts----------------------------------------------------	
	if ((0x00000000) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CFG)& 0x00030003)) 	
		{src_res0 =1;
	    tmp= (ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_DMA_CFG)& 0x00010003);
		rumboot_printf("src_res0_tmp =%x\n",tmp);
		rumboot_printf("src_res0 error\n");}
		else 
		{src_res0=0;}	
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_POINTER)& 0x00010001))
		{src_res1 =1;
		 tmp= (ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_DMA_POINTER)& 0x00010001);
		rumboot_printf("src_res1_tmp =%x\n",tmp);
		rumboot_printf("src_res1 error\n");}	
		else 
		{src_res1=0;} 
	
	if ((0x00000000) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_INT_STATUS)& 0x00000001)) 	
		{src_res2 =1;
	    tmp= ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_INT_STATUS);
		rumboot_printf("src_res2_tmp =%d\n",tmp);
		rumboot_printf("src_res2 error\n");}
		else 
		{src_res2=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_INT_RESET)& 0x00003fff)) 
		{src_res3 =1;
		tmp= ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_INT_RESET);
		rumboot_printf("src_res3_tmp =%x\n",tmp);
		rumboot_printf("src_res3 error\n");}	
		else 
		{src_res3=0;} 

	
	if ((0x00000000) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_INT_SET)& 0x00003fff))	
		{src_res4 =1;
		tmp= ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_INT_SET);
		rumboot_printf("src_res4_tmp =%x\n",tmp);	
		rumboot_printf("src_res4 error\n");}	
		else 
		{src_res4=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_INT_MASK)& 0x00003fff))
		{src_res5 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_INT_MASK);
		rumboot_printf("src_res5_tmp =%d\n",tmp);	
		rumboot_printf("src_res5 error\n");}	
		else 
		{src_res5=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_INT_MASK);
		rumboot_printf("src_res5_tmp =%d\n",tmp);	
	    }
	
	if ((0x00000001 & 0x0000003F) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BASE)& 0x0000003F))//not correspond in doc
		{src_res6 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BASE);
		rumboot_printf("src_res6_tmp =%d\n",tmp);	
		rumboot_printf("src_res6 error\n");}
		else 
		{src_res6=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BASE);
		rumboot_printf("src_res6_tmp =%x\n",tmp);		
		}
	if ((0x00000000) != ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CUBE_SIZE_C))
		{src_res7 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CUBE_SIZE_C);
		rumboot_printf("src_res7_tmp =%d\n",tmp);		
		rumboot_printf("src_res7 error\n");}
		else 
		{src_res7=0;	
		}

	if ((0x00000000) !=ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CUBE_SIZE_W))	
		{src_res8 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CUBE_SIZE_W);
		rumboot_printf("src_res8_tmp =%x\n",tmp);
		rumboot_printf("src_res8 error\n");}
		else 
		{src_res8=0;}

	if ((0x00000000) != ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CUBE_SIZE_H))
		{src_res9 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_CUBE_SIZE_H);
		rumboot_printf("src_res9_tmp =%x\n",tmp);	  
		rumboot_printf("src_res9 error\n");}
		else 
		{src_res9=0;}
	
	if ((0x00000000) != ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_X)) //???
		{src_res10 =1; 
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_X);
		rumboot_printf("src_res10_tmp =%x\n",tmp);
		rumboot_printf("src_res10 error\n");}
		else 
		{src_res10=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_X);
		rumboot_printf("src_res10_tmp =%x\n",tmp);	
		}
	if ((0x00000000) !=ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_Y))	
		{src_res11 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_Y);
		rumboot_printf("src_res11_tmp =%x\n",tmp);
		rumboot_printf("src_res11 error\n");}
		else 
		{src_res11=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_Y);
		rumboot_printf("src_res11_tmp =%x\n",tmp);	
		}

	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_Z))
		{src_res12 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_Z);
		rumboot_printf("src_res12_tmp =%x\n",tmp);	  
		rumboot_printf("src_res12 error\n");}
		else 
		{src_res12=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BORDER_Z);
		rumboot_printf("src_res12_tmp =%x\n",tmp);	
		}
	
	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_X))
		{src_res13 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_X);
		rumboot_printf("src_res13_tmp =%x\n",tmp);	
		rumboot_printf("src_res13 error\n");}
		else 
		{src_res13=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_X);
		rumboot_printf("src_res13_tmp =%x\n",tmp);
		}
	
	
	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_Y))
		{src_res14 =1;
	  	tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_Y);
		rumboot_printf("src_res14_tmp =%x\n",tmp);		
		rumboot_printf("src_res14 error\n");}
		else 
		{src_res14=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_Z))
		{src_res15 =1;
	  	tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_STRIDE_Z);
		rumboot_printf("src_res15_tmp =%x\n",tmp);		
		rumboot_printf("src_res15 error\n");}
		else 
		src_res15=0;
	
	if ((0x00000000) !=ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_DMA_FRAG_SIZE_ADDR))	
		{src_res16 =1;
	  	tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_FRAG_SIZE_ADDR);
		rumboot_printf("src_res16_tmp =%x\n",tmp);		
		rumboot_printf("src_res16 error\n");}
		else 
		{src_res16=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR))
		{src_res17 =1;
	  	tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR);
		rumboot_printf("src_res17_tmp =%x\n",tmp);		  
		rumboot_printf("src_res17 error\n");}
		else 
		{src_res17=0;}
	
	
	if ((0x00000000) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_DMA_XYZ_DRCT_ADDR) & 0x0000001F))
		{src_res18 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_XYZ_DRCT_ADDR );
		rumboot_printf("src_res18_tmp =%x\n",tmp);	
		rumboot_printf("src_res18 error\n");}
		else 
		{src_res18=0;
	    tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_XYZ_DRCT_ADDR );
		rumboot_printf("src_res18_tmp =%x\n",tmp);}
		
  if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_X ))
		{src_res19 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_X );
		rumboot_printf("src_res19_tmp =%d\n",tmp);	
		rumboot_printf("src_res19 error\n");}
		else 
		{src_res19=0;
	    tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_X );
		rumboot_printf("src_res19_tmp =%d\n",tmp);}
			

	if ((0x00000000) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Y)& 0x00030003)) 	
		{src_res20 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Y);
		rumboot_printf("op0_res20_tmp =%d\n",tmp);		
		rumboot_printf("op0_res20 error\n");}
		else 
		{src_res20=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Z)& 0x00010001))
		{src_res21 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_ST_SIZE_Z);
		rumboot_printf("src_res21_tmp =%d\n",tmp);	
		rumboot_printf("src_res21 error\n");}	
		else 
		{src_res21=0;}	
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_SIZE_X)& 0x00003fff)) 
		{src_res22 =1;
		rumboot_printf("src_res2 error\n");}	
		else 
		{src_res22=0;}	
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_SIZE_Y)& 0x00001fff))
		{src_res23 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_SIZE_Y);
		rumboot_printf("src_res23_tmp =%d\n",tmp);		
		rumboot_printf("src_res23 error\n");
	
		rumboot_printf("src_res23 error\n");}	
		else 
		{src_res23=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_SIZE_Y);
		rumboot_printf("src_res23_tmp =%d\n",tmp);			
		}
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_SIZE_Z)& 0x0000003F))
		{src_res24 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_SIZE_Z);
		rumboot_printf("src_res24_tmp =%d\n",tmp);	
		rumboot_printf("src_res24 error\n");}
		else 
		{src_res24=0;}
	if ((0x00000000) != ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X))
		{src_res25 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("src_res25_tmp =%d\n",tmp);	
		rumboot_printf("src_res25 error\n");}
		else 
		{src_res25=0;}

	if ((0x00000000) !=ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y))	
		{src_res26 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("src_res26_tmp =%d\n",tmp);	
		rumboot_printf("src_res26 error\n");}
		else 
		{src_res26=0;}

	if ((0x00000000) != ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Z))
		{src_res27 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("src_res27_tmp =%d\n",tmp);
		rumboot_printf("src_res27 error\n");}
		else 
		{src_res27=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA +  NU_VPE_DMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("src_res9_tmp =%d\n",tmp);		
		}
	

//----------------------------------------------------------------
	  			
	  resr =	src_res0 || src_res1|| src_res2 || src_res3 || src_res4 || src_res5 ||src_res6 || src_res7 || src_res8 ||
				src_res9 || src_res10 || src_res11 || src_res12 ||  src_res13 || src_res15 || 
				src_res16 || src_res17 || src_res18 || src_res19 || 
				src_res20 || src_res21 || src_res22 || src_res23 || src_res24 || src_res25 || src_res26 ; src_res27 ;
				
			
				
	  resw = 	resw0 || resw1||  resw2 || resw3 || resw4 || resw5 || resw6 || resw7 || resw8 || resw9 ||
				resw10 || resw11 || resw12 || resw13 || resw14 || resw15 || resw16 || 
				resw19 || resw20 ||resw21 || resw22 || resw23 || resw24 || resw25 || resw26 || resw27 || resw28 || resw29 || 
				resw30 || resw31 || resw32 ||resw33 || resw34 || resw35 || resw36 || resw37 ||
				resw38 || resw39 || resw40 ||resw41 || resw42 || resw43 || resw44 || resw45 || resw46 || resw47 || resw48 || resw49 ||
           		resw50 || resw51 ||resw52 || resw53 ||resw54 ||resw55  || resw56 || resw57 ||resw58 ||resw59 || resw60 || resw61 || resw62 ||resw63;
	  			
	
	res = resr  || resw ;

	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
	

 return 0; 
 } 



int nu_vpe_wr_regs(uintptr_t base, int32_t data) {
	  int res;
	  int op_res;
	  
	  int /*resw0,resw1,*/resw2,resw3,resw4,resw5,resw6,resw7,resw8,resw9,resw10,resw11,resw12,
		  resw13,resw14,resw15,resw16,resw17,resw18,resw19,resw20,resw21,resw22,resw23,resw24,resw25,
		  resw26,resw27,resw28,resw29,resw30,resw31,resw32,resw33,resw34,resw35,resw36,resw37,
		  resw38,resw39,resw40,resw41,resw42,resw43,resw44,resw45,resw46,resw47,resw48,resw49,resw50,
		  resw51,resw52,resw53, resw54,resw55,resw56,resw57,resw58,resw59,
		  resw60,resw61,resw62,resw63;
		  	  
	  int src_res0,src_res1,src_res2,src_res3,src_res4,src_res5,src_res6,src_res7,src_res8,src_res9,src_res10,src_res11,src_res12,
		  src_res13,src_res14,src_res15,src_res16,src_res17,src_res18,src_res19,src_res20,src_res21,src_res22,src_res23,src_res24, src_res25, 
		  src_res26,src_res27;
	 
	  int vpe_res0,vpe_res1,vpe_res2,/*vpe_res3,vpe_res4,vpe_res5,*/ vpe_res7,vpe_res8,vpe_res12; 
	  int resr;
	  int resw;
	  int32_t tmp;
	  int32_t tmpr;
	  tmpr =data;
	//----------------------VPE & DMA---------------------------------------	  

        iowrite32(tmpr, base + NU_VPE + NU_VPE_AXI_PARAM);
	if ((tmpr & 0x00000001 ) !=(ioread32(base + NU_VPE + NU_VPE_AXI_PARAM) & 0x00000001))	
		{resw2 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_AXI_PARAM);
		rumboot_printf("resw2_tmp =%d\n",tmp);
		rumboot_printf("resw2 error\n");}
		else 
		{resw2=0;}
	
        iowrite32(tmpr, base + NU_VPE + NU_VPE_CUBE_SIZE);
	if ((tmpr & 0x00001FFF ) !=(ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE) & 0x00001FFF))	
		{resw3 =1;
	    tmp = ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE);
		rumboot_printf("resw3_tmp =%d\n",tmp);
		rumboot_printf("resw3 error\n");}
		else 
		{resw3=0;}
	
        iowrite32(tmpr, base + NU_VPE + NU_VPE_INT_STATUS);
	if ((tmpr & 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_INT_STATUS) & 0x00001FFF))	
		{resw4 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_INT_STATUS);
		rumboot_printf("resw4_tmp =%d\n",tmp);
		rumboot_printf("resw4 error\n");}
		else 
		{resw4=0;}
	
       iowrite32(tmpr, base + NU_VPE + NU_VPE_INT_RESET);
	if ((tmpr & 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_INT_RESET) & 0x00001FFF))	
		{resw5 =1;
		rumboot_printf("resw5 error\n");}
		else 
		{resw5=0;}
	
	 iowrite32(tmpr, base + NU_VPE + NU_VPE_INT_SET);
	if ((tmpr) != ioread32(base + NU_VPE + NU_VPE_INT_SET))	
		{resw6 =1;
		rumboot_printf("resw6 error\n");}
		else 
		{resw6=0;}
	
	iowrite32(tmpr, base + NU_VPE + NU_VPE_INT_MASK);
	if ((tmpr & 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_INT_MASK) & 0x00001FFF))	
		{resw7 =1;
		rumboot_printf("resw7 error\n");}
		else 
		{resw7=0;}
	
	iowrite32(tmpr, base + NU_VPE + NU_VPE_CUBE_SIZE_C);
	if ((tmpr& 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE_C) & 0x00001FFF))	
		{resw8 =1;
		rumboot_printf("resw8 error\n");}
		else 
		{resw8=0;}
	
	iowrite32(tmpr, base + NU_VPE + NU_VPE_CUBE_SIZE_W);	
	if ((tmpr& 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE_W) & 0x00001FFF))	
		{resw9 =1;
		rumboot_printf("resw9 error\n");}
		else			
	    {resw9=0;}
	
	iowrite32(tmpr, base + NU_VPE + NU_VPE_CUBE_SIZE_H);	
	if ((tmpr & 0x00000100) !=(ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE_H) & 0x00000100))	
		{resw10 =1;
		rumboot_printf("resw10 error\n");}
		else 
		{resw10=0;}	
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP_MODE);
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP_MODE) & 0x00003FFF))	//DOC ??	
		{resw11 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP_MODE);
		rumboot_printf("resw11_tmp =%x\n",tmp);
		rumboot_printf("resw11 error\n");}
		else 
		{resw11=0;}
//------------------------------OP0-------------------------------------------	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_CFG);
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP0_CFG) & 0x00003FFF))	//DOC ??	
		{resw12 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_CFG);
		rumboot_printf("resw12_tmp =%x\n",tmp);
		rumboot_printf("resw12 error\n");}
		else 
		{resw12=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_ALU_CFG );	
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP0_ALU_CFG ) & 0x00003FFF))	//DOC ??	
		{resw13 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_ALU_CFG );
		rumboot_printf("resw13_tmp =%x\n",tmp);	
		rumboot_printf("resw13 error\n");}
		else 
		{resw13=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL);		
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL) & 0x00003FFF))	//DOC ??
		{resw14 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL);
		rumboot_printf("resw14_tmp =%x\n",tmp);		
		rumboot_printf("resw14 error\n");}
		else 
		{resw14=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_MUL_CFG);	
	if ((tmpr  & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP0_MUL_CFG) & 0x00003FFF))	//DOC ??
		{resw15 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_MUL_CFG);
		rumboot_printf("resw15_tmp =%x\n",tmp);		
		rumboot_printf("resw15 error\n");}
		else 
		{resw15=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL);	
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL) & 0x00003FFF))	//DOC ??
		{resw16 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL);
		rumboot_printf("resw16_tmp =%x\n",tmp);			
		rumboot_printf("resw16 error\n");}
		else 
		{resw16=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_NORM_PARAM);	

		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_NORM_PARAM);		
	if ((tmpr& 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP0_NORM_PARAM)& 0x00003FFF)) //DOC ??	
		{resw17 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP0_NORM_PARAM);
		rumboot_printf("resw17_tmp =%x\n",tmp);	
		rumboot_printf("resw17 error\n");}
		else 
		{resw17=0;}
//--------------------------------OP1-----------------------------------	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP1_CFG);
	if ((tmpr& 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP1_CFG)& 0x00003FFF)) //DOC ??	
		{resw18 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_CFG);
		rumboot_printf("resw18_tmp =%x\n",tmp);
		rumboot_printf("resw18 error\n");}
		else 
		{resw18=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP1_ALU_CFG);	
		if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP1_ALU_CFG)& 0x00003FFF))	
		{resw19 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_ALU_CFG);
		rumboot_printf("resw19_tmp =%d\n",tmp);	
		rumboot_printf("resw19 error\n");}
		else 
		{resw19=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP1_ALU_SRC_VAL);
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_OP1_ALU_SRC_VAL)& 0x00003FFF))	
		{resw20 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_ALU_SRC_VAL);
		rumboot_printf("resw20_tmp =%d\n",tmp);	
		rumboot_printf("resw20 error\n");}
		else 
		{resw20=0;}	

		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP1_MUL_CFG);	
	if ((tmpr & 0x00000003) != (ioread32(base + NU_VPE + NU_VPE_OP1_MUL_CFG)& 0x00000003))	
		{resw21 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_MUL_CFG);
		rumboot_printf("resw21_tmp =%d\n",tmp);	
		rumboot_printf("resw21 error\n");}
		else 
		{resw21=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP1_MUL_SRC_VAL);	
	if ((tmpr & 0x00000003) != (ioread32(base + NU_VPE + NU_VPE_OP1_MUL_SRC_VAL)& 0x00000003))	
		{resw22 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP1_MUL_SRC_VAL);
		rumboot_printf("resw22_tmp =%d\n",tmp);	
		rumboot_printf("resw22 error\n");}
		else 
		{resw22=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP1_NORM_PARAM);	
	if ((tmpr  & 0x01F3FF33 ) !=(ioread32(base + NU_VPE + NU_VPE_OP1_NORM_PARAM) & 0x01F3FF33))
		{resw23 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP1_NORM_PARAM);
		 rumboot_printf("resw23_tmp =%d\n",tmp);
		rumboot_printf("resw23 error\n");}
		else 
		{resw23=0;}
 
	//----------------------OP2--------------------------------------
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_CFG);		
	if ((tmpr  & 0x0000007F)!=(ioread32(base + NU_VPE + NU_VPE_OP2_CFG) & 0x0000007F)) 	
		{resw24 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_CFG);
		rumboot_printf("resw24_tmp =%x\n",tmp);
		rumboot_printf("resw24 error\n");}
		
		else 
		{resw24=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_CFG);
		rumboot_printf("resw24_tmp =%x\n",tmp);}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_CFG);
	if ((tmpr  & 0x00003F01)!=(ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CFG)& 0x00003F01)) 	
		{resw25 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CFG);
		rumboot_printf("resw25_tmp =%d\n",tmp);	
		rumboot_printf("resw25 error\n");}
		else 
		{resw25=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL);
	if ((tmpr &  0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL)& 0x0000FFFF))
		{resw26 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_SRC_VAL);
		rumboot_printf("resw26_tmp =%d\n",tmp);		
		rumboot_printf("resw26 error\n");}
		else 
		{resw26=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
	if ((tmpr & 0x00000001) !=  (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL)& 0x00000001))
		{resw27 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
		rumboot_printf("resw27_tmp =%d\n",tmp);
		rumboot_printf("resw27 error\n");}
		else 
		{resw27=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL);
	if ((tmpr & 0x0000FFFF ) !=(ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL)& 0x0000FFFF))	
		{resw28 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CVT_SCALE_VAL);
		rumboot_printf("resw28_tmp =%d\n",tmp);	
		rumboot_printf("resw28 error\n");}
		else 
		{resw28=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL);
	if ((tmpr & 0x000301ff) != (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL)& 0x0003013f))
		{resw29 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CVT_TRUNC_VAL);
		rumboot_printf("resw29_tmp =%d\n",tmp);
		rumboot_printf("resw29 error\n");}
		else 
		{resw29=0;}
	
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_CMP_RESULT);	
	if ((tmpr &  0x0000007F) !=(ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CMP_RESULT) & 0x0003013F)) 	
		{resw30 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CMP_RESULT);
		rumboot_printf("resw30_tmp =%x\n",tmp);	
		rumboot_printf("resw30 error\n");}	
		else 
		{resw30=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_CFG);	
	if ((tmpr &  0x00003F01 ) !=(ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CFG)&  0x00003F01)) 	
		{resw31 =1;
	    tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CFG);
		rumboot_printf("resw31_tmp =%d\n",tmp);
		rumboot_printf("resw31 error\n");}
		else 
		{resw31=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL);		
	if ((tmpr &   0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL)& 0x0000FFFF))
		{resw32 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_SRC_VAL);
		rumboot_printf("resw32_tmp =%d\n",tmp);	
		rumboot_printf("resw32 error\n");}
		else 
		{resw32=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL);	
	if ((tmpr &  0x00000001) !=  (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL)& 0x00000001))
		{resw33 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
		rumboot_printf("resw33_tmp =%d\n",tmp);	
		rumboot_printf("resw33 error\n");}
		else 
		{resw33=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL);	
	if ((tmpr &   0x0000FFFF) !=(ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL)& 0x0000FFFF))	
		{resw34 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_SCALE_VAL);
		rumboot_printf("resw34 =%d\n",tmp);	
		rumboot_printf("resw34 error\n");}
		else 
		{resw34=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL);	
	if ((tmpr &   0x000301ff)!= (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL)& 0x000301ff))
		{resw35 =1;	
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_TRUNC_VAL);
		rumboot_printf("resw35_tmp =%d\n",tmp);	
		rumboot_printf("resw35 error\n");}
		else 
		{resw35=0;}	
         	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_NORM_PARAM);	
	if ((tmpr &0x0000007F) !=(ioread32(base + NU_VPE + NU_VPE_OP2_NORM_PARAM) & 0x0000007F)) 	
		{resw36 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_NORM_PARAM);
		rumboot_printf("resw36_tmp =%d\n",tmp);
		rumboot_printf("resw36 error\n");}	
		else 
		{resw36=0;}
//-------------------------LUT----------------------------	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_ACCESS_CFG);		
	if ((tmpr&  0x00000003) !=(ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_CFG) &  0x00000003)) 		
		{resw37 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_ACCESS_CFG);
		rumboot_printf("resw37_tmp =%d\n",tmp);
		rumboot_printf("resw37 error\n");}	
		else 
		{resw37=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_ACCESS_DATA);	
	if ((tmpr) != ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_DATA))	
		{resw38 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_ACCESS_DATA);
		rumboot_printf("resw38_tmp =%d\n",tmp);	
		rumboot_printf("resw38 error\n");}
		else 
		{resw38=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_CFG);	
	if ((tmpr & 0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_LUT_CFG) & 0x0000FFFF))	
		{resw39 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_CFG);
		rumboot_printf("resw39_tmp =%x\n",tmp);	
		rumboot_printf("resw39 error\n");}
		else 
		{resw39=0;}
	
	    iowrite32(tmpr, base + NU_VPE +  NU_VPE_LUT_SYM_POINT);		
	if ((tmpr & 0x0000FFFF) != (ioread32(base + NU_VPE +  NU_VPE_LUT_SYM_POINT)& 0x0000FFFF))	
		{resw40 =1;
		tmp= ioread32(base + NU_VPE +   NU_VPE_LUT_SYM_POINT);
		rumboot_printf("resw40_tmp =%x\n",tmp);
		rumboot_printf("resw40 error\n");}
		else 
		{resw40=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_X_SHIFT);		
	if ((tmpr & 0x0000003F) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_SHIFT)& 0x0000003F))		
		{resw41 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_X_SHIFT)& 0x0000003F;
		rumboot_printf("resw41_tmp =%x\n",tmp);
		rumboot_printf("resw41 error\n");}
		else 
		{resw41=0;}	

	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT);	
	if ((tmpr & 0x00000003 ) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT)& 0x00000003))		
		{resw42 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB2_X_SHIFT);
		rumboot_printf("resw42_tmp =%x\n",tmp);
		rumboot_printf("resw42 error\n");}
		else 
		{resw42=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_X_START);
	if ((tmpr) != ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_START))		
		{resw43 =1;
	    tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_X_START);
		rumboot_printf("resw43_tmp =%x\n",tmp);	
		rumboot_printf("resw43 error\n");}
		else 
		{resw43=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_X_END);
	if ((tmpr & 0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_END) & 0x0000FFFF))	
		{resw44 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_END);
		rumboot_printf("resw44_tmp =%x\n",tmp);
		rumboot_printf("resw44 error\n");}
		else 
        {resw44=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_X_START);	
	if ((tmpr & 0x0000FFFF) != (ioread32(base + NU_VPE +NU_VPE_LUT_TAB2_X_START)& 0x0000FFFF))	
		{resw45 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_START);
		rumboot_printf("resw45 =%x\n",tmp);	
		rumboot_printf("resw45 error\n");}
		else 
		{resw45=0;}
	

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_X_END);
	if ((tmpr & 0x0000003F) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_END)& 0x0000003F))		
		{resw46 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB2_X_END);
		rumboot_printf("resw46_tmp =%x\n",tmp);	
		rumboot_printf("resw46 error\n");}
		else 
		{resw46=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE);
   if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE)& 0x000301ff))		
		{resw47 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_SLOPE_SCALE);
		rumboot_printf("resw47_tmp =%x\n",tmp);
		rumboot_printf("resw47 error\n");}
		else 
		{resw47=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT);
   if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT)& 0x000301ff))		
		{resw48 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB1_SLOPE_SHIFT);
		rumboot_printf("resw48_tmp =%x\n",tmp);
		rumboot_printf("resw48 error\n");}
		else 
		{resw48=0;}			

	iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE);
   if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE)& 0x000301ff))		
		{resw49 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB2_SLOPE_SCALE);
		rumboot_printf("resw49_tmp =%x\n",tmp);
		rumboot_printf("resw49 error\n");}
		else 
		{resw49=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT);
   if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT)& 0x000301ff))		
		{resw50 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_LUT_TAB2_SLOPE_SHIFT);
		rumboot_printf("resw50_tmp =%x\n",tmp);
		rumboot_printf("resw50 error\n");}
		else 
		{resw50=0;}	
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL);
  if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL)& 0x000301ff))		
		{resw51 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OUT_CVT_OFFSET_VAL);
		rumboot_printf("resw51_tmp =%x\n",tmp);
		rumboot_printf("resw51 error\n");}
		else 
		{resw51=0;}	
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OUT_CVT_SCALE_VAL);	
	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_OUT_CVT_SCALE_VAL)& 0x000301ff))		
		{resw52 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OUT_CVT_SCALE_VAL);
		rumboot_printf("resw52_tmp =%x\n",tmp);
		rumboot_printf("resw52 error\n");}
		else 
		{resw52=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL);	
	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL)& 0x000301ff))		
		{resw53 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OUT_CVT_TRUNC_VAL);
		rumboot_printf("resw53_tmp =%x\n",tmp);
		rumboot_printf("resw53 error\n");}
		else 
		{resw53=0;}

	iowrite32(tmpr, base + NU_VPE + NU_VPE_INF_NUM_IN);	
	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_INF_NUM_IN)& 0x000301ff))		
		{resw54 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_INF_NUM_IN);
		rumboot_printf("resw54_tmp =%x\n",tmp);
		rumboot_printf("resw54 error\n");}
		else 
		{resw54=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_NAN_NUM_IN);	
	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_NAN_NUM_IN)& 0x000301ff))		
		{resw55 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_NAN_NUM_IN);
		rumboot_printf("resw55_tmp =%x\n",tmp);
		rumboot_printf("resw55 error\n");}
		else 
		{resw55=0;}	

	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_NAN_NUM_OUT)& 0x000301ff))		
		{resw56 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_NAN_NUM_OUT);
		rumboot_printf("resw56_tmp =%x\n",tmp);
		rumboot_printf("resw56 error\n");}
		else 
		{resw56=0;}
	
	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_SATUR_NUM_OUT)& 0x000301ff))		
		{resw57 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_SATUR_NUM_OUT);
		rumboot_printf("resw57_tmp =%x\n",tmp);
		rumboot_printf("resw57 error\n");}
		else 
		{resw57=0;}

	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + LUT_CNT_OVRF)& 0x000301ff))		
		{resw58 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_OVRF);
		rumboot_printf("resw58_tmp =%x\n",tmp);
		rumboot_printf("resw58 error\n");}
		else 
		{resw58=0;}	
	
	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + LUT_CNT_UNDF)& 0x000301ff))		
		{resw59 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_UNDF);
		rumboot_printf("resw59_tmp =%x\n",tmp);
		rumboot_printf("resw59 error\n");}
		else 
		{resw59=0;}

	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + LUT_CNT_HIT_TAB0)& 0x000301ff))		
		{resw60 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_HIT_TAB0);
		rumboot_printf("resw60_tmp =%x\n",tmp);
		rumboot_printf("resw60 error\n");}
		else 
		{resw60=0;}
	
	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + LUT_CNT_HIT_TAB1)& 0x000301ff))		
		{resw61 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_HIT_TAB1);
		rumboot_printf("resw61_tmp =%x\n",tmp);
		rumboot_printf("resw61 error\n");}
		else 
		{resw61=0;}	
	
	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + LUT_CNT_HIT_HYBR)& 0x000301ff))		
		{resw62 =1;
		tmp= ioread32(base + NU_VPE +  LUT_CNT_HIT_HYBR);
		rumboot_printf("resw62_tmp =%x\n",tmp);
		rumboot_printf("resw62 error\n");}
		else 
		{resw62=0;}

	if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_SOFT_RESET)& 0x000301ff))		
		{resw63 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_SOFT_RESET);
		rumboot_printf("resw63_tmp =%x\n",tmp);
		rumboot_printf("resw63 error\n");}
		else 
		{resw63=0;}
	
	//----------------------Configuration VPE_DMA ------------------------------------------------------		

		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_CFG);		
	if ((tmpr & 0x00003fff) != (ioread32(base +  NU_VPE_DST_WDMA + NU_VPE_DMA_CFG)& 0x00003fff)) 
		{src_res0 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_CFG);
		rumboot_printf("src_res0_tmp =%x\n",tmp);	
		rumboot_printf("src_res0 error\n");}	
		else 
		{src_res0=0;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_CFG);
		rumboot_printf("src_res0_tmp =%x\n",tmp); }	

		
	    iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_POINTER);		
	if ((tmpr & 0x00003fff) !=(ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_POINTER)& 0x00003fff))	
		{src_res1 =1;
		rumboot_printf("src_res1 error\n");}	
		else 
		{src_res1=0;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_POINTER);
		rumboot_printf("src_res1_tmp =%x\n",tmp);
		}
	
	
	    iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_INT_STATUS);		
	if ((tmpr & 0x00003fff) !=(ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_STATUS)& 0x00003fff))	
		{src_res2 =1;
		rumboot_printf("src_res2 error\n");}	
		else 
		{src_res2=0;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_STATUS);
		rumboot_printf("src_res2_tmp =%x\n",tmp);
		}
	
		
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_INT_RESET);
	if ((tmpr& 0x00003fff) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_RESET)& 0x00003fff)) //DOC ???
		{src_res3 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_RESET);
		rumboot_printf("src_res3_tmp =%x\n",tmp);	
		rumboot_printf("src_res3 error\n");}	
		else 
		{src_res3=0;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_RESET);
		rumboot_printf("src_res2_tmp =%x\n",tmp);
		}


	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_INT_SET);		
	if ((tmpr) != ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_SET))
		{src_res4 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_SET);
		rumboot_printf("src_res4_tmp =%x\n",tmp);	
		rumboot_printf("src_res4 error\n");}
		else 
		{src_res4=0;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_SET);
		rumboot_printf("src_res4_tmp =%x\n",tmp);		
	    }
		
		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_INT_MASK);		
	if ((tmpr) != ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_MASK))
		{src_res5 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_MASK);
		rumboot_printf("src_res5_tmp =%x\n",tmp);	
		rumboot_printf("src_res5 error\n");}
		else 
		{src_res5=0;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_INT_MASK);
		rumboot_printf("src_res4_tmp =%x\n",tmp);		
	    }
		

	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BASE);	
	if ((tmpr& 0x0003FFFF) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BASE) & 0x0003FFFF))  //DOC ???	
		{src_res6 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BASE);	
		rumboot_printf("src_res6_tmp =%x\n",tmp);		  
		rumboot_printf("src_res6 error\n");}
		else 
		{src_res6=0;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BASE);	
		rumboot_printf("src_res5_tmp =%x\n",tmp);		
	    }
	
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_C);		
	if ((tmpr& 0x0003FFFF) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_C) & 0x0003FFFF))  //DOC ???
		{src_res7 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_C);	
		rumboot_printf("src_res7_tmp =%x\n",tmp);	
		rumboot_printf("src_res7 error\n");}
		else 
		{src_res7=0;}
	
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_C);		
	if ((tmpr& 0x0003FFFF) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_W) & 0x0003FFFF))  //DOC ???
		{src_res8 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_W);	
		rumboot_printf("src_res8_tmp =%x\n",tmp);	
		rumboot_printf("src_res8 error\n");}
		else 
		{src_res8=0;}
	
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_H);		
	if ((tmpr& 0x0003FFFF) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_H) & 0x0003FFFF))  //DOC ???
		{src_res9 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_CUBE_SIZE_H);	
		rumboot_printf("src_res9_tmp =%x\n",tmp);	
		rumboot_printf("src_res9 error\n");}
		else 
		{src_res9=0;}
	
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_X);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_X)& 0x0003FFF))  //DOC ???
		{src_res10 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_X);	
		rumboot_printf("src_res10_tmp =%x\n",tmp);		
		rumboot_printf("src_res10 error\n");}
		else 
		src_res10=0;
	
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Y);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Y)& 0x0003FFF))  //DOC ???
		{src_res11 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Y);	
		rumboot_printf("src_res11_tmp =%x\n",tmp);		
		rumboot_printf("src_res11 error\n");}
		else 
		src_res11=0;
	
iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Z);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Z)& 0x0003FFF))  //DOC ???
		{src_res12 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BORDER_Z);	
		rumboot_printf("src_res12_tmp =%x\n",tmp);		
		rumboot_printf("src_res12 error\n");}
		else 
		src_res12=0;
	
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_X);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_X)& 0x0003FFF))  //DOC ???
		{src_res13 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA +NU_VPE_DMA_STRIDE_X);
		rumboot_printf("src_res13_tmp =%x\n",tmp);	
		rumboot_printf("src_res13 error\n");}
		else 
		{src_res13=0;}
	
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_Y);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_Y)& 0x0003FFF))  //DOC ???
		{src_res14 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA +NU_VPE_DMA_STRIDE_Y);
		rumboot_printf("src_res14_tmp =%x\n",tmp);	
		rumboot_printf("src_res14 error\n");}
		else 
		{src_res14=0;}

	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_Z);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_DST_WDMA + NU_VPE_DMA_STRIDE_Z)& 0x0003FFF))  //DOC ???
		{src_res15 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA +NU_VPE_DMA_STRIDE_Z);
		rumboot_printf("src_res15_tmp =%x\n",tmp);	
		rumboot_printf("src_res15 error\n");}
		else 
		{src_res15=0;}
	
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_SIZE_ADDR);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_SIZE_ADDR)& 0x0003FFF))  //DOC ???
		{src_res16 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_SIZE_ADDR);	
		rumboot_printf("src_res16_tmp =%x\n",tmp);	
		rumboot_printf("src_res16 error\n");}
		else 
		{src_res16=0;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_SIZE_ADDR);	
		rumboot_printf("src_res19_tmp =%x\n",tmp);}
		
	iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR);		
	if ((tmpr & 0x0000001F) != (ioread32( base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR) & 0x0000001F))
		{src_res17 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR );
		rumboot_printf("src_res17_tmp =%x\n",tmp);	
		rumboot_printf("src_res17 error\n");}
		else 
		{src_res17=0;
	    tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_FRAG_LAST_SIZE_ADDR );
		rumboot_printf("src_res17_tmp =%x\n",tmp);}
		
		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_XYZ_DRCT_ADDR);		
	if ((tmpr) != ioread32( base + NU_VPE_DST_WDMA + NU_VPE_DMA_XYZ_DRCT_ADDR ))
		{src_res18 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_XYZ_DRCT_ADDR );
		rumboot_printf("src_res18_tmp =%x\n",tmp);	
		rumboot_printf("src_res18 error\n");}
		else 
		{src_res18=0;
	    tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_XYZ_DRCT_ADDR );
		rumboot_printf("src_res18_tmp =%x\n",tmp);}	

		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_ST_SIZE_X);	
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_ST_SIZE_X)& 0x00003fff)) 
		{src_res19 =1;
		rumboot_printf("op0_res19 error\n");}	
		else 
		{src_res19=0;}
	
		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_ST_SIZE_Y);	
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_ST_SIZE_Y)& 0x00003fff)) 
		{src_res20 =1;
		rumboot_printf("op0_res20 error\n");}	
		else 
		{src_res20=0;}
	
	
		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_ST_SIZE_Z);	
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_ST_SIZE_Z)& 0x00003fff)) 
		{src_res21 =1;
		rumboot_printf("op0_res21 error\n");}	
		else 
		{src_res21=0;}
	
		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_SIZE_X);	
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_SIZE_X)& 0x00003fff)) 
		{src_res22 =1;
		rumboot_printf("op0_res22 error\n");}	
		else 
		{src_res22=0;}
	
		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_SIZE_Y);	
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_SIZE_Y)& 0x00003fff)) 
		{src_res23 =1;
		rumboot_printf("op0_res20 error\n");}	
		else 
		{src_res23=0;}
	
	
		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_SIZE_Z);	
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_SIZE_Z)& 0x00003fff)) 
		{src_res24 =1;
		rumboot_printf("op0_res21 error\n");}	
		else 
		{src_res24=0;}


	
		iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X);	
	if ((tmpr & 0x0000003F ) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X)& 0x0000003F))
		{src_res25 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("op0_res25_tmp =%x\n",tmp);		
		rumboot_printf("op0_res25 error\n");}
		else 
		{src_res25=0;}
	
	   iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y);	
	if ((tmpr & 0x0000003F ) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y)& 0x0000003F))
		{src_res26 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("op0_res26_tmp =%x\n",tmp);		
		rumboot_printf("op0_res26 error\n");}
		else 
		{src_res26=0;}
	
	  iowrite32(tmpr, base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Z);	
	if ((tmpr & 0x0000003F ) != (ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Z)& 0x0000003F))
		{src_res27 =1;
		tmp= ioread32(base + NU_VPE_DST_WDMA + NU_VPE_DMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("op0_res27_tmp =%x\n",tmp);		
		rumboot_printf("op0_res27 error\n");}
		else 
		{src_res27=0;}
	
	
//----------------------------------------------------------------
	  		
		  resr = /*src_res0 || src_res1|| src_res2 || */src_res3 || src_res4 || src_res5 || src_res7 || src_res8 ||
				src_res9 || src_res10 || src_res11 || src_res12 || src_res13 || src_res14 || src_res15 || 
				src_res16 || src_res17 || src_res18 || src_res19 || src_res21 || src_res22 || src_res23 ||src_res24 ||
				src_res25 ||  src_res26 || src_res27;
				
				
	  resw = 	/*resw0 || resw1|| */resw2 || resw3 || resw4 || resw5 || resw6 || resw7 || resw8 || resw9 ||
				resw10 || resw11 || resw12 || resw13 || resw14 || resw15 || resw16 || resw17 ||
				resw18 || resw19  || resw20 || resw21 || resw22 || resw23 || resw24 || resw25 || resw26 || resw27 || resw28 ||
				resw29 ||resw30 || resw31 ||resw32 || resw33 ||resw34 ||resw35 || resw36 || resw37 || resw38 || resw39 ||resw40 ||resw41 ||
				resw42 || resw43 || resw44 || resw45 || resw46 || resw47 || resw48 || resw49 || resw50 || resw51 || resw52 ||resw53 || resw54 || resw55
				|| resw56 || resw57 || resw58 || resw59 || resw60 || resw61 || resw62  || resw63; 
	  			
	
	res = resr  || resw ;

	if( res ==1) {
	 rumboot_printf("READ_REGS ERROR\n");
    return -1;}
	

 return 0; 
 } 
int nu_apb_running_one(uintptr_t base, uint32_t vpe_offset, uint32_t mask, int end_number )
{
    uint32_t data = 1;

	
    for ( int i = 0; i< end_number; i++)
    {
        iowrite32((data & mask)    ,(base+vpe_offset));
        if (ioread32(base+vpe_offset) != (data & mask))
        {
            rumboot_printf("VPE apb running one ERROR data=%x\n", data);
            return 1;
        }
        data = (data << 1);
    }
    return 0;
}


int main() {
 
  int res1;
  int res2; 
  int res3;
  int res4;
  uint32_t   vpe_rdma_offset;
  rumboot_printf("Test VPE regs run\n");
  rumboot_printf("Read REGS after reset begin\n");
  
   // READ VPE REGs initial state 

	res1 = nu_vpe_rd_regs(NU_VPE_STANDALONE_BASE);
	if (res1 !=0)
	{rumboot_printf( "check after reset  FAILED\n ");}
	 else
	{rumboot_printf("Check after reset PASSED\n");} 
	{rumboot_printf( "\n" );}
	rumboot_printf("Write-READ begin\n");
	 
	res2 = nu_vpe_wr_regs(NU_VPE_STANDALONE_BASE,0xffffffff);
	if (res2 !=0)
	{rumboot_printf( "Check regs after ONEs write FAILED\n" );}
    else
	{rumboot_printf( "Check regs after ONEs write PASSED\n" );}
    {rumboot_printf( "\n" );}
	
	rumboot_printf("Write-READ zero's begin\n"); 
	
	res3 = nu_vpe_wr_regs(NU_VPE_STANDALONE_BASE,0x00000000 );
	if (res3 !=0)
	{rumboot_printf( "Check regs after  ZEROs write  FAILED\n" );}
    else
	{rumboot_printf( "Check regs after  ZEROs write  PASSED\n" );}
    {rumboot_printf( "\n" );}
	
	vpe_rdma_offset =  NU_VPE_OP2_RDMA + NU_VPE_DMA_BASE; // NU_VPE_RDMA_BASE_L;
	res4 = nu_apb_running_one( NU_VPE_STANDALONE_BASE, vpe_rdma_offset,0xffffffff,32);	
	if  (res4 !=0)
	{rumboot_printf( "Check running ONE FAILED\n");	}
	 else 
	{rumboot_printf( "Check 'running one' PASSED\n");	}

	if (( res1 !=0) |( res2 !=0) | ( res3 !=0) | ( res4 !=0) )
	{rumboot_printf("Test FAILED\n");
	return 1;		
 }	
   rumboot_printf("Test PASSED\n");  
   return 0;

}
