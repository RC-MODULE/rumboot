#ifndef CRC8_H
#define CRC8_H

#include <stdint.h>

/*
  Name  : CRC-8
  Poly  : 0x07    x^8 + x^2 + x^1 + x^0
  Init  : 0x00
  Revert: false
  XorOut: 0x00
  Check : 0xF7 ("123456789")
*/
uint8_t crc8(uint32_t start_addr, uint32_t len);

#endif
