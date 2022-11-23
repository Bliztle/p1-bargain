/// INCLUDES
#include <stdio.h>
#include <string.h>

/// DEFINES
#define QUIT_CMD "!q"
#define HELP_CMD "!h"

/// TYPEDEFS
typedef struct {
    // TODO: add settings
} basket_item_s;

/// FUNCTION PROTOTYPES
/**
 * Returns if user inputs !q, otherwise redirects to other settings
 */
void menu_settings();

/**
 * Prints the current value of deviation, then changes it based on user input, and updates the deviation in settings.conf
 */
void menu_edit_deviation();

/**
 * Prints the current value of distance, then changes it based on user input, and updates the distance in settings.conf
 */
void menu_edit_distance();

/**
 * Prints the current value of address, then changes it based on user input, and updates the coordinates in settings.conf
 */
void menu_edit_address();

/**
 * Prints the current export path, then changes it based on user input, and updates the path in settings.conf
 */
void menu_edit_path();

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

/// HELPER FUNCTIONS
/**
 * Handles user input and calls functions depending on the input
 * @param user_input the user input
 * @param func_name the name of the function calling this function
*/
void get_settings_input(char* user_input, const char* func_name);