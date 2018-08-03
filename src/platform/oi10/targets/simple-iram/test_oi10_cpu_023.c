#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/dma2plb6.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>


//size in bytes
#define TEST_DATA_SIZE 128
//test data content
#define TEST_DATA_CONTENT 0x0123456789ABCDEF

#define SHOW_STATUS(status) switch(status){ \
                case error_alignnment: \
                case error_scatter_alignment: \
                    rumboot_printf("DMA2PLB6: Error alignment\n"); \
                    break; \
                case error_read_data_regular: \
                case error_read_data_scatter_or_resume: \
                    rumboot_printf("DMA2PLB6: Error read data\n"); \
                    break; \
                case error_read_request_regular: \
                case error_read_request_scatter_or_resume: \
                    rumboot_printf("DMA2PLB6: Error read request\n"); \
                    break; \
                case error_write_request: \
                    rumboot_printf("DMA2PLB6: Error write request\n"); \
                    break; \
                }


//IM0, 128-bit alignment (16 bytes)
static uint8_t source_area_im0 [TEST_DATA_SIZE]__attribute__((aligned(16)));
static uint8_t dest_area_im0 [TEST_DATA_SIZE] __attribute__((aligned(16)));


static uint64_t fill_word = TEST_DATA_CONTENT;


uint32_t dma2plb6_get_bytesize(transfer_width transfer_width_code)
{
    switch(transfer_width_code){
    case tr_width_byte:
        return 1;
        break;
    case tr_width_halfword:
        return 2;
        break;
    case tr_width_word:
        return 4;
        break;
    case tr_width_doubleword:
        return 8;
        break;
    case tr_width_quadword:
        return 16;
        break;
    }
    return 0;
}

void fill(uint64_t *s, uint64_t pattern, uint32_t size_in_bytes)
{
    uint32_t i;
    for (i = 0; i < (size_in_bytes>>3); i++)
        s[i] = pattern;
}

uint32_t compare(uint32_t s_addr, uint32_t d_addr,uint32_t size_in_bytes)
{
    uint32_t i;
    for (i = 0; i < size_in_bytes; i+=8){
        if(ioread64(s_addr + i) != ioread64(d_addr + i))
        {
            rumboot_printf("compare failed, expected value: 0x%x\n", ioread64(s_addr + i));
            rumboot_printf("read value: 0x%x\n", ioread64(d_addr + i));
            return false;
        }
    }
    return true;
}

uint32_t check_dma2plb6_0_mem_to_mem(uint32_t source_ea, uint32_t dest_ea, uint64_t source_phys, uint64_t dest_phys)
{
    dma2plb6_setup_info dma_info;
    channel_status status = {};
    dma_info.base_addr = DCR_DMAPLB6_BASE;
    dma_info.source_adr = source_phys;
    dma_info.dest_adr = dest_phys;
    dma_info.priority = priority_medium_low;
    dma_info.striding_info.striding = striding_none;
    dma_info.tc_int_enable = false;
    dma_info.err_int_enable = false;
    dma_info.int_mode = int_mode_level_wait_clr;
    dma_info.channel = channel0;
    for (dma_info.rw_transfer_size = rw_tr_size_1q; dma_info.rw_transfer_size <= rw_tr_size_8q; dma_info.rw_transfer_size++)
    {
        rumboot_printf("rw_transfer_size = 0x%x\n", dma_info.rw_transfer_size);
        for(dma_info.transfer_width = tr_width_byte; dma_info.transfer_width <= tr_width_quadword; dma_info.transfer_width++)
        {
            fill((uint64_t *)source_ea, fill_word, TEST_DATA_SIZE);
            fill((uint64_t *)dest_ea, 0x0, TEST_DATA_SIZE);
            dma_info.count = TEST_DATA_SIZE/dma2plb6_get_bytesize(dma_info.transfer_width);
            if(dma2plb6_single_copy(&dma_info, &status) == false){
                SHOW_STATUS(status.spec_error_status);
                return false;
            } else {
                TEST_ASSERT(compare(source_ea, dest_ea, TEST_DATA_SIZE) == true, "Memory comparison failed");
                fill_word = ~fill_word;
                rumboot_printf("done: tr_width = 0x%x\n", dma_info.transfer_width);
            }
        }
    }
    return true;
}


int main(void)
{
    volatile int64_t src_im0_physical, dst_im0_physical;
    volatile int64_t src_im1_physical, dst_im1_physical;
    volatile int64_t src_em2_physical, dst_em2_physical;

    emi_init();

    //prepare physical address
    set_mem_window(MEM_WINDOW_SHARED);//WORKAROUND
    src_im0_physical = get_physical_addr((uint32_t)source_area_im0, 0);
    dst_im0_physical = get_physical_addr((uint32_t)dest_area_im0, 0);

    set_mem_window(MEM_WINDOW_0);//WORKAROUND
    src_im1_physical = get_physical_addr(IM1_BASE, 0);
    dst_im1_physical = src_im1_physical;

    src_em2_physical = get_physical_addr(EM2_BASE, 0);
    dst_em2_physical = src_em2_physical;

    TEST_ASSERT(src_im0_physical >=0,"IM0 src addr is not presented in UTLB");
    TEST_ASSERT(dst_im0_physical >=0,"IM0 dst addr is not presented in UTLB");
    TEST_ASSERT(src_em2_physical >=0,"EM2 src/dst addr is not presented in UTLB");


   rumboot_printf("im0->im0\n");
   TEST_ASSERT(check_dma2plb6_0_mem_to_mem((uint32_t)source_area_im0,
                                           (uint32_t)dest_area_im0,
                                           src_im0_physical,
                                           dst_im0_physical) == true, "IM0-to-IM0 failed");

   rumboot_printf("im0->im1\n");
   TEST_ASSERT(check_dma2plb6_0_mem_to_mem((uint32_t)source_area_im0,
                                           IM1_BASE,
                                           src_im0_physical,
                                           dst_im1_physical) == true, "IM0-to-IM1 failed");

   rumboot_printf("im1->im0\n");
   TEST_ASSERT(check_dma2plb6_0_mem_to_mem(IM1_BASE,
                                           (uint32_t)dest_area_im0,
                                           dst_im1_physical,
                                           dst_im0_physical) == true, "IM1-to-IM0 failed");


   rumboot_printf("em2->im0\n");
   TEST_ASSERT(check_dma2plb6_0_mem_to_mem(EM2_BASE,
                                           (uint32_t)dest_area_im0,
                                           src_em2_physical,
                                           dst_im0_physical) == true, "EM2-to-IM0 failed");


   rumboot_printf("im0->em2\n");
   TEST_ASSERT(check_dma2plb6_0_mem_to_mem((uint32_t)source_area_im0,
                                           EM2_BASE,
                                           src_im0_physical,
                                           dst_em2_physical) == true, "IM0-to-EM2 failed");

   rumboot_printf("em2->im1\n");
   TEST_ASSERT(check_dma2plb6_0_mem_to_mem(EM2_BASE,
                                           IM1_BASE,
                                           src_em2_physical,
                                           dst_im1_physical) == true, "EM2-to-IM1 failed");

   rumboot_printf("im1->em2\n");
   TEST_ASSERT(check_dma2plb6_0_mem_to_mem(IM1_BASE,
                                           EM2_BASE,
                                           src_im1_physical,
                                           dst_em2_physical) == true, "IM1-to-EM2 failed");

    return 0;
}
