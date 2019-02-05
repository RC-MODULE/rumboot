/*
 * test_oi10_cpu_021.c
 *
 *  Created on: Jan 28, 2018
 *      Author: v.fufaev
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/irq.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/interrupts.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/l2c.h>
#include <platform/regs/regs_l2c_l2.h>

//Cache states
#define  CST_I   0b000          // Invalid
#define  CST_P   0b001          // Pending/Reserved
#define  CST_S   0b010          // Shared
#define  CST_SL  0b011          // Shared Last
#define  CST_E   0b100          // Exclusive
#define  CST_T   0b101          // Tagged
#define  CST_M   0b110          // Modified
#define  CST_MU  0b111          // Modified Unsolicited

#define TEST_ADDR           (0x100)
#define TEST_DATA           (0xC0DEBEEF)

//Cache state
#define TEST_STATE          CST_M

/*                        MMU_TLB_ENTRY(  ERPN,     RPN,         EPN,          DSIZ,              IL1I,   IL1D,    W,      I,      M,      G,           E,                 UX, UW, UR,     SX, SW, SR     DULXE,  IULXE,       TS,         TID,                WAY,              BID,             V )*/
#define TLB_ENTRY0_CACHE  MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY1_CACHE  MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY0_INV    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY1_INV    MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b0 )

//Data for L2C TAG testing
volatile uint32_t tag_data[] ={
    0x0002033,    0x0002004,    0x000209F,    0x00020A8,
    0x000201A,    0x000202D,    0x00020BE,    0x0002025
};
volatile uint32_t tag_ecc[] ={
    0x00,    0x7F,    0x0F,    0x70,
    0x4C,    0x33,    0x5A,    0x2A
};
//Data for L2C DATA testing
volatile uint64_t __attribute__((aligned(128))) data_data[] = {
    0x0000000000000197,    0x0000000000000000,    0x0000000000000090,    0x0000000000000107,
    0x0000000000000110,    0x0000000000000087,    0x0000000000000115,    0x0000000000000082,
    0x0000000000000197,    0xFFFFFFFFFFFFFFFF,    0x00FFFFFFFFFFE013,    0xFF00000000001117,
    0x00FFFFFFFF001133,    0xFF00000000FFE037,    0x00FFFF0000FFFF82,    0xFF0000FFFF000186,
    0x00FF00FF00FF00FF,    0xFF00FF00FF00FE97,    0xF0F0F0F0F0F0F194,    0x0F0F0F0F0F0F0F90,
    0x33333333333333B4,    0xCCCCCCCCCCCCCDB0,    0xAAAAAAAAAAAAABB0,    0x55555555555555B4,
    0x30A0000000000000,    0xFFFFFFFFFFFFFFFF,    0xF0FFFFFFFFFFFFFF,    0x30A0000000000000,
    0xA4DFFFFFFF000000,    0x0780000000FFFFFF,    0xB19FFF0000FFFF00,    0x110000FFFF0000FF,
    0x00FF00FF00FF00FF,    0xA060FF00FF00FF00,    0xA110F0F0F0F0F0F0,    0x018F0F0F0F0F0F0F,
    0x0553333333333333,    0xAACCCCCCCCCCCCCC,    0x28AAAAAAAAAAAAAA,    0x84F5555555555555
};
volatile uint32_t data_ecc[] ={
    0x00,    0xFF,    0x0F,    0xF0,
    0xCC,    0x33,    0xAA,    0x55,
    0x00,    0xFF,    0xFF,    0x00,
    0x00,    0xFF,    0x00,    0xFF,
    0x00,    0xFF,    0xF0,    0x0F,
    0x33,    0xCC,    0xAA,    0x55,
    0x00,    0xFF,    0xFF,    0x00,
    0x00,    0xFF,    0x00,    0xFF,
    0x00,    0xFF,    0xF0,    0x0F,
    0x33,    0xCC,    0xAA,    0x55
};

volatile static uint32_t mc_cnt = 0;
volatile static uint32_t l2c_DataUE_cnt = 0, l2c_DataCE_cnt = 0;
volatile static uint32_t l2c_TagUE_cnt = 0, l2c_TagCE_cnt = 0;
static struct rumboot_irq_entry *tbl;

#define L2C_INTERRUPT_MASK ((1 << L2MCKEN_L2AMCK1_i) | (1 << L2MCKEN_L2AMCK2_i) | (1 << L2MCKEN_L1CMCK0_i))

#define L2C_DATA_UE_MASK ((1 << L2ARRSTAT1_DataUE0_i) | (1 << L2ARRSTAT1_DataUE1_i) | \
                          (1 << L2ARRSTAT1_DataUE2_i) | (1 << L2ARRSTAT1_DataUE3_i) | \
                          (1 << L2ARRSTAT1_DataUE4_i) | (1 << L2ARRSTAT1_DataUE5_i) | \
                          (1 << L2ARRSTAT1_DataUE6_i) | (1 << L2ARRSTAT1_DataUE7_i))

#define L2C_DATA_CE_MASK ((1 << L2ARRSTAT2_DataCE0_i) | (1 << L2ARRSTAT2_DataCE1_i) | \
                          (1 << L2ARRSTAT2_DataCE2_i) | (1 << L2ARRSTAT2_DataCE3_i) | \
                          (1 << L2ARRSTAT2_DataCE4_i) | (1 << L2ARRSTAT2_DataCE5_i) | \
                          (1 << L2ARRSTAT2_DataCE6_i) | (1 << L2ARRSTAT2_DataCE7_i))

#define L2C_TAG_UE_MASK  ((1 << L2ARRSTAT1_TagUE0_i) | (1 << L2ARRSTAT1_TagUE1_i) | \
                          (1 << L2ARRSTAT1_TagUE2_i) | (1 << L2ARRSTAT1_TagUE3_i))

#define L2C_TAG_CE_MASK  ((1 << L2ARRSTAT2_TagCE0_i) | (1 << L2ARRSTAT2_TagCE1_i) | \
                          (1 << L2ARRSTAT2_TagCE2_i) | (1 << L2ARRSTAT2_TagCE3_i))

bool check_data_ecc(uint32_t indx)
{
    volatile uint64_t exp_data = data_data[indx];
    volatile uint32_t exp_ecc  = data_ecc[indx];

    volatile uint64_t data,data_err;
    volatile uint32_t ecc,ecc_err;
    uint32_t addr = TEST_ADDR + (indx*8);
    uint32_t error_shift = (indx & 0xF);

    rumboot_printf("------------------------------------\n");
    rumboot_printf("Write address: 0x%x\n", addr);
    rumboot_printf("Write data: 0x%x_%x\n", (uint32_t)(exp_data >>32),(uint32_t)(exp_data & 0xFFFFFFFF));
    iowrite64(exp_data ,addr);
    //isync();

    //check with L2C Array Interface
    uint32_t ext_phys_addr = (uint32_t)(rumboot_virt_to_phys( (void *)addr ) >> 32),
                 phys_addr = (uint32_t)(rumboot_virt_to_phys( (void *)addr ) & 0xFFFFFFFF);

    //get way
    int32_t cache_way = -1;
    if (l2c_arracc_get_way_by_address( DCR_L2C_BASE, ext_phys_addr, phys_addr, &cache_way ) == false)
    {
        rumboot_printf("ERROR: reading (way) via L2ARRACC*\n");
        return false;
    }
    if (l2c_arracc_data_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &data) == false)
    {
        rumboot_printf("ERROR: reading (data) via L2ARRACC*\n");
        return false;
    }
    ecc = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 );

    rumboot_printf("Read raw data: 0x%x_%x\n",(uint32_t)(data >>32),(uint32_t)(data & 0xFFFFFFFF));
    if(exp_data != data)
    {
        rumboot_printf("ERROR: data mismatch!\n");
        return false;
    }
    rumboot_printf("Expected ECC: 0x%x\nRead ECC:     0x%x\n",exp_ecc, ecc);
    if(exp_ecc != ecc)
    {
        rumboot_printf("ERROR: ECC mismatch!\n");
        return false;
    }

    mc_cnt = 0;
    l2c_DataUE_cnt = 0; l2c_DataCE_cnt = 0;
    l2c_TagUE_cnt = 0; l2c_TagCE_cnt = 0;

    rumboot_printf("Insert error to the bit #%d of data and write\n",error_shift);
    data = exp_data ^ (1 << error_shift);
    rumboot_printf("Write raw data: 0x%x_%x\n", (uint32_t)(data >>32),(uint32_t)(data & 0xFFFFFFFF));
    if(l2c_arracc_data_write_by_way_wo_gen_ecc( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, data, exp_ecc) == false)
    {
        rumboot_printf("ERROR: write (data) via L2ARRACC*\n");
        return false;
    }

    if (l2c_arracc_data_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &data_err) == false)
    {
        rumboot_printf("ERROR: reading (data) via L2ARRACC*\n");
        return false;
    }
    ecc_err = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 );
    rumboot_printf("Read raw data: 0x%x_%x\n",(uint32_t)(data_err >>32),(uint32_t)(data_err & 0xFFFFFFFF));
    rumboot_printf("Read ECC: 0x%x\n",ecc_err);

    data = ioread64(addr);
    rumboot_printf("Read data: 0x%x_%x\n",(uint32_t)(data >>32),(uint32_t)(data & 0xFFFFFFFF));
    if(exp_data != data)
    {
        rumboot_printf("ERROR: data mismatch!\n");
        return false;
    }
    if (l2c_arracc_data_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &data) == false)
    {
        rumboot_printf("ERROR: reading (data) via L2ARRACC*\n");
        return false;
    }
    ecc = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 );

    rumboot_printf("Read raw data: 0x%x_%x\n",(uint32_t)(data >>32),(uint32_t)(data & 0xFFFFFFFF));
    rumboot_printf("Read ECC: 0x%x\n", ecc);

//    rumboot_printf("mc_cnt=%d, l2c_DataCE_cnt=%d, l2c_DataUE_cnt=%d, l2c_TagCE_cnt=%d, l2c_TagUE_cnt=%d\n",
//                    mc_cnt,l2c_DataCE_cnt,l2c_DataUE_cnt,l2c_TagCE_cnt,l2c_TagUE_cnt);

    if((mc_cnt == 0) || (l2c_DataCE_cnt == 0)) {
        rumboot_printf("ERROR: some machine check interrupts not occured\n");
        return false;
    }
    if((l2c_DataUE_cnt > 0) || (l2c_TagUE_cnt > 0) || (l2c_TagCE_cnt > 0)) {
        rumboot_printf("ERROR: Unexpected L2C machine check interrupt\n");
        return false;
    }

    mc_cnt = 0;
    l2c_DataUE_cnt = 0; l2c_DataCE_cnt = 0;
    l2c_TagUE_cnt = 0; l2c_TagCE_cnt = 0;

    rumboot_printf("Insert error to bits #%d and #%d of data and write\n",error_shift,error_shift+1);
    data = exp_data ^ (3 << error_shift);
    rumboot_printf("Write raw data: 0x%x_%x\n", (uint32_t)(data >>32),(uint32_t)(data & 0xFFFFFFFF));
    if(l2c_arracc_data_write_by_way_wo_gen_ecc( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, data, exp_ecc) == false)
    {
        rumboot_printf("ERROR: write (data) via L2ARRACC*\n");
        return false;
    }
    if (l2c_arracc_data_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &data_err) == false)
    {
        rumboot_printf("ERROR: reading (data) via L2ARRACC*\n");
        return false;
    }
    ecc_err = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 );
    rumboot_printf("Read raw data: 0x%x_%x\n",(uint32_t)(data_err >>32),(uint32_t)(data_err & 0xFFFFFFFF));
    rumboot_printf("Read ECC: 0x%x\n",ecc_err);

    data = ioread64(addr);
    rumboot_printf("Read data: 0x%x_%x\n",(uint32_t)(data >>32),(uint32_t)(data & 0xFFFFFFFF));
    if(exp_data == data)
    {
        rumboot_printf("ERROR: data mismatch!\n");
        return false;
    }
    if (l2c_arracc_data_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &data) == false)
    {
        rumboot_printf("ERROR: reading (data) via L2ARRACC*\n");
        return false;
    }
    ecc = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 );

    rumboot_printf("Read raw data: 0x%x_%x\n",(uint32_t)(data >>32),(uint32_t)(data & 0xFFFFFFFF));
    rumboot_printf("Read ECC: 0x%x\n", ecc);

//    rumboot_printf("mc_cnt=%d, l2c_DataCE_cnt=%d, l2c_DataUE_cnt=%d, l2c_TagCE_cnt=%d, l2c_TagUE_cnt=%d\n",
//                    mc_cnt,l2c_DataCE_cnt,l2c_DataUE_cnt,l2c_TagCE_cnt,l2c_TagUE_cnt);

    if((mc_cnt == 0) || (l2c_DataUE_cnt == 0)) {
        rumboot_printf("ERROR: some machine check interrupts not occured\n");
        return false;
    }
    if((l2c_DataCE_cnt > 0) || (l2c_TagUE_cnt > 0) || (l2c_TagCE_cnt > 0)) {
        rumboot_printf("ERROR: Unexpected L2C machine check interrupt\n");
        return false;
    }

    rumboot_printf("Remove error from data and write\n");
    rumboot_printf("Write raw data: 0x%x_%x\n", (uint32_t)(exp_data >>32),(uint32_t)(exp_data & 0xFFFFFFFF));
    if(l2c_arracc_data_write_by_way_wo_gen_ecc( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, exp_data, exp_ecc) == false)
    {
        rumboot_printf("ERROR: write (data) via L2ARRACC*\n");
        return false;
    }
    if (l2c_arracc_data_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &data_err) == false)
    {
        rumboot_printf("ERROR: reading (data) via L2ARRACC*\n");
        return false;
    }
    exp_ecc = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 );
    rumboot_printf("Read raw data: 0x%x_%x\n",(uint32_t)(data_err >>32),(uint32_t)(data_err & 0xFFFFFFFF));
    rumboot_printf("Read ECC: 0x%x\n",ecc_err);
    if(exp_data == data_err)
    {
        rumboot_printf("ERROR: data mismatch!\n");
        return false;
    }
    if(exp_ecc == exp_ecc)
    {
        rumboot_printf("ERROR: ECC mismatch!\n");
        return false;
    }

    return true;
}

bool check_tag_ecc(uint32_t indx)
{
    volatile uint32_t ecc,state_tag;
    volatile uint32_t ecc_err,state_tag_err;
    volatile uint32_t exp_ecc   = tag_ecc[indx];
    volatile uint32_t exp_tag   = tag_data[indx];
    uint32_t error_shift = (indx & 0xF);

    rumboot_printf("------------------------------------\n");
    rumboot_printf("Write tag: 0x%x\n", exp_tag);

    uint32_t exp_ext_phys_addr = tag_data[indx] >> 16,
                 exp_phys_addr = (tag_data[indx] & 0xFFFF) << 16;

    //only for ERPN=0. need fix for any
    uint32_t addr = (tag_data[indx] & 0xFFFF) << 16;

    uint32_t ext_phys_addr = (uint32_t)(rumboot_virt_to_phys( (void *)addr ) >> 32),
                 phys_addr = (uint32_t)(rumboot_virt_to_phys( (void *)addr ) & 0xFFFFFFFF);

    rumboot_printf("virtual address  = 0x%x\n", addr);
    rumboot_printf("physical address = 0x%x_%x (exp)\n", exp_ext_phys_addr, exp_phys_addr);
    rumboot_printf("physical address = 0x%x_%x (virt2phys)\n", ext_phys_addr, phys_addr);

    rumboot_memfill8_modelling((void*)addr, 0x8000, 0x00, 0x00); //workaround (init 32KB SRAM0)

    rumboot_printf("write 0x%x to addr 0x%x\n", TEST_DATA, addr);
    iowrite32(TEST_DATA, addr);
    //read and check
    uint32_t data = ioread32(addr);
    msync();
    rumboot_printf("read  0x%x from addr 0x%x\n", data, addr);
    if (data != TEST_DATA)
    {
        rumboot_printf("ERROR: data mismatch!\n");
        return false;
    }

    //get way
    int32_t cache_way = -1;
    if (l2c_arracc_get_way_by_address( DCR_L2C_BASE, ext_phys_addr, phys_addr, &cache_way ) == false)
    {
        rumboot_printf("ERROR: reading (way) via L2ARRACC*\n");
        return false;
    }
    rumboot_printf("cache way = %x\n", cache_way);

    uint32_t exp_state_tag = (TEST_STATE << 29) | (exp_tag << 3);
    //get tag
    if (l2c_arracc_tag_info_read_by_way (DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &state_tag) == false)
    {
        rumboot_printf("ERROR: reading (tag) via L2ARRACC*\n");
        return false;
    }
    ecc = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 ) >> 1;

    rumboot_printf("state & tag = %x (exp)\nstate & tag = %x\n", exp_state_tag, state_tag);
    if (state_tag != exp_state_tag)
    {
        rumboot_printf("ERROR: invalid tag\n");
        return false;
    }

    rumboot_printf("ECC = 0x%x (exp)\nECC = 0x%x\n",exp_ecc, ecc);
    if(exp_ecc != ecc)
    {
        rumboot_printf("ERROR: ECC mismatch!\n");
        return false;
    }

    mc_cnt = 0;
    l2c_DataUE_cnt = 0; l2c_DataCE_cnt = 0;
    l2c_TagUE_cnt = 0; l2c_TagCE_cnt = 0;

    rumboot_printf("Insert error to the bit #%d of tag and write\n",error_shift);
    state_tag = exp_state_tag ^ (1 << (error_shift+3));
    rumboot_printf("Write raw state & tag: 0x%x\n", state_tag);
    if(l2c_arracc_tag_info_write_by_way_wo_gen_ecc( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, state_tag, exp_ecc << 1) == false)
    {
        rumboot_printf("ERROR: write (tag) via L2ARRACC*\n");
        return false;
    }

    if (l2c_arracc_tag_info_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &state_tag_err) == false)
    {
        rumboot_printf("ERROR: reading (tag) via L2ARRACC*\n");
        return false;
    }
    ecc_err = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 ) >> 1;
    rumboot_printf("Read raw state & tag: 0x%x\n",state_tag_err);
    rumboot_printf("Read ECC: 0x%x\n",ecc_err);

    data = ioread32(addr);
    rumboot_printf("Read data: 0x%x\n",data);
    if (data != TEST_DATA)
    {
        rumboot_printf("ERROR: data mismatch!\n");
        return false;
    }
    if (l2c_arracc_tag_info_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &state_tag) == false)
    {
        rumboot_printf("ERROR: reading (tag) via L2ARRACC*\n");
        return false;
    }
    ecc = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 ) >> 1;

    rumboot_printf("Read raw state & tag: 0x%x\n",state_tag);
    rumboot_printf("Read ECC: 0x%x\n", ecc);

 //   rumboot_printf("mc_cnt=%d, l2c_DataCE_cnt=%d, l2c_DataUE_cnt=%d, l2c_TagCE_cnt=%d, l2c_TagUE_cnt=%d\n",
 //                   mc_cnt,l2c_DataCE_cnt,l2c_DataUE_cnt,l2c_TagCE_cnt,l2c_TagUE_cnt);

    if((mc_cnt == 0) || (l2c_TagCE_cnt == 0)) {
        rumboot_printf("ERROR: some machine check interrupts not occured\n");
        return false;
    }
    if((l2c_DataUE_cnt > 0) || (l2c_DataCE_cnt > 0) || (l2c_TagUE_cnt > 0)) {
        rumboot_printf("ERROR: Unexpected L2C machine check interrupt\n");
        return false;
    }

    mc_cnt = 0;
    l2c_DataUE_cnt = 0; l2c_DataCE_cnt = 0;
    l2c_TagUE_cnt = 0; l2c_TagCE_cnt = 0;

    rumboot_printf("Insert error to bits #%d and #%d of tag and write\n",error_shift,error_shift+1);
    state_tag = exp_state_tag ^ (3 << (error_shift+3));
    rumboot_printf("Write raw state & tag: 0x%x\n", state_tag);
    if(l2c_arracc_tag_info_write_by_way_wo_gen_ecc( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, state_tag, exp_ecc << 1) == false)
    {
        rumboot_printf("ERROR: write (tag) via L2ARRACC*\n");
        return false;
    }

    if (l2c_arracc_tag_info_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &state_tag_err) == false)
    {
        rumboot_printf("ERROR: reading (tag) via L2ARRACC*\n");
        return false;
    }
    ecc_err = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 ) >> 1;
    rumboot_printf("Read raw state & tag: 0x%x\n",state_tag_err);
    rumboot_printf("Read ECC: 0x%x\n",ecc_err);

    data = ioread32(addr);
    rumboot_printf("Read data: 0x%x\n",data);
    if (data == TEST_DATA)
    {
        rumboot_printf("ERROR: data mismatch!\n");
        return false;
    }
    if (l2c_arracc_tag_info_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &state_tag) == false)
    {
        rumboot_printf("ERROR: reading (tag) via L2ARRACC*\n");
        return false;
    }
    ecc = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 ) >> 1;

    rumboot_printf("Read raw state & tag: 0x%x\n",state_tag);
    rumboot_printf("Read ECC: 0x%x\n", ecc);

//    rumboot_printf("mc_cnt=%d, l2c_DataCE_cnt=%d, l2c_DataUE_cnt=%d, l2c_TagCE_cnt=%d, l2c_TagUE_cnt=%d\n",
//                    mc_cnt,l2c_DataCE_cnt,l2c_DataUE_cnt,l2c_TagCE_cnt,l2c_TagUE_cnt);

    if((mc_cnt == 0) || (l2c_TagUE_cnt == 0)) {
        rumboot_printf("ERROR: some machine check interrupts not occured\n");
        return false;
    }
    if((l2c_DataUE_cnt > 0) || (l2c_DataCE_cnt > 0) || (l2c_TagCE_cnt > 0)) {
        rumboot_printf("ERROR: Unexpected L2C machine check interrupt\n");
        return false;
    }

    rumboot_printf("Remove errors from tag and write\n");
    rumboot_printf("Write raw state & tag: 0x%x\n", exp_state_tag);
    if(l2c_arracc_tag_info_write_by_way_wo_gen_ecc( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, exp_state_tag, exp_ecc << 1) == false)
    {
        rumboot_printf("ERROR: write (tag) via L2ARRACC*\n");
        return false;
    }

    if (l2c_arracc_tag_info_read_by_way( DCR_L2C_BASE, ext_phys_addr, phys_addr, cache_way, &state_tag_err) == false)
    {
        rumboot_printf("ERROR: reading (tag) via L2ARRACC*\n");
        return false;
    }
    ecc_err = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRACCDO2 ) >> 1;
    rumboot_printf("Read raw state & tag: 0x%x\n",state_tag_err);
    rumboot_printf("Read ECC: 0x%x\n",ecc_err);
    if(exp_state_tag == state_tag_err)
    {
        rumboot_printf("ERROR: state & tag mismatch!\n");
        return false;
    }
    if(exp_ecc == exp_ecc)
    {
        rumboot_printf("ERROR: ECC mismatch!\n");
        return false;
    }

    return true;
}

static void exception_handler(int const id, char const * const name ) {
    //rumboot_printf( "Exception: %s\n", name );
    TEST_ASSERT( (id == RUMBOOT_IRQ_MACHINE_CHECK)
              && (spr_read(SPR_MCSR_RW) == ( (0b1 << ITRPT_MCSR_MCS_i)
                                           | (0b1 << ITRPT_MCSR_L2_i) )),
                 "Unexpected exception" );
    mc_cnt += 1;
    spr_write( SPR_MCSR_C, spr_read(SPR_MCSR_RW) );
}

static void l2c0_mchkout_handler(int irq, void *args) {
    //rumboot_printf("l2c0_mchkout_handler\n");
    uint32_t L2MCK = l2c_l2_read( DCR_L2C_BASE, L2C_L2MCK );

    if(L2MCK & (~L2C_INTERRUPT_MASK)) rumboot_printf("Unexpected L2C interrupt (L2MCK = 0x%x)\n", L2MCK);
    TEST_ASSERT( !(L2MCK & (~L2C_INTERRUPT_MASK)), "Unexpected L2C interrupt" );

    if(L2MCK & (1 << L2MCKEN_L2AMCK1_i)) {
        uint32_t L2ARRSTAT1 = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRSTAT1 );
        if(L2ARRSTAT1 & L2C_DATA_UE_MASK) l2c_DataUE_cnt += 1;
        if(L2ARRSTAT1 & L2C_TAG_UE_MASK) l2c_TagUE_cnt += 1;
        l2c_l2_write( DCR_L2C_BASE, L2C_L2ARRSTAT1, L2ARRSTAT1);
        //rumboot_printf("L2ARRSTAT1 = 0x%x\n", L2ARRSTAT1);
    }
    if(L2MCK & (1 << L2MCKEN_L2AMCK2_i)) {
        uint32_t L2ARRSTAT2 = l2c_l2_read( DCR_L2C_BASE, L2C_L2ARRSTAT2 );
        if(L2ARRSTAT2 & L2C_DATA_CE_MASK) l2c_DataCE_cnt += 1;
        if(L2ARRSTAT2 & L2C_TAG_CE_MASK) l2c_TagCE_cnt += 1;
        l2c_l2_write( DCR_L2C_BASE, L2C_L2ARRSTAT2, L2ARRSTAT2);
        //rumboot_printf("L2ARRSTAT2 = 0x%x\n", L2ARRSTAT2);
    }
    if(L2MCK & (1 << L2MCKEN_L1CMCK0_i)) {
        uint32_t L2CPUSTAT = l2c_l2_read( DCR_L2C_BASE, L2C_L2CPUSTAT );
        l2c_l2_write( DCR_L2C_BASE, L2C_L2CPUSTAT, L2CPUSTAT);
        //rumboot_printf("L2CPUSTAT = 0x%x\n", L2CPUSTAT);
    }
}

static void L2C_InterruptEnable() {

l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRMCKEN1, ( 1 << L2ARRMCKEN1_TagUE0_i )  | ( 1 << L2ARRMCKEN1_TagUE1_i )
                                          | ( 1 << L2ARRMCKEN1_TagUE2_i )  | ( 1 << L2ARRMCKEN1_TagUE3_i )
                                          | ( 1 << L2ARRMCKEN1_DataUE0_i ) | ( 1 << L2ARRMCKEN1_DataUE1_i )
                                          | ( 1 << L2ARRMCKEN1_DataUE2_i ) | ( 1 << L2ARRMCKEN1_DataUE3_i )
                                          | ( 1 << L2ARRMCKEN1_DataUE4_i ) | ( 1 << L2ARRMCKEN1_DataUE5_i )
                                          | ( 1 << L2ARRMCKEN1_DataUE6_i ) | ( 1 << L2ARRMCKEN1_DataUE7_i ));

l2c_l2_write(DCR_L2C_BASE, L2C_L2ARRMCKEN2, ( 1 << L2ARRMCKEN2_TagCE0_i )  | ( 1 << L2ARRMCKEN2_TagCE1_i )
                                          | ( 1 << L2ARRMCKEN2_TagCE2_i )  | ( 1 << L2ARRMCKEN2_TagCE3_i )
                                          | ( 1 << L2ARRMCKEN2_DataCE0_i ) | ( 1 << L2ARRMCKEN2_DataCE1_i )
                                          | ( 1 << L2ARRMCKEN2_DataCE2_i ) | ( 1 << L2ARRMCKEN2_DataCE3_i )
                                          | ( 1 << L2ARRMCKEN2_DataCE4_i ) | ( 1 << L2ARRMCKEN2_DataCE5_i )
                                          | ( 1 << L2ARRMCKEN2_DataCE6_i ) | ( 1 << L2ARRMCKEN2_DataCE7_i ));

l2c_l2_write(DCR_L2C_BASE, L2C_L2CPUMCKEN,  ( 1 << L2CPUMCKEN_DrReqPE0_i )  | ( 1 << L2CPUMCKEN_DrReqPE1_i ) | ( 1 << L2CPUMCKEN_DrReqPE2_i )
                                          | ( 1 << L2CPUMCKEN_DwReqPE0_i )  | ( 1 << L2CPUMCKEN_DwReqPE1_i ) | ( 1 << L2CPUMCKEN_DwReqPE2_i )
                                          | ( 1 << L2CPUMCKEN_DwDataPE0_i ) | ( 1 << L2CPUMCKEN_DwDataPE1_i )
                                          | ( 1 << L2CPUMCKEN_DwDataPE2_i ) | ( 1 << L2CPUMCKEN_DwDataPE3_i ));
}

int main()
{
    uint32_t i;

    rumboot_printf("Start test_oi10_cpu_021_ecc.\n");
    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x8000, 0x00, 0x00); //workaround (init 32KB SRAM0)

    static const tlb_entry tlb_entries[] =
       {
        {TLB_ENTRY0_INV},  {TLB_ENTRY1_INV},
        {TLB_ENTRY0_CACHE},{TLB_ENTRY1_CACHE}
       };

    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Start TLB entries initialization... ");
    write_tlb_entries(tlb_entries, ARRAY_SIZE(tlb_entries));
    msync();
    isync();
    rumboot_printf("done\n");

    if(ARRAY_SIZE(data_data) != ARRAY_SIZE(data_ecc)) {
        rumboot_printf("Sizes of the test data arrays mismatch!\n");
        return 1;
    }
    if(ARRAY_SIZE(tag_data) != ARRAY_SIZE(tag_ecc)) {
        rumboot_printf("Sizes of the test data arrays mismatch!\n");
        return 1;
    }

    rumboot_printf("Set our own irq handlers... ");
    rumboot_irq_set_exception_handler(exception_handler);
    rumboot_irq_cli();
    tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, L2C0_MCHKOUT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, l2c0_mchkout_handler, NULL );
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( L2C0_MCHKOUT );
    L2C_InterruptEnable();
    rumboot_irq_sei();
    rumboot_printf("done\n");

    rumboot_printf("------Start checking data ECC-------\n");
    for (i = 0; i < ARRAY_SIZE(data_data); ++i)
    {
        if(!check_data_ecc(i)){
            rumboot_printf("TEST ERROR\n");
            rumboot_irq_table_activate( NULL );
            rumboot_irq_free( tbl );
            return 1;
        }
    }

    rumboot_printf("------------------------------------\n");
    rumboot_printf("------Start checking tag ECC--------\n");
    for (i = 0; i < ARRAY_SIZE(tag_data); ++i)
    {
        if(!check_tag_ecc(i)){
            rumboot_printf("TEST ERROR\n");
            rumboot_irq_table_activate( NULL );
            rumboot_irq_free( tbl );
            return 1;
        }
    }

    rumboot_printf("------------------------------------\n");
    rumboot_printf("TEST OK\n");
    rumboot_irq_table_activate( NULL );
    rumboot_irq_free( tbl );
    return 0;
}



