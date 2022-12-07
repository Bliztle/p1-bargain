#include <stdio.h>
#include <curl/curl.h>
#include "api/fetch.h"
#include "bargain.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{
    CURRENT_DIRECTORY = args[0];
    // TODO: Run conf_setup() to setup the config file.
    bargain_run_bargain();
    return 0;
}
