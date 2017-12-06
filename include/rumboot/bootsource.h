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

struct bdata {
  uint32_t base;
  uint32_t freq_in_mhz;
  uint32_t* to;
  uint32_t* from;
  uint32_t out_param[1];
};

struct rumboot_bootsource {
  const char *name;
  int namelen;
  int bdatalen;
  bool (*init) (struct bdata* );
  void (*deinit) (struct bdata* );
  bool (*load_img) (struct bdata* );
  bool (*init_gpio_mux) ();
  void (*deinit_gpio_mux) ();
  bool (*should_i_try_again) ();

};

bool bootsource_try_single(const struct rumboot_bootsource *src);
bool bootsource_try_chain(const struct rumboot_bootsource *src);

#endif
