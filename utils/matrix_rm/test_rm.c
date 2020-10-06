#include "../ini/ini.h"
#include "theini_handler/ini_loader.h"
#include "reference_model/matrix_rm.h"

#include <string.h>
#include <stdlib.h> // malloc
#include <stdio.h> // printf
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
    char *input_dir_name = "";
    
    int i;
    for(i = 1; i < argc; ++i) {
      // вычисление аргумента флага "--config"
      const char *config_flag = "--config=";
      const int config_flag_length = strlen(config_flag);

      if(!strncmp(argv[i], config_flag, config_flag_length)) {
        char *p = strpbrk(argv[i], "=");
        
        config_file_name = (char *) malloc(strlen(argv[i]) - config_flag_length + 1);
        strcpy(config_file_name, p + 1);
        dbg("config_file_name=%s\n", config_file_name);
      }

      // вычисление аргумента флага "--input_dir"
      const char *input_dir_flag = "--input_dir=";
      const int input_dir_flag_length = strlen(input_dir_flag);

      if(!strncmp(argv[i], input_dir_flag, input_dir_flag_length)) {
        char *p = strpbrk(argv[i], "=");

        input_dir_name = (char *) malloc(strlen(argv[i]) - input_dir_flag_length + 1);
        strcpy(input_dir_name, p + 1);
        dbg("input_dir_name=%s\n", input_dir_name);
      }
    }

    char userdata[USERDATA_LEN];

    int res_parse_ini = ini_parse(config_file_name, theini_handler, userdata);
    dbg("res_parse_ini=%d\n", res_parse_ini);
  // Finished parse config ini-file

    matrix_config_t *config = userdata;

  // Prepare path to files for reference model 
    int input_dir_name_length = strlen(input_dir_name);

    config->user_X_data_file_name = (char *) realloc(
      config->user_X_data_file_name,
      strlen(config->user_X_data_file_name) + input_dir_name_length
    );
    strcpy(config->user_X_data_file_name + input_dir_name_length, config->user_X_data_file_name);
    strncpy(config->user_X_data_file_name, input_dir_name, input_dir_name_length);

    config->user_W_data_base_file_name = (char *) realloc(
      config->user_W_data_base_file_name,
      strlen(config->user_W_data_base_file_name) + input_dir_name_length
    );
    strcpy(config->user_W_data_base_file_name + input_dir_name_length, config->user_W_data_base_file_name);
    strncpy(config->user_W_data_base_file_name, input_dir_name, input_dir_name_length);

    config->user_Y_data_file_name = (char *) realloc(
      config->user_Y_data_file_name,
      strlen(config->user_Y_data_file_name) + input_dir_name_length
    );
    strcpy(config->user_Y_data_file_name + input_dir_name_length, config->user_Y_data_file_name);
    strncpy(config->user_Y_data_file_name, input_dir_name, input_dir_name_length);

    config->X_data_file_name = (char *) realloc(
      config->X_data_file_name,
      strlen(config->X_data_file_name) + input_dir_name_length
    );
    strcpy(config->X_data_file_name + input_dir_name_length, config->X_data_file_name);
    strncpy(config->X_data_file_name, input_dir_name, input_dir_name_length);

    config->W_data_file_name = (char *) realloc(
      config->W_data_file_name,
      strlen(config->W_data_file_name) + input_dir_name_length
    );
    strcpy(config->W_data_file_name + input_dir_name_length, config->W_data_file_name);
    strncpy(config->W_data_file_name, input_dir_name, input_dir_name_length);

    config->Y_data_file_name = (char *) realloc(
      config->Y_data_file_name,
      strlen(config->Y_data_file_name) + input_dir_name_length
    );
    strcpy(config->Y_data_file_name + input_dir_name_length, config->Y_data_file_name);
    strncpy(config->Y_data_file_name, input_dir_name, input_dir_name_length);
  // Finished prepare

  // Start process reference model's functions
    make_all (config);
  // Finished process reference model's functions

  // Free memory
    free(config->user_X_data_file_name);
    free(config->user_W_data_base_file_name);
    free(config->user_Y_data_file_name);
    free(config->X_data_file_name);
    free(config->W_data_file_name);
    free(config->Y_data_file_name);

  printf("_____________________Test finished \"test_rm\"_____________________\n");
  return 0;
}