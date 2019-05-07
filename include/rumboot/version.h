#ifndef RUMBOOT_VERSION_H
#define RUMBOOT_VERSION_H

#include <stdint.h>

struct rumboot_version_info {
  const char* platform;
  const char* gitversion;
  uint8_t chip_id;
  uint8_t chip_rev;
};

/**
 * Returns rumboot version information structure 
 * 
 * @return
 */
const struct rumboot_version_info *rumboot_version_info();


#endif