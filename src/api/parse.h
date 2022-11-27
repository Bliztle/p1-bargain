#pragma once

typedef struct
{
    char name[100];
    double total_price;
    double size;
    int count;
    // Replace with Isak and Mette's basket item
    void *unit;
    double price_per_unit;
} store_item_s;

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

typedef enum
{
    SALLING,
    COOP
} store_group_e;

#define STORE_UID_LENGTH 37
#define STORE_NAME_LENGTH 50
#define STORE_ADDRESS_LENGTH 50
typedef char store_uid_t[STORE_UID_LENGTH];
typedef char store_name_t[STORE_NAME_LENGTH];
typedef char store_address_t[STORE_ADDRESS_LENGTH];
typedef double coord_lat_t;
typedef double coord_lon_t;
typedef struct
{
    store_uid_t uid; // Exact size of a UUID, as used by salling. Coop just uses 3-digit ints which we cast to strings
    store_name_t name;
    store_address_t address; // Denmarks longest street name is 27 characters, so this is plenty https://stiften.dk/artikel/aarhus-har-landets-l%C3%A6ngste-vejnavne
    coord_lat_t lat;
    coord_lon_t lon;
    store_group_e group;
    store_chain_e chain;
    int distance; // in meters

    store_item_s *items;
    int item_count;
    // Replace with Isak and Mette's basket item
    void *missing_items;
    int missing_items_count;
    double total_price;

} store_s;

/**
 * Parses a raw json string from Salling's store API to an array of store_s structs
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param raw_stores The raw json to parse to store_s objects
 * @param stores A pointer to a null pointer. This will be modified to point at the stores
 *
 * @returns how many stores were parsed
 */
int parse_salling_stores(char *raw_stores, store_s **stores);

/**
 * Parses a raw json string from COOP's store API to an array of store_s structs
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param raw_stores The raw json to parse to store_s objects
 * @param stores A pointer to a null pointer. This will be modified to point at the stores
 *
 * @returns how many stores were parsed
 */
int parse_coop_stores(char *raw_stores, store_s **stores);