#include "ini_gen_loader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) printf("ini_gen_loader: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif


// void init_gen_parameters(gen_parameters *config) {
//   dbg("init_gen_parameters started\n");
//   int i;

//   // for(i = 0; i < 4; ++i) {
//     // str->type[i] = (char *) malloc(8 + 1);
//     // str->type[i] = "";
//     // str->value[i] = (char *) malloc(8 + 1);
//     // str->value[i] = "";
//   // }
//   // str->name = (char *) malloc(16*sizeof(char));
//   // str->name = "";

//   // config = malloc(sizeof(gen_parameters));
//   config->type = (char **) malloc(4*sizeof(char *));
//   for(i = 0; i < 4; ++i) {
//     config->type[i] = (char *) malloc(14*sizeof(char));
//     config->type[i] = "";
//     // strcpy(config.type[i], "");
//     // config->value[i] = (char *) malloc(14*sizeof(char));
//     // strcpy(config->value[i], "");
//   }
// }

// void free_gen_parameters(gen_parameters *str) {
//   dbg("free_gen_parameters started\n");
//   int i;

//   for(i = 0; i < 4; ++i) {
//     free(str->type[i]);
//     free(str->value[i]);
//   }
//   free(str->name);
// }


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


int type_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {
  gen_parameters *data = user;

  int i;
  if(!sscanf(name, "type[%d]", &i)) {
    return 0;
  }

  if(!strncmp(name, "type", strlen("type"))) {
    data->type[i] = (char *) malloc(strlen(value)*sizeof(char)); 
    strcpy(data->type[i], value); 
  } 

  return 0;
}


int value_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *x,
  int lineno
) {
  gen_parameters *data = user;

  int i;
  if(!sscanf(name, "value[%d]", &i)) {
    return 0;
  }

  if(!strncmp(name, "value", strlen("value"))) {
    data->value[i] = (char *) malloc(strlen(x)*sizeof(char)); 
    strcpy(data->value[i], x); 
  } 

  return 0;
}


int name_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {
  gen_parameters *data = user;

  if(!strcmp(name, "name")) {
    data->name = (char *) malloc(strlen(value) + 1);
    strcpy(data->name, value);
  }

  return 0;
}