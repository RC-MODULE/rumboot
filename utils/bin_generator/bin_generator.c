#include "../ini/ini.h"
#include "theini_handler/ini_gen_loader.h"

#include <string.h>
#include <stdlib.h> // malloc, strtol
#include <stdint.h>


// #define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) printf("bin_gen: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif

#define USERDATA_LEN 1024


int main(int argc, char const *argv[]) {
  printf("_________BIN_GENERATOR STARTED_________\n");

  char *ini_name = "";
  int length = 0;
  int seed = 0;
  int i, j;

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
  
  // parse ini-file
  gen_parameters *config;

  config = malloc(sizeof(gen_parameters));
  config->type = (char **) malloc(4*sizeof(char *));
  config->value = (char **) malloc(4*sizeof(char *));

  int res_parse_ini = ini_parse(ini_name, theini_handler, config);

  dbg("i'm print...\n");
  for(i = 0; i < 4; ++i) {
    if(config->type[i] != NULL) {
      dbg("type[%d]=%s\n", i, config->type[i]);
    }
    if(config->value[i] != NULL) {
      dbg("value[%d]=%s\n", i, config->value[i]);
    }
  }
  dbg("name=%s\n", config->name);

  for(i = 0; i < 4; ++i) {
    if((config->type[i] != NULL) && (config->value[i] == NULL) || 
       (config->type[i] == NULL) && (config->value[i] != NULL)) {
      printf("Error in ini-file\n");
      return 1;
    }
  }

  // create bin-file
  FILE *fd;
  fd = fopen(config->name, "w");
  if(!fd) {
    printf("Cannot open file\n");
    return 1;
  }

  srand(seed);

  for(i = 0; i < length; ++i) {
    for(j = 0; j < 4; ++j) {
      char *current_type = config->type[j];
      char *current_value = config->value[j];

      if(current_type != NULL) {
        if(!strcmp(current_type, "int8")) {
          int8_t number = (!strcmp(current_value, "all")) ? rand() : (int8_t)strtol(current_value, NULL, 0);
          fwrite(&number, sizeof(int8_t), 1, fd);
        } else {
          if(!strcmp(current_type, "int16")) {
            int16_t number = (!strcmp(current_value, "all")) ? rand() : (int16_t)strtol(current_value, NULL, 0);
            fwrite(&number, sizeof(int16_t), 1, fd);
          } else {
            if(!strcmp(current_type, "int32")) {
              int32_t number = (!strcmp(current_value, "all")) ? rand() : (int32_t)strtol(current_value, NULL, 0);
              fwrite(&number, sizeof(int32_t), 1, fd);
            } else {}
          }
        }
      } else {}
    }
  }

  fclose(fd);

  // free memory
  free(ini_name);
  free(config->type);
  free(config->value);
  free(config->name);

  printf("_________BIN_GENERATOR FINISHED________\n");
  return 0;
}