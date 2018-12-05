
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>

#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/devices/emi.h>
#include <platform/test_assert.h>

#include <platform/devices/greth.h>
#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/devices/emi.h>
#include <platform/devices/dma2plb6.h>
#include <platform/devices/nor_1636RR4.h>
/*
 * Registers access checks
 */

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



#define TEST_DATA_SIZE 128
#define TEST_DATA_CONTENT 0xFEDCBA9876543210

 #define stmw(base, shift)\
    asm volatile (\
        "addis 26, 0,  0x0101 \n\t"\
        "addi  26, 26, 0x0101 \n\t"\
        "addis 27, 0,  0x0101 \n\t"\
        "addi  27, 27, 0x0101 \n\t"\
        "addis 28, 0,  0x0101 \n\t"\
        "addi  28, 28, 0x0101 \n\t"\
        "addis 29, 0,  0x0101 \n\t"\
        "addi  29, 29, 0x0101 \n\t"\
        "addis 30, 0,  0x0101 \n\t"\
        "addi  30, 30, 0x0101 \n\t"\
        "addis 31, 0,  0x0202 \n\t"\
        "addi  31, 31, 0x0202 \n\t"\
        "addis 25, 0, %0 \n\t"\
        "stmw 26, %1(25) \n\t"\
        ::"i"(base), "i"(shift)\
    )

struct regpoker_checker greth_check_array[] = {
    { "CTRL              ",  REGPOKER_READ32,  CTRL              , 0x98000090, 0xFE007CFF },
    { "MDIO_CTRL         ",  REGPOKER_READ32,  MDIO_CTRL         , 0x01E10140, 0xFFFFFFCF },
    { "TRANSMIT_DESCR_PTR",  REGPOKER_READ32,  TRANSMIT_DESCR_PTR, 0x0, 0x3F8 },
    { "RECEIVER_DESCR_PTR",  REGPOKER_READ32,  RECEIVER_DESCR_PTR, 0x0, 0x3F8 },
    { "EDCL_IP           ",  REGPOKER_READ32,  EDCL_IP           , EDCLIP , ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_READ32,  EDCL_MAC_MSB      , EDCLMAC_MSB, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_READ32,  EDCL_MAC_LSB      , EDCLMAC_LSB, ~0 },
    { "EDCL_IP           ",  REGPOKER_WRITE32, EDCL_IP           , 0, ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_WRITE32, EDCL_MAC_MSB      , 0, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_WRITE32, EDCL_MAC_LSB      , 0, ~0 }
};



void regs_check(uint32_t base_addr)
{
    if (base_addr==GRETH_1_BASE)
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 1, base_addr);
        greth_check_array[4].expected = EDCLIP1;
        greth_check_array[6].expected |= EDCLADDRL1;
    }
    else
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 0, base_addr);
    }
    TEST_ASSERT(rumboot_regpoker_check_array(greth_check_array, base_addr)==0, "Failed to check GRETH registers\n");
}




static void fill(uint64_t *s, uint64_t pattern, uint32_t size_in_bytes)
{
    uint32_t i;
    for (i = 0; i < (size_in_bytes>>3); i++)
        s[i] = pattern;
}

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

static uint64_t fill_word = TEST_DATA_CONTENT;

static uint32_t compare(uint32_t s_addr, uint32_t d_addr,uint32_t size_in_bytes)
{
    uint32_t i;
    volatile uint64_t temp0, temp1;
    for (i = 0; i < size_in_bytes; i+=8){

        rumboot_printf("offcet: 0x%x\n", i);

        temp0 = ioread64(s_addr + i);
        temp1 = ioread64(d_addr + i);

        //rumboot_printf("get source data: 0x%x%x\n", (uint32_t)(temp0>>32),(uint32_t)(temp0));
        //rumboot_printf("get dest data: 0x%x%x\n", (uint32_t)(temp1>>32),(uint32_t)(temp1));

        //msync();

        if(temp0 != temp1)
        {
            rumboot_printf("compare failed, expected value: 0x%x%x\n", (uint32_t)(temp0>>32),(uint32_t)(temp0));
            rumboot_printf("read value: 0x%x%x\n", (uint32_t)(temp1>>32),(uint32_t)(temp1));
            return false;
        }
    }
    return true;
}

static uint32_t check_dma2plb6_0_mem_to_mem(uint32_t source_ea, uint32_t dest_ea, uint64_t source_phys, uint64_t dest_phys)
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
            rumboot_printf("transfer_width = 0x%x\n", dma_info.transfer_width);

            rumboot_printf("fill source area\n");
            fill((uint64_t *)source_ea, fill_word, TEST_DATA_SIZE);

            rumboot_printf("fill dest area\n");
            fill((uint64_t *)dest_ea, 0x0, TEST_DATA_SIZE);




            dma_info.count = TEST_DATA_SIZE/dma2plb6_get_bytesize(dma_info.transfer_width);

            rumboot_printf("run dma copy\n");
            if(dma2plb6_single_copy(&dma_info, &status) == false){
                SHOW_STATUS(status.spec_error_status);
                return false;
            } else {
                rumboot_printf("run compare data\n");
                TEST_ASSERT(compare(source_ea, dest_ea, TEST_DATA_SIZE) == true, "Memory comparison failed");
                fill_word = ~fill_word;
                rumboot_printf("done: tr_width = 0x%x\n", dma_info.transfer_width);
            }
        }
    }
    return true;
}



static void byte_plb_axi_test (uint32_t base_addr){
    uint32_t i = 0;
    uint8_t temp = 0;

    do {
        iowrite8(i,base_addr+i);
        i++;
    } while (i!=0x100);

    i=0;
    do {
        temp = ioread8(base_addr + i);
        if (i != temp){
            //rumboot_printf("[hword_plb_axi_test] Memory comparison failed exp 0x%x act 0x%x \n", i, temp);
            TEST_ASSERT(0, "");
        }
        i++;
    } while (i!=0x100);
}


static void hword_plb_axi_test (uint32_t base_addr, uint32_t offc){
    uint32_t i = 0;
    uint16_t tdata = 0x1;
    uint16_t temp = 0;

    do {
        //rumboot_printf("[hword_plb_axi_test] write data 0x%x to addr 0x%x \n", tdata, base_addr+i+offc);
        iowrite16(tdata,base_addr+i+offc);
        tdata = tdata << 1;
        i+=2;
    } while(tdata!=0x0);

    i=0;
    tdata=0x1;
    do {
        temp = ioread16(base_addr+i+offc);
        //rumboot_printf("[hword_plb_axi_test] read data 0x%x in addr 0x%x \n", temp, base_addr+i+offc);
        if (tdata != temp){
            rumboot_printf("[hword_plb_axi_test] Memory comparison failed exp 0x%x act 0x%x \n", tdata, temp);
            TEST_ASSERT(0, "");
        }

        tdata = tdata << 1;
        i+=2;
    } while (tdata!=0x0);
}



static void word_plb_axi_test (uint32_t base_addr, uint32_t offc){
    uint32_t i = 0;
    uint32_t tdata = 0x1;
    uint32_t temp = 0;

    do {
        //rumboot_printf("[word_plb_axi_test] write data 0x%x to addr 0x%x \n", tdata, base_addr+i+offc);
        iowrite32(tdata,base_addr+i+offc);
        tdata = tdata << 1;
        i+=4;
    } while(tdata!=0x0);

    i=0;
    tdata=0x1;
    do {
        temp = ioread32(base_addr+i+offc);
        //rumboot_printf("[word_plb_axi_test] read data 0x%x in addr 0x%x \n", temp, base_addr+i+offc);
        if (tdata != temp){
            rumboot_printf("[word_plb_axi_test] Memory comparison failed exp 0x%x act 0x%x \n", tdata, temp);
            TEST_ASSERT(0, "");
        }

        tdata = tdata << 1;
        i+=4;
    } while (tdata!=0x0);
}


static void dword_plb_axi_test (uint32_t base_addr, uint32_t offc){
    uint32_t i = 0;
    uint64_t tdata = 0x1;
    uint64_t temp = 0;

    do {
        //rumboot_printf("[dword_plb_axi_test] write data 0x%x%x to addr 0x%x \n", (uint32_t)(tdata>>32), (uint32_t)(tdata&0xFFFFFFFF), base_addr+i+offc);
        iowrite64(tdata,base_addr+i+offc);
        tdata = tdata << 1;
        i+=8;
    } while(tdata!=0x0);

    i=0;
    tdata=0x1;
    do {
        temp = ioread64(base_addr+i+offc);
        //rumboot_printf("[dword_plb_axi_test] read data 0x%x in addr 0x%x \n", (uint32_t)(temp>>32), (uint32_t)(temp&0xFFFFFFFF), base_addr+i+offc);
        if (tdata != temp){
            rumboot_printf("[dword_plb_axi_test] Memory comparison failed exp 0x%x act 0x%x \n", (uint32_t)(tdata>>32), (uint32_t)(tdata&0xFFFFFFFF) , (uint32_t)(temp>>32), (uint32_t)(temp&0xFFFFFFFF));
            TEST_ASSERT(0, "");
        }

        tdata = tdata << 1;
        i+=8;
    } while (tdata!=0x0);
}

static void single_plb6_axi_test (uint32_t base_addr) {
    byte_plb_axi_test (base_addr);

    hword_plb_axi_test (base_addr, 0x0);
    hword_plb_axi_test (base_addr, 0x1);
    hword_plb_axi_test (base_addr, 0x2);
    hword_plb_axi_test (base_addr, 0x3);
    hword_plb_axi_test (base_addr, 0x4);
    hword_plb_axi_test (base_addr, 0x5);
    hword_plb_axi_test (base_addr, 0x6);
    hword_plb_axi_test (base_addr, 0x7);

    word_plb_axi_test (base_addr, 0x0);
    word_plb_axi_test (base_addr, 0x1);
    word_plb_axi_test (base_addr, 0x2);
    word_plb_axi_test (base_addr, 0x3);
    word_plb_axi_test (base_addr, 0x4);
    word_plb_axi_test (base_addr, 0x5);
    word_plb_axi_test (base_addr, 0x6);
    word_plb_axi_test (base_addr, 0x7);

    dword_plb_axi_test (base_addr, 0x0);
    dword_plb_axi_test (base_addr, 0x1);
    dword_plb_axi_test (base_addr, 0x2);
    dword_plb_axi_test (base_addr, 0x3);
    dword_plb_axi_test (base_addr, 0x4);
    dword_plb_axi_test (base_addr, 0x5);
    dword_plb_axi_test (base_addr, 0x6);
    dword_plb_axi_test (base_addr, 0x7);
}



int main(void)
{

    #ifdef CHECK_PLB6_AXI_SINGLE
        rumboot_putstring("Start test_plb6_axi_greth. Apply access to GRETH0 regs for checking PLB6->AXI convertion for single transactions\n");
        regs_check(GRETH_0_BASE);
    #endif
    rumboot_printf("END CHECK\n");


    single_plb6_axi_test (IM1_BASE);




    uint32_t src_im1, dst_im1;
    volatile uint64_t src_im1_physical, dst_im1_physical;

    src_im1 = (IM1_BASE);
    rumboot_printf("src_im1 = 0x%x\n", src_im1);

    dst_im1 = (IM1_BASE + 0x80);
    rumboot_printf("dst_im1 = 0x%x\n", dst_im1);

    set_mem_window(MEM_WINDOW_0);
    src_im1_physical = rumboot_virt_to_phys((uint32_t*)src_im1);
    rumboot_printf("src_im1_physical = 0x%x%x \n", src_im1_physical >> 32, src_im1_physical & 0xFFFFFFFF);

    dst_im1_physical = rumboot_virt_to_phys((uint32_t*)dst_im1);
    rumboot_printf("dst_im1_physical = 0x%x%x \n", dst_im1_physical >> 32,dst_im1_physical & 0xFFFFFFFF);

    TEST_ASSERT(check_dma2plb6_0_mem_to_mem(src_im1,
                                            dst_im1,
                                            src_im1_physical,
                                            dst_im1_physical) == true, "IM1-to-IM1 failed");

    return 0;
}
