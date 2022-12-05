#include "items.h"
#include <stddef.h>
#include <math.h>
#include <string.h>
#include "test_items.h"
#include "malloc.h" // TODO remove when basket is read from file.
#include "test_bargain.h"
#include "bargain.h"
#include "api/parse.h"

// TODO: Remove when basked read from file is implemented.
basket_item_s *test_get_basket(int *basket_item_counter)
{

    basket_item_s *basket = malloc(5 * sizeof(basket_item_s));
    const basket_item_s item1 = {
        .name = "Milk\0",
        .size = 1,
        .unit = LITERS,
    };

    const basket_item_s item2 = {
        .name = "Beef\0",
        .size = 2,
        .unit = KILOGRAMS,
    };

    const basket_item_s item3 = {
        .name = "Noodles\0",
        .size = 3,
        .unit = UNITS,
    };

    const basket_item_s item4 = {
        .name = "Snickers\0",
        .size = 4,
        .unit = UNITS,
    };

    const basket_item_s item5 = {
        .name = "Pudding\0",
        .size = 5,
        .unit = UNITS,
    };

    basket[0] = item1;
    basket[1] = item2;
    basket[2] = item3;
    basket[3] = item4;
    basket[4] = item5;

    *basket_item_counter = 5;
    return basket;
}

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
        if ((requested_item.unit != UNKNOWN) && (store->items[i].unit == requested_item.unit))
        {

            // If the store_item unit prise is better than the current best_item, or if it has not been set yet.
            if (store->items[i].price_per_unit < best_item.price_per_unit || best_item.price_per_unit == 50000000)
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
        else if ((requested_item.unit == UNKNOWN) || (best_item.unit == UNKNOWN && store->items[i].price < best_item.product_price))
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
    int basket_size = 10;
    // basket_item_s *basket = __fetch_mock_basket(); // test_get_basket(&basket_size);

    basket_item_s *basket = malloc(basket_size * sizeof(basket_item_s));

    strncpy(basket[0].name, "Skåvl", ITEM_NAME_SIZE);
    basket[0].size = 1;
    basket[0].unit = UNKNOWN;

    strncpy(basket[1].name, "Spade", ITEM_NAME_SIZE);
    basket[1].size = 1;
    basket[1].unit = UNKNOWN;

    strncpy(basket[2].name, "Spegepølse", ITEM_NAME_SIZE);
    basket[2].size = 1;
    basket[2].unit = UNKNOWN;

    strncpy(basket[3].name, "Sild", ITEM_NAME_SIZE);
    basket[3].size = 1;
    basket[3].unit = UNKNOWN;

    strncpy(basket[4].name, "Brød", ITEM_NAME_SIZE);
    basket[4].size = 1;
    basket[4].unit = UNKNOWN;

    strncpy(basket[5].name, "Lys", ITEM_NAME_SIZE);
    basket[5].size = 1;
    basket[5].unit = UNKNOWN;

    strncpy(basket[6].name, "Mad", ITEM_NAME_SIZE);
    basket[6].size = 1;
    basket[6].unit = UNKNOWN;

    strncpy(basket[7].name, "Æg", ITEM_NAME_SIZE);
    basket[7].size = 1;
    basket[7].unit = UNKNOWN;

    strncpy(basket[8].name, "Mælk", ITEM_NAME_SIZE);
    basket[8].size = 1;
    basket[8].unit = UNKNOWN;

    strncpy(basket[9].name, "Redskab", ITEM_NAME_SIZE);
    basket[9].size = 1;
    basket[9].unit = UNKNOWN;

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
    int buffer_length = strlen(name_to_find) + 3;
    char buffer[buffer_length];

    snprintf(buffer, buffer_length, "(%s)", name_to_find);

    return parse_try_regex_group(name_to_search, buffer) != NULL;
}
