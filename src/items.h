
#include "api/parse.h"



// Checks all items in list for best match, then returns the found item that best matches.
basket_item_s find_best_match(basket_item_s requested_item, item_s *all_items_in_store);

// Modifies `store.items` and `store.missing` items to contain the cheapest items that match each item in `basket`
void filter_items(basket_item_s* basket, int basket_size, store_s store);


// if n amounts of size can give a result within variance of requested_size, then return true, otherwise false.
// TODO: maybe find a better name/function signature?
int is_in_variation(double store_item_size, double variance, double requested_size);

