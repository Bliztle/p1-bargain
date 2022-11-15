#include <stdio.h>
#include "../libraries/curl-7.86.0/include/curl/curl.h"

void CurlTest();

int main()
{
    printf("Hello, World! - With a testottwo!\n");
    printf("\nAs if we would write any tests...\n");
    printf("\nI don't know what you're talking about\n");
    CurlTest();
    return 0;
}

void CurlTest()
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