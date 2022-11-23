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

        if (user_input[0] == quit_command[0] && user_input[1] == quit_command[1]) {
            return;
        }
    }
}
