#ifndef BARGAIN_H
#define BARGAIN_H
#include "items_types.h"
#include "stddef.h"

void bargain_run_bargain();

/**
 * Starts the process of finding the cheapest store.
 *
 * @return void.
 */
void bargain_menu_find_bargain();

/**
 * Takes an empty array of stores, calls fetch_get_stores on it,
 * and sorts them using qsort.
 * With stores_compare_stores as sorter function.
 *
 * @returns the total count of stores.
 *
 * @param stores, the empty array.
 */
int bargain_find_bargain(store_s *stores);

void bargain_print_bargain_result(store_s store);

/**
 * Takes a store_s struct, and generates a string describing
 * the contents of it in a user friendly manner.
 * The returned string is null terminated.
 *
 * @returns the null terminated string.
 *
 * @param store, the store to generate a string based on.
 */
char *bargain_get_print_bargain_string(store_s store);

/**
 * Uses the print_menu macro to print a menu based
 * on a store, where the user can either save the printed list
 * to a text file, or exit the menu.
 *
 * @returns void.
 *
 * @param store, the store to base the list on.
 */
void bargain_menu_print_bargain(store_s store);

/**
 * Takes a store and creates human readable string from the
 * items in the found items list. Then appends it to
 * another string.
 *
 * @returns void.
 *
 * @param store, the store to base the string on.
 *
 * @param string_to_append_to, the string to append the created string to.
 */
void create_found_entries(store_s store, char **string_to_append_to, size_t size_of_string);

/**
 * Takes a store and creates human readable string from the
 * items in the missing items list. Then appends it to
 * another string.
 *
 * @returns void.
 *
 * @param store, the store to base the string on.
 *
 * @param string_to_append_to, the string to append the created string to.
 */
void create_missing_entries(store_s store, char **string_to_append_to, size_t size_of_string);

/**
 * Takes a store and creates human readable string from the
 * general information regarding the store. Then appends it to
 * another string.
 *
 * @returns void.
 *
 * @param store, the store to base the string on.
 *
 * @param string_to_append_to, the string to append the created string to.
 */
void append_outro_to_string(store_s store, char **string_to_append_to, size_t size_of_string);

/**
 * Calculates the amount of bytes needet to hold the complete list
 * of items, in both the missing and found lists. Then assigns the given integers
 * to the resulting values.
 *
 * @returns void.
 *
 * @param store, the store on which to calculate entry sizes.
 *
 * @param found_list_size, pointer to the integer that holds the size of the
 * bytes needed to store a readable string repressenting the list of found items.
 *
 * @param missing_list_size, pointer to the integer that holds the size of the
 * bytes needed to store a readable string repressenting the list of missing items.
 */
void get_size_of_list_entries(store_s store, int *found_list_size, int *missing_list_size);

/**
 * Returns the human readable string repressented by the unit enum.
 *
 * @returns the unit name as a string.
 *
 * @param n, the number of the enum.
 */
char *bargain_get_unit(int n);
#endif /* BARGAIN_H */