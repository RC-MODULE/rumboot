#include <stdint.h> // for uintptr_t, uint32_t

//typedef enum com_ams {ONE_DIMENSION/*, TWO_DIMENSION*/} com_am_t;

#define COM_IRQ_MASK_COMPLETE 0x1
#define COM_IRQ_MASK_ERROR    0x2

#define COM_CONTROL_EN  0x1
#define COM_CONTROL_CPL 0x2

#define COM_COUNT 		  0x40
/*
typedef struct com_cfg {
  uint32_t  RD_Address      ;
  uint32_t  RD_Bias         ;
  uint16_t  RD_RowCounter   ;
 uint16_t 	RD_AddressMode  ;
  uint16_t  RD_MainCounter  ;
  uint8_t   RD_InterruptMask;
  uint32_t  WR_Address      ;
  uint32_t  WR_Bias         ;
  uint16_t  WR_RowCounter   ;
 uint16_t	WR_AddressMode  ;
  uint16_t  WR_MainCounter  ;
  uint8_t   WR_InterruptMask;
} com_cfg_t;
*/
int comp_dma_run( uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1 );
int comp_dma_irq_run( uint32_t src_addr, uint32_t dst_addr,uint32_t base0, uint32_t base1 );  
//int com_simple_run(uintptr_t base_tr, uintptr_t base_rcv,com_cfg_t * cfg);
int com_simple_wait_complete(uintptr_t base, uintptr_t ctrl);
//int com_simple_memcpy(uintptr_t base, com_cfg_t * cfg);
