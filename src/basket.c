#include "stdio.h"
#include "basket.h"
#include "items_types.h"
#include "config.h"

basket_s *basket_new(basket_item_s item) {
    basket_s *basket = malloc(sizeof(basket_s));
    basket->item = item;
    basket->next = NULL;
    return basket;
}

void basket_push(basket_s *basket, basket_item_s item) {
    // find the last element in the list
    basket_s *tail = basket;
    for (; tail->next != NULL; tail = tail->next);
    basket_s *new_item = basket_new(item);
    tail->next = new_item;
}

void basket_remove(basket_s *basket, size_t index) {
    basket_s *element = basket;
    basket_s *prev = NULL;
    for (int i = 0; i < index; i++) {
        prev = element;
        if (basket != NULL) {
            element = basket->next;
        }
    }
    prev->next = element->next;
    free(element);
}

void basket_remove_first(basket_s *basket) {
    basket_s *old_second = basket->next;
    basket_s new_head = *old_second;
    *basket = new_head;
    free(old_second);
}

void basket_free(basket_s *basket) {
    basket_s *element = basket;
    basket_s *prev = NULL;
    for (; element != NULL; element = element->next) {
        if (prev != NULL) {
            free(prev);
        }
        prev = element;
    }
    if (prev != NULL) {
        free(prev);
    }
}

int basket_to_array(basket_s *basket, basket_item_s **items) {
    int length = 0;
    basket_s *element = basket;
    while (element != NULL) {
        length++;
        element = element->next;
    }

    *items = malloc(length * sizeof(basket_item_s));
    element = basket;
    for (int i = 0; i < length; i++) {
        *items[i] = element->item;
        element = element->next;
    }
    return length;
}

basket_s *basket_read() {
    conf_settings_s settings;
    conf_read_settings(&settings);
    char *basket_path = settings.shopping_list_save_path;

    FILE *file = fopen(basket_path, "r");

    basket_item_s item;
    // Starting the list with an empty item is a bit hacky, but we remove it later
    basket_s *basket = basket_new(item);

    while (!feof(file)) {
        fscanf(file, " %s|%lf|%d\n", item.name, &item.size, &item.unit);
        basket_push(basket, item);
    }

    basket_remove_first(basket);

    fclose(file);

    return basket;
}

void basket_write(basket_s* basket) {
    conf_settings_s settings;
    conf_read_settings(&settings);
    char* basket_path = settings.shopping_list_save_path;

    FILE* file = fopen(basket_path, "r");

    basket_s* element = basket;
    for (; element != NULL; element = element->next) {
        fprintf(file, "%s|%lf|%d\n", element->item.name, element->item.size, element->item.unit);
    }

    fclose(file);
}
