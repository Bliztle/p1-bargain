#include "stores.h"
#include "items_types.h"
#include "menu.h"


int find_bargain(store_s* stores) {

    int store_count = fetch_get_stores(stores);
    
    stores_populate_store_items(stores, store_count);
    
    qsort(stores, store_count, sizeof(store_s), stores_compare_stores);

    return store_count;
}

void print_bargain(store_s store) {
    printf("SHOPPING LIST\n");
    printf("--------------------------------------\n");
    printf("# | Product | count | price/unit | total price")

    for (int i = 0; i < found_items_count; i++) {
        printf("# %##d | %s | %##d | %###.2lf/%s | %###.2lf\n", 
        n, 
        store.found_items[i].name, 
        store.found_items[i].count, 
        store.found_items[i].price_per_unit,
        get_unit(store.found_items[i].unit); 
        store.found_items[i].total_price);
    }
    
   if (store.missing_items_count < 0) {
        printf("--------------------------------------\n");
        printf("UNAVAILABLE ITEMS");
        printf("--------------------------------------\n");
        printf("# | Product")

        for (int i = 0; i < store.missing_items_count ; i++) {
            printf("#%##d | %s\n", n, store.missing_items[i].name);
        }
   }

    printf("--------------------------------------\n");
    printf("Store | Address | Distance | Items found | Total price\n");
    printf("%s | %s | %##.2lf | %###d\% | %#####.2lf\n", 
        store.name, 
        store.address, 
        store.distance, 
        get_percentage(store.items_found_count, (store.items_missing_count)));
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

    char** options = { "save shopping list: !s"}
    print_bargain(store);

    int n = print_menu(options, 1, "", "!s saves the shopping list as a text file to the localtion specified in user settings.\n" );
    if (n == -1) {
        return;
    }

    else if (n > 0) {
        export_bargain(); //TODO Impliment
    }
}