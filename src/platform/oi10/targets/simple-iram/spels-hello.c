#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <rumboot/memtest.h>
#include <devices/gpio_pl061.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <regs/regs_spi.h>
#include <rumboot/io.h>
#include <regs/regs_uart_pl011.h>
#include <devices/uart_pl011.h>

/* Defaults: Start at the end of spl area, end at the stack position */
#ifndef START
#define START (&rumboot_platform_spl_end)
#endif

#ifndef END
#define END (__builtin_frame_address(0) - 1024);
#endif

#define WITH_ACCUMULATION 0x00
#define WITHOUT_ACCUMULATION 0x01
#define ITER_PAKAGE_TYPE 0x02


#define GPR_AA_TEST_RESULT 		        0
#define GPR_55_TEST_RESULT 		        1
#define RAM_AA_TEST_RESULT 		        2
#define RAM_55_TEST_RESULT 		        3
#define RAM_AA_ECC_TEST_RESULT	        4
#define RAM_55_ECC_TEST_RESULT 	        5
#define CACHE_AA_TEST_RESULT 	        6
#define CACHE_55_TEST_RESULT 	        7
#define ALU_TEST_RESULT 		        8
#define UART_TEST_RESULT 		        9
#define UART_COUNTER_TEST_RESULT 		10
#define UART_LAST_ERROR_TEST_RESULT 	11

#define SIZE 5120

uint32_t results[SIZE];

#define UART_FIFO_SIZE 32
#define UART_UNDER_TEST UART1_BASE 

static uint32_t bit_test(uint32_t reg, uint32_t pattern)
{
	uint32_t i;
	uint32_t mask = reg^pattern, err_bit = 0;
	if(mask != 0)
	{
		for(i=0;i<32;i++)
			if(mask&(1<<i))
					err_bit++;
	}
	return err_bit;
}



uint32_t uart_test(uint32_t pattern, uint32_t *counter, uint32_t *last_error)
{
    uint32_t i = 0, errors = 0, cnt =0, data = 0, temp = 0, error_word = 0;

	for(i=0;i<UART_FIFO_SIZE;i++)
   	{
		if(uart_check_rfifo_empty(UART_UNDER_TEST)!=0)
		{
			data = ioread32(UART_UNDER_TEST+UARTDR) & 0xFF;
			temp = bit_test(data,pattern);
			errors += temp;
			cnt++;
			if(temp!=0)
			{
				error_word = data;
			}
		}
   	}
	*counter = cnt;
	*last_error = error_word;

	for(i=0;i<UART_FIFO_SIZE;i++)
   	{
		if(uart_check_tfifo_full(UART_UNDER_TEST)!=0)
		{
			iowrite32(pattern, UART_UNDER_TEST+UARTDR);
		}
   	}
    return errors;
}

void write_gpr_reg(uint32_t pattern, uint32_t num)
{
    switch(num)
    {
        case 0: spr_write(SPR_USPRG0,pattern);
        case 1: spr_write(SPR_SPRG0,pattern);
        case 2: spr_write(SPR_SPRG1,pattern);
        case 3: spr_write(SPR_SPRG2,pattern);
        case 4: spr_write(SPR_SPRG3,pattern);
        case 5: spr_write(SPR_SPRG4,pattern);
        case 6: spr_write(SPR_SPRG5,pattern);
        case 7: spr_write(SPR_SPRG6,pattern);
        case 8: spr_write(SPR_SPRG7,pattern);
    }
}

uint32_t read_gpr_reg(uint32_t num)
{
   uint32_t reg_val=0;
    switch(num)
    {
        case 0: reg_val = spr_read(SPR_USPRG0);
        case 1: reg_val = spr_read(SPR_SPRG0);
        case 2: reg_val = spr_read(SPR_SPRG1);
        case 3: reg_val = spr_read(SPR_SPRG2);
        case 4: reg_val = spr_read(SPR_SPRG3);
        case 5: reg_val = spr_read(SPR_SPRG4);
        case 6: reg_val = spr_read(SPR_SPRG5);
        case 7: reg_val = spr_read(SPR_SPRG6);
        case 8: reg_val = spr_read(SPR_SPRG7);
        default: reg_val = 0;
    }
   return reg_val;
}

#define GPR_LEN 10

void gpr_fill_pattern(uint32_t pattern)
{
    uint32_t i, len =GPR_LEN;
    for(i=0; i < len; i+=sizeof(pattern))
	{
        write_gpr_reg(pattern,i);
    }
}

uint32_t gpr_check_pattern(uint32_t pattern, bool refresh)
{
	uint32_t errors, buf, reg_val, i, len =GPR_LEN;
	errors = buf = i = reg_val = 0;


	for(i=0; i < len; i+=sizeof(pattern))
	{
		buf = bit_test(read_gpr_reg(i), pattern);
		if(buf)
		{
			errors+=buf;
			if(refresh)
			{
				write_gpr_reg(pattern,i);
			}
		}
	}
	return errors;	
}

#define ALU_CYCLES 100

uint32_t alu_test(void)
{
	register uint32_t j, a = 0xAAAAAAAA, b = 0x55555555, result, errors = 0;
	for(j=0;j<ALU_CYCLES;j++)
	{
		result = a + b;
		errors+= bit_test(result,0xFFFFFFFF);
	}

    a = 0xAAAAAAAA, b = 0x55555555;
	for(j=0;j<ALU_CYCLES;j++)
	{
		result = a - b;
		errors+= bit_test(result,0x55555555);
	}

    a = 0xAAAAAAAA;
	for(j=0;j<ALU_CYCLES;j++)
	{
		result = ~a;
		errors+= bit_test(result,0x55555555);
	}

    b = 0x55555555;
	for(j=0;j<ALU_CYCLES;j++)
	{
		result = a<<1;
		errors+= bit_test(result,0xAAAAAAAA);
	}

	a = 0xAAAAAAAA, b = 0x55555555;
	for(j=0;j<ALU_CYCLES;j++)
	{
        result = a&b;
		errors+= bit_test(result,0);
	}

    a = 0xAAAAAAAA, b = 0x55555555;
	for(j=0;j<ALU_CYCLES;j++)
	{
        result = a|b;
		errors+= bit_test(result,0xFFFFFFFF);
	}

	return errors;
}


void sendchar(uint32_t ch)
{
	iowrite32(ch, GSPI_0_BASE + GSPI_SSPDR);    // Send data by SPI0
	return;
}

void spi_init(void)
{
    iowrite32((100<<8)|0x7,GSPI_0_BASE+GSPI_SSPCR0);    // CSR = 8, length = 8 bit
    iowrite32((1<<1),GSPI_0_BASE+GSPI_SSPCR1);          //  enable SPI
    iowrite32(7,GSPI_0_BASE+GSPI_SSPCPSR);              //  spi freq divider
    iowrite32(0,GSPI_0_BASE+GSPI_SSPIMSC);              //  mask spi interrupts
}

void uart_init_main(void)
{
    uart_set_baudrate(UART_UNDER_TEST,16000000,115200);
    uart_fifos_set_rx_fifo_level(UART_UNDER_TEST, UART_TX_FIFO_LEVEL_LT_7_8);
    uart_fifos_set_tx_fifo_level(UART_UNDER_TEST, UART_TX_FIFO_LEVEL_LT_7_8);
    uart_fifos_enable(UART_UNDER_TEST,true);
    uart_rts_cts_enable(UART_UNDER_TEST,true);
    uart_loopback_enable(UART_UNDER_TEST,true);
    //uart_set_word_length(UART_UNDER_TEST,8);
    uart_set_interrupt_mask(UART_UNDER_TEST,0);
    //uart_set_parity(UART_UNDER_TEST,UART_parity_no);
    uart_rx_enable(UART_UNDER_TEST,true);
    uart_tx_enable(UART_UNDER_TEST,true);
    uart_enable(UART_UNDER_TEST,true);
}

void Delay(uint32_t cycles)
{
	volatile uint32_t i;
	for(i=0;i<cycles;i++);
}

void gpio_change(uint32_t number, uint32_t val)
{
    uint32_t temp = (uint32_t)gpio_get_value(GPIO_0_BASE,5);
	temp ^= 1;
	gpio_set_value(GPIO_0_BASE,5,temp);
}

void gpio_init(void)
{
    gpio_set_direction(GPIO_0_BASE,0,gpio_pin_direction_in);    // isDose line
    gpio_set_direction(GPIO_0_BASE,1,gpio_pin_direction_in);    // isECC line
    gpio_set_direction(GPIO_0_BASE,2,gpio_pin_direction_out);   // SS line
    gpio_set_direction(GPIO_0_BASE,3,gpio_pin_direction_out);   // Low level line
    gpio_set_direction(GPIO_0_BASE,4,gpio_pin_direction_out);   // High level line
    gpio_set_direction(GPIO_0_BASE,5,gpio_pin_direction_out);   // Meander line

    gpio_set_ctrl_mode(GPIO_0_BASE,0,gpio_ctrl_mode_sw);
    gpio_set_ctrl_mode(GPIO_0_BASE,1,gpio_ctrl_mode_sw);
    gpio_set_ctrl_mode(GPIO_0_BASE,2,gpio_ctrl_mode_sw);
    gpio_set_ctrl_mode(GPIO_0_BASE,3,gpio_ctrl_mode_sw);
    gpio_set_ctrl_mode(GPIO_0_BASE,4,gpio_ctrl_mode_sw);
    gpio_set_ctrl_mode(GPIO_0_BASE,5,gpio_ctrl_mode_sw);

    gpio_set_value(GPIO_0_BASE,2,1);
    gpio_set_value(GPIO_0_BASE,3,0);
    gpio_set_value(GPIO_0_BASE,4,1);
    gpio_set_value(GPIO_0_BASE,5,0);
}


bool gpio_dose_check(void)
{
	return gpio_get_value(GPIO_0_BASE,0);
}

bool gpio_ecc_check(void)
{
	return gpio_get_value(GPIO_0_BASE,1);
}



void send_info(uint32_t source_word, uint32_t pakage_type, uint32_t unit_number)
{
	uint32_t j, i;
	uint8_t pakage[7];

	pakage[1] = pakage_type & 0xFF;
	pakage[2] = unit_number & 0xFF;
	pakage[3] = (source_word >> 24) & 0xFF;
	pakage[4] = (source_word >> 16) & 0xFF;
	pakage[5] = (source_word >> 8) & 0xFF;
	pakage[6] =(source_word >> 0) & 0xFF;

	for(i=0;i<3;i++)
	{
		pakage[0]=i;
		gpio_set_value(GPIO_0_BASE,2,0);       //change SS to low level

		for (j = 0; j < (sizeof(pakage) / sizeof(pakage[0])); j++)
        {
			sendchar(pakage[j]);
        }

        while((ioread32(GSPI_0_BASE + GSPI_SSPSR)&(1 << 0))!=0)
        {
            // While FIFO is not empty
        }

		gpio_set_value(GPIO_0_BASE,2,1);       //change SS to high level
		Delay(10);
		
	}
}


int main()
{
	uint32_t iteration = 0, errors = 0, len = 0, counter = 0, last_error = 0, cache_len = 256 * 1024;
    bool isECCon=false, isECCon_Last = true, isDose = false;

    dcr_write(0x8008003c,0x1ACCE551);
    dcr_write(0x80080130,0x7);
    dcr_write(0x80080060,0x1);

	#ifndef TEST_NO_CACHE
    static const tlb_entry sram0_tlb_entry[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b0,    0b0,    0b0,    0b0,    0b1,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    #else
    static const tlb_entry sram0_tlb_entry[] = {
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    #endif
	
	
    uart_init_main();
    spi_init();
    gpio_init();
	rumboot_platform_runtime_info->silent = 0;

	uintptr_t start = (uintptr_t) START;
	start = start + (4 - (start % 4));

	uintptr_t end = (uintptr_t) END;
	end = end - (end % 8);
    len = end - start;
#ifdef DEBUG_OUTPUT
	rumboot_printf("Test area: %x -- %x [%x bytes]\n", start, end, end - start);
#endif
    write_tlb_entries(sram0_tlb_entry, ARRAY_SIZE(sram0_tlb_entry));	

	gpr_fill_pattern(0xAAAAAAAA);
	memtest_fill_pattern(start, len, 0xAAAAAAAA);
	memtest_fill_pattern(SRAM0_BASE, cache_len, 0xAAAAAAAA);
	
	while(1)
	{
		isECCon = gpio_ecc_check();
		if(isECCon!= isECCon_Last)
		{
			if(isECCon)
			{
				// Enable ECC
			}
			else 
			{
				// Disable ECC
			}
		}
		
		isECCon_Last = isECCon;
		isDose = gpio_dose_check();
		
		errors = uart_test(0xAA,&counter,&last_error);
		send_info(errors,WITH_ACCUMULATION,UART_TEST_RESULT);
        send_info(errors,WITH_ACCUMULATION,UART_COUNTER_TEST_RESULT);
        send_info(errors,WITH_ACCUMULATION,UART_LAST_ERROR_TEST_RESULT);
		
		
		if (isDose)
		{

            errors = alu_test();
		    send_info(errors,WITH_ACCUMULATION,ALU_TEST_RESULT);

			// Check General Purpose Registers
			gpr_fill_pattern(0xAAAAAAAA);
			errors = gpr_check_pattern(0xAAAAAAAA,false);
			send_info(errors,WITH_ACCUMULATION,GPR_AA_TEST_RESULT);

			gpr_fill_pattern(0x55555555);
			errors = gpr_check_pattern(0x55555555,false);
			send_info(errors,WITH_ACCUMULATION,GPR_AA_TEST_RESULT);

			// Check RAM
			memtest_fill_pattern(start, len, 0xAAAAAAAA);
			errors = memtest_check_pattern(0xAAAAAAAA, false, start, len);
			send_info(errors,WITH_ACCUMULATION,RAM_AA_TEST_RESULT);
			#ifdef DEBUG_OUTPUT
				rumboot_printf("RAM Test Iteration: %d Errors: %u\n", iteration, errors);
			#endif

			memtest_fill_pattern(start, len, 0x55555555);
			errors += memtest_check_pattern(0x55555555, false, start, len);
			send_info(errors,WITH_ACCUMULATION,RAM_55_TEST_RESULT);
			#ifdef DEBUG_OUTPUT
				rumboot_printf("RAM Test  Iteration: %d Errors: %u\n", iteration, errors);
			#endif
			
			// Check Cache
			memtest_fill_pattern(SRAM0_BASE, cache_len, 0xAAAAAAAA);
			errors = memtest_check_pattern(0xAAAAAAAA, false, SRAM0_BASE, cache_len);
			send_info(errors,WITH_ACCUMULATION,CACHE_AA_TEST_RESULT);
			#ifdef DEBUG_OUTPUT
				rumboot_printf("Cache Test Iteration: %d Errors: %u\n", iteration, errors);
			#endif

			memtest_fill_pattern(SRAM0_BASE, cache_len, 0x55555555);
			errors += memtest_check_pattern(0x55555555, false, SRAM0_BASE, cache_len);
			send_info(errors,WITH_ACCUMULATION,CACHE_55_TEST_RESULT);
			#ifdef DEBUG_OUTPUT
				rumboot_printf("Cache Test Iteration: %d Errors: %u\n", iteration, errors);
			#endif
		}
		else
		{
			// Check General Purpose Registers
			errors = gpr_check_pattern(0xAAAAAAAA, true);
			send_info(errors,WITH_ACCUMULATION,GPR_AA_TEST_RESULT);
			
			// Check RAM
			errors = memtest_check_pattern(0xAAAAAAAA, true, start, len);
			send_info(errors,WITH_ACCUMULATION,RAM_AA_TEST_RESULT);
			#ifdef DEBUG_OUTPUT
				rumboot_printf(" RAM Test Iteration: %d Errors: %u\n", iteration, errors);
			#endif	
			
			//errors = ioread32(); // READ ECC counter
			send_info(errors,WITH_ACCUMULATION,RAM_AA_ECC_TEST_RESULT);
			
			// Check Cache
			errors = memtest_check_pattern(0xAAAAAAAA, false, SRAM0_BASE, cache_len);
			send_info(errors,WITH_ACCUMULATION,CACHE_AA_TEST_RESULT);
			#ifdef DEBUG_OUTPUT
				rumboot_printf("Cache Test Iteration: %d Errors: %u\n", iteration, errors);
			#endif
		}
		
		
		
		send_info(iteration, ITER_PAKAGE_TYPE, 0);
		iteration++;
	}
	return 0;
}