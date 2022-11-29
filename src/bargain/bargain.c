#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bargain.h"
#include "../stores.h"
#include "../items_types.h"
#include "menu.h"
// #include "api/fetch.h"

#define MAX_STORES_COUNT 30

typedef int (*compfn)(const void*, const void*);

void bargain_menu_find_bargain() {

    store_s *stores = malloc(MAX_STORES_COUNT * sizeof(store_s));
    int store_count = bargain_find_bargain(stores);

    char** options = malloc(sizeof(char) * store_count);

    char* menu_text = "Found Bargains\nStore | Address | Distance | Total price | Items found\n--------------------------------\n";

    for (int i = 0; i < store_count; i++)
    {

        char* option = malloc(40 + sizeof(stores[i].name) + sizeof(stores[i].address));

        snprintf(option, sizeof(option), "[%d]: %s | %s | %d | %lf dkk. | %d/%d\n", 
        i, 
        stores[i].name, 
        stores[i].address, 
        stores[i].distance, 
        stores[i].found_items_total_price, 
        stores[i].found_items_count, 
        stores[i].missing_items_count + stores[i].found_items_count);
        
        options[i] = option;
        free(option);
    }
    
    int selected_bargain = _display_menu(options, store_count, menu_text, "Enter a number to select a bargain.");

    if (selected_bargain == -1) {
        return;
    }

    else if (selected_bargain > 0) {
        bargain_menu_print_bargain(stores[selected_bargain]);
        bargain_menu_find_bargain();
    }

}

int bargain_find_bargain(store_s* stores) {

    int store_count = 10; //fetch_get_stores(stores); TODO: Make sure everything in store structs is initialised beyond this point.
    
    stores_populate_store_items(stores, store_count);

    qsort(stores, store_count, sizeof(store_s), (compfn)stores_compare_stores);

    return store_count;
}

char* bargain_get_print_bargain_string(store_s store) {

    char* bargain_string = "SHOPPING LIST\n--------------------------------------\n# | Product | count | price/unit | total price";

    int found_entry_size = 0;
    int missing_entry_size = 0;

    get_size_of_list_entries(store, &found_entry_size, &missing_entry_size);

    bargain_string = realloc(bargain_string, sizeof(bargain_string) + found_entry_size);

    create_found_entries(store, &bargain_string);
    
    if (store.missing_items_count < 0) {

        create_missing_entries(store, &bargain_string);

    }

    append_outro_to_string(store, &bargain_string);

    char* return_string = bargain_string;
    free(bargain_string);
    return return_string;

}

void bargain_menu_print_bargain(store_s store) {

    char* options = "save shopping list: !s";

    char* bargain_string = bargain_get_print_bargain_string(store);

    int n = _display_menu(&options, 1, bargain_string, "!s saves the shopping list as a text file to the localtion specified in user settings.\n" );
    if (n == -1) {
        return;
    }

    else if (n > 0) {
        // export_bargain(); //TODO Impliment
        bargain_menu_print_bargain(store); // Stay in menu after export.
    }
    
}

void create_found_entries(store_s store, char** string_to_append_to) {

    for (int i = 0; i < store.found_items_count; i++) {

        char* temp_string = malloc(sizeof(char) * 45 + ITEM_NAME_SIZE + 1);
        snprintf(temp_string, sizeof(temp_string), "# %d | %s | %d | %.2lf/%s | %.2lf dkk.\n", 
        i, 
        store.found_items[i].name, 
        store.found_items[i].count, 
        store.found_items[i].price_per_unit,
        bargain_get_unit(store.found_items[i].unit), 
        store.found_items[i].total_price);

        strncat(*string_to_append_to, temp_string, sizeof(*string_to_append_to) + sizeof(temp_string) - sizeof(char));
        free(temp_string);
    }
}

void create_missing_entries(store_s store, char** string_to_append_to) {

    char* missing_intro = "--------------------------------------\nUNAVAILABLE ITEMS\n--------------------------------------\n# | Product\n'\0'";


    string_to_append_to = realloc(*string_to_append_to, sizeof(*string_to_append_to) + sizeof(*string_to_append_to) + sizeof(missing_intro));
    strncat(*string_to_append_to, missing_intro, sizeof(*string_to_append_to) + sizeof(missing_intro) - sizeof(char));

    for (int i = 0; i < store.missing_items_count ; i++) {

        int size_of_name = sizeof(store.missing_items[i].name);
        char* temp_string = malloc(sizeof(char) * (7 + size_of_name));
        snprintf(temp_string, sizeof(temp_string), "#%d | %s\n", 
        i, 
        store.missing_items[i].name);

        strncat(*string_to_append_to, temp_string, sizeof(*string_to_append_to) + (sizeof(temp_string) - sizeof(char)));
        free(temp_string);

    }

}

void append_outro_to_string(store_s store, char** string_to_append_to) {

    char* items_outro = "--------------------------------------\nStore | Address | Distance | Items found | Total price\n";
    char* temp_string = malloc(sizeof(char) * 31);

    snprintf(temp_string, sizeof(temp_string), "%s | %s | %d | %d/%d | %.2lf\n", 
        store.name, 
        store.address, 
        store.distance,
        store.found_items_count, 
        store.found_items_count + store.missing_items_count,
        store.found_items_total_price);

    string_to_append_to = realloc(items_outro, sizeof(items_outro) + (sizeof(char) * (sizeof(store.name) + sizeof(store.address) + (sizeof(temp_string)))));

    strncat(*string_to_append_to, temp_string, sizeof(*string_to_append_to) + sizeof(temp_string) - sizeof(char));

    free(temp_string);


}

void get_size_of_list_entries(store_s store, int *found_list_size, int *missing_list_size) {

    if (found_list_size != 0) found_list_size = 0;
    if (missing_list_size != 0) missing_list_size = 0;

    for (int i = 0; i < store.found_items_count; i++)
    {
        found_list_size += sizeof(store.found_items[i].name) + sizeof(char) * 46;
    }
    
    for (int i = 0; i < store.missing_items_count; i++)
    {
        missing_list_size += sizeof(store.missing_items[i].name) + sizeof(char) * 6;
    }
}

char* bargain_get_unit(int n) { // TODO make this better.

    switch (n) {
        case 0: {
            return "KILOGRAMS";
        }
        case 1: {
            return "LITERS";
        }
        case 2: {
            return "UNITS";
        }
    }
    return "Error"; // lol
}
