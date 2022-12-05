#include <stdlib.h>
#include <stdio.h>

#include "stores.h"
#include "items_types.h"
#include "items.h"
#include "api/fetch.h"
#include "test_bargain.h"

void stores_populate_store_items(store_s *stores, int store_count)
{

    for (int n = 0; n < store_count; n++)
    {
        // switch in case we add more chains
        switch (stores[n].chain)
        {
        case COOP:
            test_fetch_get_store_items(&stores[n], n);
            // fetch_get_coop_items(&stores[n]); // TODO: Implement when function get_coop_items has been implemented.
            break;
        case SALLING:
            // test_fetch_get_store_items(&stores[n], n);
            fetch_get_salling_items(&(stores[n])); // TODO: Implement when function get_salling_items has been implemented.
            break;
        default:
            // TODO make able to print any type after got enum.
            printf("Store[%d].chain did not match any known case. Got enum: %d \n", n, stores[n].chain);
            break;
        }

        items_filter_items(&stores[n]);
    }
}

int stores_compare_stores(store_s *a, store_s *b)
{

    if (a->found_items_count == b->found_items_count)
    {

        return (a->found_items_total_price - b->found_items_total_price);
    }

    return (a->found_items_count - b->found_items_count);
}