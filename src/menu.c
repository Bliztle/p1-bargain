#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

static const char *INVALID_OPTION_TEXT = "Please select a valid option\n\n";

int _display_menu(char **options, int options_len, char *menu_text, char *help_text)
{
    while (1)
    {
        printf("%s\n", menu_text);
        // Print the menu options
        for (int i = 0; i < options_len; i++)
        {
            printf("[%d] %s\n", i + 1, options[i]);
        }
        printf("Enter !q to quit, or !h to get help\n");
        printf("> ");
        // We need to read the input into a string first, because we need to check whether it's a number, a command, or something invalid
        // The {'\0'} initializes the array to be filled with the '\0' character
        char input[16] = {'\0'};
        fgets(input, sizeof(input), stdin);
        // Since `fgets` puts a newline at the end of the string, we simply find the first newline character in the buffer (which is what `strcspn` does) and replace it with a null-byte to mark the end of the string.
        input[strcspn(input, "\r\n")] = '\0';

        // Try to convert the user input to a base 10 number. Returns 0 if the number is invalid
        int option = strtol(input, NULL, 10);

        if (option == 0)
        {
            if (strlen(input) != 2)
            {
                printf("%s", INVALID_OPTION_TEXT);
                continue;
            }
            if (strstr(input, "!q") != NULL)
            {
                return -1;
            }
            if (strstr(input, "!h") != NULL)
            {
                printf("%s\n\n", help_text);
            }
            else
            {
                // If the user entered something invalid, we just ignore it and try again
                printf("%s", INVALID_OPTION_TEXT);
            }
            continue;
        }
        option--; // We subtract 1 from the option, because the user sees the options starting at 1, but we store them in an array starting at 0.
        if (option > options_len - 1)
        {
            printf("%s", INVALID_OPTION_TEXT);
            continue;
        }
        return option;
    }
}

int _display_bargain_menu(char **options, int options_len, char *menu_text, char *help_text)
{
    while (1)
    {
        printf("%s\n", menu_text);
        // Print the menu options
        for (int i = options_len - 1; i > - 1; i--)
        {
            printf("| [%02d] %s |\n",
                    i + 1,
                    options[i]);
        }
        printf("|----------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
        printf("|                                                                 Enter !q to quit, or !h to get help                                                                  |\n");
        printf("|======================================================================================================================================================================|\n");
        printf("> ");
        // We need to read the input into a string first, because we need to check whether it's a number, a command, or something invalid
        // The {'\0'} initializes the array to be filled with the '\0' character
        char input[16] = {'\0'};
        fgets(input, sizeof(input), stdin);
        // Since `fgets` puts a newline at the end of the string, we simply find the first newline character in the buffer (which is what `strcspn` does) and replace it with a null-byte to mark the end of the string.
        input[strcspn(input, "\r\n")] = '\0';

        // Try to convert the user input to a base 10 number. Returns 0 if the number is invalid
        int option = strtol(input, NULL, 10);

        if (option == 0)
        {
            if (strlen(input) != 2)
            {
                printf("%s", INVALID_OPTION_TEXT);
                continue;
            }
            if (strstr(input, "!q") != NULL)
            {
                return -1;
            }
            if (strstr(input, "!h") != NULL)
            {
                printf("%s\n\n", help_text);
            }
            else
            {
                // If the user entered something invalid, we just ignore it and try again
                printf("%s", INVALID_OPTION_TEXT);
            }
            continue;
        }
        //option--; // We subtract 1 from the option, because the user sees the options starting at 1, but we store them in an array starting at 0.
        if (option > options_len || option < 0)
        {
            printf("%s", INVALID_OPTION_TEXT);
            continue;
        }
        return options_len - option;
    }
}
