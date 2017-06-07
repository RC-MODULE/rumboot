#ifndef PRINTF_H
#define PRINTF_H
#include <stdint.h>

void rumboot_printf(const char *fmt, ...);
void rumboot_print_logo();





/*
size_t strlen(const char *str);

void rumboot_putnumber(uint32_t num);
void rumboot_init_io();
*/

/* Serial routines */
/*
void rumboot_init_io_serial();
void rumboot_putnumber_serial(uint32_t num);
void rumboot_putstring_real_serial(const char *str, int len);

void uint32_to_hex_string_uppercase(uint32_t hex, char* hex_string);
void uint32_to_hex_string_lowercase(uint32_t hex, char* hex_string);
*/

#endif
