/*
Look into NXJSON or cJSON for parsing api responses
https://stackoverflow.com/a/16490394
*/
#include "parse.h"
#include "fetch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nxjson/nxjson.h"
#include <regex.h>
#include <math.h>

static const char *store_chain_map[] = {
    [STORE_CHAIN_BILKA] = "bilka",

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

        store->chain = STORE_CHAIN_BILKA;
        store->group = SALLING;
        store->distance = nx_json_get(json_store, "distance_km")->num.dbl_value * 1000;
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
        // Skip stores for which COOP doesn't return items
        int store_uid = (int)nx_json_get(json_store, "Kardex")->num.u_value;
        if (store_uid < 10000)
            continue;

        *stores = realloc(*stores, ++count * sizeof(store_s));
        store_s *store = &(*stores)[count - 1];

        store->group = COOP;
        store->distance = 0;
        snprintf(store->uid, STORE_UID_SIZE, "%d", store_uid);
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
        puts("Something went wrong searching for wares. Not everything will show up");
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
        if ((size = parse_try_extract_size(item->name, g_unit_strs[i])))
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
    double count = count_str ? strtod(count_str, NULL) : 0;
    free(count_str);
    return count;
}

char *parse_replace_char(char *source, char find, char replace)
{
    for (char *ptr = strchr(source, find);
         ptr;
         ptr = strchr(source, find))
        *ptr = replace;
    return source;
}

// Shamelessly stolen from https://stackoverflow.com/a/779960
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

int parse_replace_all_str(char **haystack, char *find, char *replace) {
    char *new = str_replace(*haystack, find, replace);
    *haystack = realloc(*haystack, strlen(new) + 1);
    strcpy(*haystack, new);
}

// Inspired by https://stackoverflow.com/a/11864144
char *parse_try_regex_group(char *source, char *regex)
{
    size_t maxGroups = 2;
    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];

    // Match on encoded strings, as REG_ICASE doesn't work on UTF-8
    char *_source = to_old_danish(source);
    char *_regex = to_old_danish(regex);

    char *group = NULL;

    if (regcomp(&regexCompiled, _regex, REG_EXTENDED | REG_ICASE))
    {
        free(_source);
        free(_regex);
        return NULL;
    };

    if (regexec(&regexCompiled, _source, maxGroups, groupArray, 0) == 0 && // Successfull regex execution
        groupArray[1].rm_so != (size_t)-1)                                // Matched on group
    {
        // Extract match using start / end indexes
        char sourceCopy[strlen(source) + 1];
        strcpy(sourceCopy, source);
        sourceCopy[groupArray[1].rm_eo] = '\0';

        int l = strlen(sourceCopy + groupArray[1].rm_so) + 1;
        group = malloc(l * sizeof(char));
        strcpy(group, sourceCopy + groupArray[1].rm_so);
        group[l - 1] = '\0';
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

int parse_coordinates(conf_address_t *formatted_address, double *lat, double *lon, char *raw_coordinates) {
    const nx_json* json = nx_json_parse_utf8(raw_coordinates);

    const nx_json* results = nx_json_get(json, "results");

    if (results->children.length == 0) return 0;

    // Save the first result
    const nx_json* result_first_child = results->children.first;
    const nx_json* result_formatted_address = result_first_child->children.first->next;
    


    // Get value of lat and long and put them into the settings struct
    *lat = nx_json_get(nx_json_get(nx_json_get(result_first_child, "geometry"), "location"), "lat")->num.dbl_value;
    *lon = nx_json_get(nx_json_get(nx_json_get(result_first_child, "geometry"), "location"), "lng")->num.dbl_value;

    if(formatted_address != NULL) {
        strcpy(*formatted_address, result_formatted_address->text_value);
        printf("\nIf the found address is wrong, then please try again in the settings menu\nFound the following address:\n%s\n\n", *formatted_address);
    }

    // Cleanup
    nx_json_free(json);
    json = NULL; // No dangling pointer

    return 1;
}
