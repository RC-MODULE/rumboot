#include "../ini/ini.h"
#include "theini_handler/ini_gen_loader.h"

#include <string.h>
#include <stdlib.h> // malloc, atoi


#define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) printf("bin_gen: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif

#define USERDATA_LEN 1024


int main(int argc, char const *argv[]) {
  char *ini_name = "";
  int length = 0;
  int seed = 0;
  int i;

  for(i = 1; i < argc; ++i) {
    // вычисление аргумента флага "--ini"
    const char *ini_flag = "--ini=";
    const int ini_flag_length = strlen(ini_flag);

    if(!strncmp(argv[i], ini_flag, ini_flag_length)) {
      char *p = strpbrk(argv[i], "=");
      
      ini_name = (char *) malloc(strlen(argv[i]) - ini_flag_length + 1);
      strcpy(ini_name, p + 1);
    }

    // вычисление аргумента флага "--length"
    const char *length_flag = "--length=";
    const int length_flag_length = strlen(length_flag);
    
    if(!strncmp(argv[i], length_flag, length_flag_length)) {
      char *p = strpbrk(argv[i], "=");
      char *length_str = "";

      length_str = (char *) malloc(strlen(argv[i]) - length_flag_length + 1);
      strcpy(length_str, p + 1);
      length = atoi(length_str);
      free(length_str);
    }

    // вычисление аргумента флага "--seed"
    const char *seed_flag = "--seed=";
    const int seed_flag_length = strlen(seed_flag);
    
    if(!strncmp(argv[i], seed_flag, seed_flag_length)) {
      char *p = strpbrk(argv[i], "=");
      char *seed_str = "";

      seed_str = (char *) malloc(strlen(argv[i]) - seed_flag_length + 1);
      strcpy(seed_str, p + 1);
      seed = atoi(seed_str);
      free(seed_str);
    }
  }

  dbg("ini_name=%s\n", ini_name);
  dbg("length=%d\n", length);
  dbg("seed=%d\n", seed);
  
  char userdata[USERDATA_LEN];

  int res_parse_ini = ini_parse(ini_name, theini_handler, userdata);
  gen_parameters *config;
  init_gen_parameters(&config);
  config = userdata;

  for(i = 0; i < 4; ++i) {
    dbg("type[%d]=%s\n", i, config->type[i]);
  }

  free(ini_name);
  // free_gen_parameters(&config);
  return 0;
}