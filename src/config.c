#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static const char* SETTINGS_PATH = "settings.conf";
static const char* TOKENS_PATH = "tokens.conf";

int main(void) {
    conf_read_settings();

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
    config_parse_path(config_file, read_settings.shopping_list_save_path);
    config_parse_distance(config_file, &read_settings.max_distance);

    printf("%s", read_settings.shopping_list_save_path);
    printf("%dm", read_settings.max_distance);

    fclose(config_file);
    return read_settings;
}

void config_write_settings(conf_settings_s settings) {

}

void config_parse_path(FILE * config_file, conf_save_path_t save_path) {
    int size = sizeof(conf_save_path_t) / sizeof(char);
    char str[size];
    fgets(str, size, config_file);
    char * ptr = strstr(str, " ") + 1; // Shift the pointer by 1 to not get whitespace in front
    strcpy(save_path, ptr);
}

void config_parse_distance(FILE * config_file, conf_max_distance_t * distance) {
    fscanf(config_file, "%*s %d", distance);
}