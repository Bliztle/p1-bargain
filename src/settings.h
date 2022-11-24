/// INCLUDES
#include <stdio.h>
#include <string.h>
#include "api/parse.h" // Include for basket struct

/// DEFINES
#define QUIT_CMD "!q"
#define HELP_CMD "!h"

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
int get_command(char* user_input);

/**
 * Standard edit format
 * @return result of get_command
 */
int std_edit(char* user_input);

/**
 * Prints the current export path, then changes it based on user input, and updates the path in settings.conf
 * @param user_input the user input
 */
void menu_edit_path(char* user_input);

/**
 * Validates the export path
 * @return true or false
 */
int validate_path();

/**
 * Prints the current value of address, then changes it based on user input, and updates the coordinates in settings.conf
 * @param user_input the user input
 */
void menu_edit_address(char* user_input);

/**
 * Get coordinates of the address
*/
void get_coord();

/**
 * Validates coordinates
 * @return true or false
 */
int validate_coord();

/**
 * Prints the current value of distance, then changes it based on user input, and updates the distance in settings.conf
 * @param user_input the user input
 */
void menu_edit_distance(char* user_input);

/**
 * Validates distance
 * @return true or false
 */
int validate_distance();

/**
 * Prints the current value of deviation, then changes it based on user input, and updates the deviation in settings.conf
 * @param user_input the user input
 */
void menu_edit_deviation(char* user_input);

/**
 * Validates the deviation
 * @return true or false
*/
int validate_deviation();

/**
 * Menu to remove an item from the basket
 * @param items
 * @param basket_item_s basket
 */
void menu_basket_remove(char* user_input, basket_item_s items);

/**
 * Menu to add an item to the basket
 * @param items
 * @param basket_item_s basket
*/
void menu_basket_add(char* user_input, basket_item_s items);

/**
 * Add item to basket
 */
void add_to_basket();

/**
 * Validate item data
 * @return true or false
*/
int validate_data();

//! TEMP FUNCTIONS
/**
 * Prints current settings in the terminal for the user
*/
void print_menu();