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
    double price_per_unit
} item_s;

typedef enum
{
    KVICKLY,
    FAKTA
} store_chain_e;

typedef struct
{
    char uid[37]; // Exact size of a UUID, as used by salling. Coop just uses 3-digit ints which we cast to strings
    char name[50];
    char address[50]; // Denmarks longest street name is 27 characters, so this is plenty https://stiften.dk/artikel/aarhus-har-landets-l%C3%A6ngste-vejnavne
    double coords[2]; // [lat, long]
    store_chain_e chain;
    int distance; // in meters
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