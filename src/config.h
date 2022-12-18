#ifndef P1_BARGAIN_CONFIG_H
#define P1_BARGAIN_CONFIG_H

#include "items_types.h"

typedef char conf_save_path_t[256];
typedef char conf_address_t[256];
typedef int conf_max_distance_t;
typedef double conf_deviance_t;

typedef struct {
    conf_save_path_t shopping_list_save_path;
    conf_address_t address;
    conf_address_t formatted_address;
    coord_lat_t address_lat;
    coord_lon_t address_lon;
    conf_max_distance_t max_distance;
    conf_deviance_t deviance;
} conf_settings_s;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api/parse.h"
#include "settings.h"

#define SETTINGS_PATH "settings.conf"

/**
 * Reads and parses the settings file stored on disk onto the address of the given struct
 * @param settings The struct to save the stored values on
 * @returns true (1) if successful and false (0) in case of an error
 */
int conf_read_settings(conf_settings_s *settings);

/**
 * Takes a struct as input and overwrites all data on config to the values in the struct
 * @param settings The struct to overwrite the current config
 * @returns true (1) if successful and false (0) in case of an error
 */
int conf_write_settings(conf_settings_s *settings);

/**
 * Used on startup, checks whether the current settings are incorrect in any way
 * @returns true (1) if successful and false (0) in case of an error
 */
void conf_setup();

/**
 * Validates each parameter in the config file
 * @returns true (1) if valid and false (0) if invalid
 */
int conf_check_valid();

/**
 * Used to create and/or overwrite the config
 */
void conf_create();

#endif