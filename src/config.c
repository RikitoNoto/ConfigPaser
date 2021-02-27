#include "config.h"



#define OPEN_ERROR "This file could not open: %s"
#define DEFAULT_SECTION_NAME "DEFAULT"
#define CONFIG_NAME_SIZE 1024
#define OPTION_TITLE_SIZE 1024
#define OPTION_VALUE_SIZE 1024
#define OPTION_BUFFER_SIZE 2048

enum ConfigLineNumber {CLN_EOF, CLN_COMMENT, CLN_SECTION_TITLE, CLN_OPTION};

struct ConfigOption
{
    char title[OPTION_TITLE_SIZE];
    char value[OPTION_VALUE_SIZE];
    config_option* next_option;
};

struct ConfigSection
{
    char name[CONFIG_NAME_SIZE];
    config_option* options;
    config_section* next_section;
};

#if (TEST_MODE == 1)

int _config_line_read(FILE* f, char* buf)
{
    return  config_line_read(f, buf);
}

config_option* _create_config_option(char* line)
{
    return create_config_option(line);
}

// config_section* _create_config_section(char* line)
// {
//     create_config_section(line);
// }

#endif

// config_section* read_config_file(const char* filename)
// {
//     FILE *f;
//     config_section* config;
//     f = fopen(filename, "r");
//     if(!f)
//     {
//         char* message;
//         sprintf(message,OPEN_ERROR, filename);
//         raise_error(message);
//     }
//     // config = create_config_sections(f);


//     fclose(f);
//     return config;
// }

char* get_config_option_title(config_option* op)
{
    return op->title;
}

char* get_config_option_value(config_option* op)
{
    return op->value;
}

// static config_section* create_config_sections(FILE* f)
// {
//     config_section section;
//     for(;;)
//     {
//         char* buf[OPTION_BUFFER_SIZE];
//         fgets(buf, sizeof(buf), f);
//         if(buf == NULL) break;
//         if(buf[0]==';') continue;


//     }
// }

static config_section* create_config_sections(FILE* f)
{
    int is_eof = 0;
    config_section* result = (config_section*)malloc(sizeof(config_section));
    config_section* current_config_section = result;
    config_option* current_config_option = NULL;
    strncpy(result->name, DEFAULT_SECTION_NAME, sizeof(DEFAULT_SECTION_NAME)+1);
    while(!is_eof)
    {
        char buf[OPTION_BUFFER_SIZE];
        switch(config_line_read(f, &buf))
        {
            case CLN_EOF:
                is_eof = 1;
                current_config_section->next_section = NULL;
                break;
            
            case CLN_COMMENT:
                break;

            case CLN_SECTION_TITLE://add title
                current_config_section->next_section = (config_section*)malloc(sizeof(config_section));
                if(current_config_section->options != NULL) current_config_section->options->next_option = NULL;
                current_config_section = current_config_section->next_section;
                break;
            
            case CLN_OPTION:
                if(current_config_option == NULL)
                {
                    current_config_section->options = create_config_option(buf);
                    current_config_option = current_config_section->options;
                }
                else
                {
                    current_config_option->next_option = create_config_option(buf);
                    current_config_option = current_config_option->next_option;
                }
                break;

            default:
                break;
        }

    }
        
}

static char* create_config_section_title(char* line)
{
    char* start_point;
    char* title = (char*)malloc(OPTION_BUFFER_SIZE);

    line = delete_indent(line);
    
    for(int i = 0; i < OPTION_BUFFER_SIZE; i++)
    {
        if(line[i]=='[')
        {
            start_point = &(line[i]);
        }
        else if(line[i]==']')
        {
            line[i] = '\0';
            break;
        }
        else if(line[i]=='\0')
        {
            raise_error("Title context is failed.");
        }

        if(i==(OPTION_BUFFER_SIZE-1))
        {
            raise_error("Title context is too long.");
        }
    }

    strncpy(title, start_point, OPTION_BUFFER_SIZE);
    return title;
}

static config_option* create_config_option(char* line)
{
    config_option *op;

    op = (config_option*)malloc(sizeof(config_option));

    char* value_start_pointer = create_option_title(line, op->title);
    create_option_value(value_start_pointer, op->value);
    return op;
}

static char* create_option_title(char* line, char* title)
{
    char* value_start_pointer;
    int is_double_quotation = 0;
    int is_equal_serch = 1;//For double quotation.
    line = delete_indent(line);
    if(*line == '\"')
    {
        is_equal_serch = 0;
        line++;
        is_double_quotation = 1;
    }
    for(int i=0; i <= OPTION_TITLE_SIZE; i++)
    {
        if(i==OPTION_TITLE_SIZE)
        {
            raise_error("Too long title or invalid title.");
        }

        if(is_double_quotation)
        {
            if(line[i]=='\"' && line[i-1]!='\\')
            {
                is_equal_serch = 1;
                line[i] = '\0';
            }
        }
        if(is_equal_serch)
        {
            if(line[i]=='=' || line[i]==':') 
            {
                line[i] = '\0';
                value_start_pointer = &(line[i+1]);
                break;
            }
            else if(line[i]==' ')
            {
                line[i] = '\0';
            }
        }
    }


    strncpy(title, line, OPTION_TITLE_SIZE);
    return value_start_pointer;
}

static void create_option_value(char* value_start_pointer, char* value)
{
    value_start_pointer = delete_indent(value_start_pointer);
    if(*value_start_pointer == '\"')
    {
        value_start_pointer++;
        for(int i=0; i<(OPTION_VALUE_SIZE-1); i++)
        {
            if(value_start_pointer[i]=='\"' && value_start_pointer[i-1]!='\\')
            {
                value_start_pointer[i] = '\0';
                break;
            }
        }
    }
    strncpy(value, value_start_pointer, OPTION_VALUE_SIZE);
}

static char* delete_indent(char* line)
{
    int buffer=0;
    for(int i=0;i<sizeof(line);i++)
    {
        if(line[i]==' '||line[i]=='\t')
        {
            buffer++;
        }
    }
    line+=buffer;
    return line;
}

static void free_config_option(config_option* op)
{
    free(op->title);
    free(op->value);
    free(op);
}

static int config_line_read(FILE* f, char* buf)
{
    /*
    read a line from stream "f".
    read line is inserted buf.
    return value is a number of kind of line.
    0: EOF
    1: comment
    2: section title
    3: option
    -1: error
    */
    fgets(buf, sizeof(buf), f);
    if(*buf==EOF) return CLN_EOF;
    for(int i=0;i < sizeof(buf);i++)
    {
        printf("buf:%c", *buf);
        if(*buf == ' ' || *buf == '\t')
        {
            buf += 1;
        }
        else if(*buf == ';'||*buf == '\n')
        {
            return CLN_COMMENT;
        }
        else if(*buf == '[')
        {
            return CLN_SECTION_TITLE;
        }
        else
        {
            return CLN_OPTION;
        }

    }
    return -1;

}

static void raise_error(char* message)
{
    fprintf(stderr, "%s", message);
    exit(1);
}