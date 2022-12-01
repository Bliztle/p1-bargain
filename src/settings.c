/*
*   Calls the settings menu and prompts the user for a choice. 
*   It then selects the right setting depending on the user input, 
*   and prints the current setting. The user is then prompted for a new setting, 
*   and the new input is validated. If it is valid it writes the value to 
*   settings.conf otherwise it gives an error.
*/

#include "settings.h"

void menu_settings() {
    char* options[4];

    options[0] = "Change the export path";
    options[1] = "Change the address";
    options[2] = "Change the distance limit";
    options[3] = "Change the size deviation";

    char* menu_text = "Choose the setting you want to edit";
    char* help_text = "!q to quit the program";

    while (1) {
        int selected_option = display_menu(options, menu_text, help_text);

        if (selected_option == -1) {
            break;
        }

        switch (selected_option) {
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

            default:
                printf("Invalid command, please enter again\n");
                break;
        }
    }
}

void settings_edit(int setting) {
    char input[MAX_INPUT_SIZE];

    char* str1 = "Current setting is ";
    char* str2 = "[setting]"; // TODO: = read from config
    char* str3 = "\nEnter new setting";

    int len = strlen(str1) + strlen(str2) + strlen(str3);

    char menu_text[len];

    strncpy(menu_text, str1, len);
    strncat(menu_text, str2, len);
    strncat(menu_text, str3, len);

    printf("\n%s\n", menu_text);

    char* help_text = "!q to go back";
    
    while (1) {
        printf(">");
        scanf(" %s", input);

        if (strstr(input, "!q")) {
            return;
        }
        else if (strstr(input, "!h")) {
            printf("%s\n", help_text);
        }
        else if (settings_validate(input, setting)) {
            // TODO: Write to config
            return;
        }
        else {
            printf("Error: invalid input\n");
        }
    }
}

int settings_validate(char *input, int setting) {
    char* raw_coordinates;
    conf_settings_s settings;

    switch (setting) { // Maps setting to the right validation
        case PATH:
            return settings_validate_path(input);

        case ADDRESS:
            fetch_coordinates(input, &raw_coordinates);
            return parse_coordinates(raw_coordinates, &settings);

        case DISTANCE:
            return settings_validate_distance(input);

        case DEVIATION:
            return settings_validate_deviation(input);

        default:
            return 0;
    }
}

int settings_validate_path(char *input) {
    // Make sure the file type is right
    if (strcmp(strstr(input, FILE_TYPE), FILE_TYPE)) {
        return 0;
    }

    // Since it technically still could be a folder, we open the file
    FILE* file = fopen(input, "r");

    if (file == NULL) { // A folder cannot be opened by fopen()
        perror("Error");
        return 0;
    }

    fclose(file);

    return 1;
}

int settings_validate_deviation(char *input) {
    // Converts string to double and checks if it's valid as a deviation
    char *endptr;
    double deviation = strtod(input, &endptr);

    return deviation >= 0;
}

int settings_validate_distance(char *input) {
    // Converts string to double and checks if it's valid as a distance
    char *endptr;
    double distance = strtod(input, &endptr);

    return distance >= 0;
}