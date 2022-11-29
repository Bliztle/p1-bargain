#include <stdio.h>
#include <curl/curl.h>
#include "api/fetch.h"
#include "settings.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{
    printf("Hello, World!\n");

    char* address = "Aalborg";

    fetch_coordinates(address);

    return 0;
}