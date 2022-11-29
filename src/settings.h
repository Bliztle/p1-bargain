/// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "api/fetch.h"

/// DEFINES
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
void settings_edit(int setting);

/**
 * @brief Validates settings
 * @param new_input new user input
 * @param setting the setting to validate
 * @return true or false
 */
int settings_validate(char* new_input, int setting);

/**
 * @brief Validates address
 * @return true or false
 */
int settings_validate_address(char* new_input);

/**
 * @brief Validates distance
 * @return true or false
 */
int settings_validate_distance(char* new_input);

/**
 * @brief Validates the deviation
 * @return true or false
*/
int settings_validate_deviation(char* new_input);

/**
 * @brief Validates the path
 * @return true or false
*/
int settings_validate_path(char* new_input);