#include "config.h"

int conf_read_settings(conf_settings_s *settings) {
    FILE *config_file = fopen(SETTINGS_PATH, "r");

    if (config_file == NULL) {
        perror("Error");

        return 0;
    }

    fscanf(config_file, "%*s %s", settings->shopping_list_save_path);
    fscanf(config_file, "%*s %[^\n]s", settings->address);
    fscanf(config_file, "%*s %lf %lf", &settings->address_lat, &settings->address_lon);
    fscanf(config_file, "%*s %d", &settings->max_distance);
    fscanf(config_file, "%*s %lf", &settings->deviance);

    fclose(config_file);

    return 1;
}

int conf_write_settings(conf_settings_s *settings) {
    FILE *config_file = fopen(SETTINGS_PATH, "w");

    if (config_file == NULL) {
        perror("File does not exist in this path");
        return 0;
    }

    fprintf(config_file, "path %s\n", settings->shopping_list_save_path);
    fprintf(config_file, "address %s\n", settings->address);
    fprintf(config_file, "coords %lf %lf\n", settings->address_lat, settings->address_lon);
    fprintf(config_file, "distance %d\n", settings->max_distance);
    fprintf(config_file, "deviance %lf\n", settings->deviance);

    fclose(config_file);

    return 1;
}

void conf_setup() {
    while (!conf_check_valid()) {
        conf_create();
    }
}

int conf_check_valid() {
    // Can it be opened?
    FILE *config_file = fopen(SETTINGS_PATH, "r");

    if (config_file == NULL) {
        perror("Error");

        return 0;
    }

    char temp_s[100];
    fscanf(config_file, "%*s %s", temp_s);

    if (!settings_validate_path(temp_s)) {
        return 0;
    }

    fscanf(config_file, "%*s %[^\n]s", temp_s);
    if (!settings_validate_address(temp_s)) {
        return 0;
    }

    //skip coords
    fscanf(config_file, "%*s %s %s", temp_s, temp_s);

    fscanf(config_file, "%*s %s", temp_s);
    if (!settings_validate_distance(temp_s)) {
        return 0;
    }

    fscanf(config_file, "%*s %s", temp_s);
    if (!settings_validate_deviation(temp_s)) {
        return 0;
    }

    fclose(config_file);

    return 1;
}

void conf_create() {
    conf_settings_s settings;

    char temp_s[100];

    printf("Please enter the save-path for your shopping list\n");
    while (1) {
        printf(">");
        scanf(" %s", temp_s);

        if (!settings_validate_path(temp_s)) {
            continue;
        }

        strcpy(settings.shopping_list_save_path, temp_s);
        break;
    }

    printf("Please enter the address\n");
    while (1) {
        printf(">");
        scanf(" %[^\n]s", temp_s);
        if (!settings_validate_address(temp_s)) {
            continue;
        }
        strcpy(settings.address, temp_s);

        char *raw_coordinates;
        fetch_coordinates(temp_s, &raw_coordinates);
        parse_coordinates(&settings.address_lat, &settings.address_lon, raw_coordinates);
        break;
    }

    printf("Please enter the distance\n");
    while (1) {
        printf(">");
        scanf(" %s", temp_s);

        if (!settings_validate_distance(temp_s)) {
            continue;
        }

        settings.max_distance = strtol(temp_s, NULL, 10);
        break;
    }

    printf("Please enter the deviance\n");
    while (1) {
        printf(">");
        scanf(" %s", temp_s);

        if (!settings_validate_deviation(temp_s)) {
            continue;
        }

        settings.deviance = strtod(temp_s, NULL);
        break;
    }

    conf_write_settings(&settings);
}