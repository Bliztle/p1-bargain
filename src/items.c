#include "items.h"
#include <stddef.h>
#include <math.h>
#include <string.h>
#include "test_items.h"


int items_find_best_match(basket_item_s requested_item, store_s *store, found_item_s *found_destination, basket_item_s *missing_destination) {

    double variance = 0.10; // TODO read from conf file


    // Initialising the fields that will be compared against in the  search loop, in order to prevent garbage memory
    // creating unexpected results.
    found_item_s best_item = {
        .price_per_unit = -1, // Set to -1 to differentiate first run.
        .count = 0,
    };

    int item_found = 0;

    for (int n = 0; n < store->items_count; n++) {

        if (items_compare_item_names(requested_item.name, store->items[n].name)
        && (store->items[n].unit == requested_item.unit)
        && (store->items[n].price_per_unit < best_item.price_per_unit || best_item.price_per_unit == - 1)) {

            int count = items_is_in_variation(store->items[n].size, variance, requested_item.size);
                
            if (count > 0) {

                best_item = items_convert_to_found_item(store->items[n], count);
                item_found = 1;

            }
        }
    }

    if (item_found) {

        *found_destination = best_item;

        return item_found;

    } else {

        *missing_destination = requested_item;

        return item_found;

    }
}

int items_is_in_variation(double store_item_size, double variance, double requested_size) {

    double min_size = requested_size * (1 - variance);
    double max_size = requested_size * (1 + variance);

    double min_n = ceil((min_size / store_item_size));

    if (store_item_size * min_n < max_size) return min_n;
    else return 0;
}

void items_filter_items(basket_item_s *basket, int basket_size, store_s *store) {

    for (int n = 0; n < basket_size; n++) {

        found_item_s found_item;
        basket_item_s missing_item;

        if (items_find_best_match(basket[n], store, &found_item, &missing_item)) {

            items_add_item_to_found(found_item, store);

        } else {

            items_add_item_to_missing(missing_item, store);

        }
    }
}

void items_add_item_to_found(found_item_s item, store_s *store) {

    store->found_items[store->found_items_count] = item;
    store->found_items_count++;

}

void items_add_item_to_missing(basket_item_s item, store_s *store) {

    store->missing_items[store->missing_items_count] = item;
    store->missing_items_count++;

}

found_item_s items_convert_to_found_item(store_item_s input_item, int item_count) {

    found_item_s t = {
            .product_price = input_item.price,
            .count = item_count,
            .size = input_item.size,
            .unit = input_item.unit,
            .price_per_unit = input_item.price_per_unit
    };

    t.total_price =  t.count * t.product_price;
    strcpy(t.name, input_item.name);

    return t;

}

int items_compare_item_names(char* name_to_find, char* name_to_search) {

    if (strstr(name_to_search, name_to_find) != NULL) {
        return 1;
    } else {
        return 0;
    }

}