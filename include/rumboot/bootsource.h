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

#if 0
struct config {
  uint32_t base;
  uint32_t freq_khz;
  uint32_t offset;
  char privdata[];
};
#endif

struct pdata {
  /*Dummy*/
};

struct rumboot_bootsource {
  const char *name;
  uint32_t base;
  uint32_t offset;
  uint32_t freq_khz;
  int namelen;
  int privdatalen;
  bool (*init) (const struct rumboot_bootsource* src, struct pdata* pdata);
  void (*deinit) (struct pdata* pdata);
  bool (*load_img) (struct pdata* pdata);
  bool (*init_gpio_mux) ();
  void (*deinit_gpio_mux) ();
  bool (*should_i_try_again) ();
};

bool bootsource_try_single(const struct rumboot_bootsource *src);
bool bootsource_try_chain(const struct rumboot_bootsource *src, const struct pdata* pdata);

#endif
