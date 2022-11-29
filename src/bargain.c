#include "stores.h"
#include "items_types.h"
#include "menu.h"
#include <strings.h>

#define MAX_STORES_COUNT 30

int find_bargain(store_s* stores) {

    int store_count = fetch_get_stores(stores); // TODO: Make sure everything in store structs are initialised beyond this point.
    
    stores_populate_store_items(stores, store_count);
    
    qsort(stores, store_count, sizeof(store_s), stores_compare_stores);

    return store_count;
}

char** get_print_bargain_string(store_s store) {

    char* bargain_string = "SHOPPING LIST\n--------------------------------------\n# | Product | count | price/unit | total price";
    
    int entry_size = 0;
    for (int i = 0; i < store.found_items_count; i++)
    {
        entry_size += sizeof(store.found_items[i].name) + sizeof(char) * 46;
    }

    char* bargain_string = realloc(bargain_string, sizeof(bargain_string) + entry_size);

    for (int i = 0; i < store.found_items_count; i++) {

        char* temp_string = malloc(sizeof(char) * 45 + ITEM_NAME_SIZE + 1);
        snprintf(temp_string, "# %###d | %s | %##d | %###.2lf/%s | %###.2lf dkk.\n", 
        i, 
        store.found_items[i].name, 
        store.found_items[i].count, 
        store.found_items[i].price_per_unit,
        get_unit(store.found_items[i].unit), 
        store.found_items[i].total_price);

        strncat(bargain_string, temp_string, sizeof(temp_string) - sizeof(char));
    }
    
   if (store.missing_items_count < 0) {

        int missing_entry_size = 0;
        for (int i = 0; i < store.missing_items_count; i++)
        {
            missing_entry_size += sizeof(store.missing_items[i].name) + sizeof(char) * 6;
        }

        char* missing_intro = "--------------------------------------\nUNAVAILABLE ITEMS\n--------------------------------------\n# | Product\n'\0'";

        realloc(bargain_string, sizeof(bargain_string) + (missing_entry_size) + sizeof(missing_intro));
        strncat(bargain_string, remove_null_terminator(missing_intro), sizeof(missing_intro) - sizeof(char));

        for (int i = 0; i < store.missing_items_count ; i++) {

            int name_char_count = count_chars_in_string(store.missing_items[i].name);
            char* temp_string = malloc(sizeof(char) * (7 + name_char_count));
            snprintf(temp_string, "#%##d | %s\n", 
            i, 
            store.missing_items[i].name);

            strncat(bargain_string, temp_string, (sizeof(temp_string) - sizeof(char)));
            free(temp_string);

        }
   }

    char* items_outro = "--------------------------------------\nStore | Address | Distance | Items found | Total price\n";
    char* temp_string = malloc(sizeof(char) * 31);

    snprintf(temp_string, "%s | %s | %##.2lf | %###d/%###d | %#####.2lf\n", 
        store.name, 
        store.address, 
        store.distance,
        store.found_items_count, 
        store.found_items_count + store.missing_items_count,
        store.found_items_total_price);

    realloc(items_outro, sizeof(items_outro) + (sizeof(char) * (sizeof(store.name) + sizeof(store.address) + (sizeof(temp_string)))));

    strncat(bargain_string, temp_string, sizeof(temp_string) - sizeof(char));

    free(temp_string);

    return &bargain_string;

}

char* get_unit(int n) {

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
}

int get_percentage(int items_found, int items_missing) {


    return ((items_found + items_missing) / 100) * items_found;
}

void menu_print_bargain(store_s store) {

    char** options = { "save shopping list: !s"};

    char** bargain_string_ptr = print_bargain(store);

    int n = print_menu(options, 1, *bargain_string_ptr, "!s saves the shopping list as a text file to the localtion specified in user settings.\n" );
    if (n == -1) {
        free(bargain_string_ptr);
        return;
    }

    else if (n > 0) {
        export_bargain(); //TODO Impliment
        free(bargain_string_ptr);
        menu_print_bargain(store); // Stay in menu after export.
    }
    
}

void menu_find_bargain() {

    store_s *stores = malloc(MAX_STORES_COUNT * sizeof(store_s));
    int store_count = find_bargain(stores);

    char** options = malloc(sizeof(char) * store_count);

    char* menu_text = "Found Bargains\nStore | Address | Distance | Total price | Items found\n--------------------------------\n";

    for (int i = 0; i < store_count; i++)
    {
        options[i] = ("[%d]: %s | %s | %d | %lf dkk. | %d/%d\n", 
        i, 
        stores[i].name, 
        stores[i].address, 
        stores[i].distance, 
        stores[i].found_items_total_price, 
        stores[i].found_items_count, 
        stores[i].missing_items_count + stores[i].found_items_count);
    }
    
    int selected_bargain = print_menu(options, store_count, menu_text, "Enter a number to select a bargain.");

    if (selected_bargain == -1) {
        return;
    }

    else if (selected_bargain > 0) {
        menu_print_bargain(stores[selected_bargain]);
        menu_find_bargain();
    }

}