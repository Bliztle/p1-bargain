/*
*   Calls the settings menu and prompts the user for a choice. 
*   It then selects the right setting depending on the user input, 
*   and prints the current setting. The user is then prompted for a new setting, 
*   and the new input is validated. If it is valid it writes the value to 
*   settings.conf otherwise it gives an error.
*/

#include "settings.h"

// Just for testing purposes
int main(void) {
    menu_settings();

    return 0;
}

void menu_settings() {
    char* options[4];

    options[0] = "Change the export path";
    options[1] = "Change the address";
    options[2] = "Change the distance limit";
    options[3] = "Change the size deviation";

    char menu_text[100] = "Choose the setting you want to edit";
    char help_text[100] = "!q to quit the program";

    while (1) {
        int option = display_menu(options, menu_text, help_text);

        switch (option) {
            case PATH:
                settings_edit(PATH);
                break;

            case ADDRESS:
                settings_edit(ADDRESS);
                break;

            case DISTANCE:
                settings_edit(DISTANCE);
                break;

            case DEVIATION:
                settings_edit(DEVIATION);
                break;
        }
    }
}

void settings_edit(int setting) {
    char menu_text[100] = "Type in the new value";
    char help_text[100] = "!q to go back";

    while (1) {
        int option = display_menu(0, menu_text, help_text);

        if (option == -1) {
            return;
        }

        if (setting == ADDRESS) { // Get the coordinates if address is chosen
            settings_get_coord();
        }
    }
}

int settings_validate(char *new_input, int setting) {
    switch (setting) { // Maps setting to the right validation
        case PATH:
            return settings_validate_path(new_input);

        case ADDRESS:
            return settings_validate_address(new_input);

        case DISTANCE:
            return settings_validate_distance(new_input);

        case DEVIATION:
            return settings_validate_deviation(new_input);

        default:
            return 0;
    }
}

int settings_validate_path(char *new_input) {

    // Make sure the file type is right
    char file_type[] = FILE_TYPE;

    if (strstr(new_input, file_type) != file_type) {
        return 0;
    }

    // Since it technically still could be a folder, we open the file
    FILE* file = fopen(new_input, "r");

    if (file == NULL) { // A folder cannot be opened by fopen()
        perror("Error");
        return 0;
    }

    fclose(file);

    return 1;
}

int settings_validate_deviation(char *new_input) {
    // Converts string to double and checks if it's valid as a deviation
    char *endptr;
    double deviation = strtod(new_input, &endptr);

    return deviation >= 0;
}

int settings_validate_distance(char *new_input) {
    // Converts string to double and checks if it's valid as a distance
    char *endptr;
    double distance = strtod(new_input, &endptr);

    return distance >= 0;
}

int settings_validate_address(char *new_input) {
    // TODO: add validation for address
}

void settings_get_coord() {
    // TODO: get user coordinates from address
}