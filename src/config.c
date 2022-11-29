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
    config_parse_address(config_file, &read_settings.address);

    printf("%s\n", read_settings.shopping_list_save_path);
    printf("%dm\n", read_settings.max_distance);
    printf("%s", read_settings.address);

    fclose(config_file);
    return read_settings;
}

void config_write_settings(conf_settings_s settings) {

}

void config_parse_path(FILE * config_file, conf_save_path_t save_path) {
    fscanf(config_file, "%*s %s", save_path);
    while (fgetc(config_file) != '\n') {
        // move the cursor to the end of the line
    }
}

void config_parse_distance(FILE * config_file, conf_max_distance_t * distance) {
    fscanf(config_file, "%*s %d", distance);
    while (fgetc(config_file) != '\n') {
        // move the cursor to the end of the line
    }
}

void config_parse_address(FILE * config_file, conf_address_t * address) {
    int size = sizeof(conf_address_t) / sizeof(char);
    char str[size];
    fgets(str, size, config_file);
    char * ptr = strstr(str, " ") + 1; // Shift the pointer by 1 to remove the whitespace
    strcpy(address, ptr);
}