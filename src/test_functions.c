
#include "test_functions.h"
#include "items_types.h"
#include "malloc.h"

int test_fetch_get_stores(store_s *stores)
{
    store_s store1 = {
        .name = "Bilka",
        .uid = "Xx_69_longasssallinguuid_420_xX",
        .group = STORE_GROUP_BILKA,
        .chain = SALLING,
        .address = "Testvænget 1 - 1337 Testby",
        .lat = 1,
        .lon = 1,
        .distance = 1,
        .items_count = 0,
        .items = malloc(3 * sizeof(store_item_s)),
        .found_items = malloc(3 * sizeof(found_item_s)),
        .found_items_count = 0,
        .missing_items = malloc(5 * sizeof(basket_item_s)),
        .missing_items_count = 0,
        .found_items_total_price = 0};

    store_s store2 = {
        .name = "Fakta",
        .uid = "1234",
        .group = STORE_GROUP_FAKTA,
        .chain = COOP,
        .address = "Testvænget 2 - 1337 Testby",
        .lat = 2,
        .lon = 2,
        .distance = 2,
        .items_count = 0,
        .items = malloc(3 * sizeof(store_item_s)),
        .found_items = malloc(3 * sizeof(found_item_s)),
        .found_items_count = 0,
        .missing_items = malloc(5 * sizeof(basket_item_s)),
        .missing_items_count = 0,
        .found_items_total_price = 0};

    store_s store3 = {
        .name = "Dagli'Brugsen",
        .uid = "5678",
        .group = STORE_GROUP_DAGLI_BRUGSEN,
        .chain = COOP,
        .address = "Testvænget 3 - 1337 Testby",
        .lat = 3,
        .lon = 3,
        .distance = 3,
        .items_count = 0,
        .items = malloc(4 * sizeof(store_item_s)),
        .found_items = malloc(3 * sizeof(found_item_s)),
        .found_items_count = 0,
        .missing_items = malloc(5 * sizeof(basket_item_s)),
        .missing_items_count = 0,
        .found_items_total_price = 0};

    stores[0] = store1;
    stores[1] = store2;
    stores[2] = store3;

    return 3;
}

void test_fetch_get_store_items(store_s *store, int store_count)
{

    switch (store_count)
    {
    case 0:
        store->items_count = test_get_items_one(store);
        break;
    case 1:
        store->items_count = test_get_items_two(store);
        break;
    case 2:
        store->items_count = test_get_items_three(store);
        break;
    default:
        break;
    }
}

int test_get_items_one(store_s *store)
{

    store_item_s item1 = {
        .name = "Milk\0",
        .price = 1,
        .size = 1,
        .unit = LITERS,
        .price_per_unit = 1};

    store_item_s item2 = {
        .name = "Beef\0",
        .price = 2,
        .size = 2,
        .unit = KILOGRAMS,
        .price_per_unit = 1};

    store_item_s item3 = {
        .name = "Noodles\0",
        .price = 3,
        .size = 1,
        .unit = UNITS,
        .price_per_unit = 3};

    store->items = malloc(3 * sizeof(store_item_s));

    store->items[0] = item1;
    store->items[1] = item2;
    store->items[2] = item3;

    return 3;
}

int test_get_items_two(store_s *store)
{

    store_item_s item1 = {
        .name = "Milk\0",
        .price = 2,
        .size = 1,
        .unit = LITERS,
        .price_per_unit = 2};

    store_item_s item2 = {
        .name = "Beef\0",
        .price = 4,
        .size = 2,
        .unit = KILOGRAMS,
        .price_per_unit = 2};

    store_item_s item3 = {
        .name = "Snickers\0",
        .price = 3,
        .size = 3,
        .unit = UNITS,
        .price_per_unit = 3};

    store->items = malloc(3 * sizeof(store_item_s));

    store->items[0] = item1;
    store->items[1] = item2;
    store->items[2] = item3;

    return 3;
}

int test_get_items_three(store_s *store)
{

    store_item_s item1 = {
        .name = "Milk\0",
        .price = 4,
        .size = 1,
        .unit = LITERS,
        .price_per_unit = 4};

    store_item_s item2 = {
        .name = "Beef\0",
        .price = 20,
        .size = 2,
        .unit = KILOGRAMS,
        .price_per_unit = 10};

    store_item_s item3 = {
        .name = "Pudding\0",
        .price = 30,
        .size = 1,
        .unit = UNITS,
        .price_per_unit = 3};

    store_item_s item4 = {
        .name = "Snickers\0",
        .price = 2,
        .size = 1,
        .unit = UNITS,
        .price_per_unit = 2};

    store->items[0] = item1;
    store->items[1] = item2;
    store->items[2] = item3;
    store->items[3] = item4;

    return 4;
}