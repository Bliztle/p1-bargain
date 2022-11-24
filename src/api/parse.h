typedef enum
{
    KILOGRAMS,
    LITERS,
    UNITS
} item_unit_e;

typedef struct
{
    char name[100];
    double price;
    double size;
    item_unit_e unit;
    double price_per_unit;
} item_s;



typedef enum
{
    KVICKLY,
    FAKTA
} store_chain_e;

typedef struct
{
    char name[100];
    double product_price;
    double total_price;
    double size;
    item_unit_e unit;
    int count;
    double price_per_unit;
} store_item_s;


typedef enum
{
    SALLING,
    COOP
} store_group_e;


typedef char store_uid_t[37];
typedef char store_name_t[50];
typedef char store_address_t[50];
typedef double coord_lat_t;
typedef double coord_lon_t;
typedef struct
{
    store_uid_t uid; // Exact size of a UUID, as used by salling. Coop just uses 3-digit ints which we cast to strings
    store_name_t name;
    store_address_t address[50]; // Denmarks longest street name is 27 characters, so this is plenty https://stiften.dk/artikel/aarhus-har-landets-l%C3%A6ngste-vejnavne
    coord_lat_t lat;
    coord_lon_t lon;
    store_group_e group;
    store_chain_e chain;
    int distance; // in meters

    store_item_s *items;
    int item_count;

    store_item_s *missing_items;
    int missing_items_count;
    double total_price;

} store_s;


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