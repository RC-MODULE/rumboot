#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <devices/rcm_na/nu_lib.h>
#include <devices/rcm_na/nu_test_lib.h>
#include <regs/regs_na.h>
#include <regs/regs_nu_mpe.h>
#include <platform/devices.h>
#include <devices/rcm_na/nu_test_macro.h>

uint32_t data;
volatile uint32_t* dump_ptr_adr;
//volatile uint32_t* dump_ptr_data;

int dump_size;
//int dump_size2;

int run_na_cmd_dma(uintptr_t npe_base,uintptr_t dump_ptr_adr, uintptr_t size){
	iowrite32( 0x100 ,npe_base + NA_CU_REGS_BASE   + NA_UNITS_MODE);
	iowrite32(  0x0002000f  ,npe_base + NA_CU_REGS_BASE   + CMD_DMA_AXI_PARAM);
	//rumboot_printf("AXI_PARAM =%x\n",ioread32(npe_base + NA_CU_REGS_BASE   + CMD_DMA_AXI_PARAM));
	iowrite32( dump_ptr_adr,npe_base + NA_CU_REGS_BASE +  CMD_DMA_BASE_ADDR);
	//rumboot_printf("BASE_ADDR =%x\n",ioread32(npe_base + NA_CU_REGS_BASE +  CMD_DMA_BASE_ADDR));
	iowrite32( size,npe_base + NA_CU_REGS_BASE +  CMD_DMA_PAGE_SIZE);
	//rumboot_printf("PAGE_SIZE =%x\n",ioread32(npe_base + NA_CU_REGS_BASE +  CMD_DMA_PAGE_SIZE));
	iowrite32( 0x1,npe_base + NA_CU_REGS_BASE +  CMD_DMA_CFG);	
	while (( (ioread32(npe_base + NA_CU_REGS_BASE +  CMD_DMA_INT_STATUS)>> 16) & 1) !=1) {}	//CMD_DMA_CNTX_APPLY	
	{iowrite32( (1<<16),npe_base + NA_CU_REGS_BASE   + CMD_DMA_INT_RESET);}
	rumboot_printf("Done CMD DMA context\n");
	return 0;
}
int na_dma_cmd_complete(uintptr_t npe_base){
	rumboot_printf("Wait NA CND DMA page complete ...\n");
	while (( (ioread32(npe_base + NA_CU_REGS_BASE +  CMD_DMA_INT_STATUS)>> 0) & 1) !=1) {}
	{iowrite32( (1<<0),npe_base + NA_CU_REGS_BASE + CMD_DMA_INT_RESET);	
	rumboot_printf(" NA CND DMA page complete done\n");
	}
	return 0;
}
int nu_mpe_rdma_w_reg_check(uintptr_t base) {
	int res;
	int res0,res1,res2,res3,res4,res5,res6,res7;
	int resw0,resw1,resw2,resw3,resw4,resw5;
 
//--------Registers WRITE CHANNEL---------------	
	if(( 0x00000072)  !=(ioread32(base + MPE_RDMA_D_BASE + X_Cfg_MSha)))
		{resw0 =1;
		rumboot_printf("X_Cfg_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + X_Cfg_MSha));
		rumboot_printf("Unexpected X_Cfg_MSha error\n");}
		else 
		{resw0=0;}
	if ((0x00000000)  != (ioread32(base + MPE_RDMA_D_BASE + WR_THRECtrl_MSha)))
		{resw1 =1;
		rumboot_printf("WR_THRECtrl_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + WR_THRECtrl_MSha));
		rumboot_printf("Unexpected WR_THRECtrl_MSha error\n");}	
		else 
		{resw1=0;}
	if(( 0x00000000)  !=(ioread32(base + MPE_RDMA_D_BASE +WR_DECCtrl_MSha)))
		{resw2 =1;
		rumboot_printf("WR_DECCtrl_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + WR_DECCtrl_MSha));
		rumboot_printf("Unexpected  WR_THRECtrl_MSha error\n");}
		else 
		{resw2=0;}
	if ((0x00000001)  != (ioread32(base + MPE_RDMA_D_BASE + CHCfg_MSha))) //0x00000009  //!!!!!!!!!!
		{resw3 =1;
		rumboot_printf("CHCfg_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + CHCfg_MSha)); 
		rumboot_printf("Unexpected CHCfg_MSha error\n");}	
		else 
		{resw3=0;}
		if ((0x00000000)  != (ioread32(base + MPE_RDMA_D_BASE + PLC_ThreSha_MSha)))
		{resw4 =1;
		rumboot_printf("PLC_ThreSha_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + PLC_ThreSha_MSha)); 
		rumboot_printf("Unexpected PLC_ThreSha_MSha error\n");}	
		else 
		{resw4=0;}
	if ((0x00000000)  != (ioread32(base + MPE_RDMA_D_BASE + VLC_CntSha_MSha) & 0x000000ff))
		{resw5 =1;
		rumboot_printf("VLC_CntSha_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + VLC_CntSha_MSha)); 
		rumboot_printf("Unexpected VLC_CntSha_MSha error\n");}	
		else 
		{resw5=0;}
//--------Registers READ CHANNEL---------------		
	if ((0x000000007)  != (ioread32(base + MPE_RDMA_D_BASE + BIASCtrl_MSha)))
		{res0 =1;
		rumboot_printf("BIASCtrl_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + BIASCtrl_MSha)); 
		rumboot_printf("Unexpected BIASCtrl_MSha error\n");}	
		else 
		{res0=0;}
	if ((0x000000000)  != (ioread32(base + MPE_RDMA_D_BASE + RD_THRECtrl_MSha)))
		{res1 =1;
		rumboot_printf("RD_THRECtrl_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + RD_THRECtrl_MSha)); 
		rumboot_printf("Unexpected RD_THRECtrl_MSha error\n");}	
		else 
		{res1=0;}
	if ((0x000000000)  != (ioread32(base + MPE_RDMA_D_BASE + RD_THRECtrl_MSha)))
		{res2 =1;
		rumboot_printf("RD_DECCtrl_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + RD_DECCtrl_MSha)); 
		rumboot_printf("Unexpected RD_DECCtrl_MSha error\n");}	
		else 
		{res2=0;}
	if ((0x0000000ff)  != (ioread32(base + MPE_RDMA_D_BASE + PADCtrl_MSha)))
		{res3 =1;
		rumboot_printf("PADCtrl_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + PADCtrl_MSha)); 
		rumboot_printf("Unexpected PADCtrl_MSha error\n");}	
		else 
		{res3=0;}
	if ((0xf040)  != (ioread32(base + MPE_RDMA_D_BASE + LPXDR_MSha) &  0xffff))
		{res4 =1;
		rumboot_printf("LPXDR_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + LPXDR_MSha) & 0xffff); 
		rumboot_printf("Unexpected LPXDR_MSha error\n");}	
		else 
		{res4=0;}
	if ((0xf580)  != (ioread32(base + MPE_RDMA_D_BASE + RPXDR_MSha)& 0xffff))
		{res5 =1;
		rumboot_printf("RPXDR_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + RPXDR_MSha)); 
		rumboot_printf("Unexpected RPXDR_MSha error\n");}	
		else 
		{res5=0;}
	if ((0x00000000)  != (ioread32(base + MPE_RDMA_D_BASE + TPYDR_MSha)& 0xffff))
		{res6 =1;
		rumboot_printf("TPYDR_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + TPYDR_MSha)); 
		rumboot_printf("Unexpected TPYDR_MSha error\n");}	
		else 
		{res6=0;}
	if ((0x00000000)  != (ioread32(base + MPE_RDMA_D_BASE + BPYDR_MSha) & 0xffff))
		{res7 =1;
		rumboot_printf("BPYDR_MSha = %x\n",ioread32(base + MPE_RDMA_D_BASE + BPYDR_MSha)); 
		rumboot_printf("Unexpected BPYDR_MSha error\n");}
		else 
		{res7=0;}		

	   
	  res = 	res0 || res1 || res2 || res3 ||   res4 || res5 || res6 || res7 || 
	            resw0 || resw1 || resw2 || resw3 ||resw4 || resw5 ;	 
	if( res ==1) {
	 rumboot_printf("READ_REGS_MPE_RDMA_CH ERROR\n");
    return -1;}
			
 return 0; 
 } 	
int main(){
  int heap_id;
  int i; 
  int j;

  uint32_t * dump_ptr_adr;
 // uint32_t * dump_ptr_adr2; 
 
  dump_size =  864;//(Bias6_Cnt - X_Cfg_MSha)*2;

  int res=0;
  int res1 =0;
  int result; 
  rumboot_printf("Hello CMD DMA\n");
  //rumboot_printf("My dump_size is %x\n",dump_size * sizeof(uint32_t));
  heap_id = nu_get_heap_id();
  dump_ptr_adr = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, (dump_size/4) * sizeof(uint32_t),8);
  if (dump_ptr_adr == NULL) return -1;

//  dump_ptr_adr2 = (uint32_t*)rumboot_malloc_from_heap_aligned(heap_id, dump_size * sizeof(uint32_t),8);
 // if (dump_ptr_adr2 == NULL) return -1;


	rumboot_printf("dump_size =%x\n",dump_size);
	
	 for ( j = 0; j < dump_size/4 ; j+=2) {
		 
		if ( j == 0 ) {
		dump_ptr_adr[j+1] = 0x000072;
		dump_ptr_adr[j] =/*0x800104://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + X_Cfg_MSha ;
		//rumboot_printf("My adr j is %x\n",dump_ptr_adr[j]);
		//rumboot_printf("My data is %x\n",dump_ptr_adr[j+1]);
		}		
	else if ( j == 2 ){ 		   
		dump_ptr_adr[j+1] = 0x00000000;
	   	dump_ptr_adr[j] = /*0x800108;// */ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_THRECtrl_MSha;
		}		
	else if (j == 4){ 		   
		dump_ptr_adr[j+1] = 0x00000000;
	   	dump_ptr_adr[j] = /*0x80010c;//*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_DECCtrl_MSha;
		}				
	else if (j == 6){ 		   
		dump_ptr_adr[j+1] = 0x00000009;
	   	dump_ptr_adr[j] = /*0x800110;//*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + CHCfg_MSha; 
		}
	 else if (j == 8){ 		   
		dump_ptr_adr[j +1] = 0x00000000;
	   	dump_ptr_adr[j] = /* 0x800114;//*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + BADR_MSha;
		}	
		
	else if (j == 10){
		dump_ptr_adr[j +1] = 0x00000100;
		dump_ptr_adr[j] = /*0x800118;//*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + LADR_MSha;
		}
	else if (j == 12){
		dump_ptr_adr[j +1] = 0x0000000f;
		dump_ptr_adr[j] =  /*0x80011c;//*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + PLC_CntSha_MSha;
		}
	else if (j == 14){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*0x800120;//*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + PLC_ThreSha_MSha;
		}	
	else if (j == 16){
		dump_ptr_adr[j +1] = 0x00000006;
		dump_ptr_adr[j] = /*0x800124;*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + PLC_CUR_Cnt;
		}
	else if (j == 18){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*0x800128; //*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + PLC_Threshold;	
		}
	else if (j == 20){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*80012c://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + VLC_CntSha_MSha;
		}
	else if (j == 22){
		dump_ptr_adr[j +1] = 00000000;
		dump_ptr_adr[j] = /*800130://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + VLC_ThreSha_MSha;
		}
		
	else if (j == 24){
		dump_ptr_adr[j +1] = 0x0000008d;
		dump_ptr_adr[j] = /*800134://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + VLC_CUR_Cnt;
		}						
	else if (j == 26){
		dump_ptr_adr[j +1] = 0x0000007f;
		dump_ptr_adr[j] = /*800138://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + VLC_Threshold;
		}
	else if (j == 28){
		dump_ptr_adr[j +1] = 0x00000006;
		dump_ptr_adr[j] = /*80013c://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias1CntSha_MSha;
		}
	else if (j == 30){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*800140://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias1CntCmp_MSha;
		}		
	else if (j == 32){
		dump_ptr_adr[j +1] = 0x00000001;
		dump_ptr_adr[j] = /*800144://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias1_Cnt;
		}				
	else if (j == 34){
		dump_ptr_adr[j +1] = 0x0000000f;
		dump_ptr_adr[j] = /*800148://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias2CntSha_MSha;
		}
	else if (j == 36){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*80014c://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias2CntCmp_MSha;
		}						
	else if (j== 38){
		dump_ptr_adr[j +1] = 0x00000002;
		dump_ptr_adr[j] = /*800150://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias2_Cnt;
		}
	else if (j== 40){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*800154://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias3CntSha_MSha;
		}		
	else if (j== 42){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*800158://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias3CntCmp_MSha;
		}				
	else if (j== 44){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*80015c://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias3_Cnt;
		}
	else if (j== 46){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*800160://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias4CntSha_MSha;
		}		
	else if (j== 48){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*800164://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias4CntCmp_MSha;
		}				
	else if (j== 50){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*800168://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias4_Cnt;
		}
	else if (j== 52){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*80016c://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias5CntSha_MSha;
		}		
	else if (j== 54){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*800170://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias5CntCmp_MSha;
		}				
	else if (j== 56){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*800174://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias5_Cnt;
		}
	else if (j== 58){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*800178://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias6CntSha_MSha;
		}		
	else if (j== 60){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*80017c://*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + WR_Bias6CntCmp_MSha;
		}				
	else if (j== 62){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = /*0x800180;//*/ MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias6_Cnt;
		}
		//----------
    	if ( j == 64 ) {
		dump_ptr_adr[j +1] = 0x00000007;
		dump_ptr_adr[j] =  MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + BIASCtrl_MSha ;
		}		
	else if ( j == 66 ){ 		   
		dump_ptr_adr[j +1] = 0x00000000;
	   	dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_THRECtrl_MSha;
		}	
	else if (j == 68){ 		   
		dump_ptr_adr[j +1] = 0x00000000;
	   	dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_DECCtrl_MSha;
		}		
		
	else if (j == 70){ 		   
		dump_ptr_adr[j +1] = 0x000000ff;
	   	dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + PADCtrl_MSha;
		}
		
	 else if (j == 72){ 		   
		dump_ptr_adr[j +1] =0x00008000;//bfca;
	   	dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + BFCA_MSha;
		}	
		
	else if (j == 74){
		dump_ptr_adr[j +1] =0xfffff02e;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + AOffset_MSha;
		}
	else if (j == 76){
		dump_ptr_adr[j +1] = 0x0000000f;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + CURRADR;
		}
	else if (j == 78){
		dump_ptr_adr[j +1] = 0xfffff040;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + LPXOffset_MSha;
		}	
	else if (j == 80){
		dump_ptr_adr[j +1] = 0xfffff580;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RPXOffset_MSha;
		}
	else if (j == 82){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + LPadXTreshold;
		}
	else if (j == 84){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RPadXTreshold; 
		}
	else if (j == 86){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + TPadYOffset_MSha;
		}
		
	else if (j == 88){
		dump_ptr_adr[j +1] = 0x49800;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + BPadYOffset_MSha;
		}						
	else if (j == 90){
		dump_ptr_adr[j +1] = 0x0000007f;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + TPYThreshold;//??????
		}
	else if (j == 92){
		dump_ptr_adr[j +1] = 0x00000006;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + BPYThreshold ; //???????
		}
	else if (j == 94){
		dump_ptr_adr[j +1] = 0x00000005;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + CntSha_MSha ;
		}		
	else if (j == 96){
		dump_ptr_adr[j +1] = 0x00000001;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + CntThresholdSha_MSha;
		}				
	else if (j == 98){
		dump_ptr_adr[j +1] = 0x0000000f;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + SecThre;
		}
	else if (j == 100){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + SecCnt;
		}						
	
	else if (j== 102){
		dump_ptr_adr[j +1] = 0x00000006;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias1Sha_MSha;
		}		
	else if (j== 104){
		dump_ptr_adr[j +1] = 0x000000e4;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias1Adr;
		}				
	else if (j== 106){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias1CntSha_MSha;
		}
		else if (j== 108){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias1CntOffset_MSha;
		}		
	else if (j== 110){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias1CntThresholdSha_MSha;
		}				
	else if (j== 112){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias1CntCmp_MSha;
		}

	else if (j== 114){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias1BCnt;
		}
	else if (j== 116){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias1BThre;
		}
	else if (j== 118){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias2Sha_MSha;
		}				
	else if (j== 120){
		dump_ptr_adr[j +1] = 0x00000540;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias2Adr;
		}
	else if (j== 122){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias2CntSha_MSha;
		}		
	else if (j== 124){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias2CntOffset_MSha;
		}				
	else if (j== 126){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE + MPE_RDMA_D_BASE + Bias2CntThresholdSha_MSha;
		}
	//-----	
	if ( j == 128 ) {
		dump_ptr_adr[j+1] = 0x000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias2CntCmp_MSha;
		}
		
	else if ( j == 130 ){ 		   
		dump_ptr_adr[j+1] = 0x00000000;
	   	dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias2BCnt;
		}	
	else if (j == 132){ 		   
		dump_ptr_adr[j+1] = 0x00000000;
	   	dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias2BThre;
		}		
		
	else if (j == 134){ 		   
		dump_ptr_adr[j+1] = 0x00000000;
	   	dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias3Sha_MSha;
		}
	 else if (j == 136){ 		   
		dump_ptr_adr[j +1] = 0x00000a80;
	   	dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias3Adr;
		}	
		
	else if (j == 138){
		dump_ptr_adr[j +1] = 0x0000006f;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias3CntSha_MSha;
		}
	else if (j == 140){
		dump_ptr_adr[j +1] = 0x0000000f;
		dump_ptr_adr[j] =  MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias3CntOffset_MSha;
		}
	else if (j == 142){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias3CntThresholdSha_MSha;

		}
	else if (j == 144){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias3CntCmp_MSha;
		}	
		
	else if (j == 146){
		dump_ptr_adr[j +1] = 0x00000006;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias3BCnt;

		}
	else if (j == 148){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias3BThre;
		}
	else if (j == 150){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias4Sha_MSha;
		}
	else if (j == 152){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias4Adr;
		}
		
	else if (j == 154){
		dump_ptr_adr[j +1] = 0x0000008d;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias4CntSha_MSha ;
	
		}						
	else if (j == 156){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias4CntOffset_MSha;
		
		}
	else if (j == 158){
		dump_ptr_adr[j +1] = 0x00000006;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias4CntThresholdSha_MSha;

		}
	else if (j == 160){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias4CntCmp_MSha;
		}		
	else if (j == 162){
		dump_ptr_adr[j +1] = 0x00000001;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias4BCnt;
		}				
	else if (j == 164){
		dump_ptr_adr[j +1] = 0x0000000f;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias4BThre;
		}
	else if (j == 166){
		dump_ptr_adr[j +1] = 0x00000800;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias5Sha_MSha;
		}						
	else if (j== 168){
		dump_ptr_adr[j +1] = 0x00000002;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias5Adr;
		}
	else if (j== 170){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias5CntSha_MSha;
		}		
	else if (j== 172){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias5CntOffset_MSha;
		}				
	else if (j== 174){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias5CntThresholdSha_MSha;
		}
	else if (j== 176){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RD_Bias5CntCmp_MSha;
		}		
	else if (j== 178){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias5BCnt;
		}				
	else if (j== 180){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias5BThre;
		}
	else if (j== 182){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias6Sha_MSha; 
		}		
	else if (j== 184){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias6Adr;
		}				
	else if (j== 186){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias6CntSha_MSha;
		}
	else if (j== 188){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias6CntOffset_MSha;
		}		
	else if (j== 190){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias6CntThresholdSha_MSha;
		}
	else if (j== 192){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias6CntCmp_MSha;
		}		
		
	else if (j== 194){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias6BCnt;
		}
	else if (j== 196){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias6BThre;
		}
	else if (j== 198){
		dump_ptr_adr[j +1] = 0x00001800;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias7Sha_MSha;
		}					
	else if (j== 200){
		dump_ptr_adr[j +1] = 0x00000005;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias7Adr;
		}				
	else if (j== 202){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias7CntSha_MSha;
		}
	else if (j== 204){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias7CntCmp_MSha;
		}					
	else if (j== 206){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + Bias7BCnt;
		}					
	else if (j== 208){
		dump_ptr_adr[j +1] = 0xfffff040;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + LPXDR_MSha;
		}						
	else if (j== 210){
		dump_ptr_adr[j +1] = 0xfffff580;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + RPXDR_MSha; 
		}
	else if (j== 212){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + TPYDR_MSha;
		}
	else if (j== 214){
		dump_ptr_adr[j +1] = 0x00000000;
		dump_ptr_adr[j] = MY_MPE_REGS_BASE +  MPE_RDMA_D_BASE + BPYDR_MSha;		
	//	rumboot_printf("My adr j is %x\n",dump_ptr_adr[j]);		
	//	rumboot_printf("My data is %x\n",dump_ptr_adr[j+1]); 				
		}		
  }


   uintptr_t ptr_adr = rumboot_virt_to_dma(dump_ptr_adr);

	res = run_na_cmd_dma(NPE_BASE,ptr_adr,(864 )); //dump_size
	// rumboot_printf("res_1= %x\n",res); 
   if (res) 
  { rumboot_printf("Test FAILED\n");
     return 1;
   }
   else {
    res = na_dma_cmd_complete(NPE_BASE);
	//	rumboot_printf("res_2= %x\n",res); 
	if (res) 
	{ rumboot_printf("Test FAILED\n");
	return 1;}
  
	iowrite32(0x007, NPE_BASE + NA_CU_REGS_BASE   + NA_UNITS_MODE); //turn on APB bus
	iowrite32(0x0, MY_MPE_REGS_BASE + MAINCNT_WR);

	iowrite32(0x7, (MY_MPE_REGS_BASE + MPE_RDMA_D_BASE + RD_DECCtrl_MSha) );
	iowrite32(0x0, MY_MPE_REGS_BASE + MAINCNT_Sha);	
	iowrite32(0x1, MY_MPE_REGS_BASE + DMA_START);

	res1 = nu_mpe_rdma_w_reg_check(MY_MPE_REGS_BASE);
   
 //  rumboot_printf("res1= %x\n",res);
   }   
	if (res1 =0)  
  {rumboot_printf("Test FAILED\n");
	return 1;}

	{rumboot_printf("Test PASSED\n");
	return 0;} 
}