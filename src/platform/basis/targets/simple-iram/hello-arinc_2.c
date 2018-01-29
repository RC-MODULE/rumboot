//------------------------------------------------------------------//
//------------------------------------------------------------------//
// All transmitters and receivers join at the same time             //
// The size (sz) of DMA buffers is the same                         //
// Two buffers ("even" and "odd") are processed in each cycle       //
// In each cycle the size increases on 1 (++sz)                     //
// The maximal size of each of DMA buffers is 16 words              //
// In the end of each cycle the changed contents of DMA buffers     //
//  of receivers are checked                                        //
// External loopback is used                                        //
// Management is carried out on a trap signal (INT_ARINC)           // 
//------------------------------------------------------------------//
//------------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/arinc.h>
#include <rumboot/irq.h>
#include <platform/devices.h>
#include <rumboot/macros.h>
#include <platform/interrupts.h>

//array[256], leveled on border of 64 bytes
int unsigned array[2048] __attribute__ ((aligned (64)));
volatile int irq_done=0;
//-----------------------------------------
static void handler0(int irq, void *arg)
{
iowrite32(0,ARINC_BASE + INT_TX + 4*5);
irq_done=1;
//  rumboot_printf("Hello from trap handler: irq_done=0x%x\n",irq_done);
}
//-----------------------------------------
int multi8_store (int,int,int);

int main()
{
  int freq_tx_value = 0xe3008005;
  int freq_rx_value = 0xfc000005;
  int sz_area = 64; //memory of DMA buffer in bytes
  int sz=1; //the initial size of the buffer
  int nums_cyc=16; //total of cycles
  int index;
  int cycles;
  int cmp = 1;
  int succ = 1;
  int check=16;
  int i;
  int unsigned p_index;
//=====================================
  
	/* Disable all interrupts */
	rumboot_irq_cli();
	
	/* Create an IRQ table */
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

    rumboot_irq_set_handler(tbl, ARINC_INT, 0, handler0, 0);

	/* Configure handler */
	/* Activate the table */
	rumboot_irq_table_activate(tbl);

	/* Enable IRQ */
	rumboot_irq_enable(ARINC_INT);
	
	/* Allow interrupt handling */
	rumboot_irq_sei();

//=====================================
p_index = (int unsigned)array;

  for (i= 0; i<1024; i= i + 8)
multi8_store(4*i,p_index + 4*i ,4);
//  for (index= 0; index<1024; ++index)
//	  array[index] = index;
  for (index= 1056; index<2032; index= index + 4)
	  array[index] = index;
  for (index= 1072; index<2048; index= index + 4)
	  array[index] = index;
//-------------------------------------
  asm("DMB");
//=====================================
//-------------------------------------
//----------------------------------------
multi8_store(freq_tx_value,ARINC_BASE + FREQ_TX       ,0);
multi8_store(freq_tx_value,ARINC_BASE + FREQ_TX + 4*8 ,0);

multi8_store(freq_rx_value,ARINC_BASE + FREQ_RX,       0);
multi8_store(freq_rx_value,ARINC_BASE + FREQ_RX + 4*8 ,0);
//----------------------------------------
//////  for (i= 0; i<16; ++i)
//////iowrite32(p_index +             sz_area*4*0  ,ARINC_BASE + AG_E_TX + 4*i); 
//////  for (i= 0; i<16; ++i)
//////iowrite32(p_index + sz_area   + sz_area*4*0  ,ARINC_BASE + AG_O_TX + 4*i); 
//////  for (i= 0; i<32; ++i)
//////iowrite32(p_index + sz_area*2 + sz_area*4*0  ,ARINC_BASE + AG_E_RX + 4*i); 
//////  for (i= 0; i<32; ++i)
//////iowrite32(p_index + sz_area*3 + sz_area*4*0  ,ARINC_BASE + AG_O_RX + 4*i); 
//----------------------------------------
multi8_store(p_index             ,ARINC_BASE + AG_E_TX       ,sz_area*4);
multi8_store(p_index + sz_area*32,ARINC_BASE + AG_E_TX + 4*8 ,sz_area*4);
multi8_store(p_index + sz_area*1 ,ARINC_BASE + AG_O_TX       ,sz_area*4);
multi8_store(p_index + sz_area*33,ARINC_BASE + AG_O_TX + 4*8 ,sz_area*4);
multi8_store(p_index + sz_area*2 ,ARINC_BASE + AG_E_RX,       sz_area*4);
multi8_store(p_index + sz_area*34,ARINC_BASE + AG_E_RX + 4*8 ,sz_area*4);
multi8_store(p_index + sz_area*66,ARINC_BASE + AG_E_RX + 4*16,sz_area*4);
multi8_store(p_index + sz_area*98,ARINC_BASE + AG_E_RX + 4*24,sz_area*4);
multi8_store(p_index + sz_area*3 ,ARINC_BASE + AG_O_RX,       sz_area*4);
multi8_store(p_index + sz_area*35,ARINC_BASE + AG_O_RX + 4*8 ,sz_area*4);
multi8_store(p_index + sz_area*67,ARINC_BASE + AG_O_RX + 4*16,sz_area*4);
multi8_store(p_index + sz_area*99,ARINC_BASE + AG_O_RX + 4*24,sz_area*4);
//----------------------------------------
multi8_store(0x00000010,ARINC_BASE + RNUM_RX,       1);
multi8_store(0x00000018,ARINC_BASE + RNUM_RX + 4*8 ,1);
multi8_store(0x00000010,ARINC_BASE + RNUM_RX + 4*16,1);
multi8_store(0x00000018,ARINC_BASE + RNUM_RX + 4*24,1);
//-----------------------------------------------
iowrite32(0x0000ffff,ARINC_BASE + LAST_WD_TMR); 
//-----------------------------------------------
iowrite32(0x00000001,ARINC_BASE + INT_TX + 4*9); 
//-----------------------------------------------
//-----------------------------------------------
  for (cycles= 0; cycles<nums_cyc; ++cycles)
{
//-----------------------------------------------
	sz = sz > 16 ? 1 : sz;
//-----------------------------------------------
multi8_store(sz,ARINC_BASE + SZ_E_TX       ,0);
multi8_store(sz,ARINC_BASE + SZ_E_TX + 4*8 ,0);
multi8_store(sz,ARINC_BASE + SZ_O_TX       ,0);
multi8_store(sz,ARINC_BASE + SZ_O_TX + 4*8 ,0);
multi8_store(sz,ARINC_BASE + SZ_E_RX,       0);
multi8_store(sz,ARINC_BASE + SZ_E_RX + 4*8 ,0);
multi8_store(sz,ARINC_BASE + SZ_E_RX + 4*16,0);
multi8_store(sz,ARINC_BASE + SZ_E_RX + 4*24,0);
multi8_store(sz,ARINC_BASE + SZ_O_RX,       0);
multi8_store(sz,ARINC_BASE + SZ_O_RX + 4*8 ,0);
multi8_store(sz,ARINC_BASE + SZ_O_RX + 4*16,0);
multi8_store(sz,ARINC_BASE + SZ_O_RX + 4*24,0);
//-----------------------------------------------
multi8_store(0x00000010,ARINC_BASE + RNUM_RX,       1);
multi8_store(0x00000018,ARINC_BASE + RNUM_RX + 4*8 ,1);
multi8_store(0x00000010,ARINC_BASE + RNUM_RX + 4*16,1);
multi8_store(0x00000018,ARINC_BASE + RNUM_RX + 4*24,1);
//-----------------------------------------------
iowrite32(0x0000ffff,ARINC_BASE + INT_LONG);
//-----------------------------------------------
iowrite32(0x0000ffff,ARINC_BASE + WAIT_TMR_TX); 
//-----------------------------------------------
iowrite32(0xffffffff,ARINC_BASE + CHANNEL_EN); 
//----------------------------------------------------
iowrite32(0x0000ffff,ARINC_BASE + INT_TX); 
//----------------------------------------------------
while (irq_done==0) //waiting for a trap
{
}
//----------------------------------------------------
while (ioread32(ARINC_BASE + COMPL_TX)!=0xffff0000) //COMPL_TX(ev)
{ }
//----------------------------------------------------
while (ioread32(ARINC_BASE + COMPL_E_RX)!=0xffffffff)
{ }
//-----------------------------------------------
iowrite32(0x0000ffff,ARINC_BASE + SW_SIG_TX); 
//-----------------------------------------------
iowrite32(0x0000ffff,ARINC_BASE + CHANNEL_DIS); 
//----------------------------------------------------
while (ioread32(ARINC_BASE + COMPL_TX)!=0xffffffff) //COMPL_TX(ev & od)
{ }
//----------------------------------------------------
while (ioread32(ARINC_BASE +COMPL_O_RX)!=0xffffffff)
{ }
//-----------------------------------------------
iowrite32(0xffff0000,ARINC_BASE + CHANNEL_DIS); 
//----------------------------------------------------
iowrite32(0x0000ffff,ARINC_BASE + SW_SIG_TX); 
//-----------------------------------------------
while ((ioread32(ARINC_BASE + CHANNEL_EN)) != 0x00000000) //waiting for EN==0
{
	 irq_done=0;
}
//----------------------------------------------------
  for (i= 0; i<check; ++i)
{
  for (index= 0; index<sz; ++index)
	  cmp= cmp & (array[0 + i*64 + index] == array[32 + i*64 + index]);
  for (index= 0; index<sz; ++index)
	  cmp= cmp & (array[16 + i*64 + index] == array[48 + i*64 + index]);
  }
//----------------------------------------------------
  for (i= 0; i<check; ++i)
{
  for (index= 0; index<sz; ++index)
	  cmp= cmp & (array[0 + i*64 + index] == array[1056 + i*64 + index]);
  for (index= 0; index<sz; ++index)
	  cmp= cmp & (array[16 + i*64 + index] == array[1072 + i*64 + index]);
  }
//----------------------------------------------------
	rumboot_printf("Hello world from ARINC_429: sz - 0x%x\n",sz);
//----------------------------------------------------
  for (i= 0; i<check; ++i)
{
  for (index= 0; index<sz; ++index)
	  array[32 + i*64 + index] = 32 + i*64 + index;
  for (index= 0; index<sz; ++index)
	  array[48 + i*64 + index] = 48 + i*64 + index;
  }
//----------------------------------------------------
  for (i= 0; i<check; ++i)
{
  for (index= 0; index<sz; ++index)
	  array[1056 + i*64 + index] = 1056 + i*64 + index;
  for (index= 0; index<sz; ++index)
	  array[1072 + i*64 + index] = 1072 + i*64 + index;
  }
//----------------------------------------------------
	sz = sz + 1;
	irq_done = 0;
//----------------------------------------------------
} //cycles

if (cmp==1) rumboot_printf("Test passed: \n");
if (cmp==1) succ= 0;
if (cmp==0) rumboot_printf("Test failed: \n");

	return succ;
}
//----------------------------------------------------
//----------------------------------------------------
//-------------------multi8_store---------------------
//----------------------------------------------------
int multi8_store (int p_arr,int reg_num,int delta)
{
asm(
"ADD r12, %2, #0 \n" //"ADD r0, delta, #0 \n"
"ADD r0, %0, #0 \n" //"ADD r0, p_arr, #0 \n"
"ADD r1, %0, r12 \n"
"ADD r2, %0, r12, LSL #1 \n"
"ADD r3, r2, r12 \n"
"ADD r4, %0, r12, LSL #2 \n"
"ADD r5, r4, r12 \n"
"ADD r6, r4, r12, LSL #1 \n"
"ADD r8, %0, r12, LSL #3 \n"
"SUB r7, r8, r12 \n"
"ADD r12, %1, #0 \n" //"ADD r0, reg_num, #0 \n"
"STM r12!, {r0-r7} \n"
:
:"r" (p_arr),"r" (reg_num),"r" (delta)
:"r0","r1","r2","r3","r4","r5","r6","r7","r8","r12"
);
	return 0;
}
//----------------------------------------------------
