#ifndef INI_GEN_LOADER_H
#define INI_GEN_LOADER_H


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

typedef struct gen_parameters {
  char **type;
  char **value;
  char *name;
} gen_parameters;

// void init_gen_parameters(gen_parameters *str);
// void free_gen_parameters(gen_parameters *str);

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

int type_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);

int value_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);

int name_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
);
#endif // INI_GEN_LOADER_H