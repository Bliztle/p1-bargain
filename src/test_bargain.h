#ifndef TEST_BARGAIN_H
#define TEST_BARGAIN_H
#include "items_types.h"
int test_bargain_make_assertions(store_s *stores, int store_count);
int test_fetch_get_stores(store_s *stores);
void test_fetch_get_store_items(store_s *store, int n);
#endif