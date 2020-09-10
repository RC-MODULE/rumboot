#ifndef INI_LOADER_H
#define INI_LOADER_H

#include <stdint.h> // uint32_t


#define MAX_SECTION_LEN 128
#define USERDATA_LEN 1024


typedef struct sectionfilter {
  const char *section;
  int (*handler)(
    void *user, 
    const char *section,
    const char *name, 
    const char *value,
    int lineno
  );
  void (*apply)(
    void *user
  );
} sectionfilter;


int theini_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);

int exit_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);

int hwc_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);

int rsc_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);

int parameters_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);

int user_data_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);

int rtl_data_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);
#endif // INI_LOADER_H
