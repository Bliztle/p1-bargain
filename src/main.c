#include <stdio.h>
#include <curl/curl.h>
#include "api/fetch.h"
#include "settings.h"
#include "api/parse.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{
    printf("Hello, World!\n");

    char input_address[10] = "Aalborg";

    fetch_coordinates(input_address);
    //parse_coordinates(address);

    return 0;
}