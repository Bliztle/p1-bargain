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

    conf_settings_s settings;

    fetch_coordinates(input_address, &raw_coordinates);

    parse_coordinates(raw_coordinates, &settings);

    printf("lat: %lf, lon: %lf\n", settings.address_lat, settings.address_lon);

    return 0;
}