/**
 * Implemented test which described in 2.4 PPC_SRAM_SDRAM_slave0_testplan.docx
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/test_event_c.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>

#define EM2_BANKS_NUM   6

#define SRAM0_WRDATA1  0xFFFFFFFF
#define SDRAM_WRDATA1  0xEEEEEEEE
#define SSRAM_WRDATA1  0xDDDDDDDD
#define PIPE_WRDATA1   0xCCCCCCCC
#define SRAM1_WRDATA1  0xBBBBBBBB
#define NOR_WRDATA1    0xAAAAAAAA

#define WRDATA2        0x00

typedef struct
{
    uint32_t A0;
    uint32_t A1;
    uint32_t A2;
    uint32_t A3;
    uint32_t D;
} test_data_and_addr_t;

typedef struct
{
    uint32_t base_addr;
    uint32_t size;
    uint32_t data;
}em2_banks_cfg_t;

em2_banks_cfg_t em2_cfg1[EM2_BANKS_NUM] = {
                                            { SRAM0_BASE,     SRAM0_SIZE,     SRAM0_WRDATA1 },
                                            { SDRAM_BASE,     SDRAM_SIZE,     SDRAM_WRDATA1 },
                                            { SSRAM_BASE,     SSRAM_SIZE,     SSRAM_WRDATA1 },
                                            { PIPELINED_BASE, PIPELINED_SIZE, PIPE_WRDATA1  },
                                            { SRAM1_BASE,     SRAM1_SIZE,     SRAM1_WRDATA1 },
                                            { NOR_BASE,       NOR_SIZE,       NOR_WRDATA1   },
                                          };

em2_banks_cfg_t em2_cfg2[EM2_BANKS_NUM] = {
                                            { SRAM0_BASE,     SRAM0_SIZE,     WRDATA2 },
                                            { SDRAM_BASE,     SDRAM_SIZE,     WRDATA2 },
                                            { SSRAM_BASE,     SSRAM_SIZE,     WRDATA2 },
                                            { PIPELINED_BASE, PIPELINED_SIZE, WRDATA2 },
                                            { SRAM1_BASE,     SRAM1_SIZE,     WRDATA2 },
                                            { NOR_BASE,       NOR_SIZE,       WRDATA2 },
                                          };

void preinit_mem(uint32_t addr)
{
    uint32_t addr_i = (addr<0x10) ? addr : (addr - 0x10);

    while (addr_i < (addr + 0x10))
    {
        if (addr_i>=NOR_BASE)
            nor_write32(0x77777777, addr_i);
        else
            iowrite32(0x77777777, addr_i);
        addr_i += 4;
    }
}


void generate_test_data_and_addr_for_bank(em2_banks_cfg_t* bank_cfg, test_data_and_addr_t* test_data_and_addr)
{
    test_data_and_addr->A0 = bank_cfg->base_addr;
    test_data_and_addr->A1 = bank_cfg->base_addr + bank_cfg->size/2 - 4;
    test_data_and_addr->A2 = bank_cfg->base_addr + bank_cfg->size/2;
    test_data_and_addr->A3 = bank_cfg->base_addr + bank_cfg->size - 4;
    test_data_and_addr->D  = bank_cfg->data;
    //rumboot_printf("0x%X 0x%X 0x%X 0x%X / 0x%X\n", test_data_and_addr->A0, test_data_and_addr->A1, test_data_and_addr->A2, test_data_and_addr->A3, test_data_and_addr->D);

    //rumboot_printf("Preinit memory\n");
    preinit_mem(test_data_and_addr->A0);
    preinit_mem(test_data_and_addr->A1);
    preinit_mem(test_data_and_addr->A2);
    preinit_mem(test_data_and_addr->A3);
}

void generate_test_data_and_addr(em2_banks_cfg_t* bank_cfg, test_data_and_addr_t* test_data_and_addr)
{
    for (int i=0; i<EM2_BANKS_NUM; i++)
        generate_test_data_and_addr_for_bank(bank_cfg + i, test_data_and_addr + i);
}

void report_data_error(uint32_t addr, uint32_t exp, uint32_t act)
{
    rumboot_printf("Data compare error at addr 0x%X!\n Expected: 0x%X\nActual: 0x%X\n", addr, exp, act);
}

void seq_n_wr_n_rd(uint32_t* data, uint32_t* addr, uint32_t len)
{
    uint32_t rd;
    uint32_t data_i;
    uint32_t addr_i;

    rumboot_printf("Check Write/Read operation:\n");

    for (int i=0; i<len; i++)
        rumboot_printf("addr%d 0x%X / data%d 0x%X\n", (i + 1), *(addr + i), (i + 1), *(data + i));


    for (int i=0; i<len; i++)
    {
        data_i = *(data + i);
        addr_i = *(addr + i);
        if (addr_i>=NOR_BASE)
            nor_write32(data_i, addr_i);
        else
            iowrite32(data_i, addr_i);
    }

    for (int i=0; i<len; i++)
    {
        data_i = *(data + i);
        addr_i = *(addr + i);
        rd = ioread32(addr_i);
        if (rd != data_i)
        {
            report_data_error(addr_i, data_i, rd);
            TEST_ASSERT(0, "Data error");
        }
    }
}

void check0(test_data_and_addr_t* test_data_and_addr)
{

    rumboot_printf("\n\n---CHECK0---\n\n");

    /*
     * W_CS0_A0, R_CS0_A0,
     * W_CS5_A3, R_CS5_A3,
     * W_CS1_A1, R_CS1_A1,
     * W_CS4_A2, R_CS4_A2,
     * W_CS2_A2, R_CS2_A2,
     * W_CS3_A1, R_CS3_A1,
     */
    seq_n_wr_n_rd( &((test_data_and_addr + 0)->D), &((test_data_and_addr + 0)->A0), 1);
    seq_n_wr_n_rd( &((test_data_and_addr + 5)->D), &((test_data_and_addr + 5)->A3), 1);
    seq_n_wr_n_rd( &((test_data_and_addr + 1)->D), &((test_data_and_addr + 1)->A1), 1);
    seq_n_wr_n_rd( &((test_data_and_addr + 4)->D), &((test_data_and_addr + 4)->A2), 1);
    seq_n_wr_n_rd( &((test_data_and_addr + 2)->D), &((test_data_and_addr + 2)->A2), 1);
    seq_n_wr_n_rd( &((test_data_and_addr + 3)->D), &((test_data_and_addr + 3)->A1), 1);
}

void check1(test_data_and_addr_t* test_data_and_addr)
{
    const uint32_t LEN = 2;
    uint32_t addr_arr[LEN];
    uint32_t data_arr[LEN];

    rumboot_printf("\n\n---CHECK1---\n\n");

     //W_CS0_A1, W_CS0_A2, R_CS0_A1, R_CS0_A2,
    addr_arr[0] = (test_data_and_addr + 0)->A1;
    addr_arr[1] = (test_data_and_addr + 0)->A2;
    data_arr[0] = (test_data_and_addr + 0)->D;
    data_arr[1] = (test_data_and_addr + 0)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS5_A2, W_CS5_A1, R_CS5_A2, R_CS5_A1,
    addr_arr[0] = (test_data_and_addr + 5)->A2;
    addr_arr[1] = (test_data_and_addr + 5)->A1;
    data_arr[0] = (test_data_and_addr + 5)->D;
    data_arr[1] = (test_data_and_addr + 5)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS1_A2, W_CS1_A3, R_CS1_A2, R_CS1_A3,
    addr_arr[0] = (test_data_and_addr + 1)->A2;
    addr_arr[1] = (test_data_and_addr + 1)->A3;
    data_arr[0] = (test_data_and_addr + 1)->D;
    data_arr[1] = (test_data_and_addr + 1)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS4_A1, W_CS4_A0, R_CS4_A1, R_CS4_A0,
    addr_arr[0] = (test_data_and_addr + 4)->A1;
    addr_arr[1] = (test_data_and_addr + 4)->A0;
    data_arr[0] = (test_data_and_addr + 4)->D;
    data_arr[1] = (test_data_and_addr + 4)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS2_A3, W_CS2_A0, R_CS2_A3, R_CS2_A0,
    addr_arr[0] = (test_data_and_addr + 2)->A3;
    addr_arr[1] = (test_data_and_addr + 2)->A0;
    data_arr[0] = (test_data_and_addr + 2)->D;
    data_arr[1] = (test_data_and_addr + 2)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS3_A0, W_CS3_A3, R_CS3_A0, R_CS3_A3,
    addr_arr[0] = (test_data_and_addr + 3)->A0;
    addr_arr[1] = (test_data_and_addr + 3)->A3;
    data_arr[0] = (test_data_and_addr + 3)->D;
    data_arr[1] = (test_data_and_addr + 3)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);
}

void check2(test_data_and_addr_t* test_data_and_addr)
{
    const uint32_t LEN = 3;
    uint32_t addr_arr[LEN];
    uint32_t data_arr[LEN];

    rumboot_printf("\n\n---CHECK2---\n\n");

     //W_CS0_A3, W_CS0_A0, W_CS0_A1, R_CS0_A3, R_CS0_A0, R_CS0_A1,
    addr_arr[0] = (test_data_and_addr + 0)->A3;
    addr_arr[1] = (test_data_and_addr + 0)->A0;
    addr_arr[2] = (test_data_and_addr + 0)->A1;
    data_arr[0] = (test_data_and_addr + 0)->D;
    data_arr[1] = (test_data_and_addr + 0)->D;
    data_arr[2] = (test_data_and_addr + 0)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS5_A0, W_CS5_A3, W_CS5_A2, R_CS5_A0, R_CS5_A3, R_CS5_A2,
    addr_arr[0] = (test_data_and_addr + 5)->A0;
    addr_arr[1] = (test_data_and_addr + 5)->A3;
    addr_arr[2] = (test_data_and_addr + 5)->A2;
    data_arr[0] = (test_data_and_addr + 5)->D;
    data_arr[1] = (test_data_and_addr + 5)->D;
    data_arr[2] = (test_data_and_addr + 5)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS1_A0, W_CS1_A1, W_CS1_A2, R_CS1_A0, R_CS1_A1, R_CS1_A2,
    addr_arr[0] = (test_data_and_addr + 1)->A0;
    addr_arr[1] = (test_data_and_addr + 1)->A1;
    addr_arr[2] = (test_data_and_addr + 1)->A2;
    data_arr[0] = (test_data_and_addr + 1)->D;
    data_arr[1] = (test_data_and_addr + 1)->D;
    data_arr[2] = (test_data_and_addr + 1)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS4_A3, W_CS4_A2, W_CS4_A1, R_CS4_A3, R_CS4_A2, R_CS4_A1
    addr_arr[0] = (test_data_and_addr + 4)->A3;
    addr_arr[1] = (test_data_and_addr + 4)->A2;
    addr_arr[2] = (test_data_and_addr + 4)->A1;
    data_arr[0] = (test_data_and_addr + 4)->D;
    data_arr[1] = (test_data_and_addr + 4)->D;
    data_arr[2] = (test_data_and_addr + 4)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS2_A1, W_CS2_A2, W_CS2_A3, R_CS2_A1, R_CS2_A2, R_CS2_A3
    addr_arr[0] = (test_data_and_addr + 2)->A1;
    addr_arr[1] = (test_data_and_addr + 2)->A2;
    addr_arr[2] = (test_data_and_addr + 2)->A3;
    data_arr[0] = (test_data_and_addr + 2)->D;
    data_arr[1] = (test_data_and_addr + 2)->D;
    data_arr[2] = (test_data_and_addr + 2)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS3_A2, W_CS3_A1, W_CS3_A0, R_CS3_A2, R_CS3_A1, R_CS3_A0
    addr_arr[0] = (test_data_and_addr + 3)->A2;
    addr_arr[1] = (test_data_and_addr + 3)->A1;
    addr_arr[2] = (test_data_and_addr + 3)->A0;
    data_arr[0] = (test_data_and_addr + 3)->D;
    data_arr[1] = (test_data_and_addr + 3)->D;
    data_arr[2] = (test_data_and_addr + 3)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);
}

void check3(test_data_and_addr_t* test_data_and_addr)
{
    const uint32_t LEN = 2;
    uint32_t addr_arr[LEN];
    uint32_t data_arr[LEN];

    rumboot_printf("\n\n---CHECK3---\n\n");

    //W_CS0_A2, W_CS0_A3, R_CS0_A2, R_CS0_A3
    addr_arr[0] = (test_data_and_addr + 0)->A2;
    addr_arr[1] = (test_data_and_addr + 0)->A3;
    data_arr[0] = (test_data_and_addr + 0)->D;
    data_arr[1] = (test_data_and_addr + 0)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS5_A1, W_CS5_A0, R_CS5_A1, R_CS5_A0
    addr_arr[0] = (test_data_and_addr + 5)->A1;
    addr_arr[1] = (test_data_and_addr + 5)->A0;
    data_arr[0] = (test_data_and_addr + 5)->D;
    data_arr[1] = (test_data_and_addr + 5)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS1_A3, W_CS1_A0, R_CS1_A3, R_CS1_A0
    addr_arr[0] = (test_data_and_addr + 1)->A3;
    addr_arr[1] = (test_data_and_addr + 1)->A0;
    data_arr[0] = (test_data_and_addr + 1)->D;
    data_arr[1] = (test_data_and_addr + 1)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS4_A0, W_CS4_A3, R_CS4_A0, R_CS4_A3
    addr_arr[0] = (test_data_and_addr + 4)->A0;
    addr_arr[1] = (test_data_and_addr + 4)->A3;
    data_arr[0] = (test_data_and_addr + 4)->D;
    data_arr[1] = (test_data_and_addr + 4)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS2_A0, W_CS2_A1, R_CS2_A0, R_CS2_A1,
    addr_arr[0] = (test_data_and_addr + 2)->A0;
    addr_arr[1] = (test_data_and_addr + 2)->A1;
    data_arr[0] = (test_data_and_addr + 2)->D;
    data_arr[1] = (test_data_and_addr + 2)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);

    //W_CS3_A3, W_CS3_A2, R_CS3_A3, R_CS3_A2
    addr_arr[0] = (test_data_and_addr + 3)->A3;
    addr_arr[1] = (test_data_and_addr + 3)->A2;
    data_arr[0] = (test_data_and_addr + 3)->D;
    data_arr[1] = (test_data_and_addr + 3)->D;
    seq_n_wr_n_rd( data_arr, addr_arr, LEN);
}

void run_checks(test_data_and_addr_t* test_data_and_addr)
{
    check0(test_data_and_addr);
    check1(test_data_and_addr);
    check2(test_data_and_addr);
    check3(test_data_and_addr);
}

int main()
{
    test_data_and_addr_t test_data_and_addr[EM2_BANKS_NUM];

    rumboot_printf("Start test_oi10_em2_204\n");

    emi_init();
    emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_off);

    generate_test_data_and_addr(em2_cfg1, test_data_and_addr);
    run_checks(test_data_and_addr);

    generate_test_data_and_addr(em2_cfg2, test_data_and_addr);
    run_checks(test_data_and_addr);

    return 0;
}
