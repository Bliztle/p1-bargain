#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *file_map[] = {
    [SETTINGS] = "settings.conf",
    [TOKENS] = "tokens.conf",
};

static const char *settings_map[] = {
    [ADDRESS] = "address",
    [COORDS] = "coords",
    [DEVIANCE] = "deviance",
    [DISTANCE] = "distance",
    [PATH] = "path",
};

static const char *tokens_map[] = {
    [SALLING] = "salling",
    [COOP] = "coop",
};

int main()
{
    printf("IO Main\n");
    // config_write_value("settings.conf", "address", "A string with spaces");
    // config_write_value("settings.conf", "appended", "some other string");
    // config_write_setting(PATH, "./din/mær");
    // config_write_setting(DISTANCE, "7");
    config_write_setting(DISTANCE, "hahahajegerikke et tal");
    return 0;
}

/**
 * Writes a configuration value to a config file, assuming file is following the "key value\n" format.
 * If the key exists, the existing value will be overridden.
 * Otherwise the key/value pair will be appended to the file.
 *
 * @returns a config status indicating wether or not the opperation was successful
 *
 * @param file_path Path to .conf file
 * @param key key to edit or add
 * @param value value to write. Cannot contain any linebreaks
 */
config_status_e config_write_value(char *file_path, char *key, char *value)
{
    if (strstr(value, "\n") != NULL)
        return CONFIG_FAILLURE;

    // Create new value
    int new_key_value_length = strlen(key) + 1 + strlen(value) + 1;
    char *new_key_value = malloc(new_key_value_length + 1);
    snprintf(new_key_value, new_key_value_length + 1, "%s %s\n", key, value);

    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        // Create file and write
        fclose(file);
        file = fopen(file_path, "w");
        fprintf(file, "%s", new_key_value);
        fclose(file);
        free(new_key_value);
        return CONFIG_SUCCESS;
    }

    // Read content
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = malloc(size + 1);
    fread(content, size, 1, file);
    fclose(file);

    // Split content into relevant parts
    char *rest = "", *key_ptr = strstr(content, key);
    if (key_ptr != NULL)
    {
        rest = strstr(key_ptr, "\n") + 1;

        // Add null-termination to make the first part of content its own string
        key_ptr[0] = '\0';
    }

    int rest_length = strlen(rest);
    int prev_length = (size + 1) - rest_length;

    int result_length = prev_length + new_key_value_length + rest_length + 1;
    char *result = malloc(result_length);
    char *result_end = result + result_length;

    // Combine and write result
    char *result_ptr = _memccpy(result, content, '\0', result_length);
    result_ptr = _memccpy(result_ptr - 1, new_key_value, '\0', result_end - result_ptr);
    if (result_ptr)
        result_ptr = _memccpy(result_ptr - 1, rest, '\0', result_end - result_ptr);

    file = fopen(file_path, "w");
    fprintf(file, "%s", result);

    // Cleanup
    fclose(file);
    free(content);
    free(new_key_value);
    free(result);

    return CONFIG_SUCCESS;
}

/**
 * Settings wrapper for config_write_value.
 *
 * @returns a config status indicating wether or not the opperation was successful
 *
 * @param key enum key for the setting to write
 * @param value value to write
 */
config_status_e config_write_setting(config_settings_e key, char *value)
{
    return config_write_value(file_map[SETTINGS], settings_map[key], value);
}

/**
 * Tokens wrapper for config_write_value.
 *
 * @returns a config status indicating wether or not the opperation was successful
 *
 * @param key enum key for token to write
 * @param value value to write
 */
config_status_e config_write_token(config_tokens_e key, char *value)
{
    return config_write_value(file_map[TOKENS], tokens_map[key], value);
}