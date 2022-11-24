#include "settings.h"

int main(void) {
    printf("build\n");

    menu_settings();

    return 0;
}

void menu_settings() {
    char* str1 = "!q";
    char str2[100];

    while (1) {
        scanf("%s", str2);

        if (str1[1] == str2[1] && str1[2] == str2[2]) {
            return;
        }
    }
}
