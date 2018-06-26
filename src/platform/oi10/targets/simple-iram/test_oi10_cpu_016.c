#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/regs/em2/regs_plb6mcif2.h>
#include <platform/regs/em2/regs_mclfir.h>

static int32_t check_array32[] = {
        0xFFFFFFFF,
        0x00000000,
        0xFFFF0000,
        0x0000FFFF,
        0xFF00FF00,
        0x00FF00FF,
        0xF0F0F0F0,
        0x0F0F0F0F,
        0xCCCCCCCC,
        0x33333333,
        0xAAAAAAAA,
        0x55555555
};

uint32_t check_plb6mcif2(const uint32_t base_address)
{
    uint32_t i;
    uint32_t result = 0;

    TEST_ASSERT(dcr_read(PLB6MCIF2_BEARL + base_address) == 0x00000000,"In PLB6MCIF2_BEARL expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_BEARU + base_address) == 0x00000000,"In PLB6MCIF2_BEARU expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_BESR + base_address) == 0x00000000,"In PLB6MCIF2_BESR expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_INTR_EN + base_address) == 0xFFE001C0,"In PLB6MCIF2_INTR_EN expected value 0xFFE001C0");
    TEST_ASSERT(dcr_read(PLB6MCIF2_MAP0CF + base_address) == 0x00000000,"In PLB6MCIF2_MAP0CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_MAP1CF + base_address) == 0x00000000,"In PLB6MCIF2_MAP1CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_MAP2CF + base_address) == 0x00000000,"In PLB6MCIF2_MAP2CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_MAP3CF + base_address) == 0x00000000,"In PLB6MCIF2_MAP3CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_MAXMEM + base_address) == 0xC0000000,"In PLB6MCIF2_MAXMEM expected value 0xC0000000");//default: [0:1]= 0b11
    TEST_ASSERT(dcr_read(PLB6MCIF2_MR0CF + base_address) == 0x00000000,"In PLB6MCIF2_MR0CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_MR1CF + base_address) == 0x00000000,"In PLB6MCIF2_MR1CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_MR2CF + base_address) == 0x00000000,"In PLB6MCIF2_MR2CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_MR3CF + base_address) == 0x00000000,"In PLB6MCIF2_MR3CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_P6BMTAG1 + base_address) == 0x00000000,"In PLB6MCIF2_P6BMTAG1 expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_P6BMTAG2 + base_address) == 0x00000000,"In PLB6MCIF2_P6BMTAG2 expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_P6BMTAG3 + base_address) == 0x00000000,"In PLB6MCIF2_P6BMTAG3 expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_PLBASYNC + base_address) == 0x00000000,"In PLB6MCIF2_PLBASYNC expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_PLBCFG + base_address) == 0x200000F0,"In PLB6MCIF2_PLBCFG expected value 0x200000F0");

    result = dcr_read(PLB6MCIF2_PLBORD + base_address);
    TEST_ASSERT(((result == 0x01000000) || (result == 0x40000000) || (result == 0x20000000)),"In PLB6MCIF2_PLBORD expected value 0x01000000 or 0x40000000 or 0x20000000");
    rumboot_puthex(result);

    TEST_ASSERT(dcr_read(PLB6MCIF2_PUABA + base_address) == 0x00000000,"In PLB6MCIF2_PUABA expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_RID + base_address) == PLB6MCIF2_WATERMARK,"In PLB6MCIF2_RevID expected value 0x00_0007_07");
    TEST_ASSERT(dcr_read(PLB6MCIF2_SR0CF + base_address) == 0x00000000,"In PLB6MCIF2_SR0CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_SR1CF + base_address) == 0x00000000,"In PLB6MCIF2_SR1CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_SR2CF + base_address) == 0x00000000,"In PLB6MCIF2_SR2CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_SR3CF + base_address) == 0x00000000,"In PLB6MCIF2_SR3CF expected value 0x00000000");
    TEST_ASSERT(dcr_read(PLB6MCIF2_STATUS + base_address) == 0x80000000,"In PLB6MCIF2_STATUS expected value 0x80000000");

    for (i = 0; i< 12; i++){
        dcr_write(PLB6MCIF2_MAP0CF + base_address, check_array32[i]);
        TEST_ASSERT(dcr_read(PLB6MCIF2_MAP0CF + base_address) == check_array32[i],"PLB6MCIF2 DCR data bus check failed");
    }

    return true;
}

uint32_t check_mclfir(const uint32_t base_address)
{
    uint32_t i;
    TEST_ASSERT(dcr_read(MCLFIR_MC_ERR0 + base_address) == 0x00000000,"In MC_ERR0 expected value 0x00000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_ERR1 + base_address) == 0x00000000,"In MC_ERR1 expected value 0x00000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_ERR_MSK0 + base_address) == 0xFFFFFFFF,"In MC_ERR_MSK0 expected value 0xFFFFFFFF");
    TEST_ASSERT(dcr_read(MCLFIR_MC_ERR_MSK1 + base_address) == 0xFFFFFFC0,"In MC_ERR_MSK1 expected value 0xFFFFFFC0");
    TEST_ASSERT(dcr_read(MCLFIR_MC_ERR_ACTION0 + base_address) == 0x00000000,"In MC_ERR_ACTION0 expected value 0x00000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_ERR_ACTION1 + base_address) == 0x00000000,"In MC_ERR_ACTION1 expected value 0x00000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_ERR_WOF0 + base_address) == 0x00000000,"In MC_ERR_WOF0 expected value 0x00000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_ERR_WOF1 + base_address) == 0x00000000,"In MC_ERR_WOF1 expected value 0x00000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_LFIR + base_address) == 0x00000000,"In MC_LFIR expected value 0x00000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_LFIR_MSK + base_address) == 0xC0000000,"In MC_LFIR_MSK expected value 0xC0000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_CONSOL_STAT + base_address) == 0x03100000,"In MC_CONSOL_STAT expected value 0x03100000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_CONSOL_STAT_TRIG + base_address) == 0x00000000,"In MC_CONSOL_STAT_TRIG expected value 0x00000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_CONSOL_STAT_MSK + base_address) == 0xFFF08000,"In MC_CONSOL_STAT_MSK expected value 0xFFF08000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_CONSOL_STATSUM + base_address) == 0x00000000,"In MC_CONSOL_STATSUM expected value 0x00000000");
    TEST_ASSERT(dcr_read(MCLFIR_MC_CONSOL_STATSUM_MSK + base_address) == 0x80000000,"In MC_CONSOL_STATSUM_MSK expected value 0x80000000");

    for (i = 0; i< 12; i++){
        dcr_write (MCLFIR_MC_ERR_ACTION0 + base_address, check_array32[i]);
        TEST_ASSERT(dcr_read(MCLFIR_MC_ERR_ACTION0 + base_address) == check_array32[i],"mclfir DCR data bus check failed");
    }

    return true;
}

int main()
{
	//rumboot_putstring("CHECK PLB6MCIF2\n");
	//check_plb6mcif2 (DCR_EM2_PLB6MCIF2_BASE);
	rumboot_putstring("CHECK MCLFIR\n");
	check_mclfir (DCR_EM2_MCLFIR_BASE);

	return 0;
}
