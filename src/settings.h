/// INCLUDES
#include <stdio.h>
#include <string.h>
#include "api/parse.h" // Include for basket struct

/// DEFINES
#define QUIT_CMD "!q"
#define HELP_CMD "!h"

/// TYPEDEFS
typedef enum {PATH, ADDRESS, DISTANCE, DEVIATION} settings_e;

/// FUNCTION PROTOTYPES
/**
 * @brief Returns if user inputs !q, otherwise redirects to other settings
 */
void menu_settings();

/**
 * @brief Handles user command and calls functions depending on the input
 * @param user_input the user input
 * @return -1, 0 or 1 depending on path
*/
int settings_get_command(char* user_input);

/**
 * @brief Get coordinates of the address
*/
void settings_get_coord();

/**
 * @brief Edit the given setting
 * @param user_input users input
 * @param setting the setting to change
 */
void settings_edit(char* user_input, int setting);

/**
 * @brief Validates address
 * @return true or false
 */
int settings_validate_address(void *value);

/**
 * @brief Validates distance
 * @return true or false
 */
int settings_validate_distance(void *value);

/**
 * @brief Validates the deviation
 * @return true or false
*/
int settings_validate_deviation(void *value);

/**
 * @brief Validates the path
 * @return true or false
*/
int settings_validate_path();

//! TEMP FUNCTIONS
/**
 * @brief Prints current settings in the terminal for the user
*/
void settings_print_menu();