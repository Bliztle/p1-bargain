#include <malloc.h>
#include <assert.h>
#include <string.h>

#include "mock_functions.h"
#include "bargain.h"
#include "items_types.h"

void test_bargain_get_unit() {
    assert(strcmp(bargain_get_unit(0), "UNKNOWN") == 0);
    assert(strcmp(bargain_get_unit(1), "KILOGRAMS") == 0);
    assert(strcmp(bargain_get_unit(2), "LITERS") == 0);
    assert(strcmp(bargain_get_unit(3), "UNITS") == 0);
}

void test_get_size_of_list_entries() {

    // Create a store object with some found and missing items
    store_s store;
    store.found_items_count = 2;
    store.missing_items_count = 3;
    store.found_items = malloc(2 * sizeof(found_item_s));
    store.missing_items = malloc(3 * sizeof(basket_item_s));

    found_item_s found_item_one = {
        .name = "apple",
    };
    found_item_s found_item_two = {
        .name = "banana",
    };
    basket_item_s missing_item_one = {
        .name = "orange",
    };
    basket_item_s missing_item_two = {
        .name = "grapes",
    };
    basket_item_s missing_item_three = {
        .name = "watermelon",
    };

    store.found_items[0] = found_item_one;
    store.found_items[1] = found_item_two;
    store.missing_items[0] = missing_item_one;
    store.missing_items[1] = missing_item_two;
    store.missing_items[2] = missing_item_three;

    int found_item_size_sum = (strlen(found_item_one.name) + strlen(found_item_two.name)) * sizeof(char);
    int missing_item_size_sum = (strlen(missing_item_one.name) + strlen(missing_item_two.name) + strlen(missing_item_three.name)) * sizeof(char);


    // Initialize found_list_size and missing_list_size
    int found_list_size = 0;
    int missing_list_size = 0;

    // Call get_size_of_list_entries()
    get_size_of_list_entries(store, &found_list_size, &missing_list_size);

    // Check that the correct values for found_list_size and missing_list_size are returned
    assert(found_list_size == found_item_size_sum);
    assert(missing_list_size == missing_item_size_sum);

    // Free memory
    free(store.found_items);
    free(store.missing_items);

}

void test_append_outro_to_string() {

    store_s store = (store_s){
        .name = "Fakta",
        .address = "Testvej 01 7000 Fredericia",
        .distance = 6500,
        .found_items_count = 10,
        .missing_items_count = 5,
        .found_items_total_price = 42.42,
    };


    size_t size_of_test_string = strlen("TEST APPEND STRING:") * sizeof(char);
    char* test_string_to_append_to = malloc(size_of_test_string);
    strcpy(test_string_to_append_to, "TEST APPEND STRING:");

    append_outro_to_string(store, &test_string_to_append_to, size_of_test_string);

    char* expected_string = "TEST APPEND STRING:--------------------------------------\nStore | Address | Distance | Items found | Total price\nFakta | Testvej 01 7000 Fredericia | 6500 | 10/15 | 42.42dkk\n";
    int expected_length = strlen(expected_string);
    int expected_size = sizeof(char) * expected_length;

    int recieved_length = strlen(test_string_to_append_to);
    int recieved_size = sizeof(char) * recieved_length;

    assert(!strcmp(test_string_to_append_to, expected_string));
    assert(expected_length == recieved_length);
    assert(expected_size == recieved_size);

    free(test_string_to_append_to);
}

void test_create_missing_entries() {
    store_s store = (store_s){
        .missing_items_count = 2,
        .missing_items = malloc(2 * sizeof(basket_item_s)),
    };

    basket_item_s missing_item_one = (basket_item_s) {
        .name = "Potatoes"
    };
    basket_item_s missing_item_two = (basket_item_s) {
        .name = "Boil em'"
    };

    store.missing_items[0] = missing_item_one;
    store.missing_items[1] = missing_item_two;


    size_t size_of_test_string = strlen("TEST APPEND STRING:") * sizeof(char);
    char* test_string_to_append_to = malloc(size_of_test_string);
    strcpy(test_string_to_append_to, "TEST APPEND STRING:");

    create_missing_entries(store, &test_string_to_append_to, size_of_test_string);

    char* expected_string = "TEST APPEND STRING:--------------------------------------\nUNAVAILABLE ITEMS\n--------------------------------------\n# | Product\n1 | Potatoes\n2 | Boil em'\n";
    int expected_length = strlen(expected_string);
    int expected_size = sizeof(char) * expected_length;

    int recieved_length = strlen(test_string_to_append_to);
    int recieved_size = sizeof(char) * recieved_length;

    assert(!strcmp(test_string_to_append_to, expected_string));
    assert(expected_length == recieved_length);
    assert(expected_size == recieved_size);

    free(test_string_to_append_to);
}

void test_create_found_entries() {
    store_s store = (store_s){
        .found_items_count = 2,
        .found_items = malloc(2 * sizeof(found_item_s)),
    };

    found_item_s found_item_one = (found_item_s) {
        .name = "Potatoes",
        .count = 5,
        .unit = UNITS,
        .total_price = 20,
        .price_per_unit = 4,
    };
    found_item_s found_item_two = (found_item_s) {
        .name = "Boil em'",
        .count = 1,
        .unit = LITERS,
        .total_price = 1,
        .price_per_unit = 1
    };

    store.found_items[0] = found_item_one;
    store.found_items[1] = found_item_two;


    size_t size_of_test_string = strlen("TEST APPEND STRING:") * sizeof(char);
    char* test_string_to_append_to = malloc(size_of_test_string);
    strcpy(test_string_to_append_to, "TEST APPEND STRING:");
    
    char* expected_string = "TEST APPEND STRING:SHOPPING LIST\n--------------------------------------\n# | Product | count | price/unit | total price\n1 | Potatoes | 5 | 4.00 dkk./UNITS | 20.00 dkk.\n2 | Boil em' | 1 | 1.00 dkk./LITERS | 1.00 dkk.\n";
    int expected_length = strlen(expected_string);
    int expected_size = sizeof(char) * expected_length;   


    create_found_entries(store, &test_string_to_append_to, size_of_test_string);


    printf("TEST DEBUG INFO\n");
    printf("Expected: '%s'\n", expected_string);
    printf("Recieved: '%s'\n", test_string_to_append_to);
    printf("TEST DEBUG INFO END\n");

    int recieved_length = strlen(test_string_to_append_to);
    int recieved_size = sizeof(char) * recieved_length;



    assert(!strcmp(test_string_to_append_to, expected_string));
    assert(expected_length == recieved_length);
    assert(expected_size == recieved_size);

    free(test_string_to_append_to);
    
}




int main() {
    printf("Testing Bargain\n");
    printf("Bargain Test 1: test_bargain_get_unit()\n");
    test_bargain_get_unit();
    printf("Bargain Test 2: test_get_size_of_list_entries()\n");
    test_get_size_of_list_entries();
    printf("Bargain Test 3: test_append_outro_to_string()\n");
    test_append_outro_to_string();
    printf("Bargain Test 4: test_create_missing_entries()\n");
    test_create_missing_entries();
    printf("Bargain Test 5: test_create_found_entries()\n");
    test_create_found_entries();
    printf("Testing Bargain Done\n");

    return 0;
}