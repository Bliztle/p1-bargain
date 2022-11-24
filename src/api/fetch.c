#include "fetch.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/**
 * Sends a get-request to the selected endpoint, modifying *result to contain the response-data.
 *
 * @details Doesn't follow the originally agreed upon specs, as those didn't include returning error codes to the caller.
 *
 * @author bliztle <asbjoern.r.e@gmail.com>
 * @param url Url to the endpoint. This is assumed to be a full URL.
 * @param token Optional bearer token used for authorization
 * @param result Pointer to the resulting string when fetch_get is done
 *
 * @returns a status code indicating the success state
 */
fetch_status_e fetch_get(char *url, char *token, char *result)
{
    char auth_header[60];
    snprintf(auth_header, 60, "Authorization: Bearer %s", token);

    CURL *curl = curl_easy_init();
    if (!curl)
        return FETCH_STATUS_CURL_ERROR;

    CURLcode res_code;
    fetch_response_s res = {result, strlen(result)};

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
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

    return FETCH_STATUS_SUCCESS;
}

// TODO: Remove this when proper functions have been implemented
/* Make a sample call to salling's "relevant products" api for "hakkekod"*/
int fetch_salling_test()
{
    char *token = "8fc39b17-cd57-40db-b9d8-85c7c3756589", *vendor = "salling";
    char auth_header[60];
    snprintf(auth_header, 60, "Authorization: Bearer %s", token);

    CURL *curl = curl_easy_init();
    if (!curl)
        return 1;

    CURLcode res_code;
    fetch_response_s res = {NULL, 0};

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.sallinggroup.com/v1-beta/product-suggestions/relevant-products?query=hakkekod");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fetch_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&res);

    res_code = curl_easy_perform(curl);

    // Check for errors
    if (res_code != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res_code));

    // Cleanup and print
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    printf("\n\nResponse:\n%s\n", res.response);

    salling_relevant_products *products = parse_salling_relevant_products(res.response);
    print_salling_relevant_products(products);
}

/**
 * Callback function for curl to write output to.
 * collects data in chunks from buffer and writes to the prelimenary response
 */
size_t fetch_write_callback(char *buffer, size_t size, size_t buffer_length, void *prelim_response)
{
    size_t realsize = size * buffer_length;
    fetch_response_s *mem = (fetch_response_s *)prelim_response;
    printf("[response length]: %zu", realsize + mem->size);

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

int fetch_get_stores(store_s *stores)
{

    int count = 0;
    count = fetch_get_coop_stores(stores, count);
    count = fetch_get_salling_stores(stores, count);
}

int fetch_get_coop_stores(store_s *stores, int count)
{
    // TODO: Read from config
    const double lat = 57.025760, lon = 9.958440;
    const int distance = 5000;
    const char *token = "ed1b7976-ca43-4bde-a930-ba3d92935464";

    return 0;
}

int fetch_get_salling_stores(store_s *stores, int count)
{
    return 0;
}