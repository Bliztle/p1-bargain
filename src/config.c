#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static const char* SETTINGS_PATH = "settings.conf";
static const char* TOKENS_PATH = "tokens.conf";

int main(void) {
    conf_settings_s current_settings = conf_read_settings();
    printf("%s\n", current_settings.shopping_list_save_path);
    printf("%d\n", current_settings.max_distance);
    printf("%s\n", current_settings.address);
    printf("%lf %lf\n", current_settings.address_lat, current_settings.address_lon);
    printf("%lf\n", current_settings.deviance);

    strcpy(current_settings.shopping_list_save_path, "C:\\Users\\username\\desktop\\bargain\\shoppinglist.txt");
    strcpy(current_settings.address , "korruptionsgade 5 100 christiansborg");
    current_settings.max_distance = 1500;
    current_settings.deviance = 0.050;
    current_settings.address_lat = 57.025760;
    current_settings.address_lon = 9.958440;

    conf_write_settings(current_settings);
}

conf_settings_s conf_read_settings() {
    FILE * config_file;
    char * path = "C:\\Users\\mlren\\CLionProjects\\p1-bargain\\src\\settings.conf";
    config_file = fopen(path, "r");
    if (config_file == NULL) {
        perror("File does not exist in this path");
        exit(EXIT_FAILURE);
    }

    conf_settings_s read_settings;

    fscanf(config_file, "%*s %s", read_settings.shopping_list_save_path);
    fscanf(config_file, "%*s %d", &read_settings.max_distance);
    fscanf(config_file, "%*s %[^\n]s", read_settings.address);

    char lat_temp[20], lon_temp[20];
    fscanf(config_file, "%*s %s %s", lat_temp, lon_temp);
    char * endptr;
    read_settings.address_lat = strtod(lat_temp, &endptr);
    read_settings.address_lon = strtod(lon_temp, &endptr);

    char dev_temp[20];
    fscanf(config_file, "%*s %s", dev_temp);
    read_settings.deviance = strtod(dev_temp, endptr);

    fclose(config_file);
    return read_settings;
}

void conf_write_settings(conf_settings_s settings) {
    FILE * config_file;
    char * path = "C:\\Users\\mlren\\CLionProjects\\p1-bargain\\src\\settings.conf";
    config_file = fopen(path, "w");
    if (config_file == NULL) {
        perror("File does not exist in this path");
        exit(EXIT_FAILURE);
    }

    fprintf(config_file, "path %s\n", settings.shopping_list_save_path);
    fprintf(config_file, "distance %d\n", settings.max_distance);
    fprintf(config_file, "address %s\n", settings.address);
    fprintf(config_file, "coords %lf %lf\n", settings.address_lat, settings.address_lon);
    fprintf(config_file, "deviance %lf", settings.deviance);

    fclose(config_file);
}