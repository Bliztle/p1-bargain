//
// Testing the read and write capabilities to/from our .conf file
//
#include "config.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {

    conf_settings_s old_settings;
    assert(conf_read_settings(&old_settings));

    conf_settings_s cmp_settings;
    conf_settings_s new_settings;
    strcpy(new_settings.shopping_list_save_path, "shopping/");
    strcpy(new_settings.address, "Selma Lagerløfs Vej 300");
    new_settings.max_distance = 2000;
    new_settings.deviance = 10;
    assert(conf_write_settings(&new_settings));

    conf_read_settings(&cmp_settings);

    assert(strcmp(cmp_settings.shopping_list_save_path, new_settings.shopping_list_save_path) == 0);
    assert(cmp_settings.max_distance == new_settings.max_distance);
    assert(strcmp(cmp_settings.address, new_settings.address) == 0);
    assert(cmp_settings.deviance == new_settings.deviance);

    conf_write_settings(&old_settings);
}