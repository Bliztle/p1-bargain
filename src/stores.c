#include <stdlib.h>
#include <stdio.h>

#include <stdlib.h>

#include "stores.h"
#include "items_types.h"
#include "items.h"
#include "api/fetch.h"

typedef int (*compfn)(const void *, const void *);

void stores_populate_store_items(store_s *stores, int store_count)
{
    printf("Searching %d stores for bargains, please wait...\n", store_count);
    for (int n = 0; n < store_count; n++)
    {
        // switch in case we add more chains
        switch (stores[n].group)
        {
        case COOP:
            fetch_get_coop_items(&stores[n]);
            break;
        case SALLING:
            fetch_get_salling_items(&(stores[n])); // TODO: Implement when function get_salling_items has been implemented.
            break;
        default:
            printf("Store %d - Store[%d].group did not match any known case.\n", stores[n].uid, n);
            break;
        }

        items_filter_items(&stores[n], 0);
        printf("%d/%d\n", n, store_count);
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

int stores_compare_distance(store_s *a, store_s *b) {

    return (a->distance - b->distance);

}