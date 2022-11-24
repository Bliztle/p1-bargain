/// INCLUDES
#include <stdio.h>
#include <string.h>
#include "api/parse.h" // Include for basket struct

/// DEFINES
#define QUIT_CMD "!q"
#define HELP_CMD "!h"

/// TYPEDEFS

enum settings {savepath_e, address_e, distance_e, deviation_e};

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
 * Prints the current value of distance, then changes it based on user input, and updates the distance in settings.conf
 * @param user_input the user input
 */
void menu_edit_distance(char* user_input);

/**
 * Prints the current value of deviation, then changes it based on user input, and updates the deviation in settings.conf
 * @param user_input the user input
 */
void menu_edit_deviation(char* user_input);



//! TEMP FUNCTIONS
/**
 * Prints current settings in the terminal for the user
*/
void settings_print_menu();

/**
 * Validates address
 * @return true or false
 */
int settings_validate_address(void *value);

/**
 * Validates distance
 * @return true or false
 */
int settings_validate_distance(void *value);

/**
 * Validates the deviation
 * @return true or false
*/
int settings_validate_deviation(void *value);

/**
 * Validates the path
 * @return true or false
*/
int settings_validate_path();