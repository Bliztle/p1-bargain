#ifndef P1_BARGAIN_SETTINGS_H
#define P1_BARGAIN_SETTINGS_H

/// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "api/fetch.h"
#include "api/parse.h"
#include "config.h"

/// DEFINES
#define MAX_INPUT_SIZE 256
#define FILE_TYPE ".txt"

/// TYPEDEFS
typedef enum {
    PATH = 1, // 1
    ADDRESS, // 2
    DISTANCE, // 3
    DEVIATION // 4
} settings_e;

/// FUNCTION PROTOTYPES
/**
 * @brief Runs the settings menu
 */
void menu_settings();

/**
 * @brief Edit the given setting
 * @param setting the setting to change
 */
void settings_edit(conf_settings_s *settings, int setting);

/**
 * @brief Validates settings
 * @param input user input
 * @param setting the setting to validate
 * @return true or false
 */
int settings_validate(char *input, int setting);

/**
 * @brief Validates the path
 * @param input user input
 * @return true or false
*/
int settings_validate_path(char *input);

/**
 * @brief Validates the address
 * @param input 
 * @return true or false 
 */
int settings_validate_address(char *input);

/**
 * @brief Validates distance
 * @param input user input
 * @return true or false
 */
int settings_validate_distance(char *input);

/**
 * @brief Validates the deviation
 * @param input user input
 * @return true or false
*/
int settings_validate_deviation(char *input);

#endif