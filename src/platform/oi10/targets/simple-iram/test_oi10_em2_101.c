#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/regpoker.h>

#include <arch/ppc_476fp_lib_c.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_event_c.h>
#include <platform/regs/regs_mclfir.h>
#include <regs/regs_emi.h>
#include <devices/ugly/emi.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>
#include <arch/regs/plb6mcif2.h>
#include <arch/mem_window.h>

void check_plb6mcif2(const uint32_t base_address)
{
    static const uint32_t reglist[] =
    {
        PLB6MCIF2_INTR_EN,  PLB6MCIF2_MAP0CF,
        PLB6MCIF2_MAP1CF,   PLB6MCIF2_MAP2CF,
        PLB6MCIF2_MAP3CF,   PLB6MCIF2_MAXMEM,
        PLB6MCIF2_MR0CF,    PLB6MCIF2_MR1CF,
        PLB6MCIF2_MR2CF,    PLB6MCIF2_MR3CF,
        PLB6MCIF2_P6BMTAG1, PLB6MCIF2_P6BMTAG2,
        PLB6MCIF2_P6BMTAG3, PLB6MCIF2_PLBASYNC,
        PLB6MCIF2_PLBCFG,   PLB6MCIF2_PLBORD,
        PLB6MCIF2_PUABA,    ~0
    };

    static const struct regpoker_checker plb6mcif2_regs_default[] = {
        //      name                check type          offset         exp val                  mask
        {"PLB6MCIF2_BEARL",     REGPOKER_READ_DCR, PLB6MCIF2_BEARL,     0x00,                0xffffffff},
        {"PLB6MCIF2_BEARU",     REGPOKER_READ_DCR, PLB6MCIF2_BEARU,     0x00,                0xffffffff},
        {"PLB6MCIF2_BESR_read", REGPOKER_READ_DCR, PLB6MCIF2_BESR_read, 0x00,                0xffffffff},
        {"PLB6MCIF2_INTR_EN",   REGPOKER_READ_DCR, PLB6MCIF2_INTR_EN,   0xFFE001C0,          0xffffffff},
        {"PLB6MCIF2_MAP0CF",    REGPOKER_READ_DCR, PLB6MCIF2_MAP0CF,    0x00,                0xffffffff},
        {"PLB6MCIF2_MAP1CF",    REGPOKER_READ_DCR, PLB6MCIF2_MAP1CF,    0x00,                0xffffffff},
        {"PLB6MCIF2_MAP2CF",    REGPOKER_READ_DCR, PLB6MCIF2_MAP2CF,    0x00,                0xffffffff},
        {"PLB6MCIF2_MAP3CF",    REGPOKER_READ_DCR, PLB6MCIF2_MAP3CF,    0x00,                0xffffffff},
        {"PLB6MCIF2_MAXMEM",    REGPOKER_READ_DCR, PLB6MCIF2_MAXMEM,    0xC0000000,          0xffffffff},  //default: [0:1]= 0b11
        {"PLB6MCIF2_MR0CF",     REGPOKER_READ_DCR, PLB6MCIF2_MR0CF,     0x00009001,          0xffffffff},
        {"PLB6MCIF2_MR1CF",     REGPOKER_READ_DCR, PLB6MCIF2_MR1CF,     0xf0009001,          0xffffffff},
        {"PLB6MCIF2_MR2CF",     REGPOKER_READ_DCR, PLB6MCIF2_MR2CF,     0x00,                0xffffffff},
        {"PLB6MCIF2_MR3CF",     REGPOKER_READ_DCR, PLB6MCIF2_MR3CF,     0x00,                0xffffffff},
        {"PLB6MCIF2_P6BMTAG1",  REGPOKER_READ_DCR, PLB6MCIF2_P6BMTAG1,  0x00,                0xffffffff},
        {"PLB6MCIF2_P6BMTAG2",  REGPOKER_READ_DCR, PLB6MCIF2_P6BMTAG2,  0x00,                0xffffffff},
        {"PLB6MCIF2_P6BMTAG3",  REGPOKER_READ_DCR, PLB6MCIF2_P6BMTAG3,  0x00,                0xffffffff},
        {"PLB6MCIF2_PLBASYNC",  REGPOKER_READ_DCR, PLB6MCIF2_PLBASYNC,  0x00,                0xffffffff},
        {"PLB6MCIF2_PLBCFG",    REGPOKER_READ_DCR, PLB6MCIF2_PLBCFG,    0x200000F1,          0xffffffff},
        {"PLB6MCIF2_PLBORD",    REGPOKER_READ_DCR, PLB6MCIF2_PLBORD,    0x40000000,          0xffffffff},
        {"PLB6MCIF2_PUABA",     REGPOKER_READ_DCR, PLB6MCIF2_PUABA,     0x00,                0xffffffff},
        {"PLB6MCIF2_RID",       REGPOKER_READ_DCR, PLB6MCIF2_RID,       PLB6MCIF2_WATERMARK, 0xffffffff},
        {"PLB6MCIF2_SR0CF",     REGPOKER_READ_DCR, PLB6MCIF2_SR0CF,     0x00,                0xffffffff},
        {"PLB6MCIF2_SR1CF",     REGPOKER_READ_DCR, PLB6MCIF2_SR1CF,     0x00,                0xffffffff},
        {"PLB6MCIF2_SR2CF",     REGPOKER_READ_DCR, PLB6MCIF2_SR2CF,     0x00,                0xffffffff},
        {"PLB6MCIF2_SR3CF",     REGPOKER_READ_DCR, PLB6MCIF2_SR3CF,     0x00,                0xffffffff},
        {"PLB6MCIF2_STATUS",    REGPOKER_READ_DCR, PLB6MCIF2_STATUS,    0x80000000,          0xffffffff},
        {/* Sentinel */ }
    };

    static const struct regpoker_checker plb6mcif2_regs_write[] = {
        //      name                check type          offset           val                  mask
        {"PLB6MCIF2_INTR_EN",   REGPOKER_WRITE_DCR, PLB6MCIF2_INTR_EN,   0xFFE001C0,          0xffffffff},
        {"PLB6MCIF2_MAP0CF",    REGPOKER_WRITE_DCR, PLB6MCIF2_MAP0CF,    0x00,                0xffffffff},
        {"PLB6MCIF2_MAP1CF",    REGPOKER_WRITE_DCR, PLB6MCIF2_MAP1CF,    0x00,                0xffffffff},
        {"PLB6MCIF2_MAP2CF",    REGPOKER_WRITE_DCR, PLB6MCIF2_MAP2CF,    0x00,                0xffffffff},
        {"PLB6MCIF2_MAP3CF",    REGPOKER_WRITE_DCR, PLB6MCIF2_MAP3CF,    0x00,                0xffffffff},
        {"PLB6MCIF2_MAXMEM",    REGPOKER_WRITE_DCR, PLB6MCIF2_MAXMEM,    0xC0000000,          PLB6MCIF2_MAXMEM_MSK},
        {"PLB6MCIF2_MR0CF",     REGPOKER_WRITE_DCR, PLB6MCIF2_MR0CF,     0x01,                PLB6MCIF2_MRxCF_MSK},
        {"PLB6MCIF2_MR1CF",     REGPOKER_WRITE_DCR, PLB6MCIF2_MR1CF,     0x00,                PLB6MCIF2_MRxCF_MSK},
        {"PLB6MCIF2_MR2CF",     REGPOKER_WRITE_DCR, PLB6MCIF2_MR2CF,     0x00,                PLB6MCIF2_MRxCF_MSK},
        {"PLB6MCIF2_MR3CF",     REGPOKER_WRITE_DCR, PLB6MCIF2_MR3CF,     0xfff80001,          PLB6MCIF2_MRxCF_MSK},
        {"PLB6MCIF2_P6BMTAG1",  REGPOKER_WRITE_DCR, PLB6MCIF2_P6BMTAG1,  0x00,                PLB6MCIF2_P6BMTAG1_MSK},
        {"PLB6MCIF2_P6BMTAG2",  REGPOKER_WRITE_DCR, PLB6MCIF2_P6BMTAG2,  0x00,                PLB6MCIF2_P6BMTAG2_MSK},
        {"PLB6MCIF2_P6BMTAG3",  REGPOKER_WRITE_DCR, PLB6MCIF2_P6BMTAG3,  0x00,                PLB6MCIF2_P6BMTAG3_MSK},
        {"PLB6MCIF2_PLBASYNC",  REGPOKER_WRITE_DCR, PLB6MCIF2_PLBASYNC,  0x00,                PLB6MCIF2_PLBASYNC_MSK},
        {"PLB6MCIF2_PLBCFG",    REGPOKER_WRITE_DCR, PLB6MCIF2_PLBCFG,    0x200000F1,          PLB6MCIF2_PLBCFG_MSK},
        {"PLB6MCIF2_PLBORD",    REGPOKER_WRITE_DCR, PLB6MCIF2_PLBORD,    0x40000000,          PLB6MCIF2_PLBORD_MSK},
        {"PLB6MCIF2_PUABA",     REGPOKER_WRITE_DCR, PLB6MCIF2_PUABA,     0x00,                0xfffffffe},
        {/* Sentinel */ }
    };

    uint32_t saved_regs[ARRAY_SIZE(reglist) - 1];
    int i;

    rumboot_printf("Check default values\n");
    TEST_ASSERT (rumboot_regpoker_check_array(plb6mcif2_regs_default, base_address) == 0, "TEST ERROR" );

    rumboot_printf("Check write/read\n");
    for(i = 0; ~reglist[i]; i++)
        saved_regs[i] = dcr_read(base_address + reglist[i]);
    TEST_ASSERT (rumboot_regpoker_check_array(plb6mcif2_regs_write, base_address) == 0, "TEST ERROR" );
    for(i = 0; ~reglist[i]; i++)
        dcr_write(base_address + reglist[i], saved_regs[i]);
}

void check_mclfir_wo (uint32_t base_addr)
{
    rumboot_printf("Check write-only regs\n");

    dcr_write(base_addr + MCLFIR_MC_ERR0 + 4, 0x80000000);//set 1 bit ([0]) of   MCLFIR_MC_ERR0     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_ERR0) == 0x80000000, "TEST_ERROR"); //0x80000000 as result - indirect     MCLFIR_MC_ERR0_OR  register check

    dcr_write(base_addr + MCLFIR_MC_ERR0 + 2, 0x7fffffff);//reset 1 bit ([0]) of MCLFIR_MC_ERR0     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_ERR0) == 0x00, "TEST_ERROR"); //0x00000000 as result - indirect     MCLFIR_MC_ERR0_AND register check

    dcr_write(base_addr + MCLFIR_MC_ERR1 + 4, 0x80000000);//set 1 bit ([0]) of   MCLFIR_MC_ERR1     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_ERR1) == 0x80000000, "TEST_ERROR"); //0x80000000 as result - indirect     MCLFIR_MC_ERR1_OR  register check

    dcr_write(base_addr + MCLFIR_MC_ERR1 + 2, 0x7fffffff);//reset 1 bit ([0]) of MCLFIR_MC_ERR1     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_ERR1) == 0x00, "TEST_ERROR"); //0x00000000 as result - indirect     MCLFIR_MC_ERR1_AND register check

    dcr_write(base_addr + MCLFIR_MC_ERR_MSK0 + 2, 0x7fffffff);//reset 1 bit ([0]) of MCLFIR_MC_ERR_MSK0     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_ERR_MSK0) == 0x7fffffff, "TEST_ERROR"); //0x7fffffff as result - indirect     MCLFIR_MC_ERR_MSK0_AND register check

    dcr_write(base_addr + MCLFIR_MC_ERR_MSK0 + 4, 0x80000000);//set 1 bit ([0]) of   MCLFIR_MC_ERR_MSK0     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_ERR_MSK0) == 0xffffffff, "TEST_ERROR"); //0xffffffff as result - indirect     MCLFIR_MC_ERR_MSK0_OR  register check

    dcr_write(base_addr + MCLFIR_MC_ERR_MSK1 + 2, 0x7fffffff);//reset 1 bit ([0]) of MCLFIR_MC_ERR_MSK1     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_ERR_MSK1) == 0x7fffffc0, "TEST_ERROR"); //0x7fffffc0 as result - indirect     MCLFIR_MC_ERR_MSK1_AND register check

    dcr_write(base_addr + MCLFIR_MC_ERR_MSK1 + 4, 0x80000000);//set 1 bit ([0]) of   MCLFIR_MC_ERR_MSK1     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_ERR_MSK1) == 0xffffffc0, "TEST_ERROR"); //0xffffffc0 as result - indirect     MCLFIR_MC_ERR_MSK1_OR  register check

    dcr_write(base_addr + MCLFIR_MC_LFIR + 2, 0x80000000);//set 1 bit ([0]) of   MCLFIR_MC_LFIR     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_LFIR) == 0x80000000, "TEST_ERROR"); //0x80000000 as result - indirect     MCLFIR_MC_LFIR_OR  register check

    dcr_write(base_addr + MCLFIR_MC_LFIR + 1, 0x7fffffff);//reset 1 bit ([0]) of MCLFIR_MC_LFIR     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_LFIR) == 0x00, "TEST_ERROR"); //0x00000000 as result - indirect     MCLFIR_MC_LFIR_AND register check

    dcr_write(base_addr + MCLFIR_MC_LFIR_MSK + 1, 0x7fffffff);//reset 1 bit ([0]) of MCLFIR_MC_LFIR_MSK     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_LFIR_MSK) == 0x40000000, "TEST_ERROR"); //0x40000000 as result - indirect     MCLFIR_MC_LFIR_MSK_AND register check

    dcr_write(base_addr + MCLFIR_MC_LFIR_MSK + 2, 0x80000000);//set 1 bit ([0]) of   MCLFIR_MC_LFIR_MSK     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_LFIR_MSK) == 0xc0000000, "TEST_ERROR"); //0xc0000000 as result - indirect     MCLFIR_MC_LFIR_MSK_OR  register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STAT + 2, 0x00008000);//set 1 bit ([16]) of   MCLFIR_MC_CONSOL_STAT     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STAT) == 0x00f08000, "TEST_ERROR"); //0x00108000 as result - indirect      MCLFIR_MC_CONSOL_STAT_OR  register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STAT + 1, 0xffff7fff);//reset 1 bit ([16]) of MCLFIR_MC_CONSOL_STAT     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STAT) == 0x00f00000, "TEST_ERROR"); //0x00100000 as result - indirect      MCLFIR_MC_CONSOL_STAT_AND register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STAT_TRIG + 2, 0x00008000);//set 1 bit ([16]) of   MCLFIR_MC_CONSOL_STAT_TRIG     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STAT_TRIG) == 0x00008000, "TEST_ERROR"); //0x00008000 as result - indirect      MCLFIR_MC_CONSOL_STAT_TRIG_OR  register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STAT_TRIG + 1, 0xffff7fff);//reset 1 bit ([16]) of MCLFIR_MC_CONSOL_STAT_TRIG     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STAT_TRIG) == 0x00, "TEST_ERROR"); //0x00000000 as result - indirect      MCLFIR_MC_CONSOL_STAT_TRIG_AND register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STAT_MSK + 1, 0xffff7fff);//reset 1 bit ([16]) of MCLFIR_MC_CONSOL_STAT_MSK     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STAT_MSK) == 0xfff00000, "TEST_ERROR"); //0xfff00000 as result - indirect      MCLFIR_MC_CONSOL_STAT_MSK_AND register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STAT_MSK + 2, 0x00008000);//set 1 bit ([16]) of   MCLFIR_MC_CONSOL_STAT_MSK     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STAT_MSK) == 0xfff08000, "TEST_ERROR"); //0xfff08000 as result - indirect      MCLFIR_MC_CONSOL_STAT_MSK_OR  register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STATSUM + 2, 0x80000000);//set 1 bit ([0]) of   MCLFIR_MC_CONSOL_STATSUM     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STATSUM) == 0x80000000, "TEST_ERROR"); //0x80000000 as result - indirect     MCLFIR_MC_CONSOL_STATSUM_OR  register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STATSUM + 1, 0x7fffffff);//reset 1 bit ([0]) of MCLFIR_MC_CONSOL_STATSUM     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STATSUM) == 0x00, "TEST_ERROR"); //0x00000000 as result - indirect     MCLFIR_MC_CONSOL_STATSUM_AND register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STATSUM_MSK + 2, 0x80000000);//set 1 bit ([0]) of   MCLFIR_MC_CONSOL_STATSUM_MSK     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STATSUM_MSK) == 0x80000000, "TEST_ERROR"); //0x80000000 as result - indirect     MCLFIR_MC_CONSOL_STATSUM_MSK_OR  register check

    dcr_write(base_addr + MCLFIR_MC_CONSOL_STATSUM_MSK + 1, 0x7fffffff);//reset 1 bit ([0]) of MCLFIR_MC_CONSOL_STATSUM_MSK     register
    TEST_ASSERT (dcr_read (base_addr + MCLFIR_MC_CONSOL_STATSUM_MSK) == 0x00, "TEST_ERROR"); //0x00000000 as result - indirect     MCLFIR_MC_CONSOL_STATSUM_MSK_AND register check

}

void check_mclfir(const uint32_t base_address)
{
    rumboot_printf("Check default values\n");

    static const struct regpoker_checker mclfir_regs_default[] = {
        //      name                       check type          offset                    exp val                  mask
        {"MCLFIR_MC_ERR0",               REGPOKER_READ_DCR, MCLFIR_MC_ERR0,               0x00,          0xffffffff},
        {"MCLFIR_MC_ERR1",               REGPOKER_READ_DCR, MCLFIR_MC_ERR1,               0x00,          0xffffffff},
        {"MCLFIR_MC_ERR_MSK0",           REGPOKER_READ_DCR, MCLFIR_MC_ERR_MSK0,           0xFFFFFFFF,    0xffffffff},
        {"MCLFIR_MC_ERR_MSK1",           REGPOKER_READ_DCR, MCLFIR_MC_ERR_MSK1,           0xFFFFFFC0,    0xffffffff},
        {"MCLFIR_MC_ERR_ACTION0",        REGPOKER_READ_DCR, MCLFIR_MC_ERR_ACTION0,        0x00,          0xffffffff},
        {"MCLFIR_MC_ERR_ACTION1",        REGPOKER_READ_DCR, MCLFIR_MC_ERR_ACTION1,        0x00,          0xffffffff},
        {"MCLFIR_MC_ERR_WOF0",           REGPOKER_READ_DCR, MCLFIR_MC_ERR_WOF0,           0x00,          0xffffffff},
        {"MCLFIR_MC_ERR_WOF1",           REGPOKER_READ_DCR, MCLFIR_MC_ERR_WOF1,           0x00,          0xffffffff},
        {"MCLFIR_MC_LFIR",               REGPOKER_READ_DCR, MCLFIR_MC_LFIR,               0x00,          0xffffffff},
        {"MCLFIR_MC_LFIR_MSK",           REGPOKER_READ_DCR, MCLFIR_MC_LFIR_MSK,           0xC0000000,    0xffffffff},
        {"MCLFIR_MC_CONSOL_STAT",        REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STAT,        0x00f00000,    0xffffffff},
        {"MCLFIR_MC_CONSOL_STAT_TRIG",   REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STAT_TRIG,   0x00,          0xffffffff},
        {"MCLFIR_MC_CONSOL_STAT_MSK",    REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STAT_MSK,    0xFFF08000,    0xffffffff},
        {"MCLFIR_MC_CONSOL_STATSUM",     REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STATSUM,     0x00,          0xffffffff},
        {"MCLFIR_MC_CONSOL_STATSUM_MSK", REGPOKER_READ_DCR, MCLFIR_MC_CONSOL_STATSUM_MSK, 0x80000000,    0xffffffff},
        {  }
    };
    TEST_ASSERT (rumboot_regpoker_check_array(mclfir_regs_default, base_address) == 0, "TEST ERROR" );

    rumboot_printf("Check write/read\n");
    static const struct regpoker_checker mclfir_regs_write[] = {
        //      name                check type            offset                val         mask
        {"MCLFIR_MC_ERR_ACTION0",  REGPOKER_WRITE_DCR, MCLFIR_MC_ERR_ACTION0,   0x00,    0xffffffff},
        {"MCLFIR_MC_ERR_ACTION1",  REGPOKER_WRITE_DCR, MCLFIR_MC_ERR_ACTION1,   0x00,    0xffffffc0},
        {"MCLFIR_MC_ERR_WOF0",     REGPOKER_WRITE_DCR, MCLFIR_MC_ERR_WOF0,      0x00,    0xffffffff},
        {"MCLFIR_MC_ERR_WOF1",     REGPOKER_WRITE_DCR, MCLFIR_MC_ERR_WOF1,      0x00,    0xffffffc0},
        { }
    };
    TEST_ASSERT (rumboot_regpoker_check_array(mclfir_regs_write, base_address) == 0, "TEST ERROR" );
    check_mclfir_wo (base_address);
}

void check_emi(const uint32_t base_address)
{
    rumboot_printf("Check default values\n");
    static const struct regpoker_checker emi_regs_default[] = {
        // name             check type     offset        exp val            mask
        {"EMI_SS0",     REGPOKER_READ_DCR, EMI_SS0,   0x800000,          0xffffffff},
        {"EMI_SD0",     REGPOKER_READ_DCR, EMI_SD0,       0x00,          0xffffffff},
        {"EMI_SS1",     REGPOKER_READ_DCR, EMI_SS1,   0x800000,          0xffffffff},
        {"EMI_SD1",     REGPOKER_READ_DCR, EMI_SD1,       0x00,          0xffffffff},
        {"EMI_SS2",     REGPOKER_READ_DCR, EMI_SS2,   0x800000,          0xffffffff},
        {"EMI_SD2",     REGPOKER_READ_DCR, EMI_SD2,       0x00,          0xffffffff},
        {"EMI_SS3",     REGPOKER_READ_DCR, EMI_SS3,   0x800000,          0xffffffff},
        {"EMI_SD3",     REGPOKER_READ_DCR, EMI_SD3,       0x00,          0xffffffff},
        {"EMI_SS4",     REGPOKER_READ_DCR, EMI_SS4,   0x800000,          0xffffffff},
        {"EMI_SD4",     REGPOKER_READ_DCR, EMI_SD4,       0x00,          0xffffffff},
        {"EMI_SS5",     REGPOKER_READ_DCR, EMI_SS5,   0x800000,          0xffffffff},
        {"EMI_SD5",     REGPOKER_READ_DCR, EMI_SD5,       0x00,          0xffffffff},
        {"EMI_RFC",     REGPOKER_READ_DCR, EMI_RFC,       0x00,          0xffffffff},
        {"EMI_HSTSR",   REGPOKER_READ_DCR, EMI_HSTSR,     0x00,          0xffffffff},
        {"EMI_ECNT20",  REGPOKER_READ_DCR, EMI_ECNT20,    0x00,          0xffffffff},
        {"EMI_ECNT53",  REGPOKER_READ_DCR, EMI_ECNT53,    0x00,          0xffffffff},
        {"EMI_BUSEN",   REGPOKER_READ_DCR, EMI_BUSEN,     0x00,          0xffffffff},
        {"EMI_WECR",    REGPOKER_READ_DCR, EMI_WECR,      0x00,          0xffffffff},
        {"EMI_FLCNTRL", REGPOKER_READ_DCR, EMI_FLCNTRL,   0x1C,          0xffffffff},
        {"EMI_IMR",     REGPOKER_READ_DCR, EMI_IMR,       0x00,          0xffffffff},
        {"EMI_IMR_SET", REGPOKER_READ_DCR, EMI_IMR_SET,   0x00,          0xffffffff},
        {"EMI_IMR_RST", REGPOKER_READ_DCR, EMI_IMR_RST,   0x00,          0xffffffff},
        {"EMI_IRR",     REGPOKER_READ_DCR, EMI_IRR,       0x00,          0xffffffff},
        {"EMI_IRR_RST", REGPOKER_READ_DCR, EMI_IRR_RST,   0x00,          0xffffffff},
        {/* Sentinel */ }
    };
    TEST_ASSERT (rumboot_regpoker_check_array(emi_regs_default, base_address) == 0, "TEST ERROR" );

    rumboot_printf("Check write/read\n");
    static const struct regpoker_checker emi_regs_write[] = {
        // name             check type       offset         val             mask
        {"EMI_SS0",     REGPOKER_WRITE_DCR, EMI_SS0,       0x00,         0x001fffff},
        {"EMI_SD0",     REGPOKER_WRITE_DCR, EMI_SD0,       0x00,         0x0000fffe},
        {"EMI_SS1",     REGPOKER_WRITE_DCR, EMI_SS1,       0x00,         0x001fffff},
        {"EMI_SD1",     REGPOKER_WRITE_DCR, EMI_SD1,       0x00,         0x0000fffe},
        {"EMI_SS2",     REGPOKER_WRITE_DCR, EMI_SS2,       0x00,         0x001fffff},
        {"EMI_SD2",     REGPOKER_WRITE_DCR, EMI_SD2,       0x00,         0x0000fffe},
        {"EMI_SS3",     REGPOKER_WRITE_DCR, EMI_SS3,       0x00,         0x001fffff},
        {"EMI_SD3",     REGPOKER_WRITE_DCR, EMI_SD3,       0x00,         0x0000fffe},
        {"EMI_SS4",     REGPOKER_WRITE_DCR, EMI_SS4,       0x00,         0x001fffff},
        {"EMI_SD4",     REGPOKER_WRITE_DCR, EMI_SD4,       0x00,         0x0000fffe},
        {"EMI_SS5",     REGPOKER_WRITE_DCR, EMI_SS5,       0x00,         0x001fffff},
        {"EMI_SD5",     REGPOKER_WRITE_DCR, EMI_SD5,       0x00,         0x0000fffe},
        {"EMI_RFC",     REGPOKER_WRITE_DCR, EMI_RFC,       0x00,         0x0003fffe},
        {"EMI_HSTSR",   REGPOKER_WRITE_DCR, EMI_HSTSR,     0x00,         0x0000003f},
        {"EMI_ECNT20",  REGPOKER_WRITE_DCR, EMI_ECNT20,    0x00,         0x00ffffff},
        {"EMI_ECNT53",  REGPOKER_WRITE_DCR, EMI_ECNT53,    0x00,         0x00ffffff},
        {"EMI_BUSEN",   REGPOKER_WRITE_DCR, EMI_BUSEN,     0x00,         0x00000001},
        {"EMI_WECR",    REGPOKER_WRITE_DCR, EMI_WECR,      0x00,         0x0000007f},
        {"EMI_FLCNTRL", REGPOKER_WRITE_DCR, EMI_FLCNTRL,   0x1C,         0x0000001f},
        {"EMI_IMR",     REGPOKER_WRITE_DCR, EMI_IMR,       0x00,         0x0001ffff},
        {"EMI_IRR",     REGPOKER_WRITE_DCR, EMI_IRR,       0x00,         0x0001ffff},
        {"EMI_ECCWRR",  REGPOKER_WRITE_DCR, EMI_ECCWRR,    0x00,         0x000000ff},
        {"EMI_ECCRDR",  REGPOKER_WRITE_DCR, EMI_ECCRDR,    0x00,         0x000000ff},
        {"EMI_H1CMR",   REGPOKER_WRITE_DCR, EMI_H1CMR,     0x00,         0x0000007f},
        {"EMI_H2CMR",   REGPOKER_WRITE_DCR, EMI_H2CMR,     0x00,         0x0000007f},
        {"EMI_RREQCMR", REGPOKER_WRITE_DCR, EMI_RREQCMR,   0x00,         0x0000007f},
        {"EMI_WREQCMR", REGPOKER_WRITE_DCR, EMI_WREQCMR,   0x00,         0x0000007f},
        {"EMI_WDCMR",   REGPOKER_WRITE_DCR, EMI_WDCMR,     0x00,         0x0000007f},
        {"EMI_H1ADR",   REGPOKER_WRITE_DCR, EMI_H1ADR,     0x00,         0xfffffff0},
        {"EMI_H2ADR",   REGPOKER_WRITE_DCR, EMI_H2ADR,     0x00,         0xfffffff0},
        {"EMI_RREQADR", REGPOKER_WRITE_DCR, EMI_RREQADR,   0x00,         0xfffffff0},
        {"EMI_WREQADR", REGPOKER_WRITE_DCR, EMI_WREQADR,   0x00,         0xfffffff0},
        {"EMI_WDADR",   REGPOKER_WRITE_DCR, EMI_WDADR,     0x00,         0xfffffff0},
        {/* Sentinel */ }
    };
    TEST_ASSERT (rumboot_regpoker_check_array(emi_regs_write, base_address) == 0, "TEST ERROR" );
}


int main()
{
// did, 21.07.21, OI10-489
    rumboot_printf("Remove O32T IM3 TLB entries\n");
    static const tlb_entry sram0_tlb_entry[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
       {MMU_TLB_ENTRY(  0x020,  0xC0070,    0x80070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,  MMU_TLBWE_BE_UND,   0b0 )},
       {MMU_TLB_ENTRY(  0x020,  0xC0070,    0xC0070,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b0 )},
       {MMU_TLB_ENTRY(  0x020,  0xC0060,    0x80060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_5,     0b0 )},
        {MMU_TLB_ENTRY(  0x020,  0xC0060,    0xC0060,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_UND,  MMU_TLBWE_BE_UND,   0b0 )}
    };
    write_tlb_entries(sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry));

    rumboot_printf("\nCHECK PLB6MCIF2\n\n");
    check_plb6mcif2 (DCR_EM2_PLB6MCIF2_BASE);

    rumboot_printf("\nCHECK EMI\n\n");
    check_emi (DCR_EM2_EMI_BASE);

    rumboot_printf("\nCHECK MCLFIR\n\n");
    check_mclfir (DCR_EM2_MCLFIR_BASE);

    return 0;
}
