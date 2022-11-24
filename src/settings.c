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
                    settings_edit(user_input, PATH);
                    return;

                case '2':
                    settings_edit(user_input, ADDRESS);
                    return;

                case '3':
                    settings_edit(user_input, DISTANCE);
                    return;

                case '4':
                    settings_edit(user_input, DEVIATION);
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

void settings_edit(char* user_input, int setting) {
    while (1) {
        //* print_setting();

        //* read_new_setting();

        if (setting == ADDRESS) {
            settings_get_coord();
        }

        if (settings_get_command(user_input) == 1) {
            return;
        }
        else if (settings_validate(user_input, setting)) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid input\n");
        }
    }
}

void menu_edit_deviation(char* user_input) {
    while (1) {
        if (settings_std_edit(user_input) == 1) {
            return;
        }

        if (settings_validate_deviation) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }
}

int settings_validate(void *value, int setting) {
    switch (setting) {
        case savepath_e:
            return settings_validate_path(value);

        case address_e:
            return settings_validate_address(value);

        case distance_e:
            return settings_validate_distance(value);

        case deviation_e:
            return settings_validate_deviation(value);
    }
}

int settings_validate_path(void *value) {

}

int settings_validate_deviation(void *value) {
    // TODO
}

int settings_validate_distance(void *value) {
    printf("adada");
}

int settings_validate_coord(void *value) {
    // TODO
}