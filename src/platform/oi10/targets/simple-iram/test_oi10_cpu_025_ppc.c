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
#include <platform/test_event_c.h>
#include <rumboot/irq.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/interrupts.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

//                                          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,              V   )
#define TLB_ENTRY_EM0_0_NOCACHE_VALID       MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_EM0_0_NOCACHE_INVALID     MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b0 )
#define TLB_ENTRY_EM0_0_CACHE_VALID         MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b1,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,  MMU_TLBWE_BE_UND,   0b1 )
#define TLB_ENTRY_BOOTROM_MIRROR_0          MMU_TLB_ENTRY(  0x01F,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,  MMU_TLBWE_BE_0,     0b1 )
//                                  .long   MMU_TLB_ENTRY(  0x01F,  0xFFFF0,    0xFFFF0,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b0,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_0,     0b1 )
//                                  .long   MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )


#define UNEXIST_DCR_ADDR                0x00
#define TEST_MPW_CPU_025_PPC_TIMEOUT    100

static const tlb_entry bootrom_mirror           = {TLB_ENTRY_BOOTROM_MIRROR_0};
static const tlb_entry em0_0_cache_on_valid     = {TLB_ENTRY_EM0_0_CACHE_VALID};
//static const tlb_entry em0_0_cache_off_valid    = {TLB_ENTRY_EM0_0_NOCACHE_VALID};
//static const tlb_entry em0_0_cache_off_invalid  = {TLB_ENTRY_EM0_0_NOCACHE_INVALID};
static const tlb_entry em0_0_cache_off_valid    = {TLB_ENTRY_EM0_0_NOCACHE_VALID};


volatile uint32_t dcu_cached_test_data __attribute__((section(".data"), aligned(0x100))) = 0xAAAA5555;

volatile uint32_t mem_test_data __attribute__((section(".data"))) = 0x5555AAAA;

volatile uint32_t MC_HANDLED __attribute__((section(".data")))   = 0;

void __attribute__((section(".text"), aligned(0x100))) cached_func_from_EM0()
{
    rumboot_printf("execute cached_func_from_EM0\n");
}


void __attribute__((section(".text"), aligned(0x100))) gen_IC_exception_asm()
{
    rumboot_printf("generate IC exception\n");
    asm volatile   (
        "icbi 0,%0\n\t"
        "msync\n\t"
        "mtspr 888, %1\n\t"
        "isync\n\t"
        "icbt 0,0,%0\n\t"
        "msync\n\t"
        "mtspr 888, %2\n\t"
        "isync\n\t"
        ::"r"((uint32_t) cached_func_from_EM0), "r"(2 << CTRL_CCR1_ICDPEI_i), "r"(0)
        :
    );
    cached_func_from_EM0();
}

static void load_code_in_cache(uint32_t start_addr, uint32_t code_size)
{
    uint32_t cur_addr;
    rumboot_printf("Caching exception generation code\n");
    for (cur_addr=start_addr; cur_addr<start_addr+code_size; cur_addr+=0x80)
    {
        rumboot_printf("icbt %x\n", cur_addr);
        icbt((void* const)cur_addr);
    }
}


inline static void enable_machine_check()
{
    msr_write(msr_read() | (1<<ITRPT_XSR_ME_i)); /* MSR[ME] - Machine check enable.     */
}

static void exit_from_machine_check_interrupt(uint32_t MCSR_value)
{
    //SPR_MCSRR0_write((uint32_t) exit_from_test);
    //SPR_MCSRR1_write(msr_read());
    //msr_write(0x0);

    spr_write(SPR_MCSR_C,MCSR_value);

    //SPR_MCSR_C_write(MCSR_value);
}

static void check_mc_status_CCR1(uint32_t mc_interrupt_status)
{
    TEST_ASSERT(mc_interrupt_status & (1 << ITRPT_MCSR_MCS_i), "Failed to set machine check interrupt");
    if (mc_interrupt_status & (1<<ITRPT_MCSR_TLB_i))
    {
        spr_write(SPR_CCR1 , spr_read(SPR_CCR1) & ~((1 << CTRL_CCR1_MMUDPEI_i)
                                                 | (1 << CTRL_CCR1_MMUTPEI_i)));
        isync();

        write_tlb_entries(&bootrom_mirror,1);//fix tlb errors
        SET_BIT(MC_HANDLED, ITRPT_MCSR_TLB_i);
        rumboot_printf("detected 'UTLB parity error' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<ITRPT_MCSR_IC_i))
    {
        spr_write(SPR_CCR1 , spr_read(SPR_CCR1) | (1 << CTRL_CCR1_DPC_i));
        isync();

        SET_BIT(MC_HANDLED, ITRPT_MCSR_IC_i);
        rumboot_printf("detected 'I-cache asynchronous error' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<ITRPT_MCSR_DC_i))
    {
        spr_write(SPR_CCR1 , spr_read(SPR_CCR1) | (1 << CTRL_CCR1_DPC_i));
        isync();

        SET_BIT(MC_HANDLED, ITRPT_MCSR_DC_i);
        rumboot_printf("detected 'D-cache error' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<ITRPT_MCSR_GPR_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_GPR_i);
        rumboot_printf("detected 'GPR parity error' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<ITRPT_MCSR_FPR_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_FPR_i);
        rumboot_printf("detected 'FPR parity error' interrupt\n");
    }
/*
    else if (mc_interrupt_status & (1<<MCSR_IMP_i))
    {
        SET_BIT(MC_HANDLED, MCSR_IMP_i);
        rumboot_printf("detected 'Imprecise machine check' interrupt\n");
    }
*/
    else if (mc_interrupt_status & (1<<ITRPT_MCSR_L2_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_L2_i);
        rumboot_printf("detected 'Error reported through the L2 cache' interrupt\n");
    }
    else if (mc_interrupt_status & (1<<ITRPT_MCSR_DCR_i))
    {
        SET_BIT(MC_HANDLED, ITRPT_MCSR_DCR_i);
        rumboot_printf("detected 'DCR timeout' interrupt\n");
    }
    else
    {
        rumboot_printf("unexpected machine check interrupt\n");
        rumboot_printf("MCSR = %x\n",mc_interrupt_status);
        //test_event(EVENT_ERROR);
    }
}

static void machine_check_interrupt_handler_CCR1(int id, const char *name)
{
    rumboot_printf("\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
    if(id == RUMBOOT_IRQ_MACHINE_CHECK) {
        uint32_t mc_interrupt_status;
        mc_interrupt_status = spr_read(SPR_MCSR_RW);
        check_mc_status_CCR1(mc_interrupt_status);
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

//static void exception_handler(int id, const char *name)
//{
//    rumboot_printf("\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
//    if(id == RUMBOOT_IRQ_MACHINE_CHECK)
//    {
//        rumboot_printf("It is OK!\n");
//        spr_write(SPR_MCSR_C,0xFFFFFFFF);
//    }
//    else
//    {
//        rumboot_printf("--- Guru Meditation ---\n");
//        rumboot_printf("MSR:  0x%x\n", msr_read());
//        rumboot_printf("SRR0: 0x%x\n", spr_read(SPR_SRR0));
//        rumboot_printf("SRR1: 0x%x\n", spr_read(SPR_SRR1));
//        rumboot_printf("CSRR0: 0x%x\n", spr_read(SPR_CSRR0));
//        rumboot_printf("CSRR1: 0x%x\n", spr_read(SPR_CSRR1));
//        rumboot_printf("MCSRR0: 0x%x\n", spr_read(SPR_MCSRR0));
//        rumboot_printf("MCSRR1: 0x%x\n", spr_read(SPR_MCSRR1));
//        rumboot_printf("---       ---       ---\n");
//        rumboot_platform_panic("Please reset or power-cycle the board\n");
//    }
//}

static void setup_machine_check_interrupt(void (*handler)(int id, const char *name))
{
    rumboot_irq_set_exception_handler(handler);

//    irq_init_cpu_int_vec();
//    irq_set_handler(RUMBOOT_IRQ_MACHINE_CHECK, interrupt_handled);

}

void test_setup_CCR1()
{
    rumboot_printf("Check interrupts created by CCR1\n");
    MC_HANDLED = 0;

    //SPR_CCR2_write(SPR_CCR2_read() | (1 << CTRL_CCR2_MCDTO_i));

    spr_write(SPR_CCR2 , spr_read(SPR_CCR2) | (1 << CTRL_CCR2_MCDTO_i));

    setup_machine_check_interrupt(machine_check_interrupt_handler_CCR1);
    write_tlb_entries(&em0_0_cache_off_valid,1);//set em0_0 cache_off tlb_entry
/*
    ddr_init_impl(
            DdrHlbId_Default,
            DdrInitMode_Default,
            DdrEccMode_Default,
            DdrPmMode_Default,
            DdrBurstLength_4,
            DdrPartialWriteMode_ReadModifyWrite);
*/
    emi_init(DCR_EM2_EMI_BASE);

//    write_tlb_entries(&em0_0_cache_off_invalid,1);//invalidate em0_0 cache_off tlb_entry
    write_tlb_entries(&em0_0_cache_on_valid,1);//set em0_0 cache_on tlb_entry
}


static void generate_TLB_mc()
{
    rumboot_printf("Check 'UTLB parity error' generation with CCR1_MMUPEI field\n");

    spr_write(SPR_CCR1 , spr_read(SPR_CCR1) | (1 << CTRL_CCR1_MMUDPEI_i)
                                            | (1 << CTRL_CCR1_MMUTPEI_i));
    isync();

    write_tlb_entries(&bootrom_mirror,1);//set tlb with errors
}

static void generate_IC_mc()
{
    rumboot_printf("Check 'I-cache asynchronous error' generation with CCR1_ICPEI field\n");
    load_code_in_cache((uint32_t)gen_IC_exception_asm, 0x40);
    gen_IC_exception_asm();
}

static void generate_DC_mc()
{
    uint8_t tmp;
    rumboot_printf("Check 'D-cache error' generation with CCR1_DCPEI field\n");
    asm volatile   (
        "dcbt 0, %1 \n\t"
        "msync\n\t"
        "mtspr 888, %2 \n\t"
        "isync\n\t"
        "stb %3, 0(%1) \n\t"
        "msync\n\t"
        "mtspr 888, %4 \n\t"
        "isync\n\t"
        "lbz %0, 0(%1) \n\t"
        :"=r"(tmp)
        :"r"((uint32_t) (&dcu_cached_test_data)), "r"((3 << CTRL_CCR1_DCDPEI_i) | (3 << CTRL_CCR1_DCLPEI_i) | (3 << CTRL_CCR1_DCTPEI_i)) ,"r"(0x77), "r"(0)
        :
    );
    rumboot_printf("tmp = %x\n", tmp);
}

/*
static void generate_GPR_mc()
{
    rumboot_printf("Check 'GPR parity error' generation with CCR1_GPRPEI field\n");
    asm volatile   (
        "lwz 3, 0(%0)\n\t"
        "lwz 4, 0(%0)\n\t"
        "mtspr 888, %1 \n\t"
        "isync\n\t"
        "lwz 3, 0(%0)\n\t"
        "lwz 4, 0(%0)\n\t"
        "isync\n\t"
        "isync\n\t"
        "cmp 0, 0, 3, 4\n\t"
        ::"r"(&mem_test_data), "r"(3 << CTRL_CCR1_GPRPEI_i), "r"(0)
        :"3","4"
    );
}
*/

static void generate_FPR_mc()
{
    rumboot_printf("Check 'FPR parity error' generation with CCR1_FPRPEI field\n");
    msr_write(msr_read() | (1<<ITRPT_XSR_FP_i));
    asm volatile   (
        "lfs 1, 0(%0)\n\t"
        "lfs 2, 0(%0)\n\t"
        "mtspr 888, %1 \n\t"
        "isync\n\t"
        "lfs 1, 0(%0)\n\t"
        "lfs 2, 0(%0)\n\t"
        "isync\n\t"
        "mtspr 888, %2 \n\t"
        "isync\n\t"
        "fcmpu 0, 1, 2\n\t"
        ::"r"(&mem_test_data), "r"(3 << CTRL_CCR1_FPRPEI_i), "r"(0)
        :"1","2"
    );
}

static void generate_DCR_mc()
{
    rumboot_printf("Check 'DCR timeout' generation\n");
    dcr_read(UNEXIST_DCR_ADDR);
}

static void wait_interrupt(ITRPT_MCSR_FIELD MCSR_bit)
{
    uint32_t t = 0;
    while (!(MC_HANDLED & (1 << MCSR_bit)) && t++<TEST_MPW_CPU_025_PPC_TIMEOUT);
    rumboot_printf("MC_HANDLED = %x\n",MC_HANDLED);
    rumboot_printf("MCSR_bit = %x\n",MCSR_bit);
    //TEST_WAIT_ASSERT ((MC_HANDLED & (1 << MCSR_bit)), 100, "Interrupt handle timeout!");
}

void check_mc_with_CCR1(ITRPT_MCSR_FIELD MCSR_bit)
{
    enable_machine_check();
    switch (MCSR_bit)
    {
        case ITRPT_MCSR_TLB_i:    generate_TLB_mc();
                            rumboot_printf("wait interrupt from MCSR_TLB\n");
                            break;
        case ITRPT_MCSR_IC_i:     generate_IC_mc();
                            rumboot_printf("wait interrupt from MCSR_IC\n");
                            break;
        case ITRPT_MCSR_DC_i:
                            //enable_L1_parity_checking();
                            generate_DC_mc();
                            rumboot_printf("wait interrupt from MCSR_DC\n");
                            break;
/*
        case ITRPT_MCSR_GPR_i:    generate_GPR_mc();
                            trace_msg("wait interrupt from MCSR_GPR\n");
                            break;
*/
        case ITRPT_MCSR_FPR_i:    generate_FPR_mc();
                            rumboot_printf("wait interrupt from MCSR_FPR\n");
                            break;
        case ITRPT_MCSR_DCR_i:    generate_DCR_mc();
                            rumboot_printf("wait interrupt from MCSR_DCR\n");
                            break;
        default: break;
    }
    wait_interrupt(MCSR_bit);
}

int main ()
{
    //test_event_send_test_id( "test_oi10_cpu_025_rst");

    rumboot_printf("TEST START\n");

    test_setup_CCR1();

    //check_mc_with_CCR1(ITRPT_MCSR_TLB_i);  //not mc
    //check_mc_with_CCR1(ITRPT_MCSR_IC_i); // mc l2c
    //check_mc_with_CCR1(ITRPT_MCSR_DC_i); // not mc
    //check_mc_with_CCR1(ITRPT_MCSR_FPR_i); // not mc
//    check_mc_with_CCR1(ITRPT_MCSR_GPR_i);
    check_mc_with_CCR1(ITRPT_MCSR_DCR_i);


    rumboot_printf("TEST OK\n");
    return 0;
}
