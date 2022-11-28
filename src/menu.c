#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

int _display_menu(char **options, int options_len, char *menu_text, char *help_text) {
    // TODO: figure out how to reprint the menu
    printf("%s\n", menu_text);
    printf("Enter !q to quit, or !h to get help\n");
    // Print the menu options
    for (int i = 0; i < options_len; i++) {
        printf("[%d] %s\n", i+1, options[i]);
    }
    while (1) {
        printf("> ");
        // Let's hope that no one decides to enter more than 16 characters in the menu :^)
        // We need to read the input into a string first, because we need to check whether it's a number, a command, or something invalid
        // The {'\0'} initializes the array to be filled with the '\0' character
        char input[16] = {'\0'};
        scanf(" %s", input);
        
        // Try to convert the user input to a base 10 number. Returns 0 if the number is invalid
        int option = strtol(input, NULL, 10);

        if (option == 0) {
            // TODO: There is probably a nicer way to write this, but this *should* cover any C-weirdness that might happen to the string
            if (strstr(input, "!q") != NULL) {
                return -1;
            }
            if (strstr(input, "!h") != NULL) {
                printf("%s\n", help_text);
            } else {
                // If the user entered something invalid, we just ignore it and try again
                printf("Please select a valid option\n");
            }
            continue;
        }
        return option;
    }
}
