#include <assert.h>
#include "settings.h"

int main(void) {
    // Path test depends on the system it is tested on

    char* path = "shopping/";
    char* address = "Selma Lagerløfs Vej 300 Aalborg 9220";
    char* distance = "10";
    char* deviation = "0.2";

    assert(settings_validate(path, PATH));
    assert(settings_validate(address, ADDRESS));
    assert(settings_validate(distance, DISTANCE));
    assert(settings_validate(deviation, DEVIATION));

    return 0;
}