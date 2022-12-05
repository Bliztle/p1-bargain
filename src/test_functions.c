
#include "test_functions.h"
#include "items_types.h"
#include "malloc.h"
#include "string.h"

int test_fetch_get_stores(store_s *stores)
{
    store_s store1 = {
        .name = "Bilka",
        .uid = "Xx_69_longasssallinguuid_420_xX",
        .group = STORE_CHAIN_BILKA,
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

int test_get_salling_basket(basket_item_s *basket)
{

    basket = malloc(10 * sizeof(basket_item_s));

    strncpy(basket[0].name, "Skåvl", ITEM_NAME_SIZE);
    basket[0].size = 1;
    basket[0].unit = UNKNOWN;

    strncpy(basket[1].name, "Spade", ITEM_NAME_SIZE);
    basket[1].size = 1;
    basket[1].unit = UNKNOWN;

    strncpy(basket[2].name, "Spegepølse", ITEM_NAME_SIZE);
    basket[2].size = 1;
    basket[2].unit = UNKNOWN;

    strncpy(basket[3].name, "Sild", ITEM_NAME_SIZE);
    basket[3].size = 1;
    basket[3].unit = UNKNOWN;

    strncpy(basket[4].name, "Brød", ITEM_NAME_SIZE);
    basket[4].size = 1;
    basket[4].unit = UNKNOWN;

    strncpy(basket[5].name, "Lys", ITEM_NAME_SIZE);
    basket[5].size = 1;
    basket[5].unit = UNKNOWN;

    strncpy(basket[6].name, "Mad", ITEM_NAME_SIZE);
    basket[6].size = 1;
    basket[6].unit = UNKNOWN;

    strncpy(basket[7].name, "Æg", ITEM_NAME_SIZE);
    basket[7].size = 1;
    basket[7].unit = UNKNOWN;

    strncpy(basket[8].name, "Mælk", ITEM_NAME_SIZE);
    basket[8].size = 1;
    basket[8].unit = UNKNOWN;

    strncpy(basket[9].name, "Redskab", ITEM_NAME_SIZE);
    basket[9].size = 1;
    basket[9].unit = UNKNOWN;

    return 10;
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
        .name = "Bananer",
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

basket_item_s *test_get_basket(int *basket_item_count)
{

    basket_item_s *basket = malloc(5 * sizeof(basket_item_s));

    const basket_item_s item1 = {
        .name = "Milk\0",
        .size = 1,
        .unit = LITERS,
    };

    const basket_item_s item2 = {
        .name = "Beef\0",
        .size = 2,
        .unit = KILOGRAMS,
    };

    const basket_item_s item3 = {
        .name = "Noodles\0",
        .size = 3,
        .unit = UNITS,
    };

    const basket_item_s item4 = {
        .name = "Snickers\0",
        .size = 4,
        .unit = UNITS,
    };

    const basket_item_s item5 = {
        .name = "Pudding\0",
        .size = 5,
        .unit = UNITS,
    };

    basket[0] = item1;
    basket[1] = item2;
    basket[2] = item3;
    basket[3] = item4;
    basket[4] = item5;

    *basket_item_count = 5;

    return basket;
}