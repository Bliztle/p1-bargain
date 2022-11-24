#include "settings.h"

int main(void) {
    printf("build success\n\n");

    menu_settings();

    return 0;
}

void menu_settings() {
    char user_input[100];

    while (1) {
        print_menu();

        scanf("%s", user_input);

        if (strlen(user_input) == 1) {
            char first_char = user_input[0];

            switch (first_char) {
                case '1':
                    menu_edit_path(user_input);
                    return;

                case '2':
                    menu_edit_address(user_input);
                    return;

                case '3':
                    menu_edit_distance(user_input);
                    return;

                case '4':
                    menu_edit_deviation(user_input);
                    return;
            }
        } 
        
        if (settings_get_command(user_input) == 1) {
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

void menu_edit_path(char* user_input) {
    while (1) {
        if (settings_std_edit(user_input) == 1) {
            return;
        }

        if (settings_validate_path) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }
    

    settings_get_command(user_input);
}

int settings_validate_path() {
    // TODO
}

void menu_edit_address(char* user_input) {
    while (1) {
        if (settings_std_edit(user_input) == 1) {
            return;
        }

        settings_get_coord();

        if (settings_validate_coord) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }
}

void settings_get_coord() {
    // TODO
}

int settings_validate_coord() {
    // TODO
}


void menu_edit_distance(char* user_input) {
    while (1) {
        if (settings_std_edit(user_input) == 1) {
            return;
        }

        if (settings_validate_distance) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }
}

int settings_validate_distance() {
    // TODO
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

int settings_validate_deviation() {
    // TODO
}

int settings_validate_setting() {

}