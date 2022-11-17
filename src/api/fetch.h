#include <stddef.h>

typedef struct
{
    char *response;
    size_t size;
} fetch_response;

void fetch_example();
int fetch_salling_test();
int get_token(char vendor[10], char token[36]);
void print_file_at_current_location();
size_t write_callback(char *buffer, size_t size, size_t buffer_length, void *prelim_response);