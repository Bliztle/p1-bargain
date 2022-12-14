#include "fetch.h"
#include "parse.h"
#include "nxjson/nxjson.h"
#include "../calc.h"
#include "../items_types.h"
#include "../config.h"
#include "../basket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define COOP_TOKEN "8042a78a1c91463e80140b0cb11b8b47"
#define SALLING_TOKEN "ed1b7976-ca43-4bde-a930-ba3d92935464"

static const char const *token_type_map[] = {
    [FETCH_AUTH_BEARER] = "Authorization: Bearer",
    [FETCH_AUTH_OCP_APIM] = "Ocp-Apim-Subscription-Key:",
};

fetch_status_e fetch_get_no_auth(char *url, char **result)
{
    return fetch_get(url, 0, NULL, result);
}

fetch_status_e fetch_get(char *url, fetch_auth_e token_type, char *token, char **result)
{
    char auth_header[60];
    snprintf(auth_header, 60, "%s %s", token_type_map[token_type], token);

    CURL *curl = curl_easy_init();
    if (!curl)
        return FETCH_STATUS_CURL_ERROR;

    CURLcode res_code;
    fetch_response_s res = {NULL, 0};

    char *_url = encode_danish(url);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, _url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fetch_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&res);

    res_code = curl_easy_perform(curl);

    // Cleanup
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(_url);

    // Check for errors
    switch (res_code)
    {
    case CURLE_OK: // We good, continue
        break;
    case CURLE_AUTH_ERROR:
        return FETCH_STATUS_UNAUTHORIZED;
    default:
        return FETCH_STATUS_UNKNOWN_ERROR;
    }

    *result = res.response;

    return FETCH_STATUS_SUCCESS;
}

size_t fetch_write_callback(char *buffer, size_t size, size_t buffer_length, void *prelim_response)
{
    size_t realsize = size * buffer_length;
    fetch_response_s *mem = (fetch_response_s *)prelim_response;

    // Allocate memory for existing response plus buffer
    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL)
        return 0;

    // Append response
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), buffer, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

char *to_old_danish(char *url)
{
    char *buff = malloc(strlen(url) + 1);
    strcpy(buff, url);

    char *ptr;
    while ((ptr = strstr(buff, "??")) != NULL)
    {
        ptr[0] = 'a';
        ptr[1] = 'e';
    }
    while ((ptr = strstr(buff, "??")) != NULL)
    {
        ptr[0] = 'o';
        ptr[1] = 'e';
    }
    while ((ptr = strstr(buff, "??")) != NULL)
    {
        ptr[0] = 'a';
        ptr[1] = 'a';
    }
    while ((ptr = strstr(buff, "??")) != NULL)
    {
        ptr[0] = 'A';
        ptr[1] = 'E';
    }
    while ((ptr = strstr(buff, "??")) != NULL)
    {
        ptr[0] = 'O';
        ptr[1] = 'E';
    }
    while ((ptr = strstr(buff, "??")) != NULL)
    {
        ptr[0] = 'A';
        ptr[1] = 'A';
    }
    return buff;
}

char *_encode_danish(char *url)
{
    int buff_length = strlen(url) + 1;
    char *buff = malloc(buff_length);
    strncpy(buff, url, buff_length);

    char *buff_ptr;
    parse_replace_all_str(&buff, "??", "%C3%A6");
    parse_replace_all_str(&buff, "??", "%C3%B8");
    parse_replace_all_str(&buff, "??", "%C3%A5");
    parse_replace_all_str(&buff, "??", "%C3%86");
    parse_replace_all_str(&buff, "??", "%C3%98");
    parse_replace_all_str(&buff, "??", "%C3%85");
    parse_replace_all_str(&buff, " ", "%20");
    return buff;
}

char *encode_danish(char *url) {
    // return to_old_danish(url);
    return _encode_danish(url);
}

fetch_status_e fetch_renew_stores()
{
    store_s *stores = NULL;
    int count = 0;
    fetch_renew_coop_stores(&stores, &count);
    fetch_renew_salling_stores(&stores, &count);
    _fetch_write_stores(stores, count);

    return FETCH_STATUS_SUCCESS;
}

fetch_status_e fetch_renew_coop_stores(store_s **stores, int *count)
{
    conf_settings_s conf;
    conf_read_settings(&conf);

    char *url = "https://api.cl.coop.dk/storeapi/v1/stores?page=1&size=5000";

    char *raw_stores = NULL;
    fetch_status_e status = fetch_get(url, FETCH_AUTH_OCP_APIM, COOP_TOKEN, &raw_stores);

    if (status != FETCH_STATUS_SUCCESS)
    {
        if (status != FETCH_STATUS_CURL_ERROR)
            free(raw_stores);
        return status;
    }

    store_s *parsed_stores = NULL;
    int new_count = parse_coop_stores(raw_stores, &parsed_stores);

    for (int i = 0; i < new_count; i++)
    {
        double distance = calc_coordinate_distance(conf.address_lat, conf.address_lon, parsed_stores[i].lat, parsed_stores[i].lon) * 1000;
        if (distance < conf.max_distance)
        {
            *stores = realloc(*stores, (++(*count)) * sizeof(store_s));
            parsed_stores[i].distance = distance;
            (*stores)[(*count) - 1] = parsed_stores[i];
            (*stores)[*count - 1] = parsed_stores[i];
        }
    }

    free(raw_stores);

    return FETCH_STATUS_SUCCESS;
}

fetch_status_e fetch_renew_salling_stores(store_s **stores, int *count)
{
    conf_settings_s conf;
    conf_read_settings(&conf);

    char url[255];
    snprintf(url, 255,
             "https://api.sallinggroup.com/v2/stores?brand=bilka&fields=address%%2Cbrand%%2Ccoordinates%%2Cdistance_km%%2Cname%%2Cid&geo=%.4f%%2C%.4f&page=1&per_page=100&radius=%.2f",
             conf.address_lat, conf.address_lon, (double)conf.max_distance / 1000);

    char *raw_stores = NULL;
    fetch_status_e status = fetch_get(url, FETCH_AUTH_BEARER, SALLING_TOKEN, &raw_stores);

    if (status != FETCH_STATUS_SUCCESS)
    {
        if (status != FETCH_STATUS_CURL_ERROR)
            free(raw_stores);
        return status;
    }

    store_s *parsed_stores = NULL;
    int new_count = parse_salling_stores(raw_stores, &parsed_stores);

    *stores = realloc(*stores, (*count + new_count) * sizeof(store_s));
    memcpy(&((*stores)[(*count)]), parsed_stores, new_count * sizeof(store_s));

    *count += new_count;
    free(raw_stores);

    return FETCH_STATUS_SUCCESS;
}

int fetch_get_stores(store_s **stores)
{
    return _fetch_read_stores(stores);
}

int _fetch_read_stores(store_s **stores)
{
    char format_string[100];
    char s_f[7];
    snprintf(s_f, 7, "%%[^%c\n]", FILE_STORES_SEPERATOR);
    snprintf(format_string, 100, "%s%c%s%c%s%c%%lf%c%%lf%c%%d%c%%d%c%%d\n",
             s_f, FILE_STORES_SEPERATOR,
             s_f, FILE_STORES_SEPERATOR,
             s_f, FILE_STORES_SEPERATOR,
             FILE_STORES_SEPERATOR,
             FILE_STORES_SEPERATOR,
             FILE_STORES_SEPERATOR,
             FILE_STORES_SEPERATOR);

    FILE *file = fopen(FILE_STORES, "r");
    if (file == NULL)
    {
        printf("Someting went wrong while reading nearby stores. Try adjusting your address\n");
        return 0;
    }

    store_s store;
    store.total_price = 0;
    store.items_count = 0;
    store.missing_items_count = 0;
    store.found_items_count = 0;
    store.items = NULL;
    store.missing_items = NULL;
    store.found_items = NULL;

    int count = 0;
    while (EOF != fscanf(file, format_string,
                         &store.uid,
                         &store.name,
                         &store.address,
                         &store.lat,
                         &store.lon,
                         &store.group,
                         &store.chain,
                         &store.distance))
    {
        // TODO: Find out why it fails if items in response is empty, which it is for all uid < 10k
        // These are already filtered out in the parse function when fetching, so this is just a temporary solution, while some users data might be stale
        if (strlen(store.uid) < 5)
            continue;

        *stores = realloc(*stores, (++count) * sizeof(store_s));
        (*stores)[count - 1] = store;
    }

    fclose(file);
    return count;
}

void _fetch_write_stores(store_s *stores, int count)
{
    FILE *file = fopen(FILE_STORES, "w");

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s%c%s%c%s%c%f%c%f%c%d%c%d%c%d\n",
                stores[i].uid, FILE_STORES_SEPERATOR,
                stores[i].name, FILE_STORES_SEPERATOR,
                stores[i].address, FILE_STORES_SEPERATOR,
                stores[i].lat, FILE_STORES_SEPERATOR,
                stores[i].lon, FILE_STORES_SEPERATOR,
                stores[i].group, FILE_STORES_SEPERATOR,
                stores[i].chain, FILE_STORES_SEPERATOR,
                stores[i].distance);
    }

    fclose(file);
}

void fetch_print_stores(store_s *stores, int count)
{
    for (int i = 0; i < count; i++)
        fetch_print_store(&stores[i]);
}

void fetch_print_store(store_s *store)
{
    printf("\n%s\n%s\n%s\n%f\n%f\n%d\n%d\n%d\n",
           store->uid,
           store->name,
           store->address,
           store->lat,
           store->lon,
           store->group,
           store->chain,
           store->distance);
}

void fetch_get_coop_items(store_s *store, int *coop_count)
{
    // Populate nx_json. If everything fails return zero items
    const nx_json *json = fetch_get_cached_coop_items(store->uid);
    if (json == NULL)
    {
        if ((*coop_count < 9)) {
            fetch_status_e status = fetch_renew_coop_items(store->uid, &json);
            (*coop_count)++;
            if (status == FETCH_STATUS_SUCCESS && json != NULL)
                return fetch_get_coop_items(store, coop_count); // Successful fetch, data was cached. Last minute weird errors are occurring when continuing with *json, but never when reading cache. Posibly memory things above my head.
        } else {
            printf("Max COOP stores requested, using cached data if available.\n");
        }
        store->items_count = 0;
        store->items = NULL;
        return;
    }

    store->items_count = parse_coop_items(json, &store->items);
}

fetch_status_e fetch_renew_coop_items(char *store_id, const nx_json **json)
{
    char url[100];
    snprintf(url, 100, "https://api.cl.coop.dk/productapi/v1/product/%s", store_id);

    char *response = NULL;
    fetch_status_e status = fetch_get(url, FETCH_AUTH_OCP_APIM, COOP_TOKEN, &response);

    if (status != FETCH_STATUS_SUCCESS)
    {
        if (status != FETCH_STATUS_CURL_ERROR) // Memory was likely allocated
            free(response);
        printf("Coop failed");
        return status;
    }

    _fetch_write_coop_items(store_id, response);
    *json = nx_json_parse_utf8(response);
    free(response);
    return FETCH_STATUS_SUCCESS;
}

const nx_json *fetch_get_cached_coop_items(char *store_id)
{
    char *content = NULL;
    int s = _fetch_read_coop_items(store_id, &content);
    if (s != 0)
    {
        return NULL;
    }

    const nx_json *json = nx_json_parse_utf8(content);
    time_t now = time(NULL);
    time_t cached = nx_json_get(json, COOP_FIELD_TIME)->num.u_value;
    if (now > cached + TIME_SECONDS_IN_WEEK) // Renew once a week to limit API calls
        return NULL;
    return nx_json_get(json, COOP_FIELD_ITEMS);
}

char *_fetch_get_coop_file_name(char *store_id)
{
    int length = strlen(store_id) + strlen(FILE_ITEMS_SUFFIX) + 1;
    char *file_name = malloc(length);
    snprintf(file_name, length, "%s%s", store_id, FILE_ITEMS_SUFFIX);
    return file_name;
}

int _fetch_read_coop_items(char *store_id, char **content)
{
    char *name = _fetch_get_coop_file_name(store_id);
    int s = parse_read_file_to_end(name, content);
    free(name);
    return s;
}

void _fetch_write_coop_items(char *store_id, char *content)
{
    char *name = _fetch_get_coop_file_name(store_id);
    FILE *fp = fopen(name, "w");
    fprintf(fp, "{\"%s\":%ld,\"%s\":%s}", COOP_FIELD_TIME, time(NULL), COOP_FIELD_ITEMS, content);
    fclose(fp);
    free(name);
}

#define MOCK_BASKET_SIZE 10
basket_item_s *__fetch_mock_basket()
{
    basket_item_s *items = malloc(MOCK_BASKET_SIZE * sizeof(basket_item_s));

    strncpy(items[0].name, "Sk??vl", ITEM_NAME_SIZE);
    items[0].size = 1;
    items[0].unit = UNKNOWN;

    strncpy(items[1].name, "Spade", ITEM_NAME_SIZE);
    items[1].size = 1;
    items[1].unit = UNKNOWN;

    strncpy(items[2].name, "Spegep??lse", ITEM_NAME_SIZE);
    items[2].size = 1;
    items[2].unit = UNKNOWN;

    strncpy(items[3].name, "Sild", ITEM_NAME_SIZE);
    items[3].size = 1;
    items[3].unit = UNKNOWN;

    strncpy(items[4].name, "Br??d", ITEM_NAME_SIZE);
    items[4].size = 1;
    items[4].unit = UNKNOWN;

    strncpy(items[5].name, "Lys", ITEM_NAME_SIZE);
    items[5].size = 1;
    items[5].unit = UNKNOWN;

    strncpy(items[6].name, "Mad", ITEM_NAME_SIZE);
    items[6].size = 1;
    items[6].unit = UNKNOWN;

    strncpy(items[7].name, "??g", ITEM_NAME_SIZE);
    items[7].size = 1;
    items[7].unit = UNKNOWN;

    strncpy(items[8].name, "M??lk", ITEM_NAME_SIZE);
    items[8].size = 1;
    items[8].unit = UNKNOWN;

    strncpy(items[9].name, "Redskab", ITEM_NAME_SIZE);
    items[9].size = 1;
    items[9].unit = UNKNOWN;

    return items;
}

void fetch_get_salling_items(store_s *store)
{
    basket_s *basket_linked_list = basket_read();
    basket_item_s *basket;
    int basket_size = basket_to_array(basket_linked_list, &basket);

    int count = 0;
    store_item_s *items = NULL;

    // Call is made for each store without cache, even though items are the same for every salling store
    // because we want the code to be modifiable if salling adds items from more than bilka to their api
    for (int i = 0; i < basket_size; i++)
    {
        char url[255];
        snprintf(url, 255, "https://api.sallinggroup.com/v1-beta/product-suggestions/relevant-products?query=%s", basket[i].name);

        char *raw_items = NULL;
        fetch_status_e status = fetch_get(url, FETCH_AUTH_BEARER, SALLING_TOKEN, &raw_items);

        if (status != FETCH_STATUS_SUCCESS)
        {
            if (status != FETCH_STATUS_CURL_ERROR)
                free(raw_items);
            continue;
        }

        if (raw_items == NULL)
        {
            return;
        }
        store_item_s *temp_items = NULL;
        int temp_count = parse_salling_items(raw_items, &temp_items);
        free(raw_items);
        items = realloc(items, (count + temp_count) * sizeof(store_item_s));
        memcpy(&(items[count]), temp_items, temp_count * sizeof(store_item_s));
        count += temp_count;
    }

    store->items = items;
    store->items_count = count;
}

fetch_status_e fetch_coordinates(char* input_address, char** raw_coordinates) {
    char* url_start = "https://maps.googleapis.com/maps/api/geocode/json?address=";
    char* token = "&key=AIzaSyCwIirwXs-zd2_TZU6uLll6BOHdaIQVDeM";

    int address_len = strlen(input_address); // Get length of the address

    // Assume every character is 2 bytes so it works with '??', '??' and '??'
    char* address = malloc(2 * address_len * (char)sizeof(int));

    // If no memory available print error and exit
    if (address == NULL) {
        perror("Exit");
        exit(EXIT_FAILURE);
    }

    strncpy(address, input_address, address_len);

    int url_len = strlen(url_start);
    int token_len = strlen(token);

    // Loop over address and checks if there is a space. A space needs to be replaced with "%20" for the url to work
    for (int i = 0; i < address_len; ++i) {
        if (address[i] == ' ') {

            address_len += 2; // Address need to grow with two spaces for the '2' and the '0'. '%' replaces ' '.

            // Using tmp variable so we dont get a memory leak if there is not enough room to reallocate
            char* tmp = realloc(address, 2 * address_len * sizeof(char));

            if (tmp == NULL) { // If not enough memory
                perror("Error");

                free(address);
                address = NULL; // No dangling pointer

                exit(EXIT_FAILURE);
            }
            else if (address != tmp) {
                address = tmp;
            }

            tmp = NULL;

            // Moves the memory so there is space for "20"
            memmove(address + i + 3, address + i + 1, address_len - i - 1);

            // Replace ' ' with '%'
            address[i] = '%';

            // Fills in the "20" on empty spaces
            address[i + 1] = '2';
            address[i + 2] = '0';
        }
    }

    int len = address_len + url_len + token_len; // Total len of the sub-strings

    char url[len];

    strncpy(url, url_start, len); // Copy first part since we dont want to append it to the empty string
    strncat(url, address, len); // Concat the two other parts
    strncat(url, token, len);

    free(address);
    address = NULL;

    fetch_status_e status = fetch_get_no_auth(url, raw_coordinates); // Call api

    return status;
}
