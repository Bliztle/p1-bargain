#include "settings.h"

int main(void) {
    printf("build\n");

    menu_settings();

    return 0;
}

void menu_settings() {
    char* quit_command = "!q";
    char user_input[100];

    while (1) {
        print_menu();

        scanf("%s", user_input);

        switch (user_input[0]) {
            case '!':
                switch (user_input[1]) {
                    case 'h':
                        break;
                    case 'q':
                        return;
                    default:
                        fprintf(stderr, "Error: invalid command");
                        return;
                }
            default:
                fprintf(stderr, "Error: invalid command");
                break;
        }
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