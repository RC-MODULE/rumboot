#include <string.h>
#include <stdio.h>
#include <rumboot/xmodem.h>
#include <rumboot/platform.h>
#include <rumboot/timer.h>
#include <stdint.h>

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 1000000UL
#define MAXRETRANS 25


unsigned short crc16(const unsigned char *buf, int sz)
{
        unsigned short crc = 0;

        while (--sz >= 0) {
                int i;
                crc ^= (unsigned short)*buf++ << 8;
                for (i = 0; i < 8; i++)
                        if (crc & 0x8000) {
                                crc = crc << 1 ^ 0x1021;
                        } else {
                                crc <<= 1;
                        }
        }
        return crc;
}


static int check(int crc, const unsigned char *buf, int sz)
{
        if (crc) {
                unsigned short crc = crc16(buf, sz);
                unsigned short tcrc = (buf[sz] << 8) + buf[sz + 1];
                if (crc == tcrc) {
                        return 1;
                }
        } else {
                int i;
                unsigned char cks = 0;
                for (i = 0; i < sz; ++i)
                        cks += buf[i];
                if (cks == buf[sz]) {
                        return 1;
                }
        }

        return 0;
}

static void flushinput(void)
{
        while (rumboot_platform_getchar(0) >= 0);
}

int xmodem_get(unsigned char *dest, int destsz)
{
        unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
        unsigned char *p;
        int bufsz, crc = 0;
        unsigned char trychar = 'C';
        unsigned char packetno = 1;
        int i, c, len = 0;
        int retry, retrans = MAXRETRANS;

        for (;; ) {
                for (retry = 0; retry < 160; ++retry) {
                        if (trychar) {
                                rumboot_platform_putchar(trychar);
                        }
                        if ((c = rumboot_platform_getchar(DLY_1S) >= 0)) {
                                switch (c) {
                                case SOH:
                                        bufsz = 128;
                                        goto start_recv;
                                case STX:
                                        bufsz = 1024;
                                        goto start_recv;
                                case EOT:
                                        flushinput();
                                        rumboot_platform_putchar(ACK);
                                        return len; /* normal end */
                                case CAN:
                                        if ((c = rumboot_platform_getchar(DLY_1S)) == CAN) {
                                                flushinput();
                                                rumboot_platform_putchar(ACK);
                                                return -1; /* canceled by remote */
                                        }
                                        break;
                                default:
                                        break;
                                }
                        }
                }
                if (trychar == 'C') {
                        trychar = NAK;
                        continue;
                }
                flushinput();
                rumboot_platform_putchar(CAN);
                rumboot_platform_putchar(CAN);
                rumboot_platform_putchar(CAN);
                return -2; /* sync error */

start_recv:
                if (trychar == 'C') {
                        crc = 1;
                }
                trychar = 0;
                p = xbuff;
                *p++ = c;
                for (i = 0; i < (bufsz + (crc ? 1 : 0) + 3); ++i) {
                        if ((c = rumboot_platform_getchar(DLY_1S)) < 0) {
                                goto reject;
                        }
                        *p++ = c;
                }

                if (xbuff[1] == (unsigned char)(~xbuff[2]) &&
                    (xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno - 1) &&
                    check(crc, &xbuff[3], bufsz)) {
                        if (xbuff[1] == packetno) {
                                unsigned int count = destsz - len;
                                if (count > bufsz) {
                                        count = bufsz;
                                }
                                if (count > 0) {
                                        memcpy (&dest[len], &xbuff[3], count);
                                        len += count;
                                }
                                ++packetno;
                                retrans = MAXRETRANS + 1;
                        }
                        if (--retrans <= 0) {
                                flushinput();
                                rumboot_platform_putchar(CAN);
                                rumboot_platform_putchar(CAN);
                                rumboot_platform_putchar(CAN);
                                return -3; /* too many retry error */
                        }
                        rumboot_platform_putchar(ACK);
                        continue;
                }
reject:
                flushinput();
                rumboot_platform_putchar(NAK);
        }
}
