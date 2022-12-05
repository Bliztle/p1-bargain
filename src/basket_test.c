#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "items_types.h"
#include "basket.h"

static const double EPSILON = 0.001;

int double_approx_equal(double a, double b, double epsilon) {
    return fabs(a-b) < epsilon;
}

int basket_items_equal(basket_item_s a, basket_item_s b) {
    // `strcmp` returns 0 if the strings are equal
    return !strcmp(a.name, b.name)
        && double_approx_equal(a.size, b.size, EPSILON)
        && a.unit == b.unit;
}


int basket_test_new() {
    basket_item_s first = {
        .name = "Milk",
        .size = 1.0,
        .unit = LITERS,
    };

    basket_s* basket = basket_new(first);

    if (!basket_items_equal(basket->item, first)) {
        return 1;
    }

    if (basket->next != NULL) {
        return 2;
    }

    basket_free(basket);

    return 0;
}

int basket_test_push() {
    basket_item_s first = {
        .name = "Milk",
        .size = 1.0,
        .unit = LITERS,
    };
    basket_item_s second = {
        .name = "Eggs",
        .size = 10.0,
        .unit = UNITS,
    };
    basket_item_s third = {
        .name = "Butter",
        .size = 0.2,
        .unit = KILOGRAMS,
    };

    basket_s* basket = basket_new(first);

    basket_push(basket, second);

    if (!basket_items_equal(basket->next->item, second)) {
        return 1;
    }

    basket_push(basket, third);

    if (!basket_items_equal(basket->next->item, second)) {
        return 2;
    }
    if (!basket_items_equal(basket->next->next->item, third)) {
        return 3;
    }

    basket_free(basket);

    return 0;
}

int basket_test_iterate() {
    basket_item_s first = {
        .name = "Milk",
        .size = 1.0,
        .unit = LITERS,
    };
    basket_item_s second = {
        .name = "Eggs",
        .size = 10.0,
        .unit = UNITS,
    };
    basket_item_s third = {
        .name = "Butter",
        .size = 0.2,
        .unit = KILOGRAMS,
    };

    basket_item_s elements[] = {first, second, third};
    int element_count = sizeof(elements)/sizeof(basket_item_s);

    basket_s* basket = basket_new(first);

    basket_push(basket, second);
    basket_push(basket, third);

    basket_s* element = basket;
    int i = 0;
    for (; element != NULL; element = element->next) {
        if (!basket_items_equal(element->item, elements[i])) {
            return i+1;
        }
        i++;
    }

    if (i != element_count) {
        return element_count;
    }

    basket_free(basket);

    return 0;
}

int basket_test_remove_middle() {

    basket_item_s first = {
        .name = "Milk",
        .size = 1.0,
        .unit = LITERS,
    };
    basket_item_s second = {
        .name = "Eggs",
        .size = 10.0,
        .unit = UNITS,
    };
    basket_item_s third = {
        .name = "Butter",
        .size = 0.2,
        .unit = KILOGRAMS,
    };

    basket_s* basket = basket_new(first);

    basket_push(basket, second);
    basket_push(basket, third);

    basket_remove(&basket, 1);

    if (!basket_items_equal(basket->item, first)) {
        return 1;
    }
    if (!basket_items_equal(basket->next->item, third)) {
        return 2;
    }

    return 0;
}

int main() {
    int basket_new_test     = !basket_test_new();
    int basket_push_test    = !basket_test_push();
    int basket_iterate_test = !basket_test_iterate();
    int basket_remove_test  = !basket_test_remove_middle();
    assert(basket_new_test);
    assert(basket_push_test);
    assert(basket_iterate_test);
    assert(basket_remove_test);
}
