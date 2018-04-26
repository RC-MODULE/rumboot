//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 registers.
// 
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/arinc.h>
#include <rumboot/irq.h>
#include <platform/devices.h>

int arinc_rst (uint32_t arinc_base_addr){
	uint32_t tmp_en ;
	uint32_t tmp;
	uint32_t tmp_e;
	uint32_t tmp_en_p;	
	tmp_en = 0x0;
	tmp_en_p=0xffffffff;
    int i;
	rumboot_printf ("TEST ARINC429 REG \n");
	rumboot_printf (" CHECK REG RST ARINC \n");
	tmp = ioread32(ARINC_BASE + CHANNEL_EN );
	//rumboot_printf("CHANNEL_EN=0x%x\n", tmp);
	if (tmp_en !=tmp ) {
	rumboot_printf("CHANNEL_EN=0x%x\n", tmp);
	return ARINC_FAILED;}
	tmp = ioread32(ARINC_BASE + CHANNEL_DIS );
	//rumboot_printf("CHANNEL_DIS=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("CHANNEL_DIS=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp = ioread32(ARINC_BASE + PARITY_BIT );
	//rumboot_printf("PARITY_BIT=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("PARITY_BIT=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp = ioread32(ARINC_BASE + PARITY_ODD );
	//rumboot_printf("PARITY_ODD=0x%x\n", tmp);
	if (tmp_en_p !=tmp ){
	rumboot_printf("PARITY_ODD=0x%x\n", tmp);
	return ARINC_FAILED; }

	tmp_e = ioread32(ARINC_BASE + TEST_EN );
	tmp = 0x0000001F & tmp_e;
	//rumboot_printf("TEST_EN=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TEST_EN=0x%x\n", tmp);
	return ARINC_FAILED;}
//------------------------------------------------------


	tmp = ioread32(ARINC_BASE + WAIT_SIG_RX );
	//rumboot_printf("WAIT_SIG_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("WAIT_SIG_RX=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp = ioread32(ARINC_BASE + TMR_MASK_RX );
	//rumboot_printf("TMR_MASK_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TMR_MASK_RX=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp = ioread32(ARINC_BASE + WAIT_TMR_TX );
	//rumboot_printf("WAIT_TMR_TXN=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("WAIT_TMR_TXN=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp = ioread32(ARINC_BASE + LABEL_EN_RX );
	if (tmp_en !=tmp ){
	rumboot_printf("LABEL_EN_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	
    tmp_en_p = 0x0c060;
	tmp_e = ioread32(ARINC_BASE + AXI_CTRL );
	tmp = 0x3ffff & tmp_e;
	if (tmp_en_p !=tmp ){
	rumboot_printf("AXI_CTRL=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp = ioread32(ARINC_BASE + FIFO_EN_RX );
	if (tmp_en !=tmp ){
	rumboot_printf("FIFO_EN_RX=0x%x\n", tmp);
	return ARINC_FAILED;}

	for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + STAT_E_TX  +4*i);
	//rumboot_printf("STAT_E_TX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("STAT_E_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

    for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + STAT_O_TX  +4*i);
	//rumboot_printf("STAT_O_TX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("STAT_O_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

	for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + TRF_E_TX  +4*i);
	//rumboot_printf("TRF_E_TX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TRF_E_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
    }
    for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + TLF_E_TX  +4*i);
	//rumboot_printf("TLF_E_TX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TLF_E_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

	for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + TRL_E_TX  +4*i);
	//rumboot_printf("TRL_E_TX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TRL_E_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}
    for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + TRF_O_TX  +4*i);
	//rumboot_printf("TRF_O_TX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TRF_O_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

	for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + TLF_O_TX  +4*i);
	//rumboot_printf("TLF_O_TX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TLF_O_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

	for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + TRL_O_TX  +4*i);
	//rumboot_printf("TRL_O_TX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TRL_O_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

    for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + TLF_E_TX  +4*i);
	//rumboot_printf("TLF_E_TX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TLF_E_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

	for (i = 0; i< 32; i++)
	{
    tmp = ioread32(ARINC_BASE + STAT_E_RX  +4*i);
	//rumboot_printf("STAT_E_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("STAT_E_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}
    for (i = 0; i< 32; i++)
	{
    tmp = ioread32(ARINC_BASE + STAT_O_RX  +4*i);
	//rumboot_printf("STAT_O_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("STAT_O_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

	for (i = 0; i< 32; i++)
	{
    tmp = ioread32(ARINC_BASE + TRF_E_RX  +4*i);
	//rumboot_printf("TRF_E_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TRF_E_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}
    for (i = 0; i< 32; i++)
	{
    tmp = ioread32(ARINC_BASE + TLF_E_RX  +4*i);
	//rumboot_printf("TLF_E_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TLF_E_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}
	for (i = 0; i< 32; i++)
    {
	tmp = ioread32(ARINC_BASE + TRL_E_RX  +4*i);
	//rumboot_printf("TRL_E_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TRL_E_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}
    for (i = 0; i< 32; i++)
	{
    tmp = ioread32(ARINC_BASE + TRF_O_RX  +4*i);
	//rumboot_printf("TRF_O_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TRF_O_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

    for (i = 0; i< 32; i++)
	{
    tmp = ioread32(ARINC_BASE + TLF_O_RX  +4*i);
	//rumboot_printf("TLF_O_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TLF_O_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}
	for (i = 0; i< 32; i++)
	{
    tmp = ioread32(ARINC_BASE + TRL_O_RX  +4*i);
	//rumboot_printf("TRL_O_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("TRL_O_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

    for (i = 0; i< 32; i++)
	{
    tmp_e = ioread32(ARINC_BASE + RNUM_RX  +4*i);
    tmp = 0xff & tmp_e;
	//rumboot_printf("RNUM_RX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("RNUM_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

    tmp_en = 0x30081F2;
    for (i = 0; i< 16; i++)
	{
    tmp_e = ioread32(ARINC_BASE + FREQ_TX  +4*i);
	tmp = 0x3FFFFFF & tmp_e;	//{tmp_e[31:29]==0,3'b0,tmp_e[25:0]
	if (tmp_en !=tmp ){
	rumboot_printf("FREQ_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

    tmp_en = 0xb01f01f2;
	for (i = 0; i< 16; i++)
	{
    tmp = ioread32(ARINC_BASE + FREQ_RX  +4*i);
	//rumboot_printf("FREQ_RX=0x%x\n", tmp);
	if (tmp_en !=tmp )
	{rumboot_printf("FREQ_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}
	tmp_en = 0x0;
	for (i = 0; i< 16; i++)
	{
    tmp_e = ioread32(ARINC_BASE + SELF_RX  +4*i);
	tmp = 0x1F & tmp_e;
	//rumboot_printf("SELF_RX=0x%x\n", tmp);
	if (tmp_en !=tmp )
	{rumboot_printf("SELF_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}
	return ARINC_OK;

}




//----------------------------------------------------------

int arinc_wr_rd (uint32_t arinc_base_addr){
	uint32_t tmp_en ;
	uint32_t tmp_en_freq;
	uint32_t tmp;
	uint32_t tmp_e;
	int i;
	rumboot_printf (" CHECK REG WR-RD ARINC \n");
	//tmp_en = 0xA5A5A5A5;
	//iowrite32(tmp_en, arinc_base_addr + CHANNEL_DIS );
	//rumboot_printf("ARINC_CHANNEL_EN=0x%x\n",tmp_en);
	//tmp_en = 0x1A2B3C4D;
	//iowrite32(tmp_en, arinc_base_addr + CHANNEL_RST );
	//rumboot_printf("CHANNEL_RST=0x%x\n",tmp_en);
	tmp_en = 0xF5324C5D;
	iowrite32(tmp_en, arinc_base_addr + PARITY_BIT );
	//rumboot_printf("PARITY_BIT=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + PARITY_BIT );
	if (tmp_en !=tmp )
	{rumboot_printf("PARITY_BIT=0x%x\n", tmp);
	return ARINC_FAILED;}
	tmp_en = 0x0ACDB3A2;
	iowrite32(tmp_en, arinc_base_addr + PARITY_ODD );
	//rumboot_printf("PARITY_ODD=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + PARITY_ODD );
	if (tmp_en !=tmp )
	{rumboot_printf("ARINC_PARITY_ODD=0x%x\n", tmp);
	return ARINC_FAILED;}
	tmp_en = 0xF;
	iowrite32(tmp_en, arinc_base_addr + TEST_EN );
	tmp_e = ioread32(ARINC_BASE + TEST_EN);
	tmp =  0x1f & tmp_e;  //{27'b0,tmp_e[4:0]};
	//rumboot_printf("ARINC_TEST_EN=0x%x\n", tmp);
	if (tmp_en !=tmp )
	{rumboot_printf("ARINC_TEST_EN=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp_en = 0x1E2B3C4D;
	iowrite32(tmp_en, arinc_base_addr + WAIT_SIG_RX );
	//rumboot_printf("WAIT_SIG_RX=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + WAIT_SIG_RX );
	if (tmp_en !=tmp )
	{rumboot_printf("WAIT_SIG_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	tmp_en = 0xA3A4A5A6;
	iowrite32(tmp_en, arinc_base_addr + TMR_MASK_RX );
	//rumboot_printf("TMR_MASK_RX=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + TMR_MASK_RX );
	if (tmp_en !=tmp )
	{rumboot_printf("TMR_MASK_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	//tmp_en = 0xA5A5A5A5;
	//iowrite32(tmp_en, arinc_base_addr + SW_SIG_RX );
	//rumboot_printf("SW_SIG_RX=0x%x\n",tmp_en);  //!!!!!!
	tmp_en = 0x1A2B3C4D;
	iowrite32(tmp_en, arinc_base_addr + WAIT_TMR_TX );
	//rumboot_printf("WAIT_TMR_TX=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + WAIT_TMR_TX );
	if (tmp_en !=tmp )
	{rumboot_printf("WAIT_TMR_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	//tmp_en = 0xA5A5A5A5;
	//iowrite32(tmp_en, arinc_base_addr + SW_SIG_TX );
	//rumboot_printf("SW_SIG_TX=0x%x\n",tmp_en);  //!!!!!!
	tmp_en = 0x1A2B3C4D;
	iowrite32(tmp_en, arinc_base_addr + LABEL_EN_RX );
	//rumboot_printf("LABEL_EN_RX=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + LABEL_EN_RX );
	if (tmp_en !=tmp )
	{rumboot_printf("LABEL_EN_RX=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp_en = 0x33C4D;
	iowrite32(tmp_en, arinc_base_addr + AXI_CTRL );
	//rumboot_printf("AXI_CTRL=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + AXI_CTRL );
	//rumboot_printf("AXI_CTRL=0x%x\n", tmp);
	if (tmp_en !=tmp )
	{rumboot_printf("AXI_CTRL=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp_en = 0xFC4D;
	iowrite32(tmp_en, arinc_base_addr + LAST_WD_TMR );
	//rumboot_printf("LAST_WD_TMR=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + LAST_WD_TMR );
	tmp = 0xffff & tmp;
	//rumboot_printf("LAST_WD_TMR=0x%x\n", tmp);
	if (tmp_en !=tmp )
	{rumboot_printf("LAST_WD_TMR=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp_en = 0xC3B2;
	iowrite32(tmp_en, arinc_base_addr + INT_TX );
	//rumboot_printf("INT_TX=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + INT_TX );

	//rumboot_printf("INT_TX=0x%x\n", tmp);
	if (tmp_en !=tmp )
	{rumboot_printf("INT_TX=0x%x\n", tmp);
	return ARINC_FAILED;}

	tmp_en = 0x1A2B3C4D;
	iowrite32(tmp_en, arinc_base_addr + INT_RX );
	//rumboot_printf("INT_RX=0x%x\n",tmp_en);
	tmp = ioread32(ARINC_BASE + INT_RX );
	//rumboot_printf("INT_RX=0x%x\n", tmp);
	if (tmp_en !=tmp )
	{rumboot_printf("INT_RX=0x%x\n", tmp);
	return ARINC_FAILED;}



    tmp_en = 0x1E;
	 for (i = 0; i< 32; i++)
	 {
	iowrite32(tmp_en, arinc_base_addr + RNUM_RX  +4*i );
    tmp_e = ioread32(ARINC_BASE + RNUM_RX  +4*i);   //write [7:4]RNUM field is hardwared
	tmp = 0xFF & tmp_e;
	//rumboot_printf("RNUMX=0x%x\n", tmp);
	if (tmp_en !=tmp ){
	rumboot_printf("RNUMX=0x%x\n", tmp);
	return ARINC_FAILED;}
	 }
    tmp_en_freq = 0xE10000DC;
	tmp_en = 0x10000DC;
    for (i = 0; i< 16; i++)
	{
	iowrite32(tmp_en_freq, arinc_base_addr + FREQ_TX  +4*i );
    tmp_e = ioread32(ARINC_BASE + FREQ_TX  +4*i);
	tmp = 0x1FFFFFF & tmp_e;  //{tmp_e[31:29]==0,3'b0,tmp_e[25:0]};
	//rumboot_printf("FREQ_TX  =0x%x\n", tmp_en);
	if (tmp_en !=tmp ){
	rumboot_printf("FREQ_TX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}

	 tmp_en_freq = 0xE10000EA;
	 tmp_en = 0x410000EA;
	for (i = 0; i< 16; i++)
	{
	iowrite32(tmp_en_freq, arinc_base_addr + FREQ_RX  +4*i );
    tmp = ioread32(ARINC_BASE + FREQ_RX  +4*i);
	//rumboot_printf("FREQ_RX=0x%x\n", tmp_en);
	if (tmp_en !=tmp ){
	rumboot_printf("FREQ_RX=0x%x\n", tmp);
	return ARINC_FAILED;
	  }
	}
	tmp_en = 0x1F;

	for (i = 0; i< 16; i++)
	{
	iowrite32(tmp_en, arinc_base_addr + SELF_RX  +4*i );
	//rumboot_printf("SELF_RX=0x%x\n", tmp_en);
    tmp_e = ioread32(ARINC_BASE + SELF_RX  +4*i);
	//rumboot_printf("SELF_RX=0x%x\n", tmp_e);
	tmp = 0x1F & tmp_e;
	//rumboot_printf("SELF_RX=0x%x\n", tmp);
	if (tmp_en !=tmp )
	{rumboot_printf("SELF_RX=0x%x\n", tmp);
	return ARINC_FAILED;}
	}
	return ARINC_OK;


	tmp_en = 0x5A5A5A5A;
	iowrite32(tmp_en, arinc_base_addr + CHANNEL_EN );
	tmp = ioread32(ARINC_BASE + CHANNEL_EN );
	//rumboot_printf("ARINC_CHANNEL_EN=0x%x\n", tmp);
	if (tmp_en !=tmp )
	{rumboot_printf("ARINC_CHANNEL_EN=0x%x\n",tmp_en);
	return ARINC_FAILED;}
	iowrite32(tmp_en, arinc_base_addr + CHANNEL_DIS );
	//rumboot_printf("ARINC_CHANNEL_DIS=0x%x\n",tmp_en);

	return TEST_OK;
}

int main()
{
    int tmp = -1;

    tmp = arinc_rst(ARINC_BASE);
       if (tmp == ARINC_FAILED){
			rumboot_printf("ARINC test ERROR \n");
			return TEST_ERROR;
	}

	tmp = arinc_wr_rd(ARINC_BASE);

	if (tmp == ARINC_FAILED){
		rumboot_printf("ARINC test ERROR  \n");
        return TEST_ERROR;
	}
		else
		{rumboot_printf("ARINC test OK\n");
        return TEST_OK;}

}
