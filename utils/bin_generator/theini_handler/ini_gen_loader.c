#include "ini_gen_loader.h"

#include <stdio.h>
#include <string.h>

#define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) printf("ini_gen_loader: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif


void init_gen_parameters(gen_parameters *str) {
  dbg("init_gen_parameters started\n");
  int i;

  for(i = 0; i < 4; ++i) {
    str->type[i] = (char *) malloc(8 + 1);
    str->type[i] = "";
    str->value[i] = (char *) malloc(8 + 1);
    str->value[i] = "";
  }
  str->name = (char *) malloc(16);
  str->name = "";
}

void free_gen_parameters(gen_parameters *str) {
  dbg("free_gen_parameters started\n");
  int i;

  for(i = 0; i < 4; ++i) {
    free(str->type[i]);
    free(str->value[i]);
  }
  free(str->name);
}


static char prevsection[MAX_SECTION_LEN];
static sectionfilter *curfilter;

sectionfilter ini_filters[] = {
  {
    .section = "EXIT",
    .handler = exit_handler,
  },
  {
    .section = "TYPE",
    .handler = type_handler,
  },
  {
    .section = "VALUE",
    .handler = value_handler,
  },
  {
    .section = "NAME",
    .handler = name_handler,
  },
};


int theini_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
){
  dbg("**theini_handler started\n"); 

  if(!strlen(prevsection) || strcmp(prevsection, section) != 0) {
    if(strlen(prevsection)) {
      dbg("Finished parsing section: %s\n", prevsection);
    }
    
    if(curfilter && curfilter->apply) {
      curfilter->apply(user);
    }

    if(strlen(section) > MAX_SECTION_LEN) {
      dbg("Section name %d too long\n", section);
    }

    strcpy(prevsection, section);
    curfilter = ini_filters;
    do {
      if(strcmp(curfilter->section, section) == 0) {
        break;
      }
      curfilter++;
    } while(curfilter->section);

    if(!curfilter->section) {
      dbg("Ignoring unknown section: %s\n", section);
    } else {
      dbg("Current filter: %s\n", curfilter->section);
    }
  }

  if(curfilter && curfilter->handler) {
    curfilter->handler(user, section, name, value, lineno);
  }

  return 0; 
}


int exit_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {
  if(strcmp(name, "mode")) {
    return 0;
  }

  if(!strcmp(value, "loop")){
    dbg("Entering endless loop\n");
    /* Endless loop */
    while(1);;
  }

  if(!strcmp(value, "next")) {
    dbg("Returning to bootrom to boot from next device\n");
    exit(0);
  }

  if (!strcmp(value, "host")) {
    dbg("Will now go back to host mode\n");
    exit(-1);
  }

  if(!strcmp(value, "wfiloop")) {
    dbg("Entering WFI loop\n");
    while(1) {
      // !!!!!
      // asm("wfi"); // GCC Inline Assembly
    }
  }

  int jid;
  if(1 == sscanf(value, "jump:%d", &jid)) {
    dbg("Jumping to boot device %d\n", jid);
    exit(jid);
  }
}


#define fill_int_box(nm)                     \
  if(strcmp(name, #nm) == 0) {           \
    data->nm = atoi(value);              \
  } // !!!! strtoul

#define fill_type(i, nm) \
  dbg("3. value=%s, name=%s, nm=%s, temp=%s\n", value, name, #nm, temp); \
  if(strcmp(temp, #nm) == 0) { \
    dbg("4. value=%s\n", value); \
    strcpy(data->nm[i], value); \
  } \
  dbg("5. data->nm[%d]=%s\n", i, data->nm[i]);


int type_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {
  dbg("0. **type_handler started, name=%s, valur=%s\n", name, value);

  gen_parameters *data = user;

  int i;
  char tmp[16];
  if(!sscanf(name, "type[%d]", &i)) {
    return 0;
  }
  dbg("1. **name=%s, value=%s, i=%d\n", name, value, i);
  char *temp = (char *) malloc(5);
  strncpy(temp, name, 4 * sizeof(char));
  // int pos = sprintf(NULL, "type[%d]", i);
  // char *barparam = &name[pos];
  dbg("2. i'm here\n");
  fill_type(i, type);
  // fill_type(i, type);
  // fill_type(i, type);
  // fill_type(i, type);

  dbg("6. type_handler finished\n");
 

  return 0;
}



int value_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {}

int name_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {}