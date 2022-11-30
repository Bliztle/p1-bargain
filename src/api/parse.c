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
        snprintf(store->uid, STORE_UID_SIZE, "%d", (int)nx_json_get(json_store, "Kardex")->num.u_value);
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

int parse_coop_items(const nx_json *json, store_item_s **items)
{
    *items = NULL;
    int count = 0;
    for (const nx_json *json_item = json->children.first;
         json_item != NULL;
         json_item = json_item->next)
    {
        *items = realloc(*items, ++count * sizeof(store_item_s));
        // store_item_s *items = &((*items)[count - 1]);

        strncpy((*items)[count-1].name, nx_json_get(json_item, "Navn")->text_value, ITEM_NAME_SIZE);
        (*items)[count-1].price = nx_json_get(json_item, "Pris")->num.dbl_value;

        // TODO: Write function looking thought names to get these
        (*items)[count-1].unit = UNKNOWN;
        (*items)[count-1].size = 0;
        (*items)[count-1].price_per_unit = 0;
    }

    return count;
}

// Heavily inspired https://stackoverflow.com/a/2029227
int parse_read_file_to_end(char *file_name, char **content)
{
    FILE *fp = fopen(file_name, "r");

    if (fp == NULL)
        return 1;

    /* Go to the end of the file. */
    if (fseek(fp, 0L, SEEK_END) == 0)
    {
        /* Get the size of the file. */
        long bufsize = ftell(fp);
        if (bufsize == -1) /* Error */
            return 1;

        /* Allocate our buffer to that size. */
        *content = realloc(*content, sizeof(char) * (bufsize + 1));

        /* Go back to the start of the file. */
        if (fseek(fp, 0L, SEEK_SET) != 0) /* Error */
        {
            free(content);
            return 1;
        }

        /* Read the entire file into memory. */
        size_t newLen = fread(*content, sizeof(char), bufsize, fp);
        if (ferror(fp) != 0)
        {
            free(content);
            return 1;
        }

        (*content)[newLen++] = '\0'; /* Just to be safe. */
    }
    fclose(fp);

    return 0;
}