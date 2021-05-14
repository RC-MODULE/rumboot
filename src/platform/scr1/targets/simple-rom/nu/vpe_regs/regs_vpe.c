
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
	  int res;
	  int op_res;
	  
	  int resw0,resw1,resw2,resw3,resw4,resw5,resw6,resw7,resw8,resw9,resw10,resw11,resw12,
		  resw13,resw14,resw15,resw16,resw17,resw18,resw19,resw20, resw21,resw22;
	  int cp0_res0,cp0_res1,cp0_res2,cp0_res3,cp0_res4,cp0_res5;
	  int cp1_res0,cp1_res1,cp1_res2,cp1_res3,cp1_res4,cp1_res5;
	  int cp2_res0,cp2_res1,cp2_res2,cp2_res3,cp2_res4,cp2_res5,cp2_res6,cp2_res7,
		  cp2_res8,cp2_res9,cp2_res10,cp2_res11;
	  	  
	  int src_res0,src_res1,src_res2,src_res3,src_res4,src_res5,src_res6,src_res7,src_res8,src_res9,src_res10,src_res11,src_res12,
		  src_res13,src_res14,src_res15,src_res16,src_res17,src_res18,src_res19, src_res21,src_res22;
	  int op0_res0,op0_res1,op0_res2,op0_res3,op0_res4,op0_res5,op0_res6,op0_res7,op0_res8,op0_res9,op0_res10,op0_res11,op0_res12,
		  op0_res13,op0_res14,op0_res15,op0_res16,op0_res17,op0_res18,op0_res19,op0_res21,op0_res22;
	  int op1_res0,op1_res1,op1_res2,op1_res3,op1_res4,op1_res5,op1_res6,op1_res7,op1_res8,op1_res9,op1_res10,op1_res11,op1_res12,
		  op1_res13,op1_res14,op1_res15,op1_res16,op1_res17,op1_res18,op1_res19,op1_res21,op1_res22;	  
	  int op2_res0,op2_res1,op2_res2,op2_res3,op2_res4,op2_res5,op2_res6,op2_res7,op2_res8,op2_res9,op2_res10,op2_res11,op2_res12,
		  op2_res13,op2_res14,op2_res15,op2_res16,op2_res17,op2_res18,op2_res19,op2_res20,op2_res21,op2_res22 ;	
	  int lut_res0,/*lut_res1,*/lut_res2,lut_res3,lut_res4,lut_res5,lut_res6,lut_res7,lut_res8,lut_res9,lut_res10,lut_res11,lut_res12, 
		  lut_res13;
	  int vpe_res0,vpe_res1,vpe_res2,vpe_res3,vpe_res4,vpe_res5,vpe_res6, vpe_res7,vpe_res8,vpe_res9, vpe_res10, vpe_res11, vpe_res12;
		  
	  int resr;
	  int resw;
	  int32_t tmp;
	//----------------------NU_VPE---------------------------------------	  
	if ((0x00000000) !=(ioread32(base + NU_VPE +NU_VPE_STATUS)& 0x00030003)) 	
		{resw0 =1;
	    tmp = ioread32(base + NU_VPE +NU_VPE_STATUS);
		rumboot_printf("resw0_tmp =%d\n",tmp);
		rumboot_printf("resw0 error\n");}
		else 
		{resw0=0;}
 
	if ((0x00000000) !=(ioread32(base + NU_VPE +NU_VPE_POINTER)& 0x00010001)) 	
		{resw1 =1;
	    tmp= ioread32(base + NU_VPE +NU_VPE_POINTER);
		rumboot_printf("resw1_tmp =%d\n",tmp);
		rumboot_printf("resw1 error\n");}
		else 
		{resw1=0;}
 
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP_ENABLE) & 0x00000001))	
		{resw2 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP_ENABLE);
		rumboot_printf("resw2_tmp =%d\n",tmp);
		rumboot_printf("resw2 error\n");}
		else 
		{resw2=0;}
 
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_DATA_W_OUT) & 0x00001FFF))	
		{resw3 =1;
	    tmp = ioread32(base + NU_VPE + NU_VPE_DATA_W_OUT);
		rumboot_printf("resw3_tmp =%d\n",tmp);
		rumboot_printf("resw3 error\n");}
		else 
		{resw3=0;}

	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_DATA_H_OUT) & 0x00001FFF))	
		{resw4 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_DATA_H_OUT);
		rumboot_printf("resw4_tmp =%d\n",tmp);
		rumboot_printf("resw4 error\n");}
		else 
		resw4=0;

	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_DATA_C_OUT) & 0x00001FFF))	
		{resw5 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_DATA_C_OUT);
		rumboot_printf("resw5_tmp =%d\n",tmp);	
		rumboot_printf("resw5 error\n");}
		else 
		resw5=0;	
	if ((0x00000000) != ioread32(base + NU_VPE + NU_VPE_WDMA_DST_BASE_L))	
		{resw6 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_DST_BASE_L);
		rumboot_printf("resw6_tmp =%d\n",tmp);	
		rumboot_printf("resw6 error\n");}
		else 
		resw6=0;	
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_WDMA_DST_LINE_STRIDE) & 0x00001FFF))	
		{resw7 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_DST_LINE_STRIDE);
		rumboot_printf("resw7_tmp =%d\n",tmp);	
		rumboot_printf("resw7 error\n");}
		else 
		resw7=0;
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_WDMA_DST_VECTOR_STRIDE) & 0x00001FFF))	
		{resw8 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_DST_VECTOR_STRIDE);
		rumboot_printf("resw8_tmp =%d\n",tmp);	
		rumboot_printf("resw8 error\n");}
		else 
		resw8=0;
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_WDMA_ELEM_STRIDE) & 0x00001FFF))	
		{resw9 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_ELEM_STRIDE);
		rumboot_printf("resw9_tmp =%d\n",tmp);	
		rumboot_printf("resw9 error\n");}
		else 
		{resw9=0;}
	
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_WDMA_DST_CFG) & 0x00000100))	
		{resw10 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_DST_CFG);
		rumboot_printf("resw10_tmp =%d\n",tmp);	
		rumboot_printf("resw10 error\n");}
		else 
		{resw10=0;}	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_X) & 0x00003FFF))	
		{resw11 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_X);
		rumboot_printf("resw11_tmp =%d\n",tmp);	
		rumboot_printf("resw11 error\n");}
		else 
		{resw11=0;}
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Y)& 0x00003FFF))	
		{resw12 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Y);
		rumboot_printf("resw12_tmp =%d\n",tmp);	
		rumboot_printf("resw12 error\n");}
		else 
		{resw12=0;}
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Z)& 0x00003FFF))	
		{resw13 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Z);
		rumboot_printf("resw13_tmp =%d\n",tmp);	
		rumboot_printf("resw13 error\n");}
		else 
		resw13=0;
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_X)& 0x00003FFF))	
		{resw14 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_X);
		rumboot_printf("resw14_tmp =%d\n",tmp);	
		rumboot_printf("resw14 error\n");}
		else 
		resw14=0;
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Y)& 0x00003FFF))		
		{resw15 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Y);
		rumboot_printf("resw15_tmp =%d\n",tmp);	
		rumboot_printf("resw15 error\n");}
		else 
		resw15=0;
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Z)& 0x00003FFF))		
		{resw16 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Z);
		rumboot_printf("resw16_tmp =%d\n",tmp);	
		rumboot_printf("resw16 error\n");}
		else 
		resw16=0;
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_X)& 0x00003FFF))	
		{resw17 =1;	
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("resw17_tmp =%d\n",tmp);	
		rumboot_printf("resw17 error\n");}
		else 
		{resw17=0;	
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("resw17_tmp =%d\n",tmp);	
	    }
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Y)& 0x00003FFF))	
		{resw18 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("resw18_tmp =%d\n",tmp);		
		rumboot_printf("resw18 error\n");}
		else 
		{resw18=0;}	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Z)& 0x00003FFF))	
		{resw19 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("resw19_tmp =%d\n",tmp);	
		rumboot_printf("resw19 error\n");}
		else 
		{resw19=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_ELMT_LAST_SIZE_ADDR)& 0x00003FFF))	
		{resw20 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_ELMT_LAST_SIZE_ADDR);
		rumboot_printf("resw20_tmp =%d\n",tmp);	
		rumboot_printf("resw20 error\n");}
		else 
		{resw20=0;}

	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_ELMT_SIZE_ADDR)& 0x00003FFF))	
		{resw21 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_ELMT_SIZE_ADDR);
		rumboot_printf("resw21_tmp =%d\n",tmp);	
		rumboot_printf("resw21 error\n");}
		else 
		{resw21=0;}	
	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_WDMA_XYZ_DRCT_ADDR)& 0x00003FFF))	
		{resw22 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_XYZ_DRCT_ADDR);
		rumboot_printf("resw22_tmp =%d\n",tmp);	
		rumboot_printf("resw22 error\n");}
		else 
		{resw22=0;}
	//----------------------OP---------------------------------------
	if ((0x00000000)!=(ioread32(base + NU_VPE + NU_VPE_OP_MODE) & 0x01F3FF33))
		{op_res =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP_MODE);
		rumboot_printf("op_res_tmp =%d\n",tmp);
		rumboot_printf("op_res error\n");}
		else 
		{op_res=0;}
 
	//----------------------OP0--------------------------------------
	
	if ((0x00000053)!=(ioread32(base + NU_VPE + NU_VPE_OP0_CFG) & 0x0000007F)) 	//DOC not corresp
		{cp0_res0 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_CFG);
		rumboot_printf("cp0_res0_tmp =%x\n",tmp);
		rumboot_printf("cp0_res0 error\n");}
		
		else 
		{cp0_res0=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_CFG);
		rumboot_printf("cp0_res0_tmp =%x\n",tmp);}
		
	
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP0_ALU_CFG)& 0x00003F01)) 	
		{cp0_res1 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_ALU_CFG);
		rumboot_printf("cp0_res1_tmp =%d\n",tmp);	
		rumboot_printf("cp0_res1 error\n");}
		else 
		{cp0_res1=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_ALU_CFG);
		rumboot_printf("cp0_res1_tmp =%d\n",tmp);
	     }

	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL)& 0x0000FFFF))
		{cp0_res2 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL);
		rumboot_printf("cp0_res2_tmp =%d\n",tmp);	
		rumboot_printf("cp0_res2 error\n");}
		else 
		{cp0_res2=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL);
		rumboot_printf("cp0_res2_tmp =%d\n",tmp);
		}
	if ((0x00000000) !=  (ioread32(base + NU_VPE + NU_VPE_OP0_MUL_CFG)& 0x00000001))
		{cp0_res3 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_MUL_CFG);
		rumboot_printf("cp0_res3_tmp =%d\n",tmp);	
		rumboot_printf("cp0_res3 error\n");}
		else 
		{cp0_res3=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_MUL_CFG);
		rumboot_printf("cp0_res3_tmp =%d\n",tmp);
		}
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL)& 0x0000FFFF))	
		{cp0_res4 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL);
		rumboot_printf("cp0_res4_tmp =%d\n",tmp);	
		rumboot_printf("cp0_res4 error\n");}
		else 
		{cp0_res4=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL);
		rumboot_printf("cp0_res4_tmp =%d\n",tmp);
		}	

	if ((0x00000000)!= (ioread32(base + NU_VPE + NU_VPE_OP0_NORM_PARAM)& 0x000301ff))
		{cp0_res5 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_NORM_PARAM);
		rumboot_printf("cp0_res5_tmp =%d\n",tmp);
		rumboot_printf("cp0_res5 error\n");}
		else 
		{cp0_res5=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_NORM_PARAM);
		rumboot_printf("cp0_res5_tmp =%d\n",tmp);
		}
	
//----------------------OP1--------------------------------------	
	if ((0x00000053)!=(ioread32(base + NU_VPE + NU_VPE_OP1_CFG) & 0x0000007F))
		{cp1_res0 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_CFG);
		rumboot_printf("cp1_res0_tmp =%x\n",tmp);	
		rumboot_printf("cp1_res0 error\n");}	
		else 
		{cp1_res0=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_CFG);
		rumboot_printf("cp1_res0_tmp =%x\n",tmp);
		}
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP1_ALU_CFG)&  0x00003F01)) 	
		{cp1_res1 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP1_ALU_CFG);
		rumboot_printf("cp1_res1_tmp =%d\n",tmp);	
		rumboot_printf("cp1_res1 error\n");}
		else 
		{cp1_res1=0;	
	    tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_ALU_CFG);
		rumboot_printf("cp1_res1_tmp =%d\n",tmp);}	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP1_ALU_SRC_VAL)& 0x0000FFFF))
		{cp1_res2 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_ALU_SRC_VAL);
		rumboot_printf("cp1_res2_tmp =%d\n",tmp);
		rumboot_printf("cp1_res2 error\n");}
		else 
		{cp1_res2=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_ALU_SRC_VAL);
		rumboot_printf("cp1_res2_tmp =%d\n",tmp);	
		}
	if ((0x00000000) !=  (ioread32(base + NU_VPE + NU_VPE_OP1_MUL_CFG)& 0x00000001))
		{cp1_res3 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_MUL_CFG);
		rumboot_printf("cp1_res3_tmp =%d\n",tmp);	
		rumboot_printf("cp1_res3 error\n");}
		else 
		{cp1_res3=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_MUL_CFG);
		rumboot_printf("cp1_res3_tmp =%d\n",tmp);
		}
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP1_MUL_SRC_VAL)& 0x0000FFFF))	
		{cp1_res4 =1;
	tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_MUL_SRC_VAL);
		rumboot_printf("cp1_res4_tmp =%d\n",tmp);	
		rumboot_printf("cp1_res4 error\n");}
		else 
		{cp1_res4=0;	
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_MUL_SRC_VAL);
		rumboot_printf("cp1_res4_tmp =%d\n",tmp);	
		}
	if ((0x00000000)!= (ioread32(base + NU_VPE + NU_VPE_OP1_NORM_PARAM)& 0x000301ff))
		{cp1_res5 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_NORM_PARAM);
		rumboot_printf("cp1_res5_tmp =%d\n",tmp);	
		rumboot_printf("cp1_res5 error\n");}
		else 
		{cp1_res5=0;	
        tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_NORM_PARAM);
		rumboot_printf("cp1_res5_tmp =%d\n",tmp);
		}
//----------------------OP2--------------------------------------	
	if ((0x00000053)!=(ioread32(base + NU_VPE + NU_VPE_OP2_CFG) & 0x0000007F)) 		
		{cp2_res0 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_CFG);
		rumboot_printf("cp2_res0_tmp =%x\n",tmp);
		rumboot_printf("cp2_res0 error\n");}	
		else 
		{cp2_res0=0;		
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_CFG);
		rumboot_printf("cp2_res0_tmp =%x\n",tmp);}
		
	if ((0x00000002)!=(ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CFG) &  0x00000003)) /*DOC??? */		
		{cp2_res1 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CFG);
		rumboot_printf("cp2_res1_tmp =%d\n",tmp);
		rumboot_printf("cp2_res1 error\n");}	
		else 
		cp2_res1=0;	
	
	if ((0x00000000) != ioread32(base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL))	/*DOC ??? */
		{cp2_res2 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL);
		rumboot_printf("cp2_res2_tmp =%d\n",tmp);
		rumboot_printf("cp2_res2 error\n");}
		else 
		{cp2_res2=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL);
		rumboot_printf("cp2_res2_tmp =%d\n",tmp);}
		
	if ((0x00000000) != ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL))	
		{cp2_res3 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
		rumboot_printf("cp2_res3_tmp =%d\n",tmp);
		rumboot_printf("cp2_res3 error\n");}
		else 
		{cp2_res3=0;	
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
		rumboot_printf("cp2_res3_tmp =%d\n",tmp);}
		
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL)& 0x0000FFFF))	
		{cp2_res4 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL);
		rumboot_printf("cp2_res4_tmp =%d\n",tmp);	
		rumboot_printf("cp2_res4 error\n");}
		else 
		{cp2_res4=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL);
		rumboot_printf("cp2_res4_tmp =%d\n",tmp);
		}
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL)& 0x0000003F))		
		{cp2_res5 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL);
		rumboot_printf("cp2_res5_tmp =%d\n",tmp);
		rumboot_printf("cp2_res5 error\n");}
		else 
		{cp2_res5=0;		
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL);
		rumboot_printf("cp2_res5_tmp =%d\n",tmp);
		}
	if ((0x00000002) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CFG)& 0x00000003))		
		{cp2_res6 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CFG);
		rumboot_printf("cp2_res6_tmp =%d\n",tmp);
		rumboot_printf("cp2_res6 error\n");}
		else 
		{cp2_res6=0;			
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CFG);
		rumboot_printf("cp2_res6_tmp =%d\n",tmp);
		}
	if ((0x00000000) != ioread32(base + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL))		
		{cp2_res7 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_SRC_VAL);
		rumboot_printf("cp2_res7_tmp =%d\n",tmp);
		rumboot_printf("cp2_res7 error\n");}
		else 
		{cp2_res7=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_SRC_VAL);
		rumboot_printf("cp2_res7_tmp =%d\n",tmp);	
		}
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL)))	
		{cp2_res8 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
		rumboot_printf("cp2_res8_tmp =%d\n",tmp);		
		rumboot_printf("cp2_res8 error\n");}
		else 
		{cp2_res8=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
		rumboot_printf("cp2_res8_tmp =%d\n",tmp);
		}
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL)& 0x0000FFFF))	
		{cp2_res9 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_SCALE_VAL);
		rumboot_printf("cp2_res9_tmp =%d\n",tmp);	
		rumboot_printf("cp2_res9 error\n");}
		else 
		{cp2_res9=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_SCALE_VAL);
		rumboot_printf("cp2_res9_tmp =%d\n",tmp);}
		
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL)& 0x0000003F))		
		{cp2_res10 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_TRUNC_VAL);
		rumboot_printf("cp2_res10_tmp =%d\n",tmp);	
		rumboot_printf("cp2_res10 error\n");}
		else 
		{cp2_res10=0;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_TRUNC_VAL);
		rumboot_printf("cp2_res10_tmp =%d\n",tmp);}
		
   if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_OP2_NORM_PARAM)& 0x000301ff))		
		{cp2_res11 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_NORM_PARAM);
		rumboot_printf("cp2_res11_tmp =%d\n",tmp);
		rumboot_printf("cp2_res11 error\n");}
		else 
		{cp2_res11=0;			
	     tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_NORM_PARAM);
		rumboot_printf("cp2_res11_tmp =%d\n",tmp);}

	//----------------------Configuration SRC_RDMA ------------------------------------------------------	
	if ((0x00000000) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_STATUS)& 0x00030003)) 	
		{src_res0 =1;
		rumboot_printf("src_res0 error\n");}
		else 
		{src_res0=0;}
	
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_POINTER)& 0x00010001))
		{src_res1 =1;
		 tmp= (ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_RDMA_POINTER)& 0x00010001);
		rumboot_printf("src_res1_tmp =%x\n",tmp);
		rumboot_printf("src_res1 error\n");}	
		else 
		{src_res1=0;} 
	
	if ((0x00000000) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_OP_ENABLE)& 0x00000001)) 	
		{src_res2 =1;
	    tmp= ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_RDMA_OP_ENABLE);
		rumboot_printf("src_res2_tmp =%d\n",tmp);
		rumboot_printf("src_res2 error\n");}
		else 
		{src_res2=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_W_IN)& 0x00003fff)) 
		{src_res3 =1;
		tmp= ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_W_IN);
		rumboot_printf("src_res3_tmp =%x\n",tmp);
		rumboot_printf("src_res3 error\n");}	
		else 
		{src_res3=0;} 

	
	if ((0x00000000) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_H_IN)& 0x00003fff))	
		{src_res4 =1;
		tmp= ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_H_IN);
		rumboot_printf("src_res4_tmp =%x\n",tmp);	
		rumboot_printf("src_res4 error\n");}	
		else 
		{src_res4=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_C_IN)& 0x00003fff)) /* DOC*/
		{src_res5 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_C_IN);
		rumboot_printf("src_res5_tmp =%d\n",tmp);	
		rumboot_printf("src_res5 error\n");}	
		else 
		{src_res5=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_C_IN);
		rumboot_printf("src_res5_tmp =%d\n",tmp);	
	    }
	
	if ((0x00000001) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_CFG)& 0x0000003F))//not correspond in doc
		{src_res6 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_CFG);
		rumboot_printf("src_res6_tmp =%d\n",tmp);	
		rumboot_printf("src_res6 error\n");}
		else 
		{src_res6=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_CFG);
		rumboot_printf("src_res6_tmp =%d\n",tmp);		
		}
	if ((0x00000000) != ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BASE_L))
		{src_res7 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BASE_L);
		rumboot_printf("src_res7_tmp =%d\n",tmp);		
		rumboot_printf("src_res7 error\n");}
		else 
		{src_res7=0;	
		}

	if ((0x00000000) !=ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_LINE_STRIDE))	
		{src_res8 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_LINE_STRIDE);
		rumboot_printf("src_res8_tmp =%x\n",tmp);
		rumboot_printf("src_res8 error\n");}
		else 
		{src_res8=0;}

	if ((0x00000000) != ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_VECTOR_STRIDE))
		{src_res9 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);
		rumboot_printf("src_res9_tmp =%x\n",tmp);	  
		rumboot_printf("src_res9 error\n");}
		else 
		{src_res9=0;}
	
	if ((0x00000000) != ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_ELEM_STRIDE)) //???
		{src_res10 =1; 
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_ELEM_STRIDE);
		rumboot_printf("src_res10_tmp =%x\n",tmp);
		rumboot_printf("src_res10 error\n");}
		else 
		{src_res10=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_ELEM_STRIDE);
		rumboot_printf("src_res10_tmp =%x\n",tmp);	
		}
	if ((0x00000000) !=ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X))	
		{src_res11 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);
		rumboot_printf("src_res11_tmp =%x\n",tmp);
		rumboot_printf("src_res11 error\n");}
		else 
		{src_res11=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);
		rumboot_printf("src_res11_tmp =%x\n",tmp);	
		}

	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y))
		{src_res12 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
		rumboot_printf("src_res12_tmp =%x\n",tmp);	  
		rumboot_printf("src_res12 error\n");}
		else 
		{src_res12=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
		rumboot_printf("src_res12_tmp =%x\n",tmp);	
		}
	
	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z))
		{src_res13 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
		rumboot_printf("src_res13_tmp =%x\n",tmp);	
		rumboot_printf("src_res13 error\n");}
		else 
		{src_res13=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
		rumboot_printf("src_res13_tmp =%x\n",tmp);
		}
	
	if ((0x00000000) !=ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_X))	
		{src_res14 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_X);
		rumboot_printf("src_res14_tmp =%x\n",tmp);		
		rumboot_printf("src_res14 error\n");}
		else 
		{src_res14=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Y))
		{src_res15 =1;
	  	tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
		rumboot_printf("src_res15_tmp =%x\n",tmp);		
		rumboot_printf("src_res15 error\n");}
		else 
		{src_res15=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Z))
		{src_res16 =1;
	  	tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);
		rumboot_printf("src_res16_tmp =%x\n",tmp);		
		rumboot_printf("src_res16 error\n");}
		else 
		src_res16=0;
	if ((0x00000000) !=ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X))	
		{src_res17 =1;
	  	tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("src_res17_tmp =%x\n",tmp);		
		rumboot_printf("src_res17 error\n");}
		else 
		{src_res17=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y))
		{src_res18 =1;
	  	tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("src_res18_tmp =%x\n",tmp);		  
		rumboot_printf("src_res18 error\n");}
		else 
		{src_res18=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z))
		{src_res19 =1;
	  	tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("src_res19_tmp =%x\n",tmp);		
		rumboot_printf("src_res19 error\n");}
		else 
		src_res19=0;

	if ((0x00000000) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_SIZE) & 0x0000001F))
		{src_res21 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("src_res21_tmp =%d\n",tmp);	
		rumboot_printf("src_res21 error\n");}
		else 
		{src_res21=0;
	    tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("src_res21_tmp =%d\n",tmp);}
		
  if ((0x00000000) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE ))
		{op2_res22 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("src_res22_tmp =%d\n",tmp);	
		rumboot_printf("src_res22 error\n");}
		else 
		{src_res22=0;
	    tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("src_res22_tmp =%d\n",tmp);}
			
//---------------------------Configuration OP0_RDMA-----------------------------------
	if ((0x00000000) !=(ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_STATUS)& 0x00030003)) 	
		{op0_res0 =1;
		rumboot_printf("op0_res0 error\n");}
		else 
		{op0_res0=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_POINTER)& 0x00010001))
		{op0_res1 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_POINTER);
		rumboot_printf("op0_res1_tmp =%d\n",tmp);	
		rumboot_printf("op0_res1 error\n");}	
		else 
		{op0_res1=0;}	
	if ((0x00000000) !=(ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_OP_ENABLE)& 0x00000001)) 	
		{op0_res2 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA +  NU_VPE_RDMA_OP_ENABLE);
		rumboot_printf("op0_res2_tmp =%d\n",tmp);
		rumboot_printf("op0_res2 error\n");}
		else 
		{op0_res2=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_W_IN)& 0x00003fff)) 
		{op0_res3 =1;
		rumboot_printf("op0_res3 error\n");}	
		else 
		{op0_res3=0;}	
	if ((0x00000000) !=(ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_H_IN)& 0x00001fff))	
		{op0_res4 =1;
		rumboot_printf("op0_res4 error\n");}	
		else 
		{op0_res4=0;}
			
	if ((0x00000000) != (ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_C_IN)& 0x00001fff))
		{op0_res5 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_C_IN);
		rumboot_printf("op0_res5_tmp =%d\n",tmp);		
		rumboot_printf("op0_res5 error\n");
	
		rumboot_printf("op0_res5 error\n");}	
		else 
		{op0_res5=0;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_C_IN);
		rumboot_printf("op0_res5_tmp =%d\n",tmp);			
		}
	
	if ((0x00000001) != (ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_CFG)& 0x0000003F))
		{op0_res6 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_CFG);
		rumboot_printf("op0_res6_tmp =%d\n",tmp);	
		rumboot_printf("op0_res6 error\n");}
		else 
		{op0_res6=0;}
	if ((0x00000000) != ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BASE_L))
		{op0_res7 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BASE_L);
		rumboot_printf("op0_res7_tmp =%d\n",tmp);	
		rumboot_printf("op0_res7 error\n");}
		else 
		{op0_res7=0;}

	if ((0x00000000) !=ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_LINE_STRIDE))	
		{op0_res8 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_LINE_STRIDE);
		rumboot_printf("op0_res8_tmp =%d\n",tmp);	
		rumboot_printf("op0_res8 error\n");}
		else 
		{op0_res8=0;}

	if ((0x00000000) != ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_VECTOR_STRIDE))
		{op0_res9 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);
		rumboot_printf("op0_res9_tmp =%d\n",tmp);
		rumboot_printf("op0_res9 error\n");}
		else 
		{op0_res9=0;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);
		rumboot_printf("op0_res9_tmp =%d\n",tmp);		
		}
	
	if ((0x00000000) != ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_ELEM_STRIDE)) //???
		{op0_res10 =1; 
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_ELEM_STRIDE);
		rumboot_printf("op0_res10_tmp =%d\n",tmp);		
		rumboot_printf("op0_res10 error\n");}
		else 
		op0_res10=0;

	if ((0x00000000) !=ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X))	
		{op0_res11 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);
		rumboot_printf("op0_res11_tmp =%d\n",tmp);		
		rumboot_printf("op0_res11 error\n");}
		else 
		{op0_res11=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y))
		{op0_res12 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
		rumboot_printf("op0_res12_tmp =%d\n",tmp);	  
		rumboot_printf("op0_res12 error\n");}
		else 
		{op0_res12=0;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
		rumboot_printf("op0_res12_tmp =%d\n",tmp);	  		
		}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z))
		{op0_res13 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);
		rumboot_printf("op0_res13_tmp =%d\n",tmp);		
		rumboot_printf("op0_res13 error\n");}
		else 
		op0_res13=0;
	
	if ((0x00000000) !=ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_X))		
		{op0_res14 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_X);
		rumboot_printf("op0_res14_tmp =%d\n",tmp);		
		rumboot_printf("op0_res14 error\n");}
		else 
		{op0_res14=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Y))
		{op0_res15 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
		rumboot_printf("op0_res15_tmp =%d\n",tmp);	  
		rumboot_printf("op0_res15 error\n");}
		else 
		{op0_res15=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Z))
		{op0_res16 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);
		rumboot_printf("op0_res16_tmp =%d\n",tmp);	
		rumboot_printf("op0_res16 error\n");}
		else 
		op0_res16=0;
	
	if ((0x00000000) !=ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X))	
		{op0_res17 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("op0_res17_tmp =%d\n",tmp);	
		rumboot_printf("op0_res17 error\n");}
		else 
		{op0_res17=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y))
		{op0_res18 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("op0_res18_tmp =%d\n",tmp);	  
		rumboot_printf("op0_res18 error\n");}
		else 
		{op0_res18=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z))
		{op0_res19 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("op0_res19_tmp =%d\n",tmp);	
		rumboot_printf("op0_res19 error\n");}
		else 
		{op0_res19=0;	
		}
		
	if ((0x00000000) != (ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_SIZE) & 0x0000001F))
		{op0_res21 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op0_res21_tmp =%d\n",tmp);	
		rumboot_printf("op0_res21 error\n");}
		else 
		{op0_res21=0;
	    tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op0_res21_tmp =%d\n",tmp);}
		
		
  if ((0x00000000) != ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_STRIDE ))
		{op0_res22 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op0_res22_tmp =%d\n",tmp);	
		rumboot_printf("op0_res22 error\n");}
		else 
		{op0_res22=0;
	    tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op0_res22_tmp =%d\n",tmp);}
						
//---------------------------Configuration OP1_RDMA-----------------------------------
	if ((0x00000000) !=(ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_STATUS)& 0x00030003)) 	
		{op1_res0 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_STATUS);
		rumboot_printf("op1_res1_tmp =%d\n",tmp);		
		rumboot_printf("op1_res0 error\n");}
		else 
		{op1_res0=0;}
	
	if ((0x00000000) != (ioread32(base +  NU_VPE_OP1_RDMA + NU_VPE_RDMA_POINTER)& 0x00010001))
		{op1_res1 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_POINTER);
		rumboot_printf("op1_res1_tmp =%d\n",tmp);
		rumboot_printf("op1_res1 error\n");}	
		else 
		{op1_res1=0;}
	
	if ((0x00000000) !=(ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_OP_ENABLE)& 0x00000001)) 	
		{op1_res2 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_OP_ENABLE);
		rumboot_printf("op1_res2_tmp =%d\n",tmp);
		rumboot_printf("op1_res2 error\n");}
		else 
		{op1_res2=0;}	
	if ((0x00000000) != (ioread32(base +  NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_W_IN)& 0x00001fff)) 
		{op1_res3 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_W_IN);
		rumboot_printf("op1_res3_tmp =%d\n",tmp);	
		rumboot_printf("op1_res3 error\n");}	
		else 
		{op1_res3=0;}	
	if ((0x00000000) !=(ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_H_IN)& 0x00001fff))	
		{op1_res4 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_H_IN);
		rumboot_printf("op1_res4_tmp =%d\n",tmp);	
		rumboot_printf("op1_res4 error\n");}	
		else 
		{op1_res4=0;}
		
	if ((0x00000000) != (ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_C_IN)& 0x00003fff))
		{op1_res5 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_C_IN);
		rumboot_printf("op1_res5_tmp =%d\n",tmp);	
		rumboot_printf("op1_res5 error\n");}	
		else 
		{op1_res5=0;}
	
	if ((0x00000001) != (ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_CFG)& 0x0000003F)) //not corresp. doc
		{op1_res6 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_CFG);
		rumboot_printf("op1_res6_tmp =%d\n",tmp);
		rumboot_printf("op1_res6 error\n");}
		else 
		{op1_res6=0;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_CFG);
		rumboot_printf("op1_res6_tmp =%d\n",tmp);	
		}
	if ((0x00000000) != ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BASE_L))
		{op1_res7 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BASE_L);
		rumboot_printf("op1_res7_tmp =%d\n",tmp);	
		rumboot_printf("op1_res7 error\n");}
		else 
		{op1_res7=0;}

	if ((0x00000000) !=ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_LINE_STRIDE))	
		{op1_res8 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_LINE_STRIDE);
		rumboot_printf("op1_res8_tmp =%d\n",tmp);	
		rumboot_printf("op1_res8 error\n");}
		else 
		{op1_res8=0;}

	if ((0x00000000) != ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_VECTOR_STRIDE))
		{op1_res9 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);
		rumboot_printf("op1_res9_tmp =%x\n",tmp);	  
		rumboot_printf("op1_res9 error\n");}
		else 
		{op1_res9=0;}
	
	if ((0x00000000) != ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_ELEM_STRIDE)) //???
		{op1_res10 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_ELEM_STRIDE);
		rumboot_printf("op1_res10_tmp =%d\n",tmp);		
		rumboot_printf("op1_res10 error\n");}
		else 
		op1_res10=0;

	if ((0x00000000) !=ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X))	
		{op1_res11 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);
		rumboot_printf("op1_res11_tmp =%d\n",tmp);	
		rumboot_printf("op1_res11 error\n");}
		else 
		{op1_res11=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y))
		{op1_res12 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
		rumboot_printf("op1_res12_tmp =%d\n",tmp);	  
		rumboot_printf("op1_res12 error\n");}
		else 
		{op1_res12=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z))
		{op1_res13 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);
		rumboot_printf("op1_res10_tmp =%d\n",tmp);	
		rumboot_printf("op1_res13 error\n");}
		else 
		op1_res13=0;
	
	if ((0x00000000) !=ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_X))	
		{op1_res14 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_X);
		rumboot_printf("op1_res14_tmp =%d\n",tmp);	
		rumboot_printf("op1_res14 error\n");}
		else 
		{op1_res14=0;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_X);
		rumboot_printf("op1_res14_tmp =%d\n",tmp);	
		}

	if ((0x00000000) != ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Y))
		{op1_res15 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
		rumboot_printf("op1_res15_tmp =%d\n",tmp);	  
		rumboot_printf("op1_res15 error\n");}
		else 
		{op1_res15=0;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
		rumboot_printf("op1_res15_tmp =%d\n",tmp);	
		}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Z))
		{op1_res16 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);
		rumboot_printf("op1_res16_tmp =%d\n",tmp);	
		rumboot_printf("op1_res16 error\n");}
		else 
		op1_res16=0;
	if ((0x00000000) !=ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X))	
		{op1_res17 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("op1_res17_tmp =%d\n",tmp);
		rumboot_printf("op1_res17 error\n");}
		else 
		{op1_res17=0;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("op1_res17_tmp =%d\n",tmp);	
	    }

	if ((0x00000000) != ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y))
	{op1_res18 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("op1_res18_tmp =%d\n",tmp);	  
		rumboot_printf("op1_res18 error\n");}
		else 
		{op1_res18=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z))
		{op1_res19 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("op1_res19_tmp =%d\n",tmp);	
		rumboot_printf("op1_res19 error\n");}
		else 
		{op1_res19=0;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("op1_res19_tmp =%d\n",tmp);}	

	if ((0x00000000) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_SIZE) & 0x0000001F))
		{op1_res21 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op1_res21_tmp =%d\n",tmp);	
		rumboot_printf("op1_res21 error\n");}
		else 
		{op2_res21=0;
	    tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op2_res21_tmp =%d\n",tmp);}
		
  if ((0x00000000) != ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_STRIDE ))
		{op1_res22 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op1_res22_tmp =%d\n",tmp);	
		rumboot_printf("op1_res22 error\n");}
		else 
		{op1_res22=0;
	    tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op2_res22_tmp =%d\n",tmp);}
		
//---------------------------Configuration OP2_RDMA-----------------------------------	
 if ((0x00000000) !=(ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_STATUS)& 0x00030003)) 	
		{op2_res0 =1;
		rumboot_printf("op2_res0 error\n");}
		else 
		{op2_res0=0;}
	
	if ((0x00000000) != (ioread32(base +  NU_VPE_OP2_RDMA + NU_VPE_RDMA_POINTER)& 0x00010001))
		{op2_res1 =1;
		rumboot_printf("op2_res1 error\n");}	
		else 
		{op2_res1=0;}
 
	if ((0x00000000) !=(ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_OP_ENABLE)& 0x00000001)) 	
		{op2_res2 =1;
		rumboot_printf("op2_res2 error\n");}
		else 
		{op2_res2=0;}	
	if ((0x00000000) != (ioread32(base +  NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_W_IN)& 0x00001fff)) 
		{op2_res3 =1;
		rumboot_printf("op2_res3 error\n");}	
		else 
		{op2_res3=0;}	
	if ((0x00000000) !=(ioread32(base +NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_H_IN)& 0x00001fff))	
		{op2_res4 =1;
		rumboot_printf("op2_res4 error\n");}	
		else 
		{op2_res4=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_C_IN)& 0x00001fff))
		{op2_res5 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_C_IN);
		rumboot_printf("op2_res5_tmp =%d\n",tmp);			
		rumboot_printf("op2_res5 error\n");}	
		else 
		{op2_res5=0;}
	
	if ((0x00000001) !=
		(ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_CFG)& 0x0000003F)) //not corresp. doc
		{op2_res6 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_CFG);
		rumboot_printf("op2_res6_tmp =%d\n",tmp);	
		rumboot_printf("op2_res6 error\n");}
		else 
		{op2_res6=0;}
	if ((0x00000000) != ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BASE_L))
		{op2_res7 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BASE_L);
		rumboot_printf("op2_res7_tmp =%d\n",tmp);
		rumboot_printf("op2_res7 error\n");}
		else 
		{op2_res7=0;}

	if ((0x00000000) !=ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_LINE_STRIDE))	
		{op2_res8 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_LINE_STRIDE);
		rumboot_printf("op2_res8_tmp =%d\n",tmp);	
		rumboot_printf("op2_res8 error\n");}
		else 
		{op2_res8=0;}

	if ((0x00000000) != ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_VECTOR_STRIDE))
		{op2_res9 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);
		rumboot_printf("op2_res9_tmp =%d\n",tmp);		  
		rumboot_printf("op2_res9 error\n");}
		else 
		{op2_res9=0;}
	
	if ((0x00000000) != ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_ELEM_STRIDE)) //???
		{op2_res10 =1;  
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_ELEM_STRIDE);
		rumboot_printf("op2_res10_tmp =%d\n",tmp);		
		rumboot_printf("op2_res10 error\n");}
		else 
		op2_res10=0;

	if ((0x00000000) !=ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X))	
		{op2_res11 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);
		rumboot_printf("op2_res11_tmp =%d\n",tmp);	
		rumboot_printf("op2_res11 error\n");}
		else 
		{op2_res11=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y))
		{op2_res12 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
		rumboot_printf("op2_res12_tmp =%d\n",tmp);	  
		rumboot_printf("op2_res12 error\n");}
		else 
		{op2_res12=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z))
		{op2_res13 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);
		rumboot_printf("op2_res13_tmp =%d\n",tmp);		
		rumboot_printf("op2_res13 error\n");}
		else 
		op2_res13=0;
	
	if ((0x00000000) !=ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_X))	
		{op2_res14 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_X);
		rumboot_printf("op2_res14_tmp =%d\n",tmp);	
		rumboot_printf("op2_res14 error\n");}
		else 
		{op2_res14=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Y))
		{op2_res15 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
		rumboot_printf("op2_res15_tmp =%d\n",tmp);	  
		rumboot_printf("op2_res15 error\n");}
		else 
		{op2_res15=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Z))
		{op2_res16 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);
		rumboot_printf("op2_res16_tmp =%d\n",tmp);	
		rumboot_printf("op2_res16 error\n");}
		else 
		op2_res16=0;
	if ((0x00000000) !=ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X))	
		{op2_res17 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("op2_res16_tmp =%d\n",tmp);	
		rumboot_printf("op2_res17 error\n");}
		else 
		{op2_res17=0;}

	if ((0x00000000) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y))
		{op2_res18 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("op2_res18_tmp =%d\n",tmp);	  
		rumboot_printf("op2_res18 error\n");}
		else 
		{op2_res18=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z))
		{op2_res19 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("op2_res19_tmp =%d\n",tmp);	
		rumboot_printf("op2_res19 error\n");}
		else 
		{op2_res19=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_OP2_ALU_CMP_RESULT))
		{op2_res20 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_OP2_ALU_CMP_RESULT);
		rumboot_printf("op2_res20_tmp =%d\n",tmp);	
		rumboot_printf("op2_res20 error\n");}
		else 
		{op2_res20=0;
	    tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_OP2_ALU_CMP_RESULT);
		rumboot_printf("op2_res20_tmp =%d\n",tmp);}
		
	if ((0x00000000) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_SIZE) & 0x0000001F))
		{op2_res21 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op2_res21_tmp =%d\n",tmp);	
		rumboot_printf("op2_res21 error\n");}
		else 
		{op2_res21=0;
	    tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op2_res21_tmp =%d\n",tmp);}
		
  if ((0x00000000) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE ))
		{op2_res22 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op2_res22_tmp =%d\n",tmp);	
		rumboot_printf("op2_res22 error\n");}
		else 
		{op2_res22=0;
	    tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op2_res22_tmp =%d\n",tmp);}
		
//-----------------------LUT-------------------------------------
if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_CFG)& 0x000301FF)) 	
		{lut_res0 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_CFG);
		rumboot_printf("lut_res0_tmp =%d\n",tmp);
		rumboot_printf("lut_res0 error\n");}
		else 
		{lut_res0=0;
	    tmp= ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_CFG);
		rumboot_printf("lut_res0_tmp =%d\n",tmp);}
/*	
	if ((0x00000000) != (ioread32(base +  NU_VPE + NU_VPE_LUT_ACCESS_DATA)& 0x0000FFFF))
		{lut_res1 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_DATA);
		rumboot_printf("lut_res1_tmp =%x\n",tmp);	
		rumboot_printf("lut_res1 error\n");}	
		else 
		{lut_res1=0;
	    tmp= ioread32(base + NU_VPE + NU_VPE_LUT_ACCESS_DATA);
		rumboot_printf("lut_res1_tmp =%x\n",tmp);}
*/
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_LUT_CFG)& 0x0000000F)) 	
		{lut_res2 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_CFG);
		rumboot_printf("lut_res2_tmp =%x\n",tmp);	
		rumboot_printf("lut_res2 error\n");}
		else 
		{lut_res2=0;
	    tmp= ioread32(base + NU_VPE + NU_VPE_LUT_CFG);
		rumboot_printf("lut_res2_tmp =%x\n",tmp);}	
		
	if ((0x00000000) != (ioread32(base +  NU_VPE + NU_VPE_LUT_SYM_POINT)& 0xFFFFFFFF)) 
		{lut_res3 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_SYM_POINT);
		rumboot_printf("lut_res3_tmp =%x\n",tmp);	
		rumboot_printf("lut_res3 error\n");}	
		else 
		{lut_res3=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_SYM_POINT);
		rumboot_printf("lut_res3_tmp =%x\n",tmp);}
	
	if ((0x00000000) !=(ioread32(base +NU_VPE + NU_VPE_LUT_TAB1_X_SHIFT)& 0x000000FF))	
		{lut_res4 =1;
		tmp= ioread32(base + NU_VPE +NU_VPE_LUT_TAB1_X_SHIFT);
		rumboot_printf("lut_res4_tmp =%x\n",tmp);	
		rumboot_printf("lut_res4 error\n");}	
		else 
		{lut_res4=0;
	    tmp= ioread32(base + NU_VPE +NU_VPE_LUT_TAB1_X_SHIFT);
		rumboot_printf("lut_res4_tmp =%x\n",tmp);}
	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT)& 0x000000FF))
		{lut_res5 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT);
		rumboot_printf("lut_res5_tmp =%x\n",tmp);			
		rumboot_printf("lut_res5 error\n");}	
		else 
		{lut_res5=0;
	    tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT);
		rumboot_printf("lut_res5_tmp =%x\n",tmp);}
	
	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_START)& 0xFFFFFFFF)) //not corresp. doc
		{lut_res6 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_LUT_TAB1_X_START);
		rumboot_printf("lut_res6_tmp =%x\n",tmp);	
		rumboot_printf("lut_res6 error\n");}
		else 
		{lut_res6=0;
	    tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_LUT_TAB1_X_START);
		rumboot_printf("lut_res6_tmp =%x\n",tmp);}
		
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_END)& 0xFFFFFFFF))
		{lut_res7 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_END);
		rumboot_printf("lut_res7_tmp =%d\n",tmp);
		rumboot_printf("lut_res7 error\n");}
		else 
		{lut_res7=0;}

	if ((0x00000000) !=(ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_START)& 0xFFFFFFFF))	
		{lut_res8 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_START);
		rumboot_printf("lut_res8_tmp =%d\n",tmp);	
		rumboot_printf("lut_res8 error\n");}
		else 
		{lut_res8=0;}

	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_END)& 0xFFFFFFFF))
		{lut_res9 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_END);
		rumboot_printf("lut_res9_tmp =%d\n",tmp);		  
		rumboot_printf("lut_res9 error\n");}
		else 
		{lut_res9=0;}
	
	if ((0x00000000) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE)& 0xFFFFFFFF)) //???
		{lut_res10 =1;  
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE);
		rumboot_printf("lut_res10_tmp =%d\n",tmp);		
		rumboot_printf("lut_res10 error\n");}
		else 
		lut_res10=0;

	if ((0x00000000) !=(ioread32( base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT)& 0x000003FF))	
		{lut_res11 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT);
		rumboot_printf("lut_res11_tmp =%d\n",tmp);	
		rumboot_printf("lut_res11 error\n");}
		else 
		{lut_res11=0;}

	if ((0x00000000) != (ioread32( base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE)& 0xFFFFFFFFF))
		{lut_res12 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE);
		rumboot_printf("lut_res12_tmp =%x\n",tmp);	  
		rumboot_printf("lut_res12 error\n");}
		else 
		{lut_res12=0;}
	
	if ((0x00000000) != (ioread32( base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT)& 0x0000003F))
		{lut_res13 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT);
		rumboot_printf("lut_res13_tmp =%d\n",tmp);		
		rumboot_printf("lut_res13 error\n");}
		else 
		lut_res13=0;
	

	if ((0x00000000) != (ioread32( base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL)& 0xFFFFFFFF))
		{vpe_res0 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL);
		rumboot_printf("vpe_res0_tmp =%d\n",tmp);	  
		rumboot_printf("vpe_res0 error\n");}
		else 
		{vpe_res0=0;}
	
	if ((0x00000000) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_OUT_CVT_SCALE_VAL)& 0x0000FFFF))
		{vpe_res1 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OUT_CVT_SCALE_VAL);
		rumboot_printf("vpe_res1_tmp =%d\n",tmp);	
		rumboot_printf("vpe_res1 error\n");}
		else 
		{vpe_res1=0;}
	if ((0x00000000) !=(ioread32( base + NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL)& 0x0000313F))	
		{vpe_res2 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL);
		rumboot_printf("vpe_res2_tmp =%d\n",tmp);	
		rumboot_printf("vpe_res2 error\n");}
		else 
		{vpe_res2=0;}

	if ((0x00000000) != ioread32( base + NU_VPE + NU_VPE_INF_NUM_IN))
		{vpe_res3 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_INF_NUM_IN);
		rumboot_printf("{vpe_res3_tmp =%d\n",tmp);	  
		rumboot_printf("{vpe_res3 error\n");}
		else 
		{vpe_res3=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE + NU_VPE_NAN_NUM_OUT))
		{vpe_res4 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_NAN_NUM_OUT );
		rumboot_printf("vpe_res4_tmp =%d\n",tmp);	
		rumboot_printf("vpe_res4 error\n");}
		else 
		{vpe_res4=0;}
	if ((0x00000000) != ioread32( base + NU_VPE + NU_VPE_SATUR_NUM_OUT ))
		{vpe_res5 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_SATUR_NUM_OUT );
		rumboot_printf("vpe_res5_tmp =%d\n",tmp);	
		rumboot_printf("vpe_res5 error\n");}
		else 
		{vpe_res5=0;}
	
	if ((0x00000000) != (ioread32( base + NU_VPE + NU_VPE_CTRL_NC ) & 0x00010001)) //DOC???
		{vpe_res6 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_CTRL_NC );
		rumboot_printf("vpe_res6_tmp =%x\n",vpe_res6);
		rumboot_printf("vpe_res6 =%d\n",tmp);		
		rumboot_printf("vpe_res6 error\n");}
		else 
		{vpe_res6=0;}
	
	if ((0x00020000) != (ioread32( base + NU_VPE + NU_VPE_AXI_PARAM ) & 0x0F07030F))
		{vpe_res7 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_AXI_PARAM );
		rumboot_printf("vpe_res7_tmp =%d\n",tmp);	
		rumboot_printf("vpe_res7 error\n");}
		else 
		{vpe_res7=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE + NU_VPE_CUBE_SIZE ))
		{vpe_res8 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_AXI_PARAM );
		rumboot_printf("vpe_res8_tmp =%d\n",tmp);	
		rumboot_printf("vpe_res8 error\n");}
		else 
		{vpe_res8=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE + REG_VPE_IRQ_RD_ADDR ))
		{vpe_res9 =1;
		tmp= ioread32(base + NU_VPE + REG_VPE_IRQ_RD_ADDR );
		rumboot_printf("vpe_res9_tmp =%x\n",tmp);	
		rumboot_printf("vpe_res9 error\n");}
		else 
		{vpe_res9=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE + REG_VPE_IRQ_CLR_ADDR ))
		{vpe_res10 =1;
		tmp= ioread32(base + NU_VPE + REG_VPE_IRQ_CLR_ADDR );
		rumboot_printf("vpe_res10_tmp =%x\n",tmp);	
		rumboot_printf("vpe_res10 error\n");}
		else 
		{vpe_res10=0;}
	
	if ((0x00000000) != ioread32( base + NU_VPE + REG_VPE_IRQ_SET_ADDR ))
		{vpe_res11 =1;
		tmp= ioread32(base + NU_VPE + REG_VPE_IRQ_SET_ADDR );
		rumboot_printf("vpe_res11_tmp =%x\n",tmp);	
		rumboot_printf("vpe_res11 error\n");}
		else 
		{vpe_res11=0;}

	if ((0x00000000) != (ioread32( base + NU_VPE + REG_VPE_IRQ_MASK_ADDR )& 0x00000003))
		{vpe_res12 =1;
		tmp= ioread32(base + NU_VPE + REG_VPE_IRQ_MASK_ADDR );
		rumboot_printf("vpe_res12_tmp =%x\n",tmp);	
		rumboot_printf("vpe_res12 error\n");}
		else 
		{vpe_res12=0;}
			
//----------------------------------------------------------------
	  		
	  resr = 	cp0_res0 || cp0_res1|| cp0_res2 || cp0_res3 || cp0_res4 || cp0_res5 ||
				cp1_res0 || cp1_res1|| cp1_res2 || cp1_res3 || cp1_res4 || cp1_res5 ||
				cp2_res2 || cp2_res1|| cp2_res2 || cp2_res3 || cp2_res4 || cp2_res5 || cp2_res6 || cp2_res7 || cp2_res8 || cp2_res9 || cp2_res10 || cp2_res11 ||
				src_res0 || src_res1|| src_res2 || src_res3 || src_res4 || src_res5 || src_res7 || src_res8 ||
				src_res9 || src_res10 || src_res11 || src_res12 || src_res13 || src_res14 || src_res15 || 
				src_res16 || src_res17 || src_res18 || src_res19 || src_res21 || src_res22 ||
				op0_res0 || op0_res1|| op0_res2 || op0_res3 || op0_res4 || op0_res5 || op0_res7 || op0_res8 ||
				op0_res9 || op0_res10 || op0_res11 || op0_res12 || op0_res13 || op0_res14 || op0_res15 || 
				op0_res16 || op0_res17 || op0_res18 || op0_res19 || op0_res21,op0_res22 ||
				op1_res0 || op1_res1|| op1_res2 || op1_res3 || op1_res4 ||  op1_res5 || op1_res7 || op1_res8 ||
				op1_res9 || op1_res10 || op1_res11 || op1_res12 || op1_res13 || op1_res14 || op1_res15 || 
				op1_res16 || op1_res17 || op1_res18 || op1_res19 || op1_res21 || op1_res22 ||
				op2_res0 || op2_res1|| op2_res2 || op2_res3 || op2_res4 || op2_res5 || op2_res7 || op2_res8 ||
				op2_res9 || op2_res10 || op2_res11 || op2_res12 || op2_res13 || op2_res14 || op2_res15 || 
				op2_res16 || op2_res17 || op2_res18 || op2_res19 || op2_res20 || op2_res21 || op2_res22 || op_res || 				
				lut_res0 || lut_res2 || lut_res3 || lut_res4 || lut_res5 || lut_res6 || lut_res7 || lut_res8 ||
				lut_res9 || lut_res10 || lut_res11 || lut_res12 || lut_res13 || 
				vpe_res0 || vpe_res1 || vpe_res2 || vpe_res3 || vpe_res4 || vpe_res5 ||
				vpe_res6 || vpe_res7 || vpe_res8 || vpe_res9 || vpe_res10 || vpe_res11  || vpe_res12; 
				
	  resw = 	resw0 || resw1||  resw2 || resw3 || resw4 || resw5 || resw6 || resw7 || resw8 || resw9 ||
				resw10 || resw11 || resw12 || resw13 || resw14 || resw15 || resw16 || resw17 ||
				resw18 || resw19 || resw20 ||resw21 || resw22; 
	  			
	
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
		  resw13,resw14,resw15,resw16,resw17,resw18,resw19,resw20,resw21,resw22;
	  int cp0_res0,cp0_res1,cp0_res2,cp0_res3,cp0_res4,cp0_res5;
	  int cp1_res0,cp1_res1,cp1_res2,cp1_res3,cp1_res4,cp1_res5;
	  int cp2_res0,cp2_res1,cp2_res2,cp2_res3,cp2_res4,cp2_res5,cp2_res6,cp2_res7,
		  cp2_res8,cp2_res9,cp2_res10,cp2_res11;
	  	  
	  int /*src_res0,src_res1,src_res2,*/src_res3,src_res4,src_res5,/*src_res6,*/src_res7,src_res8,src_res9,src_res10,src_res11,src_res12,
		  src_res13,src_res14,src_res15,src_res16,src_res17,src_res18,src_res19,src_res21,src_res22;
	  int/* op0_res0,op0_res1,op0_res2,*/op0_res3,op0_res4,op0_res5,op0_res6,op0_res7,op0_res8,op0_res9,op0_res10,op0_res11,op0_res12,
		  op0_res13,op0_res14,op0_res15,op0_res16,op0_res17,op0_res18,op0_res19,op0_res21,op0_res22;
	  int /*op1_res0,op1_res1,op1_res2,*/op1_res3,op1_res4,op1_res5,op1_res6,op1_res7,op1_res8,op1_res9,op1_res10,op1_res11,op1_res12,
		  op1_res13,op1_res14,op1_res15,op1_res16,op1_res17,op1_res18,op1_res19,op1_res21,op1_res22;	  
	  int /*op2_res0,op2_res1,op2_res2,*/op2_res3,op2_res4,op2_res5,op2_res6,op2_res7,op2_res8,op2_res9,op2_res10,op2_res11,op2_res12,
		  op2_res13,op2_res14,op2_res15,op2_res16,op2_res17,op2_res18,op2_res19,op2_res21,op2_res22;	
	  int lut_res0,lut_res1,lut_res2,lut_res3,lut_res4,lut_res5,lut_res6,lut_res7,lut_res8,lut_res9,lut_res10,lut_res11,lut_res12, 
		  lut_res13;
	  int vpe_res0,vpe_res1,vpe_res2,vpe_res3,vpe_res4,vpe_res5, vpe_res7,vpe_res8,vpe_res12; 
	  int resr;
	  int resw;
	  int32_t tmp;
	  int32_t tmpr;
	  tmpr =data;
	//----------------------WPE & WDMA---------------------------------------	  
/*	if ((0x00000000) !=(ioread32(base + NU_VPE +NU_VPE_STATUS)& 0x00030003)) 	
		{resw0 =1;
	    tmp = ioread32(base + NU_VPE +NU_VPE_STATUS);
		rumboot_printf("resw0_tmp =%d\n",tmp);
		rumboot_printf("resw0 error\n");}
		else 
		{resw0=0;}
 
	if ((0x00000000) !=(ioread32(base + NU_VPE +NU_VPE_POINTER)& 0x00010001)) 	
		{resw1 =1;
	    tmp= ioread32(base + NU_VPE +NU_VPE_POINTER);
		rumboot_printf("resw1_tmp =%d\n",tmp);
		rumboot_printf("resw1 error\n");}
		else 
		{resw1=0;}
*/	
        iowrite32(tmpr, base + NU_VPE + NU_VPE_OP_ENABLE);
	if ((tmpr & 0x00000001 ) !=(ioread32(base + NU_VPE + NU_VPE_OP_ENABLE) & 0x00000001))	
		{resw2 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_OP_ENABLE);
		rumboot_printf("resw2_tmp =%d\n",tmp);
		rumboot_printf("resw2 error\n");}
		else 
		{resw2=0;}
	
        iowrite32(tmpr, base + NU_VPE + NU_VPE_DATA_W_OUT);
	if ((tmpr & 0x00001FFF ) !=(ioread32(base + NU_VPE + NU_VPE_DATA_W_OUT) & 0x00001FFF))	
		{resw3 =1;
	    tmp = ioread32(base + NU_VPE + NU_VPE_DATA_W_OUT);
		rumboot_printf("resw3_tmp =%d\n",tmp);
		rumboot_printf("resw3 error\n");}
		else 
		{resw3=0;}
	
        iowrite32(tmpr, base + NU_VPE + NU_VPE_DATA_H_OUT);
	if ((tmpr & 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_DATA_H_OUT) & 0x00001FFF))	
		{resw4 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_DATA_H_OUT);
		rumboot_printf("resw4_tmp =%d\n",tmp);
		rumboot_printf("resw4 error\n");}
		else 
		{resw4=0;}
	
       iowrite32(tmpr, base + NU_VPE + NU_VPE_DATA_C_OUT);
	if ((tmpr & 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_DATA_C_OUT) & 0x00001FFF))	
		{resw5 =1;
		rumboot_printf("resw5 error\n");}
		else 
		{resw5=0;}
	
	 iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_DST_BASE_L);
	if ((tmpr) != ioread32(base + NU_VPE + NU_VPE_WDMA_DST_BASE_L))	
		{resw6 =1;
		rumboot_printf("resw6 error\n");}
		else 
		{resw6=0;}
	
	iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_DST_LINE_STRIDE);
	if ((tmpr & 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_WDMA_DST_LINE_STRIDE) & 0x00001FFF))	
		{resw7 =1;
		rumboot_printf("resw7 error\n");}
		else 
		{resw7=0;}
	
	iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_DST_VECTOR_STRIDE);
	if ((tmpr& 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_WDMA_DST_VECTOR_STRIDE) & 0x00001FFF))	
		{resw8 =1;
		rumboot_printf("resw8 error\n");}
		else 
		{resw8=0;}
	
	iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_ELEM_STRIDE);	
	if ((tmpr& 0x00001FFF) !=(ioread32(base + NU_VPE + NU_VPE_WDMA_ELEM_STRIDE) & 0x00001FFF))	
		{resw9 =1;
		rumboot_printf("resw9 error\n");}
		else			
	    {resw9=0;}
	
	iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_DST_CFG);	
	if ((tmpr & 0x00000100) !=(ioread32(base + NU_VPE + NU_VPE_WDMA_DST_CFG) & 0x00000100))	
		{resw10 =1;
		rumboot_printf("resw10 error\n");}
		else 
		{resw10=0;}	
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_X);
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_X) & 0x00003FFF))	//DOC ??	
		{resw11 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_X);
		rumboot_printf("resw11_tmp =%x\n",tmp);
		rumboot_printf("resw11 error\n");}
		else 
		{resw11=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Y);
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Y) & 0x00003FFF))	//DOC ??	
		{resw12 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Y);
		rumboot_printf("resw12_tmp =%x\n",tmp);
		rumboot_printf("resw12 error\n");}
		else 
		{resw12=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Z);	
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Z) & 0x00003FFF))	//DOC ??	
		{resw13 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_ST_SIZE_Z);
		rumboot_printf("resw13_tmp =%x\n",tmp);	
		rumboot_printf("resw13 error\n");}
		else 
		{resw13=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_X);		
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_X) & 0x00003FFF))	//DOC ??
		{resw14 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_X);
		rumboot_printf("resw14_tmp =%x\n",tmp);		
		rumboot_printf("resw14 error\n");}
		else 
		{resw14=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Y);	
	if ((tmpr  & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Y) & 0x00003FFF))	//DOC ??
		{resw15 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Y);
		rumboot_printf("resw15_tmp =%x\n",tmp);		
		rumboot_printf("resw15 error\n");}
		else 
		{resw15=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Z);	
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Z) & 0x00003FFF))	//DOC ??
		{resw16 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_SIZE_Z);
		rumboot_printf("resw16_tmp =%x\n",tmp);			
		rumboot_printf("resw16 error\n");}
		else 
		{resw16=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_X);	
	if ((tmpr& 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_X)& 0x00003FFF)) //DOC ?? bug fix!!!!	
		{resw17 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("resw17_tmp =%x\n",tmp);		
		rumboot_printf("resw17 error\n");}
		else 
		{resw17=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Y);		
	if ((tmpr& 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Y)& 0x00003FFF)) //DOC ??	
		{resw18 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("resw18_tmp =%x\n",tmp);	
		rumboot_printf("resw18 error\n");}
		else 
		{resw18=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Z);
	if ((tmpr& 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Z)& 0x00003FFF)) //DOC ??	
		{resw19 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("resw19_tmp =%x\n",tmp);
		rumboot_printf("resw19 error\n");}
		else 
		{resw19=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_ELMT_LAST_SIZE_ADDR);	
		if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_ELMT_LAST_SIZE_ADDR)& 0x00003FFF))	
		{resw20 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_ELMT_LAST_SIZE_ADDR);
		rumboot_printf("resw20_tmp =%d\n",tmp);	
		rumboot_printf("resw20 error\n");}
		else 
		{resw20=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_ELMT_SIZE_ADDR);
	if ((tmpr & 0x00003FFF) != (ioread32(base + NU_VPE + NU_VPE_WDMA_ELMT_SIZE_ADDR)& 0x00003FFF))	
		{resw21 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_ELMT_SIZE_ADDR);
		rumboot_printf("resw21_tmp =%d\n",tmp);	
		rumboot_printf("resw21 error\n");}
		else 
		{resw21=0;}	

		iowrite32(tmpr, base + NU_VPE + NU_VPE_WDMA_XYZ_DRCT_ADDR);	
	if ((tmpr & 0x00000003) != (ioread32(base + NU_VPE + NU_VPE_WDMA_XYZ_DRCT_ADDR)& 0x00000003))	
		{resw22 =1;
		tmp = ioread32(base + NU_VPE + NU_VPE_WDMA_XYZ_DRCT_ADDR);
		rumboot_printf("resw22_tmp =%d\n",tmp);	
		rumboot_printf("resw22 error\n");}
		else 
		{resw22=0;}
	//----------------------OP---------------------------------------
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP_MODE);	
	if ((tmpr  & 0x01F3FF33 ) !=(ioread32(base + NU_VPE + NU_VPE_OP_MODE) & 0x01F3FF33))
		{op_res =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP_MODE);
		 rumboot_printf("op_res_tmp =%d\n",tmp);
		rumboot_printf("op_res error\n");}
		else 
		{op_res=0;}
 
	//----------------------OP0--------------------------------------
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_CFG);		
	if ((tmpr  & 0x0000007F)!=(ioread32(base + NU_VPE + NU_VPE_OP0_CFG) & 0x0000007F)) 	
		{cp0_res0 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_CFG);
		rumboot_printf("cp0_res0_tmp =%x\n",tmp);
		rumboot_printf("cp0_res0 error\n");}
		
		else 
		{cp0_res0=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP0_CFG);
		rumboot_printf("cp0_res0_tmp =%x\n",tmp);}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_ALU_CFG);
	if ((tmpr  & 0x00003F01)!=(ioread32(base + NU_VPE + NU_VPE_OP0_ALU_CFG)& 0x00003F01)) 	
		{cp0_res1 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_ALU_CFG);
		rumboot_printf("cp0_res1_tmp =%d\n",tmp);	
		rumboot_printf("cp0_res1 error\n");}
		else 
		{cp0_res1=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL);
	if ((tmpr &  0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL)& 0x0000FFFF))
		{cp0_res2 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_ALU_SRC_VAL);
		rumboot_printf("cp0_res2_tmp =%d\n",tmp);		
		rumboot_printf("cp0_res2 error\n");}
		else 
		{cp0_res2=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_MUL_CFG);
	if ((tmpr & 0x00000001) !=  (ioread32(base + NU_VPE + NU_VPE_OP0_MUL_CFG)& 0x00000001))
		{cp0_res3 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_MUL_CFG);
		rumboot_printf("cp0_res3_tmp =%d\n",tmp);
		rumboot_printf("cp0_res3 error\n");}
		else 
		{cp0_res3=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL);
	if ((tmpr & 0x0000FFFF ) !=(ioread32(base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL)& 0x0000FFFF))	
		{cp0_res4 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_MUL_SRC_VAL);
		rumboot_printf("cp0_res4_tmp =%d\n",tmp);	
		rumboot_printf("cp0_res4 error\n");}
		else 
		{cp0_res4=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_NORM_PARAM);
	if ((tmpr & 0x000301ff) != (ioread32(base + NU_VPE + NU_VPE_OP0_NORM_PARAM)& 0x000301ff))
		{cp0_res5 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_NORM_PARAM);
		rumboot_printf("cp0_res5_tmp =%d\n",tmp);
		rumboot_printf("cp0_res5 error\n");}
		else 
		{cp0_res5=0;}
	
//----------------------OP1--------------------------------------	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP1_CFG);	
	if ((tmpr &  0x0000007F) !=(ioread32(base + NU_VPE + NU_VPE_OP1_CFG) & 0x0000007F)) 	
		{cp1_res0 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_CFG);
		rumboot_printf("cp1_res0_tmp =%x\n",tmp);	
		rumboot_printf("cp1_res0 error\n");}	
		else 
		{cp1_res0=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP1_ALU_CFG);	
	if ((tmpr &  0x00003F01 ) !=(ioread32(base + NU_VPE + NU_VPE_OP1_ALU_CFG)&  0x00003F01)) 	
		{cp1_res1 =1;
	    tmp= ioread32(base + NU_VPE +  NU_VPE_OP1_ALU_CFG);
		rumboot_printf("cp1_res1_tmp =%d\n",tmp);
		rumboot_printf("cp1_res1 error\n");}
		else 
		{cp1_res1=0;}
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL);		
	if ((tmpr &   0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_OP0_ALU_SRC_VAL)& 0x0000FFFF))
		{cp1_res2 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_ALU_SRC_VAL);
		rumboot_printf("cp1_res2_tmp =%d\n",tmp);	
		rumboot_printf("cp1_res2 error\n");}
		else 
		{cp1_res2=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_MUL_CFG);	
	if ((tmpr &  0x00000001) !=  (ioread32(base + NU_VPE + NU_VPE_OP0_MUL_CFG)& 0x00000001))
		{cp1_res3 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_MUL_CFG);
		rumboot_printf("cp1_res3_tmp =%d\n",tmp);	
		rumboot_printf("cp1_res3 error\n");}
		else 
		{cp1_res3=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL);	
	if ((tmpr &   0x0000FFFF) !=(ioread32(base + NU_VPE + NU_VPE_OP0_MUL_SRC_VAL)& 0x0000FFFF))	
		{cp1_res4 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_MUL_SRC_VAL);
		rumboot_printf("cp1_res2_tmp =%d\n",tmp);	
		rumboot_printf("cp0_res4 error\n");}
		else 
		{cp1_res4=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP0_NORM_PARAM);	
	if ((tmpr &   0x000301ff)!= (ioread32(base + NU_VPE + NU_VPE_OP0_NORM_PARAM)& 0x000301ff))
		{cp1_res5 =1;	
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP0_NORM_PARAM);
		rumboot_printf("cp1_res5_tmp =%d\n",tmp);	
		rumboot_printf("cp1_res5 error\n");}
		else 
		{cp1_res5=0;}	
         
		
//----------------------OP2--------------------------------------	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_CFG);	
	if ((tmpr &0x0000007F) !=(ioread32(base + NU_VPE + NU_VPE_OP2_CFG) & 0x0000007F)) 	
		{cp2_res0 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_CFG);
		rumboot_printf("cp2_res0_tmp =%d\n",tmp);
		rumboot_printf("cp2_res0 error\n");}	
		else 
		{cp2_res0=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_CFG);		
	if ((tmpr&  0x00000003) !=(ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CFG) &  0x00000003)) 		
		{cp2_res1 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CFG);
		rumboot_printf("cp2_res1_tmp =%d\n",tmp);
		rumboot_printf("cp2_res1 error\n");}	
		else 
		{cp2_res1=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL);	
	if ((tmpr) != ioread32(base + NU_VPE + NU_VPE_OP2_ALU_SRC_VAL))	
		{cp2_res2 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_SRC_VAL);
		rumboot_printf("cp2_res2_tmp =%d\n",tmp);	
		rumboot_printf("cp2_res2 error\n");}
		else 
		{cp2_res2=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL);	
	if ((tmpr & 0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_OFFSET_VAL) & 0x0000FFFF))	
		{cp2_res3 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CVT_OFFSET_VAL);
		rumboot_printf("cp2_res3_tmp =%x\n",tmp);	
		rumboot_printf("cp2_res3 error\n");}
		else 
		{cp2_res3=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL);		
	if ((tmpr & 0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_SCALE_VAL)& 0x0000FFFF))	
		{cp2_res4 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CVT_SCALE_VAL);
		rumboot_printf("cp2_res4_tmp =%x\n",tmp);
		rumboot_printf("cp2_res4 error\n");}
		else 
		{cp2_res4=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL);		
	if ((tmpr & 0x0000003F) != (ioread32(base + NU_VPE + NU_VPE_OP2_ALU_CVT_TRUNC_VAL)& 0x0000003F))		
		{cp2_res5 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_ALU_CVT_TRUNC_VAL)& 0x0000003F;
		rumboot_printf("cp2_res5_tmp =%x\n",tmp);
		rumboot_printf("cp2_res5 error\n");}
		else 
		{cp2_res5=0;}	

	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_CFG);	
	if ((tmpr & 0x00000003 ) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CFG)& 0x00000003))		
		{cp2_res6 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CFG);
		rumboot_printf("cp2_res6_tmp =%d\n",tmp);
		rumboot_printf("cp2_res6 error\n");}
		else 
		{cp2_res6=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL);
	if ((tmpr) != ioread32(base + NU_VPE + NU_VPE_OP2_MUL_SRC_VAL))		
		{cp2_res7 =1;
	    tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_SRC_VAL);
		rumboot_printf("cp2_res7_tmp =%d\n",tmp);	
		rumboot_printf("cp2_res7 error\n");}
		else 
		{cp2_res7=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
	if ((tmpr & 0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL) & 0x0000FFFF))	
		{cp2_res8 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_OFFSET_VAL);
		rumboot_printf("cp2_res8_tmp =%d\n",tmp);
		rumboot_printf("cp2_res8 error\n");}
		else 
        {cp2_res8=0;}
	
	    iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL);	
	if ((tmpr & 0x0000FFFF) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL)& 0x0000FFFF))	
		{cp2_res9 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_SCALE_VAL);
		rumboot_printf("cp2_res9_tmp =%d\n",tmp);	
		rumboot_printf("cp2_res9 error\n");}
		else 
		{cp2_res9=0;}
	

		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL);
	if ((tmpr & 0x0000003F) != (ioread32(base + NU_VPE + NU_VPE_OP2_MUL_CVT_TRUNC_VAL)& 0x0000003F))		
		{cp2_res10 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_MUL_CVT_TRUNC_VAL);
		rumboot_printf("cp2_res10_tmp =%x\n",tmp);	
		rumboot_printf("cp2_res10 error\n");}
		else 
		{cp2_res10=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OP2_NORM_PARAM);
   if ((tmpr & 0x000301ff ) !=
		(ioread32(base + NU_VPE + NU_VPE_OP2_NORM_PARAM)& 0x000301ff))		
		{cp2_res11 =1;
		tmp= ioread32(base + NU_VPE +  NU_VPE_OP2_NORM_PARAM);
		rumboot_printf("cp2_res11_tmp =%x\n",tmp);
		rumboot_printf("cp2_res11 error\n");}
		else 
		{cp2_res11=0;}			

	//----------------------Configuration SRC_RDMA ------------------------------------------------------		
/*	if ((0x00000000) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_STATUS)& 0x00030003)) 	
		{src_res0 =1;
		rumboot_printf("src_res0 error\n");}
		else 
		{src_res0=0;}
	
	if ((0x00000000) != (ioread32(base + base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_POINTER)& 0x00010001))
		{src_res1 =1;
		rumboot_printf("src_res1 error\n");}	
		else 
		{src_res1=0;} */
	
/*	
		iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_OP_ENABLE);	
	if ((tmpr & 0x00000001) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_OP_ENABLE)& 0x00000001)) 	
		{src_res2 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA +  NU_VPE_RDMA_OP_ENABLE);
		rumboot_printf("src_res2_tmp =%x\n",tmp);
		rumboot_printf("src_res2 error\n");}
		else 
		{src_res2=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA +  NU_VPE_RDMA_OP_ENABLE);
		rumboot_printf("src_res2_tmp =%x\n",tmp);	
	     }
	
*/		
		iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_W_IN);		
	if ((tmpr & 0x00003fff) != (ioread32(base +  NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_W_IN)& 0x00003fff)) 
		{src_res3 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_W_IN);
		rumboot_printf("src_res3_tmp =%x\n",tmp);	
		rumboot_printf("src_res3 error\n");}	
		else 
		{src_res3=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_W_IN);
		rumboot_printf("src_res3_tmp =%x\n",tmp); }	

		
	    iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_H_IN);		
	if ((tmpr & 0x00003fff) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_H_IN)& 0x00003fff))	
		{src_res4 =1;
		rumboot_printf("src_res4 error\n");}	
		else 
		{src_res4=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_H_IN);
		rumboot_printf("src_res4_tmp =%x\n",tmp);
		}
	
		
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_C_IN);

	if ((tmpr& 0x00003fff) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_C_IN)& 0x00003fff)) //DOC ???
		{src_res5 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_C_IN);
		rumboot_printf("src_res5_tmp =%x\n",tmp);	
		rumboot_printf("src_res5 error\n");}	
		else 
		{src_res5=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_DATA_C_IN);
		rumboot_printf("src_res5_tmp =%x\n",tmp);
		}
/*		
		iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_OP_ENABLE);	
	if ((tmpr& 0x0000003F) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_CFG)& 0x0000003F))
		{src_res6 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_OP_ENABLE);
		rumboot_printf("src_res6_tmp =%x\n",tmp);
		rumboot_printf("src_res6 error\n");}
		else 
		{src_res6=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_OP_ENABLE);
		rumboot_printf("src_res6_tmp =%x\n",tmp);	
		}
	*/	

	iowrite32(tmpr, base +  NU_VPE_SRC_RDMA + NU_VPE_RDMA_BASE_L);		
	if ((tmpr) != ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BASE_L))
		{src_res7 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BASE_L);
		rumboot_printf("src_res7_tmp =%x\n",tmp);	
		rumboot_printf("src_res7 error\n");}
		else 
		{src_res7=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BASE_L);
		rumboot_printf("src_res7_tmp =%x\n",tmp);		
	    }
		
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_LINE_STRIDE);	
	if ((tmpr & 0x0003FFFF) !=(ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_LINE_STRIDE) & 0x0003FFFF)) //DOC ???	
		{src_res8 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_LINE_STRIDE);
		rumboot_printf("src_res8_tmp =%x\n",tmp);
		rumboot_printf("src_res8 error\n");}
		else 
		{src_res8=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_LINE_STRIDE);
		rumboot_printf("src_res8_tmp =%x\n",tmp);		
		}

	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);	
	if ((tmpr& 0x0003FFFF) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_VECTOR_STRIDE) & 0x0003FFFF))  //DOC ???	
		{src_res9 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);	
		rumboot_printf("src_res9_tmp =%x\n",tmp);		  
		rumboot_printf("src_res9 error\n");}
		else 
		{src_res9=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);	
		rumboot_printf("src_res9_tmp =%x\n",tmp);		
	    }
	
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_ELEM_STRIDE);		
	if ((tmpr& 0x0003FFFF) != (ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_ELEM_STRIDE) & 0x0003FFFF))  //DOC ???
		{src_res10 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_ELEM_STRIDE);	
		rumboot_printf("src_res10_tmp =%x\n",tmp);	
		rumboot_printf("src_res10 error\n");}
		else 
		{src_res10=0;}
	
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X) & 0x0003FFF))  //DOC ???	
		{src_res11 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);	
		rumboot_printf("src_res11_tmp =%x\n",tmp);	
		rumboot_printf("src_res11 error\n");}
		else 
		{src_res11=0;}
	
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y)& 0x0003FFF))  //DOC ???
		{src_res12 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);	
		rumboot_printf("src_res12_tmp =%x\n",tmp);		  
		rumboot_printf("src_res12 error\n");}
		else 
		{src_res12=0;}
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z)& 0x0003FFF))  //DOC ???
		{src_res13 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);	
		rumboot_printf("src_res13_tmp =%x\n",tmp);		
		rumboot_printf("src_res13 error\n");}
		else 
		src_res13=0;
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_X);	
	if ((tmpr& 0x0003FFF) !=(ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_X)& 0x0003FFF))  //DOC ???		
		{src_res14 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA +NU_VPE_RDMA_BOX_SIZE_X);
		rumboot_printf("src_res14_tmp =%x\n",tmp);		
		rumboot_printf("src_res14 error\n");}
		else 
		{src_res14=0;}
	
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Y)& 0x0003FFF))  //DOC ???
		{src_res15 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA +NU_VPE_RDMA_BOX_SIZE_Y);
		rumboot_printf("src_res15_tmp =%x\n",tmp);	  
		rumboot_printf("src_res15 error\n");}
		else 
		{src_res15=0;}
	
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_SIZE_Z)& 0x0003FFF))  //DOC ???
		{src_res16 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA +NU_VPE_RDMA_BOX_SIZE_Z);
		rumboot_printf("src_res16_tmp =%x\n",tmp);	
		rumboot_printf("src_res16 error\n");}
		else 
		{src_res16=0;}
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X)& 0x0003FFF))  //DOC ??? bug fix
		{src_res17 =1;	
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
		rumboot_printf("src_res17_tmp =%x\n",tmp);	
		rumboot_printf("src_res17 error\n");}
		else 
		{src_res17=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
		rumboot_printf("src_res17_tmp =%x\n",tmp);}	

	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y)& 0x0003FFF))  //DOC ???
		{src_res18 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);	
		rumboot_printf("src_res18_tmp =%x\n",tmp);
		rumboot_printf("src_res18 error\n");}
		else 
		{src_res18=0;}
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z)& 0x0003FFF))  //DOC ???
		{src_res19 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);	
		rumboot_printf("src_res19_tmp =%x\n",tmp);	
		rumboot_printf("src_res19 error\n");}
		else 
		{src_res19=0;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);	
		rumboot_printf("src_res19_tmp =%x\n",tmp);}
		
	iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_SIZE);		
	if ((tmpr & 0x0000001F) != (ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_SIZE) & 0x0000001F))
		{src_res21 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("src_res21_tmp =%d\n",tmp);	
		rumboot_printf("src_res21 error\n");}
		else 
		{src_res21=0;
	    tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("src_res21_tmp =%d\n",tmp);}
		
		iowrite32(tmpr, base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_STRIDE);		
	if ((tmpr) != ioread32( base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_STRIDE ))
		{src_res22 =1;
		tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("src_res22_tmp =%x\n",tmp);	
		rumboot_printf("src_res22 error\n");}
		else 
		{src_res22=0;
	    tmp= ioread32(base + NU_VPE_SRC_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("src_res22_tmp =%x\n",tmp);}	
//---------------------------Configuration OP0_RDMA-----------------------------------
	/*if ((0x00000000) !=(ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_STATUS)& 0x00030003)) 	
		{op0_res0 =1;
		rumboot_printf("op0_res0 error\n");}
		else 
		{op0_res0=0;}
	
	if ((0x00000000) != (ioread32(base  + NU_VPE_OP0_RDMA + NU_VPE_RDMA_POINTER)& 0x00010001))
		{op0_res1 =1;
		rumboot_printf("op0_res1 error\n");}	
		else 
		{op0_res1=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_OP_ENABLE);	
	if ((tmpr &  0x00000001 ) !=(ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_OP_ENABLE)& 0x00000001)) 	
		{op0_res2 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA+  NU_VPE_RDMA_OP_ENABLE);
		rumboot_printf("op0_res2_tmp =%d\n",tmp);
		rumboot_printf("op0_res2 error\n");}
		else 
		{op0_res2=0;}
	*/

		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_W_IN);	
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_W_IN)& 0x00003fff)) 
		{op0_res3 =1;
		rumboot_printf("op0_res3 error\n");}	
		else 
		{op0_res3=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_H_IN);	
	if ((tmpr & 0x00003fff) !=(ioread32(base  + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_H_IN)& 0x00003fff))	
		{op0_res4 =1;
		rumboot_printf("op0_res4 error\n");}	
		else 
		{op0_res4=0;}

		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_C_IN);			
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_C_IN)& 0x00003fff))
		{op0_res5 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_DATA_C_IN);
		rumboot_printf("op0_res6_tmp =%x\n",tmp);	
		rumboot_printf("op0_res5 error\n");}	
		else 
		{op0_res5=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_CFG);	
	if ((tmpr & 0x0000003F ) != (ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_CFG)& 0x0000003F))
		{op0_res6 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_CFG);
		rumboot_printf("op0_res6_tmp =%x\n",tmp);		
		rumboot_printf("op0_res6 error\n");}
		else 
		{op0_res6=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BASE_L);	
	if ((tmpr) != ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BASE_L))
		{op0_res7 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BASE_L);
		rumboot_printf("op0_res7_tmp =%x\n",tmp);	
		rumboot_printf("op0_res7 error\n");}
		else 
		{op0_res7=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_LINE_STRIDE);
	if ((tmpr& 0x0003FFFF) !=(ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_LINE_STRIDE)& 0x0003FFFF)) //DOC ??	
		{op0_res8 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_LINE_STRIDE);
		rumboot_printf("op0_res8_tmp =%x\n",tmp);	
		rumboot_printf("op0_res8 error\n");}
		else 
		{op0_res8=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);
	if ((tmpr& 0x0003FFFF) != (ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_VECTOR_STRIDE) & 0x0003FFFF)) //DOC ?? 
		{op0_res9 =1;
	  	tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);
		rumboot_printf("op0_res9_tmp =%x\n",tmp);
		rumboot_printf("op0_res9 error\n");}
		else 
		{op0_res9=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_ELEM_STRIDE);	
	if ((tmpr & 0x0003FFFF) != (ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_ELEM_STRIDE) & 0x0003FFFF)) //DOC ??
		{op0_res10 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_ELEM_STRIDE);
		rumboot_printf("op0_res10_tmp =%x\n",tmp);
		rumboot_printf("op0_res10 error\n");}
		else 
		{op0_res10=0;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_ELEM_STRIDE);
		rumboot_printf("op0_res10_tmp =%x\n",tmp);}
		
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);
	if ((tmpr& 0x000003FFF) !=(ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X) & 0x000003FFF)) //DOC ??	
		{op0_res11 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);
		rumboot_printf("op0_res11_tmp =%x\n",tmp);	
		rumboot_printf("op0_res11 error\n");}
		else 
		{op0_res11=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
	if ((tmpr & 0x000003FFF) != (ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y)& 0x000003FFF)) //DOC ??
		{op0_res12 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
		rumboot_printf("op0_res12_tmp =%x\n",tmp);	  
		rumboot_printf("op0_res12 error\n");}
		else 
		{op0_res12=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);	
	if ((tmpr & 0x000003FFF) != (ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z)& 0x000003FFF)) //DOC ??
		{op0_res13 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);
		rumboot_printf("op0_res13_tmp =%x\n",tmp);	
		rumboot_printf("op0_res13 error\n");}
		else 
		{op0_res13=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_X);	
	if ((tmpr & 0x000003FFF) !=(ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_X)& 0x000003FFF)) //DOC ??	
		{op0_res14 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_X);
		rumboot_printf("op0_res14_tmp =%x\n",tmp);	
		rumboot_printf("op0_res14 error\n");}
		else 
		{op0_res14=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
	if ((tmpr & 0x000003FFF) != (ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Y)& 0x000003FFF)) //DOC ??
		{op0_res15 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
		rumboot_printf("op0_res15_tmp =%x\n",tmp);	  
		rumboot_printf("op0_res15 error\n");}
		else 
		{op0_res15=0;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
		rumboot_printf("op0_res16_tmp =%x\n",tmp);}	
		
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);	
	if ((tmpr & 0x000003FFF) != (ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Z)& 0x000003FFF)) //DOC ??
		{op0_res16 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);
		rumboot_printf("op0_res16_tmp =%x\n",tmp);	
		rumboot_printf("op0_res16 error\n");}
		else 
		{op0_res16=0;}

		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
	if ((tmpr & 0x000003FFF) != (ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X)& 0x000003FFF)) //DOC ??	bug fix
		{op0_res17 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);
		rumboot_printf("op0_res17_tmp =%x\n",tmp);	
		rumboot_printf("op0_res17 error\n");		
		}
		else 
		{op0_res17=0;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
		rumboot_printf("op0_res17_tmp =%x\n",tmp);		
		}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);
	if ((tmpr & 0x000003FFF) != (ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y)& 0x000003FFF)) //DOC ??
		{op0_res18 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);
		rumboot_printf("op0_res18_tmp =%x\n",tmp);	  
		rumboot_printf("op0_res18 error\n");}
		else 
		{op0_res18=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);	
	if ((tmpr & 0x000003FFF) != (ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z)& 0x000003FFF)) //DOC ??
		{op0_res19 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);
		rumboot_printf("op0_res19_tmp =%x\n",tmp);	
		rumboot_printf("op0_res19 error\n");}
		else 
		{op0_res19=0;}
	
	iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_SIZE);		
	if ((tmpr & 0x0000001F) != (ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_SIZE) & 0x0000001F))
		{op0_res21 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op0_res21_tmp =%d\n",tmp);	
		rumboot_printf("op0_res21 error\n");}
		else 
		{op1_res21=0;
	    tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op0_res21_tmp =%d\n",tmp);}
		
		iowrite32(tmpr, base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_STRIDE);		
	if ((tmpr) != ioread32( base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_STRIDE ))
		{op1_res22 =1;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op0_res22_tmp =%d\n",tmp);	
		rumboot_printf("op0_res22 error\n");}
		else 
		{op0_res22=0;
	    tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op0_res22_tmp =%d\n",tmp);}
	
//---------------------------Configuration OP1_RDMA-----------------------------------
	/*if ((0x00000000) !=(ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_STATUS)& 0x00030003)) 	
		{op1_res0 =1;
		rumboot_printf("op1_res0 error\n");}
		else 
		{op1_res0=0;}	
	if ((0x00000000) != (ioread32(base + base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_POINTER)& 0x00010001))
		{op1_res1 =1;
		rumboot_printf("op1_res1 error\n");}	
		else 
		{op1_res1=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_OP_ENABLE);	
	if ((tmpr & 0x00000001) !=(ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_OP_ENABLE)& 0x00000001)) 	
		{op1_res2 =1;
		rumboot_printf("op1_res2 error\n");}
		else 
		{op1_res2=0;}
*/	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_W_IN);	
	if ((tmpr& 0x00003fff) != (ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_W_IN)& 0x00003fff)) 
		{op1_res3 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_W_IN);	
		rumboot_printf("op1_res3_tmp =%x\n",tmp);			
		rumboot_printf("op1_res3 error\n");}	
		else 
		{op1_res3=0;}

		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_H_IN);	
	if ((tmpr& 0x00003fff) !=(ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_H_IN)& 0x00003fff))	
		{op1_res4 =1;
		rumboot_printf("op1_res4 error\n");}	
		else 
		{op1_res4=0;}

		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_C_IN);			
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_C_IN)& 0x00003fff))
		{op1_res5 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_DATA_C_IN);	
		rumboot_printf("op1_res5_tmp =%x\n",tmp);		
		rumboot_printf("op1_res5 error\n");}	
		else 
		{op1_res5=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_CFG);	
	if ((tmpr & 0x0000003F) != (ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_CFG)& 0x0000003F))
		{op1_res6 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_CFG);	
		rumboot_printf("op1_res6_tmp =%x\n",tmp);		
		rumboot_printf("op1_res6 error\n");}
		else 
		{op1_res6=0;}

		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BASE_L);	
	if ((tmpr) != ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BASE_L))
		{op1_res7 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BASE_L);	
		rumboot_printf("op1_res7_tmp =%x\n",tmp);		
		rumboot_printf("op1_res7 error\n");}
		else 
		{op1_res7=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_LINE_STRIDE);
	if ((tmpr  & 0x0003FFFF) !=(ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_LINE_STRIDE)& 0x0003FFFF)) //DOC	
		{op1_res8 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_LINE_STRIDE);	
		rumboot_printf("op1_res8_tmp =%x\n",tmp);		
		rumboot_printf("op1_res8 error\n");}
		else 
		{op1_res8=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);
	if ((tmpr &  0x0003FFFF) != (ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_VECTOR_STRIDE)& 0x0003FFFF)) //DOC
		{op1_res9 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);	
		rumboot_printf("op1_res9_tmp =%x\n",tmp);		  
		rumboot_printf("op1_res9 error\n");}
		else 
		{op1_res9=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_ELEM_STRIDE);	
	if ((tmpr & 0x0003FFFF) != (ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_ELEM_STRIDE)& 0x0003FFFF))  //DOC ??
		{op1_res10 =1; 
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_ELEM_STRIDE);	
		rumboot_printf("op1_res10_tmp =%x\n",tmp);		
		rumboot_printf("op1_res10 error\n");}
		else 
		{op1_res10=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);
	if ((tmpr& 0x00003FFF) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X)& 0x00003FFF))  //DOC ??	
		{op1_res11 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);	
		rumboot_printf("op1_res11_tmp =%x\n",tmp);		
		rumboot_printf("op1_res11 error\n");}
		else 
		{op1_res11=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);
	if ((tmpr & 0x00003FFF) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y)& 0x00003FFF))  //DOC ??
		{op1_res12 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);	
		rumboot_printf("op1_res12_tmp =%x\n",tmp);		  
		rumboot_printf("op1_res12 error\n");}
		else 
		{op1_res12=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);	
	if ((tmpr& 0x00003FFF) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z)& 0x00003FFF))  //DOC ??
		{op1_res13 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);	
		rumboot_printf("op1_res13_tmp =%x\n",tmp);		
		rumboot_printf("op1_res13 error\n");}
		else 
		{op1_res13=0;}

		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_X);	
	if ((tmpr& 0x00003FFF) !=(ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_X)& 0x00003FFF))  //DOC ??	
		{op1_res14 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_X);	
		rumboot_printf("op1_res14_tmp =%x\n",tmp);		
		rumboot_printf("op1_res14 error\n");}
		else 
		{op1_res14=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);
	if ((tmpr& 0x00003FFF) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Y)& 0x00003FFF))  //DOC ??
		{op1_res15 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);	
		rumboot_printf("op1_res15_tmp =%x\n",tmp);		  
		rumboot_printf("op1_res15 error\n");}
		else 
		{op1_res15=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);	
	if ((tmpr& 0x00003FFF) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Z)& 0x00003FFF))  //DOC ??
		{op1_res16 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);	
		rumboot_printf("op1_res16_tmp =%x\n",tmp);		
		rumboot_printf("op1_res16 error\n");}
		else 
		{op1_res16=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
	if ((tmpr& 0x00003FFF) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X)& 0x00003FFF))  //DOC ??	bug fix
		{op1_res17 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
		rumboot_printf("op1_res17_tmp =%x\n",tmp);		
		rumboot_printf("op1_res17 error\n");}
		else 
		{op1_res17=0;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
		rumboot_printf("op1_res17_tmp =%x\n",tmp);	
		}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);
		if ((tmpr& 0x00003FFF) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y)& 0x00003FFF))  //DOC ??	
		{op1_res18 =1;
	  	tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);	
		rumboot_printf("op1_res18_tmp =%x\n",tmp);		
		rumboot_printf("op1_res18 error\n");}
		else 
		{op1_res18=0;
	  	tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);	
		rumboot_printf("op1_res18_tmp =%x\n",tmp);		
		}
	
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);	
	if ((tmpr& 0x00003FFF) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z)& 0x00003FFF))
		{op1_res19 =1;
	  	tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);	
		rumboot_printf("op1_res19_tmp =%x\n",tmp);	
		rumboot_printf("op1_res19 error\n");}
		else 
		{op1_res19=0;}
		
	/*if ((tmpr) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_OP2_ALU_CMP_RESULT))
		{op2_res20 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_OP2_ALU_CMP_RESULT);
		rumboot_printf("op2_res20_tmp =%d\n",tmp);	
		rumboot_printf("op2_res20 error\n");}
		else 
		{op2_res20=0;
	    tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_OP2_ALU_CMP_RESULT);
		rumboot_printf("op2_res20_tmp =%d\n",tmp);}	*/
		
	iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_SIZE);		
	if ((tmpr & 0x0000001F) != (ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_SIZE) & 0x0000001F))
		{op1_res21 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op1_res21_tmp =%d\n",tmp);	
		rumboot_printf("op1_res21 error\n");}
		else 
		{op1_res21=0;
	    tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op1_res21_tmp =%d\n",tmp);}
		
		iowrite32(tmpr, base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_STRIDE);		
	if ((tmpr) != ioread32( base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_STRIDE ))
		{op1_res22 =1;
		tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op1_res22_tmp =%d\n",tmp);	
		rumboot_printf("op1_res22 error\n");}
		else 
		{op1_res22=0;
	    tmp= ioread32(base + NU_VPE_OP1_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op1_res22_tmp =%d\n",tmp);}
					
//---------------------------Configuration OP2_RDMA-----------------------------------	
/* if ((0x00000000) !=(ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_STATUS)& 0x00030003)) 	
		{op2_res0 =1;
		rumboot_printf("op2_res0 error\n");}
		else 
		{op2_res0=0;}
	
	if ((0x00000000) != (ioread32(base +  NU_VPE_OP2_RDMA + NU_VPE_RDMA_POINTER)& 0x00010001))
		{op2_res1 =1;
		rumboot_printf("op2_res1 error\n");}	
		else 
		{op2_res1=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_OP_ENABLE);	
	if ((tmpr & 0x00000001) !=(ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_OP_ENABLE)& 0x00000001)) 	
		{op2_res2 =1;
		rumboot_printf("op2_res2 error\n");}
		else 
		{op2_res2=0;}
	*/
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_W_IN);		
	if ((tmpr & 0x00001fff) != (ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_W_IN)& 0x00001fff)) 
		{op2_res3 =1;
		rumboot_printf("op2_res3 error\n");}	
		else 
		{op2_res3=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_H_IN);		
	if ((tmpr & 0x00001fff) !=(ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_H_IN)& 0x00001fff))	
		{op2_res4 =1;
		rumboot_printf("op2_res4 error\n");}	
		else 
		{op2_res4=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_C_IN);				
	if ((tmpr & 0x00003fff) != (ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_C_IN)& 0x00003fff)) //DOC ???
		{op2_res5 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_DATA_C_IN);	
		rumboot_printf("op2_res5_tmp =%x\n",tmp);		
		rumboot_printf("op2_res5 error\n");}	
		else 
		{op2_res5=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_CFG);		
	if ((tmpr & 0x0000003F) != (ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_CFG)& 0x0000003F))
		{op2_res6 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_CFG);	
		rumboot_printf("op2_res6_tmp =%x\n",tmp);	
		rumboot_printf("op2_res6 error\n");}
		else 
		{op2_res6=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BASE_L);		
	if ((tmpr) != ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BASE_L))
		{op2_res7 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BASE_L);	
		rumboot_printf("op2_res7_tmp =%x\n",tmp);	
		rumboot_printf("op2_res7 error\n");}
		else 
		{op2_res7=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_LINE_STRIDE);	
	if ((tmpr& 0x0003FFFF) !=(ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_LINE_STRIDE)& 0x0003FFFF)) 	
		{op2_res8 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_LINE_STRIDE);	
		rumboot_printf("op2_res8_tmp =%x\n",tmp);	
		rumboot_printf("op2_res8 error\n");}
		else 
		{op2_res8=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);	
	if ((tmpr& 0x0003FFFF) != (ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_VECTOR_STRIDE)& 0x0003FFFF)) 
		{op2_res9 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_VECTOR_STRIDE);	
		rumboot_printf("op2_res9_tmp =%x\n",tmp);	  
		rumboot_printf("op2_res9 error\n");}
		else 
		{op2_res9=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_ELEM_STRIDE);		
	if ((tmpr& 0x0003FFFF) != (ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_ELEM_STRIDE)& 0x0003FFFF)) 
		{op2_res10 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_ELEM_STRIDE);	
		rumboot_printf("op2_res10_tmp =%x\n",tmp);	
		rumboot_printf("op2_res10 error\n");}
		else 
		{op2_res10=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);	
	if ((tmpr& 0x0003FFF) !=(ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X)& 0x00003FFF)) 	
		{op2_res11 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_X);	
		rumboot_printf("op2_res11_tmp =%x\n",tmp);	
		rumboot_printf("op2_res11 error\n");}
		else 
		{op2_res11=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);	
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y)& 0x00003FFF)) 
		{op2_res12 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Y);	
		rumboot_printf("op2_res12_tmp =%x\n",tmp);	  
		rumboot_printf("op2_res12 error\n");}
		else 
		{op2_res12=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);		
	if ((tmpr& 0x0003FFF) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z)& 0x00003FFF))  //DOC ??
		{op2_res13 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_ST_SIZE_Z);	
		rumboot_printf("op2_res13_tmp =%x\n",tmp);	
		rumboot_printf("op2_res13 error\n");}
		else 
		{op2_res13=0;}

		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_X);		
	if ((tmpr & 0x00003FFF) !=(ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_X)& 0x00003FFF)) //DOC ??	
		{op2_res14 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_X);	
		rumboot_printf("op2_res14_tmp =%x\n",tmp);	
		rumboot_printf("op2_res14 error\n");}
		else 
		{op2_res14=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Y);	
	if ((tmpr  & 0x00003FFF) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Y)& 0x00003FFF)) //DOC ??	
		{op2_res15 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA +NU_VPE_RDMA_BOX_SIZE_Y);	
		rumboot_printf("op2_res15_tmp =%x\n",tmp);	    
		rumboot_printf("op2_res15 error\n");}
		else 
		{op2_res15=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);		
	if ((tmpr  & 0x00003FFF) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Z)& 0x00003FFF)) //DOC ??
		{op2_res16 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_SIZE_Z);	
		rumboot_printf("op2_res16_tmp =%x\n",tmp);	
		rumboot_printf("op2_res16 error\n");}
		else 
		{op2_res16=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
	if ((tmpr & 0x00003FFF) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X)& 0x00003FFF)) //DOC ??	 bug fix
		{op2_res17 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
		rumboot_printf("op2_res17_tmp =%x\n",tmp);	
		rumboot_printf("op2_res17 error\n");}
		else 
		{op2_res17=0;
		tmp= ioread32(base + NU_VPE_OP0_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_X);	
		rumboot_printf("op2_res17_tmp =%x\n",tmp);}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);
	if ((tmpr & 0x00003FFF) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y) & 0x00003FFF)) //DOC ??
		{op2_res18 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Y);	
		rumboot_printf("op2_res18_tmp =%x\n",tmp);	  
		rumboot_printf("op2_res18 error\n");}
		else 
		{op2_res18=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);	
	if ((tmpr & 0x00003FFF) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z) & 0x00003FFF)) //DOC ??
		{op2_res19 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BOX_OFFSET_SIZE_Z);	
		rumboot_printf("op2_res19_tmp =%x\n",tmp);	
		rumboot_printf("op2_res19 error\n");}
		else 
		{op2_res19=0;}
	
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_SIZE);		
	if ((tmpr & 0x0000001F) != (ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_SIZE) & 0x0000001F))
		{op2_res21 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op2_res21_tmp =%d\n",tmp);	
		rumboot_printf("op2_res21 error\n");}
		else 
		{op2_res21=0;
	    tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_SIZE );
		rumboot_printf("op2_res21_tmp =%d\n",tmp);}
		
		iowrite32(tmpr, base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE);		
  if ((tmpr) != ioread32( base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE ))
		{op2_res22 =1;
		tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op2_res22_tmp =%d\n",tmp);	
		rumboot_printf("op2_res22 error\n");}
		else 
		{op2_res22=0;
	    tmp= ioread32(base + NU_VPE_OP2_RDMA + NU_VPE_RDMA_BATCH_STRIDE );
		rumboot_printf("op2_res22_tmp =%d\n",tmp);}
			
	

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_CFG);
	if ((tmpr & 0x0000000F) !=(ioread32(base + NU_VPE + NU_VPE_LUT_CFG)& 0x0000000F)) 	
		{lut_res2 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_CFG);
		rumboot_printf("lut_res2_tmp =%x\n",tmp);	
		rumboot_printf("lut_res2 error\n");}
		else 
		{lut_res2=0;
	    tmp= ioread32(base + NU_VPE + NU_VPE_LUT_CFG);
		rumboot_printf("lut_res2_tmp =%x\n",tmp);}
		
		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_SYM_POINT);		
	if ((tmpr) != (ioread32(base +  NU_VPE + NU_VPE_LUT_SYM_POINT)& 0xFFFFFFFF)) 
		{lut_res3 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_SYM_POINT);
		rumboot_printf("lut_res3_tmp =%x\n",tmp);	
		rumboot_printf("lut_res3 error\n");}	
		else 
		{lut_res3=0;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_SYM_POINT);
		rumboot_printf("lut_res3_tmp =%x\n",tmp);}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_X_SHIFT);	
	if ((tmpr & 0x000000FF) !=(ioread32(base +NU_VPE + NU_VPE_LUT_TAB1_X_SHIFT)& 0x000000FF))	
		{lut_res4 =1;
		tmp= ioread32(base + NU_VPE +NU_VPE_LUT_TAB1_X_SHIFT);
		rumboot_printf("lut_res4_tmp =%x\n",tmp);	
		rumboot_printf("lut_res4 error\n");}	
		else 
		{lut_res4=0;
	    tmp= ioread32(base + NU_VPE +NU_VPE_LUT_TAB1_X_SHIFT);
		rumboot_printf("lut_res4_tmp =%x\n",tmp);}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT);	
	if ((tmpr & 0x000000FF) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT)& 0x000000FF))
		{lut_res5 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT);
		rumboot_printf("lut_res5_tmp =%x\n",tmp);			
		rumboot_printf("lut_res5 error\n");}	
		else 
		{lut_res5=0;
	    tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_SHIFT);
		rumboot_printf("lut_res5_tmp =%x\n",tmp);}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_X_START);	
	if ((tmpr) !=(ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_START)& 0xFFFFFFFF)) //not corresp. doc
		{lut_res6 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_START);
		rumboot_printf("lut_res6_tmp =%x\n",tmp);	
		rumboot_printf("lut_res6 error\n");}
		else 
		{lut_res6=0;
	    tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_START);
		rumboot_printf("lut_res6_tmp =%x\n",tmp);}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_X_END);		
	if ((tmpr) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_END)& 0xFFFFFFFF))
		{lut_res7 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_X_END);
		rumboot_printf("lut_res7_tmp =%d\n",tmp);
		rumboot_printf("lut_res7 error\n");}
		else 
		{lut_res7=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_X_START);
	if ((tmpr) !=(ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_START)& 0xFFFFFFFF))	
		{lut_res8 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_START);
		rumboot_printf("lut_res8_tmp =%d\n",tmp);	
		rumboot_printf("lut_res8 error\n");}
		else 
		{lut_res8=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_X_END);
	if ((tmpr) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_END)& 0xFFFFFFFF))
		{lut_res9 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_X_END);
		rumboot_printf("lut_res9_tmp =%d\n",tmp);		  
		rumboot_printf("lut_res9 error\n");}
		else 
		{lut_res9=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE);	
	if ((tmpr) != (ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE)& 0xFFFFFFFF)) //???
		{lut_res10 =1;  
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SCALE);
		rumboot_printf("lut_res10_tmp =%d\n",tmp);		
		rumboot_printf("lut_res10 error\n");}
		else 
		lut_res10=0;

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT);
	if ((tmpr &0x000003FF) !=(ioread32( base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT)& 0x000003FF))	
		{lut_res11 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB1_SLOPE_SHIFT);
		rumboot_printf("lut_res11_tmp =%d\n",tmp);	
		rumboot_printf("lut_res11 error\n");}
		else 
		{lut_res11=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE);
	if ((tmpr) != (ioread32( base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE)& 0xffffffff))
		{lut_res12 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SCALE)& 0xffffffff;
		rumboot_printf("lut_res12_tmp =%x\n",tmp);
		rumboot_printf("lut_res12 error\n");}
		else 
		{lut_res12=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT);	
	if ((tmpr & 0x0000003F) != (ioread32( base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT)& 0x0000003F))
		{lut_res13 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_LUT_TAB2_SLOPE_SHIFT);
		rumboot_printf("lut_res13_tmp =%x\n",tmp);		
		rumboot_printf("lut_res13 error\n");}
		else 
		lut_res13=0;
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL);
	if ((tmpr) != (ioread32( base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL)& 0xffffffff))
		{vpe_res0 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OUT_CVT_OFFSET_VAL);
		rumboot_printf("vpe_res0_tmp =%x\n",tmp);	  
		rumboot_printf("vpe_res0 error\n");}
		else 
		{vpe_res0=0;}

		iowrite32(tmpr, base + NU_VPE + NU_VPE_OUT_CVT_SCALE_VAL);	
	if ((tmpr & 0x0000FFFF) != (ioread32( base + NU_VPE + NU_VPE_OUT_CVT_SCALE_VAL)& 0x0000FFFF))
		{vpe_res1 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OUT_CVT_SCALE_VAL);
		rumboot_printf("vpe_res1_tmp =%x\n",tmp);	
		rumboot_printf("vpe_res1 error\n");}
		else 
		{vpe_res1=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL);		
	if ((tmpr & 0x0000313F) !=(ioread32( base + NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL)& 0x0000313F))	
		{vpe_res2 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_OUT_CVT_TRUNC_VAL);
		rumboot_printf("vpe_res2_tmp =%x\n",tmp);	
		rumboot_printf("vpe_res2 error\n");}
		else 
		{vpe_res2=0;}
/*	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_INF_NUM_IN);	
	if ((tmpr) != ioread32( base + NU_VPE + NU_VPE_INF_NUM_IN))
		{vpe_res3 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_INF_NUM_IN);
		rumboot_printf("{vpe_res3_tmp =%x\n",tmp);	  
		rumboot_printf("{vpe_res3 error\n");}
		else 
		{vpe_res3=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_NAN_NUM_OUT);	
	if ((tmpr) != ioread32( base + NU_VPE + NU_VPE_NAN_NUM_OUT))
		{vpe_res4 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_NAN_NUM_OUT );
		rumboot_printf("vpe_res4_tmp =%x\n",tmp);	
		rumboot_printf("vpe_res4 error\n");}
		else 
		{vpe_res4=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_SATUR_NUM_OUT );	
	if ((tmpr) != ioread32( base + NU_VPE + NU_VPE_SATUR_NUM_OUT ))
		{vpe_res5 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_SATUR_NUM_OUT );
		rumboot_printf("vpe_res5_tmp =%d\n",tmp);	
		rumboot_printf("vpe_res5 error\n");}
		else 
		{vpe_res5=0;}
*/
		iowrite32(tmpr, base + NU_VPE + NU_VPE_AXI_PARAM );		
	if ((tmpr &  0x0F07030F) != (ioread32( base + NU_VPE + NU_VPE_AXI_PARAM ) & 0x0F07030F))
		{vpe_res7 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_AXI_PARAM );
		rumboot_printf("vpe_res7_tmp =%d\n",tmp);	
		rumboot_printf("vpe_res7 error\n");}
		else 
		{vpe_res7=0;}
	
		iowrite32(tmpr, base + NU_VPE + NU_VPE_CUBE_SIZE );		
	if ((tmpr) != ioread32( base + NU_VPE + NU_VPE_CUBE_SIZE ))
		{vpe_res8 =1;
		tmp= ioread32(base + NU_VPE + NU_VPE_CUBE_SIZE );
		rumboot_printf("vpe_res8_tmp =%d\n",tmp);	
		rumboot_printf("vpe_res8 error\n");}
		else 
		{vpe_res8=0;}
/*	
		iowrite32(tmpr, base + NU_VPE + REG_VPE_IRQ_MASK_ADDR );	
	if ((tmpr & 0x00000003) != (ioread32( base + NU_VPE + REG_VPE_IRQ_MASK_ADDR )& 0x00000003))
		{vpe_res12 =1;
		tmp= ioread32(base + NU_VPE + REG_VPE_IRQ_MASK_ADDR );
		rumboot_printf("vpe_res12_tmp =%x\n",tmp);	
		rumboot_printf("vpe_res12 error\n");}
		else 
		{vpe_res12=0;}	
*/	
//----------------------------------------------------------------
	  		
	  resr = 	cp0_res0 || cp0_res1|| cp0_res2 || cp0_res3 || cp0_res4 || cp0_res5 ||
				cp1_res0 || cp1_res1|| cp1_res2 || cp1_res3 || cp1_res4 || cp1_res5 ||
				cp2_res2 || cp2_res1|| cp2_res2 || cp2_res3 || cp2_res4 || cp2_res5 || cp2_res6 || cp2_res7|| cp2_res8 || cp2_res9 || cp2_res10 || cp2_res11 ||
				/*src_res0 || src_res1|| src_res2 || */src_res3 || src_res4 || src_res5 || src_res7 || src_res8 ||
				src_res9 || src_res10 || src_res11 || src_res12 || src_res13 || src_res14 || src_res15 || 
				src_res16 || src_res17 || src_res18 || src_res19 || src_res21 ||
				/*op0_res0 || op0_res1|| op0_res2 || */op0_res3 || op0_res4 || op0_res5 || op0_res7 || op0_res8 ||
				op0_res9 || op0_res10 || op0_res11 || op0_res12 || op0_res13 || op0_res14 || op0_res15 || 
				op0_res16 || op0_res17 || op0_res18 || op0_res19 ||
				/*op1_res0 || op1_res1|| op1_res2 || */op1_res3 || op1_res4 || op1_res5 || op1_res7 || op1_res8 ||
				op1_res9 || op1_res10 || op1_res11 || op1_res12 || op1_res13 || op1_res14 || op1_res15 || 
				op1_res16 || op1_res17 || op1_res18 || op1_res19 || op1_res21 ||op1_res22 ||
				/*op2_res0 || op2_res1|| op2_res2 || */op2_res3 || op2_res4 || op2_res5 || op2_res7 || op2_res8 ||
				op2_res9 || op2_res10 || op2_res11 || op2_res12 || op2_res13 || op2_res14 || op2_res15 || 
				op2_res16 || op2_res17 || op2_res18 || op2_res19 || op2_res21 ||op2_res22 || op_res ||
			     lut_res0 || lut_res2 || lut_res3 || lut_res4 || lut_res5 || lut_res6 || lut_res7 || lut_res8 ||
				lut_res9 || lut_res10 || lut_res11 || lut_res12 || lut_res13 ||
				vpe_res0 ||vpe_res1 || vpe_res2 || vpe_res7 || vpe_res8 || vpe_res12; 
				
	  resw = 	/*resw0 || resw1|| */resw2 || resw3 || resw4 || resw5 || resw6 || resw7 || resw8 || resw9 ||
				resw10 || resw11 || resw12 || resw13 || resw14 || resw15 || resw16 || resw17 ||
				resw18 || resw19 ; 
	  			
	
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
	
	vpe_rdma_offset =  NU_VPE_OP2_RDMA + NU_VPE_RDMA_BASE_L;
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
