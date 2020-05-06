#include <rumboot/timer.h>
#include <string.h>
#include <stdlib.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <devices/pl022.h>

static const struct rumboot_bootsource *src;
static char pdata[64];

void serprog_init()
{
	rumboot_platform_runtime_info->silent = false;
    src= &rumboot_platform_get_bootsources()[BOOT_ID];
	bootsource_init(src, (void *) pdata);
	rumboot_printf("SERPROG READY!\n");
}

static void spi_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
	if (src->chipselect) {
		pl022_internal_cs(src->base, 1); /* Make sure internal CS is '1' */
		src->chipselect(src, pdata, select);
	} else {
		pl022_internal_cs(src->base, select);
	}
	mdelay(1);
}

uint8_t ispTransmit_hw(uint8_t send_byte) {
  pl022_xfer(src->base, (uint8_t *) &send_byte, (uint8_t *) &send_byte, 1);
  return send_byte;
}

static inline void select_chip(void)
{
    spi_cs(src, pdata, 0);
}

static inline void unselect_chip(void)
{
    spi_cs(src, pdata, 1);
}


#define S_ACK 0x06
#define S_NAK 0x15

#define S_CMD_NOP          0x00UL /* No operation                                 */
#define S_CMD_Q_IFACE      0x01UL /* Query interface version                      */
#define S_CMD_Q_CMDMAP     0x02UL /* Query supported commands bitmap              */
#define S_CMD_Q_PGMNAME    0x03UL /* Query programmer name                        */
#define S_CMD_Q_SERBUF     0x04UL /* Query Serial Buffer Size                     */
#define S_CMD_Q_BUSTYPE    0x05UL /* Query supported bustypes                     */
#define S_CMD_Q_CHIPSIZE   0x06UL /* Query supported chipsize (2^n format)        */
#define S_CMD_Q_OPBUF      0x07UL /* Query operation buffer size                  */

#define S_CMD_Q_WRNMAXLEN  0x08UL /* Query Write to opbuf: Write-N maximum length */
#define S_CMD_R_BYTE       0x09UL /* Read a single byte                           */
#define S_CMD_R_NBYTES     0x0AUL /* Read n bytes                                 */
#define S_CMD_O_INIT       0x0BUL /* Initialize operation buffer                  */
#define S_CMD_O_WRITEB     0x0CUL /* Write opbuf: Write byte with address         */
#define S_CMD_O_WRITEN     0x0DUL /* Write to opbuf: Write-N                      */
#define S_CMD_O_DELAY      0x0EUL /* Write opbuf: udelay                          */
#define S_CMD_O_EXEC       0x0FUL /* Execute operation buffer                     */

#define S_CMD_SYNCNOP      0x10UL /* Special no-operation that returns NAK+ACK    */
#define S_CMD_Q_RDNMAXLEN  0x11UL /* Query read-n maximum length                  */
#define S_CMD_S_BUSTYPE    0x12UL /* Set used bustype(s).                         */
#define S_CMD_O_SPIOP      0x13UL /* Perform SPI operation.                       */
#define S_CMD_O_XXX        'X' /* Test IO                       */


#define S_IFACE_VERSION		0x01		/* Version of the protocol */
#define S_PGM_NAME		"rumboot:" RUMBOOT_PLATFORM /* The program's name */

/*
 * we must split in 3 parts because else avr-gcc doesn't seem to
 *  be able to compute stuff like 1<<S_CMD_SYNCNOP (it returns 0)
 */
#define SUPPORTED_COMMANDS_LOW ( ( \
	(1<<S_CMD_NOP) | (1<<S_CMD_Q_IFACE) | (1<<S_CMD_Q_CMDMAP) \
	| (1<<S_CMD_Q_PGMNAME) | (1<<S_CMD_Q_SERBUF) | (1<<S_CMD_Q_BUSTYPE) \
        ) & 0xff)
#define SUPPORTED_COMMANDS_HIGH ( ( ( \
	(1<<(S_CMD_SYNCNOP - 16)) | (1<<(S_CMD_O_SPIOP - 16)) | (1<<(S_CMD_S_BUSTYPE - 16)) \
	) & 0xff ) )

#define SUPPORTED_BUS 0x08


/* get 24bit values in little endian */
uint32_t get24_le()
{
	uint32_t val = 0;

	val = (uint32_t)rumboot_platform_getchar(-1) << 0;
	val |= (uint32_t)rumboot_platform_getchar(-1) << 8;
	val |= (uint32_t)rumboot_platform_getchar(-1) << 16;

	return val;
}


void serprog_handle_command(unsigned char command)
{
	int i;
	uint32_t slen = 0; /* write len */
	uint32_t rlen = 0; /* read len */
	switch (command){
		case S_CMD_NOP:
			rumboot_platform_putchar(S_ACK);
			break;
		case S_CMD_Q_IFACE:
			rumboot_platform_putchar(S_ACK);
			rumboot_platform_putchar(S_IFACE_VERSION);
			/* little endian multibyte value to complete to 16bit */
			rumboot_platform_putchar(0);
			break;
		case S_CMD_Q_CMDMAP:
			rumboot_platform_putchar(S_ACK);
			/* little endian */
			rumboot_platform_putchar(SUPPORTED_COMMANDS_LOW);
			rumboot_platform_putchar(0x00);
			rumboot_platform_putchar(SUPPORTED_COMMANDS_HIGH);
			for (i=0;i<29;i++){
				rumboot_platform_putchar(0x0);
			}
			break;
		case S_CMD_Q_PGMNAME:
			rumboot_platform_putchar(S_ACK);
			rumboot_printf(S_PGM_NAME);
			for (i=strlen(S_PGM_NAME);i<16;i++){
				rumboot_platform_putchar(0);
			}
			break;
		case S_CMD_Q_SERBUF:
			rumboot_platform_putchar(S_ACK);
			rumboot_platform_putchar(0xFF);
			rumboot_platform_putchar(0xFF);
			break;
		case S_CMD_Q_BUSTYPE:
			rumboot_platform_putchar(S_ACK);
			rumboot_platform_putchar(0b00001000);
			break;
		case S_CMD_Q_CHIPSIZE:
			break;
		case S_CMD_Q_OPBUF:
			break;
		case S_CMD_Q_WRNMAXLEN:
			break;
		case S_CMD_R_BYTE:
			break;
		case S_CMD_R_NBYTES:
			break;
		case S_CMD_O_INIT:
			break;
		case S_CMD_O_WRITEB:
			break;
		case S_CMD_O_WRITEN:
			break;
		case S_CMD_O_DELAY:
			break;
		case S_CMD_O_EXEC:
			break;
		case S_CMD_SYNCNOP:
			/* Make sure SPI is initialized */
			rumboot_platform_putchar(S_NAK);
			rumboot_platform_putchar(S_ACK);
			break;
		case S_CMD_Q_RDNMAXLEN:
			break;
		case S_CMD_S_BUSTYPE:
			switch (rumboot_platform_getchar(-1)) {
				case SUPPORTED_BUS:
					rumboot_platform_putchar(S_ACK);
					break;
				default:
					rumboot_platform_putchar(S_NAK);
					break;
			}
			break;
		case S_CMD_O_SPIOP:
			/* get slen */
			slen = get24_le();
			/* get rlen */
			rlen = get24_le();

			select_chip();
			while (slen--)
					ispTransmit_hw(rumboot_platform_getchar(-1));

			rumboot_platform_putchar(S_ACK);

			while (rlen--)
				rumboot_platform_putchar(ispTransmit_hw(0x0));

			unselect_chip();
			break;
		case S_CMD_O_XXX:
			rumboot_printf("test\n");
			rumboot_platform_putchar(S_ACK);
		default:
			break;
	}
}


int main()
{
    serprog_init();

    while(1) {
        int c = rumboot_platform_getchar(-1);

    	if (c != -1) {
    		serprog_handle_command((uint8_t) c);
    	}
    }
}
