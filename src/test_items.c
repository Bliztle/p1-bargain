#include <assert.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#include "items.h"
#include "items_types.h"
#include "mock_functions.h"

// Test items_compare_item_names()
void test_items_compare_item_names()
{
    // Test with matching strings
    assert(items_compare_item_names("apple", "red apple") == 1);
    assert(items_compare_item_names("banana", "banana bread") == 1);

    // Test with non-matching strings
    assert(items_compare_item_names("grape", "red apple") == 0);
    assert(items_compare_item_names("cake", "banana bread") == 0);
}

// Test items_compare_item_units()
void test_items_compare_item_units()
{
    // Test with matching units
    assert(items_compare_item_units(KILOGRAMS, KILOGRAMS) == 1);
    assert(items_compare_item_units(LITERS, LITERS) == 1);

    // Test with non-matching units
    assert(items_compare_item_units(KILOGRAMS, LITERS) == 0);
    assert(items_compare_item_units(LITERS, KILOGRAMS) == 0);
}

// Test items_compare_item_price_per_unit()
void test_items_compare_item_price_per_unit()
{
    // Test with lower prices
    assert(items_compare_item_price_per_unit(10, 5) == 1);
    assert(items_compare_item_price_per_unit(5, 1) == 1);

    // Test with equal prices
    assert(items_compare_item_price_per_unit(5, 5) == 0);
    assert(items_compare_item_price_per_unit(10, 10) == 0);

    // Test with higher prices
    assert(items_compare_item_price_per_unit(1, 5) == 0);
    assert(items_compare_item_price_per_unit(5, 10) == 0);
}

// Test items_alternate_unit_match() (continued)
void test_items_alternate_unit_match()
{
    // Test with matching units and equal prices
    assert(items_alternate_unit_match(KILOGRAMS, KILOGRAMS, 5, 5) == 0);
    assert(items_alternate_unit_match(LITERS, LITERS, 1, 1) == 0);

    // Test with non-matching units and equal prices
    assert(items_alternate_unit_match(KILOGRAMS, LITERS, 5, 5) == 0);
    assert(items_alternate_unit_match(LITERS, KILOGRAMS, 1, 1) == 0);

    // Test with unknown units
    assert(items_alternate_unit_match(UNKNOWN, LITERS, 5, 10) == 1);
    assert(items_alternate_unit_match(LITERS, UNKNOWN, 10, 5) == 0);
}

// Test items_is_in_variation()
void test_items_is_in_variation()
{
    // Test with store item size of 5, variance of 0.5, and requested size of 10
    double store_item_size = 5;
    double variance = 0.5;
    double requested_size = 10;

    int expected = 2;
    int result = items_is_in_variation(store_item_size, variance, requested_size);

    assert(result == expected);

    // Test with store item size of 10, variance of 0.5, and requested size of 5
    store_item_size = 10;
    variance = 0.5;
    requested_size = 5;

    expected = 0;
    result = items_is_in_variation(store_item_size, variance, requested_size);

    assert(result == expected);

    // Test with store item size of 10, variance of 0.1, and requested size of 11
    store_item_size = 11;
    variance = 0.1;
    requested_size = 10;

    expected = 1;
    result = items_is_in_variation(store_item_size, variance, requested_size);

    assert(result == expected);

    // Test with store item size of 10, variance of 0.1, and requested size of 8
    store_item_size = 10;
    variance = 0.1;
    requested_size = 8;

    expected = 0;
    result = items_is_in_variation(store_item_size, variance, requested_size);

    assert(result == expected);
}

// Test items_convert_to_found_item()
void test_items_convert_to_found_item()
{
    // Test with UNKNOWN unit and item count of 1
    store_item_s input_item = {
        .name = "apple",
        .price = 1,
        .size = 5,
        .unit = UNKNOWN,
    };
    int item_count = 1;

    found_item_s expected = {
        .name = "apple",
        .total_price = 1,
        .product_price = 1,
        .count = 1,
        .size = 5,
        .unit = UNKNOWN,
    };
    found_item_s result = items_convert_to_found_item(input_item, item_count);

    assert(strcmp(result.name, expected.name) == 0);
    assert(result.total_price == expected.total_price);
    assert(result.product_price == expected.product_price);
    assert(result.count == expected.count);
    assert(result.size == expected.size);
    assert(result.unit == expected.unit);

        // Test with LITERS unit and item count of 1
    input_item = (store_item_s){
        .name = "apple",
        .price = 1,
        .size = 5,
        .unit = LITERS,
    };
    
    input_item.price_per_unit = (input_item.size / input_item.price);
    item_count = 1;

    expected = (found_item_s){
        .name = "apple",
        .total_price = 1,
        .product_price = 1,
        .count = 1,
        .size = 5,
        .unit = LITERS,
    };
    
    expected.price_per_unit = (expected.size / expected.product_price);
    result = items_convert_to_found_item(input_item, item_count);

    assert(strcmp(result.name, expected.name) == 0);
    assert(result.total_price == expected.total_price);
    assert(result.product_price == expected.product_price);
    assert(result.count == expected.count);
    assert(result.size == expected.size);
    assert(result.unit == expected.unit);
    assert(result.price_per_unit == expected.price_per_unit);

    // Test with non-UNKNOWN unit and item count of 2
    input_item = (store_item_s){
        .name = "banana",
        .price = 5,
        .size = 10,
        .unit = LITERS,
        .price_per_unit = 1
    };
    item_count = 2;
    input_item.price_per_unit = (input_item.size / input_item.price);

    expected = (found_item_s){
        .name = "banana",
        .total_price = 10,
        .product_price = 5,
        .count = 2,
        .size = 10,
        .unit = LITERS,
        .price_per_unit = 1
    };
    expected.price_per_unit = (expected.size / expected.product_price);
    result = items_convert_to_found_item(input_item, item_count);

    assert(strcmp(result.name, expected.name) == 0);
    assert(result.total_price == expected.total_price);
    assert(result.product_price == expected.product_price);
    assert(result.count == expected.count);
    assert(result.size == expected.size);
    assert(result.unit == expected.unit);
    assert(result.price_per_unit == expected.price_per_unit);
}

// Test items_add_item_to_found() and items_add_item_to_missing()
void test_items_add_item_to_found_and_missing()
{
    // Test with found item
    found_item_s item = {
        .name = "apple",
        .total_price = 5,
        .product_price = 2,
        .count = 2,
        .size = 5,
        .unit = UNKNOWN,
        .price_per_unit = 0
    };
    store_s store = {
        .found_items_count = 0,
        .found_items_total_price = 0
    };
    store.found_items = malloc(sizeof(found_item_s));

    items_add_item_to_found(item, &store);

    assert(store.found_items_count == 1);
    assert(!strcmp(store.found_items[0].name, item.name));
    assert(store.found_items[0].total_price == item.total_price);
    assert(store.found_items[0].product_price == item.product_price);
    assert(store.found_items[0].count == item.count);
    assert(store.found_items[0].size == item.size);
    assert(store.found_items[0].unit == item.unit);
    assert(store.found_items[0].price_per_unit == item.price_per_unit);
    assert(store.found_items_total_price == 5);

    free(store.found_items);

    // Test with missing item
    basket_item_s missing_item = {
        .name = "banana",
        .size = 5,
        .unit = UNKNOWN
    };
    store = (store_s){
        .missing_items_count = 0
    };

    store.missing_items = malloc(sizeof(basket_item_s));

    items_add_item_to_missing(missing_item, &store);

    assert(store.missing_items_count == 1);
    assert(!strcmp(store.missing_items[0].name, missing_item.name));
    assert(store.missing_items[0].size == missing_item.size);
    assert(store.missing_items[0].unit == missing_item.unit);

    free(store.missing_items);

}

// Test items_find_best_match()
void test_items_find_best_match()
{
    // Test with matching item
    basket_item_s requested_item = {
        .name = "apple",
        .size = 5,
        .unit = UNITS
    };
    store_s store = {
        .items_count = 1,
        };

    store.items = malloc(sizeof(store_item_s));
    store.items[0] = (store_item_s){
                .name = "apple",
                .price = 20,
                .size = 5,
                .unit = UNITS,
                .price_per_unit = 0
            };
    
    store.items[0].price_per_unit = store.items[0].size / store.items[0].price;

    found_item_s found_destination;
    basket_item_s missing_destination;

    int expected = 1;
    int result = items_find_best_match(requested_item, &store, &found_destination, &missing_destination);

    assert(result == expected);
    assert(!strcmp(found_destination.name, "apple"));
    assert(found_destination.total_price == 20);
    assert(found_destination.product_price == 20);
    assert(found_destination.count == 1);
    assert(found_destination.size == 5);
    assert(found_destination.unit == UNITS);
    assert(found_destination.price_per_unit == (store.items[0].size / store.items[0].price));

    free(store.items);

    // Test with missing item
    requested_item = (basket_item_s){
        .name = "banana",
        .size = 5,
        .unit = KILOGRAMS
    };
    store = (store_s){
        .items_count = 1,
        };

    store.items = malloc(sizeof(store_item_s));
    store.items[0] = (store_item_s){
                .name = "apple",
                .price = 20,
                .size = 5,
                .unit = UNITS,
                .price_per_unit = 0
            };

    store.items[0].price_per_unit = store.items[0].size / store.items[0].price;

    expected = 0;
    result = items_find_best_match(requested_item, &store, &found_destination, &missing_destination);

    assert(result == expected);
    assert(!strcmp(missing_destination.name, "banana"));
    assert(missing_destination.size == 5);
    assert(missing_destination.unit == KILOGRAMS);

    
    free(store.items);

    // Test with unknow unit type.
    requested_item = (basket_item_s){
        .name = "banana",
        .size = 20,
        .unit = UNKNOWN
    };
    store = (store_s){
        .items_count = 1,
        };

    store.items = malloc(sizeof(store_item_s) * 2);
    store.items[0] = (store_item_s){
                .name = "banana",
                .price = 20,
                .size = 5,
                .unit = UNITS,
                .price_per_unit = 0
            };    
    store.items[1] = (store_item_s){
                .name = "banana",
                .price = 30,
                .size = 20,
                .unit = UNITS,
                .price_per_unit = 0
            };

    store.items[0].price_per_unit = store.items[0].size / store.items[0].price;

    expected = 1;
    result = items_find_best_match(requested_item, &store, &found_destination, &missing_destination);

    assert(result == expected);
    assert(!strcmp(found_destination.name, "banana"));
    assert(found_destination.total_price == 20);
    assert(found_destination.product_price == 20);
    assert(found_destination.count == 1);
    assert(found_destination.size == 5);
    assert(found_destination.unit == UNITS);
    assert(found_destination.price_per_unit == (store.items[0].size / store.items[0].price));

    
    free(store.items);
}

void test_items_filter_items() {

    // Test variables
    int basket_item_count;
    basket_item_s *basket = test_get_basket(&basket_item_count);

    // Create a mock store with 3 items, where the first and third items match the first and fifth items in the basket
    store_s store = {
        .items = malloc(3 * sizeof(store_item_s)),
        .found_items = malloc(2 * sizeof(found_item_s)),
        .missing_items = malloc(3 * sizeof(basket_item_s)),
        .items_count = 3
    };

    store.items[0] = (store_item_s){
        .name = "Milk",
        .size = 1,
        .unit = LITERS,
    };

    store.items[1] = (store_item_s){
        .name = "Not in basket",
        .size = 5,
        .unit = UNITS,
    };

    store.items[2] = (store_item_s){
        .name = "Pudding",
        .size = 5,
        .unit = UNITS,
    };

    // Call the function being tested
    items_filter_items(&store);

    // Verify the item counts
    assert(store.found_items_count == 2);
    assert(store.missing_items_count == 3);

    // Verify the individual items
    assert(strcmp(store.found_items[0].name, "Milk") == 0);
    assert(strcmp(store.found_items[1].name, "Pudding") == 0);

    assert(strcmp(store.missing_items[0].name, "Beef") == 0);
    assert(strcmp(store.missing_items[1].name, "Noodles") == 0);
    assert(strcmp(store.missing_items[2].name, "Snickers") == 0);

    free(store.items);
    free(store.found_items);

}

// Run all tests
int main()
{
    test_items_compare_item_names();
    test_items_compare_item_units();
    test_items_compare_item_price_per_unit();
    test_items_alternate_unit_match();
    test_items_convert_to_found_item();
    test_items_add_item_to_found_and_missing();
    test_items_is_in_variation();
    test_items_find_best_match();
    test_items_filter_items();

    return 0;
}
