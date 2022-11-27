#include <assert.h>
#include <stdio.h>
#include "fetch.h"

/**
 * Tests for fetch.c
 *
 * All parse.h functions are also tested as part of these functions (as of 2022-11-27)
 *
 * fetch_renew_salling_stores: fetch status is good
 * fetch_renew_stores: fetch status is good
 * fetch_read_stores: count is more than 0
 */
int main()
{
    int count = 0;
    store_s *stores = NULL;

    printf("Fetch renew tests\n");
    assert(fetch_renew_salling_stores(&stores, &count) == FETCH_STATUS_SUCCESS);
    //! Not run, as we have limited api calls
    // assert(fetch_renew_coop_stores(&stores, &count) == FETCH_STATUS_SUCCESS);
    // assert(fetch_renew_stores() == FETCH_STATUS_SUCCESS);

    printf("\nfetch read test\n");
    stores = NULL;
    int read_count = _fetch_read_stores(&stores);
    assert(read_count > 0);
    fetch_print_stores(stores, read_count);

    return 0;
}