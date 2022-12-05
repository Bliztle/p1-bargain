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
    new_settings.max_distance = 2000;
    strcpy(new_settings.address, "korruptionsgade 5 100 christiansborg");
    new_settings.deviance = 10;
    assert(conf_write_settings(&new_settings));


    conf_read_settings(&settings);
    assert(settings.max_distance == new_settings.max_distance);
    assert(strcmp(settings.address, new_settings.address) == 0);
    assert(settings.deviance == new_settings.deviance);
}