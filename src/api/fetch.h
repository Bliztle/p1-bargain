#include <stddef.h>

typedef enum
{
    FETCH_STATUS_SUCCESS,
    FETCH_STATUS_CURL_ERROR,
    FETCH_STATUS_UNAUTHORIZED,
    FETCH_STATUS_UNKNOWN_ERROR
} fetch_status_e;

typedef struct
{
    char *response;
    size_t size;
} fetch_response_s;

fetch_status_e fetch_get(char *url, char *token, char *result);
int fetch_salling_test();
size_t fetch_write_callback(char *buffer, size_t size, size_t buffer_length, void *prelim_response);

int fetch_get_stores(store_s *stores);
int fetch_get_coop_stores(store_s *stores, int count);
int fetch_get_salling_stores(store_s *stores, int count);