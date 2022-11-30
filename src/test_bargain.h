#ifndef TEST_BARGAIN_H
#define TEST_BARGAIN_H
#include "items_types.h"
int test_bargain_make_assertions(store_s *stores);
int test_fetch_get_stores(store_s *stores);
void test_fetch_get_store_items(store_s *store, int n);
int test_get_items_one(store_s *store);
int test_get_items_two(store_s *store);
int test_get_items_three(store_s *store);
int test_get_basket(basket_item_s *basket);
#endif