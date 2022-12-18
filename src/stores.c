#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "stores.h"
#include "items_types.h"
#include "items.h"
#include "api/fetch.h"

typedef int (*compfn)(const void *, const void *);

void pop_store(store_s *store) {

        // switch in case we add more chains
    switch (store->group)
    {
        case COOP:
            fetch_get_coop_items(store);
            break;
        case SALLING:
            fetch_get_salling_items(store); // TODO: Implement when function get_salling_items has been implemented.
            break;
        default:
            printf("Store[%d].group did not match any known case.\n", store->uid);
            break;
    }

    items_filter_items(store, 0);

}


void stores_populate_store_items(store_s *stores, int store_count)
{
    pthread_t threads[store_count];
    for (int n = 0; n < store_count; n++)
    {
        pthread_create(&threads[n], NULL, pop_store, &(stores[n]));
        
        // switch in case we add more chains
        //switch (stores[n].group)
        //{
        //case COOP:
        //    fetch_get_coop_items(&stores[n]);
        //    break;
        //case SALLING:
        //    fetch_get_salling_items(&(stores[n])); // TODO: Implement when function get_salling_items has been implemented.
        //    break;
        //default:
        //    printf("Store %d - Store[%d].group did not match any known case.\n", stores[n].uid, n);
        //    break;
        //}

        //items_filter_items(&stores[n], 0);
    }

    for (int i = 0; i < store_count; i++)
    {
        pthread_join(threads[i]);
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