#include "test_bargain.h"
#include "bargain.h"
#include "items_types.h"
#include "malloc.h"

    const store_s store1 = {
        .name = "Bilka",
        .group = STORE_GROUP_BILKA,
        .chain = SALLING,
        .address = "Test1",
        .lat = 1,
        .lon = 1,
        .distance = 1,
        .items_count = 0,
        .found_items_count = 0,
        .missing_items_count = 0,
        .found_items_total_price = 0
    };

    const store_s store2 = {
        .name = "Fakta",
        .group = STORE_GROUP_FAKTA,
        .chain = COOP,
        .address = "Test2",
        .lat = 2,
        .lon = 2,
        .distance = 2,
        .items_count = 0,
        .found_items_count = 0,
        .missing_items_count = 0,
        .found_items_total_price = 0
    };

    const store_s store3 = {
        .name = "Dagli'Brugsen",
        .group = STORE_GROUP_DAGLI_BRUGSEN,
        .chain = COOP,
        .address = "Test3",
        .lat = 3,
        .lon = 3,
        .distance = 3,
        .items_count = 0,
        .found_items_count = 0,
        .missing_items_count = 0,
        .found_items_total_price = 0
    };



int main() {

    bargain_menu_find_bargain();

    return 0;
}

int test_fetch_get_stores(store_s **stores) {

    stores = malloc(3 * sizeof(store_s));

    store_s store_one = store1;
    store_s store_two = store2;
    store_s store_three = store3;

    store_one.items = malloc(3 * sizeof(store_item_s));
    store_two.items = malloc(3 * sizeof(store_item_s));
    store_three.items = malloc(3 * sizeof(store_item_s));


    stores[0] = &store_one ;
    stores[1] = &store_two ;
    stores[2] = &store_three;

    return 3;

}

void test_fetch_get_store_items(store_s **stores, int n) {

    switch (n) {
        case 0: {
            stores[n]->items_count = test_get_items_one(stores[n]);
            break;
            }
        case 1: {
            stores[n]->items_count = test_get_items_two(stores[n]);
            break;
            }
        case 2: {
            stores[n]->items_count = test_get_items_three(stores[n]);
            break;
            }
        default:
            break;
    }
}

int test_get_items_one(store_s *store) {

    store_item_s item1 = {
        .name = "Milk",
        .price = 1,
        .size = 1,
        .unit = LITERS,
        .price_per_unit = 1
    };

    store_item_s item2 = {
        .name = "Beef",
        .price = 2,
        .size = 2,
        .unit = KILOGRAMS,
        .price_per_unit = 2
    };

    store_item_s item3 = {
        .name = "Noodles",
        .price = 3,
        .size = 3,
        .unit = UNITS,
        .price_per_unit = 3
    };

    store->items[0] = item1;
    store->items[1] = item2;
    store->items[2] = item3;

    return 3;
}

int test_get_items_two(store_s *store) {

    store_item_s item1 = {
        .name = "Milk",
        .price = 1,
        .size = 1,
        .unit = LITERS,
        .price_per_unit = 1
    };

    store_item_s item2 = {
        .name = "Beef",
        .price = 2,
        .size = 2,
        .unit = KILOGRAMS,
        .price_per_unit = 2
    };

    store_item_s item3 = {
        .name = "Snickers",
        .price = 3,
        .size = 3,
        .unit = UNITS,
        .price_per_unit = 3
    };

    store->items[0] = item1;
    store->items[1] = item2;
    store->items[2] = item3;

    return 3;
}

int test_get_items_three(store_s *store) {

    store_item_s item1 = {
        .name = "Milk",
        .price = 1,
        .size = 1,
        .unit = LITERS,
        .price_per_unit = 1
    };

    store_item_s item2 = {
        .name = "Beef",
        .price = 2,
        .size = 2,
        .unit = KILOGRAMS,
        .price_per_unit = 2
    };

    store_item_s item3 = {
        .name = "Pudding",
        .price = 3,
        .size = 3,
        .unit = UNITS,
        .price_per_unit = 3
    };

    store->items[0] = item1;
    store->items[1] = item2;
    store->items[2] = item3;

    return 3;
}

int test_get_basket(basket_item_s **basket) {

    basket = malloc(5 * sizeof(basket_item_s));

    basket_item_s item1 = {
        .name = "Milk",
        .size = 1,
        .unit = LITERS,
    };

    basket_item_s item2 = {
        .name = "Beef",
        .size = 2,
        .unit = KILOGRAMS,
    };

    basket_item_s item3 = {
        .name = "Noodles",
        .size = 3,
        .unit = UNITS,
    };

    basket_item_s item4 = {
        .name = "Snickers",
        .size = 4,
        .unit = UNITS,
    };

    basket_item_s item5 = {
        .name = "Pudding",
        .size = 5,
        .unit = UNITS,
    };

    basket[0] = &item1;
    basket[1] = &item2;
    basket[2] = &item3;
    basket[3] = &item4;
    basket[4] = &item5;
    
    return 5;
}

