#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/platform.h>
#include <rumboot/hexdump.h>
#include <ctype.h>


#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 16
#endif
 
void rumboot_hexdump(void *mem, unsigned int len)
{
        unsigned int i, j;
        
        for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
        {
                /* print offset */
                if(i % HEXDUMP_COLS == 0)
                {
                        rumboot_printf("0x%06x: ", i);
                }
 
                /* print hex data */
                if(i < len)
                {
                        rumboot_printf("%02x ", 0xFF & ((char*)mem)[i]);
                }
                else /* end of block, just aligning for ASCII dump */
                {
                        rumboot_printf("   ");
                }
                
                /* print ASCII dump */
                if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
                {
                        for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
                        {
                                if(j >= len) /* end of block, not really printing */
                                {
                                        rumboot_putchar(' ');
                                }
                                else if(isprint(((char*)mem)[j])) /* printable char */
                                {
                                        rumboot_putchar(0xFF & ((char*)mem)[j]);        
                                }
                                else /* other char */
                                {
                                        rumboot_putchar('.');
                                }
                        }
                        rumboot_putchar('\n');
                }
        }
}
