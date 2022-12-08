#include "test_bargain.h"
#include "bargain.h"
#include "items_types.h"
#include "malloc.h"
#include <assert.h>
#include <string.h>
#include "mock_functions.h"

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

int main()
{
    store_s *stores = malloc(sizeof(store_s) * 3);
    int stores_count = bargain_find_bargain(&stores);
    test_bargain_make_assertions(stores, stores_count);

    return 0;
}

store_s *get_expected_stores()
{
    store_s *stores = malloc(sizeof(store_s) * 3);
    store_s store1 = {
        .name = "Bilka",
        .uid = "Xx_69_longasssallinguuid_420_xX",
        .group = STORE_CHAIN_BILKA,
        .chain = SALLING,
        .address = "Testvænget 1 - 1337 Testby",
        .lat = 1,
        .lon = 1,
        .distance = 1,
        .items_count = 3,
        .items = malloc(3 * sizeof(store_item_s)),
        .found_items = malloc(3 * sizeof(found_item_s)),
        .found_items_count = 3,
        .missing_items = malloc(5 * sizeof(basket_item_s)),
        .missing_items_count = 2,
        .found_items_total_price = 12,
    };

    stores[0] = store1;

    test_get_items_one(&store1);

    found_item_s store_1_found_milk = {
        .name = "Milk\0",
        .size = 1,
        .unit = LITERS,
        .price_per_unit = 1,
        .product_price = 1,
        .total_price = 1,
        .count = 1,
    };

    found_item_s store_1_found_beef = {
        .name = "Beef\0",
        .size = 2,
        .unit = KILOGRAMS,
        .price_per_unit = 1,
        .product_price = 2,
        .total_price = 2,
        .count = 1,
    };

    found_item_s store_1_found_noodles = {
        .name = "Noodles\0",
        .size = 1,
        .unit = UNITS,
        .price_per_unit = 3,
        .product_price = 3,
        .total_price = 9,
        .count = 3,
    };

    store1.found_items[0] = store_1_found_milk;
    store1.found_items[1] = store_1_found_beef;
    store1.found_items[2] = store_1_found_noodles;

    store1.missing_items[0] = item4;
    store1.missing_items[1] = item5;

    store_s store2 = {
        .name = "Fakta",
        .uid = "1234",
        .group = STORE_GROUP_FAKTA,
        .chain = COOP,
        .address = "Testvænget 2 - 1337 Testby",
        .lat = 2,
        .lon = 2,
        .distance = 2,
        .items_count = 3,
        .items = malloc(3 * sizeof(store_item_s)),
        .found_items = malloc(3 * sizeof(found_item_s)),
        .found_items_count = 2,
        .missing_items = malloc(5 * sizeof(basket_item_s)),
        .missing_items_count = 3,
        .found_items_total_price = 3,
    };

    stores[1] = store2;

    found_item_s store_2_found_milk = {
        .name = "Milk\0",
        .size = 1,
        .unit = LITERS,
        .price_per_unit = 2,
        .product_price = 2,
        .total_price = 2,
        .count = 1,
    };

    found_item_s store_2_found_beef = {
        .name = "Beef\0",
        .size = 2,
        .unit = KILOGRAMS,
        .price_per_unit = 2,
        .product_price = 4,
        .total_price = 4,
        .count = 1,
    };

    store2.found_items[0] = store_2_found_milk;
    store2.found_items[1] = store_2_found_beef;

    store2.missing_items[0] = item3;
    store2.missing_items[1] = item4;
    store2.missing_items[2] = item5;

    test_get_items_two(&store2);

    store_s store3 = {
        .name = "Dagli'Brugsen",
        .uid = "5678",
        .group = STORE_GROUP_DAGLI_BRUGSEN,
        .chain = COOP,
        .address = "Testvænget 3 - 1337 Testby",
        .lat = 3,
        .lon = 3,
        .distance = 3,
        .items_count = 4,
        .items = malloc(3 * sizeof(store_item_s)),
        .found_items = malloc(4 * sizeof(found_item_s)),
        .found_items_count = 4,
        .missing_items = malloc(5 * sizeof(basket_item_s)),
        .missing_items_count = 1,
        .found_items_total_price = 182,
    };

    stores[2] = store3;

    found_item_s store_3_found_milk = {
        .name = "Milk\0",
        .size = 1,
        .unit = LITERS,
        .price_per_unit = 4,
        .product_price = 4,
        .total_price = 4,
        .count = 1,
    };
    found_item_s store_3_found_beef = {
        .name = "Beef\0",
        .size = 2,
        .unit = KILOGRAMS,
        .price_per_unit = 10,
        .product_price = 20,
        .total_price = 20,
        .count = 1,
    };
    found_item_s store_3_found_pudding = {
        .name = "Pudding\0",
        .size = 1,
        .unit = UNITS,
        .price_per_unit = 30,
        .product_price = 30,
        .total_price = 150,
        .count = 5,
    };
    found_item_s store_3_found_snickers = {
        .name = "Snickers\0",
        .size = 1,
        .unit = UNITS,
        .price_per_unit = 2,
        .product_price = 2,
        .total_price = 8,
        .count = 4,
    };

    store3.found_items[0] = store_3_found_milk;
    store3.found_items[1] = store_3_found_beef;
    store3.found_items[2] = store_3_found_snickers;
    store3.found_items[3] = store_3_found_pudding;

    store3.missing_items[0] = item3;

    test_get_items_three(&stores[2]);

    return stores;
}

int compare_found_items(found_item_s x, found_item_s y)
{
    if (strcmp(x.name, y.name) == 0 && x.product_price == y.product_price && x.price_per_unit == y.price_per_unit && x.size == y.size && x.unit == y.unit && x.count == y.count && x.total_price == y.total_price)
    {
        return 1;
    }
    return 0;
}

int compare_basket_items(basket_item_s x, basket_item_s y)
{
    if (strcmp(x.name, y.name) == 0 && x.size == y.size && x.unit == y.unit)
    {
        return 1;
    }
    return 0;
}

int compare_store_items(store_item_s x, store_item_s y)
{
    if (strcmp(x.name, y.name) == 0 && x.price == y.price && x.price_per_unit == y.price_per_unit && x.size == y.size && x.unit == y.unit)
    {
        return 1;
    }
    return 0;
}

int compare_stores_structs(store_s x, store_s y)
{

    if (x.items_count != y.items_count)
    {
        return 0;
    }

    if (x.found_items_count != y.found_items_count)
    {
        return 0;
    }

    if (x.missing_items_count != y.missing_items_count)
    {
        return 0;
    }

    for (int i = 0; i < x.items_count; i++)
    {

        if (!compare_store_items(x.items[i], y.items[i]))
        {
            return 0;
        }
    }

    for (int i = 0; i < x.found_items_count; i++)
    {

        if (!compare_found_items(x.found_items[i], y.found_items[i]))
        {
            return 0;
        }
    }

    for (int i = 0; i < x.missing_items_count; i++)
    {
        if (!compare_basket_items(x.missing_items[i], y.missing_items[i]))
        {
            return 0;
        }
    }
    return 1;
}

// int test_stores(store_s *stores, int store_count)
// {
//     int expected_stores_count = 3;
//     store_s *expected_stores = get_expected_stores();
//     int store_counts_match = (expected_stores_count == store_count);
//     printf("Store count matches: %d == %d?\n", expected_stores_count, store_count);
//     assert(store_counts_match);
//     for (int i = 0; i < 3; i++)
//     {
//         int test_store_result_match_expectation = (!compare_stores_structs(stores[i], expected_stores[i]));
//         assert(test_store_result_match_expectation);
//     }
//     return 1;
// }

int test_bargain_make_assertions(store_s *stores, int store_count)
{
    // test_stores(stores, store_count);
    return 0;
}

int test_bargain_get_basket(basket_item_s *basket)
{
    // basket_item_s item1 = {
    //     .name = "Milk",
    //     .size = 1,
    //     .unit = LITERS,
    // };

    // basket_item_s item2 = {
    //     .name = "Beef",
    //     .size = 2,
    //     .unit = KILOGRAMS,
    // };

    // basket_item_s item3 = {
    //     .name = "Noodles",
    //     .size = 3,
    //     .unit = UNITS,
    // };

    // basket_item_s item4 = {
    //     .name = "Snickers",
    //     .size = 4,
    //     .unit = UNITS,
    // };

    // basket_item_s item5 = {
    //     .name = "Pudding",
    //     .size = 5,
    //     .unit = UNITS,
    // };

    basket[0] = item1;
    basket[1] = item2;
    basket[2] = item3;
    basket[3] = item4;
    basket[4] = item5;

    return 5;
}
