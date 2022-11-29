#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static const char* SETTINGS_PATH = "settings.conf";
static const char* TOKENS_PATH = "tokens.conf";

int main(void) {
    
    return 0;
}

conf_settings_s conf_read_settings() {
    FILE * config_file;
    char * path = "NEEDS PATH";
    config_file = fopen(path, "r");
    if (config_file == NULL) {
        perror("File does not exist in this path");
        exit(EXIT_FAILURE);
    }

    conf_settings_s read_file;

    strcpy(read_file.shopping_list_save_path, config_parse_path());


    return read_file;
}

void config_write_settings(conf_settings_s settings) {

}

char * config_parse_path() {

}