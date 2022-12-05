//
// Testing the read and write capabilities to/from our .conf file
//
#include "config.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {

    conf_settings_s settings;
    assert(conf_read_settings(&settings));

    conf_settings_s new_settings;
    strcpy(new_settings.shopping_list_save_path, "stores.txt"); //Test by using stores.txt
    new_settings.max_distance = 2000;
    strcpy(new_settings.address, "korruptionsgade 5 100 christiansborg");
    new_settings.address_lat = 47.1915271;
    new_settings.address_lon = -52.8393801;
    new_settings.deviance = 0.25;
    assert(conf_write_settings(new_settings));

    conf_settings_s compare;
    conf_read_settings(&compare);

    assert(strcmp(compare.shopping_list_save_path, new_settings.shopping_list_save_path) == 0);
    assert(compare.max_distance == new_settings.max_distance);
    assert(strcmp(compare.address, new_settings.address) == 0);
    assert(compare.address_lat - new_settings.address_lat < 0.0001 && compare.address_lat - new_settings.address_lat > -0.0001);
    assert(compare.address_lon - new_settings.address_lon < 0.0001 && compare.address_lon - new_settings.address_lon > -0.0001);
    assert(compare.deviance == new_settings.deviance);

    conf_write_settings(settings);

    assert(conf_setup());
}