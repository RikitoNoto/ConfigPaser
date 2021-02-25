#ifndef _CONFIG_TEST_H
#define _CONFIG_TEST_H 1
#define TEST_MODE 1

#include "../src/config.h"
#include <stdio.h>
#include <stdlib.h>
FILE* test_file_open(char* filenames);
int config_line_read_test(FILE* f, int correct);
int create_config_option_test(char* line);
#endif