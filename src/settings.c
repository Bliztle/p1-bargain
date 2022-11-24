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

        int res = get_command(user_input, __func__);

        if (res == 1) {
            break;
        }

        fprintf(stderr, "Error: Invalid command\n");
    }
}

void print_menu() {
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

int get_command(char* user_input, const char* func_name) {
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

int std_edit(char* user_input, const char* func_name) {
    //* print_setting();

    //* read_new_setting();

    return get_command(user_input, __func__);
}

void menu_edit_path(char* user_input) {
    while (1) {
        if (std_edit(user_input, __func__) == 1) {
            return;
        }

        if (validate_path) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }


    get_command(user_input, __func__);
}

int validate_path() {
    // TODO
}

void menu_edit_address(char* user_input) {
    while (1) {
        if (std_edit(user_input, __func__) == 1) {
            return;
        }

        get_coord();

        if (validate_coord) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }
}

void get_coord() {
    // TODO
}

int validate_coord() {
    // TODO
}


void menu_edit_distance(char* user_input) {
    while (1) {
        if (std_edit(user_input, __func__) == 1) {
            return;
        }

        if (validate_distance) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }
}

int validate_distance() {
    // TODO
}

void menu_edit_deviation(char* user_input) {
    while (1) {
        if (std_edit(user_input, __func__) == 1) {
            return;
        }

        if (validate_deviation) {
            //* write_to_file();

            return;
        }
        else {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }
}

int validate_deviation() {
    // TODO
}

void menu_basket_remove(char* user_input, basket_item_s items) {
    // TODO
}

void menu_basket_add(char* user_input, basket_item_s items) {
    while (1) {
        //* print_basket();

        while (1) {
            //* get_data();

            int res = get_command(user_input, __func__);

            if (res == 1) {
                return;
            }

            if (validate_data()) {
                add_to_basket();
                break;
            }
            else {
                fprintf(stderr, "Error: Item invalid\n");
            }
        }
    }
}

int validate_data() {
    // TODO
}

void add_to_basket() {
    // TODO
}