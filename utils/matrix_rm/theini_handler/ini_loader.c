#include "ini_loader.h"
#include "../reference_model/matrix_rm.h"

#include <stdio.h> // printf
#include <string.h> // memset, string
// #include <tcl.h> // panic
#include <stdlib.h> // exit, strtoul


// #define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) printf("qspi: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif


static char prevsection[MAX_SECTION_LEN];
static sectionfilter *curfilter;

sectionfilter ini_filters[] = {
  {
    .section = "EXIT",
    .handler = exit_handler,
  },
  {
    .section = "HWC",
    .handler = hwc_handler,
  },
  {
    .section = "RSC",
    .handler = rsc_handler,
  },
  {
    .section = "PARAMETERS",
    .handler = parameters_handler,
  },
  {
    .section = "USER_DATA",
    .handler = user_data_handler,
  },
  {
    .section = "RTL_DATA",
    .handler = rtl_data_handler,
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
    // bzero(user, USERDATA_LEN); // dated
    // memset(user, 0, USERDATA_LEN); // first USERDATA_LEN bits in user := 0
    
    if(strlen(prevsection)) {
      dbg("Finished parsing section: %s\n", prevsection);
    }
    
    if(curfilter && curfilter->apply) {
      curfilter->apply(user);
    }
    // memset(user, 0, USERDATA_LEN); // first USERDATA_LEN bits in user := 0

    if(strlen(section) > MAX_SECTION_LEN) {
      dbg("Section name %d too long\n", section);
      // rumboot_platform_panic("Section name %d too long\n", section);
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

int hwc_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {
  dbg("**hwc_handler started\n");

  matrix_config_t *data = user;

  fill_int_box(H);
  fill_int_box(W);
  fill_int_box(C);
  fill_int_box(Tp);
  fill_int_box(Bp);
  fill_int_box(Lp);
  fill_int_box(Rp);

  return 0;
}


int rsc_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {
  dbg("**rsc_handler started\n");

  matrix_config_t *data = user;

  fill_int_box(R);
  fill_int_box(S);

  fill_int_box(C);
  // TODO: compare [HWC].C && [RSC].C
  // if(!strcmp(name, "C")) {
  //   if(data->C != atoi(value)) {
  //     printf("ERROR: in [HWC] and [RSC] parameter C has different value\n");
  //     return -1;
  //   }
  // }

  fill_int_box(Dx);
  fill_int_box(Dy);
  fill_int_box(Sx);
  fill_int_box(Sy);
  fill_int_box(K);

  return 0;
}


int parameters_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {
  dbg("**parameters_handler started\n");

  matrix_config_t *data = user;

  fill_int_box(sn);
  fill_int_box(LINES);
  fill_int_box(COLUMNS);
  fill_int_box(BUF_NMB);

  return 0;
}


#define fill_str_box(nm)                                \
  if(strcmp(name, #nm) == 0) {                          \
    data->nm = (char *) malloc(strlen(value) + 1);      \
    strcpy(data->nm, value);                            \
  }

int user_data_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {
  dbg("**user_data_handler started\n");

  matrix_config_t *data = user;
  
  fill_str_box(user_X_data_file_name);
  fill_str_box(user_W_data_base_file_name);
  fill_str_box(user_Y_data_file_name);

  return 0;
}


int rtl_data_handler(
  void *user, 
  const char *section,
  const char *name, 
  const char *value,
  int lineno
) {
  dbg("**rtl_data_handler started\n");

  matrix_config_t *data = user;
  
  fill_str_box(X_data_file_name);
  fill_str_box(W_data_file_name);
  fill_str_box(Y_data_file_name);

  return 0;
}
