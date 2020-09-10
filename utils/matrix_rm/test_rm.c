#include "algo/ini.h"
#include "theini_handler/ini_loader.h"
#include "reference_model/matrix_rm.h"

#include <string.h>
#include <stdlib.h> // malloc
#include <stdio.h> // printf
#include <stdio.h> // files
#include <assert.h>

#define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) printf("qspi: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif

#define USERDATA_LEN 1024


int main(int argc, char *argv[]) {
  printf("_____________________Test started \"test_rm\"_____________________\n");

  // Start parse config ini-file
    char *config_file_name = "";
    
    int i;
    for(i = 1; i < argc; ++i) {
      // вычисление аргумента флага "--config"
      const char *config_flag = "--config=";
      const int config_flag_length = strlen(config_flag);

      if(!strncmp(argv[i], config_flag, config_flag_length)) {
        char *p = strpbrk(argv[i], "=");
        
        config_file_name = (char *) malloc(strlen(argv[i]) - config_flag_length + 1);
        strcpy(config_file_name, p + 1);
      }
      dbg("config_file_name=%s\n", config_file_name);
    }

    char userdata[USERDATA_LEN];

    int res_parse_ini = ini_parse(config_file_name, theini_handler, userdata);
    dbg("res_parse_ini=%d\n", res_parse_ini);
  // Finished parse config ini-file

  // Start process reference model's functions
    matrix_config *config = userdata;
    
    dbg("matrix_config->H=%d\n", config->H);
    dbg("matrix_config->W=%d\n", config->W);
    dbg("matrix_config->K=%d\n", config->K);
    dbg("matrix_config->user_X_data_file_name=%s\n", config->user_X_data_file_name);
    dbg("matrix_config->Y_data_file_name=%s\n", config->Y_data_file_name);
    
  // Finished process reference model's functions

  printf("_____________________Test finished \"test_rm\"_____________________\n");
  return 0;
}