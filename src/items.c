#include "items.h"
#include <stddef.h>
#include <math.h>
#include <string.h>
#include "test_items.h"
#include "malloc.h"       // TODO remove when basket is read from file.
#include "test_bargain.h" // TODO remove when basket is read from file.

int items_find_best_match(basket_item_s requested_item, store_s *store, found_item_s *found_destination, basket_item_s *missing_destination)
{

    double variance = 0.10; // TODO read from conf file

    // Initialising the fields that will be compared against in the  search loop, in order to prevent garbage memory
    // creating unexpected results.
    found_item_s best_item = {
        .count = 0,
        .price_per_unit = -1, // Set to -1 to differentiate first run.
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
        if (store->items[i].unit == requested_item.unit)
        {

            // If the store_item unit prise is better than the current best_item, or if it has not been set yet.
            if (store->items[i].price_per_unit < best_item.price_per_unit || best_item.price_per_unit == -1)
            {

                // Gets the amount of items needed to satisfy the requested size.
                int count = items_is_in_variation(store->items[i].size, variance, requested_item.size);

                if (count > 0)
                {

                    best_item = items_convert_to_found_item(store->items[i], count);
                    item_found = 1;
                }
            }

            // If the store_item unit is not known, then it should only be used if no other store item has a known unit that matches.
        }
        else if (store->items[i].unit == UNKNOWN && best_item.unit == UNKNOWN && store->items[1].price < best_item.product_price)
        {

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

int items_is_in_variation(double store_item_size, double variance, double requested_size)
{

    double min_size = requested_size * (1 - variance);
    double max_size = requested_size * (1 + variance);

    double min_n = ceil((min_size / store_item_size));

    if (store_item_size * min_n < max_size)
        return min_n;
    return 0;
}

void items_filter_items(store_s *store)
{
    // TODO Change to read basket from file
    basket_item_s *basket = malloc(5 * sizeof(basket_item_s));
    int basket_size = test_get_basket(basket);

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
    }

    strcpy(tmp.name, input_item.name);

    return tmp;
}

int items_compare_item_names(char *name_to_find, char *name_to_search)
{

    return strstr(name_to_search, name_to_find) != NULL;
}