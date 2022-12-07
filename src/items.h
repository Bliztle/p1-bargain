#ifndef ITEMS_H
#define ITEMS_H

#include "items_types.h"

/**
 * Checks all items in list for best match, then returns the found item that best matches.
 *
 * @returns an item with a count of 0 if no match was found, otherwise returns an item with a count needed to match requested size
 *
 * @param requested_item item to search for
 * @param all_items_in_store list of all items to search through
 * @param destination address of found item, if any.
 */
int items_find_best_match(basket_item_s requested_item, store_s *store, found_item_s *found_destination, basket_item_s *missing_destination);

/**
 *  Modifies `store.items` and `store.missing` items to contain the cheapest items that match each item in `basket`, which it reads from disk.
 *
 * @returns nothing
 *
 * @param store the store to look for items in
 *
 */
void items_filter_items(store_s *store);

/**
 * calculates the amount of items necesary to make up the requested total size within a given variance
 *
 * @returns the amount of items, 0 if no number in variance of requested size is disivible by store item size.
 *
 * @param store_item_size the size of a given item
 * @param variance acceptable percentage variance given as a number between 0 and 1
 * @param requested_size the amount to match, within the variance
 */
int items_is_in_variation(double store_item_size, double variance, double requested_size);

/**
 * Adds given item to the list of items missing in the given store.
 *
 * @returns void
 *
 * @param item to add to store.
 * @param store to add item to.
 *
 * */
void items_add_item_to_missing(basket_item_s item, store_s *store);

/**
 * Adds given item to the list of items found in the given store.
 *
 * @returns void
 *
 * @param item to add to store.
 * @param store to add item to.
 *
 * */
void items_add_item_to_found(found_item_s item, store_s *store);

/**
 * Converts a store_item_s to a found_item_s.
 *
 * @returns converted found_item.
 *
 * @param input_item the item to convert.
 * @param item_count the amount of items needed to match the size requirements of found_item.
 * */
found_item_s items_convert_to_found_item(store_item_s input_item, int item_count);

/**
 * Searches for a string in another string, and returns a boolean.
 *
 * @returns a boolean denoting whether the string was found.
 *
 * @param name_to_find the string to search for.
 * @param name_to_search the string to search in.
 *
 * */
int items_compare_item_names(char *name_to_find, char *name_to_search);

int items_compare_item_units(item_unit_e requested_unit, item_unit_e store_unit);

int items_compare_item_price_per_unit(item_price_t best_item_price, item_price_t store_price);

int items_alternate_unit_match(item_unit_e requested_unit, item_unit_e best_item_unit, item_price_t store_price, item_price_t best_item_price);

#endif