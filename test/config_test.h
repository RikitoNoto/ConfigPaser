#ifndef _CONFIG_TEST_H
#define _CONFIG_TEST_H 1
#define TEST_MODE 1

#include "../src/config.h"
#include <stdio.h>
#include <stdlib.h>
FILE* test_file_open(char* filenames);
int config_line_read_test(FILE* f);
void create_config_option_test(char* line);
void create_config_section_test(char* filename);
void print_config_sections(config_section* section);
void create_config_section_title(char* line);
void create_config_option_title_test(char* line);
void delete_indent_test(char* line);
#endif