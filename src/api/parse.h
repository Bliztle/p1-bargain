#ifndef P1_BARGAIN_PARSE_H
#define P1_BARGAIN_PARSE_H

#include "../config.h"
#include "../items_types.h"
#include "nxjson/nxjson.h"

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

/**
 * Parses a json object from COOP's product API to an array of store_item_s structs
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param json Object containing api response parsed to json
 * @param stores A pointer to a null pointer. This will be modified to point at the items
 *
 * @returns how many items were parsed
 */
int parse_coop_items(const nx_json *json, store_item_s **items);

/**
 * Parses a json object from Salling's suggestions API to an array of store_item_s structs
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param json Object containing api response parsed to json
 * @param stores A pointer to a null pointer. This will be modified to point at the items
 *
 * @returns how many items were parsed
 */
int parse_salling_items(char* raw_items, store_item_s **items);

void parse_populate_item_unit(store_item_s *item);
char *parse_try_regex_group(char* source, char* regex);
double parse_try_extract_size(char *source, char *unit_str);
char *parse_replace_char(char *source, char find, char replace);

/**
 * Writes the contens of a file to the content argumment
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param file_name name / path of the file
 * @param content pointer to the string to write file content to
 *
 * @returns 0 on success, 1 on error
 */
int parse_read_file_to_end(char *file_name, char **content);

/**
* Parses coordinates from Google API into latitude and longtitude 
*
* @param lat pointer to latitude
* @param lon pointer longitude
* @param raw_coordinates Raw json to parse into latitude and longtitude
*
* @return returns true or false
*/
int parse_coordinates(double *lat, double *lon, char *raw_coordinates);

#endif