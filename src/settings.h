/// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

/// DEFINES
#define QUIT_CMD "!q"
#define HELP_CMD "!h"

#define RED "\033[31m" // Color for error messages 
#define RESET "\033[0m" // Reset to default

#define FILE_TYPE ".txt"

/// TYPEDEFS
typedef enum {
    PATH, // 0
    ADDRESS, // 1
    DISTANCE, // 2
    DEVIATION // 3
} settings_e;

/// FUNCTION PROTOTYPES
/**
 * @brief Returns if user inputs !q, otherwise redirects to other settings
 */
void menu_settings();

/**
 * @brief Handles user command and calls functions depending on the input
 * @param input the user input
 * @return -1, 0 or 1 depending on path
*/
int settings_get_command(char* input);

/**
 * @brief Get coordinates of the address
*/
void settings_get_coord();

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

void settings_print_menu(char* input);
