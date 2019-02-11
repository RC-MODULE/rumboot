#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <platform/common_macros/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/devices/mpic128.h>
#include <platform/regs/fields/emi.h>
#include <platform/interrupts.h>
#include <platform/devices/dma2plb6.h>

#define TLB_ENTRY_EM2_CACHE_WT_0   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_EM2_CACHE_WT_1   MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

#define DATA_SIZE   0x1000
#define DATA_OFFSET (DATA_SIZE + DATA_SIZE)

uint32_t src_address_array[] =
{
    SRAM0_BASE,
    SDRAM_BASE,
    SSRAM_BASE,
    PIPELINED_BASE,
    SRAM1_BASE,
    IM1_BASE,
    IM2_BASE
};

static volatile bool dma2plb6_handler = false;

static void handler_dma2plb6_ch0()
{
    rumboot_printf( "irq handler (DMA, ch0)\n");
    dma2plb6_clear_interrupt(DCR_DMAPLB6_BASE, channel0);
    dma2plb6_handler = true;
}

static void handler_system_hung()
{
    rumboot_printf( "irq handler (SYSTEM HUNG)\n");
    TEST_ASSERT(0, "ERROR! SYSTEM HUNG");
}

void init_handlers()
{
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, DMA2PLB6_DMA_IRQ_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_dma2plb6_ch0, ( void* ) 0);
    rumboot_irq_set_handler( tbl, O_SYSTEM_HUNG, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler_system_hung, ( void* ) 0);

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( DMA2PLB6_DMA_IRQ_0 );
    rumboot_irq_enable( O_SYSTEM_HUNG );
    rumboot_irq_sei();
}

void configure_dma(uint32_t base_addr, DmaChannel channel, uint32_t addr_src, uint32_t addr_dst, uint32_t count)
{
    //default
    dma2plb6_setup_info dma_info = {
            .base_addr = base_addr,
            .source_adr = rumboot_virt_to_phys((void*)addr_src),
            .dest_adr = rumboot_virt_to_phys((void*)addr_dst),
            .priority = priority_medium_low,
            .striding_info.striding = striding_none,
            .tc_int_enable = true,
            .err_int_enable = true,
            .int_mode = int_mode_level_wait_clr,
            .channel = channel,
            .rw_transfer_size = rw_tr_size_1q,
            .transfer_width = tr_width_byte,
            .count = count,
            .snp_mode = snp_mode_off
    };
    dma2plb6_mcpy_init(&dma_info);
}

int main ()
{
    test_event_send_test_id("test_oi10_cpu_032");

    emi_init(DCR_EM2_EMI_BASE);

    //init data
    rumboot_printf("Init data\n");
    for (uint32_t i = 0; i < ARRAY_SIZE(src_address_array); i++)
    {
        rumboot_memfill8((void*)src_address_array[i], DATA_SIZE, 0x00, 0x01);
        rumboot_memfill8((void*)(src_address_array[i]+DATA_OFFSET), DATA_SIZE, 0x00, 0x01);
        rumboot_memfill8_modelling((void*)(src_address_array[i]+DATA_SIZE), DATA_SIZE, 0x00, 0x00);
        rumboot_memfill8_modelling((void*)(src_address_array[i]+DATA_OFFSET+DATA_SIZE), DATA_SIZE, 0x00, 0x00);
    }

    rumboot_printf("Init handlers\n");
    init_handlers();

    rumboot_printf("Set tlb (WT)\n");
    static const tlb_entry em0_tlb_entry_wt[] = {{TLB_ENTRY_EM2_CACHE_WT_0}, {TLB_ENTRY_EM2_CACHE_WT_1}} ;
    write_tlb_entries(em0_tlb_entry_wt,ARRAY_SIZE(em0_tlb_entry_wt));

    rumboot_printf("Data size = 0x%x\n", DATA_SIZE);
    for (uint32_t i = 0; i < ARRAY_SIZE(src_address_array); i++)
    {
        rumboot_printf("Start check (addr = 0x%x)\n", src_address_array[i]);
        rumboot_printf("Start DMA\n");
        configure_dma(DCR_DMAPLB6_BASE, channel0, src_address_array[i], src_address_array[i] + DATA_SIZE, DATA_SIZE);
        dma2plb6_enable_channel(DCR_DMAPLB6_BASE, channel0);

        rumboot_printf("Start write-read\n");
        uint32_t addr = (src_address_array[i] + DATA_OFFSET);
        while (!dma2plb6_handler)
        {
            iowrite32(ioread32(addr), addr + DATA_SIZE);
            addr+=4;
        }
        dma2plb6_handler = false;
        rumboot_printf("\n");
    }

    rumboot_printf("TEST OK\n");
    return 0;
}
