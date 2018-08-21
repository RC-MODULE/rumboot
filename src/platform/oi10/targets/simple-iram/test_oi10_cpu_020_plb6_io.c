/*
 * test_oi10_cpu_020_plb6_io.c
 *
 *  Created on: Aug 20, 2018
 *      Author: s.chernousov
 */

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
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/devices/emi.h>
#include <platform/devices/dma2plb6.h>
#include <platform/regs/regs_plb4plb6.h>
#include <platform/regs/regs_plb4ahb.h>
#include <platform/devices/l2c.h>

/* HEADER */

#define TEST_VALUE_CLEAR                                0x0
#define TEST_VALUE_DMA0_FULL_L2C_LINE                   0x11111111
#define TEST_VALUE_DMA0_QWORD                           0x22222222
#define L2C_TAG_CACHE_STATE_INVALID                     0x0
#define L2C_TAG_CACHE_STATE_FROM_ARRACCDO0(val)         ( ((val) >> IBM_BIT_INDEX(32, 2)) & 0x7)

typedef enum {
    test_io10_cpu_020_plb6_io_full_l2c_line_size = 128,
    test_io10_cpu_020_plb6_io_half_l2c_line_size = 64,
    test_io10_cpu_020_plb6_io_qword_size = 16
}test_io10_cpu_020_plb6_io_data_size;

/* END HEADER */

static void cache_and_modify_data(uint32_t test_value, uint32_t source, uint32_t target)
{
    uint32_t i;
    rumboot_printf("cache_and_modify_data with value: %x\n", test_value);
    for (i = 0; i < 128; i += 4) {
        iowrite32(test_value, source + i);
        TEST_ASSERT((ioread32 (source + i) == test_value),"Data from memory do not correspond to the data sent to memory");
    }
    for (i = 0; i < 128; i += 4) {
        iowrite32(TEST_VALUE_CLEAR, target + i);
        TEST_ASSERT((ioread32(target + i) == TEST_VALUE_CLEAR),"Data from memory do not correspond to the data sent to memory");
    }
    msync();
}

static void check_values(uint32_t check_value, test_io10_cpu_020_plb6_io_data_size data_size, uint32_t target) {
    uint32_t i;
    rumboot_printf("check_values with value: %x\n", check_value);
    rumboot_printf("size: %x\n", data_size);
    for (i = 0; i < data_size; i += 4)
        COMPARE_VALUES(ioread32(target + i), check_value, "Incorrect data were read!");
}

inline void dma2plb6_trace_status(
        channel_status status) {
    switch (status.spec_error_status) {
    case error_alignnment:
    case error_scatter_alignment:
        rumboot_printf("DMA2PLB6: Error alignment\n");
        break;
    case error_read_data_regular:
    case error_read_data_scatter_or_resume:
        rumboot_printf("DMA2PLB6: Error read data\n");
        break;
    case error_read_request_regular:
    case error_read_request_scatter_or_resume:
        rumboot_printf("DMA2PLB6: Error read request\n");
        break;
    case error_write_request:
        rumboot_printf("DMA2PLB6: Error write request\n");
        break;
    default:
        rumboot_printf("DMA2PLB6: Unexpected status\n");
    }
}

static void test_procedure_with_dma(uint32_t dma_base, test_io10_cpu_020_plb6_io_data_size data_size, uint32_t source, uint32_t target)
{
    uint64_t src = 0;
    uint64_t dst = 0;

    dma2plb6_setup_info dma_info;
    channel_status dma_status = { };
    transfer_width curent_width = tr_width_quadword;

    src = rumboot_virt_to_dma( (volatile void *) source);
    dst = rumboot_virt_to_dma( (volatile void *) target);

    dma_info.base_addr = dma_base;
    dma_info.source_adr = src;
    dma_info.dest_adr = dst;
    dma_info.priority = priority_medium_low;
    dma_info.striding_info.striding = striding_none;
    dma_info.tc_int_enable = false;
    dma_info.err_int_enable = false;
    dma_info.int_mode = int_mode_level_wait_clr;
    dma_info.channel = channel0;
    dma_info.transfer_width = curent_width;
    dma_info.rw_transfer_size = rw_tr_size_8q;
    dma_info.snp_mode = snp_mode_src_dst;

    dma_info.count = (data_size >> curent_width) + 1;

    rumboot_printf("DMA prepared, starting copy.\n");
    if (dma2plb6_single_copy_coherency_required(&dma_info, &dma_status) == false)
    {
        dma2plb6_trace_status(dma_status);
        TEST_ASSERT(0, "DMA copy failed.\n");
    }
    rumboot_printf("DMA copy completed.\n");
}

static void  clear_test_fields (uint32_t clear_value, uint32_t source, uint32_t target)
{
    uint32_t i;
    rumboot_printf("clear_test_fields with value: %x\n", clear_value);
    for (i = 0; i < 128; i += 4)
    {
        iowrite32(clear_value, (source + i));
        TEST_ASSERT((ioread32(source + i) == clear_value),"Data from memory do not correspond to the data sent to memory");
    }
    for (i = 0; i < 128; i += 4)
    {
        iowrite32(clear_value, target + i);
        TEST_ASSERT((ioread32(target + i) == clear_value),"Data from memory do not correspond to the data sent to memory");
    }
    msync();
}

static void prepare_snoop_ready_window_in_PLB6_address_space(uint32_t base_address)
{
    uint32_t value;
    //addresses via PLB4PLB6_1
    dcr_write(base_address + SNOOP_LADDRL0, 0x00000000);
    dcr_write(base_address + SNOOP_UADDRL0, 0x00000000);
    dcr_write(base_address + SNOOP_LADDRH0, 0xFFFFFFFF);
    dcr_write(base_address + SNOOP_UADDRH0, 0x00000003);
    value = dcr_read(base_address + P46CR);
    value |= (1 << IBM_BIT_INDEX(32, 23));
    dcr_write(base_address + P46CR, value);
    msync();
}

static void  prepare_plb4xahb_2_upper_address_bits(uint32_t base_address, uint32_t upper_address_bits)
{
    uint32_t data = 0;
    data = dcr_read(base_address + CONTROL);
    data &= (7 << 3);
    data |= ((upper_address_bits & 7) << 3);
    dcr_write(base_address + CONTROL, data);
}

void check_target_is_invalidated_in_l2c (uint32_t l2c_base, uint32_t target, bool assert)
{
    uint64_t physical_addr;
    volatile uint32_t tag_data;
    volatile int32_t  way;
    rumboot_printf("check_target_is_invalidated_in_l2c\n");
    physical_addr = get_physical_addr (target, 0);
    rumboot_printf("physical_addr low == %x\n", (uint32_t)(physical_addr & 0xFFFFFFFF));
    rumboot_printf("physical_addr high == %x\n", (uint32_t)((physical_addr >> 32) & 0xFFFFFFFF));
    msync();
    if( l2c_arracc_get_way_by_address(l2c_base, (uint32_t)(physical_addr >> 32), (uint32_t)(physical_addr & 0xFFFFFFFF), &way) )
    {
        if( l2c_arracc_tag_info_read_by_way(
                l2c_base,
                (uint32_t)(physical_addr >> 32),
                (uint32_t)(physical_addr & 0xFFFFFFFF),
                way,
                &tag_data) )
        {
            rumboot_printf("tag_data == %x\n", tag_data);
            if (assert)
            {
                TEST_ASSERT((L2C_TAG_CACHE_STATE_FROM_ARRACCDO0(tag_data) == L2C_TAG_CACHE_STATE_INVALID),"Target area was not invalidated!\n");
            }
            else
                if (L2C_TAG_CACHE_STATE_FROM_ARRACCDO0(tag_data) != L2C_TAG_CACHE_STATE_INVALID)
                {
                    rumboot_printf("Target data is not invalid for address: %x\n", target);
                }
        }
        else
            rumboot_printf("Target tag was not found in L2C\n");
    }
    else
        rumboot_printf("Target tag way was not found in L2C\n");
}

void rwnitc_check (uint32_t source, uint32_t target, uint32_t xor_invertor)
{
    rumboot_printf("\n\nrwnitc_check\n");
    rumboot_printf("source == %x\n", source);
    rumboot_printf("target == %x\n", target);
    rumboot_printf("xor_invertor == %x\n", xor_invertor);
    cache_and_modify_data((TEST_VALUE_DMA0_FULL_L2C_LINE ^ xor_invertor), source, target);
    test_procedure_with_dma(DCR_DMAPLB6_BASE,
            test_io10_cpu_020_plb6_io_full_l2c_line_size, source, target);
    check_values((TEST_VALUE_DMA0_FULL_L2C_LINE ^ xor_invertor),
            test_io10_cpu_020_plb6_io_full_l2c_line_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    cache_and_modify_data((TEST_VALUE_DMA0_QWORD ^ xor_invertor), source, target);
    test_procedure_with_dma(DCR_DMAPLB6_BASE, test_io10_cpu_020_plb6_io_qword_size, source, target);
    check_values((TEST_VALUE_DMA0_QWORD ^ xor_invertor), test_io10_cpu_020_plb6_io_qword_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);
}

void __attribute__((section(".EM0.text"))) write_check(uint32_t source, uint32_t target, uint32_t xor_invertor)
{
    uint32_t l2c_base = DCR_L2C_BASE;
    rumboot_printf("\n\nwrite_check\n");
    rumboot_printf("source == %x\n", source);
    rumboot_printf("target == %x\n", target);
    rumboot_printf("xor_invertor == %x\n", xor_invertor);
    rumboot_printf("l2c_base == %x\n", l2c_base);

    cache_and_modify_data((TEST_VALUE_DMA0_FULL_L2C_LINE ^ xor_invertor), source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, false);
    test_procedure_with_dma(DCR_DMAPLB6_BASE,
            test_io10_cpu_020_plb6_io_full_l2c_line_size, source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, true);
    check_values((TEST_VALUE_DMA0_FULL_L2C_LINE ^ xor_invertor),
            test_io10_cpu_020_plb6_io_full_l2c_line_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);

    cache_and_modify_data((TEST_VALUE_DMA0_QWORD ^ xor_invertor), source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, false);
    test_procedure_with_dma(DCR_DMAPLB6_BASE, test_io10_cpu_020_plb6_io_qword_size, source, target);
    check_target_is_invalidated_in_l2c(l2c_base, target, true);
    check_values((TEST_VALUE_DMA0_QWORD ^ xor_invertor), test_io10_cpu_020_plb6_io_qword_size, target);
    clear_test_fields(TEST_VALUE_CLEAR, source, target);
}

static void prepare_devices() {

    emi_init(DCR_EM2_EMI_BASE);

    prepare_snoop_ready_window_in_PLB6_address_space(DCR_PLB4PLB6_0_BASE);
    prepare_plb4xahb_2_upper_address_bits(DCR_PLB4AHB_0_BASE, 0x2);
    dci(2);
    msync();
    isync();
}

int main ()
{
    #define source_test_data_rwnitc_sram0 (SRAM0_BASE)
    #define target_test_data_rwnitc_sram0 (SRAM0_BASE + test_io10_cpu_020_plb6_io_full_l2c_line_size)

    #define source_test_data_write_sram1 (SRAM1_BASE)
    #define target_test_data_write_sram1 (SRAM1_BASE + test_io10_cpu_020_plb6_io_full_l2c_line_size)

    rumboot_printf("(uint32_t) source_test_data_rwnitc_SRAM0 == %x\n", (uint32_t) source_test_data_rwnitc_sram0);
    rumboot_printf("(uint32_t) target_test_data_rwnitc_SRAM0 == %x\n", (uint32_t) target_test_data_rwnitc_sram0);

    rumboot_printf("(uint32_t) source_test_data_write_sram1 == %x\n", (uint32_t) source_test_data_write_sram1);
    rumboot_printf("(uint32_t) target_test_data_write_sram1 == %x\n", (uint32_t) target_test_data_write_sram1);

    prepare_devices();

    //SRAM0
    //RWNITC checks
    rumboot_printf("Checks\n");
    rwnitc_check((uint32_t) source_test_data_rwnitc_sram0, (uint32_t)target_test_data_rwnitc_sram0, 0x0);

    //WRITE checks
    write_check((uint32_t) source_test_data_write_sram1, (uint32_t)target_test_data_write_sram1, 0x0);

    return 0;
}
