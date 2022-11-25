#include <stdio.h>
#include <curl/curl.h>
#include "api/fetch.h"
#include "items.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{
    CURRENT_DIRECTORY = args[0];
    printf("Hello, World!\n");
    return 0;
}