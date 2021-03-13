
/**
* @file config.c
* @brief This file is for read ini file.
* @author RikitoNoto
* @date 2021/03/07
* @details
* This file is for read and write ini file.
*/
#define CONFIG_STATIC 1
#include "config.h"



#define OPEN_ERROR "This file could not open: %s"
#define DEFAULT_SECTION_TITLE "DEFAULT"
#define CONFIG_NAME_SIZE 1024
#define OPTION_TITLE_SIZE 1024
#define OPTION_VALUE_SIZE 1024
#define OPTION_BUFFER_SIZE 2048


struct ConfigOption
{
    char title[OPTION_TITLE_SIZE];
    char value[OPTION_VALUE_SIZE];
    config_option* next_option;
};

struct ConfigSection
{
    char title[CONFIG_NAME_SIZE];
    config_option* options;
    config_section* next_section;
};

/*================================================================
    For Test
==================================================================*/
#if (TEST_MODE == 1)

int _config_line_read(FILE* f, char* buf, int maxcount)
{
    return  config_line_read(f, buf, maxcount);
}

config_option* _create_config_option(char* line)
{
    return create_config_option(line);
}

config_section* _create_config_sections(FILE* f)
{
    return create_config_sections(f);
}

char* _delete_indent(char* line, int* delete_count, int loop_count)
{
    return delete_indent(line, delete_count, loop_count);
}

char* _create_config_section_title(char* line)
{
    return create_config_section_title(line);
}

char* _create_option_title(char* line, char* title)
{
    return create_option_title(line, title);
}


#endif
/*================================================================
==================================================================*/

/*================================================================
    Accessor
==================================================================*/
char* get_config_section_title(config_section* section)
{
    return section->title;
}

config_option* get_config_section_options(config_section* section)
{
    return section->options;
}

config_section* get_next_config_section(config_section* section)
{
    return section->next_section;
}

char* get_config_option_title(config_option* op)
{
    return op->title;
}

char* get_config_option_value(config_option* op)
{
    return op->value;
}

config_option* get_next_config_option(config_option* op)
{
    return op->next_option;
}

/*================================================================
==================================================================*/

/**
* @brief read config file and return config_section structure.
* @param[in] filename Path of config file.
* @return Config sections infomation. This value is a chain structure.
* @details
* This function read ini file.
* After read, call create_config_section function.
* Retun value is chain structure, that structure allow you to access another section by next_section member. 
* if fail opening file, call raise_error function with OPEN_ERROR message.
*/
config_section* read_config_file(const char* filename)
{
    FILE *f;
    config_section* config;

    f = fopen(filename, "r");//file open.
    if(!f)
    {
        char* message = (char*)malloc(1024);
        sprintf(message,OPEN_ERROR, filename);
        raise_error(message);//call raise_error function with OPEN_ERROR message.
    }

    config = create_config_sections(f);//call section create function.

    fclose(f);//file close.
    return config;
}

/**
* @brief create config_section structure.
* @param[in] f FILE stream object of read ini file.
* @return config_section structure. That is the infomation of sections of receive ini file.
* @details
* This function read a receive file and create config_section structure.
*/
static config_section* create_config_sections(FILE* f)
{
    int is_eof = 0;
    config_section* result = (config_section*)malloc(sizeof(config_section));
    config_section* current_config_section = result;
    config_option* current_config_option = NULL;
    strncpy(result->title, DEFAULT_SECTION_TITLE, sizeof(DEFAULT_SECTION_TITLE)+1);
    while(!is_eof)
    {
        char buf[OPTION_BUFFER_SIZE];
        switch(config_line_read(f, buf, sizeof(buf)))
        {
            case CLK_EOF:
                is_eof = 1;
                current_config_section->next_section = NULL;
                break;

            case CLK_COMMENT:
                break;

            case CLK_SECTION_TITLE:
                current_config_section->next_section = (config_section*)malloc(sizeof(config_section));
                if(current_config_section->options != NULL) current_config_option->next_option = NULL;
                current_config_option = NULL;
                current_config_section = current_config_section->next_section;
                strncpy(current_config_section->title, create_config_section_title(buf), CONFIG_NAME_SIZE); 
                break;

            case CLK_OPTION:
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

    return result;
}


/**
* @brief create the title from a line for a config section struct.
* @param[in] line a section line surrounded '[]' of a ini file.
* @return the title.
* @details
* take out title surrounded '[]'.
* ignore indents and spaces before '['.
*/
static char* create_config_section_title(char* line)
{
    char* title = (char*)malloc(OPTION_BUFFER_SIZE);
    strncpy(title, line, OPTION_BUFFER_SIZE);

    for(int i = 0; i < OPTION_BUFFER_SIZE; i++)
    {
        if(title[i]=='[')
        {
            title = title + i + 1;
        }
        else if(title[i]==']')
        {
            title[i] = '\0';
            break;
        }
        else if(title[i]=='\0')
        {
            raise_error("Title context is failed.");
        }

        if(i==(OPTION_BUFFER_SIZE-1))
        {
            raise_error("Title context is too long.");
        }
    }

    return title;
}


/**
* @brief create a config option struct.
* @param[in] line a option line of a ini file.
* @return a config option struct.
* @details
* create config option struct.
* this function allocate memory for option struct,
* so there is a need free memory through free_config_option function.
*/
static config_option* create_config_option(char* line)
{
    config_option *op;

    op = (config_option*)malloc(sizeof(config_option));

    char* value_start_pointer = create_option_title(line, op->title);
    create_option_value(value_start_pointer, op->value);
    return op;
}

/**
* @brief create the title from a line for a config option struct.
* @param[in] line a option line of a ini file.
* @param[out] title the title of a option read from a line.
* @return a point to start of option value.
* @details
* read a line and create title,
* after return a pointer to start of option value.
*/
static char* create_option_title(char* line, char* title)
{
    char* value_start_pointer;
    int is_double_quotation = 0;
    int is_equal_serch = 1;//For double quotation.
    line = delete_indent(line, NULL, OPTION_TITLE_SIZE);
    
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
            line[OPTION_TITLE_SIZE-1]='\0';
            printf("%s", line);
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

/**
* @brief create the value from a line for a config option struct.
* @param[in] value_start_pointer point to start of value in a line.
* @param[out] value the value of an option read from a line.
* @return void.
* @details
* read a line and create value.
*/
static void create_option_value(char* value_start_pointer, char* value)
{
    value_start_pointer = delete_indent(value_start_pointer, NULL, OPTION_VALUE_SIZE);
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

/**
* @brief delete indents and spaces from first in a line.
* @param[in] line a line to delete indents and spaces.
* @param[out] delete_count count of characters deleted.
* @param[in] loop_count max count of loop.
* @return start point other characters.
* @details
* delete indents and spaces from first in a line until other characters.
*/
static char* delete_indent(char* line, int* delete_count, int loop_count)
{
    int buffer=0;
    for(int i=0;i<loop_count;i++)
    {
        if(line[i]==' '||line[i]=='\t') buffer++;
        else break;
    }
    line+=buffer;
    if(delete_count != NULL) *delete_count = buffer;
    return line;
}

/**
* @brief free a config section struct.
* @param[in] section config section struct.
* @return void
* @details
* free a config section struct.
* that time this function call a function for free a config option struct.
*/
void free_config_section(config_section* section)
{
    config_section* current_config_section;
    config_section* free_sec;
    current_config_section = section;
    while(1)
    {
        if(current_config_section == NULL) break;
        free_sec = current_config_section;
        current_config_section = free_sec->next_section;
        free(free_sec->title);
        free_config_option(free_sec->options);
        free(free_sec);
    }
}

/**
* @brief free a config option struct.
* @param[in] op config option struct.
* @return void
* @details
* free a config option struct.
*/
static void free_config_option(config_option* op)
{
    config_option* current_config_option;
    config_option* free_op;
    current_config_option = op;
    while(1)
    {
        if(current_config_option == NULL) break;
        free_op = current_config_option;
        current_config_option = free_op->next_option;
        free(free_op->title);
        free(free_op->value);
        free(free_op);
    }
}


/**
* @brief read a line and return this line infomation, store a line in buf.
* @param[in] f file stream to read.
* @param[out] buf buffer to store a line.
* @param[in] maxcount max character count of read file.
* @return Kind of line infomation.return integer value in ConfigLineKind.
* @details
* This function read a line in file stream.
* A line of read is start from
*     '['          -> return "title" value.
*     ';','\n','\r'-> return "comment" value.
*     EOF          -> return "EOF" value.
*     other        -> return "option" value.
* If occuered error, return -1.
*/
static int config_line_read(FILE* f, char* buf, int maxcount)
{
    if(fgets(buf, maxcount, f)==NULL) return CLK_EOF;//read a line. if EOF return EOF.
    for(int i=0;i < maxcount;i++)//loop until maxcount.
    {
        if(*buf == ' ' || *buf == '\t')
        {
            //if first character of a line is space or tab, move first position to next.
            buf++;
        }
        else if(*buf == ';'||*buf == '\n'||*buf == '\r')
        {
            return CLK_COMMENT;
        }
        else if(*buf == '[')
        {
            return CLK_SECTION_TITLE;
        }
        else
        {
            return CLK_OPTION;
        }

    }
    return -1;

}

/**
* @brief raise an error with out a message on the stderr.
* @param[in] message message for out on the stderr.
* @return void
* @details
* out message on the stderr, and exit this process.
*/
static void raise_error(char* message)
{
    fprintf(stderr, "%s", message);
    perror(NULL);
    exit(1);
}