#ifndef STORES_H
#define STORES_H
#include "items_types.h"

/**
 * Puts the items from the stores into list of shops, calls filter items
 * @returns nothing
 * 
 * @param stores list of stores to find items for
 * @param store_count amount of stores to find items for
*/
void stores_populate_store_items(store_s **stores, int store_count);

/**
 * Helping function for q sort. Evalutes 2 shops first on amount of found items, then if those are even on cheapest total price.
 * 
 * @returns 0 in case of total equality, positive number if a is better and negative if b is. 
 * @param a first store for comparison
 * @param b secound store for comparison 
*/

int stores_compare_stores(store_s* a, store_s* b);

#endif /* STORES_H */