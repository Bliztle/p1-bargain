#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#include "bargain.h"
#include "stores.h"
#include "items_types.h"
#include "menu.h"
#include "api/fetch.h"
#include "mock_functions.h"
#include "config.h"
#include "basket.h"
#include "settings.h"

#define MAX_STORES_COUNT 3
#define MAIN_MENU_ITEMS_COUNT 3
#define ASSUME_DISTANCE_DIGITS_MAX 10
#define ASSUME_ITEM_COUNT_DIGITS_MAX 4
#define ASSUME_ITEMS_FOUND_DIGITS_MAX 4
#define ASSUME_ITEMS_MISSING_DIGITS_MAX 4
#define ASSUME_TOTAL_PRICE_DIGITS_MAX 10


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
            menu_basket_edit();
            break;
        case 2:
            //menu_settings();
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

store_s *filter_stores(store_s *stores, int store_count, int *bargain_counter)
{
    store_s *bargains = malloc(sizeof(store_s) * store_count);

    int skipped = 0;
    for (int i = 0; i < store_count; i++)
    {
        if (stores[i].found_items_count > 0)
        {

            bargains[i - skipped] = stores[i - skipped];            // deep_copy_store(&stores[i]);
            (*bargain_counter)++;

        }
        else
        {
            skipped++;
        }
    }

    return bargains;
}

void bargain_menu_find_bargain()
{
    store_s *stores = NULL;
    int store_count = bargain_find_bargain(&stores);
    
    int bargain_count = 0;
    store_s *bargains = filter_stores(stores, store_count, &bargain_count);

    free(stores);

    char *options[bargain_count];

    char *menu_text = "============================================================================================================================\n                                                       Found Bargains                                                             \n============================================================================================================================\n[#] : Store                          | Address                                  | Distance | Total price  | Items found\n----------------------------------------------------------------------------------------------------------------------------";
                       
    for (int i = 0; i < bargain_count; i++)
    {

        size_t option_length = (100 + strlen(bargains[i].name) + strlen(bargains[i].address)) * sizeof(char);
        char *option = malloc(option_length);

        snprintf(option, option_length, ": %-*s | %-*s | %-*d m. | %-*.2lf dkk. | %*d/%-*d items",
                 30, bargains[i].name,
                 40, bargains[i].address,
                 5, bargains[i].distance,
                 7, bargains[i].found_items_total_price,
                 4, bargains[i].found_items_count,
                 4, bargains[i].missing_items_count + bargains[i].found_items_count);

        options[i] = option;
    }
    int selected_bargain = 10;

    while (1)
    {
        selected_bargain = display_menu(options, menu_text, "Enter a number to select a bargain.");
        if (selected_bargain == -1)
        {   
            for (int i = 0; i < bargain_count; i++)
            {
                free_store(&bargains[i]);
                free(options[i]);
            }
            free(bargains);
            printf("Goodbye!\n"); 
            exit(EXIT_SUCCESS);
        }
        bargain_menu_print_bargain(bargains[selected_bargain]);
    }
}

int bargain_find_bargain(store_s **stores)
{
    int store_count = fetch_get_stores(stores);
    printf("Store count: %d\n", store_count);
    stores_populate_store_items(*stores, store_count);

    qsort(*stores, store_count, sizeof(store_s), (compfn)stores_compare_stores);

    return store_count;
}

void bargain_print_bargain_result(store_s store)
{

    printf("|===========================================================================================================|\n");
    printf("|                                               SHOPPING LIST                                               |\n");
    printf("|===========================================================================================================|\n");
    printf("|  # | Product                                           |   count   |       price/unit       | total price |\n");
    printf("|-----------------------------------------------------------------------------------------------------------|\n");
    for (int i = 0; i < store.found_items_count; i++)
    {
        printf("| %*d | %-*s | %*d pcs. | %*.2lf dkk./%-*s | %*.2lf dkk.|\n", 3, i + 1, 49, store.found_items[i].name, 4, store.found_items[i].count, 6, store.found_items[i].price_per_unit, 10, bargain_get_unit(store.found_items[i].unit), 7, store.found_items[i].total_price);
    }
    printf("|===========================================================================================================|\n");
    printf("|                                               Missing Items                                               |\n");
    printf("|===========================================================================================================|\n");
    printf("|  # | Product                                                                                              |\n");
    printf("|-----------------------------------------------------------------------------------------------------------|\n");

    for (int i = 0; i < store.missing_items_count; i++)
    {
        printf("| %*d | %-*s |\n", 3, i + 1, 100, store.missing_items[i].name);
    }

    printf("|===========================================================================================================|\n");
    printf("|                                                   STORE                                                   |\n");
    printf("|===========================================================================================================|\n");
    printf("| Name                                                | Address                                             |\n");

    printf("| %-*s | %-*s |\n",
            51, store.name,
            51, store.address);

    printf("|-----------------------------------------------------------------------------------------------------------|\n");
    printf("|              Distance             |            Items Found            |            Total Price            |\n");

    printf("| %*d m.             | %*d/%-*d | %*.2lf dkk.            |\n",
           18, store.distance,
           16, store.found_items_count,
           16, store.missing_items_count + store.found_items_count,
           17, store.found_items_total_price);

    
    printf("|===========================================================================================================|\n");

}

void bargain_menu_print_bargain(store_s store)
{
    char *options = ": save shopping list";

    int selected_option = 10;
    while (selected_option != -1)
    {
        bargain_print_bargain_result(store);
        selected_option = display_menu(&options, "", "[1]: saves the shopping list as a text file to the localtion specified in user settings.\n");
        if (selected_option == 0) {
            printf("Exporting...\n");
            conf_settings_s settings;

            conf_read_settings(&settings);
            if (bargain_export(store, settings)) {

                printf("File exported to %s\n", settings.shopping_list_save_path);
                printf("Have a nice day!\n");
                exit(EXIT_SUCCESS);

            } else {

                printf("Failed exporting to %s\n", settings.shopping_list_save_path);
                perror("error: ");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int bargain_export(store_s store, conf_settings_s settings) {

    char filename[100];

    time_t curtime;

    time(&curtime);

    char *time = ctime(&curtime);

    time[strlen(time) - 1] = 0;

    snprintf(filename, 100, "%s%s-%s%s", settings.shopping_list_save_path, store.name, time, ".txt");

    FILE *export_file = fopen(filename, "w");

    if (export_file == NULL) {
        return 0;
    }

    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "|                                               SHOPPING LIST                                               |\n");
    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "| # | Product                                            |   count   |       price/unit       | total price |\n");
    fprintf(export_file, "|-----------------------------------------------------------------------------------------------------------|\n");

    for (int i = 0; i < store.found_items_count; i++)
    {
        fprintf(export_file, "| %d | %-*s | %*d pcs. | %*.2lf dkk./%-*s | %*.2lf dkk.|\n", i + 1, 50, store.found_items[i].name, 4, store.found_items[i].count, 6, store.found_items[i].price_per_unit, 10, bargain_get_unit(store.found_items[i].unit), 7, store.found_items[i].total_price);
    }

    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "|                                               Missing Items                                               |\n");
    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "| # | Product                                                                                               |\n");
    fprintf(export_file, "|-----------------------------------------------------------------------------------------------------------|\n");

    for (int i = 0; i < store.missing_items_count; i++)
    {
        fprintf(export_file, "| %d | %-*s |\n", i + 1, 101, store.missing_items[i].name);
    }

    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "|                                                   STORE                                                   |\n");
    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "| Name                                                | Address                                             |\n");

    fprintf(export_file, "| %-*s | %-*s |\n",
            51, store.name,
            51, store.address);

    fprintf(export_file, "|-----------------------------------------------------------------------------------------------------------|\n");
    fprintf(export_file, "|              Distance             |            Items Found            |            Total Price            |\n");

    fprintf(export_file, "| %*d m.             | %*d/%-*d | %*.2lf dkk.            |\n",
           18, store.distance,
           16, store.found_items_count,
           16, store.missing_items_count + store.found_items_count,
           17, store.found_items_total_price);

    
    fprintf(export_file, "|===========================================================================================================|\n");

    fclose(export_file);

    return 1;

}

/* 
char *bargain_get_print_bargain_string(store_s store)
{
    // realloc on a something not allocated by malloc is undefined behaviour.
    // So we need to allocate a string with the size of the initial data, and then copy it to the malloced string.
    // char *bargain_string_start = 
    // int bargain_string_start_length = strlen(bargain_string_start);
    char *bargain_string = "";

    int found_entry_size = 0;
    int missing_entry_size = 0;

    get_size_of_list_entries(store, &found_entry_size, &missing_entry_size);

    bargain_string = realloc(bargain_string, strlen(bargain_string) * sizeof(char) + found_entry_size);

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

    // Prøv med præcise array størelser, da vi kender dem fra source store_s. possible segfault løsning.

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

int get_size_of_found_line(item_name_t item_name) {

    return sizeof(char) * (
        30
        + ASSUME_ITEMS_FOUND_DIGITS_MAX
        + strlen(item_name)
        + ASSUME_ITEM_COUNT_DIGITS_MAX
        + ASSUME_TOTAL_PRICE_DIGITS_MAX
        + ASSUME_TOTAL_PRICE_DIGITS_MAX);

}

void create_found_entries(store_s store, char **string_to_append_to, size_t size_of_string)
{

    char *found_intro = "SHOPPING LIST\n======================================\n# | Product | count | price/unit | total price\n";

    int size_to_add = 0;

    size_to_add += strlen(found_intro) * sizeof(char);

    for (int i = 0; i < store.found_items_count; i++)
    {
        size_to_add += get_size_of_found_line(store.found_items[i].name);
    }

    char *temp = calloc(size_of_string + size_to_add, sizeof(char));
    snprintf(temp, size_of_string + strlen(found_intro) * sizeof(char) + 1, "%s%s", *string_to_append_to, found_intro);

    // "%s%d | %s | %d | %.2lf/%s | %.2lf dkk.\n"
    for (int i = 0; i < store.found_items_count; i++)
    {

        char *item = calloc(strlen(temp) + get_size_of_found_line(store.found_items[i].name) + 6, sizeof(char)); 

        snprintf(item, ((1 + strlen(temp)) * sizeof(char)) + get_size_of_found_line(store.found_items[i].name), "%s%d | %s | %d | %.2lf dkk./%s | %.2lf dkk.\n",
        temp,
        i + 1,
        store.found_items[i].name,
        store.found_items[i].count,
        store.found_items[i].price_per_unit,
        bargain_get_unit(store.found_items[i].unit),
        store.found_items[i].total_price);

        strncpy(temp, item, (strlen(temp) + 5) * sizeof(char) + get_size_of_found_line(store.found_items[i].name));
        free(item);
    }

    strncpy(*string_to_append_to, temp, size_of_string + size_to_add);

    free(temp);
}

void create_missing_entries(store_s store, char **string_to_append_to, size_t size_of_string)
{
    char *missing_intro = "--------------------------------------\nUNAVAILABLE ITEMS\n--------------------------------------\n# | Product\n\n";

    int size_to_add = 0;

    size_to_add += strlen(missing_intro) * sizeof(char);

    for (int i = 0; i < store.missing_items_count; i++)
    {
        size_to_add += sizeof(char) * (
        5 
        + strlen(store.missing_items[i].name)
        + ASSUME_ITEMS_MISSING_DIGITS_MAX);
    }
    
    char *temp = calloc(size_of_string + size_to_add, sizeof(char));
    snprintf(temp, size_of_string + strlen(missing_intro) * sizeof(char), "%s%s", *string_to_append_to, missing_intro);

    for (int i = 0; i < store.missing_items_count; i++)
    {
        char *item = calloc(strlen(temp) + strlen(store.missing_items[i].name) + 5, sizeof(char)); 

        snprintf(item, (strlen(temp) + 5 + strlen(store.missing_items[i].name) + ASSUME_ITEMS_MISSING_DIGITS_MAX) * sizeof(char),"%s%d | %s\n",
        temp,
        i + 1,
        store.missing_items[i].name);

        strncpy(temp, item, (strlen(temp) + strlen(store.missing_items[i].name) + 5) * sizeof(char));
        free(item);
    }

    strncpy(*string_to_append_to, temp, size_of_string + size_to_add);

    free(temp);
    
}

void append_outro_to_string(store_s store, char **string_to_append_to, size_t size_of_string)
{
    char *items_outro = "--------------------------------------\nStore | Address | Distance | Items found | Total price\n\0";

    // Add the total size of the finished string.
    int size_to_add = sizeof(char) * (
        20 
        + strlen(items_outro)
        + strlen(store.name) 
        + strlen(store.name)
        + strlen(store.name)
        + ASSUME_DISTANCE_DIGITS_MAX
        + ASSUME_ITEMS_FOUND_DIGITS_MAX 
        + (ASSUME_ITEMS_FOUND_DIGITS_MAX + ASSUME_ITEMS_MISSING_DIGITS_MAX) 
        + ASSUME_TOTAL_PRICE_DIGITS_MAX);

    // Make room for final string in destination.
    char *temp = calloc(size_of_string + size_to_add, sizeof(char));
    snprintf(temp, size_of_string + size_to_add, "%s%s%s | %s | %d | %d/%d | %.2lfdkk\n",
             *string_to_append_to,
             items_outro,
             store.name,
             store.address,
             store.distance,
             store.found_items_count,
             store.found_items_count + store.missing_items_count,
             store.found_items_total_price);

    *string_to_append_to = realloc(*string_to_append_to, size_of_string + size_to_add);
    strncpy(*string_to_append_to, temp, (size_of_string + size_to_add));

    free(temp);
    //printf(string_to_append_to);
}

void get_size_of_list_entries(store_s store, int *found_list_size, int *missing_list_size)
{

    if (*found_list_size != 0)
        *found_list_size = 0;
    if (*missing_list_size != 0)
        *missing_list_size = 0;

    for (int i = 0; i < store.found_items_count; i++)
    {
        *found_list_size += (strlen(store.found_items[i].name)) * sizeof(char);
    }

    for (int i = 0; i < store.missing_items_count; i++)
    {
        *missing_list_size += (strlen(store.missing_items[i].name)) * sizeof(char);
    }
}

*/

char *bargain_get_unit(int n)
{
    switch (n)
    {
        case 0:
        {
            return "UNKNOWN";
        }
        case 1:
        {
            return "KILOGRAMS";
        }
        case 2:
        {
            return "LITERS";
        }
        case 3:
        {
            return "UNITS";
        }
    }
    printf("ERROR IN UNIT ENUM, GOT '%d', EXPECTED '0' OR '1' OR '2' OR '3'.\n");
    exit(EXIT_FAILURE);
}
