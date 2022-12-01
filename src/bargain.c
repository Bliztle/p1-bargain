#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bargain.h"
#include "stores.h"
#include "items_types.h"
#include "menu.h"
#include "test_bargain.h"
// #include "api/fetch.h"

#define MAX_STORES_COUNT 3

typedef int (*compfn)(const void *, const void *);

void bargain_menu_find_bargain()
{

    store_s *stores = malloc(MAX_STORES_COUNT * sizeof(store_s));
    int store_count = bargain_find_bargain(stores);

    char *options[store_count];

    char *menu_text = "Found Bargains\nStore | Address | Distance | Total price | Items found\n--------------------------------\n";

    for (int i = 0; i < store_count; i++)
    {

        size_t option_length = (100 + strlen(stores[i].name) + strlen(stores[i].address)) * sizeof(char);
        char *option = malloc(option_length);

        snprintf(option, option_length, ": %s | %s | %d m. | %lf dkk. | %d/%d\n",
                 stores[i].name,
                 stores[i].address,
                 stores[i].distance,
                 stores[i].found_items_total_price,
                 stores[i].found_items_count,
                 stores[i].missing_items_count + stores[i].found_items_count);

        options[i] = option;
    }

    int selected_bargain = display_menu(options, menu_text, "Enter a number to select a bargain.");

    if (selected_bargain == -1)
    {
        return;
    }

    else if (selected_bargain >= 0)
    {
        bargain_menu_print_bargain(stores[selected_bargain]);
        bargain_menu_find_bargain();
    }
}

int bargain_find_bargain(store_s *stores)
{

    int store_count = test_fetch_get_stores(stores); // fetch_get_stores(&stores); TODO: Make sure everything in store structs is initialised beyond this point.

    stores_populate_store_items(stores, store_count);

    qsort(stores, store_count, sizeof(store_s), (compfn)stores_compare_stores);

    return store_count;
}

void bargain_print_bargain_result(store_s store)
{

    printf("SHOPPING LIST\n--------------------------------------\n# | Product | count | price/unit | total price\n");

    for (int i = 0; i < store.found_items_count; i++)
    {

        printf("%d | %s | %d | %lf | %lf\n", i + 1, store.found_items[i].name, store.found_items[i].count, store.found_items[i].price_per_unit, store.found_items[i].total_price);
    }

    printf("--------------------------------------\n Missing Items\n--------------------------------------\n # | Product\n--------------------------------------\n");

    for (int i = 0; i < store.missing_items_count; i++)
    {
        printf("%d | %s\n", i + 1, store.missing_items[i].name);
    }

    printf("--------------------------------------\n # | Distance | Items found | Total price\n");

    printf("Store: %s\nAddress: %s\nDistance: %dm.\nItems Found: %d/%d\nTotal Price: %lf dkk.\n",
           store.name,
           store.address,
           store.distance,
           store.found_items_count,
           store.missing_items_count + store.found_items_count,
           store.found_items_total_price);
}

char *bargain_get_print_bargain_string(store_s store)
{
    // realloc on a something not allocated by malloc is undefined behaviour.
    // So we need to allocate a string with the size of the initial data, and then copy it to the malloced string.
    char *bargain_string_start = "SHOPPING LIST\n--------------------------------------\n# | Product | count | price/unit | total price\0";
    char *bargain_string = malloc(strlen(bargain_string_start) * sizeof(char));
    strncat(bargain_string, bargain_string_start, strlen(bargain_string_start) * sizeof(char));

    int found_entry_size = 0;
    int missing_entry_size = 0;

    get_size_of_list_entries(store, &found_entry_size, &missing_entry_size);

    bargain_string = realloc(bargain_string, sizeof(bargain_string) + found_entry_size);

    create_found_entries(store, &bargain_string, (strlen(bargain_string) * sizeof(char)));

    if (store.missing_items_count < 0)
    {

        create_missing_entries(store, &bargain_string, (strlen(bargain_string) * sizeof(char)));
    }

    append_outro_to_string(store, &bargain_string, (strlen(bargain_string) * sizeof(char)));

    char *return_string = bargain_string;
    free(bargain_string);
    return return_string;
}

void bargain_menu_print_bargain(store_s store)
{

    char *options = "save shopping list: !s";

    // char *bargain_string = bargain_get_print_bargain_string(store);
    bargain_print_bargain_result(store);

    int n = display_menu(&options, "", "!s saves the shopping list as a text file to the localtion specified in user settings.\n");
    if (n == -1)
    {
        return;
    }
    else if (n > 0)
    {
        // export_bargain(); //TODO Impliment
        bargain_print_bargain_result(store);
        bargain_menu_print_bargain(store); // Stay in menu after export.
    }
}

void create_found_entries(store_s store, char **string_to_append_to, size_t size_of_string)
{

    for (int i = 0; i < store.found_items_count; i++)
    {

        char *temp_string = malloc(sizeof(char) * 45 + ITEM_NAME_SIZE + 1);
        snprintf(temp_string, sizeof(temp_string), "# %d | %s | %d | %.2lf/%s | %.2lf dkk.\n",
                 i,
                 store.found_items[i].name,
                 store.found_items[i].count,
                 store.found_items[i].price_per_unit,
                 bargain_get_unit(store.found_items[i].unit),
                 store.found_items[i].total_price);

        strncat(*string_to_append_to, temp_string, size_of_string + sizeof(temp_string) - sizeof(char));
        free(temp_string);
    }
}

void create_missing_entries(store_s store, char **string_to_append_to, size_t size_of_string)
{

    char *missing_intro = "--------------------------------------\nUNAVAILABLE ITEMS\n--------------------------------------\n# | Product\n\0";

    string_to_append_to = realloc(*string_to_append_to, sizeof(*string_to_append_to) + sizeof(*string_to_append_to) + sizeof(missing_intro));
    strncat(*string_to_append_to, missing_intro, sizeof(*string_to_append_to) + sizeof(missing_intro) - sizeof(char));

    for (int i = 0; i < store.missing_items_count; i++)
    {

        int size_of_name = sizeof(store.missing_items[i].name);
        char *temp_string = malloc(sizeof(char) * (7 + size_of_name));
        snprintf(temp_string, sizeof(temp_string), "#%d | %s\n",
                 i,
                 store.missing_items[i].name);

        strncat(*string_to_append_to, temp_string, sizeof(*string_to_append_to) + (sizeof(temp_string) - sizeof(char)));
        free(temp_string);
    }
}

void append_outro_to_string(store_s store, char **string_to_append_to, size_t size_of_string)
{

    char *items_outro = "--------------------------------------\nStore | Address | Distance | Items found | Total price\n\0";
    *string_to_append_to = realloc(*string_to_append_to, size_of_string + sizeof(char) * strlen(items_outro));

    char *temp_string = malloc(35 + strlen(items_outro) * sizeof(char));

    snprintf(temp_string, sizeof(temp_string), "%s%s | %s | %d | %d/%d | %.2lf\n",
             items_outro,
             store.name,
             store.address,
             store.distance,
             store.found_items_count,
             store.found_items_count + store.missing_items_count,
             store.found_items_total_price);

    *string_to_append_to = realloc(temp_string, size_of_string + strlen(temp_string) * sizeof(char));

    strncat(*string_to_append_to, temp_string, strlen(*string_to_append_to) * sizeof(char) + size_of_string);

    free(temp_string);
}

void get_size_of_list_entries(store_s store, int *found_list_size, int *missing_list_size)
{

    if (found_list_size != 0)
        found_list_size = 0;
    if (missing_list_size != 0)
        missing_list_size = 0;

    for (int i = 0; i < store.found_items_count; i++)
    {
        found_list_size += sizeof(store.found_items[i].name) + sizeof(char) * 46;
    }

    for (int i = 0; i < store.missing_items_count; i++)
    {
        missing_list_size += sizeof(store.missing_items[i].name) + sizeof(char) * 6;
    }
}

char *bargain_get_unit(int n)
{ // TODO make this better.

    switch (n)
    {
    case 0:
    {
        return "KILOGRAMS";
    }
    case 1:
    {
        return "LITERS";
    }
    case 2:
    {
        return "UNITS";
    }
    }
    return "Error"; // lol
}
