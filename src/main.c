#include <stdio.h>
#include <curl/curl.h>
#include "api/fetch.h"
#include "settings.h"
#include "api/parse.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{
    printf("Hello, World!\n");

    char* address = malloc(100 * sizeof(char));

    if (address == NULL) {
        perror("Exit");
        exit(EXIT_FAILURE);
    }

    strcpy(address, "Løgumkloster");

    fetch_coordinates(address);
    parse_coordinates(address);

    return 0;
}