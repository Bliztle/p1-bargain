#pragma once
#include "api/parse.h"

typedef char conf_address_t[50];
// TODO: The typedefs for coordinates already exist in parse.h. Maybe they should be somewhere else?
typedef double conf_deviance_t;
typedef double conf_max_distance_t;
typedef char conf_save_path_t[256];

typedef struct {
    conf_address_t address;
    coord_lat_t address_lat;
    coord_lon_t address_lon;
    conf_max_distance_t max_deviance;
    conf_save_path_t shopping_list_save_path;
} conf_settings_s;

/**
 * Reads and parses the settings file stored on disk
 * @returns the parsed config
 */
conf_settings_s conf_read_settings();

/**
 * Writes the given settings to the settings file on disk
 * @param settings The settings to write
 */
void conf_write_settings(conf_settings_s settings);