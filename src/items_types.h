#ifndef P1_BARGAIN_ITEMS_TYPES_H
#define P1_BARGAIN_ITEMS_TYPES_H

#define ITEM_NAME_SIZE 100
#define STORE_NAME_SIZE 50
#define STORE_ADDRESS_SIZE 50 // Denmarks longest street name is 27 characters, so this is plenty https://stiften.dk/artikel/aarhus-har-landets-l%C3%A6ngste-vejnavne
#define STORE_UID_SIZE 37 // Exact size of a UUID, as used by salling. Coop just uses 3-digit ints which we cast to strings

typedef double coord_lat_t;
typedef double coord_lon_t;

typedef char item_name_t[ITEM_NAME_SIZE];
typedef double item_size_t;
typedef double item_price_t;
typedef char store_address_t[STORE_ADDRESS_SIZE];
typedef char store_name_t[STORE_NAME_SIZE];
typedef item_price_t store_price_t;
typedef char store_uid_t[STORE_UID_SIZE];

typedef enum
{
    KILOGRAMS,
    LITERS,
    UNITS
} item_unit_e;

typedef enum
{
    SALLING,
    COOP
} store_group_e;

typedef enum
{
    STORE_GROUP_BILKA,

    STORE_GROUP_SUPER_BRUGSEN,
    STORE_GROUP_KVICKLY,
    STORE_GROUP_DAGLI_BRUGSEN,
    STORE_GROUP_FAKTA, // COOP 365 falls in this category, due to the out-facing of Fakta
    STORE_GROUP_FAKTA_GERMANY,
    STORE_GROUP_IRMA,
    STORE_GROUP_GROENLAND,
    STORE_GROUP_COOP_DK, // coop.dk
    STORE_GROUP_FK,
    STORE_GROUP_COOP_MAD
} store_chain_e;

typedef struct
{
    item_name_t name;
    item_price_t price;
    item_size_t size;
    item_unit_e unit;
    item_price_t price_per_unit;
} store_item_s;

typedef struct
{
    item_name_t name;
    item_price_t total_price;
    item_price_t product_price;
    int count;
    item_price_t size;
    item_unit_e unit;
    item_price_t price_per_unit;
} found_item_s;

typedef struct {
    item_name_t name;
    item_size_t size;
    item_unit_e unit;
} basket_item_s;


typedef struct
{
    store_uid_t uid; 
    store_name_t name;
    store_address_t address; 
    coord_lat_t lat;
    coord_lon_t lon;
    store_chain_e chain;
    store_group_e group;
    store_item_s *items;
    int items_count;
    int distance; // in meters

    store_price_t total_price;
    basket_item_s *missing_items;
    found_item_s *found_items;
    int missing_items_count;
    int found_items_count;

} store_s;



#endif //P1_BARGAIN_ITEMS_TYPES_H
