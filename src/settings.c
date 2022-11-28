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
    printf("\nbuild success\n\n");

    menu_settings();

    return 0;
}

void menu_settings() {
    // Store the user's input-string in a "large enough" array
    char user_input[100];

    /*settings_print_menu();*/
    //TODO display_menu(options, menu_text, help_text);

    

    display_menu(options, menu_text, help_text);


    while (1) {
        scanf("%s", user_input);

        // Edit the chosen setting from numeration
        if (strlen(user_input) == 1) {
            char first_char = user_input[0];

            switch (first_char) {
                case '1':
                    settings_edit(PATH);
                    break;

                case '2':
                    settings_edit(ADDRESS);
                    break;

                case '3':
                    settings_edit(DISTANCE);
                    break;

                case '4':
                    settings_edit(DEVIATION);
                    break;
            }
        } 
        // If !q quit the loop
        else if (settings_get_command(user_input) == 1) {
            break;
        }
        else {// Input didn't relate to any command or setting, so try again
            fprintf(stderr, RED "\nError: Invalid option\n\n" RESET);
            printf("Please try again>");
        }
    }
}

void settings_print_menu() { // Temporary printing of the settings
    char savepath[] = "IDK lol";
    printf("1. Change savepath\nCurrent savepath is: %s\n\n", savepath);

    char address[] = "308 negra arroyo lane, albuquerque, New Mexico";
    printf("2. Change address\nCurrent address is: %s\n\n", address);

    double distance = 1.5;
    printf("3. Change distance\nCurrent distance is: %.2lf km\n\n", distance);

    double deviation = 0.1;
    printf("4. Change deviation\nCurrent deviation is: %.2lf\n\n", deviation);

    printf("Please enter the setting you wish to edit> ");
}

int settings_get_command(char* input) { // Look for command through the input
    if (!strcmp(input, QUIT_CMD)) { // Command is quit (!q)
        return 1;
    }
    else if (!strcmp(input, HELP_CMD)) { // Command is help (!h)
        printf("Help menu\n");
        return 0;
    }
    else { // No command was found
        return -1;
    }
}

void settings_edit(int setting) {
    while (1) {
        //* char **options, char* menu_text, char* help_text, 
        //TODO display_menu(options, menu_text, help_text);

        // Store the user's input-string in a "large enough" array
        char new_input[100];

        printf("\nCurrent setting is [setting]\n");
        printf("Enter new setting>");
        scanf("%s", new_input);

        if (setting == ADDRESS) { // Get the coordinates if address is chosen
            settings_get_coord();
        }

        if (settings_get_command(new_input) == 1) { // Check for command
            return;
        }
        else if (settings_validate(new_input, setting)) { // Validation of user's input
            // TODO: write_to_file();

            return;
        }
        else {
            fprintf(stderr, RED "Error: Invalid input\n" RESET);
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
        perror(RED "Error" RESET);
        return 0;
    }

    fclose(file);

    return 1;
}

int settings_validate_deviation(char *new_input) {
    // Converts string to double and checks if it's valid as a deviation
    char *endptr;
    double deviation = strtod(new_input, &endptr);

    if (deviation >= 0) return 1;

    return 0;
}

int settings_validate_distance(char *new_input) {
    // Converts string to double and checks if it's valid as a distance
    char *endptr;
    double distance = strtod(new_input, &endptr);

    if (distance >= 0) return 1;

    return 0;
}

int settings_validate_address(char *new_input) {
    // TODO: add validation for address
}

void settings_get_coord() {
    // TODO: get user coordinates from address
}