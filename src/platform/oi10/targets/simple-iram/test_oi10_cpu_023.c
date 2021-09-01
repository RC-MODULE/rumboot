#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <arch/ppc_476fp_mmu.h>
#include <arch/common_macros.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <arch/dma2plb6.h>

#ifndef POWER_TEST
#include <platform/devices/nor_1636RR4.h>
#include <platform/test_assert.h>
#include <platform/test_event_codes.h>
#endif



//size in bytes
#ifndef TEST_DATA_SIZE
#define TEST_DATA_SIZE 128
#endif
//test data content
#define TEST_DATA_CONTENT 0x0123456789ABCDEF


//IM0, 128-bit alignment (16 bytes)
static uint8_t source_area_im0 [TEST_DATA_SIZE]__attribute__((aligned(16)));
static uint8_t dest_area_im0 [TEST_DATA_SIZE] __attribute__((aligned(16)));


static uint64_t fill_word = TEST_DATA_CONTENT;


static uint32_t dma2plb6_get_bytesize(transfer_width transfer_width_code)
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

static struct dma2plb6_setup_info get_default_dma_info(DmaChannel channel, uint32_t* ch_src, uint32_t* ch_dst)
{
    struct dma2plb6_setup_info dma_info;

    dma_info.base_addr = DCR_DMAPLB6_BASE;
    dma_info.source_adr = rumboot_virt_to_phys(ch_src);
    dma_info.dest_adr = rumboot_virt_to_phys(ch_dst);
    dma_info.priority = priority_medium_low;
    dma_info.striding_info.striding = striding_none;
    dma_info.tc_int_enable = false;
    dma_info.err_int_enable = false;
    dma_info.int_mode = int_mode_level_wait_clr;
    dma_info.channel = channel;
    dma_info.transfer_width = tr_width_quadword;
    dma_info.rw_transfer_size = rw_tr_size_8q;
    dma_info.count = TEST_DATA_SIZE/dma2plb6_get_bytesize(dma_info.transfer_width);

    return dma_info;
}


static void fill(uint64_t *s, uint64_t pattern, uint32_t size_in_bytes)
{
    uint32_t i;
    uint32_t addr;

    for (i = 0; i < (size_in_bytes); i+=8)
    {
        addr = (uint32_t)(s) + i;
        iowrite64(pattern, addr);
    }
}

//static void mem_fill(uint32_t* mem, uint64_t pattern, size_t size)
//{
//    size_t index = 0;
//
//    while(index < size)
//    {
//        iowrite64(pattern, (uint32_t)(mem + index/sizeof(mem)));
//        index += 8;
//    }
//}

static uint32_t compare(uint32_t s_addr, uint32_t d_addr,uint32_t size_in_bytes)
{
    uint32_t i;
    for (i = 0; i < size_in_bytes; i+=8){

        uint64_t s_data = ioread64(s_addr + i);
        uint64_t d_data = ioread64(d_addr + i);

        if(s_data != d_data)
        {
            rumboot_printf("Source addr: 0x%x\n", s_addr + i);
            rumboot_printf("Destination addr: 0x%x\n", d_addr + i);
            rumboot_printf("compare failed, expected value: 0x%x%x\n", (uint32_t)(s_data >> 32), (uint32_t)(s_data & 0xFFFFFFFF));
            rumboot_printf("read value: 0x%x%x\n", (uint32_t)(d_data >> 32), (uint32_t)(d_data & 0xFFFFFFFF));
            return false;
        }
    }
    return true;}


#ifndef POWER_TEST

static void nor_fill(uint32_t* mem, uint64_t pattern, size_t size)
{
    size_t index = 0;

    while(index < size)
    {
        nor_write32((pattern >> 32), (uint32_t)(mem + index));
        nor_write32((pattern & 0xffffffff), (uint32_t)(mem + index + 4));
        index += 8;
    }
}

static uint32_t check_dma2plb6_0_mem_to_mem(uint32_t source_ea, uint32_t dest_ea, uint64_t source_phys, uint64_t dest_phys)
{
    struct dma2plb6_setup_info dma_info;
    struct channel_status status = {};
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
            rumboot_printf("transfer_width = 0x%x\n", dma_info.transfer_width);

            fill((uint64_t *)source_ea, fill_word, TEST_DATA_SIZE);
            fill((uint64_t *)dest_ea, 0x0, TEST_DATA_SIZE);

            dma_info.count = TEST_DATA_SIZE/dma2plb6_get_bytesize(dma_info.transfer_width);

            if(dma2plb6_single_copy(&dma_info, &status) == false){
                dma2plb6_trace_error_status(&status);
                return false;
            } else {
                TEST_ASSERT(compare(source_ea, dest_ea, TEST_DATA_SIZE) == true, "Memory comparison failed");
                fill_word = ~fill_word;
//                rumboot_printf("done: tr_width = 0x%x\n", dma_info.transfer_width);
            }
        }
    }
    return true;
}


static uint32_t check_single_channel()
{
    uint32_t src_im0, dst_im0;
    uint32_t src_im1, dst_im1;
    uint32_t src_em2, dst_em2;

    volatile uint64_t src_im0_physical, dst_im0_physical;
    volatile uint64_t src_im1_physical, dst_im1_physical;
    volatile uint64_t src_em2_physical, dst_em2_physical;

    src_im0 = (uint32_t)source_area_im0;
    dst_im0 = (uint32_t)dest_area_im0;

    src_im1 = (IM1_BASE);
    dst_im1 = (IM1_BASE + 0x80);

    src_em2 = (EM2_BASE);
    dst_em2 = (EM2_BASE + 0x80);


    //prepare physical address
    set_mem_window(MEM_WINDOW_SHARED);//WORKAROUND
    src_im0_physical = rumboot_virt_to_phys((uint32_t*)src_im0);
    dst_im0_physical = rumboot_virt_to_phys((uint32_t*)dst_im0);

    set_mem_window(MEM_WINDOW_0);//WORKAROUND
    src_im1_physical = rumboot_virt_to_phys((uint32_t*)src_im1);
    dst_im1_physical = rumboot_virt_to_phys((uint32_t*)dst_im1);

    src_em2_physical = rumboot_virt_to_phys((uint32_t*)src_em2);
    dst_em2_physical = rumboot_virt_to_phys((uint32_t*)dst_em2);

    TEST_ASSERT(src_im0_physical >=0,"IM0 src addr is not presented in UTLB");
    TEST_ASSERT(dst_im0_physical >=0,"IM0 dst addr is not presented in UTLB");
    TEST_ASSERT(src_im1_physical >=0,"IM1 src addr is not presented in UTLB");
    TEST_ASSERT(dst_im1_physical >=0,"IM1 dst addr is not presented in UTLB");
    TEST_ASSERT(src_em2_physical >=0,"EM2 src addr is not presented in UTLB");
    TEST_ASSERT(dst_em2_physical >=0,"EM2 dst addr is not presented in UTLB");


    rumboot_printf("im0->im0\n");
    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_im0,
                                           dst_im0,
                                           src_im0_physical,
                                           dst_im0_physical) == true, "IM0-to-IM0 failed");

    rumboot_printf("im0->im1\n");
    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_im0,
                                           dst_im1,
                                           src_im0_physical,
                                           dst_im1_physical) == true, "IM0-to-IM1 failed");

    rumboot_printf("im1->im0\n");
    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_im1,
                                           dst_im0,
                                           src_im1_physical,
                                           dst_im0_physical) == true, "IM1-to-IM0 failed");

    rumboot_printf("im1->im1\n");
    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_im1,
                                          dst_im1,
                                          src_im1_physical,
                                          dst_im1_physical) == true, "IM1-to-IM1 failed");

    rumboot_printf("em2->im0\n");
    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_em2,
                                           dst_im0,
                                           src_em2_physical,
                                           dst_im0_physical) == true, "EM2-to-IM0 failed");


    rumboot_printf("im0->em2\n");
    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_im0,
                                           dst_em2,
                                           src_im0_physical,
                                           dst_em2_physical) == true, "IM0-to-EM2 failed");

    rumboot_printf("em2->im1\n");
    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_em2,
                                           dst_im1,
                                           src_em2_physical,
                                           dst_im1_physical) == true, "EM2-to-IM1 failed");

    rumboot_printf("im1->em2\n");
    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_im1,
                                           dst_em2,
                                           src_im1_physical,
                                           dst_em2_physical) == true, "IM1-to-EM2 failed");

    rumboot_printf("em2->em2\n");
    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_em2,
                                          dst_em2,
                                          src_em2_physical,
                                          dst_em2_physical) == true, "EM2-to-EM2 failed");

    return 0;
}

static uint32_t check_multiple_channels_4()
{
    uint32_t* ch0_src = (uint32_t*)(SRAM0_BASE);
    uint32_t* ch0_dst = (uint32_t*)(SDRAM_BASE + 0x80);
    uint32_t* ch1_src = (uint32_t*)(SDRAM_BASE);
    uint32_t* ch1_dst = (uint32_t*)(SSRAM_BASE + 0x80);
    uint32_t* ch2_src = (uint32_t*)(SSRAM_BASE);
    uint32_t* ch2_dst = (uint32_t*)(PIPELINED_BASE + 0x80);
    uint32_t* ch3_src = (uint32_t*)(PIPELINED_BASE);
    uint32_t* ch3_dst = (uint32_t*)(SRAM1_BASE + 0x80);

    rumboot_putstring("Initializing memory ...\n");
    fill((uint64_t*)ch0_src, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch0_dst, 0x00, TEST_DATA_SIZE);
    fill((uint64_t*)ch1_src, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch1_dst, 0x00, TEST_DATA_SIZE);
    fill((uint64_t*)ch2_src, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch2_dst, 0x00, TEST_DATA_SIZE);
    fill((uint64_t*)ch3_src, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch3_dst, 0x00, TEST_DATA_SIZE);

    struct dma2plb6_setup_info ch0_dma_info = get_default_dma_info(channel0, ch0_src, ch0_dst);
    struct dma2plb6_setup_info ch1_dma_info = get_default_dma_info(channel1, ch1_src, ch1_dst);
    struct dma2plb6_setup_info ch2_dma_info = get_default_dma_info(channel2, ch2_src, ch2_dst);
    struct dma2plb6_setup_info ch3_dma_info = get_default_dma_info(channel3, ch3_src, ch3_dst);

    struct channel_status ch0_status = {};
    struct channel_status ch1_status = {};
    struct channel_status ch2_status = {};
    struct channel_status ch3_status = {};

    rumboot_putstring("Initializing DMA channels ...\n");
    dma2plb6_mcpy(&ch0_dma_info);
    dma2plb6_mcpy(&ch1_dma_info);
    dma2plb6_mcpy(&ch2_dma_info);
    dma2plb6_mcpy(&ch3_dma_info);

    rumboot_putstring("Waiting for DMA transfer complete ...\n");
    if (wait_dma2plb6_mcpy(&ch0_dma_info, &ch0_status))
    {
        rumboot_putstring("DMA2PLB6: Channel 0 transfer succeeded.\n");
        if (wait_dma2plb6_mcpy(&ch1_dma_info, &ch1_status))
        {
            rumboot_putstring("DMA2PLB6: Channel 1 transfer succeeded.\n");
            if (wait_dma2plb6_mcpy(&ch2_dma_info, &ch2_status))
            {
                rumboot_putstring("DMA2PLB6: Channel 2 transfer succeeded.\n");
                if (wait_dma2plb6_mcpy(&ch3_dma_info, &ch3_status))
                {
                    rumboot_putstring("DMA2PLB6: Channel 3 transfer succeeded.\n");
                }
                else
                {
                    rumboot_putstring("DMA2PLB6: Channel 3 transfer error.\n");
                    return 1;
                }
            }
            else
            {
                rumboot_putstring("DMA2PLB6: Channel 2 transfer error.\n");
                return 1;
            }
        }
        else
        {
            rumboot_putstring("DMA2PLB6: Channel 1 transfer error.\n");
            return 1;
        }
    }
    else
    {
        rumboot_putstring("DMA2PLB6: Channel 0 transfer error.\n");
        return 1;
    }

    rumboot_putstring("Memory check ...\n");
    TEST_ASSERT(compare((uint32_t)ch0_src, (uint32_t)ch0_dst, TEST_DATA_SIZE) == true, "Channel0: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch1_src, (uint32_t)ch1_dst, TEST_DATA_SIZE) == true, "Channel1: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch2_src, (uint32_t)ch2_dst, TEST_DATA_SIZE) == true, "Channel2: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch3_src, (uint32_t)ch3_dst, TEST_DATA_SIZE) == true, "Channel3: memory comparison failed");

    return 0;
}

static uint32_t check_multiple_channels_2()
{
    uint32_t* ch0_src = (uint32_t*)(SRAM1_BASE);
    uint32_t* ch0_dst = (uint32_t*)(SRAM0_BASE + 0x80);
    uint32_t* ch1_src = (uint32_t*)(NOR_BASE);
    uint32_t* ch1_dst = (uint32_t*)(SRAM0_BASE + 0x100);

    rumboot_putstring("Initializing memory ...\n");
    fill((uint64_t*)ch0_src, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch0_dst, 0x00, TEST_DATA_SIZE);
    nor_fill((uint32_t*)ch1_src, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch1_dst, 0x00, TEST_DATA_SIZE);

    struct dma2plb6_setup_info ch0_dma_info = get_default_dma_info(channel0, ch0_src, ch0_dst);
    struct dma2plb6_setup_info ch1_dma_info = get_default_dma_info(channel1, ch1_src, ch1_dst);

    struct channel_status ch0_status = {};
    struct channel_status ch1_status = {};

    rumboot_putstring("Initializing DMA channels ...\n");
    dma2plb6_mcpy(&ch0_dma_info);
    dma2plb6_mcpy(&ch1_dma_info);

    rumboot_putstring("Waiting for DMA transfer complete ...\n");
    if (wait_dma2plb6_mcpy(&ch0_dma_info, &ch0_status))
    {
        rumboot_putstring("DMA2PLB6: Channel 0 transfer succeeded.\n");
        if (wait_dma2plb6_mcpy(&ch1_dma_info, &ch1_status))
        {
            rumboot_putstring("DMA2PLB6: Channel 1 transfer succeeded.\n");
        }
        else
        {
            rumboot_putstring("DMA2PLB6: Channel 1 transfer error.\n");
            return 1;
        }
    }
    else
    {
        rumboot_putstring("DMA2PLB6: Channel 0 transfer error.\n");
        return 1;
    }

    rumboot_putstring("Memory check ...\n");
    TEST_ASSERT(compare((uint32_t)ch0_src, (uint32_t)ch0_dst, TEST_DATA_SIZE) == true, "Channel0: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch1_src, (uint32_t)ch1_dst, TEST_DATA_SIZE) == true, "Channel1: memory comparison failed");

    return 0;
}
#endif

//////
// Functions for o32t power test (test_oi10_hscb_com_fpu_simult_big.c)

uint32_t* ch0_src_p;
uint32_t* ch0_dst_p;
uint32_t* ch1_src_p;
uint32_t* ch1_dst_p;
uint32_t* ch2_src_p;
uint32_t* ch2_dst_p;
uint32_t* ch3_src_p;
uint32_t* ch3_dst_p;

struct channel_status ch0_status_p = {};
struct channel_status ch1_status_p = {};
struct channel_status ch2_status_p = {};
struct channel_status ch3_status_p = {};

struct dma2plb6_setup_info ch0_dma_info_p;
struct dma2plb6_setup_info ch1_dma_info_p;
struct dma2plb6_setup_info ch2_dma_info_p;
struct dma2plb6_setup_info ch3_dma_info_p;

void dma2plb6_4ch_init()
{
       
    uint32_t* ch0_src_p = (uint32_t*)(IM0_BASE + 0x10000); // 64KB
    uint32_t* ch0_dst_p = (uint32_t*)(IM0_BASE + 0x10000 + 0x2000); // (64 + 8)KB
    uint32_t* ch1_src_p = (uint32_t*)(IM0_BASE + 0x10000 + 0x4000); // (64 + 16)KB
    uint32_t* ch1_dst_p = (uint32_t*)(IM0_BASE + 0x10000 + 0x6000); // (64 + 24)KB
    uint32_t* ch2_src_p = (uint32_t*)(IM0_BASE + 0x10000 + 0x8000); // (64 + 32)KB
    uint32_t* ch2_dst_p = (uint32_t*)(IM0_BASE + 0x10000 + 0xa000); // (64 + 40)KB
    uint32_t* ch3_src_p = (uint32_t*)(IM0_BASE + 0x10000 + 0xc000); // (64 + 48)KB
    uint32_t* ch3_dst_p = (uint32_t*)(IM0_BASE + 0x10000 + 0xe000); // (64 + 56)KB

    rumboot_putstring("DMA2PLB6: Initializing memory ...\n");
    fill((uint64_t*)ch0_src_p, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch0_dst_p, 0x00, TEST_DATA_SIZE);
    fill((uint64_t*)ch1_src_p, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch1_dst_p, 0x00, TEST_DATA_SIZE);
    fill((uint64_t*)ch2_src_p, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch2_dst_p, 0x00, TEST_DATA_SIZE);
    fill((uint64_t*)ch3_src_p, fill_word, TEST_DATA_SIZE);
    fill((uint64_t*)ch3_dst_p, 0x00, TEST_DATA_SIZE);  
    
}

void dma2plb6_4ch_start()
{

    /*struct channel_status ch0_status_p = {};
    struct channel_status ch1_status_p = {};
    struct channel_status ch2_status_p = {};
    struct channel_status ch3_status_p = {};
    */
    
    struct dma2plb6_setup_info ch0_dma_info_p = get_default_dma_info(channel0, ch0_src_p, ch0_dst_p);
    struct dma2plb6_setup_info ch1_dma_info_p = get_default_dma_info(channel1, ch1_src_p, ch1_dst_p);
    struct dma2plb6_setup_info ch2_dma_info_p = get_default_dma_info(channel2, ch2_src_p, ch2_dst_p);
    struct dma2plb6_setup_info ch3_dma_info_p = get_default_dma_info(channel3, ch3_src_p, ch3_dst_p);

    rumboot_putstring("DMA2PLB6: Initializing DMA channels ...\n");
    dma2plb6_mcpy(&ch0_dma_info_p);
    dma2plb6_mcpy(&ch1_dma_info_p);
    dma2plb6_mcpy(&ch2_dma_info_p);
    dma2plb6_mcpy(&ch3_dma_info_p);
}

void dma2plb6_4ch_finish()
{


    rumboot_putstring("DMA2PLB6: Waiting for DMA transfer complete ...\n");
    if (wait_dma2plb6_mcpy(&ch0_dma_info_p, &ch0_status_p))
    {
        rumboot_putstring("DMA2PLB6: Channel 0 transfer succeeded.\n");
        if (wait_dma2plb6_mcpy(&ch1_dma_info_p, &ch1_status_p))
        {
            rumboot_putstring("DMA2PLB6: Channel 1 transfer succeeded.\n");
            if (wait_dma2plb6_mcpy(&ch2_dma_info_p, &ch2_status_p))
            {
                rumboot_putstring("DMA2PLB6: Channel 2 transfer succeeded.\n");
                if (wait_dma2plb6_mcpy(&ch3_dma_info_p, &ch3_status_p))
                {
                    rumboot_putstring("DMA2PLB6: Channel 3 transfer succeeded.\n");
                }
                else
                {
                    rumboot_putstring("DMA2PLB6: Channel 3 transfer error.\n");
                    //return 1;
                }
            }
            else
            {
                rumboot_putstring("DMA2PLB6: Channel 2 transfer error.\n");
                //return 1;
            }
        }
        else
        {
            rumboot_putstring("DMA2PLB6: Channel 1 transfer error.\n");
            //return 1;
        }
    }
    else
    {
        rumboot_putstring("DMA2PLB6: Channel 0 transfer error.\n");
        //return 1;
    }

    /*
    rumboot_putstring("DMA2PLB6: Memory check ...\n");
    TEST_ASSERT(compare((uint32_t)ch0_src, (uint32_t)ch0_dst, TEST_DATA_SIZE) == true, "Channel0: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch1_src, (uint32_t)ch1_dst, TEST_DATA_SIZE) == true, "Channel1: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch2_src, (uint32_t)ch2_dst, TEST_DATA_SIZE) == true, "Channel2: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch3_src, (uint32_t)ch3_dst, TEST_DATA_SIZE) == true, "Channel3: memory comparison failed");
    */

    //return 0;
}

#ifndef POWER_TEST
int main(void)
{
    uint32_t result = 0;

    rumboot_putstring("Starting test_oi10_cpu_023 ...\n");

    emi_init(DCR_EM2_EMI_BASE);

    result += check_single_channel();
    result += check_multiple_channels_4();
    result += check_multiple_channels_2();

    if (result)
        rumboot_putstring("Test has been failed.\n");
    else
        rumboot_putstring("Test has been finished successfully.\n");

    return result;
}
#endif
