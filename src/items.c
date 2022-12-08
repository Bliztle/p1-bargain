#include "items.h"
#include <stddef.h>
#include <math.h>
#include <string.h>
#include "malloc.h" // TODO remove when basket is read from file.
#include "test_bargain.h"
#include "bargain.h"
#include "api/parse.h"
#include "mock_functions.h" // TODO: Remove when basket read from file is implemented.

int items_find_best_match(basket_item_s requested_item, store_s *store, found_item_s *found_destination, basket_item_s *missing_destination)
{

    double variance = 0.10; // TODO read from conf file

    // Initialising the fields that will be compared against in the  search loop, in order to prevent garbage memory
    // creating unexpected results.
    found_item_s best_item = {
        .count = 0,
        .price_per_unit = 50000000, // Set to -1 to differentiate first run.
        .product_price = 50000000,
        .unit = UNKNOWN,
    };

    int item_found = 0;
    for (int i = 0; i < store->items_count; i++)
    {

        // The goal of this loop is to find the cheapest match for the requested item.
        // The cheapest match is the item with the lowest price per unit, that is within the variance of the requested size.

        // If the itemname contains the requested item name.
        if (!(items_compare_item_names(requested_item.name, store->items[i].name)))
            continue;

        // If the store_item unit is known.
        if (items_compare_item_units(requested_item.unit, store->items[i].unit)
            && items_compare_item_price_per_unit(best_item.price_per_unit, store->items[i].price_per_unit)) {

                // Gets the amount of items needed to satisfy the requested size.
                int count = items_is_in_variation(store->items[i].size, variance, requested_item.size);

                if (count > 0) {
                    best_item = items_convert_to_found_item(store->items[i], count);
                    item_found = 1;
                }
        }
        // If the store_item unit is not known, then it should only be used if no other store item has a known unit that matches.
        else if (items_alternate_unit_match(requested_item.unit, best_item.unit, store->items[i].price, best_item.product_price)) {
            best_item = items_convert_to_found_item(store->items[i], 1);
            item_found = 1;
        }

    }

    if (item_found)
    {

        *found_destination = best_item;

        return item_found;
    }

    *missing_destination = requested_item;

    return item_found;
}

void items_filter_items(store_s *store)
{
    // basket_item_s *basket = __fetch_mock_basket(); // test_get_basket(&basket_size);

    // TODO Change to read basket from file
    int basket_size = 0;
    basket_item_s *basket = test_get_basket(&basket_size);

    store->found_items = malloc(sizeof(found_item_s) * store->items_count);
    store->missing_items = malloc(sizeof(basket_item_s) * store->items_count);

    for (int i = 0; i < basket_size; i++)
    {

        found_item_s found_item;
        basket_item_s missing_item;

        if (items_find_best_match(basket[i], store, &found_item, &missing_item))
        {

            items_add_item_to_found(found_item, store);
        }
        else
        {

            items_add_item_to_missing(missing_item, store);
        }
    }
}

int items_is_in_variation(double store_item_size, double variance, double requested_size)
{
    // Check if exact match is possible - note; ((requested_size / store_item_size) % 1 == 0) does not work :(
    if (ceil(requested_size / store_item_size) == floor(requested_size / store_item_size)) {
        return (requested_size / store_item_size);
    }

    double min_size = requested_size * (1 - variance);
    double max_size = requested_size * (1 + variance);

    double min_n = ceil((min_size / store_item_size));

    if (store_item_size * min_n <= max_size)

        return min_n;
    return 0;
}

void items_add_item_to_found(found_item_s item, store_s *store)
{

    store->found_items[store->found_items_count] = item;
    store->found_items_count++;
    store->found_items_total_price += item.total_price;
}

void items_add_item_to_missing(basket_item_s item, store_s *store)
{

    store->missing_items[store->missing_items_count] = item;
    store->missing_items_count++;
}

found_item_s items_convert_to_found_item(store_item_s input_item, int item_count)
{

    found_item_s tmp = {
        .product_price = input_item.price,
        .size = input_item.size,
        .unit = input_item.unit,
    };

    if (input_item.unit == UNKNOWN)
    {
        tmp.count = 1;
        tmp.total_price = input_item.price;
    }
    else
    {
        tmp.count = item_count;
        tmp.total_price = input_item.price * item_count;
        tmp.price_per_unit = tmp.size / tmp.product_price;
    }

    strcpy(tmp.name, input_item.name);

    return tmp;
}

int items_compare_item_names(char *name_to_find, char *name_to_search)
{
    return strstr(name_to_search, name_to_find) != NULL;
}


int items_compare_item_units(item_unit_e requested_unit, item_unit_e store_unit){
    return ((requested_unit != UNKNOWN) && (store_unit == requested_unit));
}

int items_compare_item_price_per_unit(item_price_t best_item_price, item_price_t store_price){

    return (store_price < best_item_price || best_item_price == 50000000);
}

int items_alternate_unit_match(item_unit_e requested_unit, item_unit_e best_item_unit, item_price_t store_price, item_price_t best_item_price){

        return (requested_unit == UNKNOWN) || (best_item_unit == UNKNOWN && store_price < best_item_price);
}