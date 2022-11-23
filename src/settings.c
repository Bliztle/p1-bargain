#include "settings.h"

int main(void) {
    printf("build success\n\n");

    menu_settings();

    return 0;
}

void menu_settings() {
    char user_input[100];

    get_settings_input(user_input, __func__);
}

void get_settings_input(char* user_input, const char* func_name) {
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