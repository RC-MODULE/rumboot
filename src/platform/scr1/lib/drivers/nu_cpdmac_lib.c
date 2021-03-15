


#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 
#include <regs/regs_nu_cpdmac.h> 



void nu_cpdmac_trn512_config       (uintptr_t base, void *addr, int size)
{
  uint64_t* ptr;
  
  ptr = (uint64_t*) addr;
  
  for (int i=0; i<8; i++) {
    iowrite32(size/64,       base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_MainCounter_tr);// one element from 64byte line
    iowrite32((uint32_t)ptr, base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_Address_tr);
    iowrite32(64,            base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_Bias_tr);   // Go to the next 512bit (64byte) line each time
    iowrite32(1,             base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_RowCounter_tr); //Single Word In A Column
    iowrite32(1,             base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_AddressMode_tr);// 2-Dimensional Mode
    ptr++;
  }
  
}
void nu_cpdmac_trn512_run          (uintptr_t base)
{
  for (int i=0; i<8; i++) {
    iowrite32(1,base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_CSR_tr);
  }
}
void  nu_cpdmac_trn512_wait_complete(uintptr_t base)
{
  for (int i=0; i<8; i++) {
    while( (ioread32(base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_CSR_tr) & 2) == 0)
    {}
    iowrite32(0,base + NU_CPDMAC_REGBLOCK_OFFSET(i) +NU_CPDMAC_CSR_tr);
  }
  
}

void nu_cpdmac_rcv512_config       (uintptr_t base, void *addr, int size)
{
  uint64_t* ptr;
  
  ptr = (uint64_t*) addr;
  
  for (int i=0; i<8; i++) {
    iowrite32(size/64,       base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_MainCounter_rcv);// one element from 64byte line
    iowrite32((uint32_t)ptr, base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_Address_rcv);
    iowrite32(64,            base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_Bias_rcv);   // Go to the next 512bit (64byte) line each time
    iowrite32(1,             base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_RowCounter_rcv); //Single Word In A Column
    iowrite32(1,             base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_AddressMode_rcv);// 2-Dimensional Mode
    ptr++;
  }
  
}

void nu_cpdmac_rcv512_run          (uintptr_t base)
{
  for (int i=0; i<8; i++) {
    iowrite32(1,base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_CSR_rcv);
  }
}
void nu_cpdmac_rcv512_wait_complete(uintptr_t base)
{
  for (int i=0; i<8; i++) {
    while( (ioread32(base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_CSR_rcv) & 2) == 0)
    {}
    iowrite32(0,base + NU_CPDMAC_REGBLOCK_OFFSET(i) +NU_CPDMAC_CSR_rcv);
  }
  
}

void nu_cpdmac_trn256_config       (uintptr_t base, void *addr, int size)
{
  uint64_t* ptr;
  
  ptr = (uint64_t*) addr;
  
  for (int i=0; i<4; i++) {
    iowrite32(size/32,       base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_MainCounter_tr);
    iowrite32((uint32_t)ptr, base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_Address_tr);
    iowrite32(32,            base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_Bias_tr);   
    iowrite32(1,             base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_RowCounter_tr);
    iowrite32(1,             base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_AddressMode_tr);
    ptr++;
  }
  
}
  

void nu_cpdmac_trn256_run          (uintptr_t base)
{
  for (int i=0; i<4; i++) {
    iowrite32(1,base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_CSR_tr);
  }
}
void  nu_cpdmac_trn256_wait_complete(uintptr_t base)
{
  for (int i=0; i<4; i++) {
    while( (ioread32(base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_CSR_tr) & 2) == 0)
    {}
    iowrite32(0,base + NU_CPDMAC_REGBLOCK_OFFSET(i) +NU_CPDMAC_CSR_tr);
  }
  
}

void nu_cpdmac_rcv256_config       (uintptr_t base, void *addr, int size)
{
  uint64_t* ptr;
  
  ptr = (uint64_t*) addr;
  
  for (int i=0; i<4; i++) {
    iowrite32(size/32,       base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_MainCounter_rcv);
    iowrite32((uint32_t)ptr, base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_Address_rcv);
    iowrite32(32,            base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_Bias_rcv);   
    iowrite32(1,             base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_RowCounter_rcv);
    iowrite32(1,             base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_AddressMode_rcv);
    ptr++;
  }
  
}
void nu_cpdmac_rcv256_run          (uintptr_t base)
{
  for (int i=0; i<4; i++) {
    iowrite32(1,base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_CSR_rcv);
  }
}

void  nu_cpdmac_rcv256_wait_complete(uintptr_t base)
{
  for (int i=0; i<4; i++) {
    while( (ioread32(base + NU_CPDMAC_REGBLOCK_OFFSET(i) + NU_CPDMAC_CSR_rcv) & 2) == 0)
    {}
    iowrite32(0,base + NU_CPDMAC_REGBLOCK_OFFSET(i) +NU_CPDMAC_CSR_rcv);
  }
  
}


