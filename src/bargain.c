#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bargain.h"
#include "stores.h"
#include "items_types.h"
#include "menu.h"
#include "test_bargain.h"
#include "api/fetch.h"
#include "test_functions.h"

#define MAX_STORES_COUNT 3
#define MAIN_MENU_ITEMS_COUNT 3

typedef int (*compfn)(const void *, const void *);

void bargain_run_bargain()
{
    char *options[MAIN_MENU_ITEMS_COUNT];

    options[0] = "Find Bargains";
    options[1] = "Edit Shopping List";
    options[2] = "Settings";

    int choice = display_menu(options, "Main Menu", "Enter the number of the option you want to choose, or !q to quit");

    while (1)
    {
        if (choice == -1)
            break;

        switch (choice)
        {
        case 0:
            bargain_menu_find_bargain();
            break;
        case 1:
            // TODO: Implement Basket Editor
            break;
        case 2:
            // TODO: Implement Settings
            break;
        }

        choice = display_menu(options, "Main Menu", "Enter the number of the option you want to choose, or !q to quit");
    }
}

void free_store(store_s *store)
{
    if (store->items_count > 0)
    {
        free(store->items);
        store->items_count = 0;
    }
    if (store->found_items_count > 0)
    {
        free(store->found_items);
        store->found_items_count = 0;
    }
    if (store->missing_items_count > 0)
    {
        free(store->missing_items);
        store->missing_items_count = 0;
    }
}

store_s deep_copy_store(store_s *source)
{
    store_s new_store = {
        .group = source->group,
        .chain = source->chain,
        .lat = source->lat,
        .lon = source->lon,
        .found_items = NULL,
        .items = NULL,
        .missing_items = NULL,
        .found_items_count = source->found_items_count,
        .found_items_total_price = source->found_items_total_price,
        .items_count = source->items_count,
        .missing_items_count = source->missing_items_count};

    strncpy(new_store.name, source->name, STORE_NAME_SIZE);
    strncpy(new_store.address, source->address, STORE_ADDRESS_SIZE);
    strncpy(new_store.uid, source->uid, STORE_UID_SIZE);

    if (new_store.items_count > 0)
    {

        int size_to_malloc = sizeof(store_item_s) * (source->items_count);
        new_store.items = realloc(new_store.items, size_to_malloc);
    }
    if (new_store.missing_items_count > 0)
    {
        int size_to_malloc = sizeof(basket_item_s) * (source->missing_items_count);
        new_store.missing_items = realloc(new_store.missing_items, size_to_malloc);
    }
    if (new_store.found_items_count > 0)
    {
        int size_to_malloc = sizeof(found_item_s) * (source->found_items_count);
        new_store.found_items = realloc(new_store.found_items, size_to_malloc);
    }

    for (int i = 0; i < source->items_count; i++)
    {
        new_store.items[i] = source->items[i];
    }

    for (int i = 0; i < source->found_items_count; i++)
    {
        new_store.found_items[i] = source->found_items[i];
    }

    for (int i = 0; i < source->missing_items_count; i++)
    {
        new_store.missing_items[i] = source->missing_items[i];
    }

    free_store(source);
    return new_store;
}

store_s *filter_stores(store_s *stores, int store_count, int *bargain_counter)
{
    store_s *bargains = malloc(sizeof(store_s) * store_count);

    int skipped = 0;
    for (int i = 0; i < store_count; i++)
    {
        if (stores[i].found_items_count > 0)
        {

            bargains[i - skipped] = deep_copy_store(&stores[i]);
            (*bargain_counter)++;
        }
        else
        {
            skipped++;
        }
    }

    // free(stores);
    return bargains;
}

void bargain_menu_find_bargain()
{
    store_s *stores = NULL;
    int store_count = bargain_find_bargain(&stores);

    int bargain_count = 0;
    store_s *bargains = filter_stores(stores, store_count, &bargain_count);

    // free_store_array(stores, store_count);

    char *options[bargain_count];

    char *menu_text = "Found Bargains\nStore | Address | Distance | Total price | Items found\n--------------------------------\n";

    for (int i = 0; i < bargain_count; i++)
    {

        size_t option_length = (100 + strlen(bargains[i].name) + strlen(bargains[i].address)) * sizeof(char);
        char *option = malloc(option_length);

        snprintf(option, option_length, ": %s | %s | %d m. | %lf dkk. | %d/%d\n",
                 bargains[i].name,
                 bargains[i].address,
                 bargains[i].distance,
                 bargains[i].found_items_total_price,
                 bargains[i].found_items_count,
                 bargains[i].missing_items_count + bargains[i].found_items_count);

        options[i] = option;
    }

    int selected_bargain = 10;

    while (1)
    {
        selected_bargain = display_menu(options, menu_text, "Enter a number to select a bargain.");
        if (selected_bargain == -1)
        {
            printf("Goodbye!\n");
            exit(EXIT_SUCCESS);
        }
        bargain_menu_print_bargain(bargains[selected_bargain]);
    }

    for (int i = 0; i < bargain_count; i++)
    {
        free_store(&bargains[i]);
        free(options[i]);
    }

    free(bargains);
}

int bargain_find_bargain(store_s **stores)
{

    // int store_count = fetch_get_stores(stores);
    int store_count = fetch_get_stores(stores); // TODO: Make sure everything in store structs is initialised beyond this point.
    printf("Store count: %d\n", store_count);
    stores_populate_store_items(*stores, store_count);

    qsort(*stores, store_count, sizeof(store_s), (compfn)stores_compare_stores);

    return store_count;
}

void bargain_print_bargain_result(store_s store)
{

    printf("SHOPPING LIST\n--------------------------------------\n# | Product | count | price/unit | total price\n");

    for (int i = 0; i < store.found_items_count; i++)
    {
        // TODO: Find out why store.found_items[i].price_per_unit is 0.0
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
    int bargain_string_start_length = strlen(bargain_string_start);
    char *bargain_string = malloc(strlen(bargain_string_start) * sizeof(char));
    strncat(bargain_string, bargain_string_start, bargain_string_start_length * sizeof(char));

    int found_entry_size = 0;
    int missing_entry_size = 0;

    get_size_of_list_entries(store, &found_entry_size, &missing_entry_size);

    bargain_string = realloc(bargain_string, bargain_string_start_length + found_entry_size);

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
    // bargain_print_bargain_result(store);

    int selected_option = 10;
    while (selected_option != -1)
    {
        bargain_print_bargain_result(store);
        selected_option = display_menu(&options, "", "!s saves the shopping list as a text file to the localtion specified in user settings.\n");
    }
}

void create_found_entries(store_s store, char **string_to_append_to, size_t size_of_string)
{

    for (int i = 0; i < store.found_items_count; i++)
    {
        int l = sizeof(char) * 45 + ITEM_NAME_SIZE + 1;
        char *temp_string = malloc(l);
        snprintf(temp_string, l, "# %d | %s | %d | %.2lf/%s | %.2lf dkk.\n",
                 i,
                 store.found_items[i].name,
                 store.found_items[i].count,
                 store.found_items[i].price_per_unit,
                 bargain_get_unit(store.found_items[i].unit),
                 store.found_items[i].total_price);

        strncat(*string_to_append_to, temp_string, size_of_string + l - sizeof(char));
        free(temp_string);
    }
}

void create_missing_entries(store_s store, char **string_to_append_to, size_t size_of_string)
{

    char *missing_intro = "--------------------------------------\nUNAVAILABLE ITEMS\n--------------------------------------\n# | Product\n\0";

    int string_to_append_to_size = sizeof(char) * (strlen(*string_to_append_to) + strlen(*string_to_append_to) + strlen(missing_intro) + 1);
    string_to_append_to = realloc(*string_to_append_to, string_to_append_to_size);
    strncat(*string_to_append_to, missing_intro, sizeof(char) * (strlen(*string_to_append_to) + strlen(missing_intro)));

    for (int i = 0; i < store.missing_items_count; i++)
    {

        int size_of_name = (strlen(store.missing_items[i].name) + 256) * sizeof(char);
        char *temp_string = malloc(size_of_name);
        snprintf(temp_string, size_of_name, "#%d | %s\n",
                 i,
                 store.missing_items[i].name);

        strncat(*string_to_append_to, temp_string, string_to_append_to_size + size_of_name);
        free(temp_string);
    }
}

void append_outro_to_string(store_s store, char **string_to_append_to, size_t size_of_string)
{

    char *items_outro = "--------------------------------------\nStore | Address | Distance | Items found | Total price\n\0";
    *string_to_append_to = realloc(*string_to_append_to, size_of_string + sizeof(char) * strlen(items_outro));

    int l = 35 + strlen(items_outro) * sizeof(char);
    char *temp_string = malloc(l);

    snprintf(temp_string, l, "%s%s | %s | %d | %d/%d | %.2lf\n",
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
        found_list_size += (strlen(store.found_items[i].name) + 10) * sizeof(char);
    }

    for (int i = 0; i < store.missing_items_count; i++)
    {
        missing_list_size += (strlen(store.missing_items[i].name) + 10) * sizeof(char);
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
