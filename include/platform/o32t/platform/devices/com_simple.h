#include <stdint.h> // for uintptr_t, uint32_t

#define COM_IRQ_MASK_COMPLETE 0x1
#define COM_IRQ_MASK_ERROR    0x2

#define COM_CONTROL_EN  0x1
#define COM_CONTROL_CPL 0x2

//#define COM_DO_DELAY	0x1
#define COM_COUNT 		  0x40

int comp_dma_run_tr(uint32_t addr, uint32_t base, uint32_t mask,uint32_t count_num);
int comp_dma_run_rcv(uint32_t addr, uint32_t base, uint32_t mask,uint32_t count_num);
int comp_dma_run( uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1,uint32_t count_num);
int comp_dma_irq_run( uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1,uint32_t COM0_Cpl,uint32_t COM1_Cpl,uint32_t count_num); 
int comp_dma_run_xrdy(uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1,uint32_t count_num);
int comp_dma_run_thru(uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1,uint32_t count_num);
int com_simple_wait_complete(uintptr_t base, uintptr_t ctrl);
int com_status(uint32_t base, uint32_t direct );