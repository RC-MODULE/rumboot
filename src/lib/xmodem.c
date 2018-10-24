/*
 * Adapted from avr-liberty project
 * (c) Pascal Stang - Copyright (C) 2006
 * This code is distributed under the GNU Public License
 * which can be found at http://www.gnu.org/licenses/gpl.txt
 */

#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/xmodem.h>
#include <rumboot/platform.h>


#define XMODEM_BUFFER_SIZE		1024

uint16_t crc_xmodem_update(uint16_t crc, uint8_t data)
{
	int i;

	crc = crc ^ ((uint16_t)data << 8);
	for (i=0; i<8; i++)
	{
		if(crc & 0x8000)
			crc = (crc << 1) ^ 0x1021;
		else
			crc <<= 1;
	}

	return crc;
}

int xmodem_chksum(int crcflag, const unsigned char *buffer, int size)
{
	if(crcflag)
	{
		unsigned short crc=0;
		unsigned short pktcrc = (buffer[size]<<8)+buffer[size+1];
		while(size--)
			crc = crc_xmodem_update(crc, *buffer++);
		if(crc == pktcrc)
			return 1;
	}
	else
	{
		int i;
		unsigned char cksum = 0;
		for(i=0; i<size; ++i)
		{
			cksum += buffer[i];
		}
		if(cksum == buffer[size])
			return 1;
	}

	return 0;
}

void xmodem_flush_input(void)
{
	while(rumboot_platform_getchar(XMODEM_TIMEOUT_DELAY) >= 0);
}

size_t xmodem_get( char *to, size_t maxszs)
{
	unsigned char xmbuf[XMODEM_BUFFER_SIZE+6];
	unsigned char seqnum=1;
	unsigned short pktsize=128;
	unsigned char response='C';
	char retry=XMODEM_RETRY_LIMIT;
	unsigned char crcflag=0;
	unsigned long totalbytes=0;
	int i,c;
	size_t pos = 0;

	while(retry > 0)
	{
		rumboot_platform_putchar(response);
		if( (c = rumboot_platform_getchar(XMODEM_TIMEOUT_DELAY)) >= 0)
		{
			switch(c)
			{
			case SOH:
				pktsize = 128;
				break;
			#if(XMODEM_BUFFER_SIZE>=1024)
			case STX:
				pktsize = 1024;
				break;
			#endif
			case EOT:
				xmodem_flush_input();
				rumboot_platform_putchar(ACK);
				return totalbytes;
			case CAN:
				if((c = rumboot_platform_getchar(XMODEM_TIMEOUT_DELAY)) == CAN)
				{
					xmodem_flush_input();
					rumboot_platform_putchar(ACK);
					return XMODEM_ERROR_REMOTECANCEL;
				}
			default:
				break;
			}
		}
		else
		{
			retry--;
			continue;
		}

		if(response == 'C') crcflag = 1;
		xmbuf[0] = c;
		for(i=0; i<(pktsize+crcflag+4-1); i++)
		{
			if((c = rumboot_platform_getchar(XMODEM_TIMEOUT_DELAY)) >= 0)
			{
				xmbuf[1+i] = c;
			}
			else
			{
				retry--;
				xmodem_flush_input();
				response = NAK;
				break;
			}
		}
		if(i<(pktsize+crcflag+4-1))
			continue;

		if(	(xmbuf[1] == (unsigned char)(~xmbuf[2])) &&		// sequence number was transmitted w/o error
			xmodem_chksum(crcflag, &xmbuf[3], pktsize) )	// packet is not corrupt
		{
			if(xmbuf[1] == seqnum)
			{
				// write/deliver data
				memcpy(&to[pos], &xmbuf[3], pktsize);
				pos += pktsize;
				totalbytes += pktsize;
				seqnum++;
				retry = XMODEM_RETRY_LIMIT;
				response = ACK;
				continue;
			}
			else if(xmbuf[1] == (unsigned char)(seqnum-1))
			{
				response = ACK;
				continue;
			}
			else
			{
				xmodem_flush_input();
				rumboot_platform_putchar(CAN);
				rumboot_platform_putchar(CAN);
				rumboot_platform_putchar(CAN);
				return XMODEM_ERROR_OUTOFSYNC;
			}
		}
		else
		{
			retry--;
			xmodem_flush_input();
			response = NAK;
			continue;
		}
	}

	xmodem_flush_input();
	rumboot_platform_putchar(CAN);
	rumboot_platform_putchar(CAN);
	rumboot_platform_putchar(CAN);
	return XMODEM_ERROR_RETRYEXCEED;
}
