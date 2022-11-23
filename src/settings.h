/// INCLUDES
#include <stdio.h>
#include <string.h>
#include "api/parse.h"

/// DEFINES
#define QUIT_CMD "!q"
#define HELP_CMD "!h"

/// FUNCTION PROTOTYPES
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
 * Prints the current value of distance, then changes it based on user input, and updates the distance in settings.conf
 * @param user_input the user input
 */
void menu_edit_distance(char* user_input);

/**
 * Prints the current value of deviation, then changes it based on user input, and updates the deviation in settings.conf
 * @param user_input the user input
 */
void menu_edit_deviation(char* user_input);

/**
 * Returns if user inputs !q, otherwise redirects to other settings
 */
void menu_settings();

/**
 * Removes an item from the basket
 * @param items
 */
void menu_basket_remove(basket_item_s items);

/**
 * Adds an item to the basket
 * @param items
*/
void menu_basket_add(basket_item_s items);

/**
 * Prints current settings in the terminal for the user
*/
void print_menu();

/// HELPER FUNCTIONS
/**
 * Handles user command and calls functions depending on the input
 * @param user_input the user input
 * @param func_name the name of the function calling this function
*/
int get_command(char* user_input, const char* func_name);