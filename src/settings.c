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
        get_settings_input(user_input, __func__);

        char first_char = user_input[0];

        if (strlen(user_input) == 1) {
            switch (first_char) {
                case '1':
                    printf("Edit savepath\n");
                    break;

                case '2':
                    printf("Edit address\n");
                    break;

                case '3':
                    printf("Edit distance\n");
                    break;

                case '4':
                    printf("Edit deviation\n");
                    break;

                default:
                    printf("Invalid input\n");
                    break;
            }
        }

    }
}

void get_settings_input(char* user_input, const char* func_name) {
    scanf("%s", user_input);

    if (!strcmp(user_input, QUIT_CMD)) {
        return;
    }
    else if (!strcmp(user_input, HELP_CMD)) {
        printf("Help menu\n");
    }
    else {
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