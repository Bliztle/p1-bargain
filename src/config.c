#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define SETTINGS_PATH "settings.conf"

//static const char* TOKENS_PATH = "tokens.conf";

int conf_read_settings(conf_settings_s * conf_settings) {
    FILE * config_file = fopen(SETTINGS_PATH, "r");
    if (config_file == NULL) {
        perror("File does not exist in this path");
        return 0;
    }

    conf_settings_s read_settings;

    fscanf(config_file, "%*s %s", conf_settings->shopping_list_save_path);
    fscanf(config_file, "%*s %d", &conf_settings->max_distance);
    fscanf(config_file, "%*s %[^\n]s", conf_settings->address);

    char lat_temp[20], lon_temp[20];
    fscanf(config_file, "%*s %s %s", lat_temp, lon_temp);
    char * endptr;
    conf_settings->address_lat = strtod(lat_temp, &endptr);
    conf_settings->address_lon = strtod(lon_temp, &endptr);

    char dev_temp[20];
    fscanf(config_file, "%*s %s", dev_temp);
    conf_settings->deviance = strtod(dev_temp, &endptr);

    fclose(config_file);
    return 1;
}

int conf_write_settings(conf_settings_s settings) {
    FILE * config_file = fopen(SETTINGS_PATH, "w");
    if (config_file == NULL) {
        perror("File does not exist in this path");
        return 0;
    }

    fprintf(config_file, "path %s\n", settings.shopping_list_save_path);
    fprintf(config_file, "distance %d\n", settings.max_distance);
    fprintf(config_file, "address %s\n", settings.address);
    fprintf(config_file, "coords %lf %lf\n", settings.address_lat, settings.address_lon);
    fprintf(config_file, "deviance %lf", settings.deviance);

    fclose(config_file);
    return 1;
}

/*
int conf_setup() {
    if (!conf_check_valid()) {
        conf_create();
        if (conf_check_valid()) {
            printf("Error in creation of file");
            return 0;
        }
    }
    return 1;
}

int conf_check_valid() {

}*/