
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>


#define test_read_ID
#define test_LSCB0
#define test_LSCB1

#define LSCB0_BASE          0x3C050000
#define LSCB1_BASE          0x3C055000
#define LSIF1_MGPIO0_BASE   0x3C067000
#define LSIF1_MGPIO1_BASE   0x3C068000
#define LSIF1_MGPIO2_BASE   0x3C069000

#define GPIO0_BASE          0x3C065000
#define LSCB0_M0_IRQ_s      105
#define LSCB1_M1_IRQ_s      68

/* GPIO regs */
#define GPIODATA            0x000
#define GPIODIR             0x400
#define GPIOIS              0x404
#define GPIOIBE             0x408
#define GPIOIEV             0x40C
#define GPIOIE              0x410
#define GPIORIS             0x414
#define GPIOMIS             0x418
#define GPIOIC              0x41C
#define GPIOAFSEL           0x420
#define GPIOPeriphID0       0xFE0
#define GPIOPeriphID1       0xFE4
#define GPIOPeriphID2       0xFE8
#define GPIOPeriphID3       0xFEC
#define GPIOPCellID0        0xFF0
#define GPIOPCellID1        0xFF4
#define GPIOPCellID2        0xFF8
#define GPIOPCellID3        0xFFC

#define LCSB_TIMEOUT        0x1000

#define TEST_OK             0x0000
#define TEST_ERROR          0x0001

#define rumboot_puthex(VAL)     \
    rumboot_printf("0x%X", (uint32_t)(VAL))

#define MEM32(ADDR)         *((uint32_t*)(ADDR))

static volatile uint32_t IRQ;

/*
void NON_CR_INTERRUPT_HANDLER(void)
{
	rumboot_putstring ("NON_CR_INTERRUPT_HANDLER \n");
    uint32_t source_vector = mpic_get_ncr_interrupt_vector(Processor0);
    rumboot_putstring("NON_CRITICAL int handler message ");
    rumboot_puthex (source_vector);
    if (source_vector == LSCB0_M0_IRQ_s)
    {
        rumboot_putstring("source_vector == LSCB0_M0_IRQ_s \n");
        IRQ = 1;
    }
    if (source_vector == LSCB1_M1_IRQ_s)
    {
        rumboot_putstring("source_vector == LSCB1_M1_IRQ_s \n");
        IRQ = 1;
    }
	END_NONCR_INT_P0;
}
*/


/*
void lscb_irq_set()
{
    // Initialization Inrerrupt controller
	IRQ = 0;
    rumboot_putstring ("    Start MPIC initialization  \n");
    mpic_reset();
    mpic_pass_through_disable();
    mpic_setup_ext_interrupt(LSCB0_M0_IRQ_s, MPIC_PRIOR_1,int_sense_edge,int_pol_pos,Processor0);
    mpic_setup_ext_interrupt(LSCB1_M1_IRQ_s, MPIC_PRIOR_1,int_sense_edge,int_pol_pos,Processor0);
	SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
    ppc_noncr_int_enable();
    mpic_set_interrupt_borders(10, 5); // MCK, CR borders
    mpic_set_minimal_interrupt_priority_mask(Processor0, MPIC_PRIOR_0);
    rumboot_putstring ("    MPIC ave been initialized  \n");
}
*/

int wait_lscb_int()
{
    int i;
	
	rumboot_putstring ("wait_lscb_int \n");
    for (i=1;i<=LCSB_TIMEOUT;i++)
    {
        if (IRQ==1)
        {
            IRQ = 0;
            break;
        }
    }
    if (i>=LCSB_TIMEOUT)
    {
        rumboot_putstring("Error! IRQ flag wait timeout!");
        return TEST_ERROR;
    }
    return TEST_OK;
}

//---------------------------------
int main(void)
{
	uint32_t    test_result,
	            reg_access,
	            read_mem,
	            read_lscb_tx_int;
//	uint32_t    read_config;

    test_result = TEST_OK;
#ifdef test_read_ID
    rumboot_putstring("---------- test_read_ID LSCB ---------- \n");
    MEM32(LSCB0_BASE + 0x0004) = 0x0000001F;
    read_mem = MEM32(LSCB0_BASE + 0x0004);
    rumboot_printf("test_read_ID LSCB0 write=0x0000001F read=0x%X\n",
	        read_mem);
    MEM32(LSCB1_BASE + 0x0004) = 0x0000001F;
    read_mem = MEM32(LSCB1_BASE + 0x0004);
    rumboot_printf("test_read_ID LSCB1 write=0x0000001Ff read=0x%X\n",
	        read_mem);
#endif
		
	rumboot_putstring("---------- Start test LSCB ---------- \n");
	rumboot_putstring("------------- init gpio ------------- \n");
	MEM32(LSIF1_MGPIO0_BASE + GPIOAFSEL) = 0xFF;
	MEM32(LSIF1_MGPIO1_BASE + GPIOAFSEL) = 0xFF;
	MEM32(LSIF1_MGPIO2_BASE + GPIOAFSEL) = 0xFF;
	
	MEM32(GPIO0_BASE + GPIOAFSEL) = 0x00;	
	
	// lscb_irq_set();
	
#ifdef test_LSCB0	
	rumboot_putstring("------------ TEST LSCB0 ------------- \n");
	rumboot_putstring("------ write config: 0x0000011F ----- \n");
	MEM32(LSCB0_BASE + 0x0004) = 0x0000011F;
/*
	rumboot_putstring("------------ read config ------------ \n");
	read_config = MEM32(LSCB0_BASE+0x0004);
	rumboot_putstring(" read_config: ");
    rumboot_puthex (read_config);
*/	
	rumboot_putstring("-- read gpio for test lscb0_tx_int=1 (AB) -- \n");
	read_lscb_tx_int = MEM32(GPIO0_BASE+0x3fc);
	rumboot_putstring ("read_lscb_tx_int: ");
	rumboot_puthex(read_lscb_tx_int);
	if ((read_lscb_tx_int & 0x0000003) != 0x0000003) return TEST_ERROR;
	
	rumboot_putstring("------------ write reg ------------- \n");
	MEM32(LSCB0_BASE) = 0xC0030001;
	rumboot_putstring("write srr  (A 0x0003;  D 0x0001) \n");
	do {
    reg_access = MEM32(LSCB0_BASE);
//	rumboot_putstring("0 reg_access: ");
//   rumboot_puthex (reg_access);
	} while (reg_access >> 31);

/*	
	rumboot_putstring("-- read reg cfg5_: test msp_rtad[4:0] & msp_rtadp -- \n");
	MEM32(LSCB0_BASE) =0x80090000;
	rumboot_putstring("read reg cfg5_  \n");
	do {
    reg_access = MEM32(LSCB0_BASE);
	rumboot_putstring("reg_access: ");
    rumboot_puthex (reg_access);
	} while ( reg_access>>31 != 0 );
	rumboot_putstring("cfg5_ = ");
    rumboot_puthex (reg_access);
*/
	
	MEM32(LSCB0_BASE) = 0xC0078000;
	rumboot_putstring("write cfg3 (A 0x0007;  D 0x8000) \n");
	do
	{
	    reg_access = MEM32(LSCB0_BASE);
//	    rumboot_putstring("1 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	MEM32(LSCB0_BASE) =0xC00101E0;
	rumboot_putstring("write cfg1 (A 0x0001;  D 0x01E0) \n");
	do
	{
	    reg_access = MEM32(LSCB0_BASE);
//	    rumboot_putstring("2 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	MEM32(LSCB0_BASE) =0xC0028000;
	rumboot_putstring("write cfg2 (A 0x0002;  D 0x8000) \n");
	do
	{
	    reg_access = MEM32(LSCB0_BASE);
//	    rumboot_putstring("3 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	MEM32(LSCB0_BASE) = 0xC00D0010;
	rumboot_putstring("write bcft (A 0x000d;  D 0x0010) \n");
	do {
	    reg_access = MEM32(LSCB0_BASE);
//	    rumboot_putstring("4 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	MEM32(LSCB0_BASE) = 0xC0000009;
	rumboot_putstring("write imr  (A 0x0000;  D 0x0009) \n");
	do
	{
	    reg_access = MEM32(LSCB0_BASE);
//	    rumboot_putstring("5 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access>>31);

	rumboot_putstring("------------ write mem ------------- \n");
	MEM32(LSCB0_BASE + 0x100C) = 0x0154	;
	MEM32(LSCB0_BASE + 0x1008) = 0x00DC	;
	MEM32(LSCB0_BASE + 0x101C) = 0x0180	;
	MEM32(LSCB0_BASE + 0x1018) = 0x00DC	;
	
	MEM32(LSCB0_BASE + 0x1550) = 0x0082	;
	MEM32(LSCB0_BASE + 0x1554) = 0xf821	;
	MEM32(LSCB0_BASE + 0x1558) = 0xA5A5	;
	
	MEM32(LSCB0_BASE + 0x1600) = 0x0002	;
	MEM32(LSCB0_BASE + 0x1604) = 0xf821	;
	MEM32(LSCB0_BASE + 0x1608) = 0x5A5A	;
		                       
	MEM32(LSCB0_BASE + 0x1400) = 0x0000	;
	MEM32(LSCB0_BASE + 0x1404) = 0xFFFD	;
	MEM32(LSCB0_BASE + 0x1408) = 0x0000	;
	MEM32(LSCB0_BASE + 0x140c) = 0xFFFD	;
	rumboot_putstring("------------ write reg ------------- \n");
	MEM32(LSCB0_BASE) =0xC0030002;
	rumboot_putstring("write srr  (A 0x0003;  D 0x0002) \n");
	do
	{
        reg_access = MEM32(LSCB0_BASE);
//	    rumboot_putstring("6 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
//!!!!!!!!!!!!!!!!!
	rumboot_putstring("--- start: msp_ssflag_ext_trig 0->1 --- \n");
	MEM32(LSCB0_BASE + 0x0004) = 0x0000001F;
	MEM32(LSCB0_BASE + 0x0004) = 0x0000011F;
//!!!!!!!!!!!!!!!!!	
	
	rumboot_putstring("-- read gpio for test lscb0_tx_int_a0 1 -> 0 -- \n");
	do
	{
		read_lscb_tx_int = MEM32(GPIO0_BASE+0x3FC);
		rumboot_printf("read_lscb_tx_int: 0x%X\n", read_lscb_tx_int);
	} while (read_lscb_tx_int & 0x00000001);

    test_result = wait_lscb_int();
    if (test_result) return TEST_ERROR;
	
	read_mem = MEM32(LSCB0_BASE + 0x1000);
	rumboot_putstring("Read reg 000: ");
	rumboot_puthex (read_mem);
	if(read_mem != 0x8000) return TEST_ERROR;
	
	rumboot_putstring("------------ read word loop_test  LSCB0 ch A -------------- \n");
//	read_mem = MEM32(LSCB0_BASE+0x1558);
//	rumboot_putstring("Read mem 0x1558: ");
//	rumboot_puthex (read_mem);
	read_mem = MEM32(LSCB0_BASE + 0x155C);
	rumboot_printf("Read mem 0x155C: 0x%X\n", read_mem);
	
	rumboot_putstring("-- read gpio for test lscb0_tx_int_b0 1 -> 0 -- \n");
	do
	{
		read_lscb_tx_int = MEM32(GPIO0_BASE + 0x03FC);
//		rumboot_putstring ("read_lscb_tx_int: ");
//		rumboot_puthex(read_lscb_tx_int);
	} while ((read_lscb_tx_int & 0x00000002) == 0x00000002);
	
	test_result = wait_lscb_int();
	if(test_result) return TEST_ERROR;
	
	read_mem = MEM32(LSCB0_BASE + 0x1010);
	rumboot_printf("Read reg 004: 0x%X\n", read_mem);
	if(read_mem != 0xA000) return TEST_ERROR;
	
	rumboot_putstring("------------ read word loop_test  LSCB0 ch B -------------- \n");
//	read_mem = MEM32(LSCB0_BASE+0x1608);
//	rumboot_putstring("Read mem 0x1608: ");
//	rumboot_puthex (read_mem);
	read_mem = MEM32(LSCB0_BASE+0x160C);
	rumboot_printf("Read mem 0x160C: 0x%X\n", read_mem);
	
#endif	
	
#ifdef test_LSCB1
	rumboot_putstring("------------ TEST LSCB1 ------------- \n");
	rumboot_putstring("------------ write config ----------- \n");
	MEM32(LSCB1_BASE + 0x0004) =0x0000011F;
/*	
	rumboot_putstring("------------ read config ------------ \n");
	read_config = MEM32(LSCB1_BASE+0x0004);
	rumboot_putstring(" read_config: ");
    rumboot_puthex (read_config);
*/	
	rumboot_putstring("-- read gpio for test lscb1_tx_int=1 (AB) -- \n");
	read_lscb_tx_int = MEM32(GPIO0_BASE + 0x03FC);
	rumboot_printf("read_lscb_tx_int: 0x%X\n", read_lscb_tx_int);
	if ((read_lscb_tx_int & 0x0000000C) != 0x0000000C) return TEST_ERROR;
	rumboot_putstring("------------ write reg ------------- \n");
	MEM32(LSCB1_BASE) = 0xC0030001;
	rumboot_putstring("write srr  (A 0x0003;  D 0x0001) \n");
	do
	{
	    reg_access = MEM32(LSCB1_BASE);
//	    rumboot_putstring("0 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	MEM32(LSCB1_BASE) =0xC0078000;
	rumboot_putstring("write cfg3 (A 0x0007;  D 0x8000) \n");
	do
	{
	    reg_access = MEM32(LSCB1_BASE);
//	    rumboot_putstring("1 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	MEM32(LSCB1_BASE) = 0xC0010160;
	rumboot_putstring("write cfg1 (A 0x0001;  D 0x0160) \n");
	do
	{
	    reg_access = MEM32(LSCB1_BASE);
//	    rumboot_putstring("2 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	MEM32(LSCB1_BASE) = 0xC0028000;
	rumboot_putstring("write cfg2 (A 0x0002;  D 0x8000) \n");
	do
	{
	    reg_access = MEM32(LSCB1_BASE);
//	    rumboot_putstring("3 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	MEM32(LSCB1_BASE) = 0xC00D0010;
	rumboot_putstring("write bcft (A 0x000d;  D 0x0010) \n");
	do
	{
	    reg_access = MEM32(LSCB1_BASE);
//	    rumboot_putstring("4 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	MEM32(LSCB1_BASE) = 0xC0000009;
	rumboot_putstring("write imr  (A 0x0000;  D 0x0009) \n");
	do
	{
	    reg_access = MEM32(LSCB1_BASE);
//	    rumboot_putstring("5 reg_access: ");
//      rumboot_puthex (reg_access);
	} while (reg_access >> 31);

	rumboot_putstring("------------ write mem ------------- \n");
	MEM32(LSCB1_BASE + 0x100C) = 0x0154;
	MEM32(LSCB1_BASE + 0x1008) = 0x00DC;
	MEM32(LSCB1_BASE + 0x101C) = 0x0180;
	MEM32(LSCB1_BASE + 0x1018) = 0x00DC;
	
	MEM32(LSCB1_BASE + 0x1550) = 0x0082;
	MEM32(LSCB1_BASE + 0x1554) = 0xF821;
	MEM32(LSCB1_BASE + 0x1558) = 0xA5A5;
	
	MEM32(LSCB1_BASE + 0x1600) = 0x0002;
	MEM32(LSCB1_BASE + 0x1604) = 0xF821;
	MEM32(LSCB1_BASE + 0x1608) = 0x5A5A;
		                       
	MEM32(LSCB1_BASE + 0x1400) = 0x0000;
	MEM32(LSCB1_BASE + 0x1404) = 0xFFFD;
	MEM32(LSCB1_BASE + 0x1408) = 0x0000;
	MEM32(LSCB1_BASE + 0x140c) = 0xFFFD;
	
	rumboot_putstring("------------ write reg ------------- \n");
	MEM32(LSCB1_BASE) = 0xC0030002;
	rumboot_putstring("write srr  (A 0x0003;  D 0x0002) \n");
	do {
    reg_access = MEM32(LSCB1_BASE);
//	rumboot_putstring("6 reg_access: ");
//  rumboot_puthex (reg_access);
	} while (reg_access >> 31);
	
	rumboot_putstring("-- read gpio for test lscb1_tx_int_a1 1 -> 0 -- \n");
	do
	{
		read_lscb_tx_int = MEM32(GPIO0_BASE + 0x03FC);
//		rumboot_putstring ("read_lscb_tx_int: ");
//		rumboot_puthex(read_lscb_tx_int);
	} while ((read_lscb_tx_int & 0x00000004) == 0x00000004);
	
	test_result = wait_lscb_int();
	if (test_result) return TEST_ERROR;
	
	read_mem = MEM32(LSCB1_BASE + 0x1000);
	rumboot_printf("Read reg 000: 0x%X\n", read_mem);
	if (read_mem != 0x8000) return TEST_ERROR;
	
	rumboot_putstring("------------ read word loop_test  LSCB1 ch A -------------- \n");
//	read_mem = MEM32(LSCB1_BASE+0x1558);
//	rumboot_putstring("Read mem 0x1558: ");
//	rumboot_puthex (read_mem);
	read_mem = MEM32(LSCB1_BASE+0x155c);
	rumboot_printf("Read mem 0x155C: %0x%X\n", read_mem);
	
	rumboot_putstring("-- read gpio for test lscb1_tx_int_b1 1 -> 0 -- \n");
	do
	{
		read_lscb_tx_int = MEM32(GPIO0_BASE + 0x03FC);
//		rumboot_putstring ("read_lscb_tx_int: ");
//		rumboot_puthex(read_lscb_tx_int);
	} while ((read_lscb_tx_int & 0x00000008) == 0x00000008);
	
	test_result = wait_lscb_int();
	if (test_result) return TEST_ERROR;
	
	read_mem = MEM32(LSCB1_BASE + 0x1010);
	rumboot_printf("Read reg 004: 0x%X\n", read_mem);
	if (read_mem != 0xA000) return TEST_ERROR;
	
	rumboot_putstring("------------ read word loop_test LSCB1 ch B -------------- \n");
//	read_mem = MEM32(LSCB1_BASE+0x1608);
//	rumboot_putstring("Read mem 0x1608: ");
//	rumboot_puthex (read_mem);
	read_mem = MEM32(LSCB1_BASE + 0x160C);
	rumboot_printf("Read mem 0x160C: 0x%X\n", read_mem);
#endif

   return !!test_result;
}

