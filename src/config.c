#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define SETTINGS_PATH "settings.conf"

//static const char* TOKENS_PATH = "tokens.conf";

int conf_read_settings(conf_settings_s *settings) {
    FILE *config_file = fopen(SETTINGS_PATH, "r");

    if (config_file == NULL) {
        perror("Error");

        return 0;
    }

    fscanf(config_file, "%*s %s", settings->shopping_list_save_path);
    fscanf(config_file, "%*s %d", &settings->max_distance);
    fscanf(config_file, "%*s %[^\n]s", settings->address);

    char lat_temp[20], lon_temp[20];

    fscanf(config_file, "%*s %s %s", lat_temp, lon_temp);

    char *endptr;
    settings->address_lat = strtod(lat_temp, &endptr);
    settings->address_lon = strtod(lon_temp, &endptr);

    char dev_temp[20];

    fscanf(config_file, "%*s %s", dev_temp);

    settings->deviance = strtod(dev_temp, &endptr);

    fclose(config_file);

    return 1;
}

int conf_write_settings(settings_s *settings) {
    FILE *config_file = fopen(SETTINGS_PATH, "w");

    if (config_file == NULL) {
        perror("Error");
        return 0;
    }

    fprintf(config_file, "path %s\n", settings->shopping_list_save_path);
    fprintf(config_file, "distance %d\n", settings->max_distance);
    fprintf(config_file, "address %s\n", settings->address);
    fprintf(config_file, "coords %lf %lf\n", settings->address_lat, settings->address_lon);
    fprintf(config_file, "deviance %lf", settings->deviance);

    fclose(config_file);

    return 1;
}


int conf_setup() {
    if (!conf_check_valid()) {
        conf_create();

        if (!conf_check_valid()) {
            printf("Error in creation of file");

            return 0;
        }
    }

    return 1;
}

int conf_check_valid() {
    // Can it be opened?
    FILE * config_file = fopen(SETTINGS_PATH, "r");

    if (config_file == NULL) {
        perror("Error");

        return 0;
    }

    // Is the savepath valid?
    char temp_s[100];

    fscanf(config_file, "%*s %s", temp_s);

    FILE *save_path = fopen(temp_s, "r");

    if  ( !strstr(temp_s, ".txt") || save_path == NULL) {
        return 0;
    }

    // Is the distance valid?
    int temp_d;

    fscanf(config_file, "%*s %d", &temp_d);

    if (temp_d < 0) {
        return 0;
    }

    // are the address and coords valid?
    fscanf(config_file, "%*s %[^\n]s", temp_s);
    // TODO get coords from Google's api

    // Is the deviance valid?
    double temp_lf;

    fscanf(config_file, "%*s %lf", &temp_lf);

    if (temp_d < 0) {
        return 0;
    }

    fclose(config_file);

    return 1;
}

void conf_create() {
    FILE *config_file = fopen(SETTINGS_PATH, "w");
    settings_s settings;

    char temp_s[100];

    printf("Please enter the save-path for your shopping list\n >");

    while (1) {
        scanf(" %s", temp_s);

        if (!strstr(temp_s, ".txt")) {
            printf("Please enter the path of a .txt file\n >");
            continue;
        }
        else {
            FILE *save_path = fopen(temp_s, "r");
            if (save_path == NULL) {
                printf("path is invalid, please enter a different path\n >");
                continue;
            }
        }

        strcpy(settings.shopping_list_save_path, temp_s);
        break;
    }

    int temp_d;
    printf("please enter the distance\n >");
    while (1) {
        scanf(" %d", &temp_d);

        if (temp_d < 0) {
            printf("distance cannot be negative\n >");
            continue;
        }

        settings.max_distance = temp_d;
        break;
    }

    printf("please enter the address\n >");
    while (1) {
        scanf(" %[0-9a-zA-Z ]", temp_s);

        // TODO function to get coords

        break;
    }

    double temp_lf;
    printf("please enter the deviance\n >");
    while (1) {
        scanf(" %lf", &temp_lf);

        if (temp_lf < 0) {
            printf("deviance cannot be negative\n >");
            continue;
        }

        settings.deviance = temp_lf;
        break;
    }
    conf_write_settings(&settings);

    fclose(config_file);
}