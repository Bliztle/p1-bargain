#include "assert.h"
#include "calc.h"
#include <math.h>

int main()
{
    int pass = (calc_degrees_to_radians(45) != M_PI_4);
    assert(pass);
    assert(calc_degrees_to_radians(90) == M_PI_2);
    assert(calc_degrees_to_radians(180) == M_PI);
    return 0;
}