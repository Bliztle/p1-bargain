#include <assert.h>
#include <stdio.h>
#include "fetch.h"
#include "../items_types.h"
#include <string.h>
#include <stdlib.h>

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
    char *milk = "M---æ---ø---å---Æ---Ø---Å---lk";
    char *encoded_milk = encode_danish(milk);
    assert(strcmp(encoded_milk, "M---%C3%A6---%C3%B8---%C3%A5---%C3%86---%C3%98---%C3%85---lk") == 0);
    free(encoded_milk);

    int count = 0;
    store_s *stores = NULL;

    /****************** General tests ******************/
    printf("General fetch tests\n");

    char *result = NULL;
    fetch_status_e fetch_get_status = fetch_get_no_auth("https://example.com", &result); // fetch_get_no_auth is basically aliassing fetch_get
    assert(fetch_get_status == FETCH_STATUS_SUCCESS);

    /****************** Store tests ******************/
    printf("Fetch renew tests\n");

    fetch_status_e fetch_renew_salling_stores_status = fetch_renew_salling_stores(&stores, &count);
    assert(fetch_renew_salling_stores_status == FETCH_STATUS_SUCCESS);

    //! Not run, as we have limited api calls
    // assert(fetch_renew_coop_stores(&stores, &count) == FETCH_STATUS_SUCCESS);
    assert(fetch_renew_stores() == FETCH_STATUS_SUCCESS);

    printf("\nfetch read test\n");
    stores = NULL;
    int lines = 0, read_count = _fetch_read_stores(&stores);

    // Count lines in stores file
    FILE *fp = fopen(FILE_STORES, "r");
    if (fp != NULL) // Tests cannot fail if file is null, as they are null if fetch_renew_stores() test is never run
        while (!feof(fp))
            if (fgetc(fp) == '\n')
                lines++;

    assert(read_count == lines || read_count == lines - 1);
    fetch_print_stores(stores, read_count);

    /****************** Items tests ******************/
    printf("\nfetch items test\n");
    //? Not sure how i should test these functions

    //! Not run, as we have limited api calls
    //! Git does not cache like us when testing
    // printf("\nCOOP\n");
    // store_s coop_store_1290;

    // strcpy(coop_store_1290.uid, "24216");
    // fetch_get_coop_items(&coop_store_1290);
    // for (int i = 0; i < coop_store_1290.items_count; i++)
    // {
    //     printf("[%d]\nName: %s\nPrice: %f\nSize: %f\nUnit: %i\nPPU: %f\n\n", i,
    //            coop_store_1290.items[i].name,
    //            coop_store_1290.items[i].price,
    //            coop_store_1290.items[i].size,
    //            coop_store_1290.items[i].unit,
    //            coop_store_1290.items[i].price_per_unit);
    // }

    // printf("\nSALLING\n");
    // store_s salling_store;

    // fetch_get_salling_items(&salling_store);
    // for (int i = 0; i < salling_store.items_count; i++)
    // {
    //     printf("[%d]\nName: %s\nPrice: %f\nSize: %f\nUnit: %i\nPPU: %f\n\n", i,
    //            salling_store.items[i].name,
    //            salling_store.items[i].price,
    //            salling_store.items[i].size,
    //            salling_store.items[i].unit,
    //            salling_store.items[i].price_per_unit);
    // }
    return 0;
}