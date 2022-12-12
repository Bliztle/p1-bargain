/*
*   Calls the settings menu and prompts the user for a choice. 
*   It then selects the right setting depending on the user input, 
*   and prints the current setting. The user is then prompted for a new setting, 
*   and the new input is validated. If it is valid it writes the value to 
*   settings.conf otherwise it gives an error.
*/

#include "settings.h"

void menu_settings(conf_settings_s *settings) {
    char *options[4];

    options[0] = "Change the export path";
    options[1] = "Change the address";
    options[2] = "Change the distance limit";
    options[3] = "Change the size deviation";

    char *menu_text = "Choose the setting you want to edit";
    char *help_text = "!q to quit the program";
    
    conf_read_settings(settings);

    while (1) {
        int selected_option = display_menu(options, menu_text, help_text);

        if (selected_option == -1) { // If !q
            break;
        }

        settings_edit(settings, selected_option);
    }
}

void settings_edit(conf_settings_s *settings, int setting) { // Edit the given setting
    char input[MAX_INPUT_SIZE];

    char *str1 = "Current setting is ";
    
    char str2[MAX_INPUT_SIZE];

    switch (setting) {
        case PATH:
            sprintf(str2, "%s", settings->shopping_list_save_path);
            break;
        
        case ADDRESS:
            sprintf(str2, "%s", settings->address);
            break;

        case DISTANCE:
            sprintf(str2, "%d", settings->max_distance);
            break;

        case DEVIATION:
            sprintf(str2, "%d", settings->deviance);
            break;
    }

    char* str3 = "\nEnter new setting";

    int len = strlen(str1) + strlen(str2) + strlen(str3);

    char menu_text[len];

    strncpy(menu_text, str1, len);
    strncat(menu_text, str2, len);
    strncat(menu_text, str3, len);

    // Print the menu text with current setting
    printf("\n%s\n", menu_text);

    char* help_text = "!q to go back";
    
    while (1) {
        printf(">");
        fgets(input, MAX_INPUT_SIZE, stdin);
        input[strcspn(input, "\r\n")] = '\0';

        if (strstr(input, "!q")) { // Return to menu_settings();
            return;
        }
        else if (strstr(input, "!h")) { // Print help text
            printf("%s\n", help_text);
        }
        else if (settings_validate(input, setting)) { // If input is valid for the current setting
            /*if (setting == ADDRESS) {
                fetch_status_e status_code = fetch_renew_stores();

                if (status_code != FETCH_STATUS_SUCCESS) {
                    printf("Error: fetch code %d\n", status_code); 

                    return;
                }
            }*/

            switch (setting) {
                case PATH:
                    sprintf(settings->shopping_list_save_path, "%s", input);
                    break;
                
                case ADDRESS:
                    sprintf(settings->address, "%s", input);

                    char* raw_coordinates;

                    fetch_coordinates(input, &raw_coordinates);

                    double lat;
                    double lon;

                    parse_coordinates(lat, lon, &raw_coordinates);

                    settings->address_lat = lat;
                    settings->address_lon = lon;
                    
                    break;

                case DISTANCE:
                    settings->max_distance = strtol(input, &input, 10);
                    break;

                case DEVIATION:
                    settings->deviance = strtol(input, &input, 10);
                    break;
             }

            conf_write_settings(settings);

            return;
        }
    }
}

int settings_validate(char *input, int setting) {
    switch (setting) { // Maps setting to the right validation
        case PATH:
            return settings_validate_path(input);

        case ADDRESS:
            return settings_validate_address(input);

        case DISTANCE:
            return settings_validate_distance(input);

        case DEVIATION:
            return settings_validate_deviation(input);
    }
}

int settings_validate_path(char *input) {
    // Make sure the file type is right
    char* substring = strstr(input, FILE_TYPE);

    // If there is no substring
    if (substring == NULL) {
        printf("Error: Unsupported file name\n");
        return 0;
    }

    // If there exists a substring but its not equivalent to the file type
    if (strcmp(substring, FILE_TYPE)) {
        printf("Error: Invalid file type\n");
        return 0;
    }

    // Since it technically still could be a folder ending on .txt, we open the file
    FILE *file = fopen(input, "r");

    if (file == NULL) { // A folder cannot be opened by fopen()
        perror("Error");
        return 0;
    }

    fclose(file);

    return 1;
}

int settings_validate_address(char *input) { // Checks if it was able to fetch and parse the input
    char *raw_coordinates;
    double lat, lon;

    fetch_status_e status_fetch = fetch_coordinates(input, &raw_coordinates) != FETCH_STATUS_SUCCESS; 
  
    if (status_fetch != FETCH_STATUS_SUCCESS) {
        printf("Error: Fetch status %d\n", status_fetch);
        
        return 0;
    }

    int status_parse = parse_coordinates(&lat, &lon, raw_coordinates);

    if (!status_parse) {
        printf("Error: Invalid address\n");

        return 0;
    }

    return 1;
}

int settings_validate_deviation(char *input) {
    // Converts string to double and checks if it's valid as a deviation
    int deviation = strtol(input, &input, 10);

    if (deviation <= 0) {
        printf("Error: Invalid deviation\n");

        return 0;
    }
    
    return 1;
}

int settings_validate_distance(char *input) {
    // Converts string to double and checks if it's valid as a distance
    int distance = strtol(input, &input, 10);

    if (distance <= 0) {
        printf("Error: Invalid distance\n");

        return 0;
    }
    
    return 1;
}