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
#include <regex.h>
#include <math.h>

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
        if (*items == NULL)
        {
            printf("Failed to allocate memory for items");
            // Handle the error by returning an error code or logging a message
            return -1;
        }
        store_item_s *item = (&(*items)[count - 1]);

        strncpy(item->name, nx_json_get(json_item, "Navn")->text_value, ITEM_NAME_SIZE);
        strncat(item->name, "  |  ", ITEM_NAME_SIZE - strlen(item->name));
        strncat(item->name, nx_json_get(json_item, "Navn2")->text_value, ITEM_NAME_SIZE - strlen(item->name));
        item->price = nx_json_get(json_item, "Pris")->num.dbl_value;

        parse_populate_item_unit(item);
    }

    return count;
}

int parse_salling_items(char *raw_items, store_item_s **items)
{
    const nx_json *json = nx_json_parse_utf8(raw_items);

    const nx_json *json_suggestions = nx_json_get(json, "suggestions");
    if (json_suggestions == NULL)
    {
        puts("Something whent wrong searching for wares. Not everything will show up");
        return 0;
    }

    int count = 0;
    for (const nx_json *json_item = json_suggestions->children.first;
         json_item != NULL;
         json_item = json_item->next)
    {
        *items = realloc(*items, ++count * sizeof(store_item_s));
        store_item_s *item = (&(*items)[count - 1]);

        strncpy(item->name, nx_json_get(json_item, "title")->text_value, ITEM_NAME_SIZE);
        item->price = nx_json_get(json_item, "price")->num.dbl_value;

        if (strstr(item->name, "PAPTALLERKEN"))
        {
            int i = 0;
        }

        parse_populate_item_unit(item);
    }

    return count;
}

void parse_populate_item_unit(store_item_s *item)
{
    double size;

    /****** Units ******/
    if ((size = parse_try_extract_size(item->name, "STK")))
    {
        item->size = size;
        item->unit = UNITS;
        item->price_per_unit = item->price / item->size;
        return;
    }

    /****** Liters ******/
    char *l_unit_strs[] = {
        "L",
        "DL",
        "CL",
        "ML",
    };

    for (int i = 0; i < 4; i++)
    {
        if ((size = parse_try_extract_size(item->name, l_unit_strs[i])))
        {
            item->size = size / pow(10, i);
            item->unit = LITERS;
            item->price_per_unit = item->price / item->size;
            return;
        }
    }

    /****** KILOGRAMS ******/
    char *g_unit_strs[] = {
        "KG",
        NULL,
        NULL,
        "G",
    };

    for (int i = 0; i < 4; i++)
    {
        if ((size = parse_try_extract_size(item->name, l_unit_strs[i])))
        {
            item->size = size / pow(10, i);
            item->unit = KILOGRAMS;
            item->price_per_unit = item->price / item->size;
            return;
        }
    }

    item->unit = UNKNOWN;
    item->size = 0;
    item->price_per_unit = 0;
}

// Accepts 7 characters in unit
double parse_try_extract_size(char *source, char *unit_str)
{
    char regex_str[20];
    snprintf(regex_str, 20, "([0-9,.]+)[ ]?%s", unit_str);
    char *count_str = parse_try_regex_group(parse_replace_char(source, ',', '.'), regex_str);
    return count_str ? strtod(count_str, NULL) : 0;
}

char *parse_replace_char(char *source, char find, char replace)
{
    for (char *ptr = strchr(source, find);
         ptr;
         ptr = strchr(source, find))
        *ptr = replace;
    return source;
}

// Inspired by https://stackoverflow.com/a/11864144
char *parse_try_regex_group(char *source, char *regex)
{
    size_t maxGroups = 3;
    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];

    char *group = NULL;

    if (regcomp(&regexCompiled, regex, REG_EXTENDED | REG_ICASE))
    {
        printf("Could not compile regular expression.\n");
        return NULL;
    };

    if (regexec(&regexCompiled, source, maxGroups, groupArray, 0) == 0 && // Successfull regex execution
        groupArray[1].rm_so != (size_t)-1)                                // Matched on group
    {
        // Extract match using start / end indexes
        char sourceCopy[strlen(source) + 1];
        strcpy(sourceCopy, source);
        sourceCopy[groupArray[1].rm_eo] = 0;

        group = malloc(strlen(sourceCopy + groupArray[1].rm_so) * sizeof(char));
        strcpy(group, sourceCopy + groupArray[1].rm_so);
    }

    regfree(&regexCompiled);

    return group;
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