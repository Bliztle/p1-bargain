/// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api/parse.h" // Include for basket struct

/// DEFINES
#define QUIT_CMD "!q"
#define HELP_CMD "!h"

/// TYPEDEFS

typedef enum {SAVEPATH, ADDRESS, DISTANCE, DEVIATION} settings_e;

/// FUNCTION PROTOTYPES
/**
 * Returns if user inputs !q, otherwise redirects to other settings
 */
void menu_settings();

/**
 * Handles user command and calls functions depending on the input
 * @param user_input the user input
 * @return -1, 0 or 1 depending on path
*/
int settings_get_command(char* user_input);

/**
 * Standard edit format
 * @return result of get_command
 */
int settings_std_edit(char* user_input);

/**
 * Prints the current export path, then changes it based on user input, and updates the path in settings.conf
 * @param user_input the user input
 */
void menu_edit_path(char* user_input);

/**
 * Prints the current value of address, then changes it based on user input, and updates the coordinates in settings.conf
 * @param user_input the user input
 */
void menu_edit_address(char* user_input);

/**
 * Get coordinates of the address
*/
void settings_get_coord();

/**
 * Validates address
 * @return true or false
 */
int settings_validate(char *value, int setting);

/**
 * Validates address
 * @return true or false
 */
int settings_validate_address(char *value);

/**
 * Validates distance
 * @return true or false
 */
int settings_validate_distance(char *value);

/**
 * Validates the deviation
 * @return true or false
*/
int settings_validate_deviation(char *value);

/**
 * Validates the path
 * @return true or false
*/
int settings_validate_path(char *value);

//! TEMP FUNCTIONS
/**
 * Prints current settings in the terminal for the user
*/
void settings_print_menu();
