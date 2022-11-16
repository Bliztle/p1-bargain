#include "fetch.h"
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

int main(int argc, char **args)
{
    fetch_salling_test();
    return 0;
}

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

/* Make a sample call to salling's "relevant products" api for "hakkekod"*/
int fetch_salling_test()
{

    char *token = "8fc39b17-cd57-40db-b9d8-85c7c3756589", *vendor = "salling";
    char auth_header[60];
    snprintf(auth_header, 60, "Authorization: Bearer %s", token);

    CURL *curl = curl_easy_init();
    if (!curl)
        return 1;

    CURLcode res;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.sallinggroup.com/v1-beta/product-suggestions/relevant-products?query=hakkekod");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

    // Cleanup and print
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    printf("\n");
}

int get_token(char vendor[11], char token[37])
{
    char current_vendor[10];

    FILE *token_file = fopen("./tokens.conf", "r");

    do
    {
        int count = fscanf(token_file, "%s %s", vendor, token);
    } while (0);

    fclose(token_file);
}