#include <assert.h>
#include "api/parse.h"
#include "stdio.h"
#include "stdlib.h"
#include "test_items.h"
#include "items.h"
#include "time.h"

int main() {
    test_items_filter_items();
}

int test_items_filter_items() {

    time_t t;
    srand((unsigned) time((&t)));

    char *names[16];

    names[0] = "Almond jelly";
    names[1] = "Ashure";
    names[2] = "Asida";
    names[3] = "Pudim Abade de Priscos";
    names[4] = "Banana pudding";
    names[5] = "Banh chuoi";
    names[6] = "Bebinca";
    names[7] = "Blancmange";
    names[8] = "Bread and butter pudding";
    names[9] = "Brown Betty";
    names[10] = "Cabinet pudding";
    names[11] = "Che";
    names[12] = "Chocolate Biscuit pudding";
    names[13] = "Christmas pudding";
    names[14] = "Clootie dumpling";
    names[15] = "Coconut pudding";

    char *not_in_store_names[200];

    not_in_store_names[0] = "banana";
    not_in_store_names[1] = "fish";
    not_in_store_names[2] = "eggs";
    not_in_store_names[3] = "pudding";
    not_in_store_names[4] = "gamble";

    store_s store = {
        .found_items = malloc(2048),
        .items = malloc(2048),
        .missing_items = malloc(2048)
    };

    store_item_s item1 = {
        .name = "Almond jelly",
        .price = rand_price(),
        .size = 1,
        .unit = 1,
    };
    store_item_s item2 = {
        .name = "Ashure",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item3 = {
        .name = "Asida",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item4 = {
        .name = "Pudim Abade de Priscos",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item5 = {
        .name = "Banana pudding",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item6 = {
        .name = "Banh chuoi",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item7 = {
        .name = "Bebinca",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item8 = {
        .name = "Blancmange",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item9 = {
        .name = "Bread and butter pudding",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item10 = {
        .name = "Brown Betty",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item11 = {
        .name = "Cabinet pudding",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item12 = {
        .name = "Che",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item13 = {
        .name = "Chocolate Biscuit pudding",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item14 = {
        .name = "Christmas pudding",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item15 = {
        .name = "Clootie dumpling",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };
    store_item_s item16 = {
        .name = "Coconut pudding",
        .price = rand_price(),
        .size = rand_size(),
        .unit = rand_unit(),
    };

    item1.price_per_unit = item1.price / item1.size;
    item2.price_per_unit = item2.price / item2.size;
    item3.price_per_unit = item3.price / item3.size;
    item4.price_per_unit = item4.price / item4.size;
    item5.price_per_unit = item5.price / item5.size;
    item6.price_per_unit = item6.price / item6.size;
    item7.price_per_unit = item7.price / item7.size;
    item8.price_per_unit = item8.price / item8.size;
    item9.price_per_unit = item9.price / item9.size;
    item10.price_per_unit = item10.price / item10.size;
    item11.price_per_unit = item11.price / item11.size;
    item12.price_per_unit = item12.price / item12.size;
    item13.price_per_unit = item13.price / item13.size;
    item14.price_per_unit = item14.price / item14.size;
    item15.price_per_unit = item15.price / item15.size;
    item16.price_per_unit = item16.price / item16.size;

    store.items[0] = item1;
    store.items[1] = item2;
    store.items[2] = item3;
    store.items[3] = item4;
    store.items[4] = item5;
    store.items[5] = item6;
    store.items[6] = item7;
    store.items[7] = item8;
    store.items[8] = item9;
    store.items[9] = item10;
    store.items[10] = item11;
    store.items[11] = item12;
    store.items[12] = item13;
    store.items[13] = item14;
    store.items[14] = item15;
    store.items[15] = item16;

    store.items_count = 16;

    basket_item_s basket[20];

    basket_item_s basket_item1 = {
        .name = "Almond jelly",
        .unit = 1,
        .size = rand_size()
    };
    basket_item_s basket_item2 = {
        .name = "Coconut pudding",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item3 = {
        .name = "Clootie dumpling",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item4 = {
        .name = "Chocolate Biscuit pudding",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item5 = {
        .name = "Che",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item6 = {
        .name = "Cabinet pudding",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item7 = {
        .name = "Brown Betty",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item8 = {
        .name = "Bread and butter pudding",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item9 = {
        .name = "Blancmange",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item10 = {
        .name = "Banh chuoi",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item11 = {
        .name = "rope",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item12 = {
        .name = "milk",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item13 = {
        .name = "genius",
        .unit = rand_unit(),
        .size = rand_size()
    };
    basket_item_s basket_item14 = {
        .name = "pudding",
        .unit = rand_unit(),
        .size = rand_size()
    };

    basket[0] = basket_item1;
    basket[1] = basket_item2;
    basket[2] = basket_item3;
    basket[3] = basket_item4;
    basket[4] = basket_item5;
    basket[5] = basket_item6;
    basket[6] = basket_item7;
    basket[7] = basket_item8;
    basket[8] = basket_item9;
    basket[9] = basket_item10;
    basket[10] = basket_item11;
    basket[11] = basket_item12;
    basket[12] = basket_item13;
    basket[13] = basket_item14;

    printf("Basket Items\n--------------------------------\n");
    for (int i = 0; i < 14; i++)
    {
        printf("Item %d:\n", i);
        printf("name: %s\n size: %lf\n unit: %s\n\n",
               basket[i].name,
               basket[i].size,
               get_unit(basket[i].unit));
    }
    printf("Basket Items End\n--------------------------------\n");

    store.found_items = malloc(2048);
    store.found_items_count = 0;
    store.missing_items = malloc(2048);
    store.missing_items_count = 0;

    items_filter_items(basket, 14, &store);

    printf("Store Items\n--------------------------------\n");
    for (int i = 0; i < store.items_count; ++i) {

        printf("Item %d:\n", i);
        printf("name: %s\n"
               " product_price: %lf\n"
               " price_per_unit: %lf\n"
               " size: %lf\n"
               " unit: %s\n\n",
               store.items[i].name,
               store.items[i].price,
               store.items[i].price_per_unit,
               store.items[i].size,
               get_unit(store.items[i].unit));
    }

    printf("Found Items\n--------------------------------\n");
    for (int i = 0; i < store.found_items_count; i++)
    {
        printf("Item %d:\n", i);
        printf("name: %s\n "
               "product_price: %lf\n"
               " price_per_unit: %lf\n"
               " size: %lf\n unit: %s\n"
               " count: %d\n"
               " total_price: %lf\n\n",
               store.found_items[i].name,
               store.found_items[i].product_price,
               store.found_items[i].price_per_unit,
               store.found_items[i].size,
               get_unit(store.found_items[i].unit),
               store.found_items[i].count,
               store.found_items[i].product_price * store.found_items[i].count);
    }

    printf("Missing Items\n--------------------------------\n");
    for (int i = 0; i < store.missing_items_count; i++)
    {
        printf("Item %d:\n", i);
        printf("name: %s\n", store.missing_items[i].name);
        printf("size: %lf\n", store.missing_items[i].size);
        printf("unit: %s\n\n", get_unit(store.missing_items[i].unit));
    }

    free(store.found_items);
    free(store.missing_items);

    return 0;
}

int rand_size() {
    // rand() % (max_number + 1 - minimum_number) + minimum_number
    return rand() % (5 + 1 - 1) + 1;
}

int rand_unit() {
    return rand() % (2 + 1);
}

double rand_price() {
    return rand() % (500 + 1 - 1) + 1;
}

char* get_unit(int n) {

    switch (n) {
        case 0: {
            return "KILOGRAMS";
        }
        case 1: {
            return "LITERS";
        }
        case 2: {
            return "UNITS";
        }
    }
}
