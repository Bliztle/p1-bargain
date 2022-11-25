#ifndef PARSE_H
#define PARSE_H

typedef struct
{
    char id[20];
    char prod_id[20];
    char title[100];
    char description[100];
    char img[100];
    char link[100];
    double price;
} salling_relevant_products_suggestion;

typedef struct
{
    salling_relevant_products_suggestion *suggestions;
    int length;
} salling_relevant_products;

salling_relevant_products *parse_salling_relevant_products(char *json_string);
void free_salling_relevant_products(salling_relevant_products *products);
void print_salling_relevant_products(salling_relevant_products *products);

#endif