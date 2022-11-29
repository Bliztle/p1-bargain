#ifndef P1_BARGAIN_ITEMS_TYPES_H
#define P1_BARGAIN_ITEMS_TYPES_H

#define ITEM_NAME_SIZE 100 
#define STORE_NAME_SIZE 50
#define ADDRESS_SIZE 50 // Denmarks longest street name is 27 characters, so this is plenty https://stiften.dk/artikel/aarhus-har-landets-l%C3%A6ngste-vejnavne
#define SALLING_UUID_SIZE 37 // Exact size of a UUID, as used by salling. Coop just uses 3-digit ints which we cast to strings

typedef enum
{
    KILOGRAMS,
    LITERS,
    UNITS
} item_unit_e;

typedef struct
{
    char name[ITEM_NAME_SIZE];
    double price;
    double size;
    item_unit_e unit;
    double price_per_unit;
} store_item_s;

typedef struct
{
    char name[ITEM_NAME_SIZE];
    double total_price;
    double product_price;
    int count;
    double size;
    item_unit_e unit;
    double price_per_unit;
} found_item_s;

typedef struct {
    char name[ITEM_NAME_SIZE];
    double size;
    item_unit_e unit;
} basket_item_s;

typedef enum
{
    KVICKLY,
    FAKTA,
    COOP,
    SALLING
} store_chain_e;

typedef struct
{
    char uid[SALLING_UUID_SIZE]; 
    char name[STORE_NAME_SIZE];
    char address[ADDRESS_SIZE]; 
    double lat;
    double lon;
    store_chain_e chain;
    store_item_s *items;
    int items_count;

    int distance; // in meters

    basket_item_s *missing_items;
    found_item_s *found_items;
    int missing_items_count;
    int found_items_count;
    double found_items_total_price;

} store_s;



#endif //P1_BARGAIN_ITEMS_TYPES_H
