#include "stdio.h"
#include "basket.h"
#include "items_types.h"
#include "menu.h"
#include "api/parse.h"
#include <string.h>

basket_s *basket_new(basket_item_s item) {
    basket_s *basket = malloc(sizeof(basket_s));
    basket->item = item;
    basket->next = NULL;
    return basket;
}

void basket_push(basket_s *basket, basket_item_s item) {
    // find the last element in the list
    basket_s *tail = basket;
    for (; tail->next != NULL; tail = tail->next);
    basket_s *new_item = basket_new(item);
    tail->next = new_item;
}

void basket_remove(basket_s *basket, size_t index) {
    basket_s *element = basket;
    basket_s *prev = NULL;
    for (int i = 0; i < index; i++) {
        prev = element;
        if (basket != NULL) {
            element = basket->next;
        }
    }
    prev->next = element->next;
    free(element);
}

void basket_remove_first(basket_s *basket) {
    basket_s *old_second = basket->next;
    basket_s new_head = *old_second;
    *basket = new_head;
    free(old_second);
}

void basket_free(basket_s *basket) {
    basket_s *element = basket;
    basket_s *prev = NULL;
    for (; element != NULL; element = element->next) {
        if (prev != NULL) {
            free(prev);
        }
        prev = element;
    }
    if (prev != NULL) {
        free(prev);
    }
}

int basket_to_array(basket_s *basket, basket_item_s **items) {
    int length = 0;
    basket_s *element = basket;
    while (element != NULL) {
        length++;
        element = element->next;
    }

    *items = malloc(length * sizeof(basket_item_s));
    element = basket;
    for (int i = 0; i < length; i++) {
        *items[i] = element->item;
        element = element->next;
    }
    return length;
}

basket_s *basket_read() {
    FILE *file = fopen(TEMP_BASKET_PATH, "r");

    if (file == NULL) {
        return NULL;
    }

    basket_item_s item;
    // Starting the list with an empty item is a bit hacky, but we remove it later
    basket_s *basket = basket_new(item);

    while (!feof(file)) {
        fscanf(file, " %[^|]|%lf|%d\n", item.name, &item.size, &item.unit);
        basket_push(basket, item);
    }

    basket_remove_first(basket);

    fclose(file);

    return basket;
}

void basket_write(basket_s *basket) {
    FILE *file = fopen(TEMP_BASKET_PATH, "w");

    for (basket_s *element = basket; element != NULL; element = element->next) {
        fprintf(file, "%s|%lf|%d\n", element->item.name, element->item.size, element->item.unit);
    }

    fclose(file);
}

void basket_print(basket_s *basket) {
    if (basket == NULL) {
        printf("The basket is empty\n\n");
        return;
    }
    for (basket_s *element = basket; element != NULL; element = element->next) {
        printf("%s - %.2lf %s\n", element->item.name, element->item.size, UNIT_NAMES[element->item.unit]);
    }
}

void menu_basket_edit() {
    char *options[] = {
        "Add items to basket",
        "Remove items from basket",
        "Print basket",
    };

    char *menu_text = "How would you like to edit the basket?";
    char *help_text = "Use !q to go back.";

    int option;

    while ((option = display_menu(options, menu_text, help_text)) != -1) {
        switch (option) {
            case 1:
                menu_basket_add();
                break;
            case 2:
                menu_basket_remove();
                break;
            case 3: {
                basket_s *basket = basket_read();
                basket_print(basket);
                basket_free(basket);
                break;
            }
        }
    }
}

void menu_basket_add() {
    basket_s *basket = basket_read();
    char input[ITEM_NAME_SIZE] = "";
    while (strcmp(input, "!q")) {
        basket_print(basket);
        printf("Enter !q to exit or !h to get help\n");
        basket_item_s item;
        printf("Enter the name of the item: ");
        fgets(input, sizeof(input), stdin);
        // Since `fgets` puts a newline at the end of the string, we simply find the first newline character in the buffer (which is what `strcspn` does) and replace it with a null-byte to mark the end of the string.
        input[strcspn(input, "\r\n")] = '\0';

        if (!strcmp(input, "!q")) {
            // I know that goto is evil, but we need to make sure that the basket is written to disk and then freed when leaving this function
            goto cleanup;
        } else if (!strcmp(input, "!h")) {
            printf("Enter the name of the item you would like to add to your basket\n");
            printf("Enter !q to exit or !h to get help\n");
            continue;
        }

        strcpy(item.name, input);

        while (1) {
            printf("Enter the amount: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\r\n")] = '\0';

            if (!strcmp(input, "!q")) {
                goto cleanup;
            } else if (!strcmp(input, "!h")) {
                printf("Enter the amount of the item you would like to add.\n");
                printf("Valid units are: mL, cL, dL, L, G, KG, STK\n");
                printf("Enter !q to exit or !h to get help\n");
                continue;
            }

            // TODO: Figure out how to handle an empty amount
            // This is a bit of a hack, but we put the user's unit input into the name of a store_item_s, so we can pass it to parse_populate_item_unit to find out the unit the user entered
            store_item_s store_item;
            strcpy(store_item.name, input);
            parse_populate_item_unit(&store_item);
            if (store_item.unit == UNKNOWN) {
                printf("Please enter a valid amount (A number followed by a unit. Enter !h for more information)\n");
            }
            item.size = store_item.size;
            item.unit = store_item.unit;
            if (basket != NULL) {
                basket_push(basket, item);
            } else {
                basket = basket_new(item);
            }
            break;
        }
    }

cleanup:
    basket_write(basket);
    basket_free(basket);
}

void menu_basket_remove() {
    basket_s *basket = basket_read();

    static const char* INVALID_OPTION_TEXT = "Please select a valid option\n\n";

    while (1) {
        basket_print(basket);
        int item_count = 1;
        for (basket_s *element = basket; element != NULL; element = element->next) {
            printf("[%d] %s - %.2lf %s\n", item_count, element->item.name, element->item.size, UNIT_NAMES[element->item.unit]);
            item_count++;
        }

        char input[16] = "";

        printf("Enter !q to exit or !h to get help\n");
        printf("Select an item to remove: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\r\n")] = '\0';

        // Try to convert the user input to a base 10 number. Returns 0 if the number is invalid
        int option = strtol(input, NULL, 10);

        if (option == 0) {
            if (strlen(input) != 2) {
                printf("%s", INVALID_OPTION_TEXT);
                continue;
            }
            if (strstr(input, "!q") != NULL) {
                break;
            }
            if (strstr(input, "!h") != NULL) {
                printf("Enter the number of the item you wish to remove, or !q to exit.\n\n");
            } else {
                // If the user entered something invalid, we just ignore it and try again
                printf("%s", INVALID_OPTION_TEXT);
            }
            continue;
        }
        if (option > item_count) {
            printf("%s", INVALID_OPTION_TEXT);
            continue;
        }
        basket_remove(basket, option-1);
    }

    basket_write(basket);
    basket_free(basket);
}
