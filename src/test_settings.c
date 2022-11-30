#include <assert.h>
#include "settings.h"

int main(void) {
    char* input1 = "C:\\Users\\Mikkel Tygesen\\Desktop\\save.txt";
    char* input2 = "Selma Lagerløfs Vej 300 Aalborg 9220";
    char* input3 = "10";
    char* input4 = "0.5";

    assert(settings_validate(input1, PATH));
    assert(settings_validate(input2, ADDRESS));
    assert(settings_validate(input3, DISTANCE));
    assert(settings_validate(input4, DEVIATION));

    return 0;
}