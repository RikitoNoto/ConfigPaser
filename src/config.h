#ifndef _CONFIG_H
#define _CONFIG_H 1

#define __STDC_WANT_LIB_EXT1__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ConfigOption config_option;
typedef struct ConfigSection config_section;
enum ConfigLineKind {CLK_EOF, CLK_COMMENT, CLK_SECTION_TITLE, CLK_OPTION};


#ifndef TEST_MODE
#define TEST_MODE 1
#endif

#ifndef CONFIG_STATIC
#define CONFIG_STATIC 0
#endif

#if (TEST_MODE == 1)

int _config_line_read(FILE* f, char* buf, int maxcount);
config_option* _create_config_option(char* line);
config_section* _create_config_sections(FILE* f);
char* _delete_indent(char* line, int* delete_count, int loop_count);
char* _create_config_section_title(char* line);
char* _create_option_title(char* line, char* title);

#endif

config_section* read_config_file(const char* filename);

char* get_config_section_title(config_section* section);
config_option* get_config_section_options(config_section* section);
config_section* get_next_config_section(config_section* section);

char* get_config_option_title(config_option* op);
char* get_config_option_value(config_option* op);
config_option* get_next_config_option(config_option* op);

#if(CONFIG_STATIC == 1)

//=======config section functions=======
static char* create_config_section_title(char* line);
static config_section* create_config_sections(FILE* f);

//======================================

//=======config option functions=======
static config_option* create_config_option(char* line);
static char* create_option_title(char* line, char* title);
static void create_option_value(char* value_start_pointer, char* value);
//=====================================

static char* delete_indent(char* line, int* delete_count, int loop_count);
static int config_line_read(FILE* f, char* buf, int maxcount);

static void free_config_option(config_option* op);

static void raise_error(char* message);
#endif


#endif // !_CONFIG_H
