#include <stdio.h>
#include <curl/curl.h>
#include "api/fetch.h"
#include "settings.h"
#include "api/parse.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{

    char* input_address1 = "Selma Lagerløfs Vej 300 9220";
    char* raw_coordinates1;

    fetch_coordinates(input_address1, &raw_coordinates1);

    char* input_address2 = "9220 300 Selma Lagerløfs Vej";
    char* raw_coordinates2;

    fetch_coordinates(input_address2, &raw_coordinates2);

    conf_settings_s setting1;
    conf_settings_s setting2;

    parse_coordinates(raw_coordinates1, &setting1);
    parse_coordinates(raw_coordinates2, &setting2);

    printf("Address1, lat: %lf, lon: %lf\n", setting1.address_lat, setting1.address_lon);
    printf("Address2, lat: %lf, lon: %lf\n", setting2.address_lat, setting2.address_lon);

    return 0;
}