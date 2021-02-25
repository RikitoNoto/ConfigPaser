#ifndef _CONFIG_H
#define _CONFIG_H 1

#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ConfigOption config_option;
typedef struct ConfigSection config_section;
enum ConfigLineNumber;

#ifndef TEST_MODE
#define TEST_MODE 1
#endif

#if (TEST_MODE == 1)

int _config_line_read(FILE* f, char* buf);
config_option* _create_config_option(char* line);
// config_section* _create_config_section(char* line);


#endif

config_section* read_config_file(const char* filename);
char* get_config_option_title(config_option* op);
char* get_config_option_value(config_option* op);



// static config_section* create_config_sections(FILE* f);
// static config_section* create_config_section(char* line);
static config_option* create_config_option(char* line);
static char* create_option_title(char* line, char* title);
static void create_option_value(char* value_start_pointer, char* value);
static char* delete_indent(char* line);

static int config_line_read(FILE* f, char* buf);

static void free_config_option(config_option* op);

static void raise_error(char* message);



#endif // !_CONFIG_H
