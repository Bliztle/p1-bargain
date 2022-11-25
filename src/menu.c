#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

/**
 * Prints a numbered list of `options`, and allows the user to pick one of them, or get help. This is not intended to be called directly, but through the `print_menu` macro.
 * This is just a wrapper for `_print_menu` that automatically passes the length of the array.
 * @returns the index (starting at 0) of the menu option that the user picked. Returns -1 if the user wants to quit
 * @param options The list of menu options to be printed
 * @param menu_text The text presented at the top of the menu
 * @param help_text The text printed when the user asks for help
 */
#define display_menu(options, menu_text, help_text) _print_menu(options, sizeof(options)/sizeof(char*), menu_text, help_text)

int _display_menu(char **options, int options_len, char *menu_text, char *help_text) {
    while (1) {
        printf("%s\n", menu_text);
        printf("Enter !q to quit, or !h to get help");
        // Print the menu options
        for (int i = 0; i < options_len; i++) {
            printf("[%d] %s\n", i+1, options[i]);
        }
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
