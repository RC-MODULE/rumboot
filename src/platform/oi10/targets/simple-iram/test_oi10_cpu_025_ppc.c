/*
 * test_oi10_cpu_025_ppc.c
 *
 *  Created on: Jan 16, 2019
 *      Author: m.smolina
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>

#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <platform/devices/l2c.h>
#include <platform/interrupts.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_debug_fields.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>

#define EVENT_GENERATE_TLB_MC                   (TEST_EVENT_CODE_MIN + 0)
#define EVENT_CLEAR_TLB_MC                      (TEST_EVENT_CODE_MIN + 1)
#define EVENT_GENERATE_IC_MC                    (TEST_EVENT_CODE_MIN + 2)
#define EVENT_CLEAR_IC_MC                       (TEST_EVENT_CODE_MIN + 3)
#define EVENT_GENERATE_DC_MC                    (TEST_EVENT_CODE_MIN + 4)
#define EVENT_CLEAR_DC_MC                       (TEST_EVENT_CODE_MIN + 5)
#define EVENT_GENERATE_GPR_MC                   (TEST_EVENT_CODE_MIN + 6)
#define EVENT_CLEAR_GPR_MC                      (TEST_EVENT_CODE_MIN + 7)
#define EVENT_GENERATE_FPR_MC                   (TEST_EVENT_CODE_MIN + 8)
#define EVENT_CLEAR_FPR_MC                      (TEST_EVENT_CODE_MIN + 9)
#define EVENT_GENERATE_IMP_MC                   (TEST_EVENT_CODE_MIN + 10)
#define EVENT_CLEAR_IMP_MC                      (TEST_EVENT_CODE_MIN + 11)
#define EVENT_GENERATE_L2_MC                    (TEST_EVENT_CODE_MIN + 12)
#define EVENT_CLEAR_L2_MC                       (TEST_EVENT_CODE_MIN + 13)
#define EVENT_GENERATE_DCR_MC                   (TEST_EVENT_CODE_MIN + 14)
#define EVENT_CLEAR_DCR_MC                      (TEST_EVENT_CODE_MIN + 15)

//                                          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,              V   )
#define TLB_ENTRY_EM0_0_NOCACHE_VALID       MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_EM0_0_NOCACHE_INVALID     MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY_EM0_0_CACHE_VALID         MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_BOOTROM_MIRROR_0          MMU_TLB_ENTRY(  0x01F,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,  MMU_TLBWE_BE_0,     0b1 )
#define TLB_ENTRY_SS2_RESERVED              MMU_TLB_ENTRY(  0x020,  0xC0080,    0xC0080,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,  MMU_TLBWE_BE_0,     0b1 )

#define UNEXIST_DCR_ADDR                0x00
#define TEST_OI10_CPU_025_PPC_TIMEOUT    100
#define EA_SS2_RESERVED                 0xC0080000 // start effective address of TLB_ENTRY_SS2_RESERVED

//static const tlb_entry bootrom_mirror           = {TLB_ENTRY_BOOTROM_MIRROR_0};
static const tlb_entry em0_0_cache_on_valid     = {TLB_ENTRY_EM0_0_CACHE_VALID};
static const tlb_entry em0_0_cache_off_valid    = {TLB_ENTRY_EM0_0_NOCACHE_VALID};
static const tlb_entry ss2_reserved_valid       = {TLB_ENTRY_SS2_RESERVED};

volatile uint32_t mem_test_data __attribute__((section(".data"))) = 0x5555AAAA;

volatile uint32_t MC_HANDLED __attribute__((section(".data")))   = 0;

uint32_t value = 0;
typedef void func();
extern uint32_t cached_func[], func_size;

asm  (

  "   .global cached_func           \n\t"
  "   .global func_size             \n\t"
  ".align 8                         \n\t"
  "cached_func:                     \n\t"
  "   lis 3,  value@h               \n\t"
  "   ori 3, 3,  value@l            \n\t"
  "   lwz 5, 0(3)                   \n\t"
  "   li  4, 0                      \n\t"
  "   orc 5, 5, 4                   \n\t"
  "   stw 5, 0(3)                   \n\t"
  "   blr                           \n\t"
  ".align 8                         \n\t"
  "func_size:                       \n\t"
  "   .long (.-cached_func)         \n\t"
);

static void load_code_in_cache(uint32_t start_addr, uint32_t code_size)
{
    uint32_t cur_addr;
    rumboot_printf("Caching exception generation code\n");

    for (cur_addr=start_addr; cur_addr<start_addr+code_size; cur_addr+=L1C_LINE_SIZE)
    {
        rumboot_printf("icbt %x\n", cur_addr);
        icbt(0, (void* const)cur_addr);
    }
}

static void gen_IC_exception_asm()
{
    rumboot_printf("generate IC exception\n");
    func* func_test = (func*)rumboot_malloc_from_named_heap_aligned( "SRAM0", func_size, 8 );
    memcpy( func_test, cached_func, func_size );
    msync();
    load_code_in_cache((uint32_t)func_test, func_size);
    asm volatile   (
        "icbi 0,%0\n\t"
        "msync\n\t"
        "mtspr %3, %1\n\t"
        "isync\n\t"
        "icbt 0,0,%0\n\t"
        "msync\n\t"
        "mtspr %3, %2\n\t"
        "isync\n\t"
        ::"r"((uint32_t) func_test), "r"(0b11 << CTRL_CCR1_ICDPEI_i), "r"(0), "i"(SPR_CCR1)
        :
    );
    rumboot_printf("Check value\n");
    func_test ();
    rumboot_printf("value = %x\n", value);
}

inline static void enable_machine_check()
{
    msr_write(msr_read() | (1<<ITRPT_XSR_ME_i)); /* MSR[ME] - Machine check enable.     */
}

static void exit_from_machine_check_interrupt(uint32_t MCSR_value)
{
    spr_write(SPR_MCSR_C,MCSR_value);
}

static void check_mc_status_soft(uint32_t mc_interrupt_status)
{
    rumboot_printf("mc_interrupt_status = %x\n",mc_interrupt_status);

    TEST_ASSERT(mc_interrupt_status & (1 << ITRPT_MCSR_MCS_i), "Failed to set machine check interrupt");
    if (mc_interrupt_status & (1<<ITRPT_MCSR_TLB_i))
    {
        spr_write(SPR_CCR1 , spr_read(SPR_CCR1) & ~((1 << CTRL_CCR1_MMUDPEI_i)
                                                 | (1 << CTRL_CCR1_MMUTPEI_i)));
        isync();

        write_tlb_entries(&em0_0_cache_on_valid,1);//restore caching tlb
        SET_BIT(MC_HANDLED, ITRPT_MCSR_TLB_i);
        rumboot_printf("detected 'UTLB parity error' interrupt\n");
    }
    if (mc_interrupt_status & (1<<ITRPT_MCSR_IC_i))
    {
        ici (0); dci(0); dci(2);
        isync();

        SET_BIT(MC_HANDLED, ITRPT_MCSR_IC_i);
        rumboot_printf("detected 'I-cache asynchronous error' interrupt\n");
    }
    if (mc_interrupt_status & (1<<ITRPT_MCSR_DC_i))
    {
        spr_write(SPR_CCR1 , spr_read(SPR_CCR1) | (1 << CTRL_CCR1_DPC_i));
        isync();

        SET_BIT(MC_HANDLED, ITRPT_MCSR_DC_i);
        rumboot_printf("detected 'D-cache error' interrupt\n");
    }
    if (mc_interrupt_status & (1<<ITRPT_MCSR_FPR_i))
    {
        msr_write( msr_read() | (1<<ITRPT_XSR_FP_i));

        asm volatile   (
            "lfs 1, 0(%0)\n\t"
            "lfs 2, 0(%0)\n\t"
            "msync\n\t"
            "fcmpu 0, 1, 2\n\t"
            ::"r"(&mem_test_data)
            :"1","2"
        );

        SET_BIT(MC_HANDLED, ITRPT_MCSR_FPR_i);
        rumboot_printf("detected 'FPR parity error' interrupt\n");
    }

    if (mc_interrupt_status & (1<<ITRPT_MCSR_IMP_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_IMP_i);
        rumboot_printf("detected 'Imprecise machine check' interrupt\n");
    }

    if (mc_interrupt_status & (1<<ITRPT_MCSR_L2_i))
    {
        dci(0); dci(2);
        msync();
        isync();

        l2c_l2_write( DCR_L2C_BASE, L2C_L2PLBSTAT1, 0xFFFFFFFF );

        SET_BIT(MC_HANDLED, ITRPT_MCSR_L2_i);
        rumboot_printf("detected 'Error reported through the L2 cache' interrupt\n");
    }
    if (mc_interrupt_status & (1<<ITRPT_MCSR_DCR_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_DCR_i);
        rumboot_printf("detected 'DCR timeout' interrupt\n");
    }
    msync();
}

static void machine_check_interrupt_handler_soft(int id, const char *name)
{
    rumboot_printf("\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
    if(id == RUMBOOT_IRQ_MACHINE_CHECK) {
        uint32_t mc_interrupt_status;
        mc_interrupt_status = spr_read(SPR_MCSR_RW);
        check_mc_status_soft(mc_interrupt_status);
        exit_from_machine_check_interrupt(mc_interrupt_status);
    } else {
        rumboot_printf("--- Guru Meditation ---\n");
        rumboot_printf("MSR:  0x%x\n", msr_read());
        rumboot_printf("SRR0: 0x%x\n", spr_read(SPR_SRR0));
        rumboot_printf("SRR1: 0x%x\n", spr_read(SPR_SRR1));
        rumboot_printf("CSRR0: 0x%x\n", spr_read(SPR_CSRR0));
        rumboot_printf("CSRR1: 0x%x\n", spr_read(SPR_CSRR1));
        rumboot_printf("MCSRR0: 0x%x\n", spr_read(SPR_MCSRR0));
        rumboot_printf("MCSRR1: 0x%x\n", spr_read(SPR_MCSRR1));
        rumboot_printf("---       ---       ---\n");
        rumboot_platform_panic("Please reset or power-cycle the board\n");
    }
}

static void setup_machine_check_interrupt(void (*handler)(int id, const char *name))
{
    rumboot_irq_set_exception_handler(handler);
}

void test_setup_soft()
{
    rumboot_printf("Check interrupts created by CCR1\n");



    setup_machine_check_interrupt(machine_check_interrupt_handler_soft);
    write_tlb_entries(&em0_0_cache_off_valid,1);//set em0_0 cache_off tlb_entry

    emi_init(DCR_EM2_EMI_BASE);

    write_tlb_entries(&em0_0_cache_on_valid,1);//set em0_0 cache_on tlb_entry
}

static void generate_TLB_mc()
{
    rumboot_printf("Check 'UTLB parity error' generation with CCR1_MMUPEI field\n");

    uint32_t test_tlb_addr;

    spr_write(SPR_CCR1 , spr_read(SPR_CCR1) | (0b1 << CTRL_CCR1_MMUDPEI_i)
                                            | (0b1 << CTRL_CCR1_MMUTPEI_i)); //enable_mmu_parity_error_insert
    isync();

    write_tlb_entries(&em0_0_cache_off_valid,1);//set tlb with errors
    test_tlb_addr = ioread32 (SRAM0_BASE);
    rumboot_printf("test_tlb_addr = %d\n",test_tlb_addr);
}

static void generate_IC_mc()
{
    rumboot_printf("Check 'I-cache asynchronous error' generation with CCR1_ICPEI field\n");
    gen_IC_exception_asm();
}

static void generate_DC_mc()
{
    rumboot_printf("Check 'D-cache error' generation with CCR1_DCPEI field\n");
    uint8_t* data_addr = (uint8_t*)rumboot_malloc_from_named_heap_aligned( "SRAM0", 256, 8 );
    rumboot_memfill8(data_addr, 256, 0x00, 1);
    msync();

    asm volatile   (
        "dcbi 0,%0\n\t"
        "msync\n\t"
        "mtspr %3, %1\n\t"
        "isync\n\t"
        "dcbt 0,0,%0\n\t"
        "msync\n\t"
        "mtspr %3, %2\n\t"
        "isync\n\t"
        ::"r"((uint32_t) data_addr), "r"(0b11 << CTRL_CCR1_DCDPEI_i), "r"(0), "i"(SPR_CCR1)
        :
    );

    rumboot_printf("Check value\n");
    rumboot_printf("value = %x\n", ioread32((uint32_t) data_addr));
}

static void generate_FPR_mc()
{
    rumboot_printf("Check 'FPR parity error' generation with CCR1_FPRPEI field\n");

    asm volatile   (
        "lfs 1, 0(%0)\n\t"
        "lfs 2, 0(%0)\n\t"
        "msync\n\t"
        "mtspr %3, %1 \n\t"
        "isync\n\t"
        "lfs 1, 0(%0)\n\t"
        "lfs 2, 0(%0)\n\t"
        "msync\n\t"
        "mtspr %3, %2 \n\t"
        "isync\n\t"
        "fcmpu 0, 1, 2\n\t"
        ::"r"(&mem_test_data), "r"(0b11 << CTRL_CCR1_FPRPEI_i), "r"(0), "i"(SPR_CCR1)
        :"1","2"
    );

}

static void generate_DCR_and_IMP_mc()
{
    spr_write(SPR_CCR2 , spr_read(SPR_CCR2) | (1 << CTRL_CCR2_MCDTO_i)); //enable_machine_check_on_dcr_timeout
    dcr_read(UNEXIST_DCR_ADDR);
}

static void generate_DCR_mc ()
{
    rumboot_printf("Check 'DCR timeout' generation\n");
    generate_DCR_and_IMP_mc();
}

static void generate_IMP_mc ()
{
    rumboot_printf("Check 'Imprecise machine check' generation\n");
    generate_DCR_and_IMP_mc();
}

static void generate_L2C_mc()
{
    rumboot_printf("Check 'L2C error' soft generation \n");
    write_tlb_entries(&ss2_reserved_valid,1); 
    msync();
    iowrite32(ioread32(EA_SS2_RESERVED) | 0xF000F001, EA_SS2_RESERVED); 
}

static void wait_interrupt(ITRPT_MCSR_FIELD MCSR_bit)
{
    TEST_WAIT_ASSERT ((MC_HANDLED & (1 << MCSR_bit)), TEST_OI10_CPU_025_PPC_TIMEOUT, "Interrupt handle timeout!");
    rumboot_printf("MC_HANDLED = %x\n",MC_HANDLED);
    rumboot_printf("MCSR_bit = %d\n",MCSR_bit);
}

void check_mc_with_soft(ITRPT_MCSR_FIELD MCSR_bit)
{
    MC_HANDLED = 0;
    msync();
    enable_machine_check();
    switch (MCSR_bit)
    {
        case ITRPT_MCSR_TLB_i:    generate_TLB_mc();
                            rumboot_printf("wait interrupt from MCSR_TLB\n");
                            break;
        case ITRPT_MCSR_IC_i:     generate_IC_mc();
                            rumboot_printf("wait interrupt from MCSR_IC\n");
                            break;
        case ITRPT_MCSR_DC_i:     generate_DC_mc();
                            rumboot_printf("wait interrupt from MCSR_DC\n");
                            break;
        case ITRPT_MCSR_FPR_i:    generate_FPR_mc();
                            rumboot_printf("wait interrupt from MCSR_FPR\n");
                            break;
        case ITRPT_MCSR_DCR_i:    generate_DCR_mc ();
                            rumboot_printf("wait interrupt from MCSR_DCR\n");
                            break;
        case ITRPT_MCSR_IMP_i:    generate_IMP_mc();
                            rumboot_printf("wait interrupt from MCSR_DCR\n");
                            break;
        case ITRPT_MCSR_L2_i: generate_L2C_mc();
                            rumboot_printf("wait interrupt from MCSR_L2C\n");
                            break;
        default: break;
    }

    wait_interrupt(MCSR_bit);
}

#ifdef USE_HARDWARE_PARTS
void check_mc_status_inj(uint32_t mc_interrupt_status_inj)
{
    TEST_ASSERT(mc_interrupt_status_inj & (1 << ITRPT_MCSR_MCS_i), "Failed to set machine check interrupt");
    if (mc_interrupt_status_inj & (1<<ITRPT_MCSR_TLB_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_TLB_i);
        test_event(EVENT_CLEAR_TLB_MC);
        rumboot_printf("detected 'UTLB parity error' interrupt\n");
    }
    if (mc_interrupt_status_inj & (1<<ITRPT_MCSR_IC_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_IC_i);
        test_event(EVENT_CLEAR_IC_MC);
        rumboot_printf("detected 'I-cache asynchronous error' interrupt\n");
    }
    if (mc_interrupt_status_inj & (1<<ITRPT_MCSR_DC_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_DC_i);
        test_event(EVENT_CLEAR_DC_MC);
        rumboot_printf("detected 'D-cache error' interrupt\n");
    }
    if (mc_interrupt_status_inj & (1<<ITRPT_MCSR_GPR_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_GPR_i);
        test_event(EVENT_CLEAR_GPR_MC);
        rumboot_printf("detected 'GPR parity error' interrupt\n");
    }
    if (mc_interrupt_status_inj & (1<<ITRPT_MCSR_FPR_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_FPR_i);
        test_event(EVENT_CLEAR_FPR_MC);
        rumboot_printf("detected 'FPR parity error' interrupt\n");
    }
    if (mc_interrupt_status_inj & (1<<ITRPT_MCSR_L2_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_L2_i);
        test_event(EVENT_CLEAR_L2_MC);
        rumboot_printf("detected 'Error reported through the L2 cache' interrupt\n");
    }
    if (mc_interrupt_status_inj & (1<<ITRPT_MCSR_DCR_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_DCR_i);
        test_event(EVENT_CLEAR_DCR_MC);
        rumboot_printf("detected 'DCR timeout' interrupt\n");
    }
}

void machine_check_interrupt_handler_inj(int id, const char *name)
{
    rumboot_printf("\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
    if(id == RUMBOOT_IRQ_MACHINE_CHECK) {
        uint32_t mc_interrupt_status_inj;
        mc_interrupt_status_inj = spr_read(SPR_MCSR_RW);
        check_mc_status_inj(mc_interrupt_status_inj);
        exit_from_machine_check_interrupt(mc_interrupt_status_inj);
    } else {
        rumboot_printf("--- Guru Meditation ---\n");
        rumboot_printf("MSR:  0x%x\n", msr_read());
        rumboot_printf("SRR0: 0x%x\n", spr_read(SPR_SRR0));
        rumboot_printf("SRR1: 0x%x\n", spr_read(SPR_SRR1));
        rumboot_printf("CSRR0: 0x%x\n", spr_read(SPR_CSRR0));
        rumboot_printf("CSRR1: 0x%x\n", spr_read(SPR_CSRR1));
        rumboot_printf("MCSRR0: 0x%x\n", spr_read(SPR_MCSRR0));
        rumboot_printf("MCSRR1: 0x%x\n", spr_read(SPR_MCSRR1));
        rumboot_printf("---       ---       ---\n");
        rumboot_platform_panic("Please reset or power-cycle the board\n");
    }
}

void test_setup_inj()
{
    rumboot_printf("Check interrupts created by injectors\n");
    MC_HANDLED = 0;
    setup_machine_check_interrupt(machine_check_interrupt_handler_inj);
}

static void wait_MCSR_value(ITRPT_MCSR_FIELD MCSR_bit)
{
    TEST_WAIT_ASSERT (spr_read(SPR_MCSR_RW) & (1 << MCSR_bit), TEST_OI10_CPU_025_PPC_TIMEOUT, "Failed waiting MCSR value!");

    if (MCSR_bit==ITRPT_MCSR_IMP_i) {
        test_event(EVENT_CLEAR_IMP_MC);
        rumboot_printf("Writing 0x00 to ESR\n");
        spr_write(SPR_ESR , 0x00);
    }

    spr_write(SPR_MCSR_C , 1 << MCSR_bit);
}

void check_mc_with_injector(uint32_t event_code, ITRPT_MCSR_FIELD MCSR_bit)
{
    if (MCSR_bit == ITRPT_MCSR_DCR_i )
    spr_write(SPR_CCR2 , spr_read(SPR_CCR2) | (1 << CTRL_CCR2_MCDTO_i)); //enable_machine_check_on_dcr_timeout

    enable_machine_check();
    test_event(event_code);

    (MCSR_bit==ITRPT_MCSR_IMP_i) ? wait_MCSR_value(MCSR_bit):wait_interrupt(MCSR_bit);
}
#endif

int main ()
{
    test_event_send_test_id( "test_oi10_cpu_025_ppc");

#ifdef USE_HARDWARE_PARTS
    test_setup_inj();
    check_mc_with_injector( EVENT_GENERATE_TLB_MC, ITRPT_MCSR_TLB_i  );
    check_mc_with_injector( EVENT_GENERATE_IC_MC,  ITRPT_MCSR_IC_i   );
    check_mc_with_injector( EVENT_GENERATE_DC_MC,  ITRPT_MCSR_DC_i   );
    check_mc_with_injector( EVENT_GENERATE_GPR_MC, ITRPT_MCSR_GPR_i  );
    check_mc_with_injector( EVENT_GENERATE_FPR_MC, ITRPT_MCSR_FPR_i  );
    check_mc_with_injector( EVENT_GENERATE_IMP_MC, ITRPT_MCSR_IMP_i  );
    check_mc_with_injector( EVENT_GENERATE_L2_MC,  ITRPT_MCSR_L2_i   );
    check_mc_with_injector( EVENT_GENERATE_DCR_MC, ITRPT_MCSR_DCR_i  );
#endif

    test_setup_soft();
    check_mc_with_soft(ITRPT_MCSR_TLB_i);
    check_mc_with_soft(ITRPT_MCSR_IC_i);
    check_mc_with_soft(ITRPT_MCSR_DC_i);
    check_mc_with_soft(ITRPT_MCSR_FPR_i);
    check_mc_with_soft(ITRPT_MCSR_DCR_i);
    check_mc_with_soft(ITRPT_MCSR_IMP_i);
    check_mc_with_soft(ITRPT_MCSR_L2_i);

    return 0;
}
