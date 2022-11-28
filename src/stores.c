#include "stores.h"
#include "items_types.h"
#include <stdlib.h>

void stores_populate_store_items(store_s* stores, int store_count);
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
            printf("Store[%d].chain did not match any known case. Got enum: %d \n", n, store[n].chain);
            break;
        }
        //TODO remove basket from call, filter will read basket from file
        items_filter_items(basket_item_s *basket, int basket_size, store_s *store);
    }
}

int stores_compare_stores(const void* a, const void* b){

    if (a.found_items_count == b.found_items_count) {
        double a_price = 0;
        double b_price = 0;

        for (int i = 0; i < a.found_items_count; i++){
            a_price += found_items[n].total_price;
        }
        
        while (b_price <= a_price + 1){  //totally needless optimazion Isak liked
            for (int i = 0; i < b.found_items_count; i++){
                b_price += found_items[n].total_price;
            }
            break;
        } 
        
        return (a_price - b_price);
    }
    return (a.found_items_count - b.found_itemsount);
}