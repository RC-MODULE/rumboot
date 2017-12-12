#ifndef BOOTSOURCE_H
#define BOOTSOURCE_H

#include <stdbool.h>
#include <stdlib.h>

#include <rumboot/bootheader.h>

#define DBG_ASSERT(statement, message) \
  { \
    if(statement) {\
      rumboot_printf("%s\n", message); \
      while(1); \
    } \
  }

struct rumboot_bootsource {
  const char *name;
  uint32_t base;
  uint32_t offset;
  uint32_t freq_khz;
  int privdatalen;
  bool (*init) (const struct rumboot_bootsource* src, void* pdata);
  void (*deinit) (void* pdata);
  bool (*load_img) (void* pdata);
  bool (*init_gpio_mux) (void* pdata);
  void (*deinit_gpio_mux) (void* pdata);
  bool (*are_load_attempts) (void* pdata);
};

bool bootsource_try_single(const struct rumboot_bootsource *src);
bool bootsource_try_chain(const struct rumboot_bootsource *src);

#endif
