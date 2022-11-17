#include "fetch.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// int main(int argc, char **args)
// {
//     fetch_salling_test();
//     return 0;
// }

// TODO: Remove this when proper functions have been implemented
void fetch_example()
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
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
    fetch_response res = {NULL, 0};

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.sallinggroup.com/v1-beta/product-suggestions/relevant-products?query=hakkekod");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
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
size_t write_callback(char *buffer, size_t size, size_t buffer_length, void *prelim_response)
{
    size_t realsize = size * buffer_length;
    fetch_response *mem = (fetch_response *)prelim_response;
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