#include <rumboot/bitswapper.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/io.h>
#include <rumboot/hexdump.h>
#include <rumboot/xmodem.h>

#define FLASH_CMD_READ_ID		    0x90
#define FLASH_CMD_READ_STATUS		0x70
#define FLASH_CMD_RESET			    0xff
#define FLASH_CMD_BLOCK_ERASE		0x20
#define FLASH_CMD_ERASE_CONFIRM		0xd0
#define FLASH_CMD_CLEAR_STATUS		0x50
#define FLASH_CMD_SUSPEND_ERASE		0xb0
#define FLASH_CMD_WRITE			    0x40
#define FLASH_CMD_WRITE_BUFF		0xe8
#define FLASH_CMD_PROG_RESUME		0xd0
#define FLASH_CMD_PROTECT		    0x60
#define FLASH_CMD_PROTECT_SET		0x01
#define FLASH_CMD_PROTECT_CLEAR		0xd0
#define FLASH_STATUS_DONE	        0x80

struct bitswapper addr_swap = {
    .num_bits = 32,  
    .endian_swap = 0,
    .dst = {
        23,
        16,
        17,
        18,
        19,
        20,
        21,
        15,
        9,
        8,
        7,
        6,
        5,
        4,
        3,
        2,
        22,
        14,
        13,
        10,
        11,
        12,
        1,
        0,
        24,
        25,
        26,
        27,
        28,
        29,
        30,
        31
    }
};

#if 1
struct bitswapper data_swap = {
    .num_bits = 32,  
    .endian_swap = 1,
    .dst = {
        7,
        5,
        3,
        1,
        8,
        10,
        12,
        14,
        6,
        4,
        2,
        0,
        9,
        11,
        13,
        15,
        16,
        18,
        20,
        22,
        24,
        26,
        28,
        30,
        17,
        19,
        21,
        23,
        25,
        27,
        29,
        31,
    }
};
#endif


#define conv_addr(A) (2*A+!(A&0x1))  /* Convert a word mode command to byte mode command :
                                           Word Mode Command    Byte Mode Command
                                                0x555      ->     0xAAA
                                                0x2AA      ->     0x555
                                                0x55       ->     0xAA            */

#define CMD(A) (A+(A<<16))
#define sh_addr(A) (A<<2)

struct nor_flash {
    uintptr_t base; 
    int erase_size;
    struct bitswapper addr_swap;
    struct bitswapper data_swap;
};

#define USE_BSW

void flash_write(struct nor_flash * fl, uint32_t addr, uint32_t value)
{
    #ifdef USE_BSW
    uint32_t taddr = fl->base + (bitswapper_transmute(&fl->addr_swap, addr) << 2);
    uint32_t tdata = bitswapper_transmute(&fl->data_swap, value);
    #else
    uint32_t taddr = int_turned_addr(addr);
    uint32_t tdata = int_turned_data(value); 
    #endif

    //rumboot_printf("flash write %x (swapped %x): data %x (swapped %x)\n", addr, taddr, value, tdata);
    iowrite32(tdata, taddr);
}

uint32_t flash_read(struct nor_flash * fl, uint32_t addr)
{
    #ifdef USE_BSW    
    uint32_t taddr = bitswapper_transmute(&fl->addr_swap, addr) << 2;
    #else
    uint32_t taddr = int_turned_addr(addr);
    #endif

    uint32_t data = ioread32(fl->base + taddr);

    #ifdef USE_BSW    
    uint32_t tdata = bitswapper_transmute_reverse(&fl->data_swap, data);
    #else
    uint32_t tdata = int_turned_data(data);    
    #endif
    //rumboot_printf("read %x\n", tdata);
    return tdata;
}



void flash_reset(struct nor_flash * fl)
{
	flash_write(fl, 0x555,      0xF0AAF0AA ); /* 1st Cycle */
	flash_write(fl, 0x2AA,      0x00550055 ); /* 2nd Cycle */
	flash_write(fl, 0x123,      0xF0F0F0F0 ); /* 3rd Cycle: write 0x00F0 to ANY address */
} 


static  void flash_program_word(struct nor_flash *fl, uint32_t addr, uint32_t data)
{
                flash_write(fl, (0x555), 0xaaaaaaaa);
                flash_write(fl, (0x2AA), 0x55555555);
                flash_write(fl, (0x555), 0xa0a0a0a0);
                iowrite32(data, fl->base + addr);
                mdelay(1); //workaround
                if (ioread32(fl->base + addr) != data) {
                    //rumboot_printf("expected: %x got %x\n", data, ioread32(fl->base + addr));
                }
}

static int flash_wait_operation_completion (struct nor_flash *fl, uint32_t timeout_ms)
{
	uint32_t r1, r2;
	/* watch the toggle bits in the status word for success. */
    int ret = 0;
	/* Step 1: Read DQ6 (into  a word)*/
	r1 = flash_read(fl, 0x123);
	while(1)
	{
		/*
		*TimeOut: If, for some reason, the hardware fails then this
		*loop exit and the function return flash_OperationTimeOut.
		*/

		/*Step 2: Read DA1,DQ5 and DQ6 (into another word)*/
		r2 = flash_read(fl, 0x123);

		/* Step 3: If DQ6 did not toggle between the two reads then return Flash_Success*/
		if ((r1 & 1<<6) != (r2 & 1<<6))   /*is DQ6 toggling?*/
		{
			/*DQ6 is toggling.*/
			if ( !(r2 & (1<<5)) )
				if ( !(r2 & 1<<1) )
				{
					/* DQ1 and DQ5 are zero.*/
					r1 = r2;

					/*Step 4: Else if DQ5 is zero  and DQ!=1  then operation is not yet complete, goto 1*/
					continue;
				}
			/* DQ1 or DQ5 is 1.*/

			/*Step 5: Else (DQ5 != 0) and DQ1==0, read DQ6 again*/
			r1 = flash_read(fl, 0x123);
			r2 = flash_read(fl, 0x123);

			/* Step 6: If DQ6 did not toggle between the last two reads then return
			           Flash_Success*/
			if ((r1 & (1<<6)) != (r2 & 1<<6))   /* is DQ6 toggling? */
			{
				/*
				 *DQ6 is toggling, an error has occurred.
				 *We don't know whether a write or an erase
				 *operation is/was running, so we can't
				 *return anything useful as an error code.
				 */

				/* Step 7: Else return error */
				if (r2 & 1<<6)
					ret = -1;
				else
					ret = -2;
			}
			return ret;
		}
		else
			/*DQ6 is not toggling, operation is complete.*/
			return 0;
	}
	return -3;/*if exit from while loop then time out exceeded */
}

static int flash_erase_sector(struct nor_flash *fl, int s)
{
	        flash_write(fl, 0x00555, CMD(0x00AA) );
	        flash_write(fl, 0x002AA, CMD(0x0055) );
	        flash_write(fl, 0x00555, CMD(0x0080) );
	        flash_write(fl, 0x00555, CMD(0x00AA) );
	        flash_write(fl, 0x002AA, CMD(0x0055) );
	        flash_write(fl, (s << 0x10),    CMD(0x0030) );
            
            while( ! ((flash_read(fl, (s << 0x10))) & CMD(0x0008)));;
            int ret = flash_wait_operation_completion(fl, 2000);
            //rumboot_printf("erase %d\n", ret);
            flash_reset(fl);
            return ret;
}

uint32_t flash_read_id(struct nor_flash * fl, int offset)
{

   // uint32_t id;
	/* Step 1: Send the AutoSelect command */
	flash_write(fl, 0x00555, CMD(0x00AA) ); /* 1st Cycle */
	flash_write(fl, 0x002AA, CMD(0x0055) ); /* 2nd Cycle */
	flash_write(fl, 0x00555, CMD(FLASH_CMD_READ_ID) ); /* 3rd Cycle */

	/* Step 2: Read the DeviceId */
    uint32_t ret = flash_read(fl, offset);
	/* Step 3: Return to Read Array Mode */
    flash_reset(fl);

	/* Step 4: Check flash response (more flashes could give different results) */
	return ret;

}

uint32_t flash_get_erase_size(struct nor_flash *fl)
{
    	flash_reset(fl);
        flash_write(fl, 0x00555, CMD(0x0098) ); /* 1st Cycle */

    	uint32_t esize = ( ( ( flash_read(fl, sh_addr(0x002F)) & CMD(0x00FF) ) +
	                  ( ( flash_read(fl, sh_addr(0x0030)) & CMD(0x00FF) ) << 8 ) ) << 8 );
        esize = esize * 2 / 4 / 0xa; // (0xa ? Why?)
        flash_reset(fl);
        return esize;
        

}


#include <devices/ugly/emi.h>

#ifndef NOR_OFFSET
    #define NOR_OFFSET 0
#endif

void packet_rx_cb(size_t curpos, void *ptr, size_t length, void *arg)
{
    struct nor_flash *fl = arg;
    uint32_t *uiptr = ptr;
//    if (curpos % fl->erase_size == 0) {
//        flash_erase_sector(fl, (curpos / fl->erase_size));
//    }
    int i; 
    for (i=0; i<length / 4; i++) {
        flash_program_word(fl, NOR_OFFSET + curpos + i*4, uiptr[i]);
    }
}

int main() {
	rumboot_platform_runtime_info->silent = false;
    struct nor_flash fl;
    fl.base = NOR_BASE;
    fl.addr_swap = addr_swap;
    fl.data_swap = data_swap;

#if 1
    rumboot_printf( "Init ext_clk \n");  
    uint32_t read_data;   
    dcr_write(0x8008003c,0x1ACCE551);
    read_data = dcr_read(0x8008003c);
    rumboot_printf( "read_data 0x80080130 = %x\n", read_data);
    
    dcr_write(0x80080130,0x7);
    read_data = dcr_read(0x80080130);
    rumboot_printf( "read_data 0x80080130 = %x\n", read_data);
    
    dcr_write(0x80080060,0x1);
    read_data = dcr_read(0x80080060);
    rumboot_printf( "read_data 0x80080060 = %x\n", read_data);
    read_data = dcr_read(0x80080130);
    rumboot_printf( "read_data 0x80080130 = %x\n", read_data);

    rumboot_printf("start!\n");

    emi_init(DCR_EM2_EMI_BASE);
#endif

#if 0
    uint32_t test = 0xdeadc0de;
    rumboot_printf("test %x %x %x\n", 
        test, 
        (uint32_t) (bitswapper_transmute(&fl.addr_swap, test)),
        int_turned_addr(test));

    rumboot_printf("test %x %x %x\n", 
        1, 
        (uint32_t) bitswapper_transmute(&fl.addr_swap, 1),
        int_turned_addr(1));
#endif 

    flash_reset(&fl);
    uint32_t id = flash_read_id(&fl, 1);
    rumboot_printf("Flash manufacturer id 0x%x\n", id & 0xffff);
    uint32_t esz = flash_get_erase_size(&fl);
    fl.erase_size = esz;
    rumboot_printf("Flash erase size 0x%x / %d bytes\n", esz, esz);

	rumboot_hexdump((void *)NOR_BASE + NOR_OFFSET, 1024);
    int i; 
    for (i=0; i<512; i++) {
        int r = flash_erase_sector(&fl, i);
        rumboot_printf("Erasing sector %d (%d)\n", i, r);
    }
	rumboot_hexdump((void *)NOR_BASE + NOR_OFFSET, 1024);


    rumboot_printf("boot: Press 'X' and send me the image\n");
    while ('X' != rumboot_getchar(1000));;

	size_t bytes = xmodem_get_async(512 * 1024 * 1024, packet_rx_cb, (void *) &fl);
	rumboot_printf("Operation completed, %d bytes written\n", bytes);

    return 0;
}