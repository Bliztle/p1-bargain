
#include "api/parse.h"



// Checks all items in list for best match, then returns the found item that best matches.
store_item_s items_find_best_match(store_item_s requested_item, store_s store);

// Modifies `store.items` and `store.missing` items to contain the cheapest items that match each item in `basket`
void items_filter_items(store_item_s* basket, int basket_size, store_s store);


// if n amounts of size can give a result within variance of requested_size, then return true, otherwise false.
// TODO: maybe find a better name/function signature?
int items_is_in_variation(double store_item_size, double variance, double requested_size);

