#include "fetch.h"
#include "parse.h"
#include "../calc.h"
#include "../items_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

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

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fetch_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&res);

    res_code = curl_easy_perform(curl);

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

    // Cleanup
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

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

    // Prepend response
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), buffer, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
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
    // TODO: Read from config
    double lat = 57.025760, lon = 9.958440;
    int distance = 6000;
    char *token = "8042a78a1c91463e80140b0cb11b8b47";

    // char url[255];
    // snprintf(url, 255,
    //          "https://api.cl.coop.dk/storeapi/v1/stores?page=1&size=5000",
    //          distance, lat, lon);

    char *url = "https://api.cl.coop.dk/storeapi/v1/stores?page=1&size=5000";

    // char url[255];
    // snprintf(url, 255,
    //          "https://api.cl.coop.dk/storeapi/v1/stores/find/radius/%d?latitude=%f&longitude=%f",
    //          distance, lat, lon);

    char *raw_stores = NULL;
    fetch_status_e status = fetch_get(url, FETCH_AUTH_OCP_APIM, token, &raw_stores);

    if (status != FETCH_STATUS_SUCCESS)
        return status;

    store_s *parsed_stores = NULL;
    int new_count = parse_coop_stores(raw_stores, &parsed_stores);

    for (int i = 0; i < new_count; i++)
    {
        if (calc_coordinate_distance(lat, lon, parsed_stores[i].lat, parsed_stores[i].lon) * 1000 < distance)
        // if (1)
        {
            *stores = realloc(*stores, (++(*count)) * sizeof(store_s));
            (*stores)[*count - 1] = parsed_stores[i];
        }
    }

    // *stores = realloc(*stores, (*count + new_count) * sizeof(store_s));
    // memcpy(&(*stores[(*count)]), parsed_stores, new_count * sizeof(store_s));

    // *count += new_count;
    free(raw_stores);

    return FETCH_STATUS_SUCCESS;
}

fetch_status_e fetch_renew_salling_stores(store_s **stores, int *count)
{
    // TODO: Read from config
    double lat = 57.025760, lon = 9.958440;
    int distance = 6000; // Closest store is 6 km away
    char *token = "ed1b7976-ca43-4bde-a930-ba3d92935464";

    char url[255];
    snprintf(url, 255,
             "https://api.sallinggroup.com/v2/stores?brand=bilka&fields=address%%2Cbrand%%2Ccoordinates%%2Cdistance_km%%2Cname%%2Cid&geo=%.4f%%2C%.4f&page=1&per_page=100&radius=%.2f",
             lat, lon, (double)distance / 1000);

    char *raw_stores = NULL;
    fetch_status_e status = fetch_get(url, FETCH_AUTH_BEARER, token, &raw_stores);

    if (status != FETCH_STATUS_SUCCESS)
        return status;

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

fetch_status_e fetch_coordinates(char* input_address, char** raw_coordinates) {
    char* url_start = "https://maps.googleapis.com/maps/api/geocode/json?address=";
    char* token = "&key=AIzaSyCwIirwXs-zd2_TZU6uLll6BOHdaIQVDeM";

    int address_len = strlen(input_address);
    char* address = malloc(address_len * sizeof(int));

    if (address == NULL) {
        perror("Exit");
        exit(EXIT_FAILURE);
    }

    strncpy(address, input_address, address_len);

    int url_len = strlen(url_start);
    int token_len = strlen(token);

    for (int i = 0; i < address_len; ++i) {
        if (address[i] == ' ') {    

            address_len += 2;

            char* tmp = realloc(address, address_len * sizeof(int));

            if (tmp == NULL) {
                perror("Error");

                free(address);
                address = NULL;

                exit(EXIT_FAILURE);
            }
            else if (address != tmp) {
                address = tmp;
            }

            tmp = NULL;

            memmove(address + i + 3, address + i + 1, address_len - i - 1);

            address[i] = '%';
            address[i + 1] = '2';
            address[i + 2] = '0';
        }
    }

    int len = address_len + url_len + token_len;

    char url[len + 1];

    strncpy(url, url_start, len);
    strncat(url, address, len);
    strncat(url, token, len);

    free(address);
    address = NULL;

    fetch_status_e status = fetch_get_no_auth(url, raw_coordinates);

    if (status != FETCH_STATUS_SUCCESS) {
        return status;
    }
}