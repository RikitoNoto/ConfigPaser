#include "config_test.h"


int main(int argc, char* argv[])
{
	// if(argc < 3)
    // {
    //     fprintf(stderr, "Usage: give a file name of ini file and true num.\n");
    //     exit(1);
    // }
    //config_line_read_test(test_file_open("test.ini"),atoi(argv[2]));

    if(argc < 2)
    {
        fprintf(stderr, "Usage: give a string of a line.\n");
        exit(1);
    }
    create_config_option_test(argv[1]);

}

FILE* test_file_open(char* filenames)
{
    FILE* f = fopen(filenames, "r");
    return f;
}

int config_line_read_test(FILE* f, int correct)
{
    char buf[1024];
    int value = _config_line_read(f, buf);
    printf("%d", value);
    if(value==correct)
    {
        printf("OK:%s\n",buf);
    }
    else
    {
        printf("NG:%s\n", buf);
    }

    return -1;
}

int create_config_option_test(char* line)
{
    config_option *op;
    op = malloc(sizeof(op));
    op = _create_config_option(line);
    printf("title:%s\n", get_config_option_title(op));
    printf("value:%s\n", get_config_option_value(op));
}