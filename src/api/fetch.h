#ifndef P1_BARGAIN_FETCH_H
#define P1_BARGAIN_FETCH_H

#include <stddef.h>
#include "../items_types.h"
#include "nxjson/nxjson.h"

#define FILE_STORES "stores.txt"
#define FILE_STORES_SEPERATOR '|'
#define FILE_ITEMS_SUFFIX "-coop-items.txt"
#define FILE_ITEMS_SEPERATOR '|'

#define COOP_FIELD_TIME "cache_time"
#define COOP_FIELD_ITEMS "items"
#define TIME_SECONDS_IN_WEEK 60 * 60 * 24 * 7

typedef enum
{
    FETCH_STATUS_SUCCESS,
    FETCH_STATUS_CURL_ERROR,
    FETCH_STATUS_UNAUTHORIZED,
    FETCH_STATUS_UNKNOWN_ERROR
} fetch_status_e;

typedef enum
{
    FETCH_AUTH_BEARER,
    FETCH_AUTH_OCP_APIM
} fetch_auth_e;

typedef struct
{
    char *response;
    size_t size;
} fetch_response_s;

/**
 * Sends a get-request to the selected endpoint, modifying *result to contain the response-data.
 *
 * @details Doesn't follow the originally agreed upon specs, as those didn't include returning error codes to the caller.
 *
 * @author bliztle <asbjoern.r.e@gmail.com>
 * @param url Url to the endpoint. This is assumed to be a full URL.
 * @param token_type type of auth token
 * @param token Optional bearer token used for authorization
 * @param result Pointer to the resulting string when fetch_get is done
 *
 * @returns a status code indicating the success state
 */
fetch_status_e fetch_get(char *url, fetch_auth_e token_type, char *token, char **result);

/**
 * Sends a get-request to the selected endpoint, modifying *result to contain the response-data.
 *
 * @details Doesn't follow the originally agreed upon specs, as those didn't include returning error codes to the caller.
 *
 * @author bliztle <asbjoern.r.e@gmail.com>
 * @param url Url to the endpoint. This is assumed to be a full URL.
 * @param result Pointer to the resulting string when fetch_get is done
 *
 * @returns a status code indicating the success state
 */
fetch_status_e fetch_get_no_auth(char *url, char **result);

basket_item_s *__fetch_mock_basket();

/**
 * Callback function for curl to write output to.
 * collects data in chunks from buffer and writes to the prelimenary response
 */
size_t fetch_write_callback(char *buffer, size_t size, size_t buffer_length, void *prelim_response);

char *encode_danish(char *url);

/**
 * Renews all stores and prints them to stores.txt
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @returns success status for renewal
 */
fetch_status_e fetch_renew_stores();

/**
 * Gets all coop stores within the radius from settings, and appends it to the stores array
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param stores pointer to an array of stores. Modifies to realloc with new stores
 * @param count pointer to the length of the stores array. Modifies to update count
 *
 * @returns a success state
 */
fetch_status_e fetch_renew_coop_stores(store_s **stores, int *count);

/**
 * Gets all salling stores within the radius from settings, and appends it to the stores array
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param stores pointer to an array of stores. Modifies to realloc with new stores
 * @param count pointer to the length of the stores array. Modifies to update count
 *
 * @returns a success state
 */
fetch_status_e fetch_renew_salling_stores(store_s **stores, int *count);

/**
 * Gets the array of relevant stores
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param stores pointer to an array of stores, to write the stores to
 *
 * @returns amount of stores written
 */
int fetch_get_stores(store_s **stores);

/**
 * Reads stores from stores.txt
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param stores pointer to an array of stores, to write the stores to
 *
 * @returns amount of stores written
 */
int _fetch_read_stores(store_s **stores);

/**
 * Writes an array of stores to stores.txt
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param stores array of stores
 * @param count length of array
 */
void _fetch_write_stores(store_s *stores, int count);

/**
 * Prints stores to stdout
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param stores array of stores to print
 * @param count length of stores array
 */
void fetch_print_store(store_s *store);

/**
 * Prints a store to stdout
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param stores pointer to the store to print
 */
void fetch_print_stores(store_s *stores, int count);

void fetch_get_coop_items(store_s *store);

fetch_status_e fetch_renew_coop_items(char *store_id, const nx_json **json);

const nx_json *fetch_get_cached_coop_items(char *store_id);

char *_fetch_get_coop_file_name(char *store_id);

int _fetch_read_coop_items(char *store_id, char **content);

void _fetch_write_coop_items(char *store_id, char *content);

void fetch_get_salling_items(store_s *store);

/**
 * Get address coordinates from Google Maps
 * @param input_address the address that the user inputs
 * @param raw_coordinates the raw json as output parameter
 * @return success status
*/
fetch_status_e fetch_coordinates(char* input_address, char** raw_coordinates); 

#endif