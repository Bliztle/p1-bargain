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
            menu_settings();
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

            bargains[i - skipped] = stores[i]; // deep_copy_store(&stores[i]);
            (*bargain_counter)++;
        }
        else
        {
            skipped++;
        }
    }

    return bargains;
}

int bargain_check_string_length(char *input, int target) {

    char dest[100];
    memset(dest, 0, sizeof(dest));
    strcpy(dest, input);
    
    int amount = 0;

    for (int i = 0; i < 100; i++)
    {
        if (((int) dest[i]) > 127) { // 127 is the largest ascii character included in the ASCII printable characters category - https://www.ascii-code.com/
            amount++;
        }
    }
    // 'æ' || (int) input[index] == 198 || (int) input[index] == 'ø' || (int) input[index] == 'Ø' || (int) input[index] == 'å' || (int) input[index] == 'Å'

    return target + (amount / 2);

}

void bargain_menu_find_bargain()
{
    store_s *stores = NULL;
    int store_count = bargain_find_bargain(&stores);

    int bargain_count = 0;
    store_s *bargains = filter_stores(stores, store_count, &bargain_count);

    free(stores);

    char *options[bargain_count];

    char *menu_text = "\n|======================================================================================================================================================================|\n|                                                                            Found Bargains                                                                            |\n|======================================================================================================================================================================|\n| [##] : Store                                                   | Address                                                |    Total price    | Items Found | Distance |\n|----------------------------------------------------------------------------------------------------------------------------------------------------------------------|";
       
    for (int i = bargain_count - 1; i >= 0; i--)
    {
        size_t option_length = 190;
        char *option = malloc(option_length);

        snprintf(option, option_length, ": %-*s | %-*s |   %*.2lf dkk.   | %*d/%-*d | %*d m.",
                 bargain_check_string_length(bargains[i].name, 55), bargains[i].name,
                 bargain_check_string_length(bargains[i].address, 54), bargains[i].address,
                 8, bargains[i].found_items_total_price,
                 5, bargains[i].found_items_count,
                 5, bargains[i].missing_items_count + bargains[i].found_items_count,
                 5, bargains[i].distance);

        options[bargain_count - i - 1] = option;
    }
    int selected_bargain = 10;

    while (1)
    {
        selected_bargain = display_bargain_menu(options, menu_text, "Enter a number to select a bargain.");
        if (selected_bargain == -1)
        {
            for (int i = 0; i < bargain_count; i++)
            {
                free_store(&bargains[i]);
                free(options[i]);
            }
            free(bargains);
            return;
        }
        bargain_menu_print_bargain(bargains[selected_bargain]);
    }
}

int bargain_find_bargain(store_s **stores)
{
    int store_count = fetch_get_stores(stores);
    qsort(*stores, store_count, sizeof(store_s), (compfn)stores_compare_distance);
    stores_populate_store_items(*stores, store_count);
    qsort(*stores, store_count, sizeof(store_s), (compfn)stores_compare_stores);

    return store_count;
}

void bargain_print_bargain_result(store_s store) {

    printf("\n|============================================================================================================|\n");
    printf("|                                                SHOPPING LIST                                               |\n");
    printf("|============================================================================================================|\n");
    printf("|   # | Product                                           |   Count   |       Price/Unit       | Total Price |\n");
    printf("|------------------------------------------------------------------------------------------------------------|\n");

    for (int i = 0; i < store.found_items_count; i++)
    {
        printf("| %*d | %-*s  | %*d pcs. | %*.2lf dkk./%-*s | %*.2lf dkk.|\n",
               3, i + 1,
               bargain_check_string_length(store.found_items[i].name, 48), store.found_items[i].name,
               4, store.found_items[i].count,
               6, store.found_items[i].price_per_unit,
               10, bargain_get_unit(store.found_items[i].unit),
               7, store.found_items[i].total_price);
    }

    printf("|============================================================================================================|\n");
    printf("|                                                Missing Items                                               |\n");
    printf("|============================================================================================================|\n");
    printf("|   # | Product                                                                                              |\n");
    printf("|------------------------------------------------------------------------------------------------------------|\n");

    for (int i = 0; i < store.missing_items_count; i++)
    {
        printf("| %*d | %-*s |\n",
               3, i + 1,
               bargain_check_string_length(store.missing_items[i].name, 100), store.missing_items[i].name);
    }

    printf("|============================================================================================================|\n");
    printf("|                                                    STORE                                                   |\n");
    printf("|============================================================================================================|\n");
    printf("| Name                                                 | Address                                             |\n");

    printf("| %-*s | %-*s |\n",
           bargain_check_string_length(store.name, 52), store.name,
           bargain_check_string_length(store.address, 51), store.address);

    printf("|------------------------------------------------------------------------------------------------------------|\n");
    printf("|              Distance              |            Items Found            |            Total Price            |\n");

    printf("| %*d m.              | %*d/%-*d | %*.2lf dkk.            |\n",
           18, store.distance,
           16, store.found_items_count,
           16, store.missing_items_count + store.found_items_count,
           17, store.found_items_total_price);

    printf("|============================================================================================================|\n");
}

void bargain_menu_print_bargain(store_s store)
{
    char *options = ": save shopping list";

    int selected_option = 10;
    while (selected_option != -1)
    {
        bargain_print_bargain_result(store);
        selected_option = display_menu(&options, "", "[1]: saves the shopping list as a text file to the localtion specified in user settings.\n");
        if (selected_option == 0)
        {
            printf("Exporting...\n");
            conf_settings_s settings;

            conf_read_settings(&settings);
            if (bargain_export(store, settings))
            {
                char s[MAX_INPUT_SIZE];
                printf("Go back with !q, or exit with !e\n");
                
                while (1) {

                    scanf("%s", &s);

                    if (strcmp(s, "!e") == 0) {
                        printf("Have a nice day!\n");
                        exit(EXIT_SUCCESS);
                    }
                    else if (strcmp(s, "!q") == 0) {
                        return;
                    }
                    else {
                        printf("Please enter a valid option.\n");
                    }
                }
            }
            else
            {

                printf("Failed exporting to %s\n", settings.shopping_list_save_path);
                perror("error: ");
                exit(EXIT_FAILURE);
            }
        }
    }
}


int bargain_export(store_s store, conf_settings_s settings) {

    char filename[310];

    time_t curtime;

    time(&curtime);

    char *time = ctime(&curtime);

    time[strlen(time) - 1] = 0;

    snprintf(filename, 310, "%s%s-%s%s", settings.shopping_list_save_path, store.name, time, ".txt");

    parse_replace_char(filename, ' ', '_');
    parse_replace_char(filename, ':', '-');
    parse_replace_char(filename, ',', '-');

    FILE *export_file = fopen(filename, "w"); 

    if (export_file == NULL)
    {
        return 0;
    }

    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "|                                               SHOPPING LIST                                               |\n");
    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "| # | Product                                            |   count   |       price/unit       | total price |\n");
    fprintf(export_file, "|-----------------------------------------------------------------------------------------------------------|\n");

    for (int i = 0; i < store.found_items_count; i++)
    {
        fprintf(export_file, "| %d | %-*s | %*d pcs. | %*.2lf dkk./%-*s | %*.2lf dkk.|\n", 
                              i + 1, 
                              bargain_check_string_length(store.found_items[i].name, 50), store.found_items[i].name, 
                              4, store.found_items[i].count, 
                              6, store.found_items[i].price_per_unit, 
                              10, bargain_get_unit(store.found_items[i].unit), 
                              7, store.found_items[i].total_price);
    }

    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "|                                               Missing Items                                               |\n");
    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "| # | Product                                                                                               |\n");
    fprintf(export_file, "|-----------------------------------------------------------------------------------------------------------|\n");

    for (int i = 0; i < store.missing_items_count; i++)
    {
        fprintf(export_file, "| %d | %-*s |\n", 
                              i + 1, 
                              bargain_check_string_length(store.missing_items[i].name, 101), store.missing_items[i].name);
    }

    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "|                                                   STORE                                                   |\n");
    fprintf(export_file, "|===========================================================================================================|\n");
    fprintf(export_file, "| Name                                                | Address                                             |\n");

    fprintf(export_file, "| %-*s | %-*s |\n",
                          bargain_check_string_length(store.name, 51), store.name,
                          bargain_check_string_length(store.address, 51), store.address);

    fprintf(export_file, "|-----------------------------------------------------------------------------------------------------------|\n");
    fprintf(export_file, "|              Distance             |            Items Found            |            Total Price            |\n");

    fprintf(export_file, "| %*d m.             | %*d/%-*d | %*.2lf dkk.            |\n",
                          18, store.distance,
                          16, store.found_items_count,
                          16, store.missing_items_count + store.found_items_count,
                          17, store.found_items_total_price);

    
    fprintf(export_file, "|===========================================================================================================|\n");

    fclose(export_file);
    
    printf("File exported to %s\n", filename);

    return 1;
}

char *bargain_get_unit(int n)
{
    switch (n)
    {
    case 1:
        return "KILOGRAMS";
    case 2:
        return "LITERS";
    case 3:
        return "UNITS";
    default:
        return "UNKNOWN";
    }
}