/*
Look into NXJSON or cJSON for parsing api responses
https://stackoverflow.com/a/16490394
*/
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include <string.h>
#include "nxjson/nxjson.h"

// TODO: Remove this when proper functions have been implemented
salling_relevant_products *parse_salling_relevant_products(char *json_string)
{
    const nx_json *json = nx_json_parse_utf8(json_string);
    const nx_json *json_suggestions = nx_json_get(json, "suggestions");
    printf("Suggestion length: %i\n", json_suggestions->children.length);

    salling_relevant_products *products = malloc(sizeof(salling_relevant_products));
    if (products == NULL)
    {
        printf("Out of memory at %s:%i", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    products->length = json_suggestions->children.length;
    products->suggestions = malloc(sizeof(salling_relevant_products_suggestion) * json_suggestions->children.length);
    if (products->length > 0 && products == NULL)
    {
        printf("Out of memory at %s:%i", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    // nxjson does have a function to get an element at an index, but it simply traverses a list, making this more effective
    nx_json *json_product = json_suggestions->children.first;
    for (int i = 0; i < products->length; i++)
    {
        strcpy(products->suggestions[i].id, nx_json_get(json_product, "id")->text_value);
        strcpy(products->suggestions[i].prod_id, nx_json_get(json_product, "prod_id")->text_value);
        strcpy(products->suggestions[i].title, nx_json_get(json_product, "title")->text_value);
        strcpy(products->suggestions[i].description, nx_json_get(json_product, "description")->text_value);
        strcpy(products->suggestions[i].img, nx_json_get(json_product, "img")->text_value);
        strcpy(products->suggestions[i].link, nx_json_get(json_product, "link")->text_value);
        products->suggestions[i].price = nx_json_get(json_product, "price")->num.dbl_value;
        json_product = json_product->next;
    }

    return products;
}

// TODO: Remove this when proper functions have been implemented
void free_salling_relevant_products(salling_relevant_products *products)
{
    free(products->suggestions);
    free(products);
}

// TODO: Remove this when proper functions have been implemented
void print_salling_relevant_products(salling_relevant_products *products)
{
    printf("\nPrinting relevant products from salling\n");
    for (int i = 0; i < products->length; i++)
    {
        printf(
            "[%i]\n"
            "[id]: %s\n"
            "[prod_id]: %s\n"
            "[title]: %s\n"
            "[description]: %s\n"
            "[img]: %s\n"
            "[link]: %s\n"
            "[price]: %.2f\n\n",
            i,
            products->suggestions[i].id,
            products->suggestions[i].prod_id,
            products->suggestions[i].title,
            products->suggestions[i].description,
            products->suggestions[i].img,
            products->suggestions[i].link,
            products->suggestions[i].price);
    }
}