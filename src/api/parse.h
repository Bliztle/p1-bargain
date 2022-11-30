#ifndef P1_BARGAIN_PARSE_H
#define P1_BARGAIN_PARSE_H

#include "../items_types.h"

/**
 * Parses a raw json string from Salling's store API to an array of store_s structs
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param raw_stores The raw json to parse to store_s objects
 * @param stores A pointer to a null pointer. This will be modified to point at the stores
 *
 * @returns how many stores were parsed
 */
int parse_salling_stores(char *raw_stores, store_s **stores);

/**
 * Parses a raw json string from COOP's store API to an array of store_s structs
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param raw_stores The raw json to parse to store_s objects
 * @param stores A pointer to a null pointer. This will be modified to point at the stores
 *
 * @returns how many stores were parsed
 */
int parse_coop_stores(char *raw_stores, store_s **stores);

#endif