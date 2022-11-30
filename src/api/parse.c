/*
Look into NXJSON or cJSON for parsing api responses
https://stackoverflow.com/a/16490394
*/
#include "parse.h"
#include "../items_types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nxjson/nxjson.h"

static const char *store_chain_map[] = {
    [STORE_GROUP_BILKA] = "bilka",

    [STORE_GROUP_SUPER_BRUGSEN] = "SuperBrugsen",
    [STORE_GROUP_KVICKLY] = "Kvickly",
    [STORE_GROUP_DAGLI_BRUGSEN] = "Dagli'Brugsen",
    [STORE_GROUP_FAKTA] = "Fakta",
    [STORE_GROUP_FAKTA_GERMANY] = "FaktaGermany",
    [STORE_GROUP_IRMA] = "Irma",
    [STORE_GROUP_GROENLAND] = "Grønland",
    [STORE_GROUP_COOP_DK] = "Coop.dk",
    [STORE_GROUP_FK] = "FK",
    [STORE_GROUP_COOP_MAD] = "COOP MAD",
    NULL,
};

int parse_salling_stores(char *raw_stores, store_s **stores)
{
    const nx_json *json = nx_json_parse_utf8(raw_stores);
    int count = 0;

    nx_json *json_store = json->children.first;
    while (json_store != NULL)
    {
        *stores = realloc(*stores, ++count * sizeof(store_s));
        store_s *store = &(*stores)[count - 1];

        store->chain = STORE_GROUP_BILKA;
        store->group = SALLING;
        store->distance = nx_json_get(json_store, "distance_km")->num.dbl_value;
        strncpy(store->uid, nx_json_get(json_store, "id")->text_value, STORE_UID_SIZE);
        strncpy(store->name, nx_json_get(json_store, "name")->text_value, STORE_NAME_SIZE);

        const nx_json *coordinates = nx_json_get(json_store, "coordinates");
        store->lat = coordinates->children.last->num.dbl_value;
        store->lon = coordinates->children.first->num.dbl_value;

        const nx_json *address = nx_json_get(json_store, "address");
        snprintf((char *)store->address, STORE_ADDRESS_SIZE, "%s, %s %s",
                 nx_json_get(address, "street")->text_value,
                 nx_json_get(address, "zip")->text_value,
                 nx_json_get(address, "city")->text_value);

        json_store = json_store->next;
    }

    return count;
}

int parse_coop_stores(char *raw_stores, store_s **stores)
{
    const nx_json *json = nx_json_parse_utf8(raw_stores);
    int count = 0;

    for (nx_json *json_store = nx_json_get(json, "Data")->children.first;
         json_store != NULL;
         json_store = json_store->next)
    {
        *stores = realloc(*stores, ++count * sizeof(store_s));
        store_s *store = &(*stores)[count - 1];

        store->group = SALLING;
        store->distance = 0; // TODO: Add distance, possibly doing something while filtering on it
        snprintf(store->uid, STORE_UID_SIZE, "%d", (int)nx_json_get(json_store, "StoreId")->num.u_value);
        strncpy(store->name, nx_json_get(json_store, "Name")->text_value, STORE_NAME_SIZE);

        const nx_json *coordinates = nx_json_get(nx_json_get(json_store, "Location"), "coordinates");
        store->lat = coordinates->children.last->num.dbl_value;
        store->lon = coordinates->children.first->num.dbl_value;

        const char *retail_group = nx_json_get(json_store, "RetailGroupName")->text_value;
        for (int i = 0; store_chain_map[i] != NULL; i++)
        {
            if (strcmp(retail_group, store_chain_map[i]) == 0)
            {
                store->chain = i;
                break;
            }
        }

        snprintf((char *)store->address, STORE_ADDRESS_SIZE, "%s, %d %s",
                 nx_json_get(json_store, "Address")->text_value,
                 (int)nx_json_get(json_store, "Zipcode")->num.u_value,
                 nx_json_get(json_store, "City")->text_value);
    }

    return count;
}

void parse_coordinates(char* raw_coordinates) {
    printf("Raw coords: %s", raw_coordinates);

    const nx_json* json = nx_json_parse_utf8(raw_coordinates);
    
    printf("%s", raw_coordinates);



    nx_json_free(json);
}