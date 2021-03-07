#include "config_test.h"


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage: give a string of a line.\n");
        exit(1);
    }
    // printf("=====delete indent test=====\n");
    // delete_indent_test(argv[1]);
    // printf("=====create config section title test=====\n");
    // create_config_section_title(argv[1]);
    // printf("=====config option title test=====\n");
    // create_config_option_title_test(argv[1]);
    // printf("=====line read test=====\n");
    // config_line_read_test(test_file_open("test.ini"));
    printf("=====config section test=====\n");
    create_config_section_test(argv[1]);

}

FILE* test_file_open(char* filenames)
{
    FILE* f = fopen(filenames, "r");
    if(f==NULL)
    {
        char message[1024];
        sprintf(message, "%s is invalid file name\n", filenames);
        perror(message);
        exit(1);
    }
    return f;
}

int config_line_read_test(FILE* f)
{
    int is_eof = 0;
    while(!is_eof)
    {
        char buf[1024];
        int value = _config_line_read(f, buf, sizeof(buf));
        switch(value)
        {
            case CLK_EOF:
                printf("eof\n");
                is_eof = 1;
                break;
            
            case CLK_COMMENT: 
                printf("comment:%s", buf);
                break;
            
            case CLK_SECTION_TITLE:
                printf("title:%s", buf);
                break;

            case CLK_OPTION:
                printf("option:%s", buf);
                break;
        }
    }

    return -1;
}

void create_config_option_test(char* line)
{
    config_option *op;
    op = malloc(sizeof(op));
    op = _create_config_option(line);
    printf("title:%s\n", get_config_option_title(op));
    printf("value:%s\n", get_config_option_value(op));
}

void create_config_section_test(char* filename)
{
    FILE* f = test_file_open(filename);
    config_section* result = _create_config_sections(f);
    fclose(f);

    print_config_sections(result);
}

void print_config_sections(config_section* section)
{
    config_section* current_config_section = section;
    while(1)
    {
        if(current_config_section == NULL) break;
        config_option* current_option = get_config_section_options(current_config_section);
        printf("<SectionTitle: %s>\n", get_config_section_title(current_config_section));
        printf("<Options>\n");
        while(1)
        {
            if(current_option== NULL) break;
            printf("\t%s: %s", get_config_option_title(current_option), get_config_option_value(current_option));
            current_option = get_next_config_option(current_option);
        }

        current_config_section = get_next_config_section(current_config_section);
    }
}

void create_config_section_title(char* line)
{
    printf("%s\n",_create_config_section_title(line));
}

void create_config_option_title_test(char* line)
{
    char* title;
    title = (char*)malloc(1024);
    _create_option_title(line, title);
    printf("%s\n", title);
}

void delete_indent_test(char* line)
{
    printf("receve string:\n%s\n", line);
    int count;
    line = _delete_indent(line, &count, 1024);
    printf("indent deleted:\n%s\n",line);
    printf("delete charactors count:%d\n", count);
}