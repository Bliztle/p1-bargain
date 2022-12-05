#include <stdio.h>
#include <curl/curl.h>
#include "api/fetch.h"
#include "config.h"
#include "settings.h"
#include "api/parse.h"


char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{
    conf_settings_s settings;
    menu_settings(&settings);

    return 0;
}