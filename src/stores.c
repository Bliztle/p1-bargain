#include "stores.h"
#include "items_types.h"
#include <stdlib.h>

void stores_populate_store_items(store_s* stores, int store_count) {
    for (int n = 0; n < store_count; n++) {
        //switch in case we add more chains
        switch (stores[n].chain)
        {
        case COOP:
            get_coop_items();
            break;
        case SALLING:
            get_salling_items();
            break;
        default:
            //TODO make able to print any type after got enum.
            printf("Store[%d].chain did not match any known case. Got enum: %d \n", n, stores[n].chain);
            break;
        }
        //TODO remove basket from call, filter will read basket from file
        items_filter_items(stores[n]);
    }
}

int stores_compare_stores(store_s a, store_s b) {

    if (a.found_items_count == b.found_items_count) {
        
        return (a.found_items_total_price - b.found_items_total_price);

    }
    
    return (a.found_items_count - b.found_items_count);
}