//------------------------------------------------------------------//
//------------------------------------------------------------------//
// The configuration is created:                                    //
//  - DMA buffers (even and odd) of the FIFO buffer                 //
//    of the transmitter i (the size changes in an internal cycle   //
//    from 1 to 16 words)                                           //
//  - FIFO buffer of the transmitter i and the transmitter i        //
//  - the receiver i                                                //
//  - FIFO of receivers with number i or i+16                       //
//  - DMA buffers of the FIFO buffer of receivers                   //
//    with number i or i+16                                         //
//    (the size same as at the transmitter)                         //
// For everyone i two DMA buffers are processed by turns in a cycle //
//------------------------------------------------------------------//
//------------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/arinc.h>
#include <rumboot/irq.h>
#include <platform/devices.h>
#include <rumboot/macros.h>

//array[256], leveled on border of 16 bytes
int unsigned array[64] __attribute__ ((aligned (64)));
int err = 0;
int ch; //first channel number
int ch1;

int launch_tx_rx (int);
int stus_check (int);

int main()
{
  int freq_0_tx_value = 0xe3008006;
  int nums = 16; //number of cycles for the channel (different sz)
  int nums_ch = 4; //number of channels (different ch)
  int frst_ch = 16; //first of channels
  int szf = 1; //current size of DMA buffers in words value - from 1 to sz_area/4)
  int sz_area = 64; //memory of DMA buffer in bytes
  int tmp;
  int tmp1;
  int sz;
  int cmp = 1;
  int succ = 1;
  int index;
  int cycles;
  int channs;
  int unsigned p_index;

//-------------------------------------
  for (index= 0; index<32; ++index)
	  array[index] = index;
//-------------------------------------
  asm("DMB");
//=====================================
p_index = (int unsigned)array;
//=====================================
	ch = frst_ch;
//=====================================
  for (channs= ch; channs< nums_ch+ frst_ch; ++channs)
{
//=====================================
  for (index= 32; index<64; ++index)
	  array[index] = index;
//-------------------------------------
  asm("DMB");
//-------------------------------------
	ch1 = ch > 31 ? 0 : ch;
	ch = ch > 31 ? 0 : ch > 15 ? ch - 16 : ch;
	sz = szf;
//-------------------------------------
iowrite32(p_index + sz_area*0  ,ARINC_BASE + AG_E_TX + 4*ch); 
//-------------------------------------
iowrite32(p_index + sz_area*1  ,ARINC_BASE + AG_O_TX + 4*ch); 
//-------------------------------------
iowrite32(p_index + sz_area*2  ,ARINC_BASE + AG_E_RX + 4*ch1); 
//-------------------------------------
iowrite32(p_index + sz_area*3  ,ARINC_BASE + AG_O_RX + 4*ch1); 
//-------------------------------------
iowrite32(0x00000001<< ch,ARINC_BASE + LAST_WD_TMR); 
//-----------------------------------------------
tmp  = ioread32(ARINC_BASE + FREQ_TX + 4*ch);
tmp1 = ioread32(ARINC_BASE + FREQ_RX + 4*ch1);
//-----------------------------------------------
	rumboot_printf("Hello world from ARINC_429: channel - 0x%x\n",ch1);
	rumboot_printf("freq_tx_reg=0x%x\n", tmp);
	rumboot_printf("freq_rx_reg=0x%x\n", tmp1);
//	rumboot_printf("array[1]=0x%x\n", array[1]);
//	rumboot_printf("array[2]=0x%x\n", array[2]);
	rumboot_printf("p_index=0x%x\n", p_index);
//-----------------------------------------------
  for (cycles= 0; cycles< nums; ++cycles)
{
//-----------------------------------------------
iowrite32(sz  ,ARINC_BASE + SZ_E_TX + 4*ch); 
//-------------------------------------
iowrite32(sz  ,ARINC_BASE + SZ_O_TX + 4*ch); 
//-------------------------------------
iowrite32(sz  ,ARINC_BASE + SZ_E_RX + 4*ch1); 
//-------------------------------------
iowrite32(sz  ,ARINC_BASE + SZ_O_RX + 4*ch1); 
//-------------------------------------
//-----------------------------------------------
if (launch_tx_rx (freq_0_tx_value))
{
	rumboot_printf("---------------------- \n");
	rumboot_printf("Error in a serial line \n");
	rumboot_printf("---------------------- \n");
}
//-----------------------------------------------
//-----------------------------------------------
  for (index= 0; index<sz; ++index)
	  cmp= cmp & (array[index + 0 * sz_area/4] == array[index + 2 * sz_area/4]);
  for (index= 0; index<sz; ++index)
	  cmp= cmp & (array[index + 1 * sz_area/4] == array[index + 3 * sz_area/4]);
//-----------------------------------------------
//	rumboot_printf("array[32]=0x%x\n", array[32]);
//	rumboot_printf("array[33]=0x%x\n", array[33]);
//	rumboot_printf("array[34]=0x%x\n", array[34]);
//	rumboot_printf("array[35]=0x%x\n", array[35]);
//	rumboot_printf("array[36]=0x%x\n", array[36]);
//	rumboot_printf("array[37]=0x%x\n", array[37]);
//	rumboot_printf("array[38]=0x%x\n", array[38]);
//	rumboot_printf("array[39]=0x%x\n", array[39]);
//	rumboot_printf("array[40]=0x%x\n", array[40]);
//	rumboot_printf("array[41]=0x%x\n", array[41]);
//	rumboot_printf("array[42]=0x%x\n", array[42]);
//	rumboot_printf("array[43]=0x%x\n", array[43]);
//	rumboot_printf("array[44]=0x%x\n", array[44]);
//	rumboot_printf("array[45]=0x%x\n", array[45]);
//	rumboot_printf("array[46]=0x%x\n", array[46]);
//	rumboot_printf("array[47]=0x%x\n", array[47]);
//	rumboot_printf("array[48]=0x%x\n", array[48]);
//	rumboot_printf("array[49]=0x%x\n", array[49]);
//	rumboot_printf("array[50]=0x%x\n", array[50]);
//	rumboot_printf("array[51]=0x%x\n", array[51]);
//	rumboot_printf("array[52]=0x%x\n", array[52]);
//	rumboot_printf("array[53]=0x%x\n", array[53]);
//	rumboot_printf("array[54]=0x%x\n", array[54]);
//	rumboot_printf("array[55]=0x%x\n", array[55]);
//	rumboot_printf("array[56]=0x%x\n", array[56]);
//	rumboot_printf("array[57]=0x%x\n", array[57]);
//	rumboot_printf("array[58]=0x%x\n", array[58]);
//	rumboot_printf("array[59]=0x%x\n", array[59]);
//	rumboot_printf("array[60]=0x%x\n", array[60]);
//	rumboot_printf("array[61]=0x%x\n", array[61]);
//	rumboot_printf("array[62]=0x%x\n", array[62]);
//	rumboot_printf("array[63]=0x%x\n", array[63]);
//	rumboot_printf("cmp=0x%x\n", cmp);

	sz = sz + 1;
}
	ch = ch1 + 1;
}

if (cmp==1) rumboot_printf("Test passed: \n");
if (cmp==1) succ= 0;
if (cmp==0) rumboot_printf("Test failed: \n");

	return succ;
}

//----------------------------------------------------
//-------------------launch_tx_rx---------------------
//----------------------------------------------------
int launch_tx_rx (int freq_0_tx_value)
{
//  int stus_27tx;
//  int ltmp;
//  int ltmp1;
  int freq_0_rx_value = 0xfc0001f2;
  int mask = 0x00001fff;
//----------------------------------------------------
  freq_0_rx_value = (freq_0_rx_value & ~mask) +
	            (freq_0_tx_value &  mask);
//----------------------------------------------------
iowrite32(freq_0_rx_value,ARINC_BASE + FREQ_RX + 4*ch); 

iowrite32(0x00000010+ ch,ARINC_BASE + RNUM_RX + 4*ch1); 

//iowrite32(0x00000010,ARINC_BASE + SELF_RX + 4*ch1); 
//----------------------------------------------------
iowrite32(freq_0_tx_value,ARINC_BASE + FREQ_TX + 4*ch); 
//----------------------------------------------------
iowrite32(0x00010001<< ch,ARINC_BASE + CHANNEL_EN); 
//----------------------------------------------------
while (stus_check(ioread32(ARINC_BASE + STAT_E_TX + 4*ch)))
{
}
//----------------------------------------------------
iowrite32(0x00010000<< ch,ARINC_BASE + CHANNEL_DIS); 
//----------------------------------------------------
while (stus_check(ioread32(ARINC_BASE + STAT_E_RX + 4*ch1)))
{
}
//----------------------------------------------------
iowrite32(0x00000001<< ch,ARINC_BASE + CHANNEL_DIS); 
//----------------------------------------------------
while ((ioread32(ARINC_BASE + CHANNEL_EN)) != 0x00000000)
{}
//----------------------------------------------------
//ltmp  = ioread32(ARINC_BASE + FREQ_TX + 4*ch);
//ltmp1 = ioread32(ARINC_BASE + FREQ_RX + 4*ch);

//	rumboot_printf("freq_tx_fun=0x%x\n", ltmp);
//	rumboot_printf("freq_rx_fun=0x%x\n", ltmp1);
	 
	return err;
}
//----------------------------------------------------

//----------------------------------------------------
//-------------------stus_check-----------------------
//----------------------------------------------------
int stus_check (int stus_value)
{
  int out;

out = stus_value;

if ((out >> 27 & 1)==0)

{
//----------------------------------------------------
if (((out >> 31 & 1)==1) & ((out >> 30 & 1)==0))
	;
//	rumboot_printf("stus1_value=0x%x\n", out);
//----------------------------------------------------
	return ((((stus_value << 6 ) != 0) &
     ((out >> 31 & 1)==1) &
     ((out >> 26 & 1)==0) &
    (((out >> 30 & 1)==0) |
     ((out >> 28 & 1)==1))) ==0); 
}
else
{
        err = 1;
	rumboot_printf("stus_value=0x%x\n", out);
	return 0;
}
}
//----------------------------------------------------

