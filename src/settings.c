#include "settings.h"



int main(void) {
    printf("build success\n\n");
    menu_settings();

    return 0;
}

void menu_settings() {
    char user_input[100];

    while (1) {
        //print_menu();

        scanf("%s", user_input);

        if (strlen(user_input) == 1) {
            char first_char = user_input[0];

            switch (first_char) {
                case '1':
                    return;

                case '2':
                    return;

                case '3':
                    return;

                case '4':
                    return;
            }
        } 

        else if (settings_get_command(user_input) == 1) {
            break;
        }

        fprintf(stderr, "Error: Invalid command\n");
    }
}

void settings_print_menu() {
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

int settings_get_command(char* user_input) {
    if (!strcmp(user_input, QUIT_CMD)) {
        return 1;
    }
    else if (!strcmp(user_input, HELP_CMD)) {
        printf("Help menu\n");
        return 0;
    }
    else {
        return -1;
    }
}

int settings_std_edit(char* user_input) {
    //* print_setting();

    //* read_new_setting();

    return settings_get_command(user_input);
}


void settings_edit(char* user_input, int setting) {
    while (1) {
        //* print_setting();

        //* read_new_setting();

        if (settings_get_command(user_input) == 1) {
            return;
        }

        if (settings_validate(user_input, setting)) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid input\n");
        }
    }
}

int settings_validate(char *value,int setting) {
    switch (setting) {
        case SAVEPATH:
            return settings_validate_path(value);

        case ADDRESS:
            return settings_validate_address(value);

        case DISTANCE:
            return settings_validate_distance(value);

        case DEVIATION:
            return settings_validate_deviation(value);

        default:
            return 0;
    }
}
int settings_validate_path(char *value) {
    FILE* file = fopen(value, "r");

    if (file == NULL) {
        perror("error");
        return 0;
    }

    fclose(file);
    return 1;
}

int settings_validate_deviation(char *value) {
    char *endptr;
    double deviation = strtod(value, &endptr);

    if (deviation >= 0) return 1;

    return 0;
}

int settings_validate_distance(char *value) {
    char *endptr;
    double distance = strtod(value, &endptr);

    if (distance >= 0) return 1;

    return 0;
}

int settings_validate_address(char *value) {
    // TODO
}

void settings_get_coord() {
    // TODO
}