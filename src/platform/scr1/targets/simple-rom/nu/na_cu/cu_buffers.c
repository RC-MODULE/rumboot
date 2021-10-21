
#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices/nu_lib.h>
#include <platform/devices/nu_test_lib.h>
#include <regs/regs_na.h>
#include <platform/devices.h>

void mpe_dma_config(uintptr_t base, uint16_t* source_ptr, uint32_t buf_vecs, uint32_t base_in_buf) {
  //rumboot_printf("Configuring MPE_DMA\n");
  // Read Channel
  iowrite32(0x00000001,base+BIASCtrl_MSha);
  iowrite32(0x00000000,base+RD_THRECtrl_MSha);
  iowrite32(0x00000000,base+RD_DECCtrl_MSha);
  iowrite32(0x00000000,base+PADCtrl_MSha);
  iowrite32((uint32_t)source_ptr,base+BFCA_MSha);
  iowrite32(0x00000000,base+AOffset_MSha);
  iowrite32(0x0000007F,base+CntSha_MSha);
  iowrite32(0x00000000,base+CntThresholdSha_MSha);
  iowrite32(0x00000080,base+Bias1Sha_MSha);
  iowrite32(buf_vecs-1,base+RD_Bias1CntSha_MSha);
  iowrite32(0x00000000,base+Bias1CntOffset_MSha);
  iowrite32(0x00000000,base+Bias1CntThresholdSha_MSha);
  iowrite32(0x00000000,base+RD_Bias1CntCmp_MSha);
  // Write Channel
  iowrite32(0x00000011,base+X_Cfg_MSha);
  iowrite32(0x0000000C,base+CHCfg_MSha);
  iowrite32(base_in_buf,base+BADR_MSha);
  iowrite32(base_in_buf+(buf_vecs/16+((buf_vecs%16)>0))*16,base+LADR_MSha);

  if (buf_vecs > 128) {
    iowrite32(0x0000007F,base+PLC_CntSha_MSha);
    iowrite32(0x00000001,base+WR_THRECtrl_MSha);
    iowrite32(buf_vecs-1,base+PLC_ThreSha_MSha);
    int div = buf_vecs/128;
    int mod = buf_vecs%128;
    iowrite32(div+(mod>0)-1,base+WR_Bias1CntSha_MSha);
  }
  else {
    iowrite32(buf_vecs-1,base+PLC_CntSha_MSha);
    iowrite32(0x00000000,base+WR_Bias1CntSha_MSha);
  }
  iowrite32(0x00000000,base+VLC_CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias1CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias2CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias2CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias3CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias3CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias4CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias4CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias5CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias5CntCmp_MSha);
  iowrite32(0x00000000,base+WR_Bias6CntSha_MSha);
  iowrite32(0x00000000,base+WR_Bias6CntCmp_MSha);
  iowrite32(0x00000000,base+MAINCNT_Sha);
}

void mpe_dma_start(uintptr_t base, uint32_t dpnd) {
  //rumboot_printf("Starting MPE_DMA\n");
  iowrite32(0x00000001|(dpnd<<24),base+DMA_START);
}

CubeMetrics* metrics_in_data;
CubeMetrics* metrics_etalon_data; // reference
uint16_t *in_data;
uint16_t *etalon_data;

uint32_t send_and_check_cfifo(uint32_t commands, uint32_t device_offset, uint32_t dpnd_device_offset, uint32_t status_offset, uint32_t cfifo_stat_offset, uint32_t status_mask, uint32_t status_val) {
  for (uint32_t i=0; i<commands; i++) {iowrite32(0,NPE_BASE+device_offset);} // dummy command

  if ((ioread32(NPE_BASE+dpnd_device_offset+status_offset)&status_mask)!=status_val) {
    rumboot_printf("ERROR: Device with offset = 0x%x ended works before buffer command stat read!\n",device_offset);
    return -1;
  };

  uint32_t tmp = (ioread32(NPE_BASE+NA_CU_REGS_BASE+cfifo_stat_offset)&0xFFFF);
  if (tmp!=(commands+1)) {
    rumboot_printf("ERROR: Commands number =0x%x. Not qeual refenerce = 0x%x!\n",tmp,commands+1);
    return -1;
  }

  if ((ioread32(NPE_BASE+dpnd_device_offset+status_offset)&status_mask)!=status_val) {
    rumboot_printf("ERROR: Device with offset = 0x%x ended may works before buffer command stat read!\n",device_offset);
    return -1;
  };
  return 0;
}

uint32_t check_buf_stat(uint32_t stat_offset, uint32_t sfifo_stat_reference) {
  uint32_t tmp = ioread32(NPE_BASE+NA_CU_REGS_BASE+stat_offset);
  uint32_t cfifo_stat = tmp&0xFFFF;
  uint32_t sfifo_stat = tmp>>16;
  if (sfifo_stat!=0) {
    rumboot_printf("ERROR: Status of sfifo by addr = 0x%x not equal zero! sfifo stat = 0x%x\n",NPE_BASE+NA_CU_REGS_BASE+stat_offset,sfifo_stat);
    return -1;
  }
  if (cfifo_stat>=sfifo_stat_reference) {
    rumboot_printf("ERROR: Status of cfifo by addr = 0x%x not (smaller or equal) than reference! cfifo stat = 0x%x, reference = 0x%x\n",NPE_BASE+NA_CU_REGS_BASE+stat_offset,cfifo_stat,sfifo_stat_reference);
    return -1;
  }
  return 0;
}

int main()
{
  int heap_id;
  heap_id = nu_get_heap_id();

  metrics_in_data = rumboot_malloc_from_heap_aligned(heap_id,sizeof(metrics_in_data),128);
  rumboot_platform_request_file("metrics_in_arr",(uintptr_t)metrics_in_data);

  in_data = rumboot_malloc_from_heap_aligned(heap_id,metrics_in_data->s,128);
  rumboot_platform_request_file("in_arr", (uintptr_t)in_data);

  uint32_t mpe_in_data_addr = (uint32_t)in_data;
 
  mpe_dma_config(NPE_BASE+NA_MPE_BASE+MPE_RDMA_W_BASE,(uint16_t*)mpe_in_data_addr,(metrics_in_data->s)/128 /* in vectors */, 0);
  mpe_dma_config(NPE_BASE+NA_MPE_BASE+MPE_RDMA_D_BASE,(uint16_t*)mpe_in_data_addr,(metrics_in_data->s)/128 /* in vectors */, 0);

  // MPE_D
  mpe_dma_start(NPE_BASE+NA_MPE_BASE+MPE_RDMA_W_BASE,0);

  iowrite32((1<<25)|1,NPE_BASE+NA_MPE_BASE+MPE_RDMA_D_BASE); // start command with dependency of mpe_dma_w
  while ((ioread32(NPE_BASE+NA_CU_REGS_BASE+NA_MPE_D_CMD_FIFO_STAT)&0xFFFF)!=1) {};
  if (send_and_check_cfifo(3,NA_MPE_BASE+MPE_RDMA_D_BASE,NA_MPE_BASE+MPE_RDMA_W_BASE,DMA_STS,NA_MPE_D_CMD_FIFO_STAT,0x3,0x3)==-1) {return -1;}
  while ((ioread32(NPE_BASE+MPE_RDMA_W_BASE+DMA_STS)&0x3)!=0) {};

  if (check_buf_stat(NA_MPE_D_CMD_FIFO_STAT,3)==-1) {return -11;}
  rumboot_printf("MPE buffers ok!\n");

  // VPE
  mpe_dma_start(NPE_BASE+NA_MPE_BASE+MPE_RDMA_W_BASE,0);
  
  iowrite32((1<<25)|1,NPE_BASE+NA_VPE_BASE); // start command with dependency of mpe_dma_w
  while ((ioread32(NPE_BASE+NA_CU_REGS_BASE+NA_VPE_CMD_FIFO_STAT)&0xFFFF)!=1) {};
  if (send_and_check_cfifo(3,NA_VPE_BASE,NA_MPE_BASE+MPE_RDMA_W_BASE,DMA_STS,NA_VPE_CMD_FIFO_STAT,0x3,0x3)==-1) {return -2;}
  while ((ioread32(NPE_BASE+MPE_RDMA_W_BASE+DMA_STS)&0x3)!=0) {};

  if (check_buf_stat(NA_VPE_CMD_FIFO_STAT,3)==-1) {return -12;}
  rumboot_printf("VPE buffers ok!\n");
  
  // PPE_R
  mpe_dma_start(NPE_BASE+NA_MPE_BASE+MPE_RDMA_W_BASE,0);
  
  iowrite32((1<<25)|1,NPE_BASE+NA_PPE_RDMA_BASE); // start command with dependency of mpe_dma_w
  while ((ioread32(NPE_BASE+NA_CU_REGS_BASE+NA_PPE_R_CMD_FIFO_STAT)&0xFFFF)!=1) {};
  if (send_and_check_cfifo(3,NA_PPE_RDMA_BASE,NA_MPE_BASE+MPE_RDMA_W_BASE,DMA_STS,NA_PPE_R_CMD_FIFO_STAT,0x3,0x3)==-1) {return -3;}
  while ((ioread32(NPE_BASE+MPE_RDMA_W_BASE+DMA_STS)&0x3)!=0) {};

  if (check_buf_stat(NA_PPE_R_CMD_FIFO_STAT,3)==-1) {return -13;}

  // PPE_W
  mpe_dma_start(NPE_BASE+NA_MPE_BASE+MPE_RDMA_W_BASE,0);
  
  iowrite32((1<<25)|1,NPE_BASE+NA_PPE_WDMA_BASE); // start command with dependency of mpe_dma_w
  while ((ioread32(NPE_BASE+NA_CU_REGS_BASE+NA_PPE_W_CMD_FIFO_STAT)&0xFFFF)!=1) {};
  if (send_and_check_cfifo(3,NA_PPE_WDMA_BASE,NA_MPE_BASE+MPE_RDMA_W_BASE,DMA_STS,NA_PPE_W_CMD_FIFO_STAT,0x3,0x3)==-1) {return -4;}
  while ((ioread32(NPE_BASE+MPE_RDMA_W_BASE+DMA_STS)&0x3)!=0) {};

  if (check_buf_stat(NA_PPE_W_CMD_FIFO_STAT,3)==-1) {return -14;}
  rumboot_printf("PPE buffers ok!\n");

  return 0;
}
