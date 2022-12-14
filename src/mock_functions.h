#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

#include "items_types.h"

int test_fetch_get_stores(store_s *stores);
void test_fetch_get_store_items(store_s *store, int n);
int test_get_salling_basket(basket_item_s *basket);
int test_get_items_one(store_s *store);
int test_get_items_two(store_s *store);
int test_get_items_three(store_s *store);
basket_item_s *test_get_basket(int *basket_item_count);

#endif /* TEST_FUNCTIONS.H */