#include <stdio.h>
#include <curl/curl.h>
#include "api/fetch.h"
#include "settings.h"
#include "api/parse.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{

    char* raw_coordinates;
    char* input_address = "Selma Lagerløfs Vej 300";

    fetch_coordinates(input_address, &raw_coordinates);
    
    printf("%s", raw_coordinates);

    return 0;
}