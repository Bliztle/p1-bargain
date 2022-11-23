#include "settings.h"

int main(void) {
    printf("build success\n\n");

    menu_settings();

    return 0;
}

void menu_settings() {
    char user_input[2];

    while (1) {
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
}