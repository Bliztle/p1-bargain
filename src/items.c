#include "items.h"
#include <stddef.h>
#include <math.h>

/**
 * Checks all items in list for best match, then returns the found item that best matches.
 * 
 * @returns an item with a count of 0 if no match was found, otherwise returns an item with a count needed to match requested size
 * 
 * @param requested_item item to search for
 * @param all_items_in_store list of all items to search through
*/
store_item_s items_find_best_match(store_item_s requested_item, store_s store) {
    double variance = 0.10; // TODO read from conf file
    store_item_s best_item ={
        .price_per_unit = 500000, //Has to be initialized for first comparison
        .count = 0,
        .name = requested_item.name,
        .size = requested_item.size,
        .unit = requested_item.unit
    };

    int item_found = 0;

    for (int n = 0; n < 2000; n++) { //TODO get length of list

        if (strstr(all_items_in_store[n].name, requested_item.name) != NULL &&
            all_items_in_store[n].unit == requested_item.unit &&
            all_items_in_store[n].price_per_unit < best_item.price_per_unit) {

                int count = items_is_in_variation(all_items_in_store[n].size, variance, requested_item.size);
                
                if (count > 0) {

                    store_item_s t = {
                        .name = all_items_in_store[n].name,
                        .product_price = all_items_in_store[n].product_price,
                        .price_per_unit = all_items_in_store[n].price_per_unit,
                        .size = all_items_in_store[n].size,
                        .unit = all_items_in_store[n].unit,
                        .count = count
                    };

                    best_item = t;
                    item_found = 1;

                }
        }
    }

    if (item_found) {

        store.found_items[store.found_items_count + 1] = best_item;
        store.found_items_count++;
        return item_found;
    } else {

        store.missing_items[store.missing_items_count + 1] = requested_item;
        store.missing_items_count++;
        return item_found;
    }




   return best_item;
}

/**
 * calculates the amount of items necesary to make up the requested total size within a given variance
 * 
 * @retruns the amount of items, 0 if not possible
 * 
 * @param store_item_size the size of a given item
 * @param variance acceptable percentage variance given as a number between 0 and 1
 * @param requested_size the amount to match, within the variance
*/
int items_is_in_variation(double store_item_size, double variance, double requested_size) {

    double min_size = requested_size * (1 - variance);
    double max_size = requested_size * (1 + variance);

    double min_n = ceil((min_size / store_item_size));

    if (store_item_size * min_n < max_size) return min_n;
    else return 0;
}

/**
 *  Modifies `store.items` and `store.missing` items to contain the cheapest items that match each item in `basket`
 * 
 * @returns nothing
 * 
 * @param basket list of items to look for
 * @param basket_size amount of items to look for
 * @param store the store to look for items in
 * 
*/
void items_filter_items(store_item_s* basket, int basket_size, store_s store) {

    for (int n = 0; n < basket_size; n++) {

        basket[n] = items_find_best_match(basket[n], store.items);
    }
}