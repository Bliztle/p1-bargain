#pragma once
#include "api/parse.h"

typedef char conf_address_t[100];
// TODO: The typedefs for coordinates already exist in parse.h. Maybe they should be somewhere else?
typedef double conf_deviance_t;
typedef int conf_max_distance_t;
typedef char conf_save_path_t[256];

typedef struct {
    conf_save_path_t shopping_list_save_path;
    conf_max_distance_t max_distance;
    conf_address_t address;
    coord_lat_t address_lat;
    coord_lon_t address_lon;
    conf_deviance_t deviance;
} conf_settings_s;

/**
 * Reads and parses the settings file stored on disk onto the address of the given struct
 * @param conf_settings The struct to save the stored values on
 * @returns true (1) if successful and false (0) in case of an error
 */
int conf_read_settings(conf_settings_s * conf_settings);

/**
 * @param settings The struct to overwrite the current config
 * @returns true (1) if successful and false (0) in case of an error
 */
int conf_write_settings(conf_settings_s settings);
