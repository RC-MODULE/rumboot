#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/timer.h>
#include <rumboot/testsuite.h>
#include <rumboot/rumboot.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <regs/regs_muart.h>
#include <devices/muart.h>
#include <regs/regs_mdma.h>
#include <devices/mdma_chan_api.h>

struct rumboot_irq_entry *tbl;

static uint32_t BASE[] = {UART0_BASE, UART1_BASE};
static uint32_t INTR[] = {UART0_INTR, UART1_INTR};

static void handler(int irq, void *arg)
{
        volatile uint32_t *M = arg;
        uint32_t reg;
        M[1]++;
        reg = ioread32(M[0] + MUART_STATUS);
        rumboot_printf("MUART_STATUS = %x\n", reg);
        rumboot_printf("IRQ arrived, arg=%x base=%x irq=%d, count=%d\n",
                      arg, M[0], irq, M[1]);
}

static void set_muart(uint32_t base)
{
    struct muart_conf conf;
        conf.wlen = WL_8;
        conf.stp2 = STP1;
        conf.is_even = 0;
        conf.is_parity = 1;
        conf.mode = RS_232;
        conf.is_loopback = 1;
        conf.baud_rate = 12500000;// bod/s (bdiv=4)
        conf.is_dma = 0;

    rumboot_printf("SET UART: base=%x stp2=%x wlen=%x mode=%x " \
          "pan=%x eps=%x\n",  base,
          conf.stp2, conf.wlen, conf.mode, conf.is_parity, conf.is_even);

    muart_soft_reset(base);
    muart_init(base, &conf);
}

static bool test_APB_GIC(uint32_t uart_N)
{
    uint32_t *M;
    uint32_t base = BASE[uart_N];
    uint32_t intr = INTR[uart_N];
    uint32_t  reg_fifo_state=0;
#define FIFOWM  0x0100010
    int cnt = (FIFOWM & 0xFF)+4;
    char *ch;
    int heap_id = 0;

    rumboot_printf("====== Interrupt by FIFO filling\n");

    //Set handler
    M = (uint32_t *)rumboot_malloc_from_heap_aligned(heap_id,16,ALIGN4);
    M[0]=base;
    M[1]=0;
    rumboot_irq_set_handler(tbl, intr, 0, handler, (void *)M);
    // Configure and Activate the table
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(0); //UART0_INTR);
    rumboot_irq_sei();

    set_muart(base);
    set_reg(base, MUART_FIFOWM, FIFOWM); //0x2000200
    set_reg(base, MUART_MASK, 0x1FF); //[0:8]
    muart_dump(base);
    rumboot_printf("arg=%x base=%x \n", M, M[0]);
    // Interrupt call
    rumboot_printf("====== Interrupt call\n");
    muart_enable(base);

    if((ch=(char *)rumboot_malloc_from_heap_aligned(heap_id,cnt+4,ALIGN4)) == NULL){
        rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
        return false;
    }
    muart_write_data_apb(base, ch, cnt);

    rumboot_free(ch);
    reg_fifo_state = ioread32(base + MUART_FIFO_STATE);
    rumboot_printf("fifo_state=%x limit=%x\n",reg_fifo_state,FIFOWM&0xFF);
    if ( M[1] == 0) {
        rumboot_printf("FAIL:Uart interrupt is not happened \n");
        return false;
    }
    reg_fifo_state = ioread32(base + MUART_FIFO_STATE);
    rumboot_printf("fifo_state=%x limit=%x\n",reg_fifo_state,FIFOWM&0xFF);
    if (reg_fifo_state < (FIFOWM&0xFF)) {
        rumboot_printf("FAIL:Uart interrupt is happened before\n");
        return false;
    }
    return true;
}
/* Declare the testsuite structure */
TEST_SUITE_BEGIN(APB_GIC_test, "APB Interface and GIC")
TEST_ENTRY("test_APB_GIC_base_0", test_APB_GIC, 0), //UART0_BASE
TEST_ENTRY("test_APB_GIC_base_1", test_APB_GIC, 1), //UART1_BASE
TEST_SUITE_END();

uint32_t main()
{
    int res = 0;

    rumboot_printf("Test 1.4 MUART APB GIC\n");
    rumboot_printf("Test 1.5 MUART APB EXT_IRQ_GEN\n");
    rumboot_printf("Interrupt by FIFO overflow\n");

    /* Disable all interrupts */
    rumboot_irq_cli();

    /* Create an IRQ table */
    tbl = rumboot_irq_create(NULL);

    /* Run APB Interface and GIC test suite */
    res = test_suite_run(NULL, &APB_GIC_test);
//    rumboot_printf("%d tests from suite failed\n", res);
    if (res == 0) {
       rumboot_printf("TEST PASS: %d errors happened\n", res);
    }else{
        rumboot_printf("TEST FAIL: %d errors happened\n", res);
    }

    /* Deinit */
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    return res;
}
