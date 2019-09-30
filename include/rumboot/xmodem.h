/*
 * Adapted from avr-liberty project
 * (c) Pascal Stang - Copyright (C) 2006
 * This code is distributed under the GNU Public License
 * which can be found at http://www.gnu.org/licenses/gpl.txt
 */


#ifndef XMODEM_H
#define XMODEM_H

// xmodem control characters
#define SOH			0x01
#define STX			0x02
#define EOT			0x04
#define ACK			0x06
#define NAK			0x15
#define CAN			0x18
#define CTRLZ		0x1A

// xmodem timeout/retry parameters
#define XMODEM_TIMEOUT_DELAY	1000000
#define XMODEM_RETRY_LIMIT		128

// error return codes
#define XMODEM_ERROR_REMOTECANCEL	-1
#define XMODEM_ERROR_OUTOFSYNC		-2
#define XMODEM_ERROR_RETRYEXCEED	-3



size_t xmodem_get( char *to, size_t maxszs);
size_t xmodem_get_async(size_t maxszs, void (*recv_cb)(size_t curpos, void *ptr, size_t length, void *arg), void *arg);


#endif

//@}
