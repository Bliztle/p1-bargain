#ifndef BASKET_H
#define BASKET_H
#include <stdlib.h>
#include "items_types.h"

static const char* TEMP_BASKET_PATH = ".temp_basket.txt";

struct basket_s;
typedef struct basket_s basket_s;

typedef struct basket_s {
    basket_item_s item;
    basket_s *next;
} basket_s;

/**
 * Creates a new basket, only containing `item`.
 * Note that the basket is heap-allocated using `malloc`, so it needs to be freed using the `basket_free` method
 * @param item the item that should be in the basket
 * @return a new basket containing `item`
 */
basket_s *basket_new(basket_item_s item);

/**
 * Pushes an item to the end of a basket.
 * @param basket the basket to put the item into
 * @param item the item to push into the basket
 */
void basket_push(basket_s *basket, basket_item_s item);
/**
 * Removes an item at a given index from a basket
 * This takes a pointer pointer, since the location of the head may be set to NULL to indicate an empty list
 * @param basket a pointer to the basket where the item should be removed from
 * @param index the index of the item to be removed
 */
void basket_remove(basket_s **basket, size_t index);
/**
 * Removes and frees the first element in a basket.
 * This mostly just a utility function, and using `basket_remove` should be preferred
 * This takes a pointer pointer, since the location of the head may be set to NULL to indicate an empty list
 * @param basket the basket to remove from
 */
void basket_remove_first(basket_s **basket);
/**
 * Prints a basket to the console
 * @param basket the basket to print
 */
void basket_print(basket_s *basket);
/**
 * Frees each item in the list from memory
 * @param basket the basket to be freed
 */
void basket_free(basket_s *basket);
/**
 * Converts a basket_s to a heap-allocated array of basket_item_s
 * @param basket the list to be converted
 * @param items an output parameter, where the location of the array will be placed
 * @return the length of the array
 */
int basket_to_array(basket_s *basket, basket_item_s **items);

basket_s *basket_read();
void basket_write(basket_s *basket);

void menu_basket_edit();
void menu_basket_add();
void menu_basket_remove();
#endif
