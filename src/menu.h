#pragma once
/**
 * Prints a numbered list of `options`, and allows the user to pick one of them, or get help. This is not intended to be called directly, but through the `print_menu` macro.
 * @returns the index (starting at 0) of the menu option that the user picked. Returns -1 if the user wants to quit
 * @param options The list of menu options to be printed
 * @param options_len The length of `options`
 * @param menu_text The text presented at the top of the menu
 * @param help_text The text printed when the user asks for help
 */
int _display_menu(char** options, int options_len, char* menu_text, char* help_text);

// TODO: talk about how this should be named
/**
 * Prints a numbered list of `options`, and allows the user to pick one of them, or get help. This is not intended to be called directly, but through the `display_menu` macro.
 * This is just a wrapper for `_display_menu` that automatically passes the length of the array.
 * @returns the index (starting at 0) of the menu option that the user picked. Returns -1 if the user wants to quit
 * @param options The list of menu options to be printed
 * @param menu_text The text presented at the top of the menu
 * @param help_text The text printed when the user asks for help
 */
#define display_menu(options, menu_text, help_text) _display_menu(options, sizeof(options)/sizeof(char*), menu_text, help_text)
