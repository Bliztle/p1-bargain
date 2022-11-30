#include <stdio.h>
#include <curl/curl.h>
#include "api/fetch.h"
#include "settings.h"
#include "api/parse.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{
    printf("Hello, World!\n");

    char *input_address = "aalborg";
    char *raw_coordinates;

    fetch_coordinates(input_address, &raw_coordinates);
    parse_coordinates(raw_coordinates);

    return 0;
}